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
* Description: Provides testing of CMnMapView class methods
*
*/



// INCLUDE FILES

#include <StifParser.h>
#include <Stiftestinterface.h>
#include "testcmnmapview.h"
#include <MnMapView.h>
#include <MnProviderFinder.h>
#include <MnProvider.h>
#include <EPos_CPosLandmark.h>
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_PosLandmarkSerialization.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CTestMnMapView::CTestMnMapView
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CTestMnMapView::CTestMnMapView( CStifLogger* aLog ) :
				CActive(EPriorityStandard), iLog(aLog)
	{
	CActiveScheduler::Add(this);
	}

// -----------------------------------------------------------------------------
// CTestMnMapView::NewL
// 
// 
// -----------------------------------------------------------------------------
//	
CTestMnMapView* CTestMnMapView::NewL(CStifLogger* aLog)
	{
	CTestMnMapView* self = new (ELeave) CTestMnMapView( aLog );
	CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); 
    return self; 
	}

// -----------------------------------------------------------------------------
// CTestMnMapView::ConstructL
// 
// 
// -----------------------------------------------------------------------------
//
void CTestMnMapView::ConstructL()
	{
	Cancel();
	}	

// -----------------------------------------------------------------------------
// CTestMnMapView::~CTestMnMapView
// 
// 
// -----------------------------------------------------------------------------
//
CTestMnMapView::~CTestMnMapView()
	{
	
	}
	
// -----------------------------------------------------------------------------
// CTestMnMapView::CreateMapViewL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMnMapView::CreateMapViewL( CStifItemParser& /*aItem*/ )
	{
	// Find one provider to be used by mapview
	RPointerArray<CMnProvider> providerArray;
	MnProviderFinder::FindProvidersL( providerArray, CMnProvider::EServiceMapView );
	if( providerArray.Count() < 1 )
		{
		// No provider found
		iLog->Log(_L("Provider not found, CreateNavigatorL fails"));
		User::Leave( KErrNotFound );
		}
	// Use first provider
	CMnMapView* mapView = CMnMapView::NewL( *providerArray.operator[](0) );
	delete mapView;
	providerArray.ResetAndDestroy();
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// CTestMnMapView::CreateChainedMapViewL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMnMapView::CreateChainedMapViewL( CStifItemParser& /*aItem*/ )
	{
	// Find one provider to be used by mapview
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
	// Use provider
	CMnMapView* mapView = CMnMapView::NewChainedL( *providerArray.operator[](i) );
	delete mapView;
	providerArray.ResetAndDestroy();
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// CTestMnMapView::AddLinkedLmksToShowL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMnMapView::AddLinkedLmksToShowL( CStifItemParser& /*aItem*/ )
	{
	// Find one provider to be used by mapview
	RPointerArray<CMnProvider> providerArray;
	MnProviderFinder::FindProvidersL( providerArray, CMnProvider::EServiceMapView );
	if( providerArray.Count() < 1 )
		{
		// No provider found
		iLog->Log(_L("Provider not found, CreateNavigatorL fails"));
		User::Leave( KErrNotFound );
		}
	// Use first provider
	CMnMapView* mapView = CMnMapView::NewL( *providerArray.operator[](0) );
	CleanupStack::PushL( mapView );	
	// Open test database, get landmark ids
	CPosLandmarkDatabase* lmkDatabase = CPosLandmarkDatabase::OpenL();
	CleanupStack::PushL( lmkDatabase );	
	// Get URI
	HBufC* uri = lmkDatabase->DatabaseUriLC();
	// Init
	ExecuteAndDeleteLD( lmkDatabase->InitializeL() );
	// Get Landmark Iterator
	CPosLmItemIterator* iterator = lmkDatabase->LandmarkIteratorL();
	CleanupStack::PushL( iterator );
	// Get Ids
	RArray<TPosLmItemId> lmkIdArray;
	TPosLmItemId id;
	while( (id = iterator->NextL()) != KPosLmNullItemId )
		{
		lmkIdArray.Append( id );
		}
	// Add
	mapView->AddLandmarksToShowL( uri->Des(), lmkIdArray );
	
	CleanupStack::PopAndDestroy( 4, mapView );
	ReleaseLandmarkResources();
	providerArray.ResetAndDestroy();
	return KErrNone;
	}	

// -----------------------------------------------------------------------------
// CTestMnMapView::AddLmksToShowL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMnMapView::AddLmksToShowL( CStifItemParser&  /*aItem*/ )
	{
	// Find one provider to be used by mapview
	RPointerArray<CMnProvider> providerArray;
	MnProviderFinder::FindProvidersL( providerArray, CMnProvider::EServiceMapView );
	if( providerArray.Count() < 1 )
		{
		// No provider found
		iLog->Log(_L("Provider not found, CreateNavigatorL fails"));
		User::Leave( KErrNotFound );
		}
	// Use first provider
	CMnMapView* mapView = CMnMapView::NewL( *providerArray.operator[](0) );
	CleanupStack::PushL( mapView );	
	// 
	RPointerArray<CPosLandmark> lmkArray;
	// Create two landmarks, append to array
	// 1
	CPosLandmark* landmark1 = CPosLandmark::NewL();
	CleanupStack::PushL( landmark1 );
	lmkArray.Append( landmark1 );
	// 2
	CPosLandmark* landmark2 = CPosLandmark::NewL();
	CleanupStack::PushL( landmark2 );
	lmkArray.Append( landmark2 );	
	// Add
	mapView->AddLandmarksToShowL( lmkArray );
	
	CleanupStack::PopAndDestroy( 3, mapView );
	ReleaseLandmarkResources();
	providerArray.ResetAndDestroy();
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// CTestMnMapView::AddPackedLmksToShowL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMnMapView::AddPackedLmksToShowL( CStifItemParser& /*aItem*/ )
	{
	// Find one provider to be used by mapview
	RPointerArray<CMnProvider> providerArray;
	MnProviderFinder::FindProvidersL( providerArray, CMnProvider::EServiceMapView );
	if( providerArray.Count() < 1 )
		{
		// No provider found
		iLog->Log(_L("Provider not found, CreateNavigatorL fails"));
		User::Leave( KErrNotFound );
		}
	// Use first provider
	CMnMapView* mapView = CMnMapView::NewL( *providerArray.operator[](0) );
	CleanupStack::PushL( mapView );	
	// 
	RArray<TPtrC8> packedLmksArray;
	// Create two landmarks, pack and append to array
	// 1
	CPosLandmark* landmark1 = CPosLandmark::NewL();
	CleanupStack::PushL( landmark1 );
	HBufC8* lmkBuffer1 = PosLandmarkSerialization::PackL( *landmark1 );
	packedLmksArray.Append( *lmkBuffer1 );
	// 2
	CPosLandmark* landmark2 = CPosLandmark::NewL();
	CleanupStack::PushL( landmark2 );
	HBufC8* lmkBuffer2 = PosLandmarkSerialization::PackL( *landmark1 );
	packedLmksArray.Append( *lmkBuffer2 );
//	lmkArray.Append( landmark2 );	
	// Add 
	mapView->AddLandmarksToShowL( packedLmksArray );
	
	CleanupStack::PopAndDestroy( 3, mapView );
	ReleaseLandmarkResources();
	providerArray.ResetAndDestroy();
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// CTestMnMapView::ResetLmksToShowL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMnMapView::ResetLmksToShowL( CStifItemParser& /*aItem*/ )
	{
	// Find one provider to be used by mapview
	RPointerArray<CMnProvider> providerArray;
	MnProviderFinder::FindProvidersL( providerArray, CMnProvider::EServiceMapView );
	if( providerArray.Count() < 1 )
		{
		// No provider found
		iLog->Log(_L("Provider not found, CreateNavigatorL fails"));
		User::Leave( KErrNotFound );
		}
	// Use first provider
	CMnMapView* mapView = CMnMapView::NewL( *providerArray.operator[](0) );
	CleanupStack::PushL( mapView );	
	// 
	RArray<TPtrC8> packedLmksArray;
	// Create two landmarks, pack and append to array
	// 1
	CPosLandmark* landmark1 = CPosLandmark::NewL();
	CleanupStack::PushL( landmark1 );
	HBufC8* lmkBuffer1 = PosLandmarkSerialization::PackL( *landmark1 );
	packedLmksArray.Append( *lmkBuffer1 );
	// 2
	CPosLandmark* landmark2 = CPosLandmark::NewL();
	CleanupStack::PushL( landmark2 );
	HBufC8* lmkBuffer2 = PosLandmarkSerialization::PackL( *landmark1 );
	packedLmksArray.Append( *lmkBuffer2 );
//	lmkArray.Append( landmark2 );	
	// Add 
	mapView->AddLandmarksToShowL( packedLmksArray );
	
	// Reset landmarks to show
	mapView->ResetLandmarksToShow();
	CleanupStack::PopAndDestroy( 3, mapView );
	ReleaseLandmarkResources();
	providerArray.ResetAndDestroy();
	return KErrNone;
	}	

// -----------------------------------------------------------------------------
// CTestMnMapView::SetMapAreaL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMnMapView::SetMapAreaL( CStifItemParser& /*aItem*/ )
	{
	// Find one provider to be used by mapview
	RPointerArray<CMnProvider> providerArray;
	MnProviderFinder::FindProvidersL( providerArray, CMnProvider::EServiceMapView );
	if( providerArray.Count() < 1 )
		{
		// No provider found
		iLog->Log(_L("Provider not found, CreateNavigatorL fails"));
		User::Leave( KErrNotFound );
		}
	// Use first provider
	CMnMapView* mapView = CMnMapView::NewL( *providerArray.operator[](0) );
	CleanupStack::PushL( mapView );	
	// Set map area
	TCoordinate center( 21.22, 56.45 );
	mapView->SetMapAreaL( center, 7.5 );
	
	CleanupStack::PopAndDestroy( mapView );
	providerArray.ResetAndDestroy();
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// CTestMnMapView::ResetMapAreaL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMnMapView::ResetMapAreaL( CStifItemParser& /*aItem*/ )
	{
	// Find one provider to be used by mapview
	RPointerArray<CMnProvider> providerArray;
	MnProviderFinder::FindProvidersL( providerArray, CMnProvider::EServiceMapView );
	if( providerArray.Count() < 1 )
		{
		// No provider found
		iLog->Log(_L("Provider not found, CreateNavigatorL fails"));
		User::Leave( KErrNotFound );
		}
	// Use first provider
	CMnMapView* mapView = CMnMapView::NewL( *providerArray.operator[](0) );
	CleanupStack::PushL( mapView );	
	// Set map area
	TCoordinate center( 21.22, 56.45 );
	mapView->SetMapAreaL( center, 7.5 );
	
	// Reset map area
	mapView->ResetMapArea();
	CleanupStack::PopAndDestroy( mapView );
	providerArray.ResetAndDestroy();
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// CTestMnMapView::SetCustomRequestTextL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMnMapView::SetCustomRequestTextL( CStifItemParser& aItem )
	{
	TPtrC requestText;
	TInt err = aItem.GetNextString( requestText );
	// Find one provider to be used by mapview
	RPointerArray<CMnProvider> providerArray;
	MnProviderFinder::FindProvidersL( providerArray, CMnProvider::EServiceMapView );
	if( providerArray.Count() < 1 )
		{
		// No provider found
		iLog->Log(_L("Provider not found, CreateNavigatorL fails"));
		User::Leave( KErrNotFound );
		}
	// Use first provider
	CMnMapView* mapView = CMnMapView::NewL( *providerArray.operator[](0) );
	CleanupStack::PushL( mapView );	
	// Set request text
	mapView->SetCustomRequestTextL( requestText );
	CleanupStack::PopAndDestroy( mapView );
	providerArray.ResetAndDestroy();
	return KErrNone;
	}		
	
// -----------------------------------------------------------------------------
// CTestMnMapView::SetUseDefaultRequestTextL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMnMapView::SetUseDefaultRequestTextL( CStifItemParser& /*aItem*/ )
	{
	// Find one provider to be used by mapview
	RPointerArray<CMnProvider> providerArray;
	MnProviderFinder::FindProvidersL( providerArray, CMnProvider::EServiceMapView );
	if( providerArray.Count() < 1 )
		{
		// No provider found
		iLog->Log(_L("Provider not found, CreateNavigatorL fails"));
		User::Leave( KErrNotFound );
		}
	// Use first provider
	CMnMapView* mapView = CMnMapView::NewL( *providerArray.operator[](0) );
	CleanupStack::PushL( mapView );	
	// Set default request text
	mapView->SetUseDefaultRequestText();
	CleanupStack::PopAndDestroy( mapView );
	providerArray.ResetAndDestroy();
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// CTestMnMapView::SetCurrentLocationOptionL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMnMapView::SetCurrentLocationOptionL( CStifItemParser& /*aItem*/ )
	{
	// Find one provider to be used by mapview
	RPointerArray<CMnProvider> providerArray;
	MnProviderFinder::FindProvidersL( providerArray, CMnProvider::EServiceMapView );
	if( providerArray.Count() < 1 )
		{
		// No provider found
		iLog->Log(_L("Provider not found, CreateNavigatorL fails"));
		User::Leave( KErrNotFound );
		}
	// Use first provider
	CMnMapView* mapView = CMnMapView::NewL( *providerArray.operator[](0) );
	CleanupStack::PushL( mapView );	
	// Set current location option
	
	mapView->SetCurrentLocationOption( CMnMapView::ECurrentLocationEnabled );
	CleanupStack::PopAndDestroy( mapView );
	providerArray.ResetAndDestroy();
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// CTestMnMapView::GetCurrentLocationOptionL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMnMapView::GetCurrentLocationOptionL( CStifItemParser& /*aItem*/ )
	{
	// Find one provider to be used by mapview
	RPointerArray<CMnProvider> providerArray;
	MnProviderFinder::FindProvidersL( providerArray, CMnProvider::EServiceMapView );
	if( providerArray.Count() < 1 )
		{
		// No provider found
		iLog->Log(_L("Provider not found, CreateNavigatorL fails"));
		User::Leave( KErrNotFound );
		}
	// Use first provider
	CMnMapView* mapView = CMnMapView::NewL( *providerArray.operator[](0) );
	CleanupStack::PushL( mapView );	
	// Set current location option
	CMnMapView::TCurrentLocationOption option = mapView->CurrentLocationOption();
	CleanupStack::PopAndDestroy( mapView );
	providerArray.ResetAndDestroy();
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// CTestMnMapView::SetAndGetOptionsL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMnMapView::SetAndGetOptionsL( CStifItemParser& /*aItem*/ )
	{
	// Find one provider to be used by mapview
	RPointerArray<CMnProvider> providerArray;
	MnProviderFinder::FindProvidersL( providerArray, CMnProvider::EServiceMapView );
	if( providerArray.Count() < 1 )
		{
		// No provider found
		iLog->Log(_L("Provider not found, CreateNavigatorL fails"));
		User::Leave( KErrNotFound );
		}
	// Use first provider
	CMnMapView* mapView = CMnMapView::NewL( *providerArray.operator[](0) );
	CleanupStack::PushL( mapView );
	
	// Set map view option
	mapView->SetOptions( CMnMapView::EOptionRestrictSelection );
	// Get map view option
	CMnMapView::TOptions options = mapView->Options();
	
	CleanupStack::PopAndDestroy( mapView );
	providerArray.ResetAndDestroy();
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// CTestMnMapView::SelectionResultTypeL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMnMapView::SelectionResultTypeL( CStifItemParser& /*aItem*/ )
	{
	// Find one provider to be used by mapview
	RPointerArray<CMnProvider> providerArray;
	MnProviderFinder::FindProvidersL( providerArray, CMnProvider::EServiceMapView );
	if( providerArray.Count() < 1 )
		{
		// No provider found
		iLog->Log(_L("Provider not found, CreateNavigatorL fails"));
		User::Leave( KErrNotFound );
		}
	// Use first provider
	CMnMapView* mapView = CMnMapView::NewL( *providerArray.operator[](0) );
	CleanupStack::PushL( mapView );
	
	
	// Get Selection Result Type
	CMnMapView::TSelectionResultType resultType = mapView->SelectionResultType();
	
	CleanupStack::PopAndDestroy( mapView );
	providerArray.ResetAndDestroy();
	return KErrNone;
	}			
	
// -----------------------------------------------------------------------------
// CTestMnMapView::ShowMapL
// 
// Not being tested, user input is required to close the provider app
// -----------------------------------------------------------------------------
//
TInt CTestMnMapView::ShowMapL( CStifItemParser& /*aItem*/ )
	{
	// Find one provider to be used by mapview
	RPointerArray<CMnProvider> providerArray;
	MnProviderFinder::FindProvidersL( providerArray, CMnProvider::EServiceMapView );
	if( providerArray.Count() < 1 )
		{
		// No provider found
		iLog->Log(_L("Provider not found, CreateNavigatorL fails"));
		User::Leave( KErrNotFound );
		}
	// Use first provider
	CMnMapView* mapView = CMnMapView::NewL( *providerArray.operator[](0) );
	CleanupStack::PushL( mapView );
	
	
	// Show map
	mapView->ShowMapL();
	
	CleanupStack::PopAndDestroy( mapView );
	providerArray.ResetAndDestroy();
	return KErrNone;
	}				
	
// -----------------------------------------------------------------------------
// CTestMnMapView::ShowCurrentLocationL
// 
// Not being tested, user input is required to close the provider app
// -----------------------------------------------------------------------------
//
TInt CTestMnMapView::ShowCurrentLocationL( CStifItemParser& /*aItem*/ )
	{
	// Find one provider to be used by mapview
	RPointerArray<CMnProvider> providerArray;
	MnProviderFinder::FindProvidersL( providerArray, CMnProvider::EServiceMapView );
	if( providerArray.Count() < 1 )
		{
		// No provider found
		iLog->Log(_L("Provider not found, CreateNavigatorL fails"));
		User::Leave( KErrNotFound );
		}
	// Use first provider
	CMnMapView* mapView = CMnMapView::NewL( *providerArray.operator[](0) );
	CleanupStack::PushL( mapView );
	
	
	// Show Current Location
	mapView->ShowCurrentLocationL();
	
	CleanupStack::PopAndDestroy( mapView );
	providerArray.ResetAndDestroy();
	return KErrNone;
	}					
	
// -----------------------------------------------------------------------------
// CTestMnMapView::SelectFromMapL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMnMapView::SelectFromMapL( CStifItemParser& /*aItem*/ )
	{
	// Find one provider to be used by mapview
	RPointerArray<CMnProvider> providerArray;
	MnProviderFinder::FindProvidersL( providerArray, CMnProvider::EServiceMapView );
	if( providerArray.Count() < 1 )
		{
		// No provider found
		iLog->Log(_L("Provider not found, CreateNavigatorL fails"));
		User::Leave( KErrNotFound );
		}
	// Use first provider
	CMnMapView* mapView = CMnMapView::NewL( *providerArray.operator[](0) );
	CleanupStack::PushL( mapView );
	
	// Async request for selecting a position from map
	mapView->SelectFromMapL( iStatus );
	SetActive();
	mapView->Cancel();
	// Wait
	CActiveScheduler::Start();
	
	CleanupStack::PopAndDestroy( mapView );
	providerArray.ResetAndDestroy();
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// CTestMnMapView::SetViewExitObserverL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMnMapView::SetViewExitObserverL( CStifItemParser& /*aItem*/ )
	{
// Find one provider to be used by mapview
	RPointerArray<CMnProvider> providerArray;
	MnProviderFinder::FindProvidersL( providerArray, CMnProvider::EServiceMapView );
	if( providerArray.Count() < 1 )
		{
		// No provider found
		iLog->Log(_L("Provider not found, CreateNavigatorL fails"));
		User::Leave( KErrNotFound );
		}
	// Use first provider
	CMnMapView* mapView = CMnMapView::NewL( *providerArray.operator[](0) );
	CleanupStack::PushL( mapView );
	
	// Set exit observer
	mapView->SetExitObserverL( *this );
	
	CleanupStack::PopAndDestroy( mapView );
	providerArray.ResetAndDestroy();
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// CTestMnMapView::RemoveViewExitObserverL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMnMapView::RemoveViewExitObserverL( CStifItemParser& /*aItem*/ )
	{
// Find one provider to be used by mapview
	RPointerArray<CMnProvider> providerArray;
	MnProviderFinder::FindProvidersL( providerArray, CMnProvider::EServiceMapView );
	if( providerArray.Count() < 1 )
		{
		// No provider found
		iLog->Log(_L("Provider not found, CreateNavigatorL fails"));
		User::Leave( KErrNotFound );
		}
	// Use first provider
	CMnMapView* mapView = CMnMapView::NewL( *providerArray.operator[](0) );
	CleanupStack::PushL( mapView );
	
	// Set exit observer
	mapView->SetExitObserverL( *this );
	// Remove exit observer
	mapView->RemoveExitObserver();
	
	CleanupStack::PopAndDestroy( mapView );
	providerArray.ResetAndDestroy();
	return KErrNone;
	}								
	
// -----------------------------------------------------------------------------
// CTestMnMapView::RunL
// 
// 
// -----------------------------------------------------------------------------
//
void CTestMnMapView::RunL()
	{
	iLog->Log(_L("RunL Called"));
	CActiveScheduler::Stop();
	}
	
// -----------------------------------------------------------------------------
// CTestMnMapView::DoCancel
// 
// 
// -----------------------------------------------------------------------------
//
void CTestMnMapView::DoCancel()
	{
	
	}	
	
// -----------------------------------------------------------------------------
// CTestMnNavigator::HandleServerAppExit
// 
// 
// -----------------------------------------------------------------------------
//
void CTestMnMapView::HandleServerAppExit( TInt /*aReason*/ )
	{
	iLog->Log(_L("Provider exits"));
	}

