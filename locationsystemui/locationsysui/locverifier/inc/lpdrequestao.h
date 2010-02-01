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


#ifndef CLPDREQUESTAO_H
#define CLPDREQUESTAO_H

//  INCLUDES
#include <e32base.h>
#include <lbs/epos_privacy.h>

#include "lpdnotifreqinfo.h"

// FORWARD DECLARATIONS
class CLpdVerifierPlugin;
class RPosRequestorStack;

// CLASS DECLARATION

/**
*  Active object used by the verifier plugin. 
*  Ensures that current call stack is run to completion before a 
*  new request is handled. Otherwise access violations might occur.
*
*  @lib locverifierdlg.lib
*  @since 2.1
*/
class CLpdRequestAO : public CActive
    {    
	public: // Constructors and destructors        

        /**
        * Two-phased constructor.
        * @param aPlugin reference to the owner of this object
        * @return created object
        */
	    static CLpdRequestAO* NewL( CLpdVerifierPlugin& aPlugin );        

        /**
        * Destructor.
        */
	    virtual ~CLpdRequestAO();

    public: // New functions

        /**
        * Issues a request and completes it immediatelly, so that it
        * will be handled as soon as previous request is run to completion.
        */
        void ScheduleRequest();

        /**
        * To Queue up further verification cancel requests when a notification
        * request is currently running.
        */
		void EnqueueRequestL( TPosRequestSource aSource,
						     TPosVerifyCancelReason aReason,
			 			     TPosRequestDecision aDecision,
			 			     RPosRequestorStack* aRequestors );
		
    private:

	    /**
        * Private C++ default constructor.
        * @param aPlugin reference to the owner of this object
        */
		CLpdRequestAO( CLpdVerifierPlugin& aPlugin ); 	    

    private: // Functions from base classes
        
        /**
        * From CActive.
        */
        void RunL();
        
        /**
        * From CActive.
        */
        void DoCancel();
    	
    private: // Data

        // Verifier plugin
        CLpdVerifierPlugin& iPlugin; // doesn't own     

		// Queue of Notification Request with all the necesary information.        
        RPointerArray<CLpdNotifReqInfo> iNotificationQue;
};

#endif      // CLPDREQUESTAO_H

// End of File