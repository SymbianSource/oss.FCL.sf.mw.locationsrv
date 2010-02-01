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
* Description:  CMnGeocodingServiceBase class
*
*/


#ifndef MN_GEOCODINGSERVICEBASE_H
#define MN_GEOCODINGSERVICEBASE_H

#include "mnservicebase.h"
#include "mngeocoder.h"

class CPosLandmark;

/** @brief Base class for geocoding service implementations (KMnAppGeocodingService). 
 *
 *  Provider Application must derive its geocoding service class from this one. 
 *  This base implementation hides IPC communication details and provides 
 *  native methods for accessing various parameters, specified by client.
 *
 *  Provider Application must return pointer to instance of its class, derived
 *  from this class, in response to CAknAppService::CreateServiceL() call, 
 *  if given parameter is KMnAppGeocodingService. 
 *
 *  This class defines handlers of particular requests from client as pure 
 *  virtual methods, which derived class must implement. The parameters, which
 *  client application may specify in @ref CMnGeocoder are accessible via other methods
 *  of this class. Implementation must obey these parameters and use them
 *  as specified in @ref CMnGeocoder.
 *
 *  See parent @ref CMnServiceBase class documentation for 
 *  other details.
 *
 *  @since 3.1
 *  @lib mnservicelib.lib
 *  @ingroup MnProviderAPI
 */
class CMnGeocodingServiceBase : public CMnServiceBase
    {
    public :

        /** @brief Returns geocoding options specified by client application.
         *
         *  This options must be utilized in request handlers as specified in 
         *  @ref CMnGeocoder. 
         *  
         *  @return Options specified by client.
         */
        IMPORT_C CMnGeocoder::TOptions Options() const;

        /** @brief Completes geocoding request and sends result.
         *
         *  Implementation must call this method to signal successful completion
         *  of conversion request. Result of conversion is supplied as landmark 
         *  object. It must contain only data, which is part of conversion result,
         *  any other fields shall be left empty.
         *
         *  @param[in] aLandmark Landmark containing result of conversion.
         */
        IMPORT_C void CompleteGeocodingRequestL( const CPosLandmark& aLandmark );

    protected :
    
        /** C++ constructor */
        IMPORT_C CMnGeocodingServiceBase();
        /** Destructor */
        IMPORT_C ~CMnGeocodingServiceBase();

        /** This method must be called from derived class' ConstructL to
         *  allow internal initialization of this class. */
        IMPORT_C void BaseConstructL();

        /** @brief Handles client's request for finding address by given coordinate. 
         *
         *  Derived class must implement this method obeying specification of 
         *  @ref CMnGeocoder::FindAddressByCoordinateL() and if 
         *  @ref KMnSvcFeatureCoordToAddr feature is specified in SERVICE_INFO.
         *
         *  If given landmark coordinates cannot be resolved to address due
         *  to some ambiguity, then user shall be queried, if allowed by options
         *  (see @ref Options()). If it is not allowed, the operation
         *  shall be completed with KErrNotFound (@ref CompleteRequest()).
         *
         *  This is asynchronous request.
         *  Client's request must be completed (when resolving succeeded) with 
         *  CompleteGeocodingRequest().
         *
         *  @param[in] aLandmark Contains coordinate information to be resolved
         *                       to address.
         *  
         *  @leave KErrNotSupported Should leave if this feature is not supported.
         */
        virtual void HandleFindAddressByCoordinateL( const CPosLandmark& aLandmark ) =0;
        
        /** @brief Handles client's request for finding coordinate by given address. 
         *
         *  Derived class must implement this method obeying specification of 
         *  @ref CMnGeocoder::FindCoordinateByAddressL(const CPosLandmark&, 
         *  TRequestStatus&) and if @ref KMnSvcFeatureAddrToCoord feature is 
         *  specified in SERVICE_INFO.
         *
         *  If given address cannot be resolved to coordinates due
         *  to some ambiguity, then user shall be queried, if allowed by options
         *  (see @ref Options()).If it is not allowed, the result of the operation
         *  shall be KErrNotFound.
         *
         *  This is asynchronous request.
         *  Client's request must be completed (when resolving succeeded) with 
         *  CompleteGeocodingRequest().
         *
         *  @param[in] aLandmark Contains address information to be resolved
         *                       to coordinate.
         *  
         *  @leave KErrNotSupported Should leave if this feature is not supported.
         */
        virtual void HandleFindCoordinateByAddressL( const CPosLandmark& aLandmark ) =0;

        /** @brief Handles client's request for finding coordinate by given address. 
         *
         *  This overload is for cases when address is specified as plain string.
         *  Derived class must implement this method obeying specification of 
         *  @ref CMnGeocoder::FindCoordinateByAddressL(const TDesC& aAddress, 
         *  TRequestStatus& aRequest) and if @ref KMnSvcFeaturePlainAddrToCoord feature 
         *  is specified in SERVICE_INFO.
         *
         *  If given address cannot be resolved to coordinates due
         *  to some ambiguity, then user shall be queried, if allowed by options
         *  (see @ref Options()).If it is not allowed, the result of the operation
         *  shall be KErrNotFound.
         *
         *  This is asynchronous request.
         *  Client's request must be completed (when resolving succeeded) with 
         *  CompleteGeocodingRequest().
         *
         *  @param[in] aAddress Address information to be resolved to coordinate.
         *  
         *  @leave KErrNotSupported Should leave if this feature is not supported.
         */
        virtual void HandleFindCoordinateByAddressL( const TDesC& aAddress ) =0;

    protected: // From CAknAppServiceBase

        /** \internal */
        IMPORT_C void ServiceL( const RMessage2& aMessage );

    private:
        void StartAsyncRequestL( const RMessage2& aMessage );
        void UnpackOptionsL( const RMessage2& aMsg );
        HBufC* UnpackPlainAddressLC( const RMessage2& aMsg );

        void HandleGetConversionResultL( const RMessage2& aMsg );
    
    private:
        CMnGeocoder::TOptions   iOptions;

        HBufC8*                 iResult;
        TInt                    iResultType;
    };

#endif // MN_GEOCODINGSERVICEBASE_H


