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
* Description:  Observer to the Location Notation Preferences settings' adapters
*
*/

#ifndef M_LOCNOTPREFADAPTEROBSERVER_H_
#define M_LOCNOTPREFADAPTEROBSERVER_H_

//  System Includes
#include <e32base.h>

// User Includes

// Forward Declarations

// Constant Declarations
// Class Definition

/**
 * Observer to the Location notation preferences settings' adapter. This class
 * provides notification about the changes to the Central-repository values of
 * Location notation preferences settings.
 */
class MLocNotPrefAdapterObserver
    {
    public:
        /**
         * Creates new Location Notation Preferences plug-in view.
         */                                                 
        virtual void HandleSettingsChangeL() = 0;

    };          

#endif // M_LOCNOTPREFADAPTEROBSERVER_H_

