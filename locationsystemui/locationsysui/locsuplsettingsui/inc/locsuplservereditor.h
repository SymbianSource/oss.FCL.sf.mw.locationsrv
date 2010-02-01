/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  supl server editor
*
*/


#ifndef __LOCSUPLSERVER_EDITOR_H__
#define __LOCSUPLSERVER_EDITOR_H__


#include <AknForm.h>
#include <ConeResLoader.h>

class CAknTitlePane;
class CLocSUPLSettingsUiEngine;

class MSuplServerEditorObserver
	{
public:
	
	virtual void UpdateIapL( const TDesC& aIapName ) = 0;
	};



/**
*  A dialog that makes it possible to edit certain fields of a supl server.
*/
class CLocSUPLServerEditor : 
    public CAknForm, public MSuplServerEditorObserver
    {
    public: // Constructors and destructor

        /**
 		* Static Two phase contructor that instantiates the CLocSUPLServerEditor
		* 
        */
        static CLocSUPLServerEditor* NewL( 
        					TBool aIsEditable,
                            CLocSUPLSettingsUiEngine&       aEngine,
        					TInt64 aSlpId
		);

        /**
 		* Static Two phase contructor that instantiates the CLocSUPLServerEditor
		* 
        */
        static CLocSUPLServerEditor* NewLC( 
        					TBool aIsEditable,
                            CLocSUPLSettingsUiEngine&       aEngine,
        					TInt64 aSlpId
		);

        /**
        * Destructor.
        */
        ~CLocSUPLServerEditor();

    public: // From CAknDialog

        /**
        * DynInitMenuPaneL initializes the menu pane before it is displayed.
        *
        * @param aResourceId a resource ID indetifying the menu bar containing
        * the menu pane to be initialized.
        * @param aMenuPane the menu pane to initialize.
        */
        void DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane);

        /**
        * Processes user commands.
        *
        * @param aCommandId id of the command to respond to
        */
        void ProcessCommandL(TInt aCommandId);

        /*
         * Executes the Dialog
         */
        TInt ExecuteLD();
	
	public: // from MSuplServerEditorObserver
	
	    /*
         * Changes IAP value in AP field
         */
		void UpdateIapL( const TDesC& aIapName );

	
    protected: // From CAknForm

     /**
       * Inherited from CCoeControl
       */				    
        void HandlePointerEventL(const TPointerEvent &aPointerEvent);
        
		TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent,
		                                             TEventCode aType );
		                                             
        /**
        * Handles a dialog button press for the specified dialog button.
        *
        * @param aButtonId
        * @return ETrue if OK to exit, EFalse to keep the dialog active
        */
        TBool OkToExitL(TInt aButtonId);

        /**
        * SaveFormDataL deals with the occasion of a change from
        * edit->view mode and the user wishes to save the changes.
        *
        * @return ETrue if the editable state can be left, EFalse otherwise
        */
        TBool SaveFormDataL();
        
    private: // From CEikDialog

        /**
        * PreLayoutDynInitL initialises the dialog's controls before the dialog
        * is sized and layed out.
        */
        void PreLayoutDynInitL();

        /**
        * PostLayoutDynInitL initialises the dialog's controls after the dialog
        * has been sized but before it has been activated.
        */
        void PostLayoutDynInitL();

        /**
        * Selects desired line on startup
        */
        void SetInitialCurrentLine();

       
        /** Handles layout switch */
        void HandleResourceChange( TInt aType );

		void HandleControlStateChangeL( TInt aControlId );
 
		void LineChangedL(TInt aControlId );
		
 	private:

        enum TErrorInForm
            {
            ENoErrors
            };

        /** Constructor
         *
         */
        CLocSUPLServerEditor( 
        					TBool aIsEditable,
                            CLocSUPLSettingsUiEngine&       aEngine,
        					TInt64 aSlpId
		);

        void ConstructL();

	private: // new functions
        /**
         * Change title pane text
         */
		void ChangeTitlePaneTextL( TDesC& aText );
		
        /**
         * Load intial data values
         */
        void LoadFormValuesFromDataL();

		/*
		 * It checks if server details are empty in editor.
		 * 
		 * @ret true if event is empty else false.
		 */
		TBool IsEmptyServerL();

		/*
		 * it toggles value of popup field
		 */        
		void TogglePopupFieldControlL( TInt aControlId );

		/*
		 * it changes the MSK caption 
		 */        
		void HandleMSKCaptionL();

		/*
		 * Confirmation query note for delete operation
		 */        
		TInt DeleteConfirmationQueryL();

		/*
		 * Shows Validation Info note for server address entry 
		 */        
		void ShowServerValidationInfoNoteL( TInt aResourceId );
		
		/*
		 * Sets the cursor position to end at server address field in editor
		 */        
		void SetCursorToEndL();
		

    private: // Data

        //! The title pane that is dynamically updated
        CAknTitlePane* 				iTitlePane;

        //! The original title that the title pane displayed before modified
        HBufC* 						iOriginalTitle;

       	/*
         * Mode in which editor is opened. It can be either view or edit mode.
         */   
       	TBool                    	iIsEditMode;
      
       	/*
         * Mode in which editor is opened. It can be either view or edit mode.
         */      
      	TBool 						iIsNewServer;
      	
	   	/*
	     *  help context object
   	     */
       	TCoeHelpContext         	iHelpContext;  

		/*
		 * Reference to the SUPL Settings engine
		 */
	    CLocSUPLSettingsUiEngine&	iEngine;	
	    
	    /**
         * SUPL Servers SLP ID to get and set the server attributes.
         * Owns.
         */
        TInt64						iSlpId;
        
	    /**
         * SUPL Server Address 
         * Owns.
         */
	    HBufC* 						iServerAddress;
	    
	    /**
         * SUPL Server Access point
         * Owns.
         */
	    HBufC* 						iIap;
	    
	    /**
         * SUPL Server usage in home network
         * Owns.
         */
	    TBool 						iUsageInHomeNw;
	    
	    /**
	    	* It determines if form is modified by user
	    	*/	    
	    TBool							iIsModified;
    };

#endif // __LOCSUPLSERVER_EDITOR_H__

