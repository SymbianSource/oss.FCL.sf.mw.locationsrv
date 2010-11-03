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
* Description:  Location Notation Preferences Plugin Systen of Measurement adapter
*
*/


// System Include
#include <centralrepository.h>

// User Include
#include "locnotprefsomadapter.h"
#include "locnotprefadapterobserver.h"


// ========================= MEMBER FUNCTIONS ================================


// ---------------------------------------------------------------------------
// CLocNotPrefSystemofMeasAdapter::CLocNotPrefSystemofMeasAdapter
// Overloaded Constructor
//
// ---------------------------------------------------------------------------
//  
CLocNotPrefSystemofMeasAdapter::CLocNotPrefSystemofMeasAdapter(  
                                MLocNotPrefAdapterObserver& aAdapterObsrv )
    :CActive(EPriorityStandard),
    iObserver(aAdapterObsrv)
    {
    }

// ---------------------------------------------------------------------------
// CLocNotPrefSystemofMeasAdapter::~CLocNotPrefSystemofMeasAdapter
// Destructor
//
// ---------------------------------------------------------------------------
//
CLocNotPrefSystemofMeasAdapter::~CLocNotPrefSystemofMeasAdapter()
    {
    Cancel();
    
    delete iRepository; 
    }
    
// ---------------------------------------------------------------------------
// CLocNotPrefSystemofMeasAdapter* CLocNotPrefSystemofMeasAdapter::NewL
// Creates new Systen of Measurement adapter
//
// @param  aAdapterObsrv                        Observer to the Settings adapter
// @return CLocNotPrefSystemofMeasAdapter&  Reference to the application view
// ---------------------------------------------------------------------------
//    
CLocNotPrefSystemofMeasAdapter* CLocNotPrefSystemofMeasAdapter::NewL( 
                            MLocNotPrefAdapterObserver& aAdapterObsrv )
    {   
    CLocNotPrefSystemofMeasAdapter* self = 
                        CLocNotPrefSystemofMeasAdapter::NewLC( aAdapterObsrv );
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// CLocNotPrefSystemofMeasAdapter* CLocNotPrefSystemofMeasAdapter::NewLC
// Creates new Systen of Measurement adapter. Leaves the object on the clean-up
// stack
//
// @param  aAdapterObsrv                        Observer to the Settings adapter
// @return CLocNotPrefSystemofMeasAdapter&  Reference to the application view
// ---------------------------------------------------------------------------
// 
CLocNotPrefSystemofMeasAdapter* CLocNotPrefSystemofMeasAdapter::NewLC( 
                            MLocNotPrefAdapterObserver& aAdapterObsrv )
    {   
    CLocNotPrefSystemofMeasAdapter* self = 
                        new(ELeave) CLocNotPrefSystemofMeasAdapter( aAdapterObsrv );
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }
    
// ---------------------------------------------------------------------------
// void CLocNotPrefSystemofMeasAdapter::ConstructL
// Second Phase Constructor
//
// ---------------------------------------------------------------------------
//
void CLocNotPrefSystemofMeasAdapter::ConstructL()
    {
    CActiveScheduler::Add( this );
    
    // Create the Central repository object for manipulating Avkon Central
    // repository for System of measurements settings
    iRepository = CRepository::NewL(TUid::Uid( KCRUidLocNotationPref ));
    
    StartNotification();    
    }

// ---------------------------------------------------------------------------
// TLocSystemofMeasurementValues CLocNotPrefSystemofMeasAdapter::GetCurrentSysofMeasL
// Returns the current value of the System of measurements settings
//
// @return TLocSystemofMeasurementValues  Current value of the System of 
//                                        measurements settings
// ---------------------------------------------------------------------------
//
TLocSystemofMeasurementValues CLocNotPrefSystemofMeasAdapter::GetCurrentSysofMeasL()
    {
    
    TInt settingsValue = 0;
    User::LeaveIfError( iRepository->Get( KLocSystemofMeasurements,
                                          settingsValue ));  
    return ( static_cast<TLocSystemofMeasurementValues>( settingsValue ));
    }

// --------------------------------------------------------------------------
// void CLocNotPrefSystemofMeasAdapter::SetCurrentSysofMeasL
// Sets the value of the System of measurements settings
//
// @param aSettingsValue  New value of the System of measurements settings
// ---------------------------------------------------------------------------
//
void CLocNotPrefSystemofMeasAdapter::SetCurrentSysofMeasL( 
                            TLocSystemofMeasurementValues aSettingsValue )
    {
    User::LeaveIfError( iRepository->Set( KLocSystemofMeasurements,
                                          aSettingsValue ));
    }
 
// --------------------------------------------------------------------------
// void CLocNotPrefSystemofMeasAdapter::ToggleSysofMeasSettingsL
// Toggles the settings value
// 
// ---------------------------------------------------------------------------
//
void CLocNotPrefSystemofMeasAdapter::ToggleSysofMeasSettingsL()
    {
    TInt currentSettings = 0;
    TLocSystemofMeasurementValues newSettings = ELocMetric;
    
    // Obtain the current settings value and if its ELocMetric
    // then set ELocImperial else set ELocMetric
    User::LeaveIfError( iRepository->Get( KLocSystemofMeasurements,
                                          currentSettings ));
    
    if( ELocMetric == currentSettings )
        {
        newSettings = ELocImperial;
        }
    User::LeaveIfError( iRepository->Set( KLocSystemofMeasurements,
                                          newSettings ));
    }
            
// --------------------------------------------------------------------------
// void CLocNotPrefSystemofMeasAdapter::RunL
// Inherited from CActive
//
// ---------------------------------------------------------------------------
//
void CLocNotPrefSystemofMeasAdapter::RunL()
    {
    // Issue Notification
    iObserver.HandleSettingsChangeL();
    
    // Schedule for listening to change events again
    StartNotification();
    }

// --------------------------------------------------------------------------
// void CLocNotPrefSystemofMeasAdapter::DoCancel
// Inherited from CActive
//
// ---------------------------------------------------------------------------
//
void CLocNotPrefSystemofMeasAdapter::DoCancel()
    {
    // Cancel the outstanding CR notification request
    if( iRepository )
        {
        iRepository->NotifyCancel( KLocSystemofMeasurements );        
        }
    }
    
// --------------------------------------------------------------------------
// void CLocNotPrefSystemofMeasAdapter::StartNotification
// Start notification from the Central repository server for any changes in the
// System of measurements key value
//
// ---------------------------------------------------------------------------
//
void CLocNotPrefSystemofMeasAdapter::StartNotification()
    {
    if( !IsActive())
        {
        iStatus = KRequestPending;
        // Request for notification
        iRepository->NotifyRequest( KLocSystemofMeasurements, iStatus);
        SetActive();          
        }  
    }
