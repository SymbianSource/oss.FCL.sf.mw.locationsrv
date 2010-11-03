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
* Description:  SUPL session UI View's container
*
*/


#ifndef C_LOCSUPLSESSIONCONTAINER_H
#define C_LOCSUPLSESSIONCONTAINER_H

// System Includes 
#include <e32base.h>
#include <coecntrl.h>
#include <eiklbo.h>

    
// Forward Declarations
class CLocSUPLSessionLBModel;
class CLocSUPLSettingsUiEngine;
class CAknSingleGraphicStyleListBox;
class CAknIconArray;
class CLocSUPLSessionView;

// Class Declarations
/**
 * Container for the SUPL session UI View.
 * 
 * Contains the Listbox control that is used to display the Active SUPL sessions
 *
 * @lib locsuplsettings.lib
 * @since S60 v3.1 
 */
class CLocSUPLSessionContainer : public CCoeControl,
                                public MEikListBoxObserver
	{
	public:
		/**
 		 * Static Two phase constructor that instantiates the CLocSUPLSessionContainer
		 * 
		 * @param aRect				            Client rectangle
         * @param aEngine                       Reference to the SUPL Settings
         *                                      engine 		 
         * @param CLocSUPLSessionView            Pointer to the current view
		 * @return CLocSUPLSessionContainer*	Reference to the object created
		 */
		static CLocSUPLSessionContainer* NewL( 
                    		const TRect&               	aRect,
                          	CLocSUPLSettingsUiEngine&	aEngine,
                          	CLocSUPLSessionView* 		aView );

		/**
		 * Static Two phase constructor that instantiates the CLocSUPLSessionContainer
		 * Leaves the object on the Cleanup stack
		 * 
		 * @param aRect				            Client rectangle
         * @param aEngine                       Reference to the SUPL Settings
         *                                      engine 		 
         * @param CLocSUPLSessionView            Pointer to the current view
		 * @return CLocSUPLSessionContainer*	Reference to the object created
		 */
		static CLocSUPLSessionContainer* NewLC( 
                    		const TRect&               	aRect,
                          	CLocSUPLSettingsUiEngine&	aEngine,
                          	CLocSUPLSessionView* 		aView );

		/**
		 * Destructor
		 */
		~CLocSUPLSessionContainer();
    
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
		TInt64 CurrentSessionId();
        					      
        /**
         * Return current selected/marked server count
         */
		TInt SelectedCount();

        /**
         * Return total no of sessions in the view
         */		
		TInt SessionCount();
        					      
        /**
         * Returns the session ids array
         */		
		RArray<TInt64>& SessionIds();
		
		 /**
      * Sets the current selected item
      */	
		void SetSelectedIndex();
		
		 /**
      * Shows context specific options menu
      */	
		void ShowContextMenuL();

     /**
      * Return the session name identified by session id
      */		
		TPtrC CurrentSessionName();
		
		/**
		 * Sets focus to the specific item in Active services view
		 * @param aIndex	index of item to be focussed
		 */
    void FocusToSessionDetailL( TInt aIndex );

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
		CLocSUPLSessionContainer( CLocSUPLSessionView* aView );
		
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
        CLocSUPLSessionView*					iView;
        
        /**
         * Help context
         */
        TCoeContextName 					iContextName;
        
        /**
         * Single Graphic Listbox to display the active sessions
         * Owns
         */
        CAknSingleGraphicStyleListBox*   	iListBox;
      //  CAknSingleNumberStyleListBox*       iListBox;
        /**
         * List Box model
         */
        CLocSUPLSessionLBModel*        		iListBoxModel;
        
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

#endif // C_LOCSUPLSESSIONCONTAINER_H

