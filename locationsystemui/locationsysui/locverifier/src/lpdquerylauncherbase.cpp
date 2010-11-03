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
* Description:  Abstract base class for queries.
*
*/


// INCLUDE FILES
#include <locverifierdlg.rsg>
#include <StringLoader.h>

#include "locverifierdlg.hrh"
#include "lpdquerylauncherbase.h"
#include "lpdresulthandler.h"
#include "locconsts.h"
#include "lpdglobalplugindialog.h"
#include "lpdmessageplugindialog.h"
#include "lpdperiodicprocessor.h"


// CONSTANTS

// Unnamed namespace for local definitions

const TUint KSelectionArrayGranularity = 5;
const TInt KVerDlgTimeout = 900000000; // == 15 minutes

#ifdef _DEBUG
_LIT( KPanicText, "CLpdQueryLauncherBase" );
enum TPanicCode
    {
    KLpdErrGeneral = 1
    };
#endif

// INCLUDE FILES
#include <s32mem.h>
#include <AknMediatorFacade.h>
#include "locverifiercoverui.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CLpdQueryLauncherBase::CLpdQueryLauncherBase
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CLpdQueryLauncherBase::CLpdQueryLauncherBase( MLpdResultHandler& aHandler )
    : iHandler( aHandler ),
      iResult( KErrArgument ),
      iCurrentIndex( -1 ),
      iExitReason( KErrNone ),
      iOwnsQueryDlg( EFalse ),
      iOwnsMsgQueryDlg( EFalse )   
    {
    }

// -----------------------------------------------------------------------------
// CLpdQueryLauncherBase::BaseConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CLpdQueryLauncherBase::BaseConstructL()
    {
    // In current implementation timer is not used. If needed, take timer 
    // into use by uncommenting the following line:
    // iTimer = CPeriodic::NewL( CActive::EPriorityStandard )
    }

// Destructor
CLpdQueryLauncherBase::~CLpdQueryLauncherBase()
    {
    // This does cleanup nicely even if dialog was not running:
    ExitQuery( EFalse ); // no callback from dialog anymore
    delete iTimer; // ok even if timer is NULL 
    }

// -----------------------------------------------------------------------------
// CLpdQueryLauncherBase::PrepareQueryResourcesL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdQueryLauncherBase::PrepareQueryResourcesL( TInt aResourceId, TInt aSuplNotifRequest)
    {
    
    if( aSuplNotifRequest )
        {
        // Cancel deletes dialog resources:
        Cancel();    
       __ASSERT_DEBUG( !iMsgQueryDlg,
                    User::Panic( KPanicText, KLpdErrGeneral ) );
  
        iMsgQueryDlg = CLpdMessagePluginDialog::NewL( *this, &iMsgQueryDlg );
        iMsgQueryDlg->PrepareLC(aResourceId);    
        CleanupStack::Pop(iMsgQueryDlg); //iMsgQueryDlg, must be pushed back when launching
        iOwnsMsgQueryDlg = ETrue; 
 
        }
    else
        {
       // Cancel deletes dialog resources:
       Cancel();    
     __ASSERT_DEBUG( !iSelectionIndex && !iQueryDlg,
                    User::Panic( KPanicText, KLpdErrGeneral ) );
        iSelectionIndex =
            new( ELeave ) CArrayFixFlat<TInt>( KSelectionArrayGranularity );
        iQueryDlg = CLpdGlobalPluginDialog::NewL( *this, &iQueryDlg, iSelectionIndex );
        iQueryDlg->PrepareLC( aResourceId );
        CleanupStack::Pop(iQueryDlg); //iQueryDlg, must be pushed back when launching
        iOwnsQueryDlg = ETrue; 
            
        }
    }

// -----------------------------------------------------------------------------
// CLpdQueryLauncherBase::SetQueryTextArray
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdQueryLauncherBase::SetQueryTextArray( MDesCArray* aArray )
    {
    __ASSERT_DEBUG( iQueryDlg, User::Panic( KPanicText, KLpdErrGeneral ) );
    iQueryDlg->SetItemTextArray( aArray );
    // ownership of array is transferred:
    iQueryDlg->SetOwnershipType( ELbmOwnsItemArray );
    }

// -----------------------------------------------------------------------------
// CLpdQueryLauncherBase::SetMessageQueryTextL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdQueryLauncherBase::SetMessageQueryTextL( const TDesC& aString, TCallBack aCallBack )
    {
    __ASSERT_DEBUG( iMsgQueryDlg, User::Panic( KPanicText, KLpdErrGeneral ) );
    
    HBufC* dialogTitle = NULL;
    // set title text
    dialogTitle = StringLoader::LoadLC( R_LOC_SUPL_PERIODIC_DIALOG_TITLE );  
    iMsgQueryDlg->QueryHeading()->SetTextL( *dialogTitle ); 
    // set message text                  
	  iMsgQueryDlg->SetMessageTextL( aString );
	  iMsgQueryDlg->SetLink( aCallBack );
	
	  CleanupStack::PopAndDestroy( dialogTitle );
    }
    
// -----------------------------------------------------------------------------
// CLpdQueryLauncherBase::StartQueryL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdQueryLauncherBase::StartQueryL()
    {
    User::LeaveIfNull( iQueryDlg );

    iExitReason = KErrNone;
    
    CLpdGlobalPluginDialog* queryDialog = iQueryDlg;
    
    // Publish the dialog to Cover UI  
    PublishDialogtoCoverL();
       
    CleanupStack::PushL( queryDialog ); // RunLD() requires this
    iOwnsQueryDlg = EFalse; // ownership transferred to RunLD
    queryDialog->RunLD();

    if ( iTimer )
        {
        TCallBack callback( HandleTimeoutCallback, this );
        TTimeIntervalMicroSeconds32 timeout( KVerDlgTimeout );
        // This timer will not expire periodically, so the second
        // parameter is not so important in this case.
        
		    iTimer->Cancel();
        iTimer->Start( timeout, timeout, callback );
        }
    }

// -----------------------------------------------------------------------------
// CLpdQueryLauncherBase::StartSuplPeriodicQueryL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdQueryLauncherBase::StartSuplPeriodicQueryL()
    {
    User::LeaveIfNull( iMsgQueryDlg );
    
    iExitReason = KErrNone;
    CLpdMessagePluginDialog* queryDialog = iMsgQueryDlg;
    
    // Publish the dialog to Cover UI  
    PublishDialogtoCoverL();
       
    CleanupStack::PushL( queryDialog ); // RunLD() requires this
    iOwnsMsgQueryDlg = EFalse; // ownership transferred to RunLD
    queryDialog->RunLD();
    
    if ( iTimer )
        {
        TCallBack callback( HandleTimeoutCallback, this );
        TTimeIntervalMicroSeconds32 timeout( KVerDlgTimeout );
        // This timer will not expire periodically, so the second
        // parameter is not so important in this case.
        
        iTimer->Cancel();
        iTimer->Start( timeout, timeout, callback );
        }
         
    }

// -----------------------------------------------------------------------------
// CLpdQueryLauncherBase::SelectionIndexes
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const CListBoxView::CSelectionIndexArray* CLpdQueryLauncherBase::SelectionIndexes() const
    {
    return iSelectionIndex;
    }

// -----------------------------------------------------------------------------
// CLpdQueryLauncherBase::HandleTimeoutCallback
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CLpdQueryLauncherBase::HandleTimeoutCallback( TAny* aThis )
    {
	  static_cast<CLpdQueryLauncherBase*>( aThis )->HandleTimeout();
    return KErrNone; // Return value is unimportant in this case
    }

// -----------------------------------------------------------------------------
// CLpdQueryLauncherBase::Cancel
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdQueryLauncherBase::Cancel()
    {
    iExitReason = KErrCancel;
    ExitQuery( ETrue );
    }

// -----------------------------------------------------------------------------
// CLpdQueryLauncherBase::CancelQuietly
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdQueryLauncherBase::CancelQuietly()
    {
    iExitReason = KErrCancel;
    ExitQuery( EFalse );
    }

// -----------------------------------------------------------------------------
// CLpdQueryLauncherBase::ListBoxL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CEikFormattedCellListBox& CLpdQueryLauncherBase::ListBoxL()
    {
	__ASSERT_DEBUG( iQueryDlg, User::Panic( KPanicText, KLpdErrGeneral ) );
    CEikFormattedCellListBox* listbox = iQueryDlg->ListControl()->Listbox();
    User::LeaveIfNull( listbox );
	  return *listbox;    
    }

// -----------------------------------------------------------------------------
// CLpdQueryLauncherBase::SetRequestorBuffer
// Sets the requestor buffer array that needs to be published to
// Cover UI
// @param aRequestorBuffer Flat buffer containing the list of requestors
// -----------------------------------------------------------------------------
//
void CLpdQueryLauncherBase::SetRequestorBuffer( TDes8&     aRequestorBuffer )
    {
    iRequestorBuffer.Set( aRequestorBuffer);
    }

// -----------------------------------------------------------------------------
// CLpdQueryLauncherBase::SetCoverUICommand
// Sets the Cover UI Command ID
// @param aCommandId Cover UI Command
// -----------------------------------------------------------------------------
//
void CLpdQueryLauncherBase::SetCoverUICommand( TInt        aCommand )
    {
    iCoverUiCommand = aCommand;
    }


// -----------------------------------------------------------------------------
// CLpdQueryLauncherBase::HandleDlgDismissedL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdQueryLauncherBase::HandleDlgDismissedL(    
    TInt aResult,
    TInt aCurrentIndex )
    {
    // ExtendedHandleDlgDismissedL() implementation probably
    // makes a callback to the observer of this class, which
    // might delete this object. Therefore it is important
    // that members are carefully used.

    if ( iTimer )
        {
        iTimer->Cancel();
        }

    iResult = aResult;
    iCurrentIndex = aCurrentIndex;
    
    if(iQueryDlg)
    {
    // Cancel the dialog from secondary
    CAknMediatorFacade* covercl = AknMediatorFacade(iQueryDlg);
    if( covercl )
      {
      covercl->CancelCommand();
      }
    }
    if(iMsgQueryDlg)
    {
      // Cancel the dialog from secondary
    CAknMediatorFacade* coverclmsg = AknMediatorFacade(iMsgQueryDlg);
    if( coverclmsg )
        {
        coverclmsg->CancelCommand();
        }
    }
    // iQueryDlg, which will delete itself, is becoming invalid now
    iQueryDlg = NULL;
    iMsgQueryDlg = NULL;
    TRAPD( err, ExtendedHandleDlgDismissedL() );
         if ( err )
             { // Leave occurred in dialog result processing, we must let
               // plugin handle the situation instead of propagating error to
               // AVKON, which would mean that dialog would not be exited and
               // user could alter his decision --> risk of conflicts.
             iHandler.HandleLeave( err );
             }
      
    }

// -----------------------------------------------------------------------------
// CLpdQueryLauncherBase::HandleTimeout
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdQueryLauncherBase::HandleTimeout()
    {
    iExitReason = KErrTimedOut;
    ExitQuery( ETrue );
    }

// -----------------------------------------------------------------------------
// CLpdQueryLauncherBase::ExitQuery
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdQueryLauncherBase::ExitQuery( TBool aCallback )
    {
    if ( iTimer )
        {
        iTimer->Cancel();
        }
    if ( iQueryDlg )
        {
        if ( iOwnsQueryDlg )
            { // Dialog not running yet --> must be explicitely deleted
            delete iQueryDlg;
            }
        else
            { // Dialog running --> exit
            TInt err = KErrNone;
            if ( aCallback )
                {
                TRAP( err, iQueryDlg->ExitDialogL() );
                }
            else
                {
                TRAP( err, iQueryDlg->ExitDialogQuietlyL() );
                }
            if ( err )
                {
                CEikonEnv::Static()->HandleError( err );
                }
            }
        iQueryDlg = NULL;
        }
    if ( iMsgQueryDlg )
        {
        if ( iOwnsQueryDlg )
            { // Dialog not running yet --> must be explicitely deleted
            delete iMsgQueryDlg;
            }
        else
            { // Dialog running --> exit
            TInt err = KErrNone;
            if ( aCallback )
                {
                TRAP( err, iMsgQueryDlg->ExitDialogL() );
                }
            else
                {
                TRAP( err, iMsgQueryDlg->ExitDialogQuietlyL() );
                }
            if ( err )
                {
                CEikonEnv::Static()->HandleError( err );
                }
            }
        iMsgQueryDlg = NULL;
        }
        iResult = KErrArgument;
        iCurrentIndex = -1;
        if( iSelectionIndex )
           {
           delete iSelectionIndex;
           iSelectionIndex = NULL;
           }
 }

// -----------------------------------------------------------------------------
// CLpdQueryLauncherBase::PublishDialogtoCoverL
// Prepare the requestor buffers and Publish the dialog to the 
// Cover UI
// -----------------------------------------------------------------------------
//
void CLpdQueryLauncherBase::PublishDialogtoCoverL()
    {
    
    CAknMediatorFacade* covercl = NULL;
	if( iQueryDlg )
		{
	    // Publish the Dialog to Cover UI
	    iQueryDlg->PublishDialogL( iCoverUiCommand, KCatUidLocationVerifierDlgs );
		
   		// Send the requestor buffer to the Cover UI
		covercl = AknMediatorFacade( iQueryDlg );
		}
	else if( iMsgQueryDlg )
		{
	    // Publish the Dialog to Cover UI
	    iMsgQueryDlg->PublishDialogL( iCoverUiCommand, KCatUidLocationVerifierDlgs );
		
   		// Send the requestor buffer to the Cover UI
		covercl = AknMediatorFacade( iMsgQueryDlg );
		}
		
    if (covercl)
        {
        RBufWriteStream& writeStream = covercl->BufStream();
        
        TUint8* ptr = const_cast<TUint8 *>(iRequestorBuffer.Ptr());
        writeStream.WriteL( ptr, iRequestorBuffer.Length());
        writeStream.CommitL();
        }
    }

//  End of File
