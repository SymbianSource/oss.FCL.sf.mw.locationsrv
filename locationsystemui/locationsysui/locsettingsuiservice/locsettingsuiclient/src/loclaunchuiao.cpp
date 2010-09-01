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
* Description:  Active Object to monitor the UI Server launch requests
*
*/


// User Include
#include "loclaunchuiao.h"
#include "loclaunchuiobserver.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ Default Constructor
// ---------------------------------------------------------------------------
//
CLocLaunchUiAO::CLocLaunchUiAO( MLocLaunchUiObserver&    aObserver )
    : CActive( EPriorityStandard ),
    iObserver( aObserver )
    {
    // Add the Active object to the Active scheduler queue
    CActiveScheduler::Add( this );    
    }

// ---------------------------------------------------------------------------
// Two Phase constructor 
// ---------------------------------------------------------------------------
//
CLocLaunchUiAO* CLocLaunchUiAO::NewL( MLocLaunchUiObserver&    aObserver )
    {
    CLocLaunchUiAO* self = CLocLaunchUiAO::NewLC( aObserver );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Two Phase constructor. Leaves the allocated object on the Clean-up stack 
// ---------------------------------------------------------------------------
//
CLocLaunchUiAO* CLocLaunchUiAO::NewLC( MLocLaunchUiObserver&    aObserver )
    {
    CLocLaunchUiAO* self = new( ELeave ) CLocLaunchUiAO( aObserver );
    CleanupStack::PushL( self );
    return self;
    }


// ---------------------------------------------------------------------------
// C++ Destructor
// ---------------------------------------------------------------------------
//
CLocLaunchUiAO::~CLocLaunchUiAO()
    {
    // Cancel any outstanding request
    Cancel();
    }
    
// ---------------------------------------------------------------------------
// Informs the active object that there is an outstanding request.
// ---------------------------------------------------------------------------
//    
void    CLocLaunchUiAO::SetRequestOutStandingL()
    {
    // Check if there is any request outstanding for there can be only one 
    // request outstanding on the active object at any instant of time
    if( IsActive())
        {
        User::Leave( KErrInUse );
        }
    iStatus = KRequestPending;
    SetActive();
    }

// ---------------------------------------------------------------------------
// Returns a reference to the active objects Status variable
// ---------------------------------------------------------------------------
// 
TRequestStatus&     CLocLaunchUiAO::GetStatusVariable()
    {
    return iStatus;
    }

// ---------------------------------------------------------------------------
// CActive RunL
// ---------------------------------------------------------------------------
//
void CLocLaunchUiAO::RunL()
    {
    // Issue a callback only when the active object has not been cancelled
    if( KErrCancel != iStatus.Int())
        {
        iObserver.LaunchUiComplete( iStatus.Int());
        }
    }
    
// ---------------------------------------------------------------------------
// CActive DoCancel
// ---------------------------------------------------------------------------
//
void CLocLaunchUiAO::DoCancel()
    {
    }
