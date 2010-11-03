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
* Description:  Observer for the SUPL Settings adapter
*
*/


#ifndef MLOCSUPLSETTINGSADAPTEROBSERVER_H_
#define MLOCSUPLSETTINGSADAPTEROBSERVER_H_

// Class Declaration

/**
 * Observer for the SUPL Settings Adapter. The observer provides callbacks for
 * any changes in the SUPL Settings UI
 *
 * @lib locsuplsettings.lib
 * @since S60 v3.1 
 */
class MLocSUPLSettingsAdapterObserver
    {
    public:
        /** 
         * Enumeration for events from the SUPL Settings UI
         */
        enum TLocSUPLSettingsEvent
        	{
        	ESUPLSettingsIntialized,
        	ESUPLSettingsInitalizeError,
        	ESUPLSettingsChanged
        	};
        	
        /**
         * Called when a change has been detected in the SUPL Settings.
         *
         * @param aEvent Information about the SUPL Settings event.
         */
        virtual void HandleSuplSettingsChangeL( TLocSUPLSettingsEvent aEvent ) = 0;

    };

#endif      // MLOCSUPLSETTINGSADAPTEROBSERVER_H_  
            
// End of File
