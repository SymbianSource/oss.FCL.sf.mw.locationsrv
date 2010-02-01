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

#ifndef CLBTMGMTLBTADAPTER_H_
#define CLBTMGMTLBTADAPTER_H_

//  System Includes
#include <e32base.h>
#include <lbtserver.h>
#include <lbtmanager.h>
#include <lbt.h>
#include <lbttriggerchangeeventobserver.h> 

// User Includes

// Forward Declarations
class MLbtMgmtAdapterObserver;
class CLbtTriggerChangeEventNotifier;

// Constant Declarations

// Class Definition

/**
 * Adapter to the Lbt operations
 */
class CLbtMgmtLbtAdapter : public CActive,
								public MLbtTriggerChangeEventObserver
    {
    public:
        /**
         * Enumeration for Trigger Status Values
         */         
        enum TTriggeringStatus
            {
            /**
             * Enumeration to display Trigger Status Value as On
             */
            EOn = 0x00000000,
            
            /**
             * Enumeration to display Trigger Status Value as Off
             */             
            EOff            
            };            
    public:
        /**
         * Creates new Lbt adapter
         *
         * @param  aAdapterObsrv                        Observer to the Lbt adapter
         * @return CLbtMgmtLbtAdapter& Reference to the adapter
         */                                                 
        static CLbtMgmtLbtAdapter* NewL( 
                            MLbtMgmtAdapterObserver& aAdapterObsrv );


        /**
         * Creates new Lbt adapter
         * Leaves the object on the Clean up stack         
         *
         * @param  aAdapterObsrv                        Observer to the Lbt adapter
         * @return CLbtMgmtLbtAdapter& Reference to the adapter
         */                                                 
        static CLbtMgmtLbtAdapter* NewLC( 
                            MLbtMgmtAdapterObserver& aAdapterObsrv );    

        
        /**
         * Destructor
         */
        ~CLbtMgmtLbtAdapter ();
        
        /**
         * Returns the Location Trigger Status settings value.
         *
         * @return TTriggeringStatus - Current Trigger Status.
         */
        CLbtMgmtLbtAdapter::TTriggeringStatus GetCurrentTriggeringStatusL();
        
        /**
         * Sets the value of Location Trigger Status settings value.
         *
         * @param aTiggerStatus - Trigger Status Value.
         */
        void SetCurrentTriggeringStatusL( CLbtMgmtLbtAdapter::TTriggeringStatus aTiggerStatus );
        
        /**
         * Toggle the value of Location Trigger Status settings value.
         *
         */
        void ToggleTiggerStatusL();
        
        /**
         * Deletes all active triggers
         */           
        void DeleteActiveTriggersL();
        
        /**
        * Return the total active triggers from Location Triggering Server.
        *
        * @return TInt - Total active triggers.
        */
        TInt ActiveTriggers();
        
        /*
         * Derived from MLbtTriggerChangeEventObserver
         */
    	void TriggerChangedL(const TLbtTriggerChangeEvent &aEvent);
        

    protected:
        /**
         * Inherited from CActive
         */
        void RunL();
        
        /**
         * Inherited from CActive
         */
        void DoCancel();
                    
    private:
        
        /**
        * Gets the total active triggers from Location Triggering Server.
        */
        void GetTotalActiveTriggersL();
        
        /**
         * Constructor
         */
        CLbtMgmtLbtAdapter( MLbtMgmtAdapterObserver&    aAdapterObsrv );

        /**
         * Second Phase Constructor
         */
        void ConstructL();
       
        /**
         * Start Notification
         */
        void StartNotification();
        
    private:
                
        /**
         * Reference to the Adapter observer
         */
        MLbtMgmtAdapterObserver&     iObserver;
        
        /**
         * Reference to the Lbt Server
         *
         */
       	 RLbtServer iLbtServ;
       	 
        /**
         * Reference to the Lbt Manager Session
         *
         */
       	 RLbtManager iLbtMgr;
       	 
        /**
         * Reference to the Lbt Manager Session
         *
         */
       	 RLbtManager iLbtMgrObserver;
       	 
        /**
         * Reference to the Lbt Manager Session
         *
         */
       	 CLbtTriggerChangeEventNotifier * iEventChangeObserver;
       	 
        /**
         * Triggering System structure
         *
         */
       	 TLbtTriggeringSystemManagementSettings iTriggerSettings;
       	 
        /**
         * Total Active Triggers
         *
         */
    	 TInt iTotalActiveTriggers;
    };          

#endif // CLBTMGMTLBTADAPTER_H_

