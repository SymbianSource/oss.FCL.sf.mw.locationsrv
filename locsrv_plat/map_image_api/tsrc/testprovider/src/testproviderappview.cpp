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
* Description: Application View for map and navigation test provider
*
*/


#include <coemain.h>
#include <aknlists.h>
#include <barsread.h>

#include "testproviderappview.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CTestProviderAppView::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CTestProviderAppView* CTestProviderAppView::NewL( const TRect& aRect )
    {
    CTestProviderAppView* self = CTestProviderAppView::NewLC( aRect );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CTestProviderAppView::NewLC()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CTestProviderAppView* CTestProviderAppView::NewLC( const TRect& aRect )
    {
    CTestProviderAppView* self = new ( ELeave ) CTestProviderAppView;
    CleanupStack::PushL( self );
    self->ConstructL( aRect );
    return self;
    }

// -----------------------------------------------------------------------------
// CTestProviderAppView::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CTestProviderAppView::ConstructL( const TRect& aRect )
    {
    // Create a window for this application view
    CreateWindowL();

    // Set the windows size
    SetRect( aRect );

    // Activate the window, which makes it ready to be drawn
    ActivateL();
    }

// -----------------------------------------------------------------------------
// CTestProviderAppView::CTestProviderAppView()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CTestProviderAppView::CTestProviderAppView()
    {
    // No implementation required
    }


// -----------------------------------------------------------------------------
// CTestProviderAppView::~CTestProviderAppView()
// Destructor.
// -----------------------------------------------------------------------------
//
CTestProviderAppView::~CTestProviderAppView()
    {
    //delete iList;
    //iList = NULL;
    }


// -----------------------------------------------------------------------------
// CTestProviderAppView::Draw()
// Draws the display.
// -----------------------------------------------------------------------------
//
void CTestProviderAppView::Draw( const TRect& /*aRect*/ ) const
    {
    // Get the standard graphics context
    CWindowGc& gc = SystemGc();

    // Gets the control's extent
    TRect rect = Rect();

    // Clears the screen
    gc.Clear( rect );
    }

