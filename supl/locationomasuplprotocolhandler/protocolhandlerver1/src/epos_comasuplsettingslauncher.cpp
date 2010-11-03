/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Active object wrapper for launching Settings UI
*
*/




// INCLUDE FILES

#include <e32std.h>
#include <e32svr.h>
#include <e32base.h>
#include <AknNotifierWrapper.h>
#include <commdb.h>                 // CommsDatabase                                    
#include <cmapplicationsettingsui.h>// Access Point Configurator
#include <cmplugincsddef.h>			// For UID of CSD BearerType
#include <cmpluginpacketdatadef.h>	// For UID of Packet data BearerType
#include <cmmanager.h>				// For Conversion from UID to Name and Name to UID 
#include <cmconnectionmethod.h> 	// For Conversion from UID to Name and Name to UID

#include <locsettingsuiclient.h>

#include "epos_comasupltrace.h"
#include "epos_comasuplsettingslauncher.h"

_LIT(KTraceFileName,"SUPL_OMA_SESSION::EPos_COMASuplSettingsLauncher.cpp");

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// COMASuplSettingsLauncher::COMASuplSettingsLauncher
// C++ constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
COMASuplSettingsLauncher::COMASuplSettingsLauncher(MOMASuplUISettingsObserver& aCallback)
    : CActive(EPriorityStandard),
    iCallback(aCallback)
    {
    CActiveScheduler::Add(this);
    }
// -----------------------------------------------------------------------------
// COMASuplSettingsLauncher::NewL
// Two-phased constructor for single key listening.
// -----------------------------------------------------------------------------
//
COMASuplSettingsLauncher* COMASuplSettingsLauncher::NewL(MOMASuplUISettingsObserver& aCallback)
    {
    COMASuplSettingsLauncher* newInstance = NewLC(aCallback);
    CleanupStack::Pop();
    return newInstance;
    }

// -----------------------------------------------------------------------------
// COMASuplSettingsLauncher::NewLC
// Two-phased constructor for single key listening. Leaves instance to cleanup stack
// -----------------------------------------------------------------------------
//
COMASuplSettingsLauncher* COMASuplSettingsLauncher::NewLC(MOMASuplUISettingsObserver& aCallback)
    {
    // check for NULL references.

    if ( !&aCallback)    
    {
        User::Leave(KErrArgument);
    }

	COMASuplSettingsLauncher* newInstance = new (ELeave) COMASuplSettingsLauncher( aCallback);
	CleanupStack::PushL( newInstance );
	newInstance->ConstructL();

	return newInstance;
    }

// Destructor
COMASuplSettingsLauncher::~COMASuplSettingsLauncher()
    {
    Cancel();
    delete iSettingsLauncher;	
    iNotifier.Close();
    delete iTrace;
    iIapSelector.Close();
    }


void COMASuplSettingsLauncher::ConstructL()
    {
    iTrace = COMASuplTrace::NewL();	
    iTrace->Trace(_L("COMASuplSettingsLauncher::ConstructL"), KTraceFileName, __LINE__);             
    iSettingsLauncher = CLocSettingsUiClient::NewL();
	User::LeaveIfError(iNotifier.Connect());
	    // Establish a connection to the RGenConAgentDialogServer
    User::LeaveIfError( iIapSelector.Connect());
    
    }

// -----------------------------------------------------------------------------
// COMASuplSettingsLauncher::LaunchSettings
// If already active, just return. Order notification otherwise and set active.
// -----------------------------------------------------------------------------
//
TInt COMASuplSettingsLauncher::LaunchSettings()
    {
    if(IsActive() && iDialogType == ESUPLTimeoutDialog)
        {
        Cancel();
        }
   else if( IsActive() && iDialogType != ESUPLTimeoutDialog)
        {
        return KErrInUse;
        }
   
	TRAPD(error,iSettingsLauncher->LaunchSettingsUiL( TUid::Uid( KLocSUPLSettingsUID ),
													  ELocSUPLIAPSelection,
													  iStatus ));

	iDialogType = ESUPLYesNoDialog;
	SetActive();
	
    return error;
    }

// -----------------------------------------------------------------------------
// COMASuplSettingsLauncher::CancelLaunch
// If active, cancel.
// -----------------------------------------------------------------------------
//
void COMASuplSettingsLauncher::CancelLaunch()
    {
    if(IsActive())
        {
        Cancel();
        }
    }

// -----------------------------------------------------------------------------
// COMASuplSettingsLauncher::RunL
// Handle notification from CenRep session.
// -----------------------------------------------------------------------------
//
void COMASuplSettingsLauncher::RunL()
    {
    // Handle notification  
    
    if(iDialogType == ESUPLYesNoDialog )
    	{
    		if(iStatus.Int() == KErrNone)
    			{
				LaunchIAPDialog();//Launch IAP Dialog
    			}
    		else
    			{
    				iCallback.SettingsUICompletedL(iStatus.Int()); // Handle notification	
    			}	
    	}
    else if(iDialogType == ESUPLIAPDialog)
    	{
    		iCallback.SettingsUICompletedL(iStatus.Int()); // Handle notification	
    	}
    	  
    else
    if(iDialogType == ESUPLUsageDialog)		  	
    	iCallback.SettingsUsageUICompletedL(iStatus.Int()); // Handle notification	
    else
    if(iDialogType == ESUPLTimeoutDialog)		  	
    	iCallback.SettingsTimeOutUICompletedL(iStatus.Int()); // Handle notification	
    }
    
// -----------------------------------------------------------------------------
// COMASuplSettingsLauncher::RunError
// Handle error from CenRep session.
// -----------------------------------------------------------------------------
//
TInt COMASuplSettingsLauncher::RunError(TInt aError)
    {
    if(iDialogType == ESUPLIAPDialog)
        {
        TRAP_IGNORE(iCallback.SettingsUICompletedL(aError));    
        }
    else
     if(iDialogType == ESUPLUsageDialog)	
        {
        TRAP_IGNORE(iCallback.SettingsUsageUICompletedL(aError));	
        }
     if(iDialogType == ESUPLTimeoutDialog)	
        {
        TRAP_IGNORE(iCallback.SettingsTimeOutUICompletedL(aError));	
        }
     return KErrNone;
    }

// -----------------------------------------------------------------------------
// COMASuplSettingsLauncher::DoCancel
// Handle cancel order on this active object.
// -----------------------------------------------------------------------------
//
void COMASuplSettingsLauncher::DoCancel()
    {
    if(IsActive())
    	{
        if(iDialogType == ESUPLIAPDialog)
            {
            iSettingsLauncher->CancelLaunchedSettingsUi();		
            }
            else
            if(iDialogType == ESUPLTimeoutDialog)
            {
            iNotifier.CancelNotifier(KNotifier3Uid);	
            }
            else
            {
            if (iRoaming)                     
                iNotifier.CancelNotifier(KNotifier2Uid);		
            else
                iNotifier.CancelNotifier(KNotifier1Uid);		
            }                
    	}    
    }


// -----------------------------------------------------------------------------
// COMASuplSettingsLauncher::LaunchSettings
// If already active, just return. Order notification otherwise and set active.
// -----------------------------------------------------------------------------
//
TInt COMASuplSettingsLauncher::LaunchSuplUsageSettingsUI(TBool aRoaming)
    {
    if(IsActive())
        {
        return KErrInUse;
        } 

	iDialogType = ESUPLUsageDialog;   

    if (aRoaming)    
        {            
        iTrace->Trace(_L("COMASuplSettingsLauncher::LaunchSuplUsage, user is roaming"),KTraceFileName, __LINE__);
        //SMP Changes
		iRoaming = ETrue;
	    iNotifier.StartNotifierAndGetResponse( iStatus, KNotifier2Uid, buffer1, buffer2 );        
        }
    else
        {            
        iTrace->Trace(_L("COMASuplSettingsLauncher::LaunchSuplUsage, user is not roaming"),KTraceFileName, __LINE__);
		//SMP Changes
	    iNotifier.StartNotifierAndGetResponse( iStatus, KNotifier1Uid, buffer1, buffer2 );
        iRoaming = EFalse;
        }
	SetActive();        

    return KErrNone;
    }

TInt COMASuplSettingsLauncher::LaunchSessionTimeOutDialog()
    {
    if(IsActive())
        {
        Cancel();
        }
    iDialogType = ESUPLTimeoutDialog;  	
    iTrace->Trace(_L("COMASuplSettingsLauncher::LaunchSessionTimeOutDialog"),KTraceFileName, __LINE__);
    iNotifier.StartNotifierAndGetResponse( iStatus, KNotifier3Uid, buffer1, buffer2 );
    SetActive();
    return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// COMASuplSettingsLauncher::LaunchIAPDialog
// If already active, just return. Order notification otherwise and set active.
// -----------------------------------------------------------------------------
//

TInt COMASuplSettingsLauncher::LaunchIAPDialog()
	{
	    if(IsActive())
        {
        return KErrInUse;
        } 
        
	iDialogType = ESUPLIAPDialog;   

    TConnectionPrefs    prefs; // filtering conditions for the access points list
    prefs.iRank         = 1;
    prefs.iDirection    = ECommDbConnectionDirectionOutgoing;
    prefs.iBearerSet    = KUidCSDBearerType | KUidPacketDataBearerType;
   
    // Launch the IAP Selector 
    iStatus = KRequestPending;
    iIapSelector.IapConnection( iSelectedIap, prefs, iStatus );
    SetActive();
		return KErrNone;
	}
	
	
TInt COMASuplSettingsLauncher::GetIAPName(TDes& aIAPName,TUint32& aIAPId)	
	{
	    TRAPD( error, ConvertIAPIdtoNameL( iSelectedIap,aIAPName ));
	    aIAPId = iSelectedIap;
		return error;
	}
	
// ---------------------------------------------------------------------------
// TUint CLocSUPLSettingsUiEngine::ConvertIAPIdtoNameL()
// Convert IAP ID to IAP Name. If a valid name exists for the IAP Id
// then the name would be copied into the aIAPName field
// 
// @param   aIAPId      The IAP Id of the Access point
// @param   aIAPName    The name field corresponding to the Access
//                      point. 
// ---------------------------------------------------------------------------
//
void COMASuplSettingsLauncher::ConvertIAPIdtoNameL( const TUint    aIAPId,
                                                         TDes&    aIAPName )
    {
    // Create the ApSelect handle
    CCommsDatabase* commdb = CCommsDatabase::NewL();
    CleanupStack::PushL( commdb );
    
    // Open the IAP table on the IAP Field
    CCommsDbTableView* table = commdb->OpenViewMatchingUintLC( TPtrC( IAP ), 
                                                               TPtrC( COMMDB_ID ),
                                                               aIAPId );
    User::LeaveIfError( table->GotoFirstRecord() );
    table->ReadTextL( TPtrC( COMMDB_NAME ), aIAPName );

    // Pop and Destroy all the local objects                                                           
    CleanupStack::PopAndDestroy( table );
    CleanupStack::PopAndDestroy( commdb );
    }
	
// End of File
