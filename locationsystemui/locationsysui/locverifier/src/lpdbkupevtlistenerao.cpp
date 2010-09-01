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
* Description:  Active object which listens to Backup / Restore Events
*
*/


// INCLUDE FILES
#include "lpdbkupevtlistenerao.h"
#include "lpdbkupevtobserver.h"
#include <connect/sbdefs.h>
#include <e32property.h>

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CLpdRequestAO::CLpdRequestAO
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CLpdBkupEvtListenerAO::CLpdBkupEvtListenerAO( MLpdBkupEvtObserver& aBkupObserver )
    : CActive( CActive::EPriorityHigh ), iBkupObserver( aBkupObserver )
    {
	CActiveScheduler::Add(this);
	}

void CLpdBkupEvtListenerAO::ConstructL()
	{
	User::LeaveIfError(iProperty.Attach(KUidSystemCategory,conn::KUidBackupRestoreKey));
	}

// -----------------------------------------------------------------------------
// CLpdRequestAO::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CLpdBkupEvtListenerAO* CLpdBkupEvtListenerAO::NewL( MLpdBkupEvtObserver& aBkupObserver )
	{
    CLpdBkupEvtListenerAO* self = new( ELeave ) CLpdBkupEvtListenerAO( aBkupObserver );
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
	}

// Destructor
CLpdBkupEvtListenerAO::~CLpdBkupEvtListenerAO()
	{
    Cancel();
    iProperty.Close();
	}

// -----------------------------------------------------------------------------
// CLpdBkupEvtListenerAO::StartL
// This method subscribes to Backup/Restore Event 
// -----------------------------------------------------------------------------
//
void CLpdBkupEvtListenerAO::StartL()
	{
	TInt backupFlag = 0;
	TInt error = iProperty.Get(backupFlag);
	switch(error)
		{
		case KErrNone:
			// Backup/Restore is already running so Leave with KErrCancel.
			// This will stop the verifier dialogs from being popped up.
			if ((backupFlag & conn::KBackupIncTypeMask) != conn::ENoBackup )
				{
				User::Leave(KErrCancel);
				}
			// fall-through necessary

		case KErrNotFound:
			iProperty.Subscribe(iStatus);
			SetActive();
			break;
		
		default:
			// In case of any other error Leave with the returned error code
			User::LeaveIfError(error);
			break;
		}
	}
	
// -----------------------------------------------------------------------------
// CLpdBkupEvtListenerAO::RunL
// Control is in this function when Backup / Restore event happens. 
// -----------------------------------------------------------------------------
//
void CLpdBkupEvtListenerAO::RunL()
    {
    // Check the value of the property.
    // If backup is started or restore is started then close the dialogs.
	  TInt backupFlag;
	  User::LeaveIfError(iProperty.Get(backupFlag));
	  if ( (backupFlag & conn::KBackupIncTypeMask) != conn::ENoBackup )
		  {
		  iBkupObserver.ExitDialogL();	
		  }
  	else
		  {
	    // Re-subscribe to the event
	    iProperty.Subscribe(iStatus);
	    SetActive();
		  }
    }

// -----------------------------------------------------------------------------
// CLpdBkupEvtListenerAO::DoCancel
// -----------------------------------------------------------------------------
//
void CLpdBkupEvtListenerAO::DoCancel()
    {
    iProperty.Cancel();
	  }

//  End of File
