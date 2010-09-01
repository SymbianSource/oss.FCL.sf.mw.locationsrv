/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
#include <cntdb.h>                  // MIdleFindObserver
#include <PbkFields.hrh>            // TPbkFieldId
#include <centralrepository.h>
#include <cenrepnotifyhandler.h>

//FORWARD DECLARATION
class CPbkFieldIdArray;
class CPbkIdleFinder;
class CPbkContactEngine;
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
class CLocRequestorUtilsResolver  :	public CBase,
                                    public MCenRepNotifyHandlerCallback 
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
		    
    private: // Constructors
        /**
        * C++ default constructor.
        */
        CLocRequestorUtilsResolver();

		/**
		* The Symbian 2nd Phase Constructor is always Private
		*/
		void ConstructL();

    private:
        /** 
        * From base class MCenRepNotifyHandlerCallback
        * This callback method is used to notify the client about
        * changes for integer value keys, i.e. key type is EIntKey.
        *
        * @param aId Id of the key that has changed.
        * @param aNewValue The new value of the key.
        */
        virtual void HandleNotifyInt( TUint32 aId, TInt aNewValue );

        /** 
        * This callback method is used to notify the client about errors
        * in the handler. Any error in handling causes the handler to stop
        * handling any more notifications.
        *
        * @param aId Id of the key this instance listens for or if 
        *            notifications for whole repository are listened, could 
        *            also be KInvalidNotificationId.
        * @param aError Error code.
        * @param aHandler Pointer to the handler instance. 
        */
        virtual void HandleNotifyError( TUint32 aId, TInt aError, 
                                        CCenRepNotifyHandler* aHandler );

    private: // New Methods
		/**
        * This resolves the requestor by searching the contacts database
        * based on the requestor Id Information.
        * This searches only the default contact database now.
        */
        void ResolveRequestorNameL( CPosRequestor& aRequestor );

		/**
        * This resolves the requestor by searching the contacts database
        * based on Phone Number. This searches only the default contact
        * database now.
        */
		void SearchByPhoneNumberL( CPosRequestor& aRequestor );

		/**
        * This resolves the requestor by searching the contacts database
        * based on Other Fields. Used to search E-mail , SIP URL and Web URL 
        * type fields. This searches only the default contact database now.
        */
		void SearchByOtherFieldsL( CPosRequestor& aRequestor, 
		                           TPbkFieldId aFieldType );

		/**
        * This resolves the requestor by searching for the requestor 
        * string in al the fields in the contacts database.
        * This searches only the default contact database now.
        */
		void SearchByAllFieldsL( CPosRequestor& aRequestor );

		/**
		* The Contact Searchs based on other fields can return contact cards
		* that do not match in the specified field but in some other field.
		* This method checks if the match occurs in the exact specified field.
		*/
		TBool IsContactExactRequestorMatchL( CPosRequestor& aRequestor,
								             TContactItemId& aCntItemId,
								             TPbkFieldId aFieldType );

		/**
		* This sets the contact card name in the requestor.
		* Also the Requestor Format is changes to GenericName
		*/
		void SetContactNameToRequestorL( CPosRequestor& aRequestor,
									     TContactItemId& aCntItemId );
			
    private: // member data
    	// Own : Handle to the Contact Engine used to resolve the requestors
		CPbkContactEngine* iPbkContactEngine;

        // Central repository instance.
        CRepository* iCenRepSession;

        // Central repository notify handler instance.
        CCenRepNotifyHandler* iCenRepNotifyHandler;
        
        // Matched numbers
        TInt iNumDigitsToMatch;
    };

#endif // CLOCREQUESTORUTILSRESOLVER_H_
            
// End of File
