/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Requestor utilities, supports Resolving of Requestors.
*
*/


#ifndef CLOCREQUESTORUTILSRESOLVER_H_
#define CLOCREQUESTORUTILSRESOLVER_H_

//  INCLUDES
#include <MVPbkContactFindObserver.h>
#include <MVPbkSingleContactOperationObserver.h>
#include <MVPbkContactLink.h>
#include <MVPbkContactStoreListObserver.h>
#include <MVPbkContactStoreObserver.h>
#include <eikenv.h>

//FORWARD DECLARATION
class CVPbkContactManager;
class CActiveSchedulerWait;
class CPosRequestor;
class MVPbkContactOperationBase;
class MVPbkContactLinkArray;
class MVPbkStoreContact;
class MVPbkFieldTypeList;
class CPbk2SortOrderManager;
class MPbk2ContactNameFormatter;

// CLASS DECLARATION

/**
*   This class is a version requestor utilities with resolving.
*  @lib locutils.lib
*  @since 3.0
*/
class CLocRequestorUtilsResolver  :	public CActive,
                                    public MVPbkContactFindObserver,
                                    public MVPbkSingleContactOperationObserver,
                                    public MVPbkContactStoreListObserver
    {
    public: // Constructors and destructor

        /**
        * Symbian OS constructor
        * @param None
        * @return new object
        */
        IMPORT_C static CLocRequestorUtilsResolver* NewL();

        /**
        * Destructor.
        */
        virtual ~CLocRequestorUtilsResolver();

	public: // Implement base class virtual methods
       /**
        * Processes the requestors. In this case there is no processing done.
        * @param aRequestors Array of Requestors
        * @return name descriptor, ownership is transferred to caller
        */
        IMPORT_C void ProcessRequestorsL( 
        				RPointerArray<CPosRequestor>& aRequestors );

        /**
        * This static method tells if requestor's name should be formatted
        * according to phone number formatting rules.
        * @param aRequestor who's name and id are investigated
        * @return ETrue if requestor has no name and id is of type 
        *               phone number.
        */
        IMPORT_C static TBool PhoneNumberAsName(
                        const CPosRequestor& aRequestor );

        /**
        * Static method for checking whether requestor id is valid or not.
        * Valid id means that a contact or service item can be created.
        * @param aRequestor requestor to be checked
        * @return ETrue if requestor id is valid, EFalse otherwise
        */
        IMPORT_C static TBool RequestorIdValid(
                        const CPosRequestor& aRequestor );

        /**
        * Returns requestor name. In this case the requestor Id String.
        * @param aRequestor Reference to the requestor for which
        *					the name is required.
        * @return name descriptor, ownership is transferred to caller
        */
        IMPORT_C HBufC* RequestorNameL( const CPosRequestor& aRequestor );

    protected: // From Base class MVPbkContactFindObserver.
       /**
        * Called when find is complete. The ownership of results in transferred
        * to this class.
        * In case of an error during find, the aResults may contain only 
        * partial results of the find.
        *
        * @param aResults Array of contact links that matched the find.
        *                 Callee must take ownership of this object in
        *                 the end of the function, ie. in case the function
        *                 does not leave.
        */
        virtual void FindCompleteL(MVPbkContactLinkArray* aResults);

       /**
        * Called in case the find fails for some reason.
        * 
        * @param aError One of the system wide error codes.
        */
        virtual void FindFailed(TInt aError);
    
    protected: // From Base class MVPbkSingleContactOperationObserver
       /**
        * Called when operation is completed.
        *
        * @param aOperation the completed operation.
        * @param aContact  the contact returned by the operation.
        *                  Client must take the ownership immediately.
        *
        */
        virtual void VPbkSingleContactOperationComplete(
                MVPbkContactOperationBase& aOperation,
                MVPbkStoreContact* aContact);

       /**
        * Called if the operation fails.
        *
        * @param aOperation    the failed operation.
        * @param aError        error code of the failure.
        */
        virtual void VPbkSingleContactOperationFailed(
                MVPbkContactOperationBase& aOperation, 
                TInt aError);

    protected: //From base class MVPbkContactStoreListObserver
        /**
         * Called when the opening process is complete, 
         * ie. all stores have been reported
         * either failed or successful open.
         */
        virtual void OpenComplete();
    
    protected: //From base class MVPbkContactStoreObserver
        /**
         * Called when a contact store is ready to use.
         */
        virtual void StoreReady(MVPbkContactStore& aContactStore);

        /**
         * Called when a contact store becomes unavailable.
         * Client may inspect the reason of the unavailability and decide whether or not
         * it will keep the store opened (ie. listen to the store events).
         * @param aContactStore The store that became unavailable.
         * @param aReason The reason why the store is unavailable.
         *                This is one of the system wide error codes.
         */
        virtual void StoreUnavailable(MVPbkContactStore& aContactStore, TInt aReason);

        /**
         * Called when changes occur in the contact store.
         * @see TVPbkContactStoreEvent
         *
         * @param aStoreEvent Event that has occured.
         */
        virtual void HandleStoreEventL(
                MVPbkContactStore& aContactStore, 
                TVPbkContactStoreEvent aStoreEvent);
    
    protected:
        /**
        * From Base class CActive
        * This method is called by the ActiveScheduler when the asynchronous
        * operation gets completed.
        */
        virtual void RunL();

        /**
        * From Base class CActive
        * If RunL leaves then it is handled in RunError.
        * @param aError the ErrorCode on which RunL did a Leave.
        */
        virtual TInt RunError(TInt aError);

        /**
        * From Base class CActive
        * DoCancel is called when the operation performed by the Active Object
        * needs to be cancelled.
        */
    	virtual void DoCancel();
    
    private: // Constructors
        /**
        * C++ default constructor.
        */
        CLocRequestorUtilsResolver();

		/**
		* The Symbian 2nd Phase Constructor is always Private
		*/
		void ConstructL();

    private: // New Methods
		/**
        * This resolves the requestor by searching the contacts database
        * based on the requestor Id Information.
        * This searches only the default contact database now.
        */
        void ResolveRequestorNameL(CPosRequestor& aRequestor);

		/**
        * This resolves the requestor by searching the contacts database
        * based on Phone Number.
        * This searches only the default contact database now.
        */
		void ResolveRequestorByPhoneNumberL(
								CPosRequestor& aRequestor);

		/**
        * This resolves the requestor by searching the contacts database
        * based on Other Fields. Used to search based on E-mail and URL 
        * type. This searches only the default contact database now.
        */
		void ResolveRequestorByOtherFieldsL(
								CPosRequestor& aRequestor,
								MVPbkFieldTypeList* aFieldType);

		/**
		* This sets the contact card name in the requestor.
		* Also the Requestor Format is changes to GenericName
		*/
		void SetContactNameToRequestorL(TInt aCntLinkArrayIndex);

    private: // Helper methods
		/**
		* Schedule this dumy Active object to run again.
		*/
        void ScheduleAORun();
        
		/**
		* Complete the Contact resolving operation.
		*/
        void CompleteRequest();
    
    private: // State of Contacts Resolving
        enum TContactResolverState
            {
            KNoProcessing = 0,
            KResolveRequestor,
            KMatchedContacts,
            KRetrievedContactInfo,
            KResolvingCompleted
            };
    
    private: // member data
        // Ref : The current requestor stack that is being resolved.
    	RPointerArray<CPosRequestor>* iRequestors;
    	
    	// Own : Handle to the Contact Engine used to resolve the requestors
		CVPbkContactManager* iPbkContactEngine;
		
		// Own: Parameter for the MPbk2ContactNameFormatter
        CPbk2SortOrderManager* iPbkSortManager;
        
		// Own: Parameter for the Find API
        MPbk2ContactNameFormatter* iPbkCntFormatter;
        
		// Own : ActiveSchedulerWait Loop.
		CActiveSchedulerWait iWait;
		
		// Own : Handle to the Current operation being performed
		MVPbkContactOperationBase* iCntsOperation;
		
		// Own : the Link to the matched contacts
		MVPbkContactLinkArray* iCntLinkArray;
		
		// Own: The Contact retrieved
		MVPbkStoreContact* iContact;

        // The current state of contacts resolving
        TContactResolverState iState;
        
		// The index of the current requestor that is being processed.
		TInt iCount;
    
        // The offset returned by the AddResourceFile
        // Used to delete the resource file.
        TInt iResourceOffset;
        
        // Eikon Env
        CEikonEnv* iEnv;
    };

#endif // CLOCREQUESTORUTILSRESOLVER_H_
            
// End of File
