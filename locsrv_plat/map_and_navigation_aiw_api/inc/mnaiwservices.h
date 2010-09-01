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
* Description:  Enumerations and structures for Map and Navigation AIW parameters
*
*/


#ifndef MN_AIWSERVICES_H
#define MN_AIWSERVICES_H

#include <lbsposition.h>

/** Common options for Map and Navigation AIW services.
 *
 *  Semantic ID must be \p EGenericParamMnCommonOptions.
 *  Type ID must be \p EVariantTypeDesC8 (packed as TPckg).
 *  @since 3.1
 *  \ingroup MnAiwAPI
 */
struct TMnAiwCommonOptionsParam
    {
    /** Whether Provider Application Server shall
     * be executed chained or standalone. */
    TBool iRunChained;
    /** Reserved for future use. */
    TInt8 reserved[8];
    };

/** Field types, to be used with TMnAiwCommonOptionsParamExtended::iFieldsSet */
enum TMnAiwCommonOptionsFields
    {
    EMnAiwCommonOptionsFieldsNotSet = 0,
    EMnAiwCommonOptionsFieldRunChained = 0x01,
    EMnAiwCommonOptionsFieldProviderId = 0x02,
    EMnAiwCommonOptionsFieldMessagesDisabled = 0x04,
    };

/** Extended common options for Map and Navigation AIW services.
 *
 *  Semantic ID must be \p EGenericParamMnCommonOptions.
 *  Type ID must be \p EVariantTypeDesC8 (packed as TPckg).
 *  @since 5.0
 *  \ingroup MnAiwAPI
 *  @see TMnAiwCommonOptionsParam
 */
struct TMnAiwCommonOptionsParamExtended
    {
    /** Bitmask of fields set in the struct, defined in @ref TMnAiwCommonOptionsFields.
     *  If a bit is unset, the appropriate fields is ignored. */
    TInt iFieldsSet;

    /** Whether Provider Application Server shall
     *  be executed chained or standalone.
     *  This field is used only if EMnAiwCommonOptionsFieldRunChained flag is 
     *  set in iFieldsSet bit map, otherwise ignored. */
    TBool iRunChained;

    /** Integer value of needed Provider application UID.
     *  This field is used only if EMnAiwCommonOptionsFieldProviderId flag is 
     *  set in iFieldsSet bit map, otherwise ignored. */
    TInt32 iProviderId;
    
    /** Defines whether UI messages are allowed to be shown to user or not.
     *  By default error messages are shown if request fails. Setting this
     *  field to ETrue will ban these messages. Client has to process
     *  the error code returned itself. */
    TBool iMessagesDisabled;

    /** Reserved for future use. */
    TInt8 reserved[32];
    };

/** Contains additional parameters for
 *  @ref svcShowMap and @ref svcSelectFromMap AIW service commands.
 *
 *  These parameters work same as defined by @ref CMnMapView.
 *
 *  Semantic ID must be \p EGenericParamMnMapViewOptions.
 *  Type ID must be \p EVariantTypeDesC8 (packed as TPckg).
 *  @since 3.1
 *  \ingroup MnAiwAPI
 */
struct TMnAiwMapViewOptionsParam
    {
    /** How current location should be drawn, 
     *  one of @ref CMnMapView::TCurrentLocationOption values */
    TUint32 iCurrentLocationOption;
    /** Bitmap of additional options of map view, see @ref CMnMapView::TOptions. */
    TUint32 iOptions;
    /** Radius of visible map area in map view. */
    TReal iRadius;
    /** Central point of map view. */
    TCoordinate iCenterPoint;
    /** Reserved for future use. */
    TInt8 reserved[8];
    };

/** Contains geocoding parameters for
 *  @ref svcAddrByCoord and @ref svcCoordByAddr AIW service commands.
 *
 *  These parameters are same as defined by @ref CMnGeocoder.
 *
 *  Semantic ID must be \p EGenericParamMnGeocodingOptions.
 *  Type ID must be \p EVariantTypeDesC8 (packed as TPckg).
 *  @since 3.1
 *  \ingroup MnAiwAPI
 */
struct TMnAiwGeocodingOptionsParam
    {
    /** Bitmap of @ref CMnGeocoder::TOption values */
    TUint32 iOptions;
    /** Reserved for future use. */
    TInt8 reserved[8];
    };


#endif // MN_AIWSERVICES_H

//End of file
