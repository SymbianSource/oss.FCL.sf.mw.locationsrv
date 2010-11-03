/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Adapter for System of Measurement.
*
*/


// System Include
#include <centralrepository.h>

// INCLUDE FILES
#include "csatellitemeasurementsetting.h"
#include "msatellitemeasurementsettingobserver.h"

// CONSTANTS


// ============================ MEMBER FUNCTIONS ==============================

// ---------------------------------------------------------------------------
// CSatelliteMeasurementSetting::CSatelliteMeasurementSetting
// Overloaded Constructor
//
// ---------------------------------------------------------------------------
//	
CSatelliteMeasurementSetting::CSatelliteMeasurementSetting(  
                                MSatelliteMeasurementSettingObserver&	aAdapterObsrv )
	:CActive(EPriorityStandard),
	iObserver(aAdapterObsrv)
	{
	}

// ---------------------------------------------------------------------------
// CSatelliteMeasurementSetting::~CSatelliteMeasurementSetting
// Destructor
//
// ---------------------------------------------------------------------------
//
CSatelliteMeasurementSetting::~CSatelliteMeasurementSetting()
	{
	Cancel();
	delete iRepository;
	}
	
// ---------------------------------------------------------------------------
// CSatelliteMeasurementSetting* CSatelliteMeasurementSetting::NewL
// Creates new System of Measurement Adapter
// ---------------------------------------------------------------------------
//    
CSatelliteMeasurementSetting* CSatelliteMeasurementSetting::NewL( 
                            MSatelliteMeasurementSettingObserver&	aAdapterObsrv )
	{	
    CSatelliteMeasurementSetting* self = 
                        CSatelliteMeasurementSetting::NewLC( aAdapterObsrv );
	CleanupStack::Pop(self);
	return self;
	}

// ---------------------------------------------------------------------------
// CSatelliteMeasurementSetting* CSatelliteMeasurementSetting::NewLC
// Creates new System of Measurement Adapter. Leaves the object on the clean-up
// stack
// ---------------------------------------------------------------------------
// 
CSatelliteMeasurementSetting* CSatelliteMeasurementSetting::NewLC( 
                            MSatelliteMeasurementSettingObserver&	aAdapterObsrv )
	{	
    CSatelliteMeasurementSetting* self = 
                        new(ELeave) CSatelliteMeasurementSetting( aAdapterObsrv );
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}
	
// ---------------------------------------------------------------------------
// void CSatelliteMeasurementSetting::ConstructL
// Second Phase Constructor
//
// ---------------------------------------------------------------------------
//
void CSatelliteMeasurementSetting::ConstructL()
	{
	CActiveScheduler::Add( this );
	
	// Create the Central repository object for manipulating Avkon Central
	// repository for System of measurements settings
	iRepository = CRepository::NewL(TUid::Uid( KCRUidLocNotationPref ));
	
	StartNotification();	
	}

// ---------------------------------------------------------------------------
// TReal32 CSatelliteMeasurementSetting::SysofMeasurementL
// Returns the current value of the System of measurements settings
//
// @return TLocSystemofMeasurementValues  Current value of the 
// 																				System of measurements settings
// ---------------------------------------------------------------------------
//
TLocSystemofMeasurementValues CSatelliteMeasurementSetting::SysofMeasurementL()
    {
    
    TInt settingsValue = 0;
    User::LeaveIfError( iRepository->Get( KLocSystemofMeasurements,
                                          settingsValue ));  
    return ( static_cast<TLocSystemofMeasurementValues>( settingsValue ));
    }

	    
// --------------------------------------------------------------------------
// void CSatelliteMeasurementSetting::RunL
// Inherited from CActive
//
// ---------------------------------------------------------------------------
//
void CSatelliteMeasurementSetting::RunL()
    {
    // Issue Notification
    iObserver.HandleSysMeasureValueL();
    
    // Schedule for listening to change events again
    StartNotification();
    }

// --------------------------------------------------------------------------
// void CSatelliteMeasurementSetting::DoCancel
// Inherited from CActive
//
// ---------------------------------------------------------------------------
//
void CSatelliteMeasurementSetting::DoCancel()
    {
    // Cancel the outstanding CR notification request
    iRepository->NotifyCancel( KLocSystemofMeasurements );
    }
    
// --------------------------------------------------------------------------
// void CSatelliteMeasurementSetting::StartNotification
// Start notification from the Central repository server for any changes in the
// System of measurements key value
//
// ---------------------------------------------------------------------------
//
void CSatelliteMeasurementSetting::StartNotification()
    {
    if( !IsActive())
        {
        iStatus = KRequestPending;
        // Request for notification
        iRepository->NotifyRequest( KLocSystemofMeasurements, iStatus);
        SetActive();          
        }  
    }
	
// End of File

