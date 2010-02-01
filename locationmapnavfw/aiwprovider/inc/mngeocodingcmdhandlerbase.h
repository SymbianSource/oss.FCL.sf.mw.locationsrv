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
* Description:  CMnGeocodingCmdHandlerBase class declaration
*
*/


#ifndef MN_GEOCODINGCMDHANDLERBASE_H
#define MN_GEOCODINGCMDHANDLERBASE_H

#include "mnaiwcommandhandlerbase.h"

class CMnGeocoder;

/** \internal
 *  Base class for Geocoding-related AIW commands
 */
class CMnGeocodingCmdHandlerBase : public CMnAiwCommandHandlerBase
    {
    public:
        ~CMnGeocodingCmdHandlerBase();
        
    protected:
        CMnGeocodingCmdHandlerBase( CMnProvider& aProvider );
            
        void ConstructL( TRunMode aRunMode );
        
        CMnGeocoder& Geocoder();

        /** Reads contents of EGenericParamMnGeocodingOptions parameter */
        void ReadGeocodingOptionsL( 
            const TAiwGenericParam& aParam,
            TMnAiwGeocodingOptionsParam& aGeoOptions );

	public: // from CMnAiwCommandHandlerBase

        void SetExitObserverL( MMnProviderExitCallback& aExitCallback );

    protected: // from CMnAiwCommandHandlerBase
    
        void HandleAiwParamL( const TAiwGenericParam& aParam );
        void HandleAiwParamLandmarksL( RArray<TPtrC8>& aLandmarkDataList );
        void ResetParametersL();

    protected:
        TPtrC8          iLandmarkData;
            
    private:
        CMnGeocoder*    iGeocoder;            
    };

#endif // MN_GEOCODINGCMDHANDLERBASE_H
