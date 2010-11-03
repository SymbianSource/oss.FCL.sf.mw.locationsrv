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
* Description:  supl session editor
*
*/


#ifndef __LOCSUPLSESSION_EDITOR_H__
#define __LOCSUPLSESSION_EDITOR_H__


#include <AknForm.h>
#include <ConeResLoader.h>

#include "locsuplsettingssessionobserver.h"

class CAknTitlePane;
class CLocSUPLSettingsUiEngine;

/**
*  A dialog that makes it possible to edit certain fields of a supl session.
*/
class CLocSUPLSessionEditor : 
    public CAknForm, public MLocSUPLSettingsSessionObserver
    {
    public: // Constructors and destructor

        /**
 		* Static Two phase constructor that instantiates the CLocSUPLSessionEditor
		* 
        */
        static CLocSUPLSessionEditor* NewL( 
                            CLocSUPLSettingsUiEngine&       aEngine,
        					TInt64 aSessionId
		);

        /**
 		* Static Two phase constructor that instantiates the CLocSUPLSessionEditor
		* 
        */
        static CLocSUPLSessionEditor* NewLC( 
                            CLocSUPLSettingsUiEngine&       aEngine,
        					TInt64 aSessionId
		);

        /**
        * Destructor.
        */
        ~CLocSUPLSessionEditor();

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
	
	public: // from MLocSUPLSettingsSessionObserver
	
        /*
         * Calls function to load the values into the editor fields
         */
		void UpdateSessionL( TUpdateType aType );
		
        /*
         * Returns the session id
         */
		TInt64 SessionId( );

	
    protected: // From CAknForm

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
        //TBool SaveFormDataL();
        
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
        * Gets the Time String from Time(in seconds)
        */
		void GetTimeStringL(HBufC*& aTimeString, TUint aSeconds);
 
		void LineChangedL(TInt aControlId );
		
 	private:

        /** Constructor
         *
         */
        CLocSUPLSessionEditor( 
                            CLocSUPLSettingsUiEngine&       aEngine,
        					TInt64 aSessionId
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

    private: // Data

        //! The title pane that is dynamically updated
        CAknTitlePane* 				iTitlePane;

        //! The original title that the title pane displayed before modified
        HBufC* 						iOriginalTitle;
      	
	   	/*
	     *  help context object
   	     */
       	TCoeHelpContext         	iHelpContext;  

		/*
		 * Reference to the SUPL Settings engine
		 */
	    CLocSUPLSettingsUiEngine&	iEngine;	
	    
	    /**
         * SUPL Session unique Id.
         */
        TInt64						iSessionId;

		/*
		 * Reference to the SUPL Settings engine
		 * Own:
		 */
	    HBufC*	iSessionName;
    };

#endif // __LOCSUPLSESSION_EDITOR_H__

