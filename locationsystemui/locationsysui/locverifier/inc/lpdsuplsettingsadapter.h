/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Adapter to the SUPL Settings API
*
*/


#ifndef CLPDSUPLSETTINGSADAPTER_H_
#define CLPDSUPLSETTINGSADAPTER_H_

// System Includes
#include <e32base.h>
#include <epos_csuplsettingparams.h>		// class CServerParams
#include <epos_csuplsettings.h>				// class CSuplSettings
#include <epos_msuplsessionobserver.h>

// User Includes

// Forward Declarations
class MLpdSUPLSettingsAdapterObserver;

// Class Declaration

/**
 * Adapter class for the SUPL settings API. Handles all the interfacing
 * functionality to the SUPL Settings API. It also observes for the Session
 * changes communicated through the MSuplSessionObserver
 */
class CLpdSUPLSettingsAdapter : public CBase, public MSuplSessionObserver
    {             
    public:
        /**
 		 * Static Two phase contructor that instantiates the CLpdSUPLSettingsAdapter
		 * 
		 * @param aObserver				    Observer to the SUPL session change
		 * @return CLpdSUPLSettingsAdapter*	Reference to the object created
		 */
        static CLpdSUPLSettingsAdapter* NewL(
                            MLpdSUPLSettingsAdapterObserver&    aObserver );

        /**
         * Destructor
         */
        virtual ~CLpdSUPLSettingsAdapter();

    private:
        /**
         * Private Constructor
         */
        CLpdSUPLSettingsAdapter( MLpdSUPLSettingsAdapterObserver&    aObserver );

        /**
         * Second phase of the two phase constructor
         */
        void ConstructL();
        
        


              	
	public:
	    /** 
	     * Inherited from MSuplSessionObserver
	     */	
		void HandleSuplSessionChangeL( 
                TSuplSessionEventType aEvent,
                TInt64 aSessionId                         
        		);	
        
         /** 
	     * Gets time period of a trigger based on session id
	     */		
        void GetTriggerPeriodL( TInt64 aSessionId, TUint& aTimePeriod );
    				
        

    private:
        
        /**
         * Observer for the specific SUPL session.
         * Does not own.
         */
		MLpdSUPLSettingsAdapterObserver& iObserver;
		
        /**
         * SUPL Settings UI
         * Owns
         */
        CSuplSettings*                      iSUPLSettings;
                
    };

#endif      // CLPDSUPLSETTINGSADAPTER_H_

// End of File
