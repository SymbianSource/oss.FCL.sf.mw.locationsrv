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
* Description:  Location Notation Preferences Plugin View's container
*
*/

#ifndef C_LOCNOTPREFPLUGINCONTAINER_H_
#define C_LOCNOTPREFPLUGINCONTAINER_H_

// System Includes 
#include <e32base.h>
#include <coecntrl.h>
#include <eiklbo.h>

// Forward Declarations
class MDesCArray;
class MLocNotPrefPluginCmdHdlr;
class CAknSettingStyleListBox;

// Class Definition

/**
 * Container for the CLocNotPrefPluginView.
 */
class CLocNotPrefPluginContainer : public CCoeControl,
                                   public MEikListBoxObserver
    {
    public:
        /**
         * Static Two phase contructor that instantiates the CLocNotPrefPluginContainer
         * 
         * @param aRect                         Client rectangle
         * @param aListBoxModel                 Model for the container's list box
         * @param aCmdHandler                   List box command handler
         * @return CLocNotPrefPluginContainer*  Reference to the object created
         */
        static CLocNotPrefPluginContainer* NewL( 
                                const TRect&                    aRect,
                                      MDesCArray&               aListBoxModel,
                                      MLocNotPrefPluginCmdHdlr& aCmdHandler );

        /**
         * Static Two phase contructor that instantiates the CLocNotPrefPluginContainer
         * Leaves the object on the Cleanup stack
         * 
         * @param aRect                         Client rectangle
         * @param aListBoxModel                 Model for the container's list box
         * @param aCmdHandler                   List box command handler     
         * @return CLocNotPrefPluginContainer*  Reference to the object created
         */
        static CLocNotPrefPluginContainer* NewLC(
                                const TRect&                    aRect,
                                      MDesCArray&               aListBoxModel,
                                      MLocNotPrefPluginCmdHdlr& aCmdHandler );
        /**
         * Destructor
         */
        ~CLocNotPrefPluginContainer();
         
        /**
         * Returns the currentlt focussed element.
         *
         * @return TInt Index of the item selected
         */
        TInt GetFocussedItem();
        
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
         * Returns the help context
         *
         * @param aContext  The help context that has to be determined
         */
        void GetHelpContext( TCoeHelpContext& aContext ) const;
        
        /**
         * Handles the pointer events
         */
        void HandlePointerEventL( const TPointerEvent &aPointerEvent );
             
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
        CLocNotPrefPluginContainer( MDesCArray&               aListBoxModel,
                                    MLocNotPrefPluginCmdHdlr& aCmdHandler );

        /**
         * Second phase of the two phase Construction process
         */
        void ConstructL( const TRect& aRect );
           
        /** 
         * Creates the List box and the Listbox model. Associates the 
         * list box model wit the list box
         *         
         */
        void CreateListboxL();
                      
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
     
    private:
        /**
         * Reference to the List box model
         */
        MDesCArray&                 iListBoxModel;
          
        /**
         * Reference to the List box command handler
         */
        MLocNotPrefPluginCmdHdlr&   iCmdHandler;
        
        /**
         * Settings list box for SUPL settings
         * Owns
         */
        CAknSettingStyleListBox*        iListBox;
        
        /**
         * Help context
         */
        TCoeContextName                 iContextName;
        
  };

#endif // C_LOCNOTPREFPLUGINCONTAINER_H_

