/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Operation for removing landmarks from a database.
*
*
*/


#ifndef CPOSLMLOCALREMOVELANDMARKSOP_H
#define CPOSLMLOCALREMOVELANDMARKSOP_H

//  INCLUDES
#include <EPos_Landmarks.h>
#include "EPos_CPosLmLocalModifyOp.h"

// CONSTANTS

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
* Operation for removing landmarks from a database. Also provides a static
* method for removing a single landmark.
*
*  @lib eposlmlocalaccess.lib
*/
NONSHARABLE_CLASS(CPosLmLocalRemoveLandmarksOp) : public CPosLmLocalModifyOp
    {

    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * Use this to remove a set of landmarks.
        * @param aDb A landmark database.
        * @param aLandmarkIdArray The landmarks to remove.
        */
        static CPosLmLocalRemoveLandmarksOp* NewL(
        /* IN */        CPosLmLocalDatabase& aDb,
        /* IN */        const RArray<TPosLmItemId>& aLandmarkIdArray
        );

        /**
        * Two-phased constructor.
        * Use this to remove all landmarks.
        * @param aDb A landmark database.
        */
        static CPosLmLocalRemoveLandmarksOp* NewL(
        /* IN */        CPosLmLocalDatabase& aDb
        );

        /**
        * Destructor.
        */
        virtual ~CPosLmLocalRemoveLandmarksOp();

    public: // Functions from base classes

        /**
        * From CPosLmLocalModifyOp.
        *
        * @param aProgress The current progress.
        */
        void DoNextStepPreL(
        /* OUT */       TReal32& aProgress
        );

        /**
        * From CPosLmLocalModifyOp.
        *
        * @param aProgress The current progress.
        * @return ETrue if a begin transaction should be made. EFalse otherwise.
        */
        TBool DoNextStepPostL(
        /* OUT */       TReal32& aProgress
        );

        /**
        * From CPosLmLocalModifyOp.
        *
        * @return Current step size.
        */
        TReal32 Step();

        /**
        * From CPosLmLocalModifyOp.
        *
        * @param aResourceId A resource id.
        * @param aNoOfSubOperations A number of sub operations.
        */
        void CheckResourceValue(
        /* IN */    TInt aResourceId,
        /* IN */    TInt aNoOfSubOperations
        );

        /**
        * From CPosLmLocalOperation;
        *
        * @param aError An error code generated by NextStepL.
        */
        void HandleError(
        /* IN/OUT */    TInt& aError
        );

    private:

        /**
        * C++ default constructor.
        * @param aDb A landmark db.
        */
        CPosLmLocalRemoveLandmarksOp(
        /* IN */        CPosLmLocalDatabase& aDb
        );

        /**
        * By default Symbian 2nd phase constructor is private.
        * @param aLandmarkIdArray The landmarks to remove.
        * @param aResourceId A resource id representing the type of operation.
        */
        void ConstructL(
        /* IN */        const RArray<TPosLmItemId>& aLandmarkIdArray,
        /* IN */        TInt aResourceId
        );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
        * Rollback if in transaction and generate an event.
        * @param aProgress Current progress.
        */
        void RollbackAndGenerateEventIfNeeded(
        /* IN */        TReal32 aProgress
        );

        /**
        * Generates an event.
        * @param aProgress Current progress.
        * @return KErrNone if successful. Otherwise any system-wide error code.
        */
        TInt GenerateEventIfNeeded(
        /* IN */        TReal32 aProgress
        );

        // Prohibit copy constructor if not deriving from CBase.
        CPosLmLocalRemoveLandmarksOp(
            const CPosLmLocalRemoveLandmarksOp&);
        // Prohibit assigment operator if not deriving from CBase.
        CPosLmLocalRemoveLandmarksOp& operator=(
            const CPosLmLocalRemoveLandmarksOp&);

    private:    // Data

        // The category to add to the set of landmarks.
        TPosLmItemId iCategoryId;

        // The landmarks to add the category to.
        RArray<TPosLmItemId> iLandmarkIdArray;

        // The current landmark handled in the id array.
        TInt iCurrent;

    };

#endif      // CPOSLMLOCALREMOVELANDMARKSOP_H

// End of File
