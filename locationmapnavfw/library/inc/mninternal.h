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
* Description:  Internal constants for Map and Navigation FW
*
*/


#ifndef MN_INTERNAL_H
#define MN_INTERNAL_H

#include <eikserverapp.h>
#include <lbsposition.h>
#include "mnmapview.h"
#include "mnmapimage.h"

// Map and Navigation Library resource file
_LIT( KLibResourceFile, "\\resource\\mnclientlib.rsc" );

// Internal panic category
_LIT( KMnPanicInternal, "MnInternal" );

// IPC constants
enum TMnIpcCode
    {
    EMnIpcNone = -1,
    EMnIpcStart = REikAppServiceBase::KServiceCmdBase,
    EMnIpcCancel,

    EMnIpcMapViewOffset = 1000,
    EMnIpcShowMap,
    EMnIpcShowCurrentLocation,
    EMnIpcSelectFromMap,
    EMnIpcGetSelectionResultLandmark,
    EMnIpcGetSelectionResultDbUri,
    EMnIpcAddLmIdsToShow,
    EMnIpcAddLandmarksToShow,
    EMnIpcResetLandmarksToShow,
    EMnIpcInitRequestText,

    EMnIpcNavigationOffset = 2000,
    EMnIpcNavigateTo,

    EMnIpcGeocodingOffset = 3000,
    EMnIpcCoordToAddress,
    EMnIpcAddressToCoord,
    EMnIpcPlainAddressToCoord,
    EMnIpcGetConversionResult,

    EMnIpcRenderImage = 4000,
    EMnIpcGetImageCoordinate,
    EMnIpcGetWorldCoordinate
    };

enum TMnIpcParams
    {
    // common params
    EMnIpcCancelRequestIdIndex = 0,

    // mapview params
    EMnIpcMapOptionsParamIndex = 0,
    EMnIpcMapSelectionTextParamIndex = 1,
    EMnIpcMapSelectionResultInfoParamIndex = 2,

    EMnIpcMapDbUriParamIndex = 0,
    EMnIpcMapLmIdsParamIndex = 1,

    EMnIpcMapLandmarksParamIndex = 0,

    EMnIpcMapSelResultBufferParamIndex = 0,

    // navigation params
    EMnIpcNaviLandmarkParamIndex = 0,

    // geocoding params
    EMnIpcGeoOptionsParamIndex = 0,
    EMnIpcGeoResultSizeParamIndex = 1,
    EMnIpcGeoLandmarkParamIndex = 2,
    EMnIpcGeoPlainAddressParamIndex = 2,

    EMnIpcGeoResultParamIndex = 0,
    EMnIpcGeoResultTypeParamIndex = 1,

    // mapimage params
    EMnIpcImageParamsParamIndex = 0,
    EMnIpcImageOptionsParamIndex = 1,
    EMnIpcImageBitmapHandleParamIndex = 2,
    };

enum TMnMapSelectionResultType
    {
    EMnMapSelResultNone,
    EMnMapSelResultLandmarkLink,
    EMnMapSelResultLandmarkInstance
    };

enum TMnGeocodingResultType
    {
    EMnGeoResultNone,
    EMnGeoResultCoordinates,
    EMnGeoResultAddress
    };

class TMnMapOptions
    {
    public:
        CMnMapView::TOptions                iOptions;
        CMnMapView::TCurrentLocationOption  iCurrentLocationOption;

        TCoordinate                         iCentralPoint;
        TReal                               iRadius;
    };

class TMnMapImageOptions
    {
    public:
        CMnMapImage::TShowOptions           iShowOptions;
        TPoint                              iTargetOrigin;
    };

#endif // MN_INTERNAL_H
