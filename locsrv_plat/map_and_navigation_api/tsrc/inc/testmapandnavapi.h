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
* Description: Declaration  of test class for map_and_navigation_api
*
*/



#ifndef TESTMAPANDNAVAPI_H
#define TESTMAPANDNAVAPI_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include "testcmnnavigator.h"
#include "testcmnmapview.h"


// Logging path
_LIT( KtestmapandnavapiLogPath, "\\logs\\testframework\\testmapandnavapi\\" ); 
// Log file
_LIT( KtestmapandnavapiLogFile, "testmapandnavapi.txt" ); 


// FORWARD DECLARATIONS
class CTestMapAndNavApi;



// CLASS DECLARATION

/**
*  Ctestmapandnavapi test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(CTestMapAndNavApi) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CTestMapAndNavApi* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CTestMapAndNavApi();
        
        

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
        CTestMapAndNavApi( CTestModuleIf& aTestModuleIf );

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
        * CreateNavigatorL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt CreateNavigatorL( CStifItemParser& aItem );
        
        /**
        * CreateChainedNavigatorL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt CreateChainedNavigatorL( CStifItemParser& aItem );
        
               
        /**
        * SetExitObserverL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt SetExitObserverL( CStifItemParser& aItem );
        
        /**
        * RemoveExitObserverL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt RemoveExitObserverL( CStifItemParser& aItem );
        
        /**
        * CreateMapViewL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt CreateMapViewL( CStifItemParser& aItem );
        
        /**
        * CreateChainedMapViewL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt CreateChainedMapViewL( CStifItemParser& aItem );
        
        /**
        * AddLinkedLmksToShowL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt AddLinkedLmksToShowL( CStifItemParser& aItem );
        
        /**
        * AddLmksToShowL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt AddLmksToShowL( CStifItemParser& aItem );
        
        /**
        * AddPackedLmksToShowL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt AddPackedLmksToShowL( CStifItemParser& aItem );
        
        /**
        * ResetLmksToShowL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt ResetLmksToShowL( CStifItemParser& aItem );
        
        /**
        * SetMapAreaL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt SetMapAreaL( CStifItemParser& aItem );
        
        /**
        * ResetMapAreaL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt ResetMapAreaL( CStifItemParser& aItem );

		/**
        * SetCustomRequestTextL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt SetCustomRequestTextL( CStifItemParser& aItem );        
        
        /**
        * SetUseDefaultRequestTextL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt SetUseDefaultRequestTextL( CStifItemParser& aItem );        
        
        /**
        * SetCurrentLocationOptionL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt SetCurrentLocationOptionL( CStifItemParser& aItem );        
        
        /**
        * GetCurrentLocationOptionL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt GetCurrentLocationOptionL( CStifItemParser& aItem );        
        
        /**
        * SetAndGetOptionsL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt SetAndGetOptionsL( CStifItemParser& aItem );        
        
        /**
        * SelectionResultTypeL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt SelectionResultTypeL( CStifItemParser& aItem );        
        
        
        /**
        * SelectFromMapL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt SelectFromMapL( CStifItemParser& aItem );
        
        /**
        * SetViewExitObserverL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt SetViewExitObserverL( CStifItemParser& aItem );
        
        /**
        * RemoveViewExitObserverL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt RemoveViewExitObserverL( CStifItemParser& aItem );
        
        //ADD NEW METHOD DEC HERE
    
    };

#endif      // TESTMAPANDNAVAPI_H

// End of File
