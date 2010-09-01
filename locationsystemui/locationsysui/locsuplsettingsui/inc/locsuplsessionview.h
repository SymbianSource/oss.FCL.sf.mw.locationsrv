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
* Description:  SUPL session UI view
*
*/

#ifndef C_LOCSUPLSESSIONVIEW_H
#define C_LOCSUPLSESSIONVIEW_H

//  System Includes
#include <data_caging_path_literals.hrh>
#include <e32base.h>
#include <aknview.h>
#include <ConeResLoader.h>

// User Includes
#include "locsuplsettingsuid.hrh"
#include "locsuplsettingseventhandler.h"

// Forward Declarations
class CLocSUPLSessionContainer;
class MLocSUPLSettingsEventHandler;
class CLocSUPLSettingsUiEngine;

// Constant Declarations
const TUid KLocSUPLSessionUiViewId = { KLOCSUPLSESSIONUIUID3 };

// Class Declaration

/**
 * SUPL server UI View. 
 */
class CLocSUPLSessionView : public CAknView
	{
    public:
        /**
         * Creates new SUPL server UI View.
         *
         * @param aEngine                   Reference to the SUPL Settings
         *                                  engine
         * @param aEventHandler			    Reference to the Event Handler
         * @return CLocSUPLSessionView*	Reference to the application view
         */                                                 
        static CLocSUPLSessionView* NewL( 
                            CLocSUPLSettingsUiEngine&       aEngine,
        					MLocSUPLSettingsEventHandler&	aEventHandler );

		/**
         * Creates new SUPL Settings UI View. Leaves the object on the 
         * Clean up stack
         *
         * @param aEngine                   Reference to the SUPL Settings
         *                                  engine          
         * @param aEventHandler			    Pointer to the Event Handler
		 * @return CLocSUPLSessionView*	Reference to the application view
         */                                                 
        static CLocSUPLSessionView* NewLC( 
                            CLocSUPLSettingsUiEngine&       aEngine,
         					MLocSUPLSettingsEventHandler&	aEventHandler );
        
		/**
		 * Destructor
		 */
		~CLocSUPLSessionView ();
		
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
       
       /**
         * Sets MSK Button Group
         */	
         void ChangeMSKL();
                
	private:
		/**
 		 * C++ Overloaded Constructor
		 */
		CLocSUPLSessionView( CLocSUPLSettingsUiEngine&     aEngine,
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
		 * Confirmation query to delete
		 */
		TInt DeletionConfirmationQueryL( const TInt aAmount );
		
		/**
		 * Deletes the selected server/s
		 */
		void DeleteSessionL();

		
		/**
		 * Gets the ids for selected items
		 */		
		void GetSelectedItemIdsL(
		    RArray<TInt64>& aSelectedItems,
		    const CArrayFix<TInt>& aSelectedIndexes ) const;
		
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
		CLocSUPLSessionContainer*		iContainer;
	};			

#endif // C_LOCSUPLSESSIONVIEW_H

