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
#include "lpdmessageplugindialog.h"
#include "lpddlgobserver.h"
#include "locverifierdlg.hrh"
#include <uikon/eiksrvui.h>
#include <AknNotiferAppServerApplication.h>
#include <e32des16.h>
#include "lpdbkupevtlistenerao.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CLpdMessagePluginDialog::CLpdMessagePluginDialog
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CLpdMessagePluginDialog::CLpdMessagePluginDialog(  
    MLpdDlgObserver& aCallBack,
    CLpdMessagePluginDialog ** aSelfPtr)
    : CAknMessageQueryDialog( static_cast<TTone>(EAvkonSIDLocationRequest) ),
      iCallBack( aCallBack ),
      iAppSwitchSuppressed( EFalse ),
      iSelfPtr( aSelfPtr )
    {
    }

// -----------------------------------------------------------------------------
// CLpdMessagePluginDialog::ConstructL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
void CLpdMessagePluginDialog::ConstructL()
	{
	iBkupListener = CLpdBkupEvtListenerAO::NewL( *this );
	iBkupListener->StartL();
	}


// -----------------------------------------------------------------------------
// CLpdMessagePluginDialog::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CLpdMessagePluginDialog* CLpdMessagePluginDialog::NewL(                    
    MLpdDlgObserver& aCallBack,
    CLpdMessagePluginDialog ** aSelfPtr
    )
    {
    CLpdMessagePluginDialog* self =
        new( ELeave ) CLpdMessagePluginDialog( aCallBack, aSelfPtr);
	  CleanupStack::PushL(self);
	  self->ConstructL();
	  CleanupStack::Pop(self);  
    return self;
    }

// Destructor
CLpdMessagePluginDialog::~CLpdMessagePluginDialog()
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
// CLpdMessagePluginDialog::PrepareLC
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdMessagePluginDialog::PrepareLC( TInt aResourceId )
	{
	CAknDialog::PrepareLC( aResourceId );
	}

// -----------------------------------------------------------------------------
// CLpdMessagePluginDialog::RunLD
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CLpdMessagePluginDialog::RunLD()
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
// CLpdMessagePluginDialog::ExitDialogL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdMessagePluginDialog::ExitDialogL()
    {
    TryExitL( EEikCmdExit );
    }

// -----------------------------------------------------------------------------
// CLpdMessagePluginDialog::ExitDialogL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdMessagePluginDialog::ExitDialogQuietlyL()
    {
    TryExitL( ELpdCmdQuietExit );
    }

// -----------------------------------------------------------------------------
// CLpdMessagePluginDialog::OkToExitL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CLpdMessagePluginDialog::OkToExitL( TInt aButtonId )
    {
    if ( aButtonId == EAknSoftkeyCancel || aButtonId == EAknSoftkeyOk )
        {
        iCallBack.HandleDlgDismissedL( aButtonId, 
                    -1 );
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

// -----------------------------------------------------------------------------
// CLpdMessagePluginDialog::HandleResourceChange
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdMessagePluginDialog::HandleResourceChange(TInt aType)
	{
	CAknMessageQueryDialog::HandleResourceChange(aType);
	}
// End of file

