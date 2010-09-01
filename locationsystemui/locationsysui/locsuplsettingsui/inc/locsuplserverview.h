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
* Description:  SUPL server UI view
*
*/

#ifndef C_LOCSUPLSERVERVIEW_H_
#define C_LOCSUPLSERVERVIEW_H_

//  System Includes
#include <data_caging_path_literals.hrh>
#include <e32base.h>
#include <aknview.h>
#include <ConeResLoader.h>

// User Includes
#include "locsuplsettingsuid.hrh"
#include "locsuplsettingseventhandler.h"

// Forward Declarations
class CLocSUPLServerContainer;
class MLocSUPLSettingsEventHandler;
class CLocSUPLSettingsUiEngine;
class CLocSUPLServerEditor;

// Constant Declarations
const TUid KLocSUPLServerUiViewId = { KLOCSUPLSERVERUIUID3 };

// Class Declaration

/**
 * SUPL server UI View. 
 */
class CLocSUPLServerView : public CAknView
	{
    public:
        /**
         * Creates new SUPL server UI View.
         *
         * @param aEngine                   Reference to the SUPL Settings
         *                                  engine
         * @param aEventHandler			    Reference to the Event Handler
         * @return CLocSUPLServerView*	Reference to the application view
         */                                                 
        static CLocSUPLServerView* NewL( 
                            CLocSUPLSettingsUiEngine&       aEngine,
        					MLocSUPLSettingsEventHandler&	aEventHandler );

		/**
         * Creates new SUPL Settings UI View. Leaves the object on the 
         * Clean up stack
         *
         * @param aEngine                   Reference to the SUPL Settings
         *                                  engine          
         * @param aEventHandler			    Pointer to the Event Handler
		 * @return CLocSUPLServerView*	Reference to the application view
         */                                                 
        static CLocSUPLServerView* NewLC( 
                            CLocSUPLSettingsUiEngine&       aEngine,
         					MLocSUPLSettingsEventHandler&	aEventHandler );
        
		/**
		 * Destructor
		 */
		~CLocSUPLServerView ();
		
        /**
         * Handles the Screen Size update
         */
        void HandleScreenSizeChange();
        
        /**
         * Updates the View
         */
        void UpdateView();
		
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
        void HandleCommandL( TInt aCommand );
        
       /**
        * From MEikMenuObserver
        */
        void DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane );
                
	private:
		/**
 		 * C++ Overloaded Constructor
		 */
		CLocSUPLServerView( CLocSUPLSettingsUiEngine&     aEngine,
		 					MLocSUPLSettingsEventHandler& aEventHandler );

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
		 * Second Phase Constructor
		 */
		TInt DeletionConfirmationQueryL( const TInt aAmount );
		
		/**
		 * Deletes the selected server/s
		 */
		void DeleteServerL();
		
		/**
		 * Sets the selected server/s as disable
		 */
		void DisableServerL();
		
		/**
		 * Sets the selected server/s as enable
		 */
		void EnableServerL();

		/**
		 * Sets MSK Button Group
		 */		
		void ChangeMSKL();
		
		/**
		 * Gets the ids for selected items
		 */		
		void GetSelectedItemIdsL(
		    RArray<TInt64>& aSelectedItems,
		    const CArrayFix<TInt>& aSelectedIndexes ) const;
		
		/**
		 * Gets the count for disabled servers
		 */		
		TInt SelectedDisabledServerCountL();
		
	private:		
		/**
		 * Reference to the event Handler for handling UI events
		 */
		MLocSUPLSettingsEventHandler&	iEventHandler;
	
	    /**
	     * Reference to the SUPL Settings engine
	     */
	    CLocSUPLSettingsUiEngine&     	iEngine;	

		/**
		 * Container Object
		 * Owns
		 */
		CLocSUPLServerContainer*		iContainer;
		
		/**
         * Container Object used while prioritizing the servers
         * TC Id: ou1cimx1#219289
         * Owns
         */		
	      CLocSUPLServerContainer*        iNewContainer;
		/**
		 * Boolean value to indicate that the SUPL settings UI is
		 * started from the settings UI server.
		 */
		TBool                         	iStartedFromServer;
		
		/**
		 * Boolean value to start the prioritize operation
		 */		
		TBool							iStartedPrioritize;
		
		/**
		 * Value to store the selected server index 
		 * for prioritize operation
		 */				
		TInt 							iSelectedIndex;
		
		/**
		 * Value to store the selected server's slpId 
		 * for prioritize operation
		 */				
		TInt64							iSelectedSlpId;
	};			

#endif // C_LOCSUPLSERVERVIEW_H_

