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
* Description:  Active object which ensures that current call stack is run to
*                completion before a new request is handled
*
*/


// INCLUDE FILES
#include "lpdrequestao.h"
#include "lpdverifierplugin.h"
#include <lbs/epos_cposrequestor.h>
#include <lbs/epos_rposrequestorstack.h>
#include <AknNotifierAppServer.h>
#include <eikenv.h>
#include <eiknotapi.h>

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CLpdRequestAO::CLpdRequestAO
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CLpdRequestAO::CLpdRequestAO( CLpdVerifierPlugin& aPlugin )
    : CActive( CActive::EPriorityHigh ), iPlugin( aPlugin )
    {
	}

// -----------------------------------------------------------------------------
// CLpdRequestAO::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CLpdRequestAO* CLpdRequestAO::NewL( CLpdVerifierPlugin& aPlugin )
	{
    CLpdRequestAO* self = new( ELeave ) CLpdRequestAO( aPlugin );
    CActiveScheduler::Add(self);

    CAknNotifierAppServer* AknNServer = 
        static_cast<CAknNotifierAppServer*>(CEikonEnv::Static()->AppServer());
    AknNServer->UnbalanceReferenceCountForNotif(aPlugin.NotifierBase()->Info().iUid,ETrue);
    // Nothing to do in the Second Phase Constructor
    return self;
	}

// Destructor
CLpdRequestAO::~CLpdRequestAO()
	{
    Cancel();
    iNotificationQue.ResetAndDestroy();
    iNotificationQue.Close();
	}

// -----------------------------------------------------------------------------
// CLpdRequestAO::ScheduleRequest
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdRequestAO::ScheduleRequest()
    {	     
    Cancel();
    SetActive();
    TRequestStatus* status = &iStatus;
    User::RequestComplete( status, KErrNone );            
    }

// -----------------------------------------------------------------------------
// CLpdRequestAO::RunL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdRequestAO::RunL()
    {	    
    // Let's call the non-leaving method so that plugin
    // takes care of error handling.
    RArray<TPosQNRequestId> requests;
    CleanupClosePushL( requests );
    iPlugin.GetRequestsL( requests );
    if ( requests.Count() == 0 )
    	{
    	if( iNotificationQue.Count() )
    	    {
        	iPlugin.FreeQueryResources();
        	CLpdNotifReqInfo* notifInfo = iNotificationQue[0];
        	iPlugin.NotifyCancellationL(notifInfo->Source(),
        								notifInfo->Reason(),
        								notifInfo->Decision(),
        								notifInfo->Requestors());
    	    iNotificationQue.Remove(0);
    	    delete notifInfo;
    	    iNotificationQue.Compress();
    	    }
        else
            {
    	    CAknNotifierAppServer* AknNServer = 
                static_cast<CAknNotifierAppServer*>(CEikonEnv::Static()->AppServer());
            AknNServer->UnbalanceReferenceCountForNotif(iPlugin.NotifierBase()->Info().iUid,EFalse);
    	    iPlugin.HandleNextRequest();
            }
    	}
    else
    	{
	    iPlugin.HandleNextRequest();
    	}
    CleanupStack::PopAndDestroy();
    }

// -----------------------------------------------------------------------------
// CLpdRequestAO::DoCancel
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdRequestAO::DoCancel()
    {
    // Not important with this AO
	}


// -----------------------------------------------------------------------------
// CLpdRequestAO::EnqueueRequestL
// -----------------------------------------------------------------------------
//
void CLpdRequestAO::EnqueueRequestL( TPosRequestSource aSource,
								    TPosVerifyCancelReason aReason,
								    TPosRequestDecision aDecision,
								    RPosRequestorStack* aRequestors )
	{
	CLpdNotifReqInfo* notifReqInfo = CLpdNotifReqInfo::NewL(aSource, 
													  aReason,
													  aDecision,
													  aRequestors);
	
	// The ownership is transferred to iNotificationQue
	iNotificationQue.Append(notifReqInfo);
	}
	
//  End of File
