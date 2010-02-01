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
* Description:  RMnMapImageServiceClient class implementation
*
*/


#include <e32base.h>
#include <fbs.h>

#include "mndebug.h"
#include "mninternal.h"
#include "mnappserviceuids.hrh"

#include "mnprovider.h"
#include "mnmapimage.h"
#include "mnmapimageserviceclient.h"

// ======== LOCAL FUNCTIONS ========

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
RMnMapImageServiceClient::RMnMapImageServiceClient()
  : iParamsDes( NULL, 0 ), iOptionsPack( iOptions )
    {
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void RMnMapImageServiceClient::Render( 
    CFbsBitmap& aBitmap, 
    TMnMapImageParams& aParams,
    TMnMapImageOptions& aOptions,
    TRequestStatus& aStatus)
    {
    LOG("RMnMapImageServiceClient::RenderL");
    TIpcArgs args;

    // bitmap handle
    TInt handle = aBitmap.Handle();
    args.Set( EMnIpcImageBitmapHandleParamIndex, handle );

    // options
    iOptions = aOptions;
    args.Set( EMnIpcImageOptionsParamIndex, &iOptionsPack );

    // params
    TPckg<TMnMapImageParams> pack( aParams );
    iParamsDes.Set( pack );
    args.Set( EMnIpcImageParamsParamIndex, &iParamsDes );
    
    SendReceive( EMnIpcRenderImage, args, aStatus );
    iLastRequest = EMnIpcRenderImage;
    }
    
// ---------------------------------------------------------------------------
// From class RApaAppServiceBase.
// ---------------------------------------------------------------------------
//
EXPORT_C TUid RMnMapImageServiceClient::ServiceUid() const
    { 
    LOG("RMnMapImageServiceClient::ServiceUid");
    return TUid::Uid( KMnAppMapImageService ); 
    }
