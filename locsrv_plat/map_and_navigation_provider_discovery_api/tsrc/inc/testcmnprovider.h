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



#ifndef CTESTMNPROVIDER_H
#define CTESTMNPROVIDER_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>



// CLASS 

class CTestMnProvider
	{
	public:	//Public constructor of class
	
		/**
		*
		*/
		CTestMnProvider(CStifLogger* aLog);
		
		static CTestMnProvider* NewL(CStifLogger* aLog);
		
		void ConstructL();
		
		~CTestMnProvider();
		
	public:	//Public methods
		
	public:	//Public methods
	
		/**
		*
		*/
		TInt DestroyProviderL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt ProviderUidL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt ProviderVersionL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt GetProviderNameL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt GetShortNameL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt GetVendorNameL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt SupportedServicesL( CStifItemParser& aItem );
		
		/**
		*
		*/
		TInt SupportedFeaturesL( CStifItemParser& aItem );
		
				
	private:
		
		// Private members
		CStifLogger* iLog;	//Log		
	};
	
#endif // CTESTMNPROVIDER_H
	