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
* Description:  RMnGeocodingServiceClient class
*
*/


#ifndef MN_GEOCODINGSERVICECLIENT_H
#define MN_GEOCODINGSERVICECLIENT_H

#include "mninternal.h"
#include "mngeocoder.h"
#include "mnserviceclientbase.h"

/** \internal 
 *  Client side of IPC with CMnGeocodingServiceBase.
 */
NONSHARABLE_CLASS( RMnGeocodingServiceClient ): public RMnServiceClientBase
    {      
    public:
        /** C++ constructor */
        RMnGeocodingServiceClient();
        ~RMnGeocodingServiceClient();

        /** Issues async request for geocoding */
        void AddressByCoordinateL(
            const CPosLandmark& aCoordinateLanmdark,
            CMnGeocoder::TOptions aOptions,
            TRequestStatus& aStatus);
        
        /** Issues async request for reverse geocoding */
        void CoordinateByAddressL(
            const CPosLandmark& aAddressLanmdark,
            CMnGeocoder::TOptions aOptions,
            TRequestStatus& aStatus);

        /** Issues async request for reverse geocoding 
         *  with plain string address 
         */
        void CoordinateByAddressL(
            const TDesC& aAddress,
            CMnGeocoder::TOptions aOptions,
            TRequestStatus& aStatus);

        /** Client takes ownership of the returned pointer, 
         *  if this method succeeds.
         *  \leaves KErrNotFound if result is not available.
         */
        void GetLastResultL( CPosLandmark*& aResult, TMnGeocodingResultType& aResultType );

    private : // from RApaAppServiceBase
        IMPORT_C TUid ServiceUid() const;
        
    private:
        TInt            iResultSize;
        TPckg<TInt>     iResultSizePtr;
        
        HBufC8*         iLandmarkBuffer;    
        HBufC*          iPlainAddress;
        
        CMnGeocoder::TOptions   iOptions;
        TPtrC8                  iOptionsPtr;
    };
    
#endif // MN_GEOCODINGSERVICECLIENT_H
