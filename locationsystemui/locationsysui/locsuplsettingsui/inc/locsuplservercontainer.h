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
* Description:  SUPL server UI View's container
*
*/


#ifndef C_LOCSUPLSERVERCONTAINER_H_
#define C_LOCSUPLSERVERCONTAINER_H_

// System Includes 
#include <e32base.h>
#include <coecntrl.h>
#include <eiklbo.h>

    
// Forward Declarations
class CLocSUPLServerLBModel;
class CLocSUPLSettingsUiEngine;
class CAknSingleNumberStyleListBox;
class CAknIconArray;
class CLocSUPLServerView;

// Class Declarations
/**
 * Container for the SUPL server UI View.
 * 
 * Contains the Listbox control that is used to configure various SUPL settings 
 * options
 *
 * @lib locsuplsettings.lib
 * @since S60 v3.1 
 */
class CLocSUPLServerContainer : public CCoeControl,
                                public MEikListBoxObserver
	{
	public:
		/**
 		 * Static Two phase contructor that instantiates the CLocSUPLServerContainer
		 * 
		 * @param aRect				            Client rectangle
         * @param aEngine                       Reference to the SUPL Settings
         *                                      engine 		 
         * @param CLocSUPLServerView            Pointer to the current view
		 * @return CLocSUPLServerContainer*	Reference to the object created
		 */
		static CLocSUPLServerContainer* NewL( 
                    		const TRect&               	aRect,
                          	CLocSUPLSettingsUiEngine&	aEngine,
                          	CLocSUPLServerView* 		aView );

		/**
		 * Static Two phase contructor that instantiates the CLocSUPLServerContainer
		 * Leaves the object on the Cleanup stack
		 * 
		 * @param aRect				            Client rectangle
         * @param aEngine                       Reference to the SUPL Settings
         *                                      engine 		 
         * @param CLocSUPLServerView            Pointer to the current view
		 * @return CLocSUPLServerContainer*	Reference to the object created
		 */
		static CLocSUPLServerContainer* NewLC( 
                    		const TRect&               	aRect,
                          	CLocSUPLSettingsUiEngine&	aEngine,
                          	CLocSUPLServerView* 		aView );

		/**
		 * Destructor
		 */
		~CLocSUPLServerContainer();
    
		/**
		 * Inherited from CCoeControl
		 */
		void HandleResourceChange( TInt aType );

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
        CCoeControl* ComponentControl( TInt aIndex ) const;
        
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
	 
        /**
         * Inherited from CCoeControl
         */
        void HandlePointerEventL(const TPointerEvent& aPointerEvent);
	public: // New functions
		
        /**
         * Refreshes the screen so that the list box reflects the latest contents
         */
         void UpdateScreenL();
         
        /**
         * Return reference to listbox
         * @return CEikListBox reference
         */
        CEikListBox& ListBoxL();
        
        /**
		 * Returns the Selected element. The list box model defines the
		 * enumeration that would determine the current element that has been
		 * selected
		 *
		 * @return TInt Index of the item selected
		 */
		TInt SelectedItemIndex();
		 
        /**
         * Return current selected server id
         */
		TInt64 CurrentSlpId();
        					      
        /**
         * Return current selected/marked server count
         */
		TInt SelectedCount();

        /**
         * Return total no of servers in the view
         */		
		TInt ServerCount();
        					      
        /**
         * Returns the slp ids array
         */		
		RArray<TInt64>& SlpIds();
		
		/**
         * Sets the current selected item
         */	
		void SetSelectedIndex();
		
		/**
         * Shows context specific options menu
         */	
		void ShowContextMenuL();

	protected:
		/**
		 * Handles Focus Change to list 
		 */
		void FocusChanged( TDrawNow aDrawNow );
		 
		/**
         * From CoeControl,SizeChanged.
         */
        void SizeChanged();

	private:
		/**
		 * Overloaded Constructor
		 */
		CLocSUPLServerContainer( CLocSUPLServerView* aView );
		
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
        
        /**
         * Append icon to icon array. CAknIconArray owns icons so it is 
         * easier to it leave safely.
         * @param aIcons icon array
         * @param aIconFileWithPath icon file name with path
         * @param aSkinID skin id of the icon
         * @param aIconGraphicsIndex picture index
         * @param aIconGraphicsMaskIndex mask index
         */
        void AddIconL( CAknIconArray&       aIcons,
                       const TDesC&         aIconFileWithPath,
                       TAknsItemID          aSkinID,
                       TInt                 aIconGraphicsIndex,
                       TInt                 aIconGraphicsMaskIndex );
	
		/**
         * Returns the Applications Icon File name with path
         */
		TFileName* IconFileNameLC() const;
		
   private:
    
        /**
         * Reference to the Current View
         */
        CLocSUPLServerView*					iView;
        
        /**
         * Help context
         */
        TCoeContextName 					iContextName;
        
        /**
         * Settings list box for SUPL settings
         * Owns
         */
        CAknSingleNumberStyleListBox*   	iListBox;
        
        /**
         * List Box model
         */
        CLocSUPLServerLBModel*        		iListBoxModel;
        
        /**
         * Boolean flag to denote whether the pen down occured
         */
        TBool								iTouchFlag;  
        
         /**
         * Selected item index value used to set new focus
         * after delete operation.
         */
       	TInt 								iSelectedIndex;
	};

#endif // C_LOCSUPLSERVERCONTAINER_H_

