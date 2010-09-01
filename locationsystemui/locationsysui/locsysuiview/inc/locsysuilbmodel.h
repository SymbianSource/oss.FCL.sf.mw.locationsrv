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
* Description:  List box for the Location Setting View
*
*/

#ifndef C_LOCSYSUILBMODEL_H_
#define C_LOCSYSUILBMODEL_H_

// System Include
#include <e32base.h>
#include <bamdesca.h>  // MDesCArray

// Forward Declarations 
class CLocationSettings;

// Class Declaration
/**
 * Model for the Settings UI view container. Takes a Settings Description array
 * and creates a sorted array from it. 
 *
 * @lib locsysuiview.lib
 * @since S60 v3.1
 */
 NONSHARABLE_CLASS( CLocSysUiLBModel ): public CBase,
						                public MDesCArray
	{
	public:
        /**
         * Two-phased constructor
         * @param aSettingsArray	    Reference to the Settings UI array.
         * @return CLocSysUiLBModel*    Reference to the object created.
         */
        static CLocSysUiLBModel* NewL(
        				RPointerArray<CLocationSettings>& aSettingsArray );

        /**
        * Destructor.
        */
        virtual ~CLocSysUiLBModel();

        /**
         * Returns the Settings UI pertaining to a particular index value.
         *
         * @param	aIndex			   Index of the Settings Ui.
         * @return	CLocationSettings& Referrence to the Settings UI corresponding
         *							   to the index.
         */
        CLocationSettings&	SettingsUiL( TInt	aIndex );

        /**
         * Updates the List box contents
         *
         * @param aSettingsArray	Reference to the Settings UI array.
         */
        void UpdateListBoxContentsL( RPointerArray<CLocationSettings>& aSettingsArray );

    protected:
        /**
         * Inherited from MDesCArray
         */
        TInt MdcaCount() const;

        /**
         * Inherited from MDesCArray
         */
        TPtrC16 MdcaPoint( TInt aIndex ) const;
        
    private:
        /**
         * C++ constructor.
         */
        CLocSysUiLBModel( RPointerArray<CLocationSettings>& aSettingsArray );
        
        /**
         * Second Phase constructor
         */
        void ConstructL();

    private:
        /**
         * Reference to the array of Settings UI.
         */
		RPointerArray<CLocationSettings>& iSettingsArray;

        /**
         * Format conversion buffer
         * Owns
         */
        HBufC* iBuffer;
	};

#endif // C_LOCSYSUILBMODEL_H_

