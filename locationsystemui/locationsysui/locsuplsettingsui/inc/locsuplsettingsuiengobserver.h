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
* Description:  Observer to the SUPL Settings UI Engine observer
*
*/


#ifndef M_LOCSUPLSETTINGSUIENGOBSERVER_H_
#define M_LOCSUPLSETTINGSUIENGOBSERVER_H_

// User Includes
#include "locsuplsettingsadapterobserver.h"

// Class Declaration
/**
 * Observer to the SUPL Settings UI Engine
 */
 
class MLocSUPLSettingsUiEngObserver
    {
    public:
        /**
         * Enumeration defining all the event types from the SUPL Settings
         * Engine
         */
        enum TSettingsEventType
            {
            EEngineInitialized = MLocSUPLSettingsAdapterObserver::ESUPLSettingsIntialized,
            EEngineInitializeFailed,            
            ESUPLSettingsChange,
            EIAPSelectionComplete,
            EIAPSelectionCancelled,
            EExitEvent
            };
    public:
        /**
         * Handles any event from the SUPL Settings Engine
         *
         * @param aEvent Event from the SUPL Settings Engine
         */
        virtual void HandleSettingsEventL( TSettingsEventType    aEvent ) = 0;
           
    };
    
#endif

