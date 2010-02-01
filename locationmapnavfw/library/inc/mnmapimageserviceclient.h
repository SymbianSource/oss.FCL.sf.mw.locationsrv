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
* Description:  RMnMapImageServiceClient class
*
*/


#ifndef MN_MAPIMAGESERVICECLIENT_H
#define MN_MAPIMAGESERVICECLIENT_H

#include "mninternal.h"
#include "mnmapimage.h"
#include "mnserviceclientbase.h"

/** @internal 
 *  Client side of IPC with CMnMapImageServiceBase.
 */
NONSHARABLE_CLASS( RMnMapImageServiceClient ): public RMnServiceClientBase
    {      
    public:
        /** C++ constructor */
        RMnMapImageServiceClient();

        /** Issues async request for rendering */
        void Render(
            CFbsBitmap& aBitmap,
            TMnMapImageParams& aParams,
            TMnMapImageOptions& aOptions,
            TRequestStatus& aStatus);

    private : // from RApaAppServiceBase
        IMPORT_C TUid ServiceUid() const;
        
    private:
        TPtr8                       iParamsDes;

        TMnMapImageOptions          iOptions;
        TPckgC<TMnMapImageOptions>  iOptionsPack;
    };
    
#endif // MN_MAPIMAGESERVICECLIENT_H
