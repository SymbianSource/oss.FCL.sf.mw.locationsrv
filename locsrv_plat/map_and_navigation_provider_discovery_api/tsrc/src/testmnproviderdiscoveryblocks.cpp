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
* Description: Entry point for test cases in stif module
*
*/



// INCLUDE FILES
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include "testmnproviderdiscovery.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CTestMnProviderDiscovery::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CTestMnProviderDiscovery::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// CTestMnProviderDiscovery::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CTestMnProviderDiscovery::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Provider Discovery
        ENTRY( "FindAllProviders", CTestMnProviderDiscovery::FindAllProvidersL ),
        ENTRY( "FindSelectedProviders", CTestMnProviderDiscovery::FindSelectedProvidersL ),
        // Provider
        ENTRY( "DestroyProvider", CTestMnProviderDiscovery::DestroyProviderL ),
        ENTRY( "ProviderUid", CTestMnProviderDiscovery::ProviderUidL ),
        ENTRY( "ProviderVersion", CTestMnProviderDiscovery::ProviderVersionL ),
        ENTRY( "GetProviderName", CTestMnProviderDiscovery::GetProviderNameL ),
        ENTRY( "GetShortName", CTestMnProviderDiscovery::GetShortNameL ),
        ENTRY( "GetVendorName", CTestMnProviderDiscovery::GetVendorNameL ),
        ENTRY( "SupportedServices", CTestMnProviderDiscovery::SupportedServicesL ),
        ENTRY( "SupportedFeatures", CTestMnProviderDiscovery::SupportedFeaturesL ),
        
        //ADD NEW ENTRY HERE

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );
	
	TInt ret = KErrNone;
	TRAPD( error, ret = RunInternalL( KFunctions, count, aItem ));
	if ( error != KErrNone )
		{
		return error;
		}
	return ret;	
	    
    }

// -----------------------------------------------------------------------------
// CTestMnProviderDiscovery::FindAllProvidersL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMnProviderDiscovery::FindAllProvidersL( CStifItemParser& aItem )
    {
    TestMnProviderFinder* providerFinder = TestMnProviderFinder::NewL( iLog );
	CleanupStack::PushL( providerFinder );
	TInt err = providerFinder->FindAllProvidersL( aItem );
	CleanupStack::PopAndDestroy( providerFinder );
	return err;	
    }
    
// -----------------------------------------------------------------------------
// CTestMnProviderDiscovery::FindSelectedProvidersL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMnProviderDiscovery::FindSelectedProvidersL( CStifItemParser& aItem )
    {
    TestMnProviderFinder* providerFinder = TestMnProviderFinder::NewL( iLog );
	CleanupStack::PushL( providerFinder );
	TInt err = providerFinder->FindSelectedProvidersL( aItem );
	CleanupStack::PopAndDestroy( providerFinder );
	return err;	
    }    

// -----------------------------------------------------------------------------
// CTestMnProviderDiscovery::DestroyProviderL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMnProviderDiscovery::DestroyProviderL( CStifItemParser& aItem )
    {
    CTestMnProvider* testProvider = CTestMnProvider::NewL( iLog );
	CleanupStack::PushL( testProvider );
	TInt err = testProvider->DestroyProviderL( aItem );
	CleanupStack::PopAndDestroy( testProvider );
	return err;	
    }    

// -----------------------------------------------------------------------------
// CTestMnProviderDiscovery::ProviderUidL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMnProviderDiscovery::ProviderUidL( CStifItemParser& aItem )
    {
    CTestMnProvider* testProvider = CTestMnProvider::NewL( iLog );
	CleanupStack::PushL( testProvider );
	TInt err = testProvider->ProviderUidL( aItem );
	CleanupStack::PopAndDestroy( testProvider );
	return err;	
    }
    
// -----------------------------------------------------------------------------
// CTestMnProviderDiscovery::ProviderVersionL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMnProviderDiscovery::ProviderVersionL( CStifItemParser& aItem )
    {
    CTestMnProvider* testProvider = CTestMnProvider::NewL( iLog );
	CleanupStack::PushL( testProvider );
	TInt err = testProvider->ProviderVersionL( aItem );
	CleanupStack::PopAndDestroy( testProvider );
	return err;	
    }  
    
// -----------------------------------------------------------------------------
// CTestMnProviderDiscovery::GetProviderNameL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMnProviderDiscovery::GetProviderNameL( CStifItemParser& aItem )
    {
    CTestMnProvider* testProvider = CTestMnProvider::NewL( iLog );
	CleanupStack::PushL( testProvider );
	TInt err = testProvider->GetProviderNameL( aItem );
	CleanupStack::PopAndDestroy( testProvider );
	return err;	
    }      
// -----------------------------------------------------------------------------
// CTestMnProviderDiscovery::GetShortNameL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMnProviderDiscovery::GetShortNameL( CStifItemParser& aItem )
    {
    CTestMnProvider* testProvider = CTestMnProvider::NewL( iLog );
	CleanupStack::PushL( testProvider );
	TInt err = testProvider->GetShortNameL( aItem );
	CleanupStack::PopAndDestroy( testProvider );
	return err;	
    }
    
// -----------------------------------------------------------------------------
// CTestMnProviderDiscovery::GetVendorNameL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMnProviderDiscovery::GetVendorNameL( CStifItemParser& aItem )
    {
    CTestMnProvider* testProvider = CTestMnProvider::NewL( iLog );
	CleanupStack::PushL( testProvider );
	TInt err = testProvider->GetVendorNameL( aItem );
	CleanupStack::PopAndDestroy( testProvider );
	return err;	
    }          

// -----------------------------------------------------------------------------
// CTestMnProviderDiscovery::SupportedServicesL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMnProviderDiscovery::SupportedServicesL( CStifItemParser& aItem )
    {
    CTestMnProvider* testProvider = CTestMnProvider::NewL( iLog );
	CleanupStack::PushL( testProvider );
	TInt err = testProvider->SupportedServicesL( aItem );
	CleanupStack::PopAndDestroy( testProvider );
	return err;	
    }          

// -----------------------------------------------------------------------------
// CTestMnProviderDiscovery::SupportedFeaturesL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMnProviderDiscovery::SupportedFeaturesL( CStifItemParser& aItem )
    {
    CTestMnProvider* testProvider = CTestMnProvider::NewL( iLog );
	CleanupStack::PushL( testProvider );
	TInt err = testProvider->SupportedFeaturesL( aItem );
	CleanupStack::PopAndDestroy( testProvider );
	return err;	
    }          

//  End of File
