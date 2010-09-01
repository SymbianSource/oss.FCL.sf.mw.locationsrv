/*
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  The implementation of Launch Manager class of Location 
*                Settings UI Server
*
*/


#include "locsettingsuilaunchmgr.h"
#include "locsettingsuilaunchao.h"

const TUid KSettingsUiMgrUid = TUid::Uid(0x101F84EF);

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CLocSettingsUILaunchMgr::Instance()
// ---------------------------------------------------------------------------
//
CLocSettingsUILaunchMgr* CLocSettingsUILaunchMgr::InstanceL()
    {
    CLocSettingsUILaunchMgr* self = static_cast<CLocSettingsUILaunchMgr*>
                                        (CCoeEnv::Static(KSettingsUiMgrUid));
    if (!self)
        {
        self = new (ELeave) CLocSettingsUILaunchMgr;
        
        CleanupStack::PushL( self );
        self->ConstructL();
        CleanupStack::Pop( self );
        }
    return self;
    }
    

// ---------------------------------------------------------------------------
// CLocSettingsUILaunchMgr::EnqueueRequestL( const RMessage2& aMessage )
// ---------------------------------------------------------------------------
//
void CLocSettingsUILaunchMgr::EnqueueRequestL( 
    const RMessage2& aMessage )
    {
    // Ownership of aMessage and aParam transferred to CLocSettingsUIInfo 
    // class.
    CLocSettingsUIInfo* reqInfo = CLocSettingsUIInfo::NewL(aMessage);
    // Passing the Ownership of the reqInfo to the CLocSettingsUISrvLaunchAO
    iLauncher->EnqueueRequest(reqInfo);
    }


// ---------------------------------------------------------------------------
// CLocSettingsUILaunchMgr::CancelRequest( const CSession2* aSession )
// ---------------------------------------------------------------------------
//
void CLocSettingsUILaunchMgr::CancelRequest( const CSession2* aSession )
    {
    iLauncher->CancelRequest(aSession);    
    }


// ---------------------------------------------------------------------------
// CLocSettingsUILaunchMgr::CloseRunningSettingsUi(TInt aErrorCode)
// ---------------------------------------------------------------------------
//
void CLocSettingsUILaunchMgr::CloseRunningSettingsUi(TInt aErrorCode)
    {
    iLauncher->LocationUIDismissed(aErrorCode);
    }


// ---------------------------------------------------------------------------
// CLocSettingsUILaunchMgr::CLocSettingsUILaunchMgr()
// ---------------------------------------------------------------------------
//
CLocSettingsUILaunchMgr::CLocSettingsUILaunchMgr()
    :CCoeStatic(KSettingsUiMgrUid)
    {
    }


// ---------------------------------------------------------------------------
// CLocSettingsUILaunchMgr::~CLocSettingsUILaunchMgr()
// ---------------------------------------------------------------------------
//
CLocSettingsUILaunchMgr::~CLocSettingsUILaunchMgr()
    {
    delete iLauncher;
    }


// ---------------------------------------------------------------------------
// CLocSettingsUILaunchMgr::ConstructL()
// ---------------------------------------------------------------------------
//
void CLocSettingsUILaunchMgr::ConstructL()
    {
    iLauncher = CLocSettingsUISrvLaunchAO::NewL();
    }


// End of file
