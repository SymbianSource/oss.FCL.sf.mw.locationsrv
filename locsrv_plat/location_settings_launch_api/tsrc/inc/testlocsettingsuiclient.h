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
* Description:   Test Module DLL to Location Settings Launch API
*
*/




#ifndef TESTLOCSETTINGSUICLIENT_H
#define TESTLOCSETTINGSUICLIENT_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>


// CONSTANTS
_LIT( KtestlocsettingsuiclientLogPath, "\\logs\\testframework\\testlocsettingsuiclient\\" ); 
_LIT( KtestlocsettingsuiclientLogFile, "testlocsettingsuiclient.txt" ); 

// FORWARD DECLARATIONS
class CTestLocSettingsUiClient;
class CTestLocSettingsUiClientAO;

// CLASS DECLARATION

/**
*  CTestLocSettingsUiClient test class for STIF Test Framework TestScripter.
*  This is the testclass for Location Settings Launch API
*
*  @lib testlocsettingsuiclient.lib
*  @since S60 v3.2
*/

NONSHARABLE_CLASS(CTestLocSettingsUiClient) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CTestLocSettingsUiClient* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CTestLocSettingsUiClient();

    public: // New functions

    public: // Functions from base classes

        /**
        * From CScriptBase Runs a script line.
        * @since S60 v3.2
        * @param aItem Script line containing method name and parameters
        * @return Symbian OS error code
        */
        virtual TInt RunMethodL( CStifItemParser& aItem );

    private:

        /**
        * C++ default constructor.
        */
        CTestLocSettingsUiClient( CTestModuleIf& aTestModuleIf );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
          void ConstructL();

        /**
        * Frees all resources allocated from test methods.
        * @since S60 v3.2
        */
          void Delete();

	    /**
	     * Test Case for create a new instance of 
	     * CLocSettingsUiClient with NewL
	     *
	     * @since S60 v3.2
	     * Returns KErrNone if successful. 
	     *
	     */ 
		  TInt TestNewL( CStifItemParser& aItem );
		
	    /**
	     * Test Case for create a new instance of 
	     * CLocSettingsUiClient with NewLC
	     *
	     * @since S60 v3.2
	     * Returns KErrNone if successful. 
	     *
	     */ 
	 	  TInt TestNewLC( CStifItemParser& aItem );
	 	  		
	    /**
	     * Test Case for Launching Settings Ui 
	     * CLocSettingsUiClient
	     *
	     * @since S60 v3.2
	     * Returns KErrNone if successful. 
	     *
	     */ 
	 	  TInt TestLaunchSettingsL( CStifItemParser& aItem );
	 	  		
	    /**
	     * Test Case for Cancelling a Launch of Settings Ui 
	     * CLocSettingsUiClient
	     *
	     * @since S60 v3.2
	     * Returns KErrNone if successful. 
	     *
	     */ 
	 	  TInt TestCancelLaunchedSettingsUi(CStifItemParser& aItem);
	 	  	 	  		
	    /**
	     * Test Case for Cancelling a Launch of Settings Ui 
	     * which results in Error
	     *
	     * @since S60 v3.2
	     * Returns KErrNone if successful. 
	     *
	     */ 
	 	  TInt TestCancelLaunchedSettingsUiErr( CStifItemParser& aItem );
	 	  	 	  		
	    /**
	     * Test Case for Closing a Position Settings 
	     * CLocSettingsUiClient
	     *
	     * @since S60 v3.2
	     * Returns KErrNone if successful. 
	     *
	     */ 
	 	  TInt TestClosePosSettings( CStifItemParser& aItem );
	 	  	 	  		
	    /**
	     * Test Case for Closing a Position Settings 
	     * which results in Error
	     *
	     * @since S60 v3.2
	     * Returns KErrNone if successful. 
	     *
	     */ 
	 	  TInt TestClosePosSettingsErr( CStifItemParser& aItem );

    private:    // Data
        /**
        * Tester Class for CTestLocSettingsUiClient API methods
        */
    	 CTestLocSettingsUiClientAO* iLocSettingsUiClientAO;
        
    };

#endif      // TESTLOCSETTINGSUICLIENT_H

// End of File
