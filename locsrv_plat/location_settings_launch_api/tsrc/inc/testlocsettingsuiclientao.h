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
* Description:   Test Class For RPositionServer
*
*/



#ifndef TESTLOCSETTINGSUICLIENTAO_H
#define TESTLOCSETTINGSUICLIENTAO_H

// System Includes
#include <StifLogger.h>
#include <StifTestModule.h>
#include <StifParser.h>
#include <e32base.h>
#include <e32std.h>


// Constants
class CLocSettingsUiClient;

// CLASS DECLARATION
/**
 *  Test Class to invoke the CLocSettingsUiClient methods
 *
 *  This class is a helper class. The status variable of this active obect is 
 *  used to provide the asynchronous method Calls.
 *
 *  @lib testlbslocacquisition.lib
 *  @since S60 v3.2
 */

class CTestLocSettingsUiClientAO :public CActive
    {
    public:
    	enum TState
    		{
    		EStart = 0,
    		EExecuting,
    		ECompleted
    		};
    
    	enum TFunction
    		{
    		ELaunchPosSettingsL = 0,
    		ELaunchPosSettingsAsEmbeddedAppL,
    		ELaunchSettingsUiL,
    		ELaunchSettingsUiAsEmbeddedAppL,
    		ELaunchSettingsUiLOverLoaded,
    		ELaunchSettingsUiAsEmbeddedAppLOverLoaded
    		};
    
    public:  // Constructors and destructor

        /**
        * Destructor. 
        */
        static CTestLocSettingsUiClientAO* NewL(CStifLogger* aLog);
        
        /**
        * Destructor. 
        */
        ~CTestLocSettingsUiClientAO();

    public:         
        
        //From CActive
          void RunL();
          void DoCancel();
		  TInt RunError(TInt aError);   

		//
	    /**
	     * Test Case for create a new instance of 
	     * CLocSettingsUiClient with NewL
	     *
	     * @since S60 v3.2
	     * Returns KErrNone if successful. 
	     *
	     */ 
		  TInt TestNewL();
		
	    /**
	     * Test Case for create a new instance of 
	     * CLocSettingsUiClient with NewLC
	     *
	     * @since S60 v3.2
	     * Returns KErrNone if successful. 
	     *
	     */ 
	 	  TInt TestNewLC();
		  		  	 		
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
	 	  TInt TestCancelLaunchedSettingsUiErr();
	 	  	 	  	 	  		
	    /**
	     * Test Case for Closing a Position Settings 
	     * CLocSettingsUiClient
	     *
	     * @since S60 v3.2
	     * Returns KErrNone if successful. 
	     *
	     */ 
	 	  TInt TestClosePosSettings();
	 	  	 	  	 	  		
	    /**
	     * Test Case for Closing a Position Settings 
	     * which results in Error
	     *
	     * @since S60 v3.2
	     * Returns KErrNone if successful. 
	     *
	     */ 
	 	  TInt TestClosePosSettingsErr();
	 	  
	private:
	
		void IssueRequestL(TFunction aFunction);
		void ConstructL();
	
        /**
        * C++ default constructor.
        */
        CTestLocSettingsUiClientAO(CStifLogger* aLog);
	 	    
    private:  //data
	    /**
	     * Loc system UI Client instance
	     */     		 
    	CLocSettingsUiClient*         iLocSettingsUiClient;

	    /**
	     * iLog instance - Not owned
	     */         
        CStifLogger* 			iLog;
        
        TInt iErr;
        
        TUid iTUid;
        
        TInt iParam;
        
        TBuf<50> iParamString;
        
        TFunction iFunction;
        
        TState iState;
	        
     };     

#endif      // TESTLOCSETTINGSUICLIENTAO_H

// End of File
