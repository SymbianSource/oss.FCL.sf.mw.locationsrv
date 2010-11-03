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
* Description:  Observer to Location Location Triggering Management settings' adapters
*
*/

#ifndef MLBTMGMTADAPTEROBSERVER_H_
#define MLBTMGMTADAPTEROBSERVER_H_

//  System Includes
#include <e32base.h>

// User Includes

// Forward Declarations

// Constant Declarations
// Class Definition

/**
 * Observer to the Location Triggering Management settings' adapter. This class
 * provides notification about the changes to the Settings values of
 * Location Triggering Management settings.
 */
class MLbtMgmtAdapterObserver
    {
    public:
        /**
         * Creates new Location Triggering Management plug-in view.
         */                                                 
        virtual void HandleSettingsChangeL() = 0;

    };          

#endif // MLBTMGMTADAPTEROBSERVER_H_

