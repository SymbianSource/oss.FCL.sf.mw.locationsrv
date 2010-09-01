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


// INCLUDE FILES
#include "lpdbasemodel.h"
#include <eikenv.h>
#include "locrequestorutilsresolver.h"
#include "lpdrequestorprocessor.h"
#include "locconsts.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CLpdBaseModel::CLpdBaseModel
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CLpdBaseModel::CLpdBaseModel(
    const CLpdRequestorProcessor& aRtorProcessor )
    : iRtorProcessor( aRtorProcessor )
    {
    // Intentionally empty
    }

// -----------------------------------------------------------------------------
// CLpdBaseModel::BaseConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CLpdBaseModel::BaseConstructL()
    {
    iBuffer = HBufC::NewL( KLocLBLineMaxLen + KLocLBItemFormatMaxLen );
    iUtils = CLocRequestorUtilsResolver::NewL();
    }

// Destructor
CLpdBaseModel::~CLpdBaseModel()
    {
    delete iBuffer;
    delete iUtils;
    }

// -----------------------------------------------------------------------------
// CLpdBaseModel::MdcaPoint
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TPtrC16 CLpdBaseModel::MdcaPoint( TInt aIndex ) const
    {
    TRAPD( err, MdcaPointL( aIndex ) );
    if ( err )
        {
        iBuffer->Des().Zero();
        CEikonEnv::Static()->HandleError(err);
        }
    return *iBuffer;
    }

// End of File
