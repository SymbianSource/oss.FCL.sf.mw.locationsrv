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
* Description:  CTestProviderMapImageService class
*
*/


#ifndef MN_TPMAPIMAGESERVICE_H
#define MN_TPMAPIMAGESERVICE_H

#include "mnmapimageservicebase.h"

class CPosLandmark;
class CTestCommandListener;

/** Implementation of geocoding service (KMnAppMapImageService). 
 */
class CTestProviderMapImageService : public CMnMapImageServiceBase
    {
    public :
        static CTestProviderMapImageService* NewL();
        
        void HandleTestCommandL( TInt aCommand );

    protected :
        /** C++ constructor */
        CTestProviderMapImageService();
        /** Destructor */
        virtual ~CTestProviderMapImageService();

        void ConstructL();

        void LeaveIfBusyL();
        void LeaveIfInstructedL();
        void StartProcessingL();
        static TInt MapImageProcessingCallBack( TAny* aPtr );
        void HandleMapImageCompletedL();
        void FinishProcessingL();

        TInt GetTestCommand();

        void RenderL();

    private:
        void DumpDataL();

    protected: // from CMnMapImageServiceBase
    
        /** Handles client's request for finding address by given coordinate. */
        void HandleRenderingL();
        
    protected: // from CMnServiceBase

        /** Reports that client cancelled request */
        void DoCancel();

    private:
    
        enum TServiceType
            {
            ENone,
            ERender,
            };
            
        TServiceType            iCurrentService;
    
        CPeriodic*              iProcessor;
    };

#endif // MN_TPMAPIMAGESERVICE_H


