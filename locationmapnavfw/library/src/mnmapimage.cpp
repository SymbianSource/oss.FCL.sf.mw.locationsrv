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
* Description:  CMnMapImage class implementation
*
*/


#include <e32base.h>
#include <e32math.h>
#include <AknServerApp.h>
#include <apaserverapp.h>
#include <ecom/ecom.h>
#include <fbs.h>

#include "mnappservices.hrh"
#include "mnerrors.h"
#include "mninternal.h"
#include "mnprovider.h"
#include "mnmapimageserviceclient.h"
#include "mncoordinateconverterbase.h"
#include "mnmapimageparams.h"
#include "mnactivecallback.h"
#include "mnmapimage.h"

// ============================== MEMBER FUNCTIONS ===========================

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
CMnMapImage::CMnMapImage()
    {
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C CMnMapImage* CMnMapImage::NewL( CMnProvider& aProvider )
    {
    CMnMapImage* self = new (ELeave) CMnMapImage;
    CleanupStack::PushL( self );
    self->ConstructL( aProvider );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnMapImage::ConstructL( CMnProvider& aProvider )
    {
    if ( !( aProvider.SupportedServices() & CMnProvider::EServiceMapImage ) )
        {
        User::Leave( KErrNotSupported );
        }

    iProviderFeatures = aProvider.SupportedFeatures( CMnProvider::EServiceMapImage );
    
    iCoordConverter = CMnCoordinateConverterBase::NewL( aProvider.Uid() );    

    iSession = new (ELeave) RMnMapImageServiceClient();
    iSession->ConnectL( aProvider.Uid() );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C CMnMapImage::~CMnMapImage()
    {
    if ( iActiveCallback )
        {
        iActiveCallback->Cancel();
        delete iActiveCallback;
        }
        
    delete iExitMonitor;
    
    if ( iSession )
        {
        iSession->Close();
        delete iSession;
        }
        
    delete iCoordConverter;
    REComSession::FinalClose();    
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C TMnMapImageParams::TMapImageTypes CMnMapImage::SupportedImageTypes()
    {
    TMnMapImageParams::TMapImageTypes supportedTypes( TMnMapImageParams::ETypeUnspecified );

    if ( iProviderFeatures & KMnSvcFeatureVectorMap )
        {
        supportedTypes |= TMnMapImageParams::ETypeVectorMap;
        }
    if ( iProviderFeatures & KMnSvcFeatureSatelliteImage )
        {
        supportedTypes |= TMnMapImageParams::ETypeSatelliteImage;
        }
    return supportedTypes;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C TMnMapImageParams& CMnMapImage::MapImageParams()
    {
    return iMapImageParams;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C const TMnMapImageParams& CMnMapImage::MapImageParams() const    
    {
    return iMapImageParams;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnMapImage::SetMapImageParams( TMnMapImageParams& aImageParams )
    {
    iMapImageParams = aImageParams;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnMapImage::SetTargetOrigin( const TPoint& aOrigin )
    {
    iTargetOrigin = aOrigin;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C TPoint CMnMapImage::TargetOrigin()
    {
    return iTargetOrigin;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnMapImage::SetShowOptions( const TShowOptions& aOptions )
    {
    iShowOptions = aOptions;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C CMnMapImage::TShowOptions CMnMapImage::ShowOptions() const
    {
    return iShowOptions;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnMapImage::RenderL( CFbsBitmap& aBitmap, TRequestStatus& aStatus )
    {
    if ( iTargetOrigin.iX < 0 || iTargetOrigin.iY < 0 )    
         {
         User::Leave( KErrArgument );
         }
        
    if ( aBitmap.SizeInPixels().iWidth < 
         iMapImageParams.Size().iWidth + iTargetOrigin.iX || 
         aBitmap.SizeInPixels().iHeight < 
         iMapImageParams.Size().iHeight + iTargetOrigin.iY )
         {
         User::Leave( KErrArgument );
         }
         
    if ( iMapImageParams.ImageType() != TMnMapImageParams::ETypeUnspecified &&
         !( iMapImageParams.ImageType() & SupportedImageTypes() )  )
        {
        User::Leave( KErrArgument );
        }

    TMnMapImageOptions options;
    options.iShowOptions = iShowOptions;
    options.iTargetOrigin = iTargetOrigin;
    iSession->Render( aBitmap, iMapImageParams, options, aStatus );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnMapImage::RenderL( CFbsBitmap& aBitmap, MMnMapImageRenderCallback& aCallback )
    {
    if ( !iActiveCallback ) 
        {
        iActiveCallback = CMnActiveCallback::NewL( *this, *iSession );
        }

    if ( iActiveCallback->IsActive() )
        {
        User::Panic( KMnPanicClientFault, KMnPanicDuplicateRequest );
        }

    // iActiveCallback member always points to CMnActiveCallback object
    CMnActiveCallback* activeCallback = reinterpret_cast<CMnActiveCallback*>( iActiveCallback );
    activeCallback->StartL( aBitmap, aCallback );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnMapImage::CancelRendering()
    {
    if ( iActiveCallback && iActiveCallback->IsActive() )
        {
        iActiveCallback->Cancel();
        }
    else
        {
        iSession->Cancel();
        }
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMnMapImage::GetImageCoordinate(
    const TCoordinate& aWorldCoordinate,
    TPoint& aImagePoint ) const
    {
    iCoordConverter->SetMapImageParams( iMapImageParams );
    return iCoordConverter->GetImageCoordinate( aWorldCoordinate, aImagePoint );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMnMapImage::GetWorldCoordinate(
    const TPoint& aImagePoint,
    TCoordinate& aWorldCoordinate ) const
    {
    iCoordConverter->SetMapImageParams( iMapImageParams );
    return iCoordConverter->GetWorldCoordinate( aImagePoint, aWorldCoordinate );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnMapImage::SetExitObserverL( MAknServerAppExitObserver& aObserver )
    {
    if ( iExitMonitor )
        {
        User::Leave( KErrAlreadyExists );
        }
    else
        {
        iExitMonitor = CApaServerAppExitMonitor::NewL( 
            *iSession, aObserver, CActive::EPriorityStandard );
        }
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnMapImage::RemoveExitObserver()
    {
    delete iExitMonitor;
    iExitMonitor = NULL;
    }
