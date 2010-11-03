/*
* Copyright (c) 2002,2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Positioning Method Settings UI main view's container class 
*                definition.
*
*/



#ifndef CLOCPSYSETTINGSCONTAINER_H_
#define CLOCPSYSETTINGSCONTAINER_H_

// INCLUDES
#include <coecntrl.h>
#include <eiklbo.h>

// FORWARD DECLARATIONS
class CAknSingleGraphicStyleListBox;
class CAknIconArray;
class CEikColumnListBox;
class CEikListBox;
class CEikStatusPane;

class CLocPsySettingsView;
class MLocPsyKeyProcessor;
class MLocPsySettings;

// CLASS DECLARATION

/**
*  CLocPsySettingsContainer  container control class.
*  
*/
class CLocPsySettingsContainer : public CCoeControl,
                                 public MEikListBoxObserver
    {
    private:
        /**
        * C++ default constructor.
        */        
        CLocPsySettingsContainer( MLocPsyKeyProcessor* aKeyProcessor, 
                                  CLocPsySettingsView* aView );

    public:
        /**
        * Destructor.
        */
        virtual ~CLocPsySettingsContainer();

        /**
        * Two-phased constructor
        *
        * @param 	aMainView 				A pointer to MainView
        * @param 	aAppUi 					A pointer to Location AppUi.
        * @param 	aRect 					A rectangle for drawing.
        * @return 	CLocPsySettingsContainer*		The newly created object.
        */
        static CLocPsySettingsContainer* NewL( 
                				  MLocPsyKeyProcessor* 	aKeyProcessor,
                				  MObjectProvider* 		aParent,
                				  CLocPsySettingsView* 	aView,
                				  MLocPsySettings* 		aSettingsModel,
        					const TRect& 			    aRect );
        /**
        * Inherited From CCoeControl
        */
        TKeyResponse OfferKeyEventL( const TKeyEvent& 	aKeyEvent,
                                     	   TEventCode 	aType );
                                     	   
        /**
        * Inherited from CCoeControl. Handles resource changes
        *
        * @param Row index
        */  	
        void HandleResourceChange( TInt aType );

        /**
        * Inherited from CCoeControl. Handles list box events
        *
        * @param aListBox    Listbox on which the event was generated
        * @param aEventType  Type of event that was generated
        */ 
        void HandleListBoxEventL( CEikListBox*      aListBox, 
        					      TListBoxEvent     aEventType );
        /**
        * Returns the help context
        *
        * @param aContext	The help context that has to be determined
        */
        void GetHelpContext( TCoeHelpContext& aContext ) const;    

        /**
        * Returns selected row in main view
        *
        * @return Row index
        */
        TInt SelectedRow() const;

        /**
        * Sets selected row in main view
        *
        * @param Row index
        */       
        void SetSelectedRow( TInt aIndex ) const;

        /**
        * Handle the mark commands for the Markable ListBox
        *
        * @param Markable Commands EAknCmdMark and EAknCmdUnmark
        */       
        void HandleMarkCommandL( TInt aCommand );

        /**
        * Updates the List box
        */
        void UpdateL();

	protected:
		/**
		* Inherited from CCoeControl
		* Handles Focus Change to list 
		*/
		void FocusChanged( TDrawNow aDrawNow );

	private:
        /**
        * By default Symbian 2nd phase constructor is private.        
        * @param aRect Frame rectangle for container.
        */
        void ConstructL( MLocPsySettings* aSettingsModel, const TRect& aRect );

        /**
        * From CoeControl,SizeChanged.
        */
        void SizeChanged();

        /**
        * From CoeControl,CountComponentControls.
        */
        TInt CountComponentControls() const;

        /**
        * From CCoeControl,ComponentControl.
        */
        CCoeControl* ComponentControl( TInt aIndex ) const;

        /**
        *  Append icon to icon array. CAknIconArray owns icons so it is 
        *  easier to it leave safely.
        *  @param aIcons icon array
        *  @param aIconFileWithPath icon file name with path
        *  @param aSkinID skin id of the icon
        *  @param aIconGraphicsIndex picture index
        *  @param aIconGraphicsMaskIndex mask index
        */
        void AddIconL(       CAknIconArray* aIcons,
                       const TDesC&         aIconFileWithPath,
                             TAknsItemID    aSkinID,
                             TInt           aIconGraphicsIndex,
                             TInt           aIconGraphicsMaskIndex );
                       
        /**
        * Gets location icon file path and name. Ownership is transferred,
        * item is left in the cleanup stack.
        * @return full path and name, ownership is transferred
        */        
        TFileName* LocIconFileNameLC() const;

        /**
        * Gets avkon icon file path and name. Ownership is transferred,
        * item is left in the cleanup stack.
        * @return full path and name, ownership is transferred
        */        
        TFileName* AvkonIconFileNameLC() const;        
                   
        /**
        * Make title
        * @param aResourceText resource to create title
        */
        void MakeTitleL( TInt aResourceText );

        /**
        * Make title
        * @param aListbox listbox reference
        * @param aResourceText resourse id
        */
        void MakeEmptyTextListBoxL( CEikListBox& aListBox, TInt aResourceText );

        /**
        * Appui's status pane getter
        * @return Statuspane pointer
        */
        CEikStatusPane* StatusPane();

        /**
        * Creates List box.
        */
        void CreateListboxL( MLocPsySettings* aSettingsModel );

        /**
        * Update icons
        */
        void UpdateIconsL();

    private: //data
        
        // help context
        TCoeContextName 					iContextName;
        
        // Key Processor to route the keys before it is processed by the control
        // not own
        MLocPsyKeyProcessor* 				iKeyProcessor;
        
        /// Own: List box.
        CAknSingleGraphicStyleListBox* 		iListBox;
        
        // The parent View
        // Not Owned
        CLocPsySettingsView* 				iParentView;
    };

#endif

// End of File CLOCPSYSETTINGSCONTAINER_H_
