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
* Description:  Implementation for document class of Settings UI Server
*
*/


#include <eikappui.h>

#include "locsettingsuisrvdocument.h"
#include "locsettingsuisrvappui.h"
#include "locsysuiengine.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CLocSettingsUISrvDoc::CLocSettingsUISrvDoc
// ---------------------------------------------------------------------------
//
CLocSettingsUISrvDoc::CLocSettingsUISrvDoc(CEikApplication &aApp)
    :CAknDocument(aApp)
    {
    }


// ---------------------------------------------------------------------------
// CLocSettingsUISrvDoc::CLocSettingsUISrvDoc
// ---------------------------------------------------------------------------
//
CLocSettingsUISrvDoc::~CLocSettingsUISrvDoc()
    {
    delete iEngine;
    }


// ---------------------------------------------------------------------------
// CLocSettingsUISrvDoc::SetEngineInstance(CLocSysUiEngine* aEngine)
// ---------------------------------------------------------------------------
//
void CLocSettingsUISrvDoc::SetEngineInstance(CLocSysUiEngine* aEngine)
    {
    iEngine = aEngine;
    }


// ---------------------------------------------------------------------------
// CLocSettingsUISrvDoc::Engine()
// ---------------------------------------------------------------------------
//
CLocSysUiEngine* CLocSettingsUISrvDoc::Engine()
    {
    return iEngine;
    }


// ---------------------------------------------------------------------------
// CLocSettingsUISrvDoc::CreateAppUiL
// ---------------------------------------------------------------------------
//
CEikAppUi* CLocSettingsUISrvDoc::CreateAppUiL()
    {
    return new(ELeave) CLocSettingsUISrvAppUi;
    }

//End of file
