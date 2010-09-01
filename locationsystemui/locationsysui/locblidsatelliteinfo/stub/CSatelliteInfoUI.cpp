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
* Description:  Implements interface for CSatelliteInfoUI Stub
*
*/


// INCLUDE FILES
#include <CSatelliteInfoUI.h>


// ============================ MEMBER FUNCTIONS ==============================
// ----------------------------------------------------------------------------
// CSatelliteInfoUI::NewL
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
EXPORT_C CSatelliteInfoUI* CSatelliteInfoUI::NewL()
    {
    CSatelliteInfoUI* self = new( ELeave ) CSatelliteInfoUI;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); // self
    return self;
    }

// ---------------------------------------------------------
// CSatelliteInfoUI::ConstructL
// ---------------------------------------------------------
//
void CSatelliteInfoUI::ConstructL()
    {    
    
    }

// ----------------------------------------------------------------------------
// CSatelliteInfoUI::CSatelliteInfoUI
// C++ default constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------------------------------
//
CSatelliteInfoUI::CSatelliteInfoUI()
    {
    }

// ---------------------------------------------------------
// CSatelliteInfoUI::~CSatelliteInfoUI
// ---------------------------------------------------------
//
EXPORT_C CSatelliteInfoUI::~CSatelliteInfoUI()
    {
    
    }

// ---------------------------------------------------------
// CSatelliteInfoUI::ExecuteLD
// ---------------------------------------------------------
//                       
EXPORT_C TInt CSatelliteInfoUI::ExecuteLD( const TDesC& /*aNameOfRule*/ )
    {
    User::Leave(KErrNotSupported);
    return 0;
    }

// ----------------------------------------------------------------------------
// CSatelliteInfoUI::HandleForegroundEventL
// ----------------------------------------------------------------------------
//
EXPORT_C void CSatelliteInfoUI::HandleForegroundEventL(TBool /*aForeground*/)
    {
    User::Leave(KErrNotSupported);
    }

// ----------------------------------------------------------------------------
// CSatelliteInfoUI::SetLaunchView
// ----------------------------------------------------------------------------
//
EXPORT_C void CSatelliteInfoUI::SetLaunchView(TSatelliteView /*aLaunchView*/)
	{
	
	}
// End of File
