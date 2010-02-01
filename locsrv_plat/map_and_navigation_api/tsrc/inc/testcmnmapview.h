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



#ifndef CTESTMNMAPVIEW_H
#define CTESTMNMAPVIEW_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <mnnavigator.h>
#include <AknServerApp.h>

// CLASS 

class CTestMnMapView : public CActive, public MAknServerAppExitObserver
	{
	public:	//Public constructor of class
	
		/**
		*
		*/
		CTestMnMapView(CStifLogger* aLog);
		
		static CTestMnMapView* NewL(CStifLogger* aLog);
		
		void ConstructL();
		
		~CTestMnMapView();
		
	public:	//Public methods
		
	public:	//Public methods
	
		/**
		*
		*/
		TInt CreateMapViewL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt CreateChainedMapViewL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt AddLinkedLmksToShowL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt AddLmksToShowL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt AddPackedLmksToShowL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt ResetLmksToShowL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt SetMapAreaL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt ResetMapAreaL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt SetCustomRequestTextL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt SetUseDefaultRequestTextL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt SetCurrentLocationOptionL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt GetCurrentLocationOptionL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt SetAndGetOptionsL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt SelectionResultTypeL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt ShowMapL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt ShowCurrentLocationL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt SelectFromMapL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt SetViewExitObserverL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt RemoveViewExitObserverL( CStifItemParser& aItem );
		
		/**
		*
		*/
		void HandleServerAppExit(TInt aReason);	
		
	protected:

		void RunL();
	
		void DoCancel();
			
	private:
		
		// Private members
		CStifLogger* iLog;	//Log		
	};
	
#endif // CTESTMNMAPVIEW_H
	