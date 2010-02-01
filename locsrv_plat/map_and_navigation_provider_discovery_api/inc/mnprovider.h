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
* Description:  CMnProvider class
*
*/


#ifndef MN_PROVIDER_H
#define MN_PROVIDER_H

#include <e32base.h>

/** Represents information about Provider Application.
 *  
 *  Use class @ref MnProviderFinder to find all provider applications.
 *
 *  @since 3.1
 *  @lib mnclientlib.dll
 *  @ingroup MnProviderDiscoveryAPI
 */   
class CMnProvider : public CBase
    {
    public :
        /** Type for bitmap of @ref TService values. */
        typedef TUint32 TServices;
        
        /** Map and Navigation application services */
        enum TService
            {
            /** Uninitialized value */
            EServiceNone            = 0x00,
            /** Defines that Provider Application supports
             *  CMnMapView */
            EServiceMapView         = 0x01,
            /** Defines that Provider Application supports
             *  CMnNavigator */
            EServiceNavigation      = 0x02,
            /** Defines that Provider Application supports
             *  CMnGeocoder */
            EServiceGeocoding       = 0x04,
            /** Defines that Provider Application supports 
             *  requests for map images */
            EServiceMapImage        = 0x08
            };
    
        /** Destructor */
        IMPORT_C ~CMnProvider();
    
        /** Returns UID of Provider Application */
        IMPORT_C TUid Uid() const;
        
        /** Returns version information of Provider Application */
        IMPORT_C TVersion Version() const;
        
        /** Returns localized name of this Provider Application.
         *
         *  Displayable name is specified in application registration file.
         *  (See APP_REGISTRATION_INFO for details).
         *  @param[out] aName Displayable name of Provider Application
         */
        IMPORT_C void GetName( TPtrC& aName ) const;
        
        /** Returns short localized name of this Provider Application.
         *
         *  Short name is specified in its application registration file.
         *  (See APP_REGISTRATION_INFO for details).
         *  @param[out] aShortName Short displayable name of Provider Application
         */
        IMPORT_C void GetShortName( TPtrC& aShortName ) const;
        
        /** Returns localized name of Provider Application's vendor. 
         *
         *  @param[out] aVendorName Vendor name of this Provider Application
         */
        IMPORT_C void GetVendorName( TPtrC& aVendorName ) const;
        
        /** Returns what map and navigation related services 
         *  are supported by this Provider Application. */
        IMPORT_C TServices SupportedServices() const;

        /* Returns what service features
         *  are supported by this Provider Application. 
         *  Service features are defined in MnAppServices.hrh.
         */
        virtual TUint32 SupportedFeatures( TService aService ) const;

    protected :
        CMnProvider();
        CMnProvider( CMnProvider& );

    protected :
        TUid iUid;
        HBufC* iName;
        HBufC* iShortName;
        HBufC* iVendorName;
        TServices iServices;
        TVersion iVersion;
    };

#endif // MN_PROVIDER_H
