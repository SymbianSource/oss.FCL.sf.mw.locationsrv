/*
* Copyright (c) 2004-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of CTestProviderDocument
*
*/


#include "testproviderappui.h"
#include "testproviderdocument.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CTestProviderDocument::CTestProviderDocument(CEikApplication& aApp) 
    : CAknDocument(aApp) 
    {
    }   

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CTestProviderDocument::ConstructL()
    {
    }    

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CTestProviderDocument* CTestProviderDocument::NewL( CEikApplication& aApp )
    {
    CTestProviderDocument* self = new (ELeave) CTestProviderDocument(aApp);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CTestProviderDocument::~CTestProviderDocument()
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CEikAppUi* CTestProviderDocument::CreateAppUiL()
    {
    // Create the application user interface, and return a pointer to it,
    // the framework takes ownership of this object
    CEikAppUi* appUi = new (ELeave) CTestProviderAppUi;
    return appUi;
    }


