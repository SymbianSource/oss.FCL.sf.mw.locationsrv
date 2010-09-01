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
* Description:  Location Triggering Managemen Plugin View's container
*
*/

#ifndef CLBTMGMTPLUGINCONTAINER_H_
#define CLBTMGMTPLUGINCONTAINER_H_

// System Includes 
#include <e32base.h>
#include <coecntrl.h>
#include <eiklbo.h>

// Forward Declarations
class MDesCArray;
class MLbtMgmtPluginCmdHdlr;
class CAknSettingStyleListBox;
class CLbtMgmtPluginView;

// Class Definition

/**
 * Container for the CLbtMgmtPluginView.
 */
class CLbtMgmtPluginContainer : public CCoeControl,
                                    public MEikListBoxObserver
    {
    public:
        /**
         * Static Two phase contructor that instantiates the CLbtMgmtPluginContainer
         * 
         * @param aRect                         Client rectangle
         * @param aListBoxModel                 Model for the container's list box
         * @param aCmdHandler                   List box command handler
         * @return CLbtMgmtPluginContainer*  Reference to the object created
         */
        static CLbtMgmtPluginContainer* NewL( 
                                const TRect&                    aRect,
                                      MDesCArray&               aListBoxModel,
                                      MLbtMgmtPluginCmdHdlr& aCmdHandler,
                                      CLbtMgmtPluginView* aView );

        /**
         * Static Two phase contructor that instantiates the CLbtMgmtPluginContainer
         * Leaves the object on the Cleanup stack
         * 
         * @param aRect                         Client rectangle
         * @param aListBoxModel                 Model for the container's list box
         * @param aCmdHandler                   List box command handler     
         * @return CLbtMgmtPluginContainer*  Reference to the object created
         */
        static CLbtMgmtPluginContainer* NewLC(
                                const TRect&                    aRect,
                                      MDesCArray&               aListBoxModel,
                                      MLbtMgmtPluginCmdHdlr& aCmdHandler,
                                      CLbtMgmtPluginView* aView );
        /**
         * Destructor
         */
        ~CLbtMgmtPluginContainer();
         
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
         * Inherited from CCoeControl
         */                         
        void HandlePointerEventL(const TPointerEvent &aPointerEvent);
 
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
         * Updates listbox model          
         */
       void Update();
       /*
        * Shows context specific menu items
        */
       void ShowContextMenuL();
             
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
        CLbtMgmtPluginContainer( MDesCArray&               aListBoxModel,
                                    MLbtMgmtPluginCmdHdlr& aCmdHandler,
                                    CLbtMgmtPluginView* aView );

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
        MLbtMgmtPluginCmdHdlr&   iCmdHandler;
        
        /**
         * Settings list box for Trigger Management settings
         * Owns
         */
        CAknSettingStyleListBox*        iListBox;    
        
        /**
         * Help context
         */
        TCoeContextName                 iContextName;
        /*
         * reference to the lbtmgmtplugin view
         */
        CLbtMgmtPluginView* iView;
        
  };

#endif // CLBTMGMTPLUGINCONTAINER_H_

