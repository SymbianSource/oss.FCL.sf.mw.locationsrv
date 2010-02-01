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
* Description: 
*     Abstract utility class for phone number formatting.
*
*/


#ifndef CLOCPHONENUMBERFORMAT_H_
#define CLOCPHONENUMBERFORMAT_H_

//  INCLUDES
#include <e32base.h>
#include <AknUtils.h>
#include <bldvariant.hrh>

// DATA TYPES
/**
* Enumeration for numberformatting results.
*/
enum TFormatResult
    {
    EResultOK,          // No loss of precision on formatting.
    EResultTruncated,   // result descriptor truncated from the start.
    EResultClipped,     // result descriptor truncated and extra dots added to start
    };


// FORWARD DECLARATIONS
class CFont;
class CEikFormattedCellListBox;
class CEikColumnListBox;
class CAknPhoneNumberGrouping;

// CLASS DECLARATION

/**
*   Class for phone number formatting. It uses the AknPhoneNumberGrouping feature.
*	AknPhoneNumberGrouping takes care of grouping based on the feature flag
*	__SERIES60_PHONE_NUMBER_GROUPING.
*	Formatting & clipping is done using AknTextUtils helper methods.
*/
class CLocPhoneNumberFormat :public CBase
    {
    public:  // Constructors and destructor

        /**
        * Symbian OS constructor
        * @return new object
        */
        IMPORT_C static CLocPhoneNumberFormat* NewL();

        /**
        * Destructor.
        */
        virtual ~CLocPhoneNumberFormat();

    protected:

        /**
        * C++ default constructor.
        */
        CLocPhoneNumberFormat();
        
        /**
        * Symbian 2nd Phase Contructor.
        * This is always protected
        */
        void ConstructL();

    public: // New functions

        /**
        * Phone number formatting for listbox texts.
        * Uses AknTextUtils for clipping long descriptors.
        * @param aNumberToFormat phone number in string to format
        * @param aFormattedString formatted number
        * @param aClipDirection tells how to clip
        * @param aListBox reference to the listbox which shows the number
        * @param aItemIndex item index
        * @param aSubCellNumber subcell number
        * @param aIcons 0->no icons, 1->ind. arrow, 2->1 icon, etc.
        * @return TFormatResult
        */
        IMPORT_C TFormatResult PhoneNumberFormatL( const TDesC& aNumberToFormat,
            TDes& aFormattedString, AknTextUtils::TClipDirection aClipDirection,
            CEikFormattedCellListBox* aListBox, TInt aItemIndex,
            TInt aSubCellNumber, TInt aIcons );
     
        /**
         * Groups the Phone number according to the current settings
         * The ownership of the grouped string is tranferred back to the 
         * callee
         * @param aNumberToGroup Phone number to group
         * @return HBufC*        Phone number after grouping. If the number
         *                       cannot be formatted then NULL is returned
         */
        IMPORT_C HBufC* PhoneNumberGroupL( const TDesC& aNumberToGroup );
        
	private:
       /**
        * Phone number formatting.
        * @param aNumberToFormat phone number in string to format
        * @param aFormattedString formatted number
        * @return TFormatResult
        */
        TFormatResult PhoneNumberFormatL( const TDesC& aNumberToFormat,
            TDes& aFormattedString );

    private:
    	CAknPhoneNumberGrouping* iPNGEngine;
    };

#endif // CLOCPHONENUMBERFORMAT_H_
            
// End of File
