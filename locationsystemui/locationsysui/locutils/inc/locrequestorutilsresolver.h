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
#include <e32base.h>

//FORWARD DECLARATION
class CPosRequestor;

// CLASS DECLARATION

/**
*   This class is a version requestor utilities with resolving.
*  @lib locutils.lib
*  @since 3.0
*/
class CLocRequestorUtilsResolver  :	public CBase
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
    };

#endif // CLOCREQUESTORUTILSRESOLVER_H_
            
// End of File
