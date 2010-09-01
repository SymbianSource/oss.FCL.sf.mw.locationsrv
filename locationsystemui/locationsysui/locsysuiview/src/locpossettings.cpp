/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Positioning Settings implementation of Location UIs.
*
*/


// System Include
#include <eikenv.h>
#include <aknViewAppUi.h>

// User Include
#include "locpossettings.h"
#include "locsysuiview.h"

// ---------------------------------------------------------------------------
// CLocPosSettings::CLocPosSettings
// Overloaded Constructor
//
// ---------------------------------------------------------------------------
//
CLocPosSettings::CLocPosSettings()
    {
    }
   
// ---------------------------------------------------------------------------
// CLocPosSettings::~CLocPosSettings
// Destructor
//
// ---------------------------------------------------------------------------
//    
CLocPosSettings::~CLocPosSettings()
    {
    }

// ---------------------------------------------------------------------------
// CLocPosSettings* CLocPosSettings::NewL
// Two phase constructor
//
// ---------------------------------------------------------------------------
//
EXPORT_C CLocPosSettings* CLocPosSettings::NewL()
    {
    CLocPosSettings* self = CLocPosSettings::NewLC();
	CleanupStack::Pop( self );
	return self;
    }
           
// ---------------------------------------------------------------------------
// CLocPosSettings* CLocPosSettings::NewL
// Two phase constructor
//
// ---------------------------------------------------------------------------
//         
EXPORT_C CLocPosSettings* CLocPosSettings::NewLC()
    {
    CLocPosSettings* self = new(ELeave) CLocPosSettings();
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
    }

// ---------------------------------------------------------------------------
// void CLocPosSettings::ConstructL
//
// ---------------------------------------------------------------------------
// 
void CLocPosSettings::ConstructL()
    {
    // Create the Positioning Specfic view and set its observer
    iView = CLocSysUiView::NewL();
    
    CEikonEnv* env = CEikonEnv::Static();
    
    // Transferring the view's control to AppUi
    (static_cast<CAknViewAppUi*>( env->EikAppUi()))->AddViewL( iView );    
    }
    
// ---------------------------------------------------------------------------
// void CLocPosSettings::LaunchLocationUIL
//
// ---------------------------------------------------------------------------
//
void CLocPosSettings::LaunchLocationUIL( TInt		    /* aUiInputParams */,
                                         MLocationUIObserver*    aObserver )
    {
    // Set the call back observer for this Launch call
    iView->SetObserver( aObserver );
    
    // Activate the Positoning Settings View
    iView->ActivateViewL();
    }
    
// ---------------------------------------------------------------------------
// void CLocPosSettings::LaunchLocationUIL
//
// ---------------------------------------------------------------------------
//
void CLocPosSettings::LaunchLocationUIL( const TDesC&           /* aUiInputParams */,
                                         MLocationUIObserver*    aObserver )
    {
    // Set the call back observer for this Launch call
    iView->SetObserver( aObserver );
    
    // Activate the Positoning Settings View
    iView->ActivateViewL();
    }
    
// ---------------------------------------------------------------------------
// void CLocPosSettings::Close
//
// ---------------------------------------------------------------------------
//
void CLocPosSettings::Close()
    {
    // Deactivate the Postioning Settings view.
    iView->DeActivateView();
    }
