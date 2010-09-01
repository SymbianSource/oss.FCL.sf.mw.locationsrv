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
* Description: This defines the engine class for Location settings Ui
*
*/


// System Include 
#include <aknViewAppUi.h>

// User Include
#include "locsettingsuiengine.h"
#include "locsettingsuiecomnotifier.h"
#include "locationsettings.h"
#include "locsettingsuipluginloader.h"
#include "locsettingsui.h"
#include "locsettingsuiparams.h"

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CLocSettingsUiEngine::CLocSettingsUiEngine
// Overloaded Constructor
//
// ---------------------------------------------------------------------------
//
CLocSettingsUiEngine::CLocSettingsUiEngine( CAknViewAppUi&	aAppUi )
	:iAppUi( aAppUi )
	{		
	}

// ---------------------------------------------------------------------------
// CLocationSystemUiView::~CLocSettingsUiEngine
// Destructor
//
// ---------------------------------------------------------------------------
//
CLocSettingsUiEngine::~CLocSettingsUiEngine()
	{

	// Delete the Settings UI Descriptions
	while ( iSettingsArray.Count() > 0 )
		{
		// Remove the first item
		CLocationSettings* item = iSettingsArray[0];
		iSettingsArray.Remove( 0 );
		
		// Delete the element
		delete item;
		item = NULL;
		}
	iSettingsArray.Reset();
	iSettingsArray.Close();
	
	// Delete the Plug-in Loader
	delete iPluginLoader;	
	}	
					 
// ---------------------------------------------------------------------------
// CLocSettingsUiEngine* CLocSettingsUiEngine::NewL
// Two Phase Constructor. 
//
// @param 	aAppUi				    Reference to the Application UI class that
//									launched the Settings Application
// @return	CLocSettingsUiEngine*	Reference to the created object
// ---------------------------------------------------------------------------  
//
EXPORT_C CLocSettingsUiEngine* CLocSettingsUiEngine::NewL( CAknViewAppUi& aAppUi )
	{
    CLocSettingsUiEngine* self = CLocSettingsUiEngine::NewLC( aAppUi );
	CleanupStack::Pop( self );
	return self;
	}
	
// ---------------------------------------------------------------------------
// CLocSettingsUiEngine* CLocSettingsUiEngine::NewLC
// Two Phase Constructor. 
//
// @param 	aAppUi					Reference to the Application UI class that
//									launched the Settings Application
// @return	CLocSettingsUiEngine*	Reference to the created object
// ---------------------------------------------------------------------------  
//
EXPORT_C CLocSettingsUiEngine* CLocSettingsUiEngine::NewLC( CAknViewAppUi&	aAppUi )
	{
    CLocSettingsUiEngine* self = new(ELeave) CLocSettingsUiEngine( aAppUi );
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}

// ---------------------------------------------------------------------------
// void CLocSettingsUiDesc::ConstructL
// Second Phase of the two phase constructor. The caption string is copied into
// 
// ---------------------------------------------------------------------------
//									 
void CLocSettingsUiEngine::ConstructL()
	{
	
	// Construct the Plug-In Loader
	iPluginLoader = CLocSettingsUiPluginLoader::NewL( iAppUi );

	}

// ---------------------------------------------------------------------------
// CLocationSettings* CLocSettingsUiEngine::CreateSettingsUiL
// 
// Creates a single Settings UI Object.
// In case the object already exists, then a reference to the existing object is
// returned and no new object is created. In either case, the class retains the
// ownership of the created object.
//
// @param aImplementationUid The Implementation UID of the plug-in that
//                           has to be created.
// @return CLocationSettings* Reference to the created object.
// ---------------------------------------------------------------------------
//
EXPORT_C CLocationSettings* CLocSettingsUiEngine::CreateSettingsUiL( 
                                                        TUid aImplementationUid )
    {
    // Scan through the existing list of Settings UIs. Incase, a UI with the
    // same UID exists then return the same. If not create a new implmentation
    // and return a reference to it
    
    TInt index = KErrNotFound;
    
    // Iterate through the Settings UI array
	for ( TInt count = 0; count < iSettingsArray.Count(); count++ )
	{
	CLocationSettings* currentItem = iSettingsArray[count];
	// If the Settings object for the Implementation UID already
	// exisits then dont do anything
	if ( aImplementationUid == currentItem->ImplementationUid())
		{
		index = count;
		break;
		}
	}
    
    if( KErrNotFound != index )
        {
        // A Settings UI corresponding to the UID already exists. Return a 
        // reference to the same.
        return iSettingsArray[index];
        }
    else
        {
        // Settings UI does not exist. Create a new implementation and 
        // return the same
        return  &( iPluginLoader->CreateSettingsUiL( aImplementationUid,
                                                     iSettingsArray ));                                                   
        
        }
    }
        
// ---------------------------------------------------------------------------
// RPointerArray<CLocationSettings>& CLocSettingsUiEngine::CreateAvailableSettingsUiL
//
// Creates all the available settings UI. This function call performs
// an ECom discovery of all the possible ECom plugins that are 
// available and creates the Settings UI components as well as the 
// plug-in UI. The ownership of the created objects is not transferred
// to the callee but retained with the Engine class. This function needs 
// that No settings UIs have been already created. Incase, there exists
// any Settings UIs then the function leaves with KErrAlreadyExists.
//
// @return RPointerArray<CLocationSettings>& Reference to the array of Settings
//                                           objects.
// ---------------------------------------------------------------------------
//
RPointerArray<CLocationSettings>& CLocSettingsUiEngine::CreateAvailableSettingsUiL()
    {
	
	// The function expects that there are no existing Settings UI elements 
	// present. Hence, the need to check
	if( iSettingsArray.Count())
	    {
	    User::Leave( KErrAlreadyExists );
	    }
	    
	// Create the Settings UIs 
	iPluginLoader->CreateAvailableSettingsUisL( iSettingsArray );
	
	// Return a reference to the created Settings UI structures
	return iSettingsArray;	
    }
        
// ---------------------------------------------------------------------------
// PointerArray<CLocationSettings>& CLocSettingsUiEngine::UpdateAvailableSettingsUisL()
//
// Updates the list of available UIs. The new list would contain all the
// latest additions to the list of UIs in addition to the existing
// UIs. The interface does not delete any instances if they are not in
// the current implementation list.
//
// @return RPointerArray<CLocationSettings>& Reference to the array of Settings
//                                           objects.   
// ---------------------------------------------------------------------------
//
RPointerArray<CLocationSettings>& CLocSettingsUiEngine::UpdateAvailableSettingsUisL()
	{
	
	// Create the Settings UIs 
	iPluginLoader->CreateAvailableSettingsUisL( iSettingsArray );
	
	// Return a reference to the created Settings UI structures
	return iSettingsArray;
	}
	
// ---------------------------------------------------------------------------
// RPointerArray<CLocationSettings>& CLocSettingsUiEngine::SettingsUiArray
// 
// Accessor function to obtain the Settings UI structures. The ownership of the 
// internal objects is not transferred.
// 
//  @return RPointerArray<CLocationSettings>& Reference to the array of Settings
//                                            objects. 
// ---------------------------------------------------------------------------
//
RPointerArray<CLocationSettings>& CLocSettingsUiEngine::SettingsUiArray()
    {
    return iSettingsArray;
    }

// ---------------------------------------------------------------------------
// void CLocSettingsUiEngine::StartSettingsEComNotification
// Starts the Ecom notification for observing loading and unloading of the 
// underlying sub-settings plug-ins
//
// @param aSettingsNotifer  Notifier for communication of changes in the 
//                          Ecom loading and unloading for sub-settings
//                          plug-ins
// ---------------------------------------------------------------------------
//
void CLocSettingsUiEngine::StartSettingsEComNotificationL( 
                            MLocSettingsUiEcomNotifer*   aNotifier )
    {
    iPluginLoader->StartEComNotifyL( aNotifier );
    }
                    
// ---------------------------------------------------------------------------
// void CLocSettingsUiEngine::StopSettingsEComNotification
// Stops ECom notification. Does nothing in case there was no outstanding
// request
// ---------------------------------------------------------------------------
//
void CLocSettingsUiEngine::StopSettingsEComNotification()
    {
    iPluginLoader->StopEComNotify();
    }
