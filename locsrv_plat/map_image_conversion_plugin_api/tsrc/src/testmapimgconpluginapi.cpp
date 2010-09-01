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
* Description:  Ctestmapimgconpluginapi class
*
*/



// INCLUDE FILES
#include <Stiftestinterface.h>
#include "testmapimgconpluginapi.h"
#include <SettingServerClient.h>


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// Ctestmapimgconpluginapi::Ctestmapimgconpluginapi
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
Ctestmapimgconpluginapi::Ctestmapimgconpluginapi( 
    CTestModuleIf& aTestModuleIf ):
        CScriptBase( aTestModuleIf )
    {
    }

// -----------------------------------------------------------------------------
// Ctestmapimgconpluginapi::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void Ctestmapimgconpluginapi::ConstructL()
    {
    //Read logger settings to check whether test case name is to be
    //appended to log file name.
    RSettingServer settingServer;
    TInt ret = settingServer.Connect();
    if(ret != KErrNone)
        {
        User::Leave(ret);
        }
    // Struct to StifLogger settigs.
    TLoggerSettings loggerSettings; 
    // Parse StifLogger defaults from STIF initialization file.
    ret = settingServer.GetLoggerSettings(loggerSettings);
    if(ret != KErrNone)
        {
        User::Leave(ret);
        } 
    // Close Setting server session
    settingServer.Close();

    TFileName logFileName;
    
    if(loggerSettings.iAddTestCaseTitle)
        {
        TName title;
        TestModuleIf().GetTestCaseTitleL(title);
        logFileName.Format(KtestmapimgconpluginapiLogFileWithTitle, &title);
        }
    else
        {
        logFileName.Copy(KtestmapimgconpluginapiLogFile);
        }

    iLog = CStifLogger::NewL( KtestmapimgconpluginapiLogPath, 
                          logFileName,
                          CStifLogger::ETxt,
                          CStifLogger::EFile,
                          EFalse );

    }

// -----------------------------------------------------------------------------
// Ctestmapimgconpluginapi::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
Ctestmapimgconpluginapi* Ctestmapimgconpluginapi::NewL( 
    CTestModuleIf& aTestModuleIf )
    {
    Ctestmapimgconpluginapi* self = new (ELeave) Ctestmapimgconpluginapi( aTestModuleIf );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;

    }

// Destructor
Ctestmapimgconpluginapi::~Ctestmapimgconpluginapi()
    { 

    // Delete resources allocated from test methods
    Delete();

    // Delete logger
    delete iLog; 

    }
    

// ========================== OTHER EXPORTED FUNCTIONS =========================

// -----------------------------------------------------------------------------
// LibEntryL is a polymorphic Dll entry point.
// Returns: CScriptBase: New CScriptBase derived object
// -----------------------------------------------------------------------------
//
EXPORT_C CScriptBase* LibEntryL( 
    CTestModuleIf& aTestModuleIf ) // Backpointer to STIF Test Framework
    {

    return ( CScriptBase* ) Ctestmapimgconpluginapi::NewL( aTestModuleIf );

    }


//  End of File
