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
* Description:  Location Triggering Management Plugin View's container
*
*/

#ifndef CLBTMGMTPLUGIN_H_
#define CLBTMGMTPLUGIN_H_

// System Includes
#include <ConeResLoader.h>

// User Includes
#include "locsettingsui.h"

// Classes referenced
class MLocSettingsUiObserver;
class CAknViewAppUi;
class CLbtMgmtPluginView;

// Constant Definition
//Location Triggering Management plug-in RSC file name
_LIT( KLbtMgmtUiResFileName, "lbtmgmtpluginview.rsc" );

// Class Definition
/**
 * Location Triggering Management plug-in.
 */
class CLbtMgmtPlugin : public CLocSettingsUi
    {
    
    public: // Constructors and destructor
        
        /**
         * Symbian OS two-phased constructor
         *
         * @param aParam Generic parameter structure to the Settings UI plug-in
         * @return 
         */
        static CLbtMgmtPlugin* NewL( TAny* aParam );
    
        /**
         * Destructor.
         */
        ~CLbtMgmtPlugin();
 
        /**
         * Inherited from CSettingsUi
         * Launches the Settings UI
         *
         * @param aUiLaunchParams   Flags that define specific configuration of
         *                          if the Settings UI, that the user of the API
         *                          wants to launch. If more than one functionality
         *                          is requested then the various combinations
         *                          are to be ORed. The interpretation of these
         *                          flag values is a part of the understanding
         *                          between the API user and the Settings UI
         */
        void LaunchL( TInt      aUiLaunchParams );

        /**
         * Inherited from CSettingsUi
         * Closes the running Settings UI prematurely.
         */
        void Close();

        /**
         * Inherited from CSettingsUi
         * Method for getting caption of this plugin. This should be the 
         * localized name of the settings view to be shown in parent view.
         *
         * @param aCaption pointer to Caption variable
         */
        void GetCaptionL( TDes& aCaption ) const;

        /**
         * Inherited from CSettingsUi
         * Returns the priority of the UI module.
         * 
         * @return Priority of the Settings UI
         */
        TInt GetPriority();
         
    private:
        /**
         * C++ default constructor.
         */
        CLbtMgmtPlugin( MLocSettingsUiObserver&      aSettingsUiObserver,
                           CAknViewAppUi&               aAppUi );

        /**
         * Second phase constructor.
         */
        void ConstructL();

        /**
         * Opens the nearest localized resourcefile using aResourceLoader. 
         * aResourceLoader should be opened only once before closing it. 
         * Otherwise it will raise a panic. Therefore it is better to have 
         * multiple resource loaders.
         * 
         * @param aResourceFileName Drive and name of resource file in format 
         *                          <path>:<rsc_file_name>
         * @param aResourceLoader Resource loader for the resource. The loader 
         *                        must not be opened allready or it will raise 
         *                        a panic. It is caller's responsibility to 
         *                        close the resource loader after using the 
         *                        resource.
         */
        void OpenLocalizedResourceFileL( const TDesC&               aResourceFileName, 
                                               RConeResourceLoader& aResourceLoader );
                                                       
  
    private:
        /**
         * Resource Loader handle for handling Location Triggering Management settings' 
         * specific resources
         */
        RConeResourceLoader             iResourceLoader;
            
        /**
         * Location Triggering Management view
         * Owns
         */
        CLbtMgmtPluginView*      iView;
        
        /**
         * Location Triggering Management view Id
         */
        TUid                        iViewId;
    };

#endif // CLBTMGMTPLUGIN_H_
