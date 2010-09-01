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
* Description:  Model base class.
*
*/


#ifndef CLPDBASEMODEL_H
#define CLPDBASEMODEL_H

//  INCLUDES
#include <e32base.h>
#include <bamdesca.h>

// FORWARD DECLARATIONS
class CLpdRequestorProcessor;
class CLocRequestorUtilsResolver;

// CLASS DECLARATION

/**
*  @lib locverifierdlg.dll
*  @since 2.1
*/
class CLpdBaseModel : public CBase, public MDesCArray
    {
    public:  // Constructors and destructor
        
        /**
        * Destructor.
        */
        virtual ~CLpdBaseModel();
        
    public: // Functions from base classes
        
        // Inherited from MDesCArray.
        TPtrC16 MdcaPoint( TInt aIndex ) const;

    protected: // New functions

        // Leaving method, wrapped by MdcaPoint
        virtual void MdcaPointL( TInt aIndex ) const = 0;

    protected: // Constructors and destructors

        /**
        * C++ constructor.
        */
        CLpdBaseModel( const CLpdRequestorProcessor& aRtorProcessor );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void BaseConstructL();

    protected:    // Data
        
        const CLpdRequestorProcessor& iRtorProcessor;

        HBufC* iBuffer; // owns

        CLocRequestorUtilsResolver* iUtils; // owns
    };

#endif      // CLPDBASEMODEL_H   
            
// End of File