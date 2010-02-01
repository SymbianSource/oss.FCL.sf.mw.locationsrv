/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Application to launch Positioning Settings for Location Centre
*
*/


// User Includes
#include "locpossettingsappdocument.h"
#include "locpossettingsappui.h"


// ======== Member functions for class  CLocPosSettingsAppDocument ===========

// ---------------------------------------------------------------------------
// CLocPosSettingsAppDocument::NewL()
// ---------------------------------------------------------------------------
//
CLocPosSettingsAppDocument* CLocPosSettingsAppDocument::NewL(
        CEikApplication& aApp)     
    {
    CLocPosSettingsAppDocument* self = new (ELeave) CLocPosSettingsAppDocument( aApp );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CLocPosSettingsAppDocument::~CLocPosSettingsAppDocument()
// ---------------------------------------------------------------------------
//
CLocPosSettingsAppDocument::~CLocPosSettingsAppDocument()
    {
    // No Implementation  
    }


// ---------------------------------------------------------------------------
// CLocPosSettingsAppDocument::CLocPosSettingsAppDocument()
// Constructs.
// ---------------------------------------------------------------------------
//
CLocPosSettingsAppDocument::CLocPosSettingsAppDocument( CEikApplication& aApp )
    :CAknDocument( aApp )    
    {
    }

// ---------------------------------------------------------------------------
// CLocPosSettingsAppDocument::ConstructL()
// ---------------------------------------------------------------------------
//
void CLocPosSettingsAppDocument::ConstructL()
    {
    // No Implementation
    }

// ----------------------------------------------------
// CLocPosSettingsAppDocument::CreateAppUiL()
// ----------------------------------------------------
//
CEikAppUi* CLocPosSettingsAppDocument::CreateAppUiL()
    {
    return new ( ELeave ) CLosPosSettingsAppUi;
    }

