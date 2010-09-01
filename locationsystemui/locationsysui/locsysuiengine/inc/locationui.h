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
* Description:  Location UI interface class. All the components intending to 
*                provide UI services through the Location UI Server must implement
*                this class
*
*/


#ifndef M_LOCATIONUI_H_
#define M_LOCATIONUI_H_

// System Includes
#include <e32base.h>

// User Include

// Forward Declarations

// Class Definition
/**
 * Observer for notification regarding the termination of the Launch call.
 *
 * @lib locsysuiengine.lib
 * @since S60 v3.2 
 */
class MLocationUIObserver
    {
    public:
    	/**
    	 * Notifies the termination of the Location UI
    	 *
    	 * @since S60 v3.2
    	 * @param aErrorCode The termination reason. KErrNone for normal 
    	 *					 terminations. In case of error or pre-mature
    	 *					 aborting System wide Error codes.
    	 */      
        virtual void LocationUIDismissed( TInt aErrorCode ) = 0;
    };
    
/**
 * Location UI interface class. All the components intending to provide Location
 * UI related services must implement this class. This class provides the interface
 * to Launch and Close a Location UI component
 *
 * @lib locsysuiengine.lib
 * @since S60 v3.2
 */
class MLocationUI
	{		   		  
	public:
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
                                        MLocationUIObserver*    aObserver ) = 0;

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
                                        MLocationUIObserver*    aObserver ) = 0;

        /**
         * Closes the running Location UI prematurely.
         */
        virtual void Close() = 0;
        
	};

#endif // M_LOCATIONUI_H_

