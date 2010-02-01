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
* Description:  Location Settings implementation of Location UIs. 
*
*/


#ifndef C_LOCATIONSETTINGS_H_
#define C_LOCATIONSETTINGS_H_

// System Include
#include <e32base.h>

// User Includes
#include "locationui.h"
#include "locsettingsuiobserver.h"

// Forward Declarations
class CLocSettingsUi;
class CAknViewAppUi;

// Class Definition
/**
 * Location Settings implementation of Location UIs. This class extends the 
 * Location UI interface for Location sub-settings plug-ins. This class acts as
 * the container class for the Location sub-settings plug-ins and takes up
 * complete ownership of these classes.
 *
 * @lib locsysuiview.lib
 * @since S60 v3.2
 */
class CLocationSettings : public CBase,
                          public MLocationUI,
                          public MLocSettingsUiObserver
	{
	public:	
        /**
         * Two Phase Constructor.
         *
         * @param   aImplementationId   Implementation UID of the settings plug-in.
         * @param   aAppUi              Application's AppUi reference.
         * @return	CLocationSettings*	Pointer to the created object
         */
        static CLocationSettings* NewL( const TUid	    aImplementationId,
                                        CAknViewAppUi&	aAppUi );
        	
        /**
         * Two Phase Constructor. The funcion leaves the CLocationSettings object
         * on the CleanupStack
         *
         * @param   aImplementationId   Implementation UID of the settings plug-in.
         * @param   aAppUi              Application's AppUi reference.
         * @return	CLocationSettings*	Pointer to the created object
         */
        static CLocationSettings* NewLC( const TUid	    aImplementationId,
                                         CAknViewAppUi&	aAppUi );
        
        /**
         * C++ Destructor
         */
        virtual ~CLocationSettings();

        /**
         * Accessor Function for the Setings Ui Caption. The function does not
         * allocate any buffer but returns its internal buffer. This buffer 
         * should not be deleted/modified by the callee.
         *
         * @return const TPtrC Caption String
         */
        TPtrC Caption();

        /**
         * Returns the priority of the UI module. 
         *
         * @return TInt Priority of the Settings UI
         */
        TInt Priority();

        /**
         * Accessor Function for the Settings Ui Implementation UID
         *
         * @return TUid Settings UI Plug In's implementation UID
         */
        const TUid	ImplementationUid();         
        
    public: // Inherited from MLocationUI
    
        /**
         * Launches a Location UI component.
         *
         * @param aUiInputParams Flags that define specific configuration of the
         *                       Location UI, that the user of the API wants to
         *                       launch. If more than one functionality is
         *                       requested then the various combinations are to be
         *                       ORed. The interpretation of these flag values is
         *                       a part of the understanding between the API user
         *                       and the corresponding Location UI.
         * @param aObserver      Observer where the termination of the Launch call
         *                       needs to be notified.
         *                          
         */
        virtual void LaunchLocationUIL( TInt		            aUiInputParams,
                                        MLocationUIObserver*    aObserver );
    
        /**
         * Launches a Location UI component.
         *
         * @param aUiInputParams String passed as a parameter to the
         *                       Location UI, that the user of the API wants to
         *                       launch. The interpretation of this string is
         *                       a part of the understanding between the API user
         *                       and the corresponding Location UI.
         * @param aObserver      Observer where the termination of the Launch call
         *                       needs to be notified.
         *                          
         */
        virtual void LaunchLocationUIL( const TDesC&		            aUiInputParams,
                                        MLocationUIObserver*    aObserver );

        /**
         * Closes the running Location UI prematurely.
         */
        virtual void Close();       
        
    public:// Inherited from MLocSettingsUiObserver     
        /**
         * Inherited from MLocSettingsUiObserver. Notification from the Settings UI
         * to indicate the termination of the Settings UI application
         *
         * @param aErrorCode The termination reason. KErrNone for normal 
         *					 terminations. In case of error or pre-mature aborting
         *					 System wide Error codes.
         */
        void SettingClosed( TInt aErrorCode );
                        
	private:
	    /**
         * C++ Constructor
         *
         * @param	aLocationSettings	Pointer to the Location sub-settings UI
         *                              plug-in. The ownership of the plug-in is 
         *                              transferred to the constructed class.
         */
        CLocationSettings( const TUid	     aImplementationId );
        			        
        /**
         * Second Phase of the two phase constructor
         *
         */
        void ConstructL( CAknViewAppUi&	aAppUi );

	private:
	    /**
         * Buffer holding the localized caption string that is used to descibe
         * the Settings UI
         * Owns
         */	 
		HBufC*			    iCaption;
			
	    /**
         * Pointer to the Location Sub-settings UI object
         * Owns
         */
		CLocSettingsUi*     iLocationSettings;
		
		/**
		 * Implementation UID of the Location sub-settings UI object
		 */
		const TUid          iImplementationId;
		
        /**
         * Location UI Observer
         * Reference
         */
        MLocationUIObserver*    iObserver;		
	    	
	};

#endif // C_LOCATIONSETTINGS_H_

