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
* Description:  Location System UI Server's engine component. This class is 
*                responsible for handling all the Location related UI components.
*
*/


#ifndef C_LOCSYSUIENGINE_H_
#define C_LOCSYSUIENGINE_H_

// System Includes
#include <e32base.h>

// User Include

// Forward Declarations
class MLocationUI;
class CLocSettingsUiEngine;
class CAknViewAppUi;
class CLocPosSettings;

// Class Definition

/**
 * Encapulates the logic necessary to launch various Location UIs. 
 * The Location UI AppServer will use this object for manipulating the Settings
 * components. The class contains funtionalities for discovering 
 * various available Location UIs, launching a UI, cancelling a UI request, 
 * terminating a UI after completion of the request..
 *
 * @lib locsysuiengine.lib
 * @since S60 v3.2
 */
class CLocSysUiEngine : public CBase
	{		   		  
	public:
        /**
         * Two Phase Constructor for creating the Location Settings UI engine. 
         *
         * @param   aAppUi			    Reference to the Application UI class. 
         * @return	CLocSysUiEngine*    Reference to the created object.
         */
        IMPORT_C static CLocSysUiEngine* NewL( 
                                CAknViewAppUi&				aAppUi );

        /**
         * Two Phase Constructor for creating the Location Settings UI engine.
         * Leaves the object on the cleanupstack.
         *
         * @param   aAppUi				Reference to the Application UI class.
         * @return	CLocSysUiEngine*    Reference to the created object.
         */
        IMPORT_C static CLocSysUiEngine* NewLC( 
                                CAknViewAppUi&				aAppUi );
        						    	   
        /**
         * Destructor
         */
        virtual ~CLocSysUiEngine();

        /**
         * Creates the sub-Settings UI and  appends it to the existing list. The ownership
         * of the created object is not returned to the callee function. The object would
         * be destroyed only on the destruction of the plug-in.
         * Incase the object alreacdy exists then the function would return a reference to
         * to the existing object.
         *
         * @param  aImplementationUid The Implementation UID of the sub-settings plug-in that
         *                            has to be created.
         * @return MLocationUI*       Reference to the sub-settings UI that has been created.
         */
        IMPORT_C MLocationUI* CreateLocationSubSettingsUIL( TUid    aImplementationUid );
        
        /**
         * Creates an instance of Postioning Settings UI. The ownership
         * of the created object is not returned to the callee function. The object would
         * be destroyed only on the destruction of the plug-in.
         * Incase the object alreacdy exists then the function would return a reference to
         * to the existing object.
         *
         * @return MLocationUI*       Reference to the Positioning UI that has been created.         
         */
        IMPORT_C MLocationUI* CreatePositioningSettingsUIL(); 
            
	private:
	    /**
         * Constructor
         */
        CLocSysUiEngine( CAknViewAppUi&				aAppUi );
		 
	private:
	    /**
	     * Reference to the applications UI
	     */
	    CAknViewAppUi&				                iAppUi;
	    
	    /**
	     * Reference to the Location Settings UI Engine. This object is used
	     * when there is a request to launch a Location sub-settings UI. The object
	     * is created at the time of the first Launch request.
	     * Owns
	     */
	    CLocSettingsUiEngine*                       iSettingsEngine;
	    
	    /**
	     * Reference to the Positioning Settings UI. This object is used
	     * when there is a request to launch Positioning Settings UI. The object
	     * is created at the time of the first Launch request.
	     */
	    CLocPosSettings*                            iPosSettings;
	};

#endif // C_LOCSYSUIENGINE_H_

