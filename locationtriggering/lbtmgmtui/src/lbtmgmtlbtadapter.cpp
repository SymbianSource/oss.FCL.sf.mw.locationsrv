/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Location Triggering Management's Lbt adapter
*
*/


// User Include
#include "lbtmgmtlbtadapter.h"
#include "lbtmgmtadapterobserver.h"
#include "lbtmgmtdebug.h"
#include <lbtstatuspskeys.h>
#include <lbttriggerfilterbyattribute.h> 
#include <lbttriggerchangeeventnotifier.h>

// System Include
#include <lbttriggeringsystemmanagementsettings.h> 
#include <lbtcommon.h>
#include <e32property.h>

// Constant Definition

// ========================= MEMBER FUNCTIONS ================================


// ---------------------------------------------------------------------------
// CLbtMgmtLbtAdapter::CLbtMgmtLbtAdapter
// Overloaded Constructor
//
// ---------------------------------------------------------------------------
//  
CLbtMgmtLbtAdapter::CLbtMgmtLbtAdapter(  
                                MLbtMgmtAdapterObserver& aAdapterObsrv )
    :CActive(EPriorityStandard),
    iObserver(aAdapterObsrv)
    {
    }

// ---------------------------------------------------------------------------
// CLbtMgmtLbtAdapter::~CLbtMgmtLbtAdapter
// Destructor
//
// ---------------------------------------------------------------------------
//
CLbtMgmtLbtAdapter::~CLbtMgmtLbtAdapter()
    {
    Cancel();
    delete iEventChangeObserver;
    iLbtMgrObserver.Close();
    iLbtMgr.Close();
    iLbtServ.Close();
    }
    
// ---------------------------------------------------------------------------
// CLbtMgmtLbtAdapter* CLbtMgmtLbtAdapter::NewL
// Creates new Trigger Status adapter
//
// @param  aAdapterObsrv                        Observer to the Settings adapter
// @return CLbtMgmtLbtAdapter& Reference to the Trigger Status adapter
// ---------------------------------------------------------------------------
//    
CLbtMgmtLbtAdapter* CLbtMgmtLbtAdapter::NewL( 
                            MLbtMgmtAdapterObserver& aAdapterObsrv )
    {   
    CLbtMgmtLbtAdapter* self = 
                        CLbtMgmtLbtAdapter::NewLC( aAdapterObsrv );
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// CLbtMgmtLbtAdapter* CLbtMgmtLbtAdapter::NewLC
// Creates new Trigger Status adapter. Leaves the object on the clean-up
// stack
//
// @param  aAdapterObsrv                        Observer to the Settings adapter
// @return CLbtMgmtLbtAdapter& Reference to the Trigger Status adapter
// ---------------------------------------------------------------------------
// 
CLbtMgmtLbtAdapter* CLbtMgmtLbtAdapter::NewLC( 
                            MLbtMgmtAdapterObserver& aAdapterObsrv )
    {   
    CLbtMgmtLbtAdapter* self = 
                        new(ELeave) CLbtMgmtLbtAdapter( aAdapterObsrv );
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }
    
// ---------------------------------------------------------------------------
// void CLbtMgmtLbtAdapter::ConstructL
// Second Phase Constructor
//
// ---------------------------------------------------------------------------
//
void CLbtMgmtLbtAdapter::ConstructL()
    {  
    
    // Create the Central repository object for manipulating Avkon Central
    // repository for Co-ordinate display settings
    User::LeaveIfError( iLbtServ.Connect() );
	User::LeaveIfError( iLbtMgr.Open(iLbtServ) );
	User::LeaveIfError( iLbtMgrObserver.Open(iLbtServ) );
    
	CActiveScheduler::Add( this );
    StartNotification();
    
    iEventChangeObserver = CLbtTriggerChangeEventNotifier::NewL( iLbtMgrObserver, *this );
    iEventChangeObserver->Start();
    
    iLbtMgr.GetTriggeringSystemSettingsL( iTriggerSettings );
    GetTotalActiveTriggersL();
    }
    
// ---------------------------------------------------------------------------
// CLbtMgmtLbtAdapter::TCordDisSetListPosition 
//                CLbtMgmtLbtAdapter::GetCurrentDisplayFormatL
// Returns the position of the Co-ordinate display format settings value.
//
// @return TCordDisSetListPosition Current postion of the Co-ordinate display
//                                 settings value.
// ---------------------------------------------------------------------------
//
CLbtMgmtLbtAdapter::TTriggeringStatus 
                CLbtMgmtLbtAdapter::GetCurrentTriggeringStatusL()
    {	
    LBTMGMTUIDEBUG("+ CLbtMgmtLbtAdapter::GetCurrentTriggeringStatusL()");
    CLbtMgmtLbtAdapter::TTriggeringStatus status = CLbtMgmtLbtAdapter::EOff;
    
    switch( iTriggerSettings.TriggeringMechanismState() )
    	{
    	case ETriggeringMechanismOn:
    		status = CLbtMgmtLbtAdapter::EOn;
    		break;
    	case ETriggeringMechanismOff:
    		status = CLbtMgmtLbtAdapter::EOff;
    		break;
    	default :
    		break;
    	}
    LBTMGMTUIDEBUG1("- CLbtMgmtLbtAdapter::GetCurrentTriggeringStatusL() - %d", status);
    
    return status;    
    }

// --------------------------------------------------------------------------
// void CLbtMgmtLbtAdapter::SetCurrentDisplayFormatL
// Sets the value of the Co-ordinate display format settings based on the
// display position.
//
// @param aSettingsPos  Position of the new Co-ordinate display settings value
// ---------------------------------------------------------------------------
//
void CLbtMgmtLbtAdapter::SetCurrentTriggeringStatusL( 
        CLbtMgmtLbtAdapter::TTriggeringStatus aSettingsValue )
   {	
   LBTMGMTUIDEBUG1("+ CLbtMgmtLbtAdapter::SetCurrentTriggeringStatusL() - %d", aSettingsValue);
    switch( aSettingsValue )
    	{
    	case CLbtMgmtLbtAdapter::EOn:
    		iTriggerSettings.SetTriggeringMechanismState(ETriggeringMechanismOn);
		    LBTMGMTUIDEBUG("Before SetTriggeringSystemSettingsL"); 
			iLbtMgr.SetTriggeringSystemSettingsL(iTriggerSettings,ETriggeringMechanismState); 
    	    LBTMGMTUIDEBUG("After SetTriggeringSystemSettingsL"); 
    		break;
    	case CLbtMgmtLbtAdapter::EOff:
    		iTriggerSettings.SetTriggeringMechanismState(ETriggeringMechanismOff);
		    LBTMGMTUIDEBUG("Before SetTriggeringSystemSettingsL"); 
			iLbtMgr.SetTriggeringSystemSettingsL(iTriggerSettings,ETriggeringMechanismState); 
    	    LBTMGMTUIDEBUG("After SetTriggeringSystemSettingsL"); 
    		break;
    	default :
    		break;
    	}	
    LBTMGMTUIDEBUG("- CLbtMgmtLbtAdapter::SetCurrentTriggeringStatusL()");	
   }
    
// --------------------------------------------------------------------------
// void CLbtMgmtLbtAdapter::RunL
// Inherited from CActive
//
// ---------------------------------------------------------------------------
//
void CLbtMgmtLbtAdapter::RunL()
    {
    LBTMGMTUIDEBUG("+ CLbtMgmtLbtAdapter::RunL() - Settings Changed");  
    // Issue Notification
    iObserver.HandleSettingsChangeL();
    
    // Schedule for listening to change events again
    StartNotification();

    LBTMGMTUIDEBUG("- CLbtMgmtLbtAdapter::RunL()");  
    }

// --------------------------------------------------------------------------
// void CLbtMgmtLbtAdapter::DoCancel
// Inherited from CActive
//
// ---------------------------------------------------------------------------
//
void CLbtMgmtLbtAdapter::DoCancel()
    {
    // Cancel the outstanding CR notification request
    iLbtMgr.CancelNotifyTriggeringSystemSettingChange();
    }
    
// --------------------------------------------------------------------------
// void CLbtMgmtLbtAdapter::StartNotification
// Start notification from the Central repository server for any changes in the
// Co-ordinate display format key value
//
// ---------------------------------------------------------------------------
//
void CLbtMgmtLbtAdapter::StartNotification()
    {
    if( !IsActive())
        {
        iStatus = KRequestPending;
        // Request for notification        
        iLbtMgr.NotifyTriggeringSystemSettingChange(iTriggerSettings, iStatus);
        SetActive(); 
        }  
    }

// ---------------------------------------------------------------------------
// void CLbtMgmtPluginEngine::ShowInfoDialog
// It will return total active triggers
// ---------------------------------------------------------------------------
//
TInt CLbtMgmtLbtAdapter::ActiveTriggers()
    {
    return iTotalActiveTriggers;
    }

// --------------------------------------------------------------------------
// TInt CLbtMgmtLbtAdapter::GetTotalActiveTriggersL()
// Gets all active triggers from Location Triggering Server. 
// ---------------------------------------------------------------------------
//
void CLbtMgmtLbtAdapter::GetTotalActiveTriggersL()
    {
    LBTMGMTUIDEBUG("+ CLbtMgmtLbtAdapter::GetTotalActiveTriggersL()");  
    RArray<TLbtTriggerId> triggerList;
    
    //Construct list options
    //XXX: should use NewLC() when it is available
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
    CleanupStack::PushL(listOptions);
        
    //Construct an attribute filter
    CLbtTriggerFilterByAttribute* attribFilter = CLbtTriggerFilterByAttribute::NewLC();

    //Add trigger state to the filter. List only enabled triggers.
    attribFilter->AddTriggerStateL( CLbtTriggerEntry::EStateEnabled );
    
    //Add trigger validity to the filter. List only valid triggers.
    attribFilter->AddTriggerValidityL( TLbtTriggerDynamicInfo::EValid );
    
    listOptions->SetFilter(attribFilter);
    
    //List triggers

   LBTMGMTUIDEBUG("Before ListTriggerIdsL");  
   TRAPD( err, iLbtMgr.ListTriggerIdsL(triggerList, listOptions ) );
   LBTMGMTUIDEBUG("After ListTriggerIdsL"); 
   
   if( err == KErrNone )
       {
       iTotalActiveTriggers = triggerList.Count();
       }
    
    //Cleanup   
    triggerList.Close();
    CleanupStack::PopAndDestroy(2);//attribFilter, listOptions

    LBTMGMTUIDEBUG("- CLbtMgmtLbtAdapter::GetTotalActiveTriggersL()");  
    }

// --------------------------------------------------------------------------
// TInt CLbtMgmtLbtAdapter::DeleteActiveTriggersL()
// Deletes all active triggers from Location Triggering Server. 
// ---------------------------------------------------------------------------
//
void CLbtMgmtLbtAdapter::DeleteActiveTriggersL()
    {
    LBTMGMTUIDEBUG("+ CLbtMgmtLbtAdapter::DeleteActiveTriggersL()");  
    //Construct an attribute filter
    CLbtTriggerFilterByAttribute* attribFilter = CLbtTriggerFilterByAttribute::NewLC();

    //Add trigger state to the filter. List only enabled triggers.
    attribFilter->AddTriggerStateL( CLbtTriggerEntry::EStateEnabled );
    
    //Add trigger validity to the filter. List only valid triggers.
    attribFilter->AddTriggerValidityL( TLbtTriggerDynamicInfo::EValid );   

    LBTMGMTUIDEBUG("Before DeleteTriggersL"); 
    iLbtMgr.DeleteTriggersL( attribFilter );
    LBTMGMTUIDEBUG("After DeleteTriggersL"); 
    
    CleanupStack::PopAndDestroy( attribFilter );
    LBTMGMTUIDEBUG("- CLbtMgmtLbtAdapter::DeleteActiveTriggersL()");  
    }

// ---------------------------------------------------------------------------
// CLbtMgmtLbtAdapter::TriggerChangedL()
// Gets the notification on Change in Trigger.
// ---------------------------------------------------------------------------
//
void CLbtMgmtLbtAdapter::TriggerChangedL(
							const TLbtTriggerChangeEvent &aEvent)
	{
    LBTMGMTUIDEBUG("+ CLbtMgmtLbtAdapter::TriggerChangedL()"); 
	switch( aEvent.iEventType )
		{
		case ELbtTriggerChangeEventMultiple:
		case ELbtTriggerChangeEventDeleted:
		case ELbtTriggerChangeEventCreated:
		case ELbtTriggerChangeEventUpdated:
			GetTotalActiveTriggersL();
		    // Issue Notification
		    iObserver.HandleSettingsChangeL();
			break;
		default:
			break;
		}
    LBTMGMTUIDEBUG("- CLbtMgmtLbtAdapter::TriggerChangedL()"); 
	}

