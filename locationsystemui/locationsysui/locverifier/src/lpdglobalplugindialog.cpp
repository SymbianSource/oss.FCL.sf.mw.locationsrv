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
* Description:  List query dialog suitable for verifier plug-in use.
*
*/


// INCLUDE FILES
#include "lpdglobalplugindialog.h"
#include "lpddlgobserver.h"
#include "locverifierdlg.hrh"
#include <uikon/eiksrvui.h>
#include <AknNotiferAppServerApplication.h>
#include <e32des16.h>
#include "lpdbkupevtlistenerao.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CLpdGlobalPluginDialog::CLpdGlobalPluginDialog
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CLpdGlobalPluginDialog::CLpdGlobalPluginDialog(                    
    MLpdDlgObserver& aCallBack,
    CLpdGlobalPluginDialog ** aSelfPtr,
    CListBoxView::CSelectionIndexArray* aSelected)
    : CAknListQueryDialog( &iDummyIndex ),
      iCallBack( aCallBack ),
      iAppSwitchSuppressed( EFalse ),
      iSelected( aSelected ),
      iSelfPtr( aSelfPtr )
    {
    }

// -----------------------------------------------------------------------------
// CLpdGlobalPluginDialog::ConstructL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
void CLpdGlobalPluginDialog::ConstructL()
	{
	iBkupListener = CLpdBkupEvtListenerAO::NewL( *this );
	iBkupListener->StartL();
	}


// -----------------------------------------------------------------------------
// CLpdGlobalPluginDialog::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CLpdGlobalPluginDialog* CLpdGlobalPluginDialog::NewL(                    
    MLpdDlgObserver& aCallBack,
    CLpdGlobalPluginDialog ** aSelfPtr,
    CListBoxView::CSelectionIndexArray* aSelected)
    {
    CLpdGlobalPluginDialog* self =
        new( ELeave ) CLpdGlobalPluginDialog( aCallBack, aSelfPtr, aSelected);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);  
    return self;
    }

// Destructor
CLpdGlobalPluginDialog::~CLpdGlobalPluginDialog()
	{
    if ( iSelfPtr )
        {
        *iSelfPtr = NULL;
        }
    if ( iBkupListener )
    	{
    	delete iBkupListener;	
    	}
	}

// -----------------------------------------------------------------------------
// CLpdGlobalPluginDialog::PrepareLC
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdGlobalPluginDialog::PrepareLC( TInt aResourceId )
	{
	CAknDialog::PrepareLC( aResourceId );
	}

// -----------------------------------------------------------------------------
// CLpdGlobalPluginDialog::RunLD
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CLpdGlobalPluginDialog::RunLD()
    {
 		if ( !iAppSwitchSuppressed )
      {
			TInt err = 
				((CAknNotifierAppServerAppUi *)
					iEikonEnv->EikAppUi())->SuppressAppSwitching(ETrue);
	 		if (!err)        
	 			{
	 			iAppSwitchSuppressed = ETrue;	
	 			}
      }
    return CAknQueryDialog::RunLD();
    }

// -----------------------------------------------------------------------------
// CLpdGlobalPluginDialog::OfferKeyEventL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TKeyResponse CLpdGlobalPluginDialog::OfferKeyEventL(
    const TKeyEvent& aKeyEvent,
    TEventCode aType )
    {
    if ( aType != EEventKey )
        {
        return EKeyWasNotConsumed;
        }
    
    TInt code = aKeyEvent.iCode;

    switch ( code )
        {
        case EKeyEscape:
            { // emergency call support
            TryExitL( ELpdCmdEmergency );
			return EKeyWasConsumed;
            }
        default:
        	break;
        }
    return ListBox()->OfferKeyEventL( aKeyEvent, aType );
    }

// -----------------------------------------------------------------------------
// CLpdGlobalPluginDialog::SetHeaderTextL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdGlobalPluginDialog::SetHeaderTextL( const TDesC& aText )
    {
    CAknPopupHeadingPane* heading = NULL;
    User::LeaveIfNull( heading = QueryHeading() );
    heading->SetTextL( aText );
    }

// -----------------------------------------------------------------------------
// CLpdGlobalPluginDialog::ExitDialogL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdGlobalPluginDialog::ExitDialogL()
    {
    TryExitL( EEikCmdExit );
    }

// -----------------------------------------------------------------------------
// CLpdGlobalPluginDialog::ExitDialogL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdGlobalPluginDialog::ExitDialogQuietlyL()
    {
    TryExitL( ELpdCmdQuietExit );
    }

// -----------------------------------------------------------------------------
// CLpdGlobalPluginDialog::OkToExitL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CLpdGlobalPluginDialog::OkToExitL( TInt aButtonId )
    {
    if ( aButtonId == EAknSoftkeyCancel || aButtonId == EAknSoftkeyOk )
        {
        TInt currentIndex = ListBox()->CurrentItemIndex();
        const CListBoxView::CSelectionIndexArray* aSelectionIndex = 
            ListBox()->View()->SelectionIndexes();
        if ( iSelected && aSelectionIndex->Count() > 0 )
            { // CSelectionIndexArray was given in constructor and
              // listbox view also has selection indexes.
            ListBox()->View()->GetSelectionIndexesL( iSelected );
            }
        iCallBack.HandleDlgDismissedL( aButtonId, currentIndex );
        }
    else if ( ( aButtonId == EEikCmdExit ) || 
              ( aButtonId == ELpdCmdEmergency ) )
        { // Dialog needs to be exited or emergency key has been pressed.
        iCallBack.HandleDlgDismissedL( aButtonId, -1 );
        }
    else if ( aButtonId == ELpdCmdQuietExit )
        {
        // No callback.
        }
    else
        {
        return EFalse; //dialog is not exited
        }

    if ( iAppSwitchSuppressed )
        {
				TInt er = 
					((CAknNotifierAppServerAppUi *)
						iEikonEnv->EikAppUi())->SuppressAppSwitching(EFalse);
		 		if (!er)
		 			{
		 			iAppSwitchSuppressed = EFalse;
		 			}
        }
    return ETrue; // dialog is exited
    }

void CLpdGlobalPluginDialog::HandleResourceChange(TInt aType)
	{
	CAknListQueryDialog::HandleResourceChange(aType);
	}
// End of file

