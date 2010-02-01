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
* Description:  Client Side handle for using the Location System UI services
*
*/


// System Includes
#include <aknenv.h>

// User Includes
#include "locsettingsuiclient.h"
#include "loclaunchuiao.h"
#include "locsettingsuiservice.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ Default Constructor
// ---------------------------------------------------------------------------
//
CLocSettingsUiClient::CLocSettingsUiClient()
    {
    }


// ---------------------------------------------------------------------------
// Second phase of the two phase constructor
// ---------------------------------------------------------------------------
//
void CLocSettingsUiClient::ConstructL()
    {
    // Obtain the UI Environment. If the Client Library is constructed from
    // a non-UI application then the value of the UI Env should be NULL
    iCEikonEnv = CEikonEnv::Static();
    
    // Create the Client side handle.
    iLocService = new ( ELeave ) RLocSettingsUiService;
        
    // Create the Active object that would be used by the client handle
    // sending asynchronous requests
    iLaunchUiAO = CLocLaunchUiAO::NewL( *iLocService );
    
    // Set the Client Handles AO. The ownership is mainted with this class
    // since the Client side handle is an RClass and should preferably not
    // have any objects allocated on the heap
    iLocService->SetLaunchAO( iLaunchUiAO );
    
    }


// ---------------------------------------------------------------------------
// Two Phase constructor 
// ---------------------------------------------------------------------------
//
EXPORT_C CLocSettingsUiClient* CLocSettingsUiClient::NewL()
    {
    CLocSettingsUiClient* self = CLocSettingsUiClient::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Two Phase constructor. Leaves the allocated object on the Clean-up stack 
// ---------------------------------------------------------------------------
//
EXPORT_C CLocSettingsUiClient* CLocSettingsUiClient::NewLC()
    {
    CLocSettingsUiClient* self = new( ELeave ) CLocSettingsUiClient;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// C++ Destructor
// ---------------------------------------------------------------------------
//
CLocSettingsUiClient::~CLocSettingsUiClient()
    {
    
    // Delete the Client side handle
    delete iLocService;
    
    // Delete the Active Object
    delete iLaunchUiAO;    
    }

// ---------------------------------------------------------------------------
// Launch a Settings UI in an ui app server
// ---------------------------------------------------------------------------
//
EXPORT_C void CLocSettingsUiClient::LaunchSettingsUiL(
                                            TUid            aImplementationUid,
                                            TInt            aParams,
                                            TRequestStatus& aStatus )
    {
    // Call the Client Handle's Launch function
    iLocService->LaunchSettingsUiL( aImplementationUid,
                                    aParams,
                                    aStatus );
    }
    
// ---------------------------------------------------------------------------
// Launch a Settings UI as an embedded application
// ---------------------------------------------------------------------------
//
EXPORT_C void CLocSettingsUiClient::LaunchSettingsUiAsEmbeddedAppL(
                                            TUid            aImplementationUid,
                                            TInt            aParams,
                                            TRequestStatus& aStatus )
    {
    // Only an UI application can make a Launch as embedded call
    if( NULL == iCEikonEnv )
        {
        User::Leave( KErrNotSupported );
        }
    // Call the Client Handle's Launch function     
    iLocService->LaunchSettingsUiAsEmbeddedAppL( aImplementationUid,
                                                 aParams,
                                                 aStatus );    
    }

// ---------------------------------------------------------------------------
// Launch a Settings UI in an ui app server
// ---------------------------------------------------------------------------
//
EXPORT_C void CLocSettingsUiClient::LaunchSettingsUiL(
                                            TUid            aImplementationUid,
                                            const TDesC&            aStringParams,
                                            TRequestStatus& aStatus )
    {
    // Call the Client Handle's Launch function
    iLocService->LaunchSettingsUiL( aImplementationUid,
                                    aStringParams,
                                    aStatus );
    }
    
// ---------------------------------------------------------------------------
// Launch a Settings UI as an embedded application
// ---------------------------------------------------------------------------
//
EXPORT_C void CLocSettingsUiClient::LaunchSettingsUiAsEmbeddedAppL(
                                            TUid            aImplementationUid,
                                            const TDesC&            aStringParams,
                                            TRequestStatus& aStatus )
    {
    // Only an UI application can make a Launch as embedded call
    if( NULL == iCEikonEnv )
        {
        User::Leave( KErrNotSupported );
        }
    // Call the Client Handle's Launch function     
    iLocService->LaunchSettingsUiAsEmbeddedAppL( aImplementationUid,
                                                 aStringParams,
                                                 aStatus );    
    }
    
// ---------------------------------------------------------------------------
// Cancels an already launched settings UI.
// ---------------------------------------------------------------------------
//    
EXPORT_C TInt CLocSettingsUiClient::CancelLaunchedSettingsUi()
    {
    return iLocService->CancelLaunchedSettingsUi();
    }
    
// ---------------------------------------------------------------------------
// Launch Positioning Settings UI in an ui app server
// ---------------------------------------------------------------------------
//
EXPORT_C void CLocSettingsUiClient::LaunchPosSettingsL(
                                            TRequestStatus& aStatus )
    {
    // Only FrameWorks  can make a Launch as non-embedded call
    if( iCEikonEnv )
        {
        User::Leave( KErrNotSupported );
        }
    // Call the Client Handle's Launch function        
    iLocService->LaunchPosSettingsL( aStatus );
    }
    
// ---------------------------------------------------------------------------
// Launch Positioning Settings UI as an embedded application
// ---------------------------------------------------------------------------
//
EXPORT_C void CLocSettingsUiClient::LaunchPosSettingsAsEmbeddedAppL(
                                                TRequestStatus& aStatus )
    {
    // Only an UI application can make a Launch as embedded call
    if( NULL == iCEikonEnv )
        {
        User::Leave( KErrNotSupported );
        }
    // Call the Client Handle's Launch function        
    iLocService->LaunchPosSettingsAsEmbeddedAppL( aStatus );
    }
    
// ---------------------------------------------------------------------------
// Closes an already launched Positioning Settings UI.
// ---------------------------------------------------------------------------
//    
EXPORT_C TInt CLocSettingsUiClient::ClosePosSettings()
    {
    return iLocService->ClosePosSettings();
    }    
