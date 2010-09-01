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
* Description:  Implementation of CTestProviderApplication
*
*/


#include "debug.h"

#include "testproviderdocument.h"
#include "testproviderappserver.h"
#include "testproviderapplication.h"

// UID for the application, this should match the uid defined in the mmp file.
static const TUid KUidTestProviderApp = {0x0313200B};

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CApaDocument* CTestProviderApplication::CreateDocumentL()
    {  
    LOG( "CreateDocumentL" );
    CApaDocument* document = CTestProviderDocument::NewL( *this );
    return document;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TUid CTestProviderApplication::AppDllUid() const
    {
    // Return the UID for the TestProvider application
    return KUidTestProviderApp;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CTestProviderApplication::NewAppServerL( CApaAppServer*& aAppServer )
    {
    LOG( "NewAppServerL" );
    
    CTestProviderAppServer* server = new (ELeave) CTestProviderAppServer;
    aAppServer = server;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CTestProviderApplication::~CTestProviderApplication()
    {
    }

