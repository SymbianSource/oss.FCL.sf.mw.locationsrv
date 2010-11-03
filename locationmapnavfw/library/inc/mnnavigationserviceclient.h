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
* Description:  RMnNavigationServiceClient class
*
*/


#ifndef MN_NAVIGATIONSERVICECLIENT_H
#define MN_NAVIGATIONSERVICECLIENT_H

#include <AknServerApp.h>

#include "mnserviceclientbase.h"

class CPosLandmark;

/** \internal */
NONSHARABLE_CLASS( RMnNavigationServiceClient ) : public RMnServiceClientBase
    {        
    public:
        /** C++ constructor */
        RMnNavigationServiceClient();
        /** C++ destructor */
        ~RMnNavigationServiceClient();
        
        void NavigateToL( const CPosLandmark& aDestinationLanmdark );

    private: // from RApaAppServiceBase
        TUid ServiceUid() const;
    };
    
#endif // MN_NAVIGATIONSERVICECLIENT_H
