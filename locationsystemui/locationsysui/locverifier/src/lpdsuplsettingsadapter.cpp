/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Adapter to the SUPL Settings API
*
*/


// System Includes
#include <epos_csuplsettings.h>
#include <locsuplsettingsui.rsg>
#include <StringLoader.h>

// User Includes
#include "lpdsuplsettingsadapter.h"
#include "lpdsuplsettingsadapterobserver.h"


// ---------------------------------------------------------------------------
// Private Constructor
// ---------------------------------------------------------------------------
//
CLpdSUPLSettingsAdapter::CLpdSUPLSettingsAdapter( 
                            MLpdSUPLSettingsAdapterObserver&    aObserver )
		:iObserver( aObserver )
    {
    }
 
// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//    
CLpdSUPLSettingsAdapter::~CLpdSUPLSettingsAdapter()
    {
   // delete iTimer;
    if( iSUPLSettings )
        {
        iSUPLSettings->RemoveSessionObserver();
        }
        
    delete iSUPLSettings;
    iSUPLSettings = NULL;	

    } 

// ---------------------------------------------------------------------------
// CLpdSUPLSettingsAdapter* CLpdSUPLSettingsAdapter::NewL
// Static Two phase contructor that instantiates the CLpdSUPLSettingsAdapter
// 
// @param aObserver				    Observer to the SUPL settings adapter
// @return CLpdSUPLSettingsAdapter*	Reference to the object created
// ---------------------------------------------------------------------------
//    
CLpdSUPLSettingsAdapter* CLpdSUPLSettingsAdapter::NewL(
                            MLpdSUPLSettingsAdapterObserver&    aObserver )
    {
    CLpdSUPLSettingsAdapter* self = 
                new ( ELeave ) CLpdSUPLSettingsAdapter( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );	
    return self;
    }

// ---------------------------------------------------------------------------
// void CLpdSUPLSettingsAdapter::ConstructL
// Second phase of the two phase constructor
// ---------------------------------------------------------------------------
// 
void CLpdSUPLSettingsAdapter::ConstructL()
    {
    // Create the SUPL Settings
    iSUPLSettings = CSuplSettings::NewL();
    
    // Set the Observer for SUPL Sessions
    iSUPLSettings->SetSessionObserverL( *this );
   
    }  

// ---------------------------------------------------------------------------
// void CLpdSUPLSettingsAdapter::HandleSuplSessionChangeL
// Handle the Supl Session changes
// ---------------------------------------------------------------------------
//     
void CLpdSUPLSettingsAdapter::HandleSuplSessionChangeL( 
                TSuplSessionEventType aEvent,
                TInt64 aSessionId                        
        		)
    {
	if( aEvent == MSuplSessionObserver::ESuplSessionDBUpdateEvent ||
		aEvent == MSuplSessionObserver::ESuplSessionDBDeleteEvent )
		  {
      iObserver.UpdateSessionL( aSessionId );
	    }	
    }

// -----------------------------------------------------------------------------
// CLpdSUPLSettingsAdapter::GetSessionNameAndPeriodL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdSUPLSettingsAdapter::GetTriggerPeriodL( TInt64 aSessionId, TUint& aTimePeriod )
    {
    CTriggerParams* trigParams = CTriggerParams::NewL();
    CleanupStack::PushL( trigParams );
     
    User::LeaveIfError( iSUPLSettings->GetTriggerParams( aSessionId, trigParams ) );
     
    TUint64 outstandingTrigger;
    TUint64 interval;
    TBool notificationPresent;
    TBool triggerNotificationStatus;  
    CTriggerParams::TTriggerType triggerType; 
    CTriggerParams::TRequestType requestType;
    
    HBufC* sessionName;
    sessionName = HBufC::NewLC( 256 ); 
                  
    sessionName->Des().Zero();
       
    User::LeaveIfError( trigParams->Get(
                             aSessionId, 
                             sessionName->Des(), 
                             notificationPresent, 
                             triggerNotificationStatus, 
                             triggerType,
                             requestType, 
                             outstandingTrigger, 
                             interval) );
    
    aTimePeriod = interval*outstandingTrigger;
    
    CleanupStack::PopAndDestroy( ); // sessionName
    CleanupStack::PopAndDestroy( ); // trigParams
    }

// End of file
