/*
* Copyright (c) 2004-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CTestProviderCoordConverter class implementation
*
*/


#include <e32base.h>

#include "debug.h"
#include "testprovidercoordconverter.h"

// ========================== MEMBER FUNCTIONS ===============================

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
CTestProviderCoordConverter* CTestProviderCoordConverter::NewL()
    {
    CTestProviderCoordConverter* self = new (ELeave) CTestProviderCoordConverter;
    return self;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
CTestProviderCoordConverter::CTestProviderCoordConverter()
    {
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
CTestProviderCoordConverter::~CTestProviderCoordConverter()
    {
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
TInt CTestProviderCoordConverter::GetImageCoordinate( 
    const TCoordinate& aWorldCoordinate, 
    TPoint& aImagePoint )
    {
    const TMnMapImageParams& params = MapImageParams();
    TReal lat = aWorldCoordinate.Latitude();
    TReal lon = aWorldCoordinate.Longitude();
    
    switch ( params.ImageType() )
        {
        case TMnMapImageParams::ETypeVectorMap:
        case TMnMapImageParams::ETypeSatelliteImage:
            {
            if ( lat < 0 || lat > params.Size().iWidth ||
                 lon < 0 || lon > params.Size().iHeight )
                {
                return KErrUnknown;
                }
            break;
            }
        default:
            return KErrArgument;
        }
    aImagePoint.iX = aWorldCoordinate.Latitude();
    aImagePoint.iY = aWorldCoordinate.Longitude();
    return KErrNone;    
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
TInt CTestProviderCoordConverter::GetWorldCoordinate( 
    const TPoint& aImagePoint, 
    TCoordinate& aWorldCoordinate )
    {
    const TMnMapImageParams& params = MapImageParams();
    switch ( params.ImageType() )
        {
        case TMnMapImageParams::ETypeVectorMap:
        case TMnMapImageParams::ETypeSatelliteImage:
            {
            if ( aImagePoint.iX < -180 || aImagePoint.iX > 180 ||
                 aImagePoint.iY < -90  || aImagePoint.iY > 90 )
                {
                return KErrUnknown;
                }
            break;
            }
        default:
            return KErrArgument;
        }
    aWorldCoordinate.SetCoordinate( aImagePoint.iX, aImagePoint.iY );
    return KErrNone;    
    }
