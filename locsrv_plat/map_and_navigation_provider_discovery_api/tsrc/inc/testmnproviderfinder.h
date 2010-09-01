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



#ifndef TESTMNNPROVIDERFINDER_H
#define TESTMNNPROVIDERFINDER_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>


// CLASS 

class TestMnProviderFinder
	{
	public:	//Public constructor of class
	
		/**
		*
		*/
		TestMnProviderFinder(CStifLogger* aLog);
		
		static TestMnProviderFinder* NewL(CStifLogger* aLog);
		
		void ConstructL();
		
		~TestMnProviderFinder();
		
	public:	//Public methods
		
	public:	//Public methods
	
		/**
		*
		*/
		TInt FindAllProvidersL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt FindSelectedProvidersL( CStifItemParser& aItem );
		
		/**
		*
		*/
		//TInt NavigateToL( CStifItemParser& aItem );
		
		/**
		*
		*/
		//TInt SetExitObserverL( CStifItemParser& aItem );
		
		/**
		*
		*/
		//TInt RemoveExitObserverL( CStifItemParser& aItem );
		
				
	private:
		
		// Private members
		CStifLogger* iLog;	//Log		
	};
	
#endif // TESTMNNPROVIDERFINDER_H
	