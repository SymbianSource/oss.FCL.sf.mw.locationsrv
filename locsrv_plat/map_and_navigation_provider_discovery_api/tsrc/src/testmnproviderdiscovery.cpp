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
* Description: Implementation of test class for map_and_navigation_provider_discovery_api
*
*/



// INCLUDE FILES
#include <Stiftestinterface.h>
#include <swinstapi.h>
#include "testmnproviderdiscovery.h"

// Literal
// Provider app sisx path
_LIT( KProviderSisxPath, "e:\\testing\\data\\mnrefprovider.sisx" ); 

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CTestMnProviderDiscovery::Ctestmnproviderdiscovery
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CTestMnProviderDiscovery::CTestMnProviderDiscovery( 
    CTestModuleIf& aTestModuleIf ):
        CScriptBase( aTestModuleIf )
    {
    }

// -----------------------------------------------------------------------------
// CTestMnProviderDiscovery::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CTestMnProviderDiscovery::ConstructL()
    {
    iLog = CStifLogger::NewL( KtestmnproviderdiscoveryLogPath, 
                          KtestmnproviderdiscoveryLogFile,
                          CStifLogger::ETxt,
                          CStifLogger::EFile,
                          EFalse );
	InstallProvider( KProviderSisxPath );
    }

// -----------------------------------------------------------------------------
// CTestMnProviderDiscovery::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CTestMnProviderDiscovery* CTestMnProviderDiscovery::NewL( 
    CTestModuleIf& aTestModuleIf )
    {
    CTestMnProviderDiscovery* self = new (ELeave) CTestMnProviderDiscovery( aTestModuleIf );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;

    }

// Destructor
CTestMnProviderDiscovery::~CTestMnProviderDiscovery()
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

    return ( CScriptBase* ) CTestMnProviderDiscovery::NewL( aTestModuleIf );

    }

// -----------------------------------------------------------------------------
// This method will install the provider application
//
// -----------------------------------------------------------------------------
//   
TInt CTestMnProviderDiscovery::InstallProvider( const TDesC& aPackagePath )
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
