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
* Description:  CTestProviderNavigationService class
*
*/


#ifndef MNTP_NAVIGATIONSERVICE_H
#define MNTP_NAVIGATIONSERVICE_H

#include "mnnavigationservicebase.h"

class CPosLandmark;

/** Implementation of Navigation service (KMnAppNavigationService). 
 */
class CTestProviderNavigationService : public CMnNavigationServiceBase
    {
    public :
        static CTestProviderNavigationService* NewL();
        
        void HandleTestCommandL( TInt aCommand );

    protected :
        /** C++ constructor */
        CTestProviderNavigationService();
        /** Destructor */
        virtual ~CTestProviderNavigationService();

        void ConstructL();

    public: // from CMnNavigationServiceBase
    
        /** Handles client's request for navigation. */
        void HandleNavigateToL(const CPosLandmark& aDestination);
    
    protected: // from CMnServiceBase

        /** Reports that client cancelled request */
        void DoCancel();

    private:
    
    };

#endif // MNTP_NAVIGATIONSERVICE_H


