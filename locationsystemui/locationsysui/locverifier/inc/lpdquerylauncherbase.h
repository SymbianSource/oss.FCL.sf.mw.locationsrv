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
* Description:  Abstract base class for queries.
*
*/


#ifndef CLPDQUERYLAUNCHERBASE_H
#define CLPDQUERYLAUNCHERBASE_H

//  INCLUDES
#include <e32base.h>
#include <badesca.h>
#include <coemain.h>
#include <AknsItemID.h>
#include <AknQueryDialog.h>
#include <EPos_CPosPrivacyNotifier.h>


// SYSTEM INCLUDE

#include "lpddlgobserver.h"
// FORWARD DECLARATIONS
class CLpdGlobalPluginDialog;
class CLpdMessagePluginDialog;
class MLpdResultHandler;
class CEikFormattedCellListBox;
class CEikonEnv;

    
// CLASS DECLARATION

/**
*  Base class which handles the dialog and expiration timer.
*  Expiration timer can be used to ensure that if the user doesn't handle
*  a dialog in certain time the dialog is closed. NOTE: In current
*  implementation the timer is not started, i.e. dialogs never expire.
*  Implementations of this class should notify a result handler
*  when dialog result is available. This base class takes care of
*  notifying possible timeout with KErrTimedOut.
*
*  @lib locverifierdlg.lib
*  @since 2.1
*/
class CLpdQueryLauncherBase : public CBase, public MLpdDlgObserver
    {
    public:  // Constructors and destructor
                
        /**
        * Destructor. Exits possibly running dialog without callback.
        */
        virtual ~CLpdQueryLauncherBase();

    public: // New functions, query setup and launching

        /**
        * Method for creating a query dialog object and reading its
        * resources. Can and must be called once before launching.
        * @param aResourceId
        * @param aSuplNotifRequest Specifies if periodic notification request or not
        */
        virtual void PrepareQueryResourcesL( TInt aResourceId, TInt aSuplNotifRequest );

        /**
        * Method for setting text array.
        * @param aArray listbox array, ownership is transferred
        */
        virtual void SetQueryTextArray( MDesCArray* aArray );

        /**
        * Method for setting message text for Message Query dialog.
        */
		virtual void SetMessageQueryTextL( const TDesC& aString, TCallBack aCallBack );
		
        /**
        * Method for launching query, note that it must have been
        * correctly prepared first.
        */
        virtual void StartQueryL();

       /**
        * Method for launching query for supl periodic request
        * note that it must have been correctly prepared first.
        */
        virtual void StartSuplPeriodicQueryL();

    public: // Other new functions

        /**
        * This method returns indexes of selected listbox items.
        * Return value is valid after dialog has been finished
        * (not cancelled) and new one has not yet been started.
        * @return selected item indexes, ownership is not transferred
        */
        const CListBoxView::CSelectionIndexArray* SelectionIndexes() const;

        /**
        * Static timeout callback needed by the expiration timer.
        * @param aThis, see TCallBack documentation
        * @return always KErrNone
        */
        static TInt HandleTimeoutCallback( TAny* aThis );
        

        /**
        * Method for cancelling query. Client classes can call this
        * method if query canceling is needed.
        */
        void Cancel();
        
  
        void CancelQuietly();

        /**
        * Returns a reference to the listbox (for formatting purposes).
        * Leaves if listbox is NULL.
        * @return listbox reference
        */
        CEikFormattedCellListBox& ListBoxL();
 
        /**
         * Sets the requestor buffer array that needs to be published to
         * Cover UI
         * @param aRequestorBuffer Flat buffer containing the list of requestors
         */
        void SetRequestorBuffer( TDes8&     aRequestorBuffer );
     
        /**
         * Sets the Cover UI Command ID
         * @param aCommandId Cover UI Command
         */
        void SetCoverUICommand( TInt        aCommand );
        

    public: // Functions from base classes

        /**
        * From MLpdDlgObserver, called when dialog is dismissed.
        * This method calls also pure virtual method
        * ExtendedHandleDlgDismissedL().
        * @param aResult code of the query
        * @param aCurrentIndex current item index
        */
        void HandleDlgDismissedL( TInt aResult, TInt aCurrentIndex);

    protected: // Constructors and destructors

        /**
        * C++ constructor.
        * @param aHandler of the query result
        */
        CLpdQueryLauncherBase( MLpdResultHandler& aHandler );

        /**
        * Symbian 2nd phase constructor.
        */
        void BaseConstructL();

    protected: // New functions

        /**
        * This pure virtual method must be implemented
        * by the derived class to handle the result code
        * appropriately and to notify the observer of this
        * class.
        */
        virtual void ExtendedHandleDlgDismissedL() = 0;

    private: // New functions

        /**
        * Internal method for handling timeout, called from static
        * TCallBack method.
        */
        void HandleTimeout();
        
        /**
        * General internal method for exiting query, called by Cancel() or
        * HandleTimeout(), or from the destructor.
        * @param aCallback ETrue if callback from the dialog is wanted.
        */
        void ExitQuery( TBool aCallback );
   
     
        /**
         * Prepare the requestor buffers and Publish the dialog to the 
         * Cover UI
         *
         */
        void PublishDialogtoCoverL();


    protected: // Data

        // Reference to the result handler
        MLpdResultHandler& iHandler;

        // Result code from the dialog
        TInt iResult;

        // Current listbox item when dialog dismiss occurred
        TInt iCurrentIndex;

        // Selected listbox items when dialog dismiss occurred
        CListBoxView::CSelectionIndexArray* iSelectionIndex; // owns
        
        // Exit reason in case of result code EAknSoftkeyExit
        TInt iExitReason;

        // Indicates whether this object owns the QueryDlg or
        // whether it is given away
        TBool iOwnsQueryDlg;
       
        // Indicates whether this object owns the msgQueryDlg or
        // whether it is given away 
        TBool iOwnsMsgQueryDlg;

        // Pointer to the list query dialog
        CLpdGlobalPluginDialog* iQueryDlg; // owns untils RunLD() called
        
        // Pointer to the message query dialog
        CLpdMessagePluginDialog* iMsgQueryDlg;

        // Expiration timer, not started in current implementation
        CPeriodic* iTimer; // owns
        
        TPtrC8     iRequestorBuffer;
        
        TInt       iCoverUiCommand;                        
        
    };

#endif      // CLPDQUERYLAUNCHERBASE_H   
            
// End of File
