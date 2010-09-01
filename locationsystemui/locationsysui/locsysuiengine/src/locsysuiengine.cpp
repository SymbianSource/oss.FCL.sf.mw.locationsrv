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
* Description:  Location System UI Server's engine component. This class serves
*                as a factory implementation for Location UI components
*
*/


// System Include
#include <aknViewAppUi.h>

// User Include
#include "locsysuiengine.h"
#include "locsettingsuiengine.h"
#include "locationsettings.h"
#include "locpossettings.h"

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CLocSysUiEngine::CLocSysUiEngine
// Overloaded Constructor
//
// ---------------------------------------------------------------------------
//
CLocSysUiEngine::CLocSysUiEngine( CAknViewAppUi&    aAppUi )
	:iAppUi( aAppUi )
	{	
	}

// ---------------------------------------------------------------------------
// CLocationSystemUiView::~CLocSysUiEngine
// Destructor
//
// ---------------------------------------------------------------------------
//
CLocSysUiEngine::~CLocSysUiEngine()
	{
	// Incase a Location sub-settings request was made then the Settings
	// UI engine has been created, In that case, the object and the 
	// associated resources would be freed.
	delete iSettingsEngine;
	
	// Incase Positioning settings request was made then the Settings UI
	// and the associated resources would be freed.
	delete iPosSettings;
	}	
					 
// ---------------------------------------------------------------------------
// CLocSysUiEngine* CLocSysUiEngine::NewL
// Two Phase Constructor for creating the Location Settings UI engine.  
//
// @param    aAppUi			    Reference to the Application UI class.
// @return	CLocSysUiEngine*    Reference to the created object.
// ---------------------------------------------------------------------------  
//
EXPORT_C CLocSysUiEngine* CLocSysUiEngine::NewL( CAknViewAppUi&	 aAppUi )
	{
    CLocSysUiEngine* self = CLocSysUiEngine::NewLC( aAppUi );
	CleanupStack::Pop( self );
	return self;
	}
	
// ---------------------------------------------------------------------------
// CLocSysUiEngine* CLocSysUiEngine::NewL
// Two Phase Constructor for creating the Location Settings UI engine.
// Leaves the object on the cleanupstack.
//
// @param    aAppUi			    Reference to the Application UI class.
// @return	CLocSysUiEngine*    Reference to the created object.
// ---------------------------------------------------------------------------  
//
EXPORT_C CLocSysUiEngine* CLocSysUiEngine::NewLC( CAknViewAppUi&	aAppUi )
	{
    CLocSysUiEngine* self = new(ELeave) CLocSysUiEngine( aAppUi );
	CleanupStack::PushL( self );
    // No second phase construction required
	return self;
	}

// ---------------------------------------------------------------------------
// MLocationUI* CLocSysUiEngine::CreateSettingsUi
// Creates the sub-Settings UI and  appends it to the existing list. The ownership
// of the created object is not returned to the callee function. The object would
// be destroyed only on the destruction of the plug-in.
// Incase the object alreacdy exists then the function would return a reference to
// to the existing object.
//
// @param  aImplementationUid The Implementation UID of the sub-settings plug-in that
//                            has to be created.
// @return MLocationUI*       Reference to the sub-settings UI that has been created.
// ---------------------------------------------------------------------------
//
EXPORT_C MLocationUI* CLocSysUiEngine::CreateLocationSubSettingsUIL( 
                                                TUid    aImplementationUid )
    {
    // If the Settings engine has already been created then use the same
    // instance. If not, create a new instance
    if( !iSettingsEngine )
        {
        iSettingsEngine = CLocSettingsUiEngine::NewL( iAppUi );
        }   
    return iSettingsEngine->CreateSettingsUiL( aImplementationUid );        
    }
    
// ---------------------------------------------------------------------------
// MLocationUI* CLocSysUiEngine::CreatePositioningSettingsUIL
// Creates an instance of Postioning Settings UI. The ownership
// of the created object is not returned to the callee function. The object would
// be destroyed only on the destruction of the plug-in.
// Incase the object alreacdy exists then the function would return a reference to
// to the existing object.
//
// @return MLocationUI*       Reference to the Positioning UI that has been created.         
// ---------------------------------------------------------------------------
//
EXPORT_C MLocationUI* CLocSysUiEngine::CreatePositioningSettingsUIL()
    {
    // If the Postioning Settings UI instance already exists then use the same
    // If not, create a new instance
    if( !iPosSettings )
        {
        iPosSettings = CLocPosSettings::NewL();
        }
    return iPosSettings;
    }


  
