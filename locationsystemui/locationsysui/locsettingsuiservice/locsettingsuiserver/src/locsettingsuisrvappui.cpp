/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  The implementation for AppUi class of Settings UI Server
*
*/


#include <w32std.h>
#include <apgwgnam.h>
#include <akntitle.h>
#include <eikdoc.h>
#include <eikapp.h>
#include <bldvariant.hrh>

#include "locsettingsuisrvappui.h"
#include "locsettingsuilaunchmgr.h"

const TInt KWsArrayGranularity = 4;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CLocSettingsUISrvAppUi::CLocSettingsUISrvAppUi()
// ---------------------------------------------------------------------------
//
CLocSettingsUISrvAppUi::CLocSettingsUISrvAppUi()
    {
    }


// ---------------------------------------------------------------------------
// CLocSettingsUISrvAppUi::CLocSettingsUISrvAppUi()
// ---------------------------------------------------------------------------
//
CLocSettingsUISrvAppUi::~CLocSettingsUISrvAppUi()
    {
    }


// ---------------------------------------------------------------------------
// CLocSettingsUISrvAppUi::CLocSettingsUISrvAppUi()
// ---------------------------------------------------------------------------
//
void CLocSettingsUISrvAppUi::ConstructL()
    {
    BaseConstructL( EAknEnableSkin | EAknEnableMSK | EAknSingleClickCompatible);

    // The Caption text for Location Settings UI Server should be that of the
    // application which is currently in the background. Hence, loop through
    // all the background applications and obtain the Caption from the 
    // application at the top of the stack
    RWsSession& wsSession=iEikonEnv->WsSession();
    TInt count=wsSession.NumWindowGroups( 0 );

    RArray<RWsSession::TWindowGroupChainInfo>* allWgIds = new (ELeave)
                        RArray<RWsSession::TWindowGroupChainInfo>( KWsArrayGranularity );
    
    CleanupDeletePushL( allWgIds );
    CleanupClosePushL( *allWgIds );
    
    User::LeaveIfError( wsSession.WindowGroupList( 0, allWgIds) );
    
    count = allWgIds->Count();
    for (TInt index=0; index<count; index++)
        {
        RWsSession::TWindowGroupChainInfo& info = (*allWgIds)[index];
        if ( info.iParentId <= 0 )
            {
            CApaWindowGroupName* windowName = 
                    CApaWindowGroupName::NewLC(wsSession, info.iId);
                    
            if ( !windowName->Hidden() && 
                 Document()->Application()->AppDllUid() != windowName->AppUid())
                    {
                    // Got the first non-hidden app. 
                    // This should be the background application.
                    
                    CAknTitlePane* title = static_cast<CAknTitlePane*>( 
                    StatusPane()->ControlL( TUid::Uid( EEikStatusPaneUidTitle )));
    
                    HBufC* buf = HBufC::NewL(windowName->Caption().Length());
                    buf->Des().Copy(windowName->Caption());
                    title->SetText( buf ); // Takes ownership of buf
                    CleanupStack::PopAndDestroy( windowName );
                    break;
                    }
            CleanupStack::PopAndDestroy( windowName );
            }
        }
    CleanupStack::PopAndDestroy( 2, allWgIds ); // destroy allwgIds
    }

// ---------------------------------------------------------------------------
// CLocSettingsUISrvAppUi::SetFullScreenState( TBool aFullScreenState )
// ---------------------------------------------------------------------------
//
void CLocSettingsUISrvAppUi::SetFullScreenState( TBool aFullScreenState )
    {
    SetFullScreenApp( aFullScreenState );
    }
    
// ---------------------------------------------------------------------------
// CLocSettingsUISrvAppUi::HandleCommandL(TInt aCommand)
// ---------------------------------------------------------------------------
//
void CLocSettingsUISrvAppUi::HandleCommandL(TInt aCommand)
    {
    switch(aCommand)
        {
        case EEikCmdExit:
        case EAknSoftkeyExit:
            {
            CLocSettingsUILaunchMgr* launchMgr = 
                                        CLocSettingsUILaunchMgr::InstanceL();
            if ( iEikonEnv->StartedAsServerApp() )
                {
                // In case of Embedded Application indicate that
                // server application is going to close after the
                // request is completed.
                launchMgr->CloseRunningSettingsUi(EEikCmdExit);
                // If it is an Embedded App then close the Server 
                // Application.
                Exit();
                }
            else
                {
                // In case of Normal launch behave as if the User
                // pressed back soft key and not exit.
                // So we will continue with the next request.
                launchMgr->CloseRunningSettingsUi(KErrNone);
                }
            break;
            }
        default:
            CEikAppUi::HandleCommandL(aCommand);
        }
    }


//End of file
