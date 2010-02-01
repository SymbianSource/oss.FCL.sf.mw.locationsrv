/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Implementation of test class for map_and_navigation_api
*
*/



// INCLUDE FILES
#include <Stiftestinterface.h>
#include <swinstapi.h>
#include "testmapandnavapi.h"

// Literal
// Provider app sisx path
_LIT( KProviderSisxPath, "e:\\testing\\data\\mnrefprovider.sisx" ); 



// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CTestMapAndNavApi::CTestMapAndNavApi
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CTestMapAndNavApi::CTestMapAndNavApi( 
    CTestModuleIf& aTestModuleIf ):
        CScriptBase( aTestModuleIf )
    {
    }

// -----------------------------------------------------------------------------
// CTestMapAndNavApi::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CTestMapAndNavApi::ConstructL()
    {
    iLog = CStifLogger::NewL( KtestmapandnavapiLogPath, 
                          KtestmapandnavapiLogFile,
                          CStifLogger::ETxt,
                          CStifLogger::EFile,
                          EFalse );
	InstallProvider( KProviderSisxPath );
    }

// -----------------------------------------------------------------------------
// CTestMapAndNavApi::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CTestMapAndNavApi* CTestMapAndNavApi::NewL( 
    CTestModuleIf& aTestModuleIf )
    {
    CTestMapAndNavApi* self = new (ELeave) CTestMapAndNavApi( aTestModuleIf );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;

    }

// Destructor
CTestMapAndNavApi::~CTestMapAndNavApi()
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

    return ( CScriptBase* ) CTestMapAndNavApi::NewL( aTestModuleIf );

    }
    
// -----------------------------------------------------------------------------
// This method will install the provider application
//
// -----------------------------------------------------------------------------
//   
TInt CTestMapAndNavApi::InstallProvider( const TDesC& aPackagePath )
    {
   // iLog->Log("Installing [%S]...", aPackagePath );

    TInt err = KErrNone;
    SwiUI::RSWInstSilentLauncher installer;

    err = installer.Connect();
    if( KErrNone != err )
    	{
    	iLog->Log(_L("Connection to installer failed"));
    	return err;
    	}
    //CHECK_EQUAL( err, KErrNone, "Connection to installer failed" );

    SwiUI::TInstallOptionsPckg options;
    options().iUpgrade = SwiUI::EPolicyAllowed;
    options().iPackageInfo = SwiUI::EPolicyAllowed;
    options().iOverwrite = SwiUI::EPolicyAllowed;
    options().iKillApp = SwiUI::EPolicyAllowed;

    err = installer.SilentInstall( aPackagePath, options );
    if( KErrNone != err )
    	{
    	iLog->Log(_L("Provider Installation failed"));
    	installer.Close();
    	return err;
    	}
    	
    //CHECK_EQUAL( err, KErrNone, "Installation failed" );
	iLog->Log(_L("Provider Installation passed"));
    installer.Close();
    return err;
    }


//  End of File
