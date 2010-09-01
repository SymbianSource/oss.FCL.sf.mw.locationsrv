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
* Description:  Helps retrieving request related information
*
*/


#ifndef CLPDPERIODICPROCESSOR_H
#define CLPDPERIODICPROCESSOR_H

//  INCLUDES
#include <EPos_CPosPrivacyNotifier.h>
#include <lbs/epos_cposrequestor.h>
#include "lpdsuplsettingsadapterobserver.h"


// FORWARD DECLARATIONS
class CEikonEnv;
class CPosPrivacyNotifier;
class CLpdVerifierSettingsLauncher;
class CLpdSUPLSettingsAdapter;


static TInt LinkClickedL(TAny* aPtr);

// CLASS DECLARATION

/**
 * Observer for the Periodic request. This will notify incase there
 * is change in periodic request contents
 * @lib locverifierdlg.lib
 * @since S60 v9.2tb
 */
class MLpdPeriodicRequestObserver
    {
    public:
            
        /**
         * Called when a change has been detected in the period
         * request contents.
         *
         */
        virtual void UpdateCurrentNotifierL( ) = 0;

    };
    
/**
*  This class processes and retreives information related to the request based on id
*  @lib locverifierdlg.dll
*  @since 2.1
*/
class CLpdPeriodicProcessor : public CBase, 
                        public MLpdSUPLSettingsAdapterObserver
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        * @return created object
        */
        static CLpdPeriodicProcessor* NewL( MLpdPeriodicRequestObserver& aObserver );
        
        /**
        * Two-phased constructor.
        * @return created object
        */
        static CLpdPeriodicProcessor* NewLC( MLpdPeriodicRequestObserver& aObserver );
        
        /**
        * Destructor.
        */
        virtual ~CLpdPeriodicProcessor();

    public: // New functions
    
        /**
        * Sets the Request Id to process
        * @param aRequestorId periodic Request Id / Session Id
        * @return ETrue if all requestor id's were valid, EFalse otherwise.
        */
        void SetSessionIdL( TInt64 aSessionId );
        
        /**
         * Sets the requestor name parameter
         * @param aRequestorName Requestor's name
         */
        void SetRequestorNameL( HBufC* aRequestorName);
        
        /**
        * Gets the current Session Id
        */
        
        TInt64 GetSessionId( );
        
        /**
        * Sets the Request Type of process
        * @param aRequestType Notification / Query Type
        */
        
        void SetRequestTypeL( CPosPrivacyNotifier::TRequestType aRequestType );
        
         /**
        * Sets the request id of the current request
        * @param aRequestId Id of request
        */
      
      
       void SetRequestorIdL( TInt32 aRequestorId );

      
        /**
        * Sets the Requestor to process
        * @param aRequestor periodic Requestor
        */
       void SetRequestorL( CPosRequestor* aRequestor );
       
       /**
        * Resets member variables
        */
       void ResetParameters();
        
	    /**
        * Prepares to set the message text in notification dialog
        */       
        void NotificationMessageTextL( HBufC*& aMessageText);

	    /**
        * Resets requestor arrays and destroys requestor objects.
        */
        TCallBack LinkCallBack();
        
        /**
        * Launches the Settings Ui for showing Supl periodic/Session details
        */
	    void LaunchSettingsL();
	    
	    
 
     public: // 

        /**
        * Inherited from MLpdSUPLSettingsAdapterObserver.
        */
        void UpdateSessionL( TInt64 aSessionId );
               
    protected: // Constructors and destructors

        /**
        * C++ constructor.
        */
        CLpdPeriodicProcessor( MLpdPeriodicRequestObserver& aObserver );
       
    
    protected: // New methods
        
        /**
          * Handles assert errors. Panics in debug build and leaves in
          * release build, as this component runs in EikSrv 
          * and must never panic  
          */
        void HandleAssertErrorL() const;

    private:
    		void ConstructL();
    		
    		void GetResourceAndPeriodL( TUint aSeconds, 
    		                                    TInt& aResourceId, TDes& aPeriodString );
    		void GetMessageStringL( HBufC*& aMessageString, 
    		                    TInt& aResourceId, TDesC& aNameString, TDesC& aPeriodString );
    		

        
    protected: //Data
    
        
    private: //Data
    
    // Observer for the current request
    MLpdPeriodicRequestObserver& iObserver;
        
    // Message Text to be displayed in Message Query control
    HBufC* iMessageText; //Owns
        
    HBufC* iRequestorName;
   
	  // Current session id
    TInt64 iSessionId; //Owns
    
    // Current Request Id
    TInt iRequestorId;
    
    // Current Request Type
    CPosPrivacyNotifier::TRequestType iRequestType;
    
    CPosRequestor::TRequestType iRequestorType;
   
    CLpdSUPLSettingsAdapter* iSUPLSettingsAdapter;
 
    // Settings Launcher
    CLpdVerifierSettingsLauncher* iVerifierSettingsLauncher; //Owns
   
    // Current Request's Requestor
    // :Owns
    CPosRequestor* iRequestor;
        
    CEikonEnv* iEikEnv;
        
    };

#endif      // CLPDPERIODICPROCESSOR_H   
            
// End of File
