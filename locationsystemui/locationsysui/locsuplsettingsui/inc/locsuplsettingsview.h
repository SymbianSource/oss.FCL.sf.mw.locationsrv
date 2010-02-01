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
* Description:  SUPL Settings UI view
*
*/

#ifndef C_LOCSUPLSETTINGSVIEW_H_
#define C_LOCSUPLSETTINGSVIEW_H_

//  System Includes
#include <data_caging_path_literals.hrh>
#include <e32base.h>
#include <aknview.h>
#include <ConeResLoader.h>

// User Includes
#include "locsuplsettingsuid.hrh"
#include "locsuplsettingseventhandler.h"

// Forward Declarations
class CLocSUPLSettingsContainer;
class MLocSUPLSettingsEventHandler;
class CLocSUPLSettingsUiEngine;


// Constant Declarations
const TUid KLocSUPLSettingsUiViewId = { KLOCSUPLSETTINGSUIUID3 };
_LIT( KLocSUPLSettingsUiResFileName, "locsuplsettingsui.rsc" );

// Class Declaration

/**
 * SUPL Settings UI View. 
 * This class also handles all View specific events for SUPL settings
 */
class CLocSUPLSettingsView : public CAknView
	{
    public:
        /**
         * Creates new SUPL Settings UI View.
         *
         * @param aEngine                   Reference to the SUPL Settings
         *                                  engine
         * @param aEventHandler			    Reference to the Event Handler
         * @return CLocSUPLSettingsView*	Reference to the application view
         */                                                 
        static CLocSUPLSettingsView* NewL( 
                            CLocSUPLSettingsUiEngine&       aEngine,
        					MLocSUPLSettingsEventHandler&	aEventHandler );

		/**
         * Creates new SUPL Settings UI View. Leaves the object on the 
         * Clean up stack
         *
         * @param aEngine                   Reference to the SUPL Settings
         *                                  engine          
         * @param aEventHandler			    Pointer to the Event Handler
		 * @return CLocSUPLSettingsView*	Reference to the application view
         */                                                 
        static CLocSUPLSettingsView* NewLC( 
                            CLocSUPLSettingsUiEngine&       aEngine,
         					MLocSUPLSettingsEventHandler&	aEventHandler );
        
		/**
		 * Destructor
		 */
		~CLocSUPLSettingsView ();
		
        /**
         * Handles the Screen Size update
         */
        void HandleScreenSizeChange();
        
        /**
         * Updates the View
         */
        void UpdateView();
        
        /**
         * Indicates that the SUPL settings view is started from the Location
         * settings UI server
         */
        void StartedFromAppServer();
            
        /**
		 * Returns the Selected element. The list box model defines the
		 * enumeration that would determine the current element that has been
		 * selected
		 *
		 * @return TInt Index of the item selected
		 */
		TInt SelectedItemIndex();
		         
		/**
         * From CAknView. 
         */
		void DoActivateL( const TVwsViewId& aPrevViewId,
		 						 TUid		 aCustomMessageId,
						   const TDesC8&	 aCustomMessage);
		
		/**
         * From CAknView. 
         */
        void DoDeactivate();
 
 		/**
         * From CAknView. Returns the View Id. Should be chosen to reflect 
		 * the Implementation UID of the Plugin
         */
        TUid Id() const;

        /**
         * From CAknView. 
         */
        void HandleCommandL(TInt aCommand);
        
       /**
        * From MEikMenuObserver
        */
        void DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane );
                	
        /**
         * Resets the Setting View Configuration
         */
        void Reset();	
		
		/**
		 * Confirmation query to delete
		 */
		TInt DeletionConfirmationQueryL( );
    
	private:
		/**
 		 * C++ Overloaded Constructor
		 */
		CLocSUPLSettingsView( CLocSUPLSettingsUiEngine&     aEngine,
		 					  MLocSUPLSettingsEventHandler&	aEventHandler );

		/**
		 * Second Phase Constructor
		 */
		void ConstructL();

       /**
        * Shows or dims help option, depending wheather help feature
        * is supported or not.
        * @param aMenuPane
        */
        void HandleHelpFeature( CEikMenuPane& aMenuPane ) const;		
		
       /**
        * Changes the command button groups for the view
        */
		void ChangeMSKL();
			
	private:		
		/**
		 * Reference to the event Handler for handling UI events
		 */
		MLocSUPLSettingsEventHandler&		iEventHandler;
	
	    /**
	     * Reference to the SUPL Settings engine
	     */
	    CLocSUPLSettingsUiEngine&           iEngine;	
		/**
		 * Container Object
		 * Owns
		 */
		CLocSUPLSettingsContainer*			iContainer;
		
		/**
		 * Boolean value to indicate that the SUPL settings UI is
		 * started from the settings UI server.
		 */
		TBool                               iStartedFromServer;

		/**
		 * Integer value to indicate whether the SUPL server view
                 * or session view was launched
		 */
		TInt                               iViewLaunched;
		
	};			

#endif // C_LOCSUPLSETTINGSVIEW_H_

