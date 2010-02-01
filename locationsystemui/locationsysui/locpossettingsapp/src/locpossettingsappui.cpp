/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Application to launch Positioning Settings for Location Centre
*
*/


// System Includes
#include <avkon.hrh>
#include <eikserverapp.h>

// User Includes
#include "locpossettingsappui.h"

// ======== Member functions for class CLosPosSettingsAppUi ==================


// ---------------------------------------------------------------------------
// void CLosPosSettingsAppUi::ConstructL
// ---------------------------------------------------------------------------
//
void CLosPosSettingsAppUi::ConstructL()
    {
    BaseConstructL(  EAknEnableSkin | EAknEnableMSK | EAknSingleClickCompatible);
    
    // Create the Postioning Settings View
    CLocSysUiView* view = CLocSysUiView::NewL();
    view->LaunchedfromPosSettingsApp();
    CleanupStack::PushL( view );
    AddViewL( view ); // transfer ownership to CAknViewAppUi
    CleanupStack::Pop( view );
    SetDefaultViewL( *view );
    }


// ---------------------------------------------------------------------------
// CLosPosSettingsAppUi::~CLosPosSettingsAppUi
// ---------------------------------------------------------------------------
//
CLosPosSettingsAppUi::~CLosPosSettingsAppUi()
    {
    }

// ---------------------------------------------------------------------------
// void CLosPosSettingsAppUi::HandleCommandL
// ---------------------------------------------------------------------------
//   
void CLosPosSettingsAppUi::HandleCommandL( TInt aCommand )
    {
    switch ( aCommand )
        {		
        case EAknSoftkeyExit: 
        	{
        	// The User has pressed the Back key from the Application or the
        	// Exit key using the Right soft key. In this case the parent
        	// application need not exit. Hence, the application can exit
        	// with EEikCmdExit itself
        	Exit();
        	break;
        	} 
        case EEikCmdExit:
            {
            // The User has exited the application using the Exit option from
            //.Options menu.  But, thats possible only if we exit the application
            // with EAknCmdExit.
            // A bit of convoluted logic but needed because we are using the
            // same components for the Settings UI Server as well as LC.
            
           	CEikAppServer* server = iEikonEnv->AppServer();
			if ( server )
				{
				server->NotifyServerExit(EAknCmdExit);
				}			
            Exit();
            break;
            }
        default:
            break;
        }
    }
