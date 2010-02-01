/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  GPS indicator plugin implementation
*
*/


// USER INCLUDES
#include "locgpsindicatorplugin.h"
#include "locgpsindicatorpluginuids.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CLocGpsIndicatorPlugin::CLocGpsIndicatorPlugin
// ---------------------------------------------------------------------------
//
CLocGpsIndicatorPlugin::CLocGpsIndicatorPlugin()
    {
    }

// ---------------------------------------------------------------------------
// CLocGpsIndicatorPlugin::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CLocGpsIndicatorPlugin::ConstructL()
    {
    }

// -----------------------------------------------------------------------------
// CLocGpsIndicatorPlugin::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CLocGpsIndicatorPlugin* CLocGpsIndicatorPlugin::NewL()
    {
    CLocGpsIndicatorPlugin* self = 
        new( ELeave ) CLocGpsIndicatorPlugin;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
//
CLocGpsIndicatorPlugin::~CLocGpsIndicatorPlugin()
    {
    }


// ---------------------------------------------------------------------------
// CLocGpsIndicatorPlugin::HandleIndicatorTapL
// ---------------------------------------------------------------------------
//
void CLocGpsIndicatorPlugin::HandleIndicatorTapL( const TInt /*aUid*/ )
    {
    }

// ---------------------------------------------------------------------------
// CLocGpsIndicatorPlugin::ProcessCommandL
// ---------------------------------------------------------------------------
//
void CLocGpsIndicatorPlugin::ProcessCommandL( TInt /* aCommandId */)
    {
    }
// ---------------------------------------------------------------------------
// CLocGpsIndicatorPlugin::HandlePreviewPopUpEventL
// ---------------------------------------------------------------------------
//
void CLocGpsIndicatorPlugin::HandlePreviewPopUpEventL( CAknPreviewPopUpController* /*aController*/,
            										   TPreviewPopUpEvent /*aEvent*/ )
    {
    }
    
//  End of File
