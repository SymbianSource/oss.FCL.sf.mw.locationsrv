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
* Description:  Abstract base class for requestor utilities.
*
*/


// INCLUDE FILES
#include "loctextutils.h"
#include <AknLayoutDef.h>
#include <AknLayout.lag>
#include <eikfrlbd.h>


// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------------------------
// LocTextUtils::ClipToFit
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//      
EXPORT_C TBool LocTextUtils::ClipToFit(
    TDes& aText,
    AknTextUtils::TClipDirection aClipDirection,
    CEikFormattedCellListBox* aListBox,
    TInt aItemIndex,
    TInt aSubCellNumber,
    TInt aIcons )
    {
    if (aIcons == 0)
    	{
    	// There is no Icon so just use the AknTextUtils API
   		return AknTextUtils::ClipToFit( aText, aClipDirection, 
   										aListBox, aItemIndex, 
   										aSubCellNumber );
    	}
    CFormattedCellListBoxData *data = 
        aListBox->ItemDrawer()->FormattedCellData();
    const CFont *font = data->Font( aListBox->ItemDrawer()->Properties( 
                                            aItemIndex ), aSubCellNumber );
	
	// Obtain the layout parameters for this element from the Avkon Layout

    TAknLayoutText textLayout;
    textLayout.LayoutText(aListBox->Rect() , 
		AKN_LAYOUT_TEXT_List_pane_texts__menu_single__Line_1(aIcons));
     
	TRect rect = textLayout.TextRect();
    TInt width = rect.Width();

    TInt clipgap = data->SubCellTextClipGap( aSubCellNumber );

	return AknTextUtils::ClipToFit( aText, *font, width,
                                  aClipDirection, width + clipgap );
    }

//  End of File
