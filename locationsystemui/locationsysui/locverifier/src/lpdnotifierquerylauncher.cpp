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
#include "lpdnotifierquerylauncher.h"
#include "lpdglobalplugindialog.h"
#include "lpdmessageplugindialog.h"
#include "lpdresulthandler.h"
#include "locverifierdlg.hrh"
#include <locverifierdlg.rsg>
#include <avkon.hrh>
#include <lbs/epos_privacy.h>

// CONSTANTS
const TInt KNonPeriodicRequest = 0;
const TInt KPeriodicRequest = 1;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CLpdNotifierQueryLauncher::CLpdNotifierQueryLauncher
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CLpdNotifierQueryLauncher::CLpdNotifierQueryLauncher( 
    MLpdResultHandler& aHandler ) 
    : CLpdQueryLauncherBase( aHandler )
    {
    // Intentionally empty
    }

// -----------------------------------------------------------------------------
// CLpdNotifierQueryLauncher::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CLpdNotifierQueryLauncher::ConstructL()
    {
    BaseConstructL();
    }

// -----------------------------------------------------------------------------
// CLpdNotifierQueryLauncher::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CLpdNotifierQueryLauncher* CLpdNotifierQueryLauncher::NewL( 
    MLpdResultHandler& aHandler )
    {
    CLpdNotifierQueryLauncher* self = new( ELeave )
             CLpdNotifierQueryLauncher( aHandler );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(self);

    return self;
    }

// Destructor
CLpdNotifierQueryLauncher::~CLpdNotifierQueryLauncher()
    {
    // Intentionally empty
    }

// -----------------------------------------------------------------------------
// CLpdNotifierQueryLauncher::PrepareNotificationResourcesL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdNotifierQueryLauncher::PrepareNotificationResourcesL(
    TPosNotificationReason aReason,
    TPosRequestDecision aDecision)
    {
    switch ( aReason )
        {
        case EPosDecisionByRequestSource:
            {
            PrepareQueryResourcesL( R_LPD_GLOBAL_NOTIFICATION_A, KNonPeriodicRequest );
            break;
            }
        case EPosVerificationTimeout:
            {
            switch ( aDecision )
                {
                case EPosDecisionAccepted:
                    {
                    PrepareQueryResourcesL( R_LPD_GLOBAL_NOTIFICATION_FAIL_A, KNonPeriodicRequest );
                    break;
                    }
                case EPosDecisionRejected:
                    {
                    PrepareQueryResourcesL( R_LPD_GLOBAL_NOTIFICATION_FAIL_R, KNonPeriodicRequest );
                    break;
                    }
                default:
                    { // EPosDecisionNotAvailable or future extensions
                    User::Leave( KErrArgument );
                    break;
                    }
                }
            break;
            }
        default:
            { // other values do not make sense
            User::Leave( KErrArgument );
            break;
            }
        }

    iQueryDlg->SetTone( EAvkonSIDLocationRequest );
    }

// -----------------------------------------------------------------------------
// CLpdNotifierQueryLauncher::PrepareSuplNotificationResourcesL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdNotifierQueryLauncher::PrepareSuplNotificationResourcesL(
        TPosNotificationReason aReason)
    {
    switch ( aReason )
        {
        case EPosDecisionByRequestSource:
            {
            PrepareQueryResourcesL( R_LPD_SUPL_PERIODIC_NOTIFICATION, KPeriodicRequest); 
            break;
            }
        case EPosVerificationTimeout:
            {
            break;
            }
        default:
            { // other values do not make sense
            User::Leave( KErrArgument );
            break;
            }
        }
    }
        
// -----------------------------------------------------------------------------
// CLpdNotifierQueryLauncher::PrepareCancelNotifResourcesL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdNotifierQueryLauncher::PrepareCancelNotifResourcesL(
    TPosRequestDecision aDecision)
    {
    switch ( aDecision )
        {
        case EPosDecisionAccepted:
            {
            PrepareQueryResourcesL( R_LPD_GLOBAL_NOTIFICATION_TIMEOUT_A, KNonPeriodicRequest );
            break;
            }
        case EPosDecisionRejected:
            {
            PrepareQueryResourcesL( R_LPD_GLOBAL_NOTIFICATION_TIMEOUT_R, KNonPeriodicRequest );
            break;
            }
        default:
            { // EPosDecisionNotAvailable or future extensions
            User::Leave( KErrArgument );
            break;
            }
        }

    iQueryDlg->SetTone( EAvkonSIDLocationRequest );
    }

// -----------------------------------------------------------------------------
// CLpdNotifierQueryLauncher::PrepareCancelNotifResourcesL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdNotifierQueryLauncher::PrepareSuplCancelNotifResourcesL(
    TPosRequestDecision aDecision )
    {
    switch ( aDecision )
        {
        case EPosDecisionAccepted:
            {
            PrepareQueryResourcesL( R_LPD_SUPL_PERIODIC_NOTIFICATION, KPeriodicRequest );
            break;
            }
        case EPosDecisionRejected:
            {
            break;
            }
        default:
            { // EPosDecisionNotAvailable or future extensions
            User::Leave( KErrArgument );
            break;
            }
        }

    }


// -----------------------------------------------------------------------------
// CLpdNotifierQueryLauncher::ExtendedHandleDlgDismissedL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdNotifierQueryLauncher::ExtendedHandleDlgDismissedL()
    {
    TInt retValue = KErrNone;
    switch ( iResult )
        {
        case EAknSoftkeyOk:
            {
            retValue = KErrNone;
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
    iHandler.HandleNotificationResultL( retValue );
    }

//  End of File
