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
* Description:  CMnProviderImpl class
*
*/


#ifndef MN_PROVIDER_IMPL_H
#define MN_PROVIDER_IMPL_H

#include <swi/sisregistrysession.h>
#include "mnprovider.h"

class RMnServiceClientBase;
class RApaLsSession;
   
/*  @brief Implementation of CMnProvider.
 *  @internal
 */
NONSHARABLE_CLASS( CMnProviderImpl ) : public CMnProvider
    {
    public :
        ~CMnProviderImpl();
        
        static CMnProviderImpl* NewL( TUid aProvider );
        static CMnProviderImpl* NewL( 
            TUid aAppUid, 
            RApaLsSession& aApaSession, 
            Swi::RSisRegistrySession& aSisSession );
        
        /** This constructor reads all provider's info from 
         *  AppArc and Swi */
        void ConstructL( TUid aAppUid );
        
        /** Performance-aware constructor. Uses ready connections to 
         *  AppArc and Swi */
        void ConstructL(
            TUid aAppUid, 
            RApaLsSession& aApaSession, 
            Swi::RSisRegistrySession& aSisSession );
            
        /** Returns trust info of provider's sis package */            
        TBool IsTrusted();    

    public : // From CMnProvider        

        TUint32 SupportedFeatures( TService aService ) const;

    protected :
        CMnProviderImpl();
        
        /** Resembles MN_SERVICE_INFO resource structure */
        struct TMnAppServiceInfo
            {
            /* Supported features of this service */
            TInt32 iServiceFeatures;
            };

        /** Reads application info from AppArc */
        void ReadAppArcInfoL( RApaLsSession& aApaSession );

        /** Looks for MN_SERVICE_INFO struct in application's SERVICE_INFO opaque data. */
        TBool ReadMnServiceInfoL(
            RApaLsSession& aApaSession, 
            TUid aServiceUid, 
            TMnAppServiceInfo& aServiceInfo );

        /** Checks service features */
        void CheckServiceFeaturesL( 
            RApaLsSession& aApaSession,
            CMnProvider::TService aService, 
            TUint32& aServiceFeatures );

        /** Reads extended service info from SERVICE_INFO struct */
        void ReadServiceFeaturesL( RApaLsSession& aApaSession );

        /** Reads package info from Installation Registry 
         *  and verifies trust level */
        void ReadPackageInfoL( Swi::RSisRegistrySession& aSisSession );
        
        /** Verifies if converter plug-in exists for provider of MapImage service */
        TBool IsConverterPluginFoundL();

        /** \brief Sets Provider Application's name */
        void SetNameL( const TDesC& aName );

        /** \brief Sets Provider Application's short name */
        void SetShortNameL( const TDesC& aShortName );

        /** \brief Sets Provider Application's vendor name */
        void SetVendorNameL( const TDesC& aVendorName );

        /** \brief Sets Provider Application's version */
        void SetVersion( TVersion aVersion );

        /** \brief Sets Provider Application's supported services */
        void SetSupportedServices( TServices aServices );
        
    protected :
        TBool   iIsTrusted;
        TUint32 iMapViewFeatures;
        TUint32 iNavigationFeatures;
        TUint32 iGeocodingFeatures;
        TUint32 iMapImageFeatures;
    };

#endif // MN_PROVIDER_IMPL_H

