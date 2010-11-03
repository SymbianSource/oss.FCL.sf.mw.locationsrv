/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CMnActiveCallback class implementation
*
*/


#include <e32base.h>

#include "mnmapimage.h"
#include "mnmapimageserviceclient.h"
#include "mnactivecallback.h"

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
CMnActiveCallback::CMnActiveCallback( 
    CMnMapImage& aOwner, 
    RMnMapImageServiceClient& aSession )
  : CActive( CActive::EPriorityStandard ), 
    iOwner( aOwner ), 
    iSession( aSession )
    {
    CActiveScheduler::Add( this );
    }
    
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
CMnActiveCallback* CMnActiveCallback::NewL( 
    CMnMapImage& aOwner,
    RMnMapImageServiceClient& aSession )
    {
    CMnActiveCallback* self = new (ELeave) CMnActiveCallback( aOwner, aSession );
    return self;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnActiveCallback::StartL(
    CFbsBitmap& aBitmap, 
    MMnMapImageRenderCallback& aCallback )
    {
    ASSERT( !IsActive() );
    iCallback = &aCallback;
    iOwner.RenderL( aBitmap, iStatus );
    SetActive();
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnActiveCallback::RunL()
    {
    ASSERT( iCallback );
    iCallback->HandleRenderingCompletedL( iStatus.Int(), iOwner );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnActiveCallback::DoCancel()
    {
    iSession.Cancel();
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
TInt CMnActiveCallback::RunError( TInt /*aError*/ )
    {
    return KErrNone;
    }
