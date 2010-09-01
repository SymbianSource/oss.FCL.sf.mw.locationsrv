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
* Description:  Client side resource handle to the UI Server
*
*/


// System Includes
#include <avkon.hrh>

// User Includes
#include "locsettingsuiservice.h"
#include "loclaunchuiao.h"
#include "locsettingsuiserverinterface.h"

// Constant Declarations
const TInt KNumofConnectAttempts = 200;
    
// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ Default Constructor
// ---------------------------------------------------------------------------
//
RLocSettingsUiService::RLocSettingsUiService()
    :iLaunchAO( NULL ),
    iRequestStatus( NULL ),
    iLaunchType( ELaunchNone ),
    iSrvMonitor( NULL )
    {
    }

// ---------------------------------------------------------------------------
// C++ Destructor
// ---------------------------------------------------------------------------
//
RLocSettingsUiService::~RLocSettingsUiService()
    {
    if( ELaunchNone != iLaunchType )
        {
        CancelLaunchedSettingsUi();
        }   
    }

// ---------------------------------------------------------------------------
// Launch a Settings UI in an ui app server
// ---------------------------------------------------------------------------
//
void RLocSettingsUiService::LaunchSettingsUiL(
                                    TUid            aImplementationUid,
                                    TInt            aParams,
                                    TRequestStatus& aStatus )
    {
    
    // Check the presence of the Active Object
    if( NULL == iLaunchAO )
        {
        User::Leave( KErrNotFound );
        }
        
    // Check if there is any outstanding requests
    if( ELaunchNone != iLaunchType )
        {
        User::Leave( KErrInUse );
        }
    
    // Create the Server Session
    ConnectServerL();
    
    // Store the aStatus variable. This would be used to notify the caller
    // on completeion of the launch message
    iRequestStatus = &aStatus;  
    *iRequestStatus = KRequestPending;
    
    // Set request outstanding on the Active object
    iLaunchAO->SetRequestOutStandingL();
        
    // Compose the Message and Issue a Launch call to the server
    TRequestStatus&     status = iLaunchAO->GetStatusVariable();
    
    // Set the Launch Type and the Service state
    iLaunchType = ELaunchNormal;
                      
    TIpcArgs args( aImplementationUid.iUid, aParams );
    
	SendReceive( ELaunchSettings, args, status );
    
    iSrvMonitor = NULL;
    }
    
// ---------------------------------------------------------------------------
// Launch a Settings UI as an embedded application
// ---------------------------------------------------------------------------
//
void RLocSettingsUiService::LaunchSettingsUiAsEmbeddedAppL(
                                            TUid            aImplementationUid,
                                            TInt            aParams,
                                            TRequestStatus& aStatus )
    {
    
    // Check the presence of the Active Object
    if( NULL == iLaunchAO )
        {
        User::Leave( KErrNotFound );
        }
        
    // Check if there is any outstanding requests
    if( ELaunchNone != iLaunchType )
        {
        User::Leave( KErrInUse );
        }
    
    // Create the Server Session
    ConnectChainedAppL( TUid::Uid( KLocSettingsUiServerUid ));
    
    // Store the aStatus variable. This would be used to notfy the caller
    // on completeion of the launch message
    iRequestStatus = &aStatus;  
    *iRequestStatus = KRequestPending;
    
    // Set request outstanding on the Active object
    iLaunchAO->SetRequestOutStandingL();
        
    // Compose the Message and Issue a Launch call to the server
    TRequestStatus&     status = iLaunchAO->GetStatusVariable();
    
    // Set the Launch Request Type and the Service state
    iLaunchType = ELaunchEmbedded;  
                      
    TIpcArgs args( aImplementationUid.iUid, aParams );
	SendReceive( ELaunchSettings, args, status );

    iSrvMonitor = CApaServerAppExitMonitor::NewL(*this, 
                                                 *this,
                                                 CActive::EPriorityStandard);
    }

// ---------------------------------------------------------------------------
// Launch a Settings UI in an ui app server
// ---------------------------------------------------------------------------
//
void RLocSettingsUiService::LaunchSettingsUiL(
                                    TUid            aImplementationUid,
                                    const TDesC&            aParamsString,
                                    TRequestStatus& aStatus )
    {
    
    // Check the presence of the Active Object
    if( NULL == iLaunchAO )
        {
        User::Leave( KErrNotFound );
        }
        
    // Check if there is any outstanding requests
    if( ELaunchNone != iLaunchType )
        {
        User::Leave( KErrInUse );
        }
    
    // Create the Server Session
    ConnectServerL();
    
    // Store the aStatus variable. This would be used to notify the caller
    // on completeion of the launch message
    iRequestStatus = &aStatus;  
    *iRequestStatus = KRequestPending;
    
    // Set request outstanding on the Active object
    iLaunchAO->SetRequestOutStandingL();
        
    // Compose the Message and Issue a Launch call to the server
    TRequestStatus&     status = iLaunchAO->GetStatusVariable();
    
    // Set the Launch Type and the Service state
    iLaunchType = ELaunchNormal;
                      
    TIpcArgs args( aImplementationUid.iUid, aParamsString.Length(), &aParamsString );
    
	SendReceive( ELaunchSettingsWithString, args, status );
    
    iSrvMonitor = NULL;
    }
    
// ---------------------------------------------------------------------------
// Launch a Settings UI as an embedded application
// ---------------------------------------------------------------------------
//
void RLocSettingsUiService::LaunchSettingsUiAsEmbeddedAppL(
                                            TUid            aImplementationUid,
                                            const TDesC&            aParamsString,
                                            TRequestStatus& aStatus )
    {
    
    // Check the presence of the Active Object
    if( NULL == iLaunchAO )
        {
        User::Leave( KErrNotFound );
        }
        
    // Check if there is any outstanding requests
    if( ELaunchNone != iLaunchType )
        {
        User::Leave( KErrInUse );
        }
    
    // Create the Server Session
    ConnectChainedAppL( TUid::Uid( KLocSettingsUiServerUid ));
    
    // Store the aStatus variable. This would be used to notfy the caller
    // on completeion of the launch message
    iRequestStatus = &aStatus;  
    *iRequestStatus = KRequestPending;
    
    // Set request outstanding on the Active object
    iLaunchAO->SetRequestOutStandingL();
        
    // Compose the Message and Issue a Launch call to the server
    TRequestStatus&     status = iLaunchAO->GetStatusVariable();
    
    // Set the Launch Request Type and the Service state
    iLaunchType = ELaunchEmbedded;  
                      
    TIpcArgs args( aImplementationUid.iUid, aParamsString.Length(), &aParamsString );
    
	SendReceive( ELaunchSettingsWithString, args, status );

    iSrvMonitor = CApaServerAppExitMonitor::NewL(*this, 
                                                 *this,
                                                 CActive::EPriorityStandard);
    }
    
// ---------------------------------------------------------------------------
// Cancels a Launched request
// ---------------------------------------------------------------------------
//    
TInt RLocSettingsUiService::CancelLaunchedSettingsUi()
    {
    // If no request is outstanding then this operation is not supported 
    // in this context
    if( ELaunchNone == iLaunchType )
        {
        return KErrNotSupported;
        }
    
    // Issue a Cancel message
    TInt error = SendReceive( ECancelSettings );
    
    if ( error )
        {
        return error;
        }
        
    // Cancel the Active objects Request
    iLaunchAO->Cancel();

    // Delete the Server Exit monitor object before closing the 
    // server session.
    delete iSrvMonitor;
    iSrvMonitor = NULL;        

    // Delete the Server session
    RAknAppServiceBase::Close();
    
    // Set the Launch type so that next launchs will succeed
    iLaunchType = ELaunchNone;
    
    // Sets the Launch 
    // Complete the request with KErrCancel
    TRequestStatus*     status = iRequestStatus;
    User::RequestComplete( status, KErrCancel );
    
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// Launch Positioning Settings UI in an ui app server
// ---------------------------------------------------------------------------
//
void RLocSettingsUiService::LaunchPosSettingsL(
                                            TRequestStatus& aStatus )
    {
    // Check the presence of the Active Object
    if( NULL == iLaunchAO )
        {
        User::Leave( KErrNotFound );
        }
        
    // Check if there is any outstanding requests
    if( ELaunchNone != iLaunchType )
        {
        User::Leave( KErrInUse );
        }
    
    // Create the Server Session
    ConnectServerL();
    
    // Store the aStatus variable. This would be used to notify the caller
    // on completeion of the launch message
    iRequestStatus = &aStatus;  
    *iRequestStatus = KRequestPending;
    
    // Set request outstanding on the Active object
    iLaunchAO->SetRequestOutStandingL();
        
    // Compose the Message and Issue a Launch call to the server
    TRequestStatus&     status = iLaunchAO->GetStatusVariable();
    
    // Set the Launch Type and the Service state
    iLaunchType = ELaunchNormal;
    
	SendReceive( ELaunchPosSettings, status );
    
    iSrvMonitor = NULL;
    }
    
// ---------------------------------------------------------------------------
// Launch Positioning Settings UI as an embesdded application
// ---------------------------------------------------------------------------
//
void RLocSettingsUiService::LaunchPosSettingsAsEmbeddedAppL(
                                                TRequestStatus& aStatus )
    {
    // Check the presence of the Active Object
    if( NULL == iLaunchAO )
        {
        User::Leave( KErrNotFound );
        }
        
    // Check if there is any outstanding requests
    if( ELaunchNone != iLaunchType )
        {
        User::Leave( KErrInUse );
        }
    
    // Create the Server Session
    ConnectChainedAppL( TUid::Uid( KLocSettingsUiServerUid ));
    
    // Store the aStatus variable. This would be used to notfy the caller
    // on completeion of the launch message
    iRequestStatus = &aStatus;  
    *iRequestStatus = KRequestPending;
    
    // Set request outstanding on the Active object
    iLaunchAO->SetRequestOutStandingL();
        
    // Compose the Message and Issue a Launch call to the server
    TRequestStatus&     status = iLaunchAO->GetStatusVariable();
    
    // Set the Launch Request Type and the Service state
    iLaunchType = ELaunchEmbedded;  

	SendReceive( ELaunchPosSettings, status );

    iSrvMonitor = CApaServerAppExitMonitor::NewL(*this, 
                                                 *this,
                                                 CActive::EPriorityStandard);
    }
    
// ---------------------------------------------------------------------------
// Closes an already launched Positioning Settings UI.
// ---------------------------------------------------------------------------
//    
TInt RLocSettingsUiService::ClosePosSettings()
    {
    // If no request is outstanding then this operation is not supported 
    // in this context
    if( ELaunchNone == iLaunchType )
        {
        return KErrNotSupported;
        }
    
    // Issue a Cancel message
    TInt error = SendReceive( ECancelPosSettings );
    
    if ( error )
        {
        return error;
        }
        
    // Cancel the Active objects Request
    iLaunchAO->Cancel();

    // Delete the Server Exit monitor object before closing the 
    // server session.
    delete iSrvMonitor;
    iSrvMonitor = NULL;        

    // Delete the Server session
    RAknAppServiceBase::Close();
    
    // Set the Launch type so that next launchs will succeed
    iLaunchType = ELaunchNone;
    
    // Sets the Launch 
    // Complete the request with KErrCancel
    TRequestStatus*     status = iRequestStatus;
    User::RequestComplete( status, KErrCancel );
    
    return KErrNone;

    }    
    
// ---------------------------------------------------------------------------
// Sets the Active Object used for issuing asynshronous requests
// ---------------------------------------------------------------------------
//
void RLocSettingsUiService::SetLaunchAO( CLocLaunchUiAO*&      aLaunchAO )
    {
    iLaunchAO = aLaunchAO; 
    }

// ---------------------------------------------------------------------------
// Inherited from MLocLaunchUiObserver
// ---------------------------------------------------------------------------
//
void RLocSettingsUiService::LaunchUiComplete( TInt    aError )
    {
    // When the server exits due to Exit Command it responds with
    // EEikCmdExit. In this case the actual Error Code is KErrNone.
    // This is needed when we have the  following scenario,
    // 1. If the user does a back from the plugin UI instead of exit,
    //    then the client library will have to delete the Server Monitor
    //    object.
    // 2. If the user does a Exit then the scenario will be handled in
    //    HandleServerAppExit.
    if ( EEikCmdExit != aError )
        {
        delete iSrvMonitor;
        iSrvMonitor = NULL;

        // Delete the Server session
        Close();
        }
    else
        {
        // In case the Error value is EEikCmdExit, change it to KErrNone.
        // The session and the Server Monitor object will be taken care of
        // by the HandleServerAppExit() which gets triggered by the Server
        // Exit.
        aError = KErrNone;
        }

    iLaunchType = ELaunchNone;
    TRequestStatus* status = iRequestStatus;
    User::RequestComplete( status, aError );
    }

// ---------------------------------------------------------------------------
// Returns the Service UID of requested service
// ---------------------------------------------------------------------------
//
TUid RLocSettingsUiService::ServiceUid() const
    {
    return TUid::Uid( KLocSettingsUiServiceId );    
    }

// ---------------------------------------------------------------------------
// Receive event about server Exit. Triggered only for Embedded Launch case.
// ---------------------------------------------------------------------------
//
void RLocSettingsUiService::HandleServerAppExit(TInt aReason)
    {
    // Delete the Server monitor object.
    delete iSrvMonitor;
    iSrvMonitor = NULL;
    // Delete the Server session
    Close();
    if ( EEikCmdExit == aReason )
        {
        // The base class implementation is a Series60 behaviour. Hence
        // it works only if the Command ID is EAknCmdExit. Whereas from the
        // Server side we exit only for EEikCmdExit. Hence this fix is
        // needed for the correct Exit behaviour for Embedded Launch scenario.
        aReason = EAknCmdExit;
        }
    // Base call the AvKon Implementation.
    MAknServerAppExitObserver::HandleServerAppExit(aReason);
    }


// ---------------------------------------------------------------------------
// Connects to a non embedded server instance.
// ---------------------------------------------------------------------------
//
void RLocSettingsUiService::ConnectServerL()
    {
    TInt retry = KNumofConnectAttempts;
    TInt err = KErrNone;

    for (;;) // Forever
    	{
    	TUid    serviceuid = TUid::Uid( KLocSettingsUiServiceId );
    	TVersion*    version = reinterpret_cast< TVersion* >( &serviceuid );
    	err = CreateSession( KLocSettingsUiServerName, 
    	                     *version,
    	                     KDefaultMessageSlots );
    	if ( err != KErrNotFound && err != KErrServerTerminated )
    		{
    		break;
    		}

    	if ( --retry == 0 )
    		{
    		break;
    		}

    	err = StartServer();

    	if ( err != KErrAlreadyExists && err != KErrNone)
    		{
    		break;
    		}
    	}
    if( KErrNone != err )
        {
        User::Leave( err );
        }
    }
 
// ---------------------------------------------------------------------------
// Starts a new server instance
// ---------------------------------------------------------------------------
//   
TInt RLocSettingsUiService::StartServer()
    {
    const TUidType serverUid(KNullUid, KNullUid, TUid::Uid( KLocSettingsUiServerUid ));

    // Simultaneous launching of two such processes should be detected 
    // when the second one attempts to create the server object, 
    // failing with KErrAlreadyExists.
    //
    RProcess server;
    TInt ret = server.Create( KLocSettingsUiServerExe, KNullDesC, serverUid );

    if (ret != KErrNone)
        {
        return ret;
        }

    TRequestStatus died;
    server.Rendezvous( died );

    if ( died != KRequestPending )
        {
        // logon failed - server is not yet running, so cannot have terminated
        User::WaitForRequest( died );           // eat signal
        server.Kill( 0 );                       // abort startup
        }
    else
        {
        server.Resume();
        User::WaitForRequest( died );           // wait for start or death
        }

    // we can't use the 'exit reason' if the server panicked as this
    // is the panic 'reason' and may be '0' which cannot be distinguished
    // from KErrNone
    ret = ( server.ExitType() == EExitPanic ) ? KErrGeneral : died.Int();
    server.Close();
    return ret;
    }
