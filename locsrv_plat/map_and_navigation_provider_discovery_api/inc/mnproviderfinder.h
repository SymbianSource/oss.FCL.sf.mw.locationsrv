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
* Description:  MnProviderFinder class
*
*/


#ifndef MN_PROVIDERFINDER_H
#define MN_PROVIDERFINDER_H

#include <e32cmn.h>
#include "mnprovider.h"

/** @brief Provides static methods for discovering
 *  Map and Navigation provider applications available in the system.
 *
 *  @since 3.1
 *  @lib mnclientlib.dll
 *  @ingroup DiscoveryAPI
 */
class MnProviderFinder
    {
    public :
        /** @brief Finds all Map and Navigation Provider Applications
         *  in the system.
         *
         *  @param[out] aProviders Array of provider applications found. This
         *                         array is left unchanged, if no providers found.
         *                         Client takes ownership of objects in the array.
         */
        IMPORT_C static void FindProvidersL(
            RPointerArray<CMnProvider>& aProviders);

        /** @brief Finds all Map and Navigation Provider Applications
         *  in the system, which are capable of specified features.
         *
         *  @param[out] aProviders Array of provider applications found. This
         *                         array is left unchanged, if no providers found.
         *                         Client takes ownership of objects in the array.
         *  @param aRequiredServices Bitmap of map and navigation features. Only
         *                           those providers, which support all these
         *                           services are listed on return.
         */
        IMPORT_C static void FindProvidersL(
            RPointerArray<CMnProvider>& aProviders,
            CMnProvider::TServices aRequiredServices);
    };

#endif // MN_PROVIDERFINDER_H

