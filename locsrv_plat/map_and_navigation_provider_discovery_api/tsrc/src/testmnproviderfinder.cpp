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
* Description: Provides testing of MnProviderFinder class methods
*
*/



// INCLUDE FILES

#include "testmnproviderfinder.h"
#include <MnProviderFinder.h>
#include <MnProvider.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// TestMnProviderFinder::TestMnProviderFinder
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
TestMnProviderFinder::TestMnProviderFinder( CStifLogger* aLog ) 
	{
	iLog = aLog;
	}

// -----------------------------------------------------------------------------
// TestMnProviderFinder::NewL
// 
// 
// -----------------------------------------------------------------------------
//	
TestMnProviderFinder* TestMnProviderFinder::NewL(CStifLogger* aLog)
	{
	TestMnProviderFinder* self = new (ELeave) TestMnProviderFinder( aLog );
	CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); 
    return self; 
	}

// -----------------------------------------------------------------------------
// TestMnProviderFinder::ConstructL
// 
// 
// -----------------------------------------------------------------------------
//
void TestMnProviderFinder::ConstructL()
	{
	
	}	

// -----------------------------------------------------------------------------
// TestMnProviderFinder::~TestMnProviderFinder
// 
// 
// -----------------------------------------------------------------------------
//
TestMnProviderFinder::~TestMnProviderFinder()
	{
	
	}
	

// -----------------------------------------------------------------------------
// TestMnProviderFinder::FindAllProvidersL
// 
// 
// -----------------------------------------------------------------------------
//
TInt TestMnProviderFinder::FindAllProvidersL( CStifItemParser& /*aItem*/ )
	{
	RPointerArray<CMnProvider> providerArray;
	MnProviderFinder::FindProvidersL( providerArray );
	
	providerArray.ResetAndDestroy();
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// TestMnProviderFinder::FindSelectedProvidersL
// 
// 
// -----------------------------------------------------------------------------
//
TInt TestMnProviderFinder::FindSelectedProvidersL( CStifItemParser& /*aItem*/ )
	{
	
	RPointerArray<CMnProvider> providerArray;
	MnProviderFinder::FindProvidersL( providerArray, CMnProvider::EServiceNavigation | CMnProvider::EServiceMapView );
	
	providerArray.ResetAndDestroy();
	return KErrNone;
	}	

