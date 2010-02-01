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



#ifndef CTESTMNNAVIGATOR_H
#define CTESTMNNAVIGATOR_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <mnnavigator.h>
#include <EPos_CPosLandmark.h>
#include <LbsPosition.h>
#include <AknServerApp.h>


// CLASS 

class CTestMnNavigator : public CBase, public MAknServerAppExitObserver
	{
	public:	//Public constructor of class
	
		/**
		*
		*/
		CTestMnNavigator(CStifLogger* aLog);
		
		static CTestMnNavigator* NewL(CStifLogger* aLog);
		
		void ConstructL();
		
		~CTestMnNavigator();
		
	public:	//Public methods
		
	public:	//Public methods
	
		/**
		*
		*/
		TInt CreateNavigatorL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt CreateChainedNavigatorL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt NavigateToL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt SetExitObserverL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt RemoveExitObserverL( CStifItemParser& aItem );
		
		/**
		*
		*/
		void HandleServerAppExit(TInt aReason);	
		
	private:
		
		// Private members
		CStifLogger* iLog;	//Log		
	};
	
#endif // CTESTMNNAVIGATOR_H
	