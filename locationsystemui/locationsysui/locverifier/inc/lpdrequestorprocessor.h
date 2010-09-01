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
* Description:  Processes requestor related data.
*
*/


#ifndef CLPDREQUESTORPROCESSOR_H
#define CLPDREQUESTORPROCESSOR_H

//  INCLUDES
#include "locrequestorutilsresolver.h"
#include <e32base.h>
#include <eiklbv.h>
#include <lbs/epos_rposrequestorstack.h>

// FORWARD DECLARATIONS
class CPosPrivacyNotifier;
class CEikFormattedCellListBox;
class CSelectionIndexArray;
class CListBoxView;
class CLpdBaseModel;

// CLASS DECLARATION

/**
*  This class reads, processes and owns requestor data.
*  @lib locverifierdlg.dll
*  @since 2.1
*/
class CLpdRequestorProcessor : public CBase
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        * @return created object
        */
        static CLpdRequestorProcessor* NewL();
        
        /**
        * Destructor.
        */
        virtual ~CLpdRequestorProcessor();

    public: // New functions

        /**
        * Reads requestor information from the privacy notifier.
        * Uses RequestorIdValid() for id validity check.
        * Calls UpdateRuleRequestorsL().
        * @param aPosNotifier reference
        * @return ETrue if all requestor id's were valid, EFalse otherwise.
        */
        TBool ReadRequestorsL( const CPosPrivacyNotifier& aPosNotifier );

        /**
        * Returns requestor array for reading.
        * @return array of requestors
        */
        const RPointerArray<CPosRequestor>& Requestors() const;
       
       /**
        * Sets Type of Request
        */
        void SetRequestType( TInt aReqType );

        /**
        * Resets requestor arrays and destroys requestor objects.
        */
        void ResetAndDestroyRequestors();

        /**
        * Method for creating a formatted array with
        * icons and requestor names.
        * Used for verifier queries and notifications.
        * @param aListBox needed for formatting data
        * @return created array, ownership is transferred to caller
        */
        CLpdBaseModel* RtorNamesForVerifNotifL(
                        CEikFormattedCellListBox& aListBox );

		/**
		* In case where the verifcation cancellation event is queued, it
		* is necessary to retrieve all the information needed for the
		* notification dialog. The information is present in the base class
		* of Query and Notification Interface.
		*/
		RPosRequestorStack* RetrieveRequestorsL( 
						const CPosPrivacyNotifier& aPosNotifier );


		/**
		* Read the requestors into the iRequestors memeber variable.
		* Process the requestors by resolving them with contacts information.
		* CLocRequestorUtilsResolver is used to resolve the requestors.
		*/
		void ReadRequestorsL( 
						const RPosRequestorStack& aRequestors );
						
       /**
		* Retrieves Requestor name from string passed
		*/
        TPtrC RetrieveRequestorNameL(  
                                    const TDesC& aString );
       /**
		* Retrieves session id from string passed
		*/
        TInt64 RetrieveSessionId(  
                              const TDesC& aString );
        /**
		* Sets session id of request
		*/
        void SetSessionId( TInt64 aSessionId );
        
        /**
		* Gets session id of request
		*/
        TInt64 GetSessionId();


		
    protected: // Constructors and destructors

        /**
        * C++ constructor.
        */
        CLpdRequestorProcessor();

    protected: // New methods

        /**
        * Handles assert errors. Panics in debug build but leaves in release
        * build, as this component runs in EikSrv and must never panic.
        */
        void HandleAssertErrorL() const;

    private:    // Data

		// Array of currently handled requestors...
        RPointerArray<CPosRequestor> iRequestors;
        
        TInt iSuplRequest;

        TInt64 iSessionId;
    };

#endif      // CLPDREQUESTORPROCESSOR_H   
            
// End of File