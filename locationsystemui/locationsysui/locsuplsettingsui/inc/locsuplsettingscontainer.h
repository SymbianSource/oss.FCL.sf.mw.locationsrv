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
* Description:  SUPL Settings UI View's container
*
*/


#ifndef C_LOCSUPLSETTINGSCONTAINER_H_
#define C_LOCSUPLSETTINGSCONTAINER_H_

// System Includes 
#include <e32base.h>
#include <coecntrl.h>
#include <eiklbo.h>

    
// Forward Declarations
class CLocSUPLSettingsView;
class CAknSettingStyleListBox;
class CLocSUPLSettingsLBModel;
class CLocSUPLSettingsUiEngine;

// Class Declarations
/**
 * Container for the SUPL Settings UI View.
 * 
 * Contains the Listbox control that is used to configure various SUPL settings 
 * options
 *
 * @lib locsuplsettings.lib
 * @since S60 v3.1 
 */
class CLocSUPLSettingsContainer : public CCoeControl,
                                  public MEikListBoxObserver
	{
	public:
		/**
 		 * Static Two phase contructor that instantiates the CLocSUPLSettingsContainer
		 * 
		 * @param aRect				            Client rectangle
         * @param aEngine                       Reference to the SUPL Settings
         *                                      engine 		 
         * @param aSettingsView                 Reference to the container's view
		 * @return CLocSUPLSettingsContainer*	Reference to the object created
		 */
		static CLocSUPLSettingsContainer* NewL( 
		                        const TRect& 	                    aRect,
		                              CLocSUPLSettingsUiEngine&     aEngine,
		                              CLocSUPLSettingsView&         aSettingsView );

		/**
		 * Static Two phase contructor that instantiates the CLocSUPLSettingsContainer
		 * Leaves the object on the Cleanup stack
		 * 
		 * @param aRect				            Client rectangle
         * @param aEngine                       Reference to the SUPL Settings
         *                                      engine 		 
         * @param aSettingsView                 Reference to the container's view		 
		 * @return CLocSUPLSettingsContainer*	Reference to the object created
		 */
		static CLocSUPLSettingsContainer* NewLC( 
		                        const TRect&                        aRect,
		                              CLocSUPLSettingsUiEngine&     aEngine,
		                              CLocSUPLSettingsView&         aSettingsView );

		/**
		 * Destructor
		 */
		~CLocSUPLSettingsContainer();
    
        /**
         * Refreshes the screen so that the list box reflects the latest contents
         */
         void UpdateScreen();
         
        /**
		 * Returns the Selected element. The list box model defines the
		 * enumeration that would determine the current element that has been
		 * selected
		 *
		 * @return TInt Index of the item selected
		 */
		TInt SelectedItemIndex();
		 
 		/**
		 * Sets focus to server detail or Active services item
		 * @param aIndex	Helps to know if server or session item has to be focussed
		 */
      	void FocusToItemL( TInt aIndex );
        
        /**
         * Returns the Active session count.
         *
         * @return TInt  SUPL Active sessions count.
         */
        TInt ActiveSessionCount( ) const;

	protected:
	
		/**
		 * Inherited from CCoeControl
		 */
		void HandleResourceChange(TInt aType);

        /**
         * Returns the help context
         *
         * @param aContext	The help context that has to be determined
         */
        void GetHelpContext( TCoeHelpContext& aContext ) const; 
        
        /**
         * From CoeControl
         */
        TInt CountComponentControls() const;

        /**
         * From CCoeControl
         */
        CCoeControl* ComponentControl(TInt aIndex) const;
        
        /**
         * Inherited from CCoeControl
         */
        TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent,
        						           TEventCode aType );
        		 
		/**
         * Inherited from MEikListBoxObserver
         */
        void HandleListBoxEventL( CEikListBox*      aListBox, 
        					      TListBoxEvent     aEventType );
        					      
	protected:
		/**
		 * Handles Focus Change to list 
		 */
		void FocusChanged(TDrawNow aDrawNow);
		 
		/**
         * From CoeControl,SizeChanged.
         */
        void SizeChanged();

	private:
		/**
		 * Overloaded Constructor
		 */
		CLocSUPLSettingsContainer( CLocSUPLSettingsView&         aSettingsView );
		
		/**
		 * Second phase of the two phase Construction process
		 */
		void ConstructL( const TRect&                        aRect ,
		                       CLocSUPLSettingsUiEngine&     aEngine );
          
        /**
         * Sets the Title text
         * @param aResourceText Resource to create title
         */
        void MakeTitleL( TInt aResourceText );
          
		/**
         * Returns the Appui's status pane
         * @return CEikStatusPane* Statuspane pointer
         */
		CEikStatusPane* StatusPane();

        /** 
         * Creates the List box and the Listbox model. Associates the 
         * list box model wit the list box
         *
         * @param aEngine    Reference to the SUPL Settings engine          
         */
        void CreateListboxL( CLocSUPLSettingsUiEngine&     aEngine );
        
    private:
    
        /**
         * Reference to the Container's view
         */
        CLocSUPLSettingsView&           iSettingsView;
        
        /**
         * Help context
         */
        TCoeContextName 				iContextName;
        
        /**
         * Settings list box for SUPL settings
         * Owns
         */
        CAknSettingStyleListBox*        iListBox;
        
        /**
         * List Box model
         */
        CLocSUPLSettingsLBModel*        iListBoxModel;
        
        /**
         * Boolean flag to denote whether the pen down occured
         */
        TBool							iTouchFlag;            
	};

#endif // C_LOCSUPLSETTINGSCONTAINER_H_

