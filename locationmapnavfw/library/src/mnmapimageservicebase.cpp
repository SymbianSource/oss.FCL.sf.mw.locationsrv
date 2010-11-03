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
* Description:  CMnMapImageServiceBase class implementation
*
*/


#include <e32base.h>
#include <fbs.h>

#include "mndebug.h"
#include "mnerrors.h"
#include "mninternal.h"
#include "mninternal.inl"

#include "mnmapimageservicebase.h"

// ========================== MEMBER FUNCTIONS ===============================

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C CMnMapImageServiceBase::CMnMapImageServiceBase()
    {
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C CMnMapImageServiceBase::~CMnMapImageServiceBase()
    {
    if ( iBitmap )
        {
        delete iBitmap;
        }
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnMapImageServiceBase::BaseConstructL()
    {
    CMnServiceBase::BaseConstructL();
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C const TMnMapImageParams& CMnMapImageServiceBase::MapImageParams() const
    {
    return iMapImageParams;
    }
        
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C TMnMapImageParams& CMnMapImageServiceBase::MapImageParams()
    {
    return iMapImageParams;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnMapImageServiceBase::SetMapImageParams( const TMnMapImageParams& aImageParams )
    {
    iMapImageParams = aImageParams;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C TRect CMnMapImageServiceBase::TargetRect() const
    {
    if ( iBitmap )
        {
        return TRect( iTargetOrigin, iBitmap->SizeInPixels() );
        }
    else
        {
        return TRect( 0, 0, 0, 0 );
        }
    }
        
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C CMnMapImage::TShowOptions CMnMapImageServiceBase::ShowOptions() const
    {
    return iShowOptions;
    }
        
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C CFbsBitmap& CMnMapImageServiceBase::TargetBitmap() const
    {
    if ( !iBitmap )
        {
        PanicServer( KErrGeneral );
        }
    return *iBitmap;
    }
        
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnMapImageServiceBase::CompleteRendering()
    {
    LOG("CMnMapImageServiceBase::CompleteRenderingL in");
    if ( CurrentAsyncRequest() != EMnIpcRenderImage )
        {
        PanicServer( KMnPanicAttemptToCompleteNoRequest );
        }
        
    if ( iMapImageParams.ImageType() == TMnMapImageParams::ETypeUnspecified ) 
        {
        PanicServer( KMnPanicInvalidResult );
        }   

    TPckg<TMnMapImageParams> paramsPack( iMapImageParams );
    TInt err = iMessage.Write( EMnIpcImageParamsParamIndex, paramsPack );

    CompleteRequest( err );
    
    delete iBitmap;
    iBitmap = NULL;

    LOG("CMnMapImageServiceBase::CompleteRenderingL out");
    }
    
// ---------------------------------------------------------------------------
//  From class CAknAppServiceBase
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnMapImageServiceBase::ServiceL( const RMessage2& aMessage )
    {
    //LOG1("CMnMapImageServiceBase::ServiceL in, function %d", aMessage.Function());
    
    switch ( aMessage.Function() )
        {
        case EMnIpcRenderImage:
            PrepareAsyncRequestL( aMessage );
            StartAsyncRequestL( aMessage );
            break;

        case EMnIpcCancel:
            HandleCancelRequestL( aMessage );
            break;
            
        default:
            CMnServiceBase::ServiceL( aMessage );
        }
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnMapImageServiceBase::StartAsyncRequestL( const RMessage2& aMessage )
    {
    LOG("CMnMapImageServiceBase::StartAsyncRequestL in");

    switch ( CurrentAsyncRequest() )
        {
        case EMnIpcRenderImage:
            UnpackImageParamsL( aMessage );
            UnpackOptionsL( aMessage );
            UnpackBitmapHandleL( aMessage );
            HandleRenderingL();
            break;
            
        default:
            User::Leave( KErrNotSupported );
        }

    LOG("CMnMapImageServiceBase::StartAsyncRequestL out");
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnMapImageServiceBase::UnpackOptionsL( const RMessage2& aMessage )
    {
    LOG("CMnMapImageServiceBase::UnpackOptionsL");
    
    TMnMapImageOptions options;
    TPckg<TMnMapImageOptions> optsPack( options );
    aMessage.ReadL( EMnIpcImageOptionsParamIndex, optsPack );
    
    if ( options.iShowOptions & CMnMapImage::EShowOptionCurrentLocation )
        {
        // leaves with KErrPermissionDenied if client does not have Location capability
        aMessage.HasCapabilityL( ECapabilityLocation );
        }
    
    iShowOptions = options.iShowOptions;
    iTargetOrigin = options.iTargetOrigin;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnMapImageServiceBase::UnpackImageParamsL( const RMessage2& aMessage )
    {
    LOG("CMnMapImageServiceBase::UnpackImageParamsL");
    
    TPckg<TMnMapImageParams> paramsPack( iMapImageParams );
    aMessage.ReadL( EMnIpcImageParamsParamIndex, paramsPack );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnMapImageServiceBase::UnpackBitmapHandleL( const RMessage2& aMessage )
    {
    TInt bitmapHandle = aMessage.Int2(); // EMnIpcImageBitmapHandleParamIndex == 2
    LOG1("CMnMapImageServiceBase::UnpackBitmapHandleL, handle %d", bitmapHandle);

    ASSERT( !iBitmap );
        
    iBitmap = new (ELeave) CFbsBitmap;
    User::LeaveIfError( iBitmap->Duplicate( bitmapHandle ) );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnMapImageServiceBase::HandleCancelRequestL( const RMessage2& aMessage )
    {
    TInt request = CurrentAsyncRequest();
    
    CMnServiceBase::HandleCancelRequestL( aMessage );
    
    if ( request == EMnIpcRenderImage )
        {
        delete iBitmap;
        iBitmap = NULL;
        }
    }
