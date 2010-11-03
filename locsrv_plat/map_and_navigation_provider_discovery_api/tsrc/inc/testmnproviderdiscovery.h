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
* Description: Declaration  of test class for map_and_navigation_provider_discovery_api
*
*/



#ifndef TESTMNPROVIDERDISCOVERY_H
#define TESTMNPROVIDERDISCOVERY_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include "testmnproviderfinder.h"
#include "testcmnprovider.h"


// Logging path
_LIT( KtestmnproviderdiscoveryLogPath, "\\logs\\testframework\\testmnproviderdiscovery\\" ); 
// Log file
_LIT( KtestmnproviderdiscoveryLogFile, "testmnproviderdiscovery.txt" ); 


// FORWARD DECLARATIONS
class CTestMnProviderDiscovery;


// CLASS DECLARATION

/**
*  Ctestmnproviderdiscovery test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(CTestMnProviderDiscovery) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CTestMnProviderDiscovery* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CTestMnProviderDiscovery();

   
    public: // Functions from base classes

        /**
        * From CScriptBase Runs a script line.
        * @since ?Series60_version
        * @param aItem Script line containing method name and parameters
        * @return Symbian OS error code
        */
        virtual TInt RunMethodL( CStifItemParser& aItem );

    protected:  // New functions

        
    protected:  // Functions from base classes

        
    private:

        /**
        * C++ default constructor.
        */
        CTestMnProviderDiscovery( CTestModuleIf& aTestModuleIf );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        
        /**
        * Frees all resources allocated from test methods.
        * @since ?Series60_version
        */
        void Delete();

        /**
        * Test methods are listed below. 
        */
		
		/**
      	* Installs provider application
      	*
      	*/
		TInt InstallProvider( const TDesC& aPackagePath );
                
        /**
        * FindAllProvidersL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt FindAllProvidersL( CStifItemParser& aItem );
        
        /**
        * FindSelectedProvidersL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt FindSelectedProvidersL( CStifItemParser& aItem );
        
        /**
        * DestroyProviderL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt DestroyProviderL( CStifItemParser& aItem );
        
        /**
        * ProviderUidL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt ProviderUidL( CStifItemParser& aItem );
        
        /**
        * ProviderVersionL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt ProviderVersionL( CStifItemParser& aItem );
        
        /**
        * GetProviderNameL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt GetProviderNameL( CStifItemParser& aItem );
        
        /**
        * GetShortNameL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt GetShortNameL( CStifItemParser& aItem );
        
        /**
        * GetVendorNameL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt GetVendorNameL( CStifItemParser& aItem );
        
        /**
        * SupportedServicesL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt SupportedServicesL( CStifItemParser& aItem );
        
        /**
        * SupportedFeaturesL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt SupportedFeaturesL( CStifItemParser& aItem );
        
        //ADD NEW METHOD DEC HERE

    };

#endif      // TESTMNPROVIDERDISCOVERY_H

// End of File
