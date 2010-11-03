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
* Description: Main View for map and navigation test provider
*
*/



#include <coemain.h>
#include <aknlists.h>
#include <barsread.h>

#include "testprovidernaviview.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CTestProviderNaviView* CTestProviderNaviView::NewL( const TRect& aRect )
    {
    CTestProviderNaviView* self = CTestProviderNaviView::NewLC( aRect );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CTestProviderNaviView* CTestProviderNaviView::NewLC( const TRect& aRect )
    {
    CTestProviderNaviView* self = new ( ELeave ) CTestProviderNaviView;
    CleanupStack::PushL( self );
    self->ConstructL( aRect );
    return self;
    }

// -----------------------------------------------------------------------------
// CTestProviderNaviView::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CTestProviderNaviView::ConstructL( const TRect& aRect )
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
CTestProviderNaviView::CTestProviderNaviView()
    { 
    }


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CTestProviderNaviView::~CTestProviderNaviView()
    {
    }


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CTestProviderNaviView::Draw( const TRect& /*aRect*/ ) const
    {
    // Get the standard graphics context
    CWindowGc& gc = SystemGc();

    // Gets the control's extent
    TRect rect = Rect();

    // Clears the screen
    gc.Clear( rect );
    }

