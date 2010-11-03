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
* Description:  CTestProviderMapViewService class
*
*/


#ifndef MNTP_MAPVIEWSERVICE_H
#define MNTP_MAPVIEWSERVICE_H

#include "mnmapviewservicebase.h"

class CPosLandmark;
class CTestCommandListener;

/** Implementation of MapView service (KMnAppMapViewService). 
 */
class CTestProviderMapViewService : public CMnMapViewServiceBase
    {
    public :
        static CTestProviderMapViewService* NewL();
        void HandleTestCommandL( TInt aCommand );
        
    protected :
        /** C++ constructor */
        CTestProviderMapViewService();
        /** Destructor */
        virtual ~CTestProviderMapViewService();

        void ConstructL();

        void LeaveIfBusyL();
        void LeaveIfInstructedL();
        void StartProcessingL();
        static TInt SelectionIdleCallBack( TAny* aPtr );
        void HandleSelectionL();
        void DoSelectionL();

        void DumpDataL();
        TInt GetTestCommand();

    protected: // from CMnMapViewServiceBase
    
        /** Handles client's request for showing map */
        void HandleShowMapL();

        /** Handles client's request for showing map from current location */
        void HandleShowCurrentLocationL();

        /** Handles client's request for selecting a location from map */
        void HandleSelectFromMapL();

    protected: // from CMnServiceBase

        /** Reports that client cancelled request */
        void DoCancel();

    private:
    
        CPosLandmark*           iLandmark;
        CPeriodic*              iProcessor;
    };

#endif // MNTP_MAPVIEWSERVICE_H


