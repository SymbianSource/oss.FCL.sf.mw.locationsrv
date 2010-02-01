/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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


//  INCLUDES
#include <e32base.h>
#include <e32property.h>

#ifndef CLPD_BKUPEVT_LISTENER_H
#define CLPD_BKUPEVT_LISTENER_H

//FORWARD DECLARATIONS
class MLpdBkupEvtObserver;
// CLASS DECLARATION

/**
*  Active object used by the verifier dialog. 
*  Ensures that during backup / Restore start the dialog is closed
*
*  @lib locverifierdlg.lib
*  @since 3.0
*/
NONSHARABLE_CLASS(CLpdBkupEvtListenerAO) : public CActive
    {    
	public: // Constructors and destructors        

        /**
        * Two-phased constructor.
        * @param aDlg reference to the owner of this object
        * @return created object
        */
	    static CLpdBkupEvtListenerAO* NewL( MLpdBkupEvtObserver& aDlg );        

        /**
        * Destructor.
        */
	    virtual ~CLpdBkupEvtListenerAO();

    public: // New functions
    
    	/**
    	* Start subscribing to Backup/Restore Events
    	* Leaves in case the backup is already running
    	*/
    	void StartL();

    private:

	    /**
        * Private C++ default constructor.
        * @param aDlg reference to the owner of this object
        */
		CLpdBkupEvtListenerAO( MLpdBkupEvtObserver& aDlg ); 	    

	    /**
        * Private Symbian 2nd Phase constructor.
        */
		void ConstructL();
		
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

        // Reference to backup event observer
        MLpdBkupEvtObserver& iBkupObserver; // doesn't own   
        // Reference to the Backup/Restore Property to Subscribe.
        RProperty iProperty;  
};

#endif      // CLPD_BKUPEVT_LISTENER_H

// End of File
