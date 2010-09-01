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
* Description: Provides testing of CMnNavigator class methods
*
*/



// INCLUDE FILES

#include "testcmnnavigator.h"
#include <MnNavigator.h>
#include <MnProviderFinder.h>
#include <MnProvider.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CTestMnNavigator::CTestMnNavigator
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CTestMnNavigator::CTestMnNavigator( CStifLogger* aLog ) 
	{
	iLog = aLog;
	}

// -----------------------------------------------------------------------------
// CTestMnNavigator::NewL
// 
// 
// -----------------------------------------------------------------------------
//	
CTestMnNavigator* CTestMnNavigator::NewL(CStifLogger* aLog)
	{
	CTestMnNavigator* self = new (ELeave) CTestMnNavigator( aLog );
	CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); 
    return self; 
	}

// -----------------------------------------------------------------------------
// CTestMnNavigator::ConstructL
// 
// 
// -----------------------------------------------------------------------------
//
void CTestMnNavigator::ConstructL()
	{
	
	}	

// -----------------------------------------------------------------------------
// CTestMnNavigator::~CTestMnNavigator
// 
// 
// -----------------------------------------------------------------------------
//
CTestMnNavigator::~CTestMnNavigator()
	{
	
	}
	

// -----------------------------------------------------------------------------
// CTestMnNavigator::CreateNavigatorL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMnNavigator::CreateNavigatorL( CStifItemParser& /*aItem*/ )
	{
	// Find one provider to be used by navigator
	RPointerArray<CMnProvider> providerArray;
	MnProviderFinder::FindProvidersL( providerArray, CMnProvider::EServiceNavigation );
	if( providerArray.Count() < 1 )
		{
		// No provider found
		iLog->Log(_L("Provider not found, CreateNavigatorL fails"));
		User::Leave( KErrNotFound );
		}
	// Use first provider
	CMnNavigator* navigator = CMnNavigator::NewL( *providerArray.operator[](0) );
	delete navigator;
	providerArray.ResetAndDestroy();
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// CTestMnNavigator::CreateChainedNavigatorL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMnNavigator::CreateChainedNavigatorL( CStifItemParser& /*aItem*/ )
	{
	// Find one provider to be used by navigator
	RPointerArray<CMnProvider> providerArray;
	MnProviderFinder::FindProvidersL( providerArray );
	TUid Uid;
    Uid.iUid = 0x03132014;
    TInt i = 0;
    for( i=0; i<providerArray.Count(); i++ )
    	{
    	if((providerArray.operator[](i))->Uid() == Uid)
    		{
    		break;
    		}			
    	}
    	
	if( i >= providerArray.Count() )
		{
		// No provider found
		iLog->Log(_L("Provider not found, CreateNavigatorL fails"));
		User::Leave( KErrNotFound );
		}
	// Use second provider
	CMnNavigator* navigator = CMnNavigator::NewChainedL( *providerArray.operator[](i) );
	delete navigator;
	for( i=0; i < providerArray.Count(); i++ )
		{
		iLog->Log(_L("One more"));
		delete  providerArray.operator[](i);	
		}
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// CTestMnNavigator::NavigateToL
// 
// Not being tested, user input is required to close the provider app
// -----------------------------------------------------------------------------
//
TInt CTestMnNavigator::NavigateToL( CStifItemParser& /*aItem*/ )
	{
	// Find one provider to be used by navigator
	RPointerArray<CMnProvider> providerArray;
	MnProviderFinder::FindProvidersL( providerArray, CMnProvider::EServiceNavigation );
	if( providerArray.Count() < 1 )
		{
		// No provider found
		iLog->Log(_L("Provider not found, CreateNavigatorL fails"));
		User::Leave( KErrNotFound );
		}
	// Use first provider
	CMnNavigator* navigator = CMnNavigator::NewL( *providerArray.operator[](0) );
	CleanupStack::PushL( navigator );
	// Create landmark
	CPosLandmark* landmark = CPosLandmark::NewL();
	CleanupStack::PushL( landmark );
	TLocality position;
	position.SetCoordinate( 45.56, 78.002 );
	// Set position
	landmark->SetPositionL( position );
	// Navigate
	navigator->NavigateToL( *landmark );
	CleanupStack::PopAndDestroy( 2, navigator );
	providerArray.ResetAndDestroy();
	return KErrNone;
	}	

// -----------------------------------------------------------------------------
// CTestMnNavigator::SetExitObserverL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMnNavigator::SetExitObserverL( CStifItemParser& /*aItem*/ )
	{
	// Find one provider to be used by navigator
	RPointerArray<CMnProvider> providerArray;
	MnProviderFinder::FindProvidersL( providerArray, CMnProvider::EServiceNavigation );
	if( providerArray.Count() < 1 )
		{
		// No provider found
		iLog->Log(_L("Provider not found, CreateNavigatorL fails"));
		User::Leave( KErrNotFound );
		}
	// Use first provider
	CMnNavigator* navigator = CMnNavigator::NewL( *providerArray.operator[](0) );
	CleanupStack::PushL( navigator );
	// Set exit observer
	navigator->SetExitObserverL( *this );
	//
	CleanupStack::PopAndDestroy( navigator );
	providerArray.ResetAndDestroy();
	return KErrNone;
	}	
	
// -----------------------------------------------------------------------------
// CTestMnNavigator::RemoveExitObserverL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMnNavigator::RemoveExitObserverL( CStifItemParser& /*aItem*/ )
	{
	// Find one provider to be used by navigator
	RPointerArray<CMnProvider> providerArray;
	MnProviderFinder::FindProvidersL( providerArray, CMnProvider::EServiceNavigation );
	if( providerArray.Count() < 1 )
		{
		// No provider found
		iLog->Log(_L("Provider not found, CreateNavigatorL fails"));
		User::Leave( KErrNotFound );
		}
	// Use first provider
	CMnNavigator* navigator = CMnNavigator::NewL( *providerArray.operator[](0) );
	CleanupStack::PushL( navigator );
	// Set exit observer
	navigator->SetExitObserverL( *this );
	// Remove exit observer
	navigator->RemoveExitObserver();
	CleanupStack::PopAndDestroy( navigator );
	providerArray.ResetAndDestroy();
	return KErrNone;
	}		


// -----------------------------------------------------------------------------
// CTestMnNavigator::HandleServerAppExit
// 
// 
// -----------------------------------------------------------------------------
//
void CTestMnNavigator::HandleServerAppExit( TInt /*aReason*/ )
	{
	iLog->Log(_L("Provider exits"));
	}

