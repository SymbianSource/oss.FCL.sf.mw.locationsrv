/*
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  E-Comm Plugin interface. Defines a settings UI component. The 
*				 Settings UI component should contain one or more Window owning 
*				 control that provides the Setting funtionality.
*
*/


#ifndef C_LOCSETTINGSUI_H_
#define C_LOCSETTINGSUI_H_

// System Include
#include <e32base.h>

// Forward Declarations
class CLocSettingsUiParams;
class MLocSettingsUiObserver;
class CAknViewAppUi;

// Global Constants
/**
 * The Interface UID for the ECom interface. The implementations of this ECom
 * plug-in have to the use this value as their Interface UID if they intend 
 * to link to the Location System UI Engine
 */
const TUid	KSettingsUiInterfaceUID			=  { 0x10275061 };

// Class Definition
/**
 * Settings UI ECom plug-in interface
 *
 * Interface class that provides the Settings UI funtionality. Any Settings UI
 * component should derive from this class and implement the abstract funtions
 * Includes support for launching a UI, premature closing of a running UI and 
 * re-launching a suspended UI application. The plug-in does not mandate the 
 * nature of the Settings Ui. Unless the Settings UI transfers the ownership
 * of its component objects ( For eg. It might decide to implement a view and 
 * transfer its ownership to the AppUi class ), it is responsible for completely
 * handling them
 *
 * @lib locsysuiview.lib
 * @since S60 v3.1
 */
class CLocSettingsUi : public CBase
	{		
	public:
        /**
         * Creates and initializes the Settings UI component. The component 
         * creation is handled through the E-Comm server
         *
         * @param aImplementationUid  Implementation UID of the Settings Ui that
         *							  has to be launched
         * @param aInitParams		  Initialization Parameters
         */
		inline static CLocSettingsUi* NewL( const TUid				aImplementationUid,
										    CLocSettingsUiParams&	aInitParams );
        /**
         * Destructor. Handles the destruction of the object through the 
         * EComm Session
         */
		inline virtual ~CLocSettingsUi();

        /**
         * Returns the Settings UI Id. A unique identifer that identifies the 
         * Settings UI. Could be the E-Comm destructor Id
         *
         * @since S60 v3.1
         * @return Settings UI Identifier
         */
        inline TUid GetSettingsUiID();

        /**
         * Launches the Settings UI
         *
         * @since S60 v3.1
         * @param aUiLaunchParams	Flags that define specific configuration of
         *                          if the Settings UI, that the user of the API
         *                          wants to launch. If more than one functionality
         *                          is requested then the various combinations
         *                          are to be ORed. The interpretation of these
         *                          flag values is a part of the understanding
         *                          between the API user and the Settings UI
         */
        virtual void LaunchL( TInt		aUiLaunchParams )			   		= 0;

        /**
         * Launches the Settings UI
         *
         * @since S60 v9.2
         * @param aUiInputParams String passed as a parameter to the
         *                       Location UI, that the user of the API wants to
         *                       launch. The interpretation of this string is
         *                       a part of the understanding between the API user
         *                       and the corresponding Location UI.
         */
        virtual void LaunchL( const TDesC&		aUiLaunchParams );

        /**
         * Closes the running Settings UI prematurely. The UI can be closed 
         * permanently or could be temporarily suspended and relaunched. Incase of 
         * temporary suspension it is left to the implementation to decide to
         * either store its current state or discard the same. The Settings UI
         * is expected to release all the resources that it owns on the Close
         * call.
         *
         * @since S60 v3.1
         */
        virtual void Close()							   = 0;

        /**
         * Method for getting caption of this plugin. This should be the 
         * localized name of the settings view to be shown in parent view.
         *
         * @since S60 v3.1
         * @param aCaption Pointer to Caption variable. The memory for the 
         *                 variable has to be allocated by the calling 
         *                 function. If it is found that there is insufficient
         *                 memory then the function can leave accordingly
         */
        virtual void GetCaptionL( TDes& aCaption ) const					   = 0;

        /**
         * Returns the priority of the UI module. The priority value is defined 
         * by the position in which the UI would be present when displayed to 
         * the user. 
         * 
         * @since S60 v3.1
         * @return Priority of the Settings UI
         */
        virtual TInt GetPriority()											   = 0;
        
	protected:
        /**
         * Default Constructor
         */
        inline CLocSettingsUi( MLocSettingsUiObserver&		aSettingsUiObserver,
        		 		       CAknViewAppUi&				aAppUi );
        		 		       
	protected:
        /**
         * Observer for the Settings UI Class
         */	    
		MLocSettingsUiObserver&		iSettingsUiObserver;
		
		/**
         * Reference to the Application Ui to which the Settings UI would belong to
         */
		CAknViewAppUi&				iAppUi;
			
	private:
        /**
         * ECOM plugin instance UID.
         */
		TUid						iDtor_ID_Key;
	};

#include "locsettingsui.inl"

#endif // C_LOCSETTINGSUI_H_

