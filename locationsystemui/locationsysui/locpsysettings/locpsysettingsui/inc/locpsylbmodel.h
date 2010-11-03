/*
* Copyright (c) 2002,2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Provides Location request detail view listbox model.
*
*/

#ifndef CLOCPSYLBMODEL_H
#define CLOCPSYLBMODEL_H

// INCLUDES
#include <e32base.h>        // CBase
#include <bamdesca.h>       // MDesCArray

// FORWARD DECLARATIONS
class MLocPsySettings;

// CLASS DECLARATION

/**
*  CLocPsyLBModel  
*  
*/
class CLocPsyLBModel : public CBase, public MDesCArray
    {
    private:
        /**
        * C++ constructor.
        * @param aListBox where the model is used, not own
        * @param aEntry pointer to log entry instance, not own
        * @param aCLocRequestorUtilsBase requestor utility
        */
        CLocPsyLBModel( MLocPsySettings* aPsySettingModel );
        
    public: 
        /**
        * Destructor
        */
        virtual ~CLocPsyLBModel();
          
        /**
        * Two-phased constructor
        * @param aPsySettingsModel, provides data about the Psy modules
        */
        static CLocPsyLBModel* NewLC( MLocPsySettings* aPsySettingModel );

        /**
        * Two-phased constructor
        * @param aPsySettingsModel, provides data about the Psy modules
        */
        static CLocPsyLBModel* NewL( MLocPsySettings* aPsySettingModel );

    private: 
        /**
        * Symbian default constructor.
        */
        void ConstructL();

    private: // From MDesCArray
        TInt    MdcaCount() const;
        TPtrC16 MdcaPoint( TInt aIndex ) const;

    private: //data
        // Ref: instance pointer
        MLocPsySettings*            iPsySettingsModel;
        /// Own: conversion buffer
        HBufC*                      iBuffer;
    };

#endif // CLOCPSYLBMODEL_H

// End of File
