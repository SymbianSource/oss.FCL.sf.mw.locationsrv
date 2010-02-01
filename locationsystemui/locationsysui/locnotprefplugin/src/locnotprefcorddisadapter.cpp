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
* Description:  Location Notation Preferences Plugin Co-ordinate display adapter
*
*/


// System Include
#include <centralrepository.h>

// User Include
#include "locnotprefcorddisadapter.h"
#include "locnotprefadapterobserver.h"

// Constant Definition
const TUint KAvkonCRUID = 0x101F876E;
const TUint KLocCoordinateDisplayUID = 0x00000009;

// ========================= MEMBER FUNCTIONS ================================


// ---------------------------------------------------------------------------
// CLocNotPrefCoordinateDisplayAdapter::CLocNotPrefCoordinateDisplayAdapter
// Overloaded Constructor
//
// ---------------------------------------------------------------------------
//  
CLocNotPrefCoordinateDisplayAdapter::CLocNotPrefCoordinateDisplayAdapter(  
                                MLocNotPrefAdapterObserver& aAdapterObsrv )
    :CActive(EPriorityStandard),
    iObserver(aAdapterObsrv)
    {
    }

// ---------------------------------------------------------------------------
// CLocNotPrefCoordinateDisplayAdapter::~CLocNotPrefCoordinateDisplayAdapter
// Destructor
//
// ---------------------------------------------------------------------------
//
CLocNotPrefCoordinateDisplayAdapter::~CLocNotPrefCoordinateDisplayAdapter()
    {
    Cancel();
    
    delete iRepository;
    }
    
// ---------------------------------------------------------------------------
// CLocNotPrefCoordinateDisplayAdapter* CLocNotPrefCoordinateDisplayAdapter::NewL
// Creates new Co-ordinate display adapter
//
// @param  aAdapterObsrv                        Observer to the Settings adapter
// @return CLocNotPrefCoordinateDisplayAdapter& Reference to the application view
// ---------------------------------------------------------------------------
//    
CLocNotPrefCoordinateDisplayAdapter* CLocNotPrefCoordinateDisplayAdapter::NewL( 
                            MLocNotPrefAdapterObserver& aAdapterObsrv )
    {   
    CLocNotPrefCoordinateDisplayAdapter* self = 
                        CLocNotPrefCoordinateDisplayAdapter::NewLC( aAdapterObsrv );
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// CLocNotPrefCoordinateDisplayAdapter* CLocNotPrefCoordinateDisplayAdapter::NewLC
// Creates new Co-ordinate display adapter. Leaves the object on the clean-up
// stack
//
// @param  aAdapterObsrv                        Observer to the Settings adapter
// @return CLocNotPrefCoordinateDisplayAdapter& Reference to the application view
// ---------------------------------------------------------------------------
// 
CLocNotPrefCoordinateDisplayAdapter* CLocNotPrefCoordinateDisplayAdapter::NewLC( 
                            MLocNotPrefAdapterObserver& aAdapterObsrv )
    {   
    CLocNotPrefCoordinateDisplayAdapter* self = 
                        new(ELeave) CLocNotPrefCoordinateDisplayAdapter( aAdapterObsrv );
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }
    
// ---------------------------------------------------------------------------
// void CLocNotPrefCoordinateDisplayAdapter::ConstructL
// Second Phase Constructor
//
// ---------------------------------------------------------------------------
//
void CLocNotPrefCoordinateDisplayAdapter::ConstructL()
    {
    CActiveScheduler::Add( this );
    
    // Create the Central repository object for manipulating Avkon Central
    // repository for Co-ordinate display settings
    iRepository = CRepository::NewL(TUid::Uid( KAvkonCRUID));
    
    StartNotification();
    }
    
// ---------------------------------------------------------------------------
// CLocNotPrefCoordinateDisplayAdapter::TCordDisSetListPosition 
//                CLocNotPrefCoordinateDisplayAdapter::GetCurrentDisplayFormatL
// Returns the position of the Co-ordinate display format settings value.
//
// @return TCordDisSetListPosition Current postion of the Co-ordinate display
//                                 settings value.
// ---------------------------------------------------------------------------
//
CLocNotPrefCoordinateDisplayAdapter::TCordDisSetListPosition 
                CLocNotPrefCoordinateDisplayAdapter::GetCurrentDisplayFormatL()
    {
    
    TInt settingsValue = EDMSS;
    User::LeaveIfError( iRepository->Get( KLocCoordinateDisplayUID,
                                          settingsValue ));
    
    TCordDisSetListPosition setPos = EPosDMSS;
    switch( settingsValue )
        {
        case EDD:
            {
            setPos = EPosDD;
            break;
            }
        case EDMM:
            {
            setPos = EPosDMM;
            break;
            }
        case EDMSS:           
        default:
            {
             // Incase there was no valid settings value present then we 
             // set the default value. Hence, nothing to do
            break;
            }                        
        }
    return setPos;
    }

// --------------------------------------------------------------------------
// void CLocNotPrefCoordinateDisplayAdapter::SetCurrentDisplayFormatL
// Sets the value of the Co-ordinate display format settings based on the
// display position.
//
// @param aSettingsPos  Position of the new Co-ordinate display settings value
// ---------------------------------------------------------------------------
//
void CLocNotPrefCoordinateDisplayAdapter::SetCurrentDisplayFormatL( 
     CLocNotPrefCoordinateDisplayAdapter::TCordDisSetListPosition aSettingsValue )
    {
    
    TCoordinateDisplayValue setValue = EDMSS;
    switch( aSettingsValue )
        {
        case EPosDD:
            {
            setValue = EDD;
            break;
            }
        case EPosDMM:
            {
            setValue = EDMM;
            break;
            }
        case EPosDMSS:           
        default:
            {
             // Incase there was no valid settings value present then we 
             // set the default value. Hence, nothing to do
            break;
            }                        
        }
    User::LeaveIfError( iRepository->Set( KLocCoordinateDisplayUID,
                                          setValue ));
    }
    
// --------------------------------------------------------------------------
// void CLocNotPrefCoordinateDisplayAdapter::RunL
// Inherited from CActive
//
// ---------------------------------------------------------------------------
//
void CLocNotPrefCoordinateDisplayAdapter::RunL()
    {
    // Issue Notification
    iObserver.HandleSettingsChangeL();
    
    // Schedule for listening to change events again
    StartNotification();
    }

// --------------------------------------------------------------------------
// void CLocNotPrefCoordinateDisplayAdapter::DoCancel
// Inherited from CActive
//
// ---------------------------------------------------------------------------
//
void CLocNotPrefCoordinateDisplayAdapter::DoCancel()
    {
    // Cancel the outstanding CR notification request
    if( iRepository )
        {
        iRepository->NotifyCancel( KLocCoordinateDisplayUID );        
        }
    }
    
// --------------------------------------------------------------------------
// void CLocNotPrefCoordinateDisplayAdapter::StartNotification
// Start notification from the Central repository server for any changes in the
// Co-ordinate display format key value
//
// ---------------------------------------------------------------------------
//
void CLocNotPrefCoordinateDisplayAdapter::StartNotification()
    {
    if( !IsActive())
        {
        // Request for notification
        iRepository->NotifyRequest( KLocCoordinateDisplayUID, iStatus);
        SetActive();          
        }  
    }
