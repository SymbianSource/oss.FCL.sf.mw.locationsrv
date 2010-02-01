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
* Description:  Message query dialog suitable for verifier plug-in use.
*
*/


#ifndef CLPDMESSAGEPLUGINDIALOG_H
#define CLPDMESSAGEPLUGINDIALOG_H

// INCLUDES
#include <aknmessagequerydialog.h>
#include "lpdbkupevtobserver.h"

// FORWARD DECLARATIONS
class MLpdDlgObserver;
class CLpdBkupEvtListenerAO;
class MLpdBkupEvtObserver;

// CLASS DECLARATION

/**
*  A message query dialog suitable for verifier plug-in use.
*  Inherits from CAknMessageQueryDialog, utilizes MLpdDlgObserver
*  for notifying when dialog is dismissed.
*
*  @lib locverifierdlg.lib
*  @since 2.1
*/
class CLpdMessagePluginDialog : public CAknMessageQueryDialog, public MLpdBkupEvtObserver
	{
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aCallBack is notified when dialog is dismissed
        * @param aSelfPtr caller's ptr is set to null when dialog is deleted
        * @param aSelected will contain selected items when available, i.e.
        *                  since observer callback (in multiselectionlist)
        * @return created object
        */
        static CLpdMessagePluginDialog* NewL(
                    MLpdDlgObserver& aCallBack,
                    CLpdMessagePluginDialog ** aSelfPtr
                    );

		
        /**
        * C++ destructor.
        */
        virtual ~CLpdMessagePluginDialog();

    public: // Functions from base classes

        /**
        * From CAknDialog. Wrapper method for enabling better unit testing
        * when using a stub implementation.
        * @param aResourceId
        */
        void PrepareLC( TInt aResourceId );

        /**
        * Wrapper that suppresses application switching
        * and call's base class RunLD().
        */
        TInt RunLD();
        

		void HandleResourceChange(TInt aType);


    public: // New functions

        /**
        * Allows exiting the dialog without user action.
        * Can be called e.g. in case of timeout. Main reason for
        * this method is that deleting a dialog is not a proper
        * way to exit it. Calling this method causes
        * MLpdDlgObserver::HandleDlgDismissedL() callback with
        * EEikCmdExit result.
        */
        void ExitDialogL();

        /**
        * Similar as ExitDialogL(), but no callback method
        * is called.
        */
        void ExitDialogQuietlyL();

    protected: // Functions from base classes

        /**
        * From CAknListQueryDialog.
        * @param aButtonId
        * @return ETrue if dialog can be dismissed
        */
        TBool OkToExitL(TInt aButtonId);

    private:  // Constructors and destructor

        /**
        * Symbian 2nd Phase Constructor.
        */
		void ConstructL();

        /**
        * C++ constructor.
        * @param aCallBack is notified when dialog is dismissed
        * @param aSelfPtr caller's ptr is set to null when dialog is deleted
        * @param aSelected will contain selected items when available, i.e.
        *                  since observer callback (in multiselectionlist)
        */
        CLpdMessagePluginDialog(
                    MLpdDlgObserver& aCallBack,
                    CLpdMessagePluginDialog ** aSelfPtr
                   );

    private:    // Data

        // Object which is notified when query is dismissed
        MLpdDlgObserver& iCallBack;

        // Holds the status of application switching
        TBool iAppSwitchSuppressed;

        // Caller's pointer to this query object is set to NULL
        // when ExecuteLD destroys the object
        CLpdMessagePluginDialog** iSelfPtr;
        
        // Reference to the Backup Event Listener
        CLpdBkupEvtListenerAO* iBkupListener;
        
	};

#endif      // CLPDMESSAGEPLUGINDIALOG_H   
            
// End of File
