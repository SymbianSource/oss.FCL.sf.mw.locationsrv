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
*     Default implementation for number formatting.
*
*/


// INCLUDE FILES
#include <AknUtils.h>
#include <aknlayoutdef.h>
#include <AknLayout.lag>
#include <eikfrlbd.h>
#include <AknPhoneNumberGrouping.h>
#include "locphonenumberformat.h"

// CONSTANT DEFINITIONS

// maximum phone number length
const TInt KLocPhoneNumberMaxLen = 48;

// ============= CLOCPHONENUMBERFORMAT MEMBER FUNCTIONS ===================

// ---------------------------------------------------------------------------
// CLocPhoneNumberFormat::NewL
// ---------------------------------------------------------------------------
//
EXPORT_C CLocPhoneNumberFormat* CLocPhoneNumberFormat::NewL()
	{
	CLocPhoneNumberFormat *self = new ( ELeave ) CLocPhoneNumberFormat;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

// ---------------------------------------------------------------------------
// CLocPhoneNumberFormat::ConstructL
// ---------------------------------------------------------------------------
//
void CLocPhoneNumberFormat::ConstructL()
	{
	iPNGEngine = CAknPhoneNumberGrouping::NewL(KLocPhoneNumberMaxLen);
    }

// ---------------------------------------------------------------------------
// CLocPhoneNumberFormat::CLocPhoneNumberFormat
// ---------------------------------------------------------------------------
//
CLocPhoneNumberFormat::CLocPhoneNumberFormat()
	{
    // Intentionally empty
	}

// ---------------------------------------------------------------------------
// CLocPhoneNumberFormat::~CLocPhoneNumberFormat
// ---------------------------------------------------------------------------
//
CLocPhoneNumberFormat::~CLocPhoneNumberFormat()
	{
	delete iPNGEngine;
	}

// ---------------------------------------------------------------------------
// CLocPhoneNumberFormat::PhoneNumberFormatL
// ---------------------------------------------------------------------------
//
TFormatResult CLocPhoneNumberFormat::PhoneNumberFormatL(
    const TDesC& aNumberToFormat,
    TDes& aFormattedString )
	{
	TFormatResult rc( EResultOK );
	if ( iPNGEngine->Set(aNumberToFormat) != KErrNone )
		{
		// If we cannot group the number then just return 
		// the ungrouped number
		return rc;
		}

	HBufC* groupedPhoneNumber = 
		HBufC::NewL(iPNGEngine->FormattedNumber().Length());
	// CAknPhoneNumberGrouping takes care of grouping based on the 
	// current settings. If the __SERIES60_PHONE_NUMBER_GROUPING 
	// feature flag is disabled then grouping is not done.
	
	TPtr grpPhoneNumberDesc = groupedPhoneNumber->Des();
	iPNGEngine->CopyFormattedNumber(grpPhoneNumberDesc);
	if (aFormattedString.MaxLength() < groupedPhoneNumber->Length())
		{
		// Store only as much as can be fitted into the resultant 
		// Descriptor
		rc = EResultTruncated;
		aFormattedString = 
			groupedPhoneNumber->Right( groupedPhoneNumber->Length() -
										(groupedPhoneNumber->Length() - 
										aFormattedString.MaxLength()) );
		}
	else
		{
		aFormattedString.Copy(*groupedPhoneNumber);
		}

	delete groupedPhoneNumber;
	return rc;
	}

// ---------------------------------------------------------------------------
// CLocPhoneNumberFormat::PhoneNumberGroupL
// ---------------------------------------------------------------------------
//	
EXPORT_C HBufC* CLocPhoneNumberFormat::PhoneNumberGroupL( const TDesC& aNumberToGroup )
    {
	if ( iPNGEngine->Set(aNumberToGroup) != KErrNone )
		{
		// If we cannot group the number then just return NULL
		return NULL;
		}

	HBufC* groupedPhoneNumber = HBufC::NewL(iPNGEngine->FormattedNumber().Length());
		
	// CAknPhoneNumberGrouping takes care of grouping based on the 
	// current settings. If the __SERIES60_PHONE_NUMBER_GROUPING 
	// feature flag is disabled then grouping is not done.
	
	TPtr grpPhoneNumberDesc = groupedPhoneNumber->Des();
	iPNGEngine->CopyFormattedNumber( grpPhoneNumberDesc );
	
	return groupedPhoneNumber;	
    }

// ---------------------------------------------------------------------------
// CLocPhoneNumberFormat::PhoneNumberFormatL
// ---------------------------------------------------------------------------
//
EXPORT_C TFormatResult CLocPhoneNumberFormat::PhoneNumberFormatL(
    const TDesC& aNumberToFormat,
    TDes& aFormattedString,
    AknTextUtils::TClipDirection aClipDirection,
    CEikFormattedCellListBox *aListBox,
    TInt aItemIndex,
    TInt aSubCellNumber,
    TInt aIcons )
	{
	TFormatResult rc = PhoneNumberFormatL( aNumberToFormat, aFormattedString );
    if (aIcons == 0)
    	{
  		// There is no Icon so just use the AknTextUtils API
   		if( AknTextUtils::ClipToFit( aFormattedString, aClipDirection, 
   										aListBox, aItemIndex,
   										aSubCellNumber ))
   			{
   			rc = EResultClipped;
   			}
    	}
	else
		{
		// Adjust for the Icon Space.
	    CFormattedCellListBoxData *data = 
	        aListBox->ItemDrawer()->FormattedCellData();
	    const CFont *font = data->Font( aListBox->ItemDrawer()->Properties( 
	                                            aItemIndex ), aSubCellNumber );
		
		//Obtain the layout parameters for this element from the Avkon Layout

	    TAknLayoutText textLayout;
	    textLayout.LayoutText(aListBox->Rect() , 
			AKN_LAYOUT_TEXT_List_pane_texts__menu_single__Line_1(aIcons));
	     
		TRect rect = textLayout.TextRect();
	    TInt width = rect.Width();

		TInt clipgap = data->SubCellTextClipGap( aSubCellNumber );

		if ( AknTextUtils::ClipToFit( aFormattedString, *font, width,
	                                  aClipDirection, width + clipgap ) )
			{
			rc = EResultClipped;
			}
		}
    return rc;
    }

//  End of File
