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
* Description:  Class which handles the dialog and expiration timer.
*
*/


// INCLUDE FILES
#include "lpdverifierquerylauncher.h"
#include "lpdglobalplugindialog.h"
#include "lpdresulthandler.h"
#include "locverifierdlg.hrh"
#include <locverifierdlg.rsg>
#include <avkon.hrh>

// CONSTANTS

const TInt KNonPeriodicRequest = 0;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CLpdVerifierQueryLauncher::CLpdVerifierQueryLauncher
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CLpdVerifierQueryLauncher::CLpdVerifierQueryLauncher( 
    MLpdResultHandler& aHandler ) 
    : CLpdQueryLauncherBase( aHandler )
    {
    // Intentionally empty
    }

// -----------------------------------------------------------------------------
// CLpdVerifierQueryLauncher::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CLpdVerifierQueryLauncher::ConstructL()
    {
    BaseConstructL();
    }

// -----------------------------------------------------------------------------
// CLpdVerifierQueryLauncher::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CLpdVerifierQueryLauncher* CLpdVerifierQueryLauncher::NewL( 
    MLpdResultHandler& aHandler )
    {
    CLpdVerifierQueryLauncher* self =
        new( ELeave ) CLpdVerifierQueryLauncher( aHandler );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(self);

    return self;
    }

// Destructor
CLpdVerifierQueryLauncher::~CLpdVerifierQueryLauncher()
    {
    // Intentionally empty
    }


// -----------------------------------------------------------------------------
// CLpdVerifierQueryLauncher::PrepareVerificationResourcesL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdVerifierQueryLauncher::PrepareVerificationResourcesL(
    TPosRequestSource aSource,
    TPosRequestDecision aDefault)
    {
    if ( aSource == EPosRequestSourceNetwork && 
         aDefault == EPosDecisionAccepted )
        {
        PrepareQueryResourcesL( R_LPD_GLOBAL_VERIFICATION_DEFAULT_A, KNonPeriodicRequest );
        }
    else if ( aSource == EPosRequestSourceNetwork && 
              aDefault == EPosDecisionRejected )
        {
        PrepareQueryResourcesL( R_LPD_GLOBAL_VERIFICATION_DEFAULT_R, KNonPeriodicRequest );
        }
    else
        { // Terminal request, EPosDecisionNotAvailable or future extensions
        PrepareQueryResourcesL( R_LPD_GLOBAL_VERIFICATION_DEFAULT_N, KNonPeriodicRequest );
        }

    iQueryDlg->SetTone( EAvkonSIDLocationRequest );
    }

// -----------------------------------------------------------------------------
// CLpdVerifierQueryLauncher::PrepareSuplVerificationResourcesL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdVerifierQueryLauncher::PrepareSuplVerificationResourcesL()
{
  PrepareQueryResourcesL( R_LPD_SUPL_PERIODIC_VERIFICATION_DEFAULT, KNonPeriodicRequest );
  iQueryDlg->SetTone( EAvkonSIDLocationRequest );
}

// -----------------------------------------------------------------------------
// CLpdVerifierQueryLauncher::ExtendedHandleDlgDismissedL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdVerifierQueryLauncher::ExtendedHandleDlgDismissedL()
    {
    TInt retValue = KErrNone;
    switch ( iResult )
        {
        case EAknSoftkeyOk:
            {
            retValue = KErrNone;
            break;
            }
        case EAknSoftkeyCancel:
            {
            retValue = KErrAccessDenied;
            break;
            }
        case EEikCmdExit:
            {
            retValue = iExitReason;
            break;
            }
        case ELpdCmdEmergency:
            { // emergency call support
            retValue = KErrAbort;
            break;
            }
        default:
            {
            retValue = KErrGeneral;
            break;
            }
        }
    iHandler.HandleVerificationResultL( retValue );
    }

//  End of File
