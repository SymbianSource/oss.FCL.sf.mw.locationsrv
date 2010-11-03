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
* Description: Entry point for test cases in stif module
*
*/



// INCLUDE FILES
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include "testmapandnavapi.h"



// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CTestMapAndNavApi::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CTestMapAndNavApi::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// CTestMapAndNavApi::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CTestMapAndNavApi::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Navigator
        ENTRY( "CreateNavigator", CTestMapAndNavApi::CreateNavigatorL ),
        ENTRY( "CreateChainedNavigator", CTestMapAndNavApi::CreateChainedNavigatorL ),
        ENTRY( "SetNavigatorExitObserver", CTestMapAndNavApi::SetExitObserverL ),
        ENTRY( "RemoveNavigatorExitObserver", CTestMapAndNavApi::RemoveExitObserverL ),
        
        // Map View
        ENTRY( "CreateMapView", CTestMapAndNavApi::CreateMapViewL ),
        ENTRY( "CreateChainedMapView", CTestMapAndNavApi::CreateChainedMapViewL ),
        ENTRY( "AddLinkedLmksToShow", CTestMapAndNavApi::AddLinkedLmksToShowL ),
        ENTRY( "AddLmksToShow", CTestMapAndNavApi::AddLmksToShowL ),
        ENTRY( "AddPackedLmksToShow", CTestMapAndNavApi::AddPackedLmksToShowL ),
        ENTRY( "ResetLmksToShow", CTestMapAndNavApi::ResetLmksToShowL ),
        ENTRY( "SetMapArea", CTestMapAndNavApi::SetMapAreaL ),
        ENTRY( "ResetMapArea", CTestMapAndNavApi::ResetMapAreaL ),
        ENTRY( "SetCustomRequestText", CTestMapAndNavApi::SetCustomRequestTextL ),
        ENTRY( "SetUseDefaultRequestText", CTestMapAndNavApi::SetUseDefaultRequestTextL ),
        ENTRY( "SetCurrentLocationOption", CTestMapAndNavApi::SetCurrentLocationOptionL ),
        ENTRY( "GetCurrentLocationOption", CTestMapAndNavApi::GetCurrentLocationOptionL ),
        ENTRY( "SetAndGetOptions", CTestMapAndNavApi::SetAndGetOptionsL ),
        ENTRY( "SelectionResultType", CTestMapAndNavApi::SelectionResultTypeL ),
        ENTRY( "SelectFromMap", CTestMapAndNavApi::SelectFromMapL ),
        ENTRY( "SetViewExitObserver", CTestMapAndNavApi::SetViewExitObserverL ),
        ENTRY( "RemoveViewExitObserver", CTestMapAndNavApi::RemoveViewExitObserverL ),
        
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
// CTestMapAndNavApi::CreateNavigatorL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMapAndNavApi::CreateNavigatorL( CStifItemParser& aItem )
    {
    CTestMnNavigator* testNavigator = CTestMnNavigator::NewL( iLog );
	CleanupStack::PushL( testNavigator );
	TInt err = testNavigator->CreateNavigatorL( aItem );
	CleanupStack::PopAndDestroy( testNavigator );
	return err;	
    }   

// -----------------------------------------------------------------------------
// CTestMapAndNavApi::CreateChainedNavigatorL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMapAndNavApi::CreateChainedNavigatorL( CStifItemParser& aItem )
    {
    CTestMnNavigator* testNavigator = CTestMnNavigator::NewL( iLog );
	CleanupStack::PushL( testNavigator );
	TInt err = testNavigator->CreateChainedNavigatorL( aItem );
	CleanupStack::PopAndDestroy( testNavigator );
	return err;	
    } 
    
// -----------------------------------------------------------------------------
// CTestMapAndNavApi::SetExitObserverL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMapAndNavApi::SetExitObserverL( CStifItemParser& aItem )
    {
    CTestMnNavigator* testNavigator = CTestMnNavigator::NewL( iLog );
	CleanupStack::PushL( testNavigator );
	TInt err = testNavigator->SetExitObserverL( aItem );
	CleanupStack::PopAndDestroy( testNavigator );
	return err;	
    }     

// -----------------------------------------------------------------------------
// CTestMapAndNavApi::RemoveExitObserverL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMapAndNavApi::RemoveExitObserverL( CStifItemParser& aItem )
    {
    CTestMnNavigator* testNavigator = CTestMnNavigator::NewL( iLog );
	CleanupStack::PushL( testNavigator );
	TInt err = testNavigator->RemoveExitObserverL( aItem );
	CleanupStack::PopAndDestroy( testNavigator );
	return err;	
    }     

    
// -----------------------------------------------------------------------------
// CTestMapAndNavApi::CreateMapViewL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMapAndNavApi::CreateMapViewL( CStifItemParser& aItem )
    {
    CTestMnMapView* testMapView = CTestMnMapView::NewL( iLog );
	CleanupStack::PushL( testMapView );
	TInt err = testMapView->CreateMapViewL( aItem );
	CleanupStack::PopAndDestroy( testMapView );
	return err;	
    }     

// -----------------------------------------------------------------------------
// CTestMapAndNavApi::CreateChainedMapViewL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMapAndNavApi::CreateChainedMapViewL( CStifItemParser& aItem )
    {
    CTestMnMapView* testMapView = CTestMnMapView::NewL( iLog );
	CleanupStack::PushL( testMapView );
	TInt err = testMapView->CreateChainedMapViewL( aItem );
	CleanupStack::PopAndDestroy( testMapView );
	return err;	
    } 
    
// -----------------------------------------------------------------------------
// CTestMapAndNavApi::AddLinkedLmksToShowL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMapAndNavApi::AddLinkedLmksToShowL( CStifItemParser& aItem )
    {
    CTestMnMapView* testMapView = CTestMnMapView::NewL( iLog );
	CleanupStack::PushL( testMapView );
	TInt err = testMapView->AddLinkedLmksToShowL( aItem );
	CleanupStack::PopAndDestroy( testMapView );
	return err;	
    }   
    
// -----------------------------------------------------------------------------
// CTestMapAndNavApi::AddLmksToShowL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMapAndNavApi::AddLmksToShowL( CStifItemParser& aItem )
    {
    CTestMnMapView* testMapView = CTestMnMapView::NewL( iLog );
	CleanupStack::PushL( testMapView );
	TInt err = testMapView->AddLmksToShowL( aItem );
	CleanupStack::PopAndDestroy( testMapView );
	return err;	
    }
    
// -----------------------------------------------------------------------------
// CTestMapAndNavApi::AddPackedLmksToShowL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMapAndNavApi::AddPackedLmksToShowL( CStifItemParser& aItem )
    {
    CTestMnMapView* testMapView = CTestMnMapView::NewL( iLog );
	CleanupStack::PushL( testMapView );
	TInt err = testMapView->AddPackedLmksToShowL( aItem );
	CleanupStack::PopAndDestroy( testMapView );
	return err;	
    }    
    
// -----------------------------------------------------------------------------
// CTestMapAndNavApi::ResetLmksToShowL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMapAndNavApi::ResetLmksToShowL( CStifItemParser& aItem )
    {
    CTestMnMapView* testMapView = CTestMnMapView::NewL( iLog );
	CleanupStack::PushL( testMapView );
	TInt err = testMapView->ResetLmksToShowL( aItem );
	CleanupStack::PopAndDestroy( testMapView );
	return err;	
    }     
    
// -----------------------------------------------------------------------------
// CTestMapAndNavApi::SetMapAreaL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMapAndNavApi::SetMapAreaL( CStifItemParser& aItem )
    {
    CTestMnMapView* testMapView = CTestMnMapView::NewL( iLog );
	CleanupStack::PushL( testMapView );
	TInt err = testMapView->SetMapAreaL( aItem );
	CleanupStack::PopAndDestroy( testMapView );
	return err;	
    }            

// -----------------------------------------------------------------------------
// CTestMapAndNavApi::ResetMapAreaL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMapAndNavApi::ResetMapAreaL( CStifItemParser& aItem )
    {
    CTestMnMapView* testMapView = CTestMnMapView::NewL( iLog );
	CleanupStack::PushL( testMapView );
	TInt err = testMapView->ResetMapAreaL( aItem );
	CleanupStack::PopAndDestroy( testMapView );
	return err;	
    }   


// -----------------------------------------------------------------------------
// CTestMapAndNavApi::SetCustomRequestTextL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMapAndNavApi::SetCustomRequestTextL( CStifItemParser& aItem )
    {
    CTestMnMapView* testMapView = CTestMnMapView::NewL( iLog );
	CleanupStack::PushL( testMapView );
	TInt err = testMapView->SetCustomRequestTextL( aItem );
	CleanupStack::PopAndDestroy( testMapView );
	return err;	
    }   

// -----------------------------------------------------------------------------
// CTestMapAndNavApi::SetUseDefaultRequestTextL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMapAndNavApi::SetUseDefaultRequestTextL( CStifItemParser& aItem )
    {
    CTestMnMapView* testMapView = CTestMnMapView::NewL( iLog );
	CleanupStack::PushL( testMapView );
	TInt err = testMapView->SetUseDefaultRequestTextL( aItem );
	CleanupStack::PopAndDestroy( testMapView );
	return err;	
    }   
    
// -----------------------------------------------------------------------------
// CTestMapAndNavApi::SetCurrentLocationOptionL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMapAndNavApi::SetCurrentLocationOptionL( CStifItemParser& aItem )
    {
    CTestMnMapView* testMapView = CTestMnMapView::NewL( iLog );
	CleanupStack::PushL( testMapView );
	TInt err = testMapView->SetCurrentLocationOptionL( aItem );
	CleanupStack::PopAndDestroy( testMapView );
	return err;	
    } 
    
// -----------------------------------------------------------------------------
// CTestMapAndNavApi::GetCurrentLocationOptionL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMapAndNavApi::GetCurrentLocationOptionL( CStifItemParser& aItem )
    {
    CTestMnMapView* testMapView = CTestMnMapView::NewL( iLog );
	CleanupStack::PushL( testMapView );
	TInt err = testMapView->GetCurrentLocationOptionL( aItem );
	CleanupStack::PopAndDestroy( testMapView );
	return err;	
    } 
    
// -----------------------------------------------------------------------------
// CTestMapAndNavApi::SetAndGetOptionsL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMapAndNavApi::SetAndGetOptionsL( CStifItemParser& aItem )
    {
    CTestMnMapView* testMapView = CTestMnMapView::NewL( iLog );
	CleanupStack::PushL( testMapView );
	TInt err = testMapView->SetAndGetOptionsL( aItem );
	CleanupStack::PopAndDestroy( testMapView );
	return err;	
    }
    
// -----------------------------------------------------------------------------
// CTestMapAndNavApi::SelectionResultTypeL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMapAndNavApi::SelectionResultTypeL( CStifItemParser& aItem )
    {
    CTestMnMapView* testMapView = CTestMnMapView::NewL( iLog );
	CleanupStack::PushL( testMapView );
	TInt err = testMapView->SelectionResultTypeL( aItem );
	CleanupStack::PopAndDestroy( testMapView );
	return err;	
    }           

// -----------------------------------------------------------------------------
// CTestMapAndNavApi::SelectFromMapL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMapAndNavApi::SelectFromMapL( CStifItemParser& aItem )
    {
    CTestMnMapView* testMapView = CTestMnMapView::NewL( iLog );
	CleanupStack::PushL( testMapView );
	TInt err = testMapView->SelectFromMapL( aItem );
	CleanupStack::PopAndDestroy( testMapView );
	return err;	
    }
    
// -----------------------------------------------------------------------------
// CTestMapAndNavApi::SetViewExitObserverL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMapAndNavApi::SetViewExitObserverL( CStifItemParser& aItem )
    {
    CTestMnMapView* testMapView = CTestMnMapView::NewL( iLog );
	CleanupStack::PushL( testMapView );
	TInt err = testMapView->SetViewExitObserverL( aItem );
	CleanupStack::PopAndDestroy( testMapView );
	return err;	
    }                   
    
// -----------------------------------------------------------------------------
// CTestMapAndNavApi::RemoveViewExitObserverL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestMapAndNavApi::RemoveViewExitObserverL( CStifItemParser& aItem )
    {
    CTestMnMapView* testMapView = CTestMnMapView::NewL( iLog );
	CleanupStack::PushL( testMapView );
	TInt err = testMapView->RemoveViewExitObserverL( aItem );
	CleanupStack::PopAndDestroy( testMapView );
	return err;	
    }                   

//  End of File
