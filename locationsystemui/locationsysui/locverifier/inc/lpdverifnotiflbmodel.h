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
* Description: This  is a header file for verification notifier list box model
*
*/


#ifndef CLPDVERIFNOTIFLBMODEL_H
#define CLPDVERIFNOTIFLBMODEL_H

//  INCLUDES
#include "lpdbasemodel.h"

// FORWARD DECLARATIONS
class CLocPhoneNumberFormat;
class CEikFormattedCellListBox;

// CLASS DECLARATION

/**
*  @lib locverifierdlg.dll
*  @since 2.1
*/
class CLpdVerifNotifLBModel : public CLpdBaseModel
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        * @param aRtorProcessor reference owning the requestor data
        * @param aListBox reference to the listbox showing the items
        * @return created object
        */
        static CLpdVerifNotifLBModel* NewL(
                        const CLpdRequestorProcessor& aRtorProcessor,
                        CEikFormattedCellListBox& aListBox );

        /**
        * Destructor.
        */
        virtual ~CLpdVerifNotifLBModel();
        
    public: // From MDesCArray

        TInt MdcaCount() const;

    protected: // From CLpdBaseModel

        void MdcaPointL( TInt aIndex ) const;

    protected: // Constructors and destructors

        /**
        * C++ constructor.
        */
        CLpdVerifNotifLBModel( const CLpdRequestorProcessor& aRtorProcessor,
                               CEikFormattedCellListBox& aListBox );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:    // Data

        CEikFormattedCellListBox& iListBox;

        CLocPhoneNumberFormat* iFormatter; // owns

    };

#endif      // CLPDVERIFNOTIFLBMODEL_H   
            
// End of File
