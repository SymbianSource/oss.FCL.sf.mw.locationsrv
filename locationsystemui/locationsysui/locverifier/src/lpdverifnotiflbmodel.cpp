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
* Description:  Listbox model representing all requestors.
*
*/


// INCLUDE FILES
#include "lpdverifnotiflbmodel.h"
#include "locrequestorutilsresolver.h"
#include "lpdrequestorprocessor.h"
#include "locphonenumberformat.h"
#include "loctextutils.h"
#include "locconsts.h"
#include <locverifierdlg.rsg>
#include <eikenv.h>

// Unnamed namespace for local definitions

const TInt KPostfixLBNameCellNbr = 0; // postfix icon
// This fixed the space for icons in case icons are needed in verifier notifiers
// Change this value to 2 if an icon is to be displayed.
const TInt KLBIconMargin = 0; // space for zero icon


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CLpdVerifNotifLBModel::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CLpdVerifNotifLBModel* CLpdVerifNotifLBModel::NewL(
    const CLpdRequestorProcessor& aRtorProcessor,
    CEikFormattedCellListBox& aListBox )
    {
    CLpdVerifNotifLBModel* self =
        new( ELeave ) CLpdVerifNotifLBModel( aRtorProcessor, aListBox );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(self);

    return self;
    }

// -----------------------------------------------------------------------------
// CLpdVerifNotifLBModel::CLpdVerifNotifLBModel
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CLpdVerifNotifLBModel::CLpdVerifNotifLBModel(
    const CLpdRequestorProcessor& aRtorProcessor,
    CEikFormattedCellListBox& aListBox )
    : CLpdBaseModel( aRtorProcessor ),
      iListBox( aListBox )
    {
    // Intentionally empty
    }

// -----------------------------------------------------------------------------
// CLpdVerifNotifLBModel::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CLpdVerifNotifLBModel::ConstructL()
    {
    BaseConstructL();
    iFormatter = CLocPhoneNumberFormat::NewL();
    }

// Destructor
CLpdVerifNotifLBModel::~CLpdVerifNotifLBModel()
    {
    delete iFormatter;
    }

// -----------------------------------------------------------------------------
// CLpdVerifNotifLBModel::MdcaCount
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CLpdVerifNotifLBModel::MdcaCount() const
    {
    TInt count = 0;
    count = iRtorProcessor.Requestors().Count();

    if ( count == 0 )
        { // if there are no requestors we must add unknown requestor
        count = 1;
        }
    return count;
    }

// -----------------------------------------------------------------------------
// CLpdVerifNotifLBModel::MdcaPointL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdVerifNotifLBModel::MdcaPointL( TInt aIndex ) const
    {
        
        
    if( aIndex >= MdcaCount() )
        {
        User::Leave( KErrArgument );
        }

    TPtr bufferDes = iBuffer->Des();
    HBufC* name = NULL;

    if ( iRtorProcessor.Requestors().Count() == 0 ) // unknown requestor
        {
        name = CEikonEnv::Static()->AllocReadResourceLC(
                                    R_LPD_UNKNOWN_REQUESTER );
        // No icons, no clipping, everything surely ok:
        bufferDes.Zero();
        bufferDes.Append( *name );
        }

    else
        {
        const CPosRequestor& requestor =
            *( iRtorProcessor.Requestors() )[aIndex];
        name = iUtils->RequestorNameL( requestor );
        CleanupStack::PushL( name );
        TPtr nameDes = name->Des();
        if ( iUtils->PhoneNumberAsName( requestor ) )
            {
            iFormatter->PhoneNumberFormatL( nameDes, bufferDes,
                                           AknTextUtils::EClipFromBeginning,
                                           &iListBox, aIndex, 
                                           KPostfixLBNameCellNbr,
                                           KLBIconMargin );
            }
        else
            {
            LocTextUtils::ClipToFit( nameDes, AknTextUtils::EClipFromEnd,
                    &iListBox, aIndex, KPostfixLBNameCellNbr, KLBIconMargin );
            bufferDes.Zero();
            bufferDes.Append( *name ); // this is safe after clipping
            }
        }

    CleanupStack::PopAndDestroy(name);
    }

// End of File
