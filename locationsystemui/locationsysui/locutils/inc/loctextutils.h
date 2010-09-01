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
* Description:  Descriptor manipulation utilities.
*
*/


#ifndef LOCTEXTUTILS_H_
#define LOCTEXTUTILS_H_

//  INCLUDES
#include <e32base.h>
#include <AknUtils.h>

// FORWARD DECLARATIONS


// CLASS DECLARATION

/**
*  Static class containing descriptor manipulation utilities.
*  @lib locutils.lib
*  @since 2.1
*/
class LocTextUtils
    {
    public: // New functions
        /**
        * Clips listbox data.
        * Use this only if you have icons, otherwise AknTextUtils.
        * @param aOriginal original text that is not modified
        * @param aClipped modified text
        * @param aClipDirection tells how to clip
        * @param aListBox reference to the listbox which shows the data
        * @param aItemIndex item index
        * @param aSubCellNumber subcell number
        * @param aIcons 0->no icons, 1->ind. arrow, 2->1 icon, etc.
        * @return ETrue if clipping occurred
        */
        IMPORT_C static TBool ClipToFit( TDes& aText,
            AknTextUtils::TClipDirection aClipDirection,
            CEikFormattedCellListBox* aListBox,
            TInt aItemIndex,
            TInt aSubCellNumber,
            TInt aIcons );
    };

#endif // LOCTEXTUTILS_H_
            
// End of File
