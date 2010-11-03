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
* Description:  Manages the E-Comm Plugin handling for the Settings UI.
*
*/


// System Include
#include <ecom/implementationinformation.h>
#include <aknViewAppUi.h>
	
// User Include
#include "locsettingsuipluginloader.h"
#include "locationsettings.h"
#include "locsettingsui.h"
#include "locsettingsuiparams.h"
#include "locsettingsuiecomnotifier.h"


// Global Constants

//Forward declaration. Function for sorted ordering of settings ui plug-ins
static TInt SettingsDescOrdering( const CLocationSettings& aSettings1, 
		  					   	  const CLocationSettings& aSettings2 );
		  					   	  
// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CLocSettingsUiPluginLoader::CLocSettingsUiPluginLoader
// Overloaded Constructor
//
// ---------------------------------------------------------------------------
//
CLocSettingsUiPluginLoader::CLocSettingsUiPluginLoader( CAknViewAppUi&	aAppUi )
	:CActive( EPriorityStandard ),
	iAppUi( aAppUi )
	{
	// Add self to the Active scheduler
	CActiveScheduler::Add( this );			
	}

// ---------------------------------------------------------------------------
// CLocSettingsUiPluginLoader::~CLocSettingsUiPluginLoader
// Destructor
//
// ---------------------------------------------------------------------------
//
CLocSettingsUiPluginLoader::~CLocSettingsUiPluginLoader()
	{
	Cancel();
	}	
					 
// ---------------------------------------------------------------------------
// CLocSettingsUiPluginLoader* CLocSettingsUiPluginLoader::NewL
// Creates an instance of the Settings UI Plug-in Loader
//
// @param  aAppUi				    	App Ui Reference
// @return CLocSettingsUiPluginLoader*	Reference to the instantiated class 
// ---------------------------------------------------------------------------  
//
CLocSettingsUiPluginLoader* CLocSettingsUiPluginLoader::NewL( 
                                                CAknViewAppUi&	aAppUi )
	{
    CLocSettingsUiPluginLoader* self = 
                CLocSettingsUiPluginLoader::NewLC( aAppUi );
	CleanupStack::Pop(self);
	return self;
	}
	
// ---------------------------------------------------------------------------
// CLocSettingsUiPluginLoader* CLocSettingsUiPluginLoader::NewLC
// Creates an instance of the Settings UI Plug-in Loader
//
// @param  aObserver				Observer to the CLocSettingsUiPluginLoader class
// @param  aAppUi					App Ui Reference
// @return CLocSettingsUiPluginLoader*	Reference to the instantiated class 
// ---------------------------------------------------------------------------  
//
CLocSettingsUiPluginLoader* CLocSettingsUiPluginLoader::NewLC( 
								                CAknViewAppUi&	aAppUi )
	{
    CLocSettingsUiPluginLoader* self = new(ELeave) CLocSettingsUiPluginLoader( aAppUi );
	CleanupStack::PushL(self);
	return self;
	}

// ---------------------------------------------------------------------------
// void CLocSettingsUiPluginLoader::CreateAvailableSettingsUisL  
// Creates all the settings UI currently available in the system. The 
// user if this interface should ensure that he passes an empty
// Settings Array structure.
// 
// @param aDescArray 		 Array of Settings UI.
// --------------------------------------------------------------------------- 
//
void CLocSettingsUiPluginLoader::CreateAvailableSettingsUisL( 
                                RPointerArray<CLocationSettings>&   aSettingsArray )
	{
	// List all the available implementations for KSettingsUiInterfaceUID
	RImplInfoPtrArray	impInfoArray;
	
	REComSession::ListImplementationsL( KSettingsUiInterfaceUID,
										impInfoArray );
	for ( TInt impInfoArrayIterator = 0; 
		 	   impInfoArrayIterator < impInfoArray.Count();
		 	   impInfoArrayIterator ++ )
		{
		// Obtain the Implementation UID
		CImplementationInformation* info = impInfoArray[impInfoArrayIterator];
        TUid implUid = info->ImplementationUid();
        
        // Create the Location Settings UI class which inturn creates the
        // sub-settings plug-in
             			
        CLocationSettings* settings = NULL;
        TRAPD( error, settings = CLocationSettings::NewL( implUid, iAppUi ););
        if( !error )
            {
            // Insert them in order. The order is specified by the SettingsDescOrdering
    		// function			
    		error = aSettingsArray.InsertInOrder( settings,
    											  SettingsDescOrdering );
    		if ( error )
    			{
    			// Issue with creating the Settings UI component. Delete it
    			delete settings;   			
    			}
            }
		}
    // Destroy the implementation info array obtained from ECom Session
	impInfoArray.ResetAndDestroy();	
	}

// ---------------------------------------------------------------------------
// void CLocSettingsUiPluginLoader::CreateSettingsUiL
// Creates the Settings UI and the corresponding description entry. This
// Location Settings object. The created object is appended to the existing
// list and a reference to it returned.
// 
// @param aImplementationUid The Implementation UID of the plug-in that
//                           has to be created
// @param aSettingsArray	 Array of Settings UIs
// @return CLocationSettings& Reference to the created object. The ownership is not
//                            by the return value.
// ---------------------------------------------------------------------------
//
CLocationSettings& CLocSettingsUiPluginLoader::CreateSettingsUiL( 
                    TUid                                 aImplementationUid,
                    RPointerArray<CLocationSettings>&    aSettingsArray )
    {
    // Create the plug in
    
	// The Function does not check for the existence of the plug-in
	// in the implementation array.
	
    // The function is not trapped. If there is a leave then the function would
    // leave with the error code generated. The calling function has to take 
    // care of the non-existence of the plug-in
	
	CLocationSettings* settings = CLocationSettings::NewLC( aImplementationUid,
	                                                        iAppUi );
    			                                                    
	// Insert them in order. The order is specified by the SettingsDescOrdering
	// function			
	User::LeaveIfError(  aSettingsArray.InsertInOrder( settings,
											           SettingsDescOrdering ));
	CleanupStack::Pop( settings );
		
    // Return the reference to the Settings UI
    return *settings;			
    }
						        
// ---------------------------------------------------------------------------
// void CLocSettingsUiPluginLoader::UpdateAvailableSettingsUisL
// Updates the Settings UI list and the description structures. The new
// additions are appened in-order. The interface does not
// delete any instances if they are not in the current implementation 
// list
// 
// @param aSettingsArray     Array of Settings UIs.
// @param aInitParams		 Opaque Initialization parameters that are 
// 							 passed onto the Settings UI. The PluginLoader 
// 							 does not understand these parameters
// ---------------------------------------------------------------------------
//
void CLocSettingsUiPluginLoader::UpdateAvailableSettingsUisL( 
						RPointerArray<CLocationSettings>&    aSettingsArray )
	{
		// List all the available implementations for KSettingsUiInterfaceUID
	RImplInfoPtrArray	impInfoArray;
	
	REComSession::ListImplementationsL( KSettingsUiInterfaceUID,
										impInfoArray );

	for ( TInt impInfoArrayIterator = 0; 
		 	   impInfoArrayIterator < impInfoArray.Count();
		 	   impInfoArrayIterator ++ )
		{
		// Obtain the Implementation UID
		CImplementationInformation* info = impInfoArray[impInfoArrayIterator];
        TUid implUid = info->ImplementationUid();
        
        // Search if the plug-in already exists.
        TInt index = KErrNotFound;
        // Iterate through the Settings UI array
		for ( TInt count = 0; count < aSettingsArray.Count(); count++ )
		{
		CLocationSettings* currentItem = aSettingsArray[count];
		// If the Settings object for the Implementation UID already
		// exisits then dont do anything
		if ( implUid == currentItem->ImplementationUid())
			{
			index = count;
			break;
			}
		}
        
        // If the index value is not set Create the plug in
        if ( KErrNotFound == index )
        	{
            // Create the Location Settings UI class which inturn creates the
            // sub-settings plug-in
             			
            CLocationSettings* settings = NULL;
            TRAPD( error, settings = CLocationSettings::NewL( implUid, iAppUi ););
            if( !error )
                {
                // Insert them in order. The order is specified by the SettingsDescOrdering
    		    // function			
    	        error = aSettingsArray.InsertInOrder( settings,
    												  SettingsDescOrdering );
    		    if ( error )
    			    {
    			    // Issue with creating the Settings UI component. Delete it
    			    delete settings;   			
    			    }
                }	
        	}
		}
	
	impInfoArray.ResetAndDestroy();	
	}

// --------------------------------------------------------------------------- 
// Inherited from CActive 
// ---------------------------------------------------------------------------
//
void CLocSettingsUiPluginLoader::RunL()
	{
	if ( iStatus.Int() != KErrCancel )
		{
		// Restart the Notification request inorder to listen for future 
		// notifications
		StartEComNotifyL();
		// Notify the state change to the observer
		if( iNotifier )
		    {
		    iNotifier->EComSessionStateChanged( iStatus.Int());
		    }		
		}
	}

// --------------------------------------------------------------------------- 
// Inherited from CActive 
// ---------------------------------------------------------------------------
//
void CLocSettingsUiPluginLoader::DoCancel()
	{
	if ( iSession )
		{
		iSession->CancelNotifyOnChange( iStatus );
		iSession->Close();
		iSession = NULL;
		}		
	}

// --------------------------------------------------------------------------- 
// Inherited from CActive 
// ---------------------------------------------------------------------------
//
TInt CLocSettingsUiPluginLoader::RunError( TInt /* aError */)
	{
	return KErrNone;
	}

// --------------------------------------------------------------------------- 
// Initiates a notify request on the ECom Session
//
// @param aSettingsNotifer  Notifier for communication of changes in the 
//                          Ecom loading and unloading for sub-settings
//                          plug-ins 
// ---------------------------------------------------------------------------
//
void CLocSettingsUiPluginLoader::StartEComNotifyL( 
                    MLocSettingsUiEcomNotifer*   aNotifier )
	{
	// Set the notifier if its valid
	if( aNotifier )
	    {
	    iNotifier = aNotifier;
	    }
	
	// If the notifier is present then initaite a request    
    // Open the ECom Session handle if its not been opened
    if( !iSession )
        {
        iSession = &( REComSession::OpenL());
        }
	
	// Issue a new request	
	if ( !IsActive())
		{
		// Set the notifier 
		iSession->NotifyOnChange( iStatus );	
		
		SetActive();
		}
	}
        
// ---------------------------------------------------------------------------
//
// Stop the outstanding EComNotification
// ---------------------------------------------------------------------------
//
void CLocSettingsUiPluginLoader::StopEComNotify()
    {
    // Cancel any outstanding request
    Cancel();
    }
        	
// ---------------------------------------------------------------------------
// static TInt SettingsDescOrdering
// Ordering function for inserting the elements into the List box array.
// The ordering is done first based on the priority. 
//
// @param CLocationSettings& First Settings Element
// @param CLocationSettings& Second Settings Element
// @return 1. zero, if the two objects are equal.
//
//		   2. a negative value, if the first object is less than the second.
//
// 		   3. a positive value, if the first object is greater than the second.
// 
// ---------------------------------------------------------------------------	
TInt SettingsDescOrdering( const CLocationSettings& aSettings1, 
		  				   const CLocationSettings& aSettings2 )
    {
    CLocationSettings* settings1 = const_cast< CLocationSettings*>(&aSettings1);
    CLocationSettings* settings2 = const_cast< CLocationSettings*>(&aSettings2);
    return ( settings1->Priority() - settings2->Priority());
	}
