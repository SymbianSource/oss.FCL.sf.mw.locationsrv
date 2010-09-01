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
* Description:  CTestProviderGeocodingService class
*
*/


#ifndef MN_TPGEOCODINGSERVICE_H
#define MN_TPGEOCODINGSERVICE_H

#include "mngeocodingservicebase.h"

class CPosLandmark;
class CTestCommandListener;

/** Implementation of geocoding service (KMnAppGeocodingService). 
 */
class CTestProviderGeocodingService : public CMnGeocodingServiceBase
    {
    public :
        static CTestProviderGeocodingService* NewL();
        
        void HandleTestCommandL( TInt aCommand );

    protected :
        /** C++ constructor */
        CTestProviderGeocodingService();
        /** Destructor */
        virtual ~CTestProviderGeocodingService();

        void ConstructL();

        void LeaveIfBusyL();
        void LeaveIfInstructedL();
        void StartProcessingL();
        static TInt GeocodingProcessingCallBack( TAny* aPtr );
        void HandleGeocodingCompletedL();
        void FinishProcessingL();

        TInt GetTestCommand();

    private:
        void DumpDataL( const TDesC& aAddress );
        void DumpDataL( const CPosLandmark& aLandmark );
        

    protected: // from CMnGeocodingServiceBase
    
        /** Handles client's request for finding address by given coordinate. */
        void HandleFindAddressByCoordinateL( const CPosLandmark& aLandmark );
        
        /** Handles client's request for finding coordinate by given address. */
        void HandleFindCoordinateByAddressL( const CPosLandmark& aLandmark );

        /** Handles client's request for finding coordinate by given address. */
        void HandleFindCoordinateByAddressL( const TDesC& aAddress );
        
    protected: // from CMnServiceBase

        /** Reports that client cancelled request */
        void DoCancel();

    private:
    
        enum TServiceType
            {
            ENone,
            ECoordToAddr,
            EAddrToCoord,
            EPlainToCoord
            };
            
        TServiceType            iCurrentService;
    
        CPosLandmark*           iLandmark;
        HBufC*                  iPlainAddress;
        
        CPeriodic*              iProcessor;
    };

#endif // MN_TPGEOCODINGSERVICE_H


