/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Map View for map and navigation test provider
*
*/



#include <coemain.h>
#include <aknlists.h>
#include <barsread.h>

#include "debug.h"
#include "testprovidermapview.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CTestProviderMapView* CTestProviderMapView::NewL( const TRect& aRect )
    {
    CTestProviderMapView* self = CTestProviderMapView::NewLC( aRect );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CTestProviderMapView* CTestProviderMapView::NewLC( const TRect& aRect )
    {
    CTestProviderMapView* self = new ( ELeave ) CTestProviderMapView;
    CleanupStack::PushL( self );
    self->ConstructL( aRect );
    return self;
    }

// -----------------------------------------------------------------------------
// CTestProviderMapView::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CTestProviderMapView::ConstructL( const TRect& aRect )
    {
    // Create a window for this application view
    CreateWindowL();

    // Set the windows size
    SetRect( aRect );

    // Activate the window, which makes it ready to be drawn
    ActivateL();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CTestProviderMapView::CTestProviderMapView()
    {
    }


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CTestProviderMapView::~CTestProviderMapView()
    {
    }


// -----------------------------------------------------------------------------
// CTestProviderMapView::Draw()
// Draws the display.
// -----------------------------------------------------------------------------
//
void CTestProviderMapView::Draw( const TRect& /*aRect*/ ) const
    {
    // Get the standard graphics context
    CWindowGc& gc = SystemGc();

    // Gets the control's extent
    TRect rect = Rect();

    // Clears the screen
    gc.Clear( rect );
    }

