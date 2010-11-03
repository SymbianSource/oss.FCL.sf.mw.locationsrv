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
* Description: Provides testing of CMnProvider class methods
*
*/



// INCLUDE FILES

#include "testcmnprovider.h"
#include <MnProviderFinder.h>
#include <MnProvider.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CTestMnProvider::CTestMnNavigator
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CTestMnProvider::CTestMnProvider( CStifLogger* aLog ) 
	{
	iLog = aLog;
	}

// -----------------------------------------------------------------------------
// CTestMnProvider::NewL
// 
// 
// -----------------------------------------------------------------------------
//	
CTestMnProvider* CTestMnProvider::NewL(CStifLogger* aLog)
	{
	CTestMnProvider* self = new (ELeave) CTestMnProvider( aLog );
	CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); 
    return self; 
	}

// -----------------------------------------------------------------------------
// CTestMnProvider::ConstructL
// 
// 
// -----------------------------------------------------------------------------
//
void CTestMnProvider::ConstructL()
	{
	
	}	

// -----------------------------------------------------------------------------
// CTestMnProvider::~CTestMnNavigator
// 
// 
// -----------------------------------------------------------------------------
//
CTestMnProvider::~CTestMnProvider()
	{
	
	}
	
// -----------------------------------------------------------------------------
// CTestMnProvider::DestroyProviderL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMnProvider::DestroyProviderL( CStifItemParser& /*aItem*/ )
	{
	// Find Providers
	RPointerArray<CMnProvider> providerArray;
	MnProviderFinder::FindProvidersL( providerArray );
	if( providerArray.Count() < 1 )
		{
		iLog->Log(_L("No Provider Found, can't test DestroyProviderL"));
		User::Leave( KErrNotFound );
		}
	// Will delete all the provider objects	
	providerArray.ResetAndDestroy();
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// CTestMnProvider::ProviderUidL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMnProvider::ProviderUidL( CStifItemParser& /*aItem*/ )
	{
	// Find Providers
	RPointerArray<CMnProvider> providerArray;
	MnProviderFinder::FindProvidersL( providerArray );
	if( providerArray.Count() < 1 )
		{
		iLog->Log(_L("No Provider Found, can't test ProviderUidL"));
		User::Leave( KErrNotFound );
		}
	// Get Uid of first provider
	TUid providerUid = providerArray.operator[](0)->Uid();
	providerArray.ResetAndDestroy();
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// CTestMnProvider::ProviderVersionL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMnProvider::ProviderVersionL( CStifItemParser& /*aItem*/ )
	{
	// Find Providers
	RPointerArray<CMnProvider> providerArray;
	MnProviderFinder::FindProvidersL( providerArray );
	if( providerArray.Count() < 1 )
		{
		iLog->Log(_L("No Provider Found, can't test ProviderVersionL"));
		User::Leave( KErrNotFound );
		}
	// Get Version of first provider
	TVersion providerVersion = providerArray.operator[](0)->Version();
	providerArray.ResetAndDestroy();
	return KErrNone;
	}	
	
// -----------------------------------------------------------------------------
// CTestMnProvider::GetProviderNameL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMnProvider::GetProviderNameL( CStifItemParser& /*aItem*/ )
	{
	// Find Providers
	RPointerArray<CMnProvider> providerArray;
	MnProviderFinder::FindProvidersL( providerArray );
	if( providerArray.Count() < 1 )
		{
		iLog->Log(_L("No Provider Found, can't test GetProviderNameL"));
		User::Leave( KErrNotFound );
		}
	// Get name of first provider
	TPtrC providerName;
	providerArray.operator[](0)->GetName( providerName );
	iLog->Log( providerName );
	providerArray.ResetAndDestroy();
	return KErrNone;
	}		

// -----------------------------------------------------------------------------
// CTestMnProvider::GetShortNameL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMnProvider::GetShortNameL( CStifItemParser& /*aItem*/ )
	{
	// Find Providers
	RPointerArray<CMnProvider> providerArray;
	MnProviderFinder::FindProvidersL( providerArray );
	if( providerArray.Count() < 1 )
		{
		iLog->Log(_L("No Provider Found, can't test GetShortNameL"));
		User::Leave( KErrNotFound );
		}
	// Get short name of first provider
	TPtrC shortName;
	providerArray.operator[](0)->GetShortName( shortName );
	iLog->Log( shortName );
	providerArray.ResetAndDestroy();
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// CTestMnProvider::GetVendorNameL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMnProvider::GetVendorNameL( CStifItemParser& /*aItem*/ )
	{
	// Find Providers
	RPointerArray<CMnProvider> providerArray;
	MnProviderFinder::FindProvidersL( providerArray );
	if( providerArray.Count() < 1 )
		{
		iLog->Log(_L("No Provider Found, can't test GetVendorNameL"));
		User::Leave( KErrNotFound );
		}
	// Get vendor name of first provider
	TPtrC vendorName;
	providerArray.operator[](0)->GetVendorName( vendorName );
	iLog->Log( vendorName );
	providerArray.ResetAndDestroy();
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// CTestMnProvider::SupportedServicesL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMnProvider::SupportedServicesL( CStifItemParser& /*aItem*/ )
	{
	// Find provider
	RPointerArray<CMnProvider> providerArray;
	MnProviderFinder::FindProvidersL( providerArray );
	if( providerArray.Count() < 1 )
		{
		iLog->Log(_L("No Provider Found, can't test SupportedServicesL"));
		User::Leave( KErrNotFound );
		}
	// Get services supported by first provider
	CMnProvider::TServices supportedServices = providerArray.operator[](0)->SupportedServices();
	providerArray.ResetAndDestroy();
	return KErrNone;
	}	

// -----------------------------------------------------------------------------
// CTestMnProvider::SupportedFeaturesL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMnProvider::SupportedFeaturesL( CStifItemParser& /*aItem*/ )
	{
	// Find provider
	RPointerArray<CMnProvider> providerArray;
	MnProviderFinder::FindProvidersL( providerArray );
	TInt prvCount = providerArray.Count();
	if( prvCount < 1 )
		{
		iLog->Log(_L("No Provider Found, can't test SupportedFeaturesL"));
		User::Leave( KErrNotFound );
		}
	// Get features supported by providers
	for (TInt i=0; i<prvCount; i++)
		{
		CMnProvider::TServices supportedServices = providerArray.operator[](i)->SupportedServices();
		TInt navFeatures = providerArray.operator[](i)->SupportedFeatures( CMnProvider::EServiceNavigation );
		TInt mapFeatures = providerArray.operator[](i)->SupportedFeatures( CMnProvider::EServiceMapView );
		TInt geoFeatures = providerArray.operator[](i)->SupportedFeatures( CMnProvider::EServiceGeocoding );
		}
	
	providerArray.ResetAndDestroy();
	return KErrNone;
	}	
