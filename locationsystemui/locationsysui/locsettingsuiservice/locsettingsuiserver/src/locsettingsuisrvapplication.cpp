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
* Description:  Implementation for application class of Location Settings UI
*                Server
*
*/


#include <eikstart.h>
#include <eikenv.h>

#include "locsettingsuisrvapplication.h"
#include "locsettingsuisrvuid.hrh"
#include "locsettingsuisrvdocument.h"
#include "locsettingsuiappserver.h"
#include "locsettingsuiserverinterface.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CLocSettingsUISrvApp::NewAppServerL
// ---------------------------------------------------------------------------
//
void CLocSettingsUISrvApp::NewAppServerL( CApaAppServer*& aAppServer )
	{
    aAppServer = new (ELeave) CLocSettingsUIAppServer();
    if ( !CEikonEnv::Static()->StartedAsServerApp() )
        {
        // In case we are running as server process for
        // framework UI requests, we need to destroy the Server object.
        // Hence store the created server object in this Application class
        // and delete in destructor.
        iAppServer = aAppServer;
        }
	}
	

// ---------------------------------------------------------------------------
// virtual Destructor
// ---------------------------------------------------------------------------
//
CLocSettingsUISrvApp::~CLocSettingsUISrvApp()
    {
    // if the server is running in embedded mode iAppServer is NULL, but
    // won't create any problem.
    delete iAppServer;
    }


// ---------------------------------------------------------------------------
// CLocSettingsUISrvApp::CreateDocumentL
// ---------------------------------------------------------------------------
//
CApaDocument* CLocSettingsUISrvApp::CreateDocumentL()
    {
	return new (ELeave) CLocSettingsUISrvDoc(*this);
    }


// ---------------------------------------------------------------------------
// CLocSettingsUISrvApp::AppDllUid
// ---------------------------------------------------------------------------
//
TUid CLocSettingsUISrvApp::AppDllUid() const
    {
    return TUid::Uid(LOCSETTINGSUISERVER_UID3);
    }


// ======== GLOBAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructs and returns an application object.
// ---------------------------------------------------------------------------
//
EXPORT_C CApaApplication* NewApplication()
    {
    CLocSettingsUISrvApp *app = new CLocSettingsUISrvApp;
    
    if ( (!CEikonEnv::Static()->StartedAsServerApp()) && 
         (app != NULL) )
        {
        // If the appliciation is not invoked with the Server differentiator
        // parameter then the NewAppserverL and the ConstructL need to be
        // called from here.
        // Ignore error cases. The server object is owned by the Application
        // class. Hence it will be destructoed in the Application class
        // destructor.
        CApaAppServer* appServer = NULL;
        TRAP_IGNORE(app->NewAppServerL(appServer);
                    appServer->ConstructL(KLocSettingsUiServerName()););
        }

    return app;
    }


// ---------------------------------------------------------------------------
// Main function of the application executable.
// ---------------------------------------------------------------------------
//
GLDEF_C TInt E32Main()
    {
    return EikStart::RunApplication( NewApplication );
    }


//End of file
