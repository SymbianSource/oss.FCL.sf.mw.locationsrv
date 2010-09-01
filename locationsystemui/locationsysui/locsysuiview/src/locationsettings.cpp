/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Location Settings implementation of Location UIs. This class
*                extends the Location UI interface for Location sub-settings
*                plug-ins.
*
*/


// System Include
#include <aknViewAppUi.h>

// User Include
#include "locationsettings.h"
#include "locsettingsui.h"
#include "locsettingsuiparams.h"

// Constant
const TInt KMaxSettingsUiCaptionSize = 0x100;

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CLocationSettings::CLocationSettings
// Overloaded Constructor
//
// ---------------------------------------------------------------------------
//
CLocationSettings::CLocationSettings( const TUid	     aImplementationId )
	:iImplementationId( aImplementationId )
	{		
	}

// ---------------------------------------------------------------------------
// CLocationSettings::~CLocationSettings
// Destructor
//
// ---------------------------------------------------------------------------
//
CLocationSettings::~CLocationSettings()
	{
	// Delete the caption string	
	delete iCaption;
	
	// Delete the contained Settings UI pointer
	delete iLocationSettings;
	}	
					 
// ---------------------------------------------------------------------------
// CLocationSettings* CLocationSettings::NewL
// Two Phase Constructor.
//
// @param   aImplementationId   Implementation UID of the settings plug-in.
// @param   aAppUi              Application's AppUi reference.
// @return	CLocationSettings*	Pointer to the created object
// ---------------------------------------------------------------------------  
//
CLocationSettings* CLocationSettings::NewL( const TUid	     aImplementationId,
                                            CAknViewAppUi&	aAppUi )
	{
    CLocationSettings* self = CLocationSettings::NewLC( aImplementationId,
                                                        aAppUi );
	CleanupStack::Pop( self );
	return self;
	}
	
// ---------------------------------------------------------------------------
// CLocationSettings* CLocationSettings::NewLC
// Two Phase Constructor.
//
// @param   aImplementationId   Implementation UID of the settings plug-in.
// @param   aAppUi              Application's AppUi reference.
// @return	CLocationSettings*	Pointer to the created object
// ---------------------------------------------------------------------------  
//
CLocationSettings* CLocationSettings::NewLC( const TUid	     aImplementationId,
                                             CAknViewAppUi&	 aAppUi )
	{
    CLocationSettings* self = new(ELeave) CLocationSettings( aImplementationId );
	CleanupStack::PushL( self );
	self->ConstructL( aAppUi );
	return self;
	}

// ---------------------------------------------------------------------------
// void CLocationSettings::ConstructL
// Second Phase of the two phase constructor.
// ---------------------------------------------------------------------------
//									 
void CLocationSettings::ConstructL( CAknViewAppUi&	 aAppUi )
	{
	// Create the Location Settings ECom plug-in
    CLocSettingsUiParams* param = CLocSettingsUiParams::NewLC( *this,
                                                               aAppUi );
                                                               
    iLocationSettings = CLocSettingsUi::NewL( iImplementationId, *param );
    
    // Pop and destroy the Settings parameter
    CleanupStack::PopAndDestroy( param );
                                                                   
	// Allocate the buffer for the Caption string and copy the caption from
	// the plug-in
	iCaption = HBufC::NewMaxL( KMaxSettingsUiCaptionSize );
	TPtr  captionPtr = iCaption->Des();
	iLocationSettings->GetCaptionL( captionPtr );
		
	}
	
// ---------------------------------------------------------------------------
// const TPtrC CLocationSettings::Caption
// Accessor Function for the Setings UI Caption
//
// @return const TPtrC Caption String
// ---------------------------------------------------------------------------
//
TPtrC CLocationSettings::Caption()
	{
	return iCaption->Des();
	}

// ---------------------------------------------------------------------------
// const TInt CLocationSettings::Priority()
// Returns the priority of the UI module. 
//
// @return const TInt Priority of the Settings UI
// ---------------------------------------------------------------------------
//
TInt CLocationSettings::Priority()	
	{
	return iLocationSettings->GetPriority();	
	}

// ---------------------------------------------------------------------------
// const TUid CLocationSettings::ImplementationUid()
// Accessor Function for the Setings UI Implementation UID
//
// @return const TUid Settings UI Plug In's implementation UID
// ---------------------------------------------------------------------------
//
const TUid CLocationSettings::ImplementationUid()
	{
	return iImplementationId;	
	}
		
// ---------------------------------------------------------------------------
// void CLocationSettings::LaunchLocationUIL
// Launches a Location UI component.
//
// @param aUiInputParams Flags that define specific configuration of the
//                       Location UI, that the user of the API wants to
//                       launch. If more than one functionality is
//                       requested then the various combinations are to be
//                       ORed. The interpretation of these flag values is
//                       a part of the understanding between the API user
//                       and the corresponding Location UI.
// @param aObserver      Observer where the termination of the Launch call
//                       needs to be notified.
// ---------------------------------------------------------------------------
//                                       
void CLocationSettings::LaunchLocationUIL( TInt		            aUiInputParams,
                                           MLocationUIObserver* aObserver )
    {
    // Set the Observer for the launch call
    iObserver = aObserver;
    
    // Launch the sub-settings UI
    iLocationSettings->LaunchL( aUiInputParams );
    }
		
// ---------------------------------------------------------------------------
// void CLocationSettings::LaunchLocationUIL
// Launches a Location UI component.
//
// @param aUiInputParams String passed as a parameter to the
//                       Location UI, that the user of the API wants to
//                       launch. The interpretation of this string is
//                       a part of the understanding between the API user
//                       and the corresponding Location UI.
// @param aObserver      Observer where the termination of the Launch call
//                       needs to be notified.
// ---------------------------------------------------------------------------
//                                       
void CLocationSettings::LaunchLocationUIL( const TDesC&		            aUiInputParams,
                                           MLocationUIObserver* aObserver )
    {
    // Set the Observer for the launch call
    iObserver = aObserver;
    
    // Launch the sub-settings UI
    iLocationSettings->LaunchL( aUiInputParams );
    }
    
// ---------------------------------------------------------------------------
// void CLocationSettings::Close
// Closes the running Location UI prematurely.
// ---------------------------------------------------------------------------
//
void CLocationSettings::Close()
    {
    // Closes the Location sub-settings
    iLocationSettings->Close();
    }

// ---------------------------------------------------------------------------	
// void CLocationSettings::SettingClosed
// Inherited from MLocSettingsUiObserver. Notification from the Settings UI
// to indicate the termination of the Settings UI application
//
// @param aErrorCode The termination reason. KErrNone for normal 
//					 terminations. In case of error or pre-mature aborting
//					 System wide Error codes.
// ---------------------------------------------------------------------------
//
void CLocationSettings::SettingClosed( TInt  aErrorCode )
	{
	if( iObserver )
	    {
	    iObserver->LocationUIDismissed( aErrorCode );
	    }
	}
