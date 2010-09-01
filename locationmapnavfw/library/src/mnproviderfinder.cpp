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
* Description:  MnProviderFinder class implementation
*
*/


#include <e32base.h>
#include <apgcli.h>

#include "mnappserviceuids.hrh"

#include "mnproviderimpl.h"
#include "mnproviderfinder.h"

#include "mndebug.h"

// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
//  Cleanup item for RPointerArray<CMnProviderImpl>
// ---------------------------------------------------------------------------
//
void CleanupPointerArray(TAny* aArray)
    {
    ( static_cast<RPointerArray<CMnProviderImpl>*>( aArray ) )->ResetAndDestroy();
    }

// ---------------------------------------------------------------------------
//  Finds service UID for given CMnProvider::TService
// ---------------------------------------------------------------------------
//
TUid ServiceUid( CMnProvider::TService aService )
    {
    switch ( aService )
        {
        case CMnProvider::EServiceMapView:
            return TUid::Uid( KMnAppMapViewService );

        case CMnProvider::EServiceNavigation:
            return TUid::Uid( KMnAppNavigationService );

        case CMnProvider::EServiceGeocoding:
            return TUid::Uid( KMnAppGeocodingService );

        case CMnProvider::EServiceMapImage:
            return TUid::Uid( KMnAppMapImageService );

        default:
            return TUid::Uid( 0 );
        }
    }

// ---------------------------------------------------------------------------
//  Finds provider in given list by UID
// ---------------------------------------------------------------------------
//
TInt FindProviderByUid( TUid aAppUid, RPointerArray<CMnProvider>& aProviders )
    {
    for (TInt i = 0; i < aProviders.Count(); i++)
        {
        if ( aProviders[i]->Uid().iUid == aAppUid.iUid )
            return i;
        }
    return KErrNotFound;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void FindAllServiceProvidersL(
    RPointerArray<CMnProvider>& aProviders,
    CMnProvider::TServices aServices )
    {
    LOG("FindAllServiceProvidersL in");

    RApaLsSession apa;
    User::LeaveIfError( apa.Connect() );
    CleanupClosePushL( apa );

    Swi::RSisRegistrySession sis;
    User::LeaveIfError( sis.Connect() );
    CleanupClosePushL( sis );

    const CMnProvider::TService KKnownServices[] =
        {
        CMnProvider::EServiceMapView,
        CMnProvider::EServiceNavigation,
        CMnProvider::EServiceGeocoding,
        CMnProvider::EServiceMapImage
        };
    const TInt KNumKnownServices = sizeof( KKnownServices ) / sizeof ( CMnProvider::TService );

    // find all providers for known services
    for ( TInt i = 0; i < KNumKnownServices; i++ )
        {
        // but consider only required services
        if ( aServices & KKnownServices[i] )
            {
            TInt err = apa.GetServerApps( ServiceUid( KKnownServices[i] ) );
            while ( !err )
                {
                TApaAppInfo app;
                err = apa.GetNextApp(app);
                if ( !err )
                    {
                    // add this app to the list, if not added yet
                    TInt index = FindProviderByUid( app.iUid, aProviders );
                    if ( index == KErrNotFound )
                        {
                        // add new provider to the list
                        LOG2("found new provider candidate uid{%08X}, name{%S}", 
                            app.iUid.iUid, &app.iFullName);

                        // find all data about this provider
                        CMnProviderImpl* provider = NULL;
                        TRAP( err, provider = CMnProviderImpl::NewL( app.iUid, apa, sis ) );
                        if ( !err )
                            {
                            if ( provider->IsTrusted() )
                                {
                                CleanupStack::PushL( provider );
                                aProviders.AppendL( provider );
                                CleanupStack::Pop( provider );
                                }
                            else
                                {
                                delete provider;
                                LOG1("untrusted provider uid{%X}", app.iUid.iUid);
                                }
                            }
                        else
                            {
                            // skip provider if cannot be constructed
                            LOG2("construction failed for uid{%X}, error %d", app.iUid.iUid, err);
                            }
                        }
                    }
                }
            }
        }

    CleanupStack::PopAndDestroy( &sis );
    CleanupStack::PopAndDestroy( &apa );

    LOG("FindAllServiceProvidersL out");
    }

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void MnProviderFinder::FindProvidersL(
    RPointerArray<CMnProvider>& aProviders)
    {
    CMnProvider::TServices allServices =
        CMnProvider::EServiceMapView |
        CMnProvider::EServiceNavigation |
        CMnProvider::EServiceGeocoding |
        CMnProvider::EServiceMapImage;

    FindAllServiceProvidersL( aProviders, allServices );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void MnProviderFinder::FindProvidersL(
    RPointerArray<CMnProvider>& aProviders,
    CMnProvider::TServices aRequiredServices)
    {
    FindAllServiceProvidersL( aProviders, aRequiredServices );

    // verify that providers meet service support requirements
    for ( TInt i = aProviders.Count() - 1; i >= 0; i-- )
        {
        CMnProvider* provider = aProviders[i];
        
        if ( ( provider->SupportedServices() & aRequiredServices ) != aRequiredServices )
            {
            aProviders.Remove( i );
            delete provider;
            }
        }
    }

