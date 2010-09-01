/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Location System Ui Plugin View's container
*
*/


#ifndef C_LOCSYSUICONTAINER_H_
#define C_LOCSYSUICONTAINER_H_

// System Includes 
#include <e32base.h>
#include <coecntrl.h>
#include <eiklbo.h>


// Forward Declarations
class CAknSettingStyleListBox;
class CLocSysUiView;
class CLocSysUiLBModel;
class CEikStatusPane;
class MLocSysUiContainerEventHandler;
class CLocationSettings;

// Class Declarations
/**
 * Container for the LocationSystemUiView. Contains the Listbox control that is used to 
 * display the various sub-views that are available. The List box takes a model
 * from the view which it inturn uses as a list box model
 *
 * @lib locsysuiview.lib
 * @since S60 v3.1
 */
 NONSHARABLE_CLASS( CLocSysUiContainer ): public CCoeControl, 
						                  public MEikListBoxObserver
	{
    private:
        
        	
	public:
        /**
         * Destructor
         */
        virtual ~CLocSysUiContainer();

		/**
         * Static Two phase contructor that instantiates the 
         * CLocationSystemUiUIContainer
         * 
         * @param aEventHandler		 Reference to the Container's event handler
         * @param aSettingsArray	 Reference to the Settings UI array.
         * @param aRect				 Client rectangle.
         * @return CLocSysUiContainer*	Reference to the object created.
         */
        static CLocSysUiContainer* NewL( 
        		MLocSysUiContainerEventHandler&	    aEventHandler,
        		RPointerArray<CLocationSettings>&   aSettingsArray,
        		const TRect& 					    aRect );

        /**
         * Static Two phase contructor that instantiates the 
         * CLocationSystemUiUIContainer. Leaves the object on the Cleanup stack
         * 
         * @param aEventHandler		 Reference to the Container's event handler
         * @param aSettingsArray	 Reference to the Settings UI array.
         * @param aRect				 Client rectangle.
         * @return CLocSysUiContainer*	Reference to the object created.
         */
        static CLocSysUiContainer* NewLC( 
        		MLocSysUiContainerEventHandler&	    aEventHandler,
        		RPointerArray<CLocationSettings>&   aSettingsArray,
        		const TRect& 					    aRect );

        /**
         * Returns the Settings Ui item that is currently focussed.
         *
         * @return	CLocationSettings& Referrence to the Settings UI corresponding
         *							   to the index.
         */
        CLocationSettings&	GetFocussedSettingsUiL();

        /**
         * Calls upon the container to update the list bix contents.
         *
         * @param aSettingsArray Reference to the Settings UI array.
         */
        void UpdateListBoxL(RPointerArray<CLocationSettings>&	aSettingsArray);
        
        /**
         * Returns the currently focussed element index
         *
         * @return TUint    Currently focussed element index    
         */
        TUint CurrentFocussedElement();
        
        /**
         * Sets the current highlighted element
         *
         * @param aCurrentElement    Currently focussed element index    
         */
        void SetFocussedElement( TUint      aCurrentElement );
        
    public:
        /**
         * Inherited from CCoeControl
         */
        TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent,
        						           TEventCode aType );
        							    
        /**
         * Inherited from CCoeControl
         */
        void HandleResourceChange(TInt aType);

        /**
         * From CoeControl
         */
        TInt CountComponentControls() const;

        /**
         * From CCoeControl
         */
        CCoeControl* ComponentControl(TInt aIndex) const;

        /**
         * From CCoeControl
         */
        void GetHelpContext( TCoeHelpContext& aContext ) const;
    
    public:
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
         *
         * @param aEventHandler		Reference to the Container's event handler
         */
        CLocSysUiContainer( MLocSysUiContainerEventHandler&	aEventHandler );
        
        /**
         * Second phase of the two phase Construction process
         *
         * @param aSettingsArray    Reference to the Settings UI array.
         * @param aRect				Client rectangle
         */
        void ConstructL( RPointerArray<CLocationSettings>&  aSettingsArray,
        			     const TRect& 						aRect );

        /** 
         * Creates the List box and the Listbox model. Associates the 
         * list box model wit the list box
         *
         * @param aSettingsArray	 Reference to the Settings UI array.
         */
        void CreateListboxL( RPointerArray<CLocationSettings>&  aSettingsArray );

        /**
         * Returns the Appui's status pane
         * @return CEikStatusPane* Statuspane pointer
         */
        CEikStatusPane* StatusPane();

        /**
         * Sets the Title text
         * @param aResourceText Resource to create title
         */
        void MakeTitleL( TInt aResourceText );

	private:
	    /**
	     * Reference to the Container's event handler
	     */
		MLocSysUiContainerEventHandler&		iEventHandler;
       
        /**
         * Help Context Name
         */ 
        TCoeContextName 					iContextName;
        
		/**
		 * List box Model
		 * Owns
		 */
		CLocSysUiLBModel*			iListBoxModel;
		
		/**
		 * List box component control
		 * Owns
		 */
		CAknSettingStyleListBox*			iListBox;
	};

#endif // C_LOCSYSUICONTAINER_H_

