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
* Description:  CMnProviderImpl class implementation
*
*/


#include <e32base.h>
#include <ecom/ecom.h>
#ifndef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <apaid.h>
#else
#include <apaid.h>
#include <apaidpartner.h>
#endif
#include <apgcli.h>
#include <swi/sisregistryentry.h>
#include <swi/sistruststatus.h>
#include <swi/sisregistrypackage.h>

#include "mnserviceclientbase.h"
#include "mnappserviceuids.hrh"

#include "mnpluginuids.hrh"
#include "mnproviderimpl.h"

#include "mndebug.h"

// ======== LOCAL FUNCTIONS ========

extern TUid ServiceUid( CMnProvider::TService aService );

// ---------------------------------------------------------------------------
//  Cleanup item for RImplInfoPtrArray
// ---------------------------------------------------------------------------
//
void CleanupImplementationsArray( TAny* aArray )
    {
    ( static_cast<RImplInfoPtrArray*>( aArray ) )->ResetAndDestroy();
    }

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
CMnProviderImpl::CMnProviderImpl()
    {
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
CMnProviderImpl::~CMnProviderImpl()
    {
    delete iName;
    delete iShortName;
    delete iVendorName;
    REComSession::FinalClose();    
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
CMnProviderImpl* CMnProviderImpl::NewL( TUid aProvider )
    {
    CMnProviderImpl* self = new(ELeave) CMnProviderImpl;
    CleanupStack::PushL( self );
    self->ConstructL( aProvider );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
CMnProviderImpl* CMnProviderImpl::NewL(
    TUid aAppUid, 
    RApaLsSession& aApaSession, 
    Swi::RSisRegistrySession& aSisSession )
    {
    CMnProviderImpl* self = new(ELeave) CMnProviderImpl;
    CleanupStack::PushL( self );
    self->ConstructL( aAppUid, aApaSession, aSisSession );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnProviderImpl::ConstructL( TUid aProvider )
    {
    RApaLsSession apa;
    User::LeaveIfError( apa.Connect() );
    CleanupClosePushL( apa );

    Swi::RSisRegistrySession sis;
    User::LeaveIfError( sis.Connect() );
    CleanupClosePushL( sis );

    ConstructL( aProvider, apa, sis );
    
    CleanupStack::PopAndDestroy( &sis );
    CleanupStack::PopAndDestroy( &apa );
    }
    
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnProviderImpl::ConstructL(
    TUid aAppUid, 
    RApaLsSession& aApaSession, 
    Swi::RSisRegistrySession& aSisSession )
    {
    iUid = aAppUid;
    ReadAppArcInfoL( aApaSession );
    ReadServiceFeaturesL( aApaSession );
    ReadPackageInfoL( aSisSession );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnProviderImpl::ReadAppArcInfoL( RApaLsSession& aApa )
    {
    LOG("CMnProviderImpl::ReadAppArcInfoL");
    
    TApaAppInfo appInfo;
    User::LeaveIfError( aApa.GetAppInfo( appInfo, iUid ) );
    
    SetNameL( appInfo.iCaption );
    SetShortNameL( appInfo.iShortCaption );
    
    // get additional data from SERVICE_INFO structs
    
    const TInt KGranularity = 8;
    CArrayFixFlat< TUid >* serviceUids = 
        new (ELeave) CArrayFixFlat< TUid > ( KGranularity );
    CleanupStack::PushL( serviceUids );
        
    aApa.GetAppServicesL( iUid, *serviceUids );
    
    for ( TInt s = 0; s < serviceUids->Count(); s++ )
        {
        switch ( (*serviceUids)[s].iUid )
            {
            case KMnAppMapViewService:
                iServices |= EServiceMapView;
                break;
            case KMnAppNavigationService:
                iServices |= EServiceNavigation;
                break;
            case KMnAppGeocodingService:
                iServices |= EServiceGeocoding;
                break;
            case KMnAppMapImageService:
                iServices |= EServiceMapImage;
                break;
            }
        }

    CleanupStack::PopAndDestroy( serviceUids );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
TBool CMnProviderImpl::ReadMnServiceInfoL(
    RApaLsSession& aApaSession, 
    TUid aServiceUid, 
    TMnAppServiceInfo& aServiceInfo)
    {
    LOG1("CMnProviderImpl::ReadMnServiceInfoL appSvcUid 0x%X", aServiceUid.iUid);

    TBool found = EFalse;

    CApaAppServiceInfoArray* infoArray = aApaSession.GetAppServiceOpaqueDataLC( iUid, aServiceUid );
    if ( infoArray )
        {
        TArray< TApaAppServiceInfo > array = infoArray->Array();
        for ( TInt i = 0; i < array.Count(); i++ )
            {
            TPtrC8 ptr = array[i].OpaqueData();
            
            if ( ptr.Size() == sizeof ( TMnAppServiceInfo ) )
                {
                memcpy(&aServiceInfo, ptr.Ptr(), sizeof ( TMnAppServiceInfo ) );
                found = ETrue;
                break;
                }
            }
        }

    CleanupStack::PopAndDestroy( infoArray );            
    return found;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnProviderImpl::CheckServiceFeaturesL( 
    RApaLsSession& aApaSession,
    CMnProvider::TService aService, 
    TUint32& aServiceFeatures )
    {
    TMnAppServiceInfo info;
    TBool found = ReadMnServiceInfoL( aApaSession, ServiceUid( aService ), info );
    if ( found && info.iServiceFeatures )
        {
        aServiceFeatures = info.iServiceFeatures;
        }
    else
        {
        iServices &= ~aService;
        }
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnProviderImpl::ReadServiceFeaturesL( RApaLsSession& aApaSession )
    {
    if ( iServices & EServiceMapView )
        {
        CheckServiceFeaturesL( aApaSession, EServiceMapView, iMapViewFeatures );
        }

    if ( iServices & EServiceNavigation )
        {
        CheckServiceFeaturesL( aApaSession, EServiceNavigation, iNavigationFeatures );
        }

    if ( iServices & EServiceGeocoding )
        {
        CheckServiceFeaturesL( aApaSession, EServiceGeocoding, iGeocodingFeatures );
        }

    if ( iServices & EServiceMapImage )
        {
        CheckServiceFeaturesL( aApaSession, EServiceMapImage, iMapImageFeatures );
        if ( !IsConverterPluginFoundL() )
            {
            iServices &= ~EServiceMapImage;
            iMapImageFeatures = 0;
            }
        }
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
#ifndef __WINS__

void CMnProviderImpl::ReadPackageInfoL( Swi::RSisRegistrySession& aSisSession )
    {
    LOG("CMnProviderImpl::ReadPackageInfoL");

    Swi::CSisRegistryPackage* package = aSisSession.SidToPackageL(iUid);
    CleanupStack::PushL( package );
    
    Swi::RSisRegistryEntry entry;
    User::LeaveIfError( entry.OpenL( aSisSession, *package ) );
    CleanupClosePushL( entry );
    
    // FW wants to know trust level of Provider Applications
    Swi::TSisTrustStatus trust = entry.TrustStatusL();
    iIsTrusted = ( trust.ValidationStatus() >= Swi::EValidatedToAnchor );
        
    // other needed info
    SetVersion( entry.VersionL() );
    
    HBufC* vendorName = entry.LocalizedVendorNameL();
    CleanupStack::PushL( vendorName );
    SetVendorNameL( vendorName->Des() );
    CleanupStack::PopAndDestroy( vendorName );

    CleanupStack::PopAndDestroy( &entry );
    CleanupStack::PopAndDestroy( package );
    }

#else

void CMnProviderImpl::ReadPackageInfoL( Swi::RSisRegistrySession& /*aSisSession */ )
    {
    LOG("CMnProviderImpl::ReadPackageInfoL");
    iIsTrusted = ETrue;
    }

#endif

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
TBool CMnProviderImpl::IsTrusted()
    {
    return iIsTrusted;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
TBool CMnProviderImpl::IsConverterPluginFoundL()
    {
    TUid converterIf = { KMnMapImageConverterIf };

    const TInt KNumUidHexDigits = 8;
    TBuf8<KNumUidHexDigits> uidText8;
    
    _LIT8( KUidHexText, "%08X" );
    uidText8.Format( KUidHexText, iUid );

    TEComResolverParams resolverParams;
    resolverParams.SetDataType( uidText8 );

    RImplInfoPtrArray implementations;
    CleanupStack::PushL( TCleanupItem( CleanupImplementationsArray, &implementations ) );
    
    REComSession::ListImplementationsL( converterIf, resolverParams, implementations );
    TBool found = ( implementations.Count() > 0 );
    CleanupStack::PopAndDestroy(); // cleanup item    
    return found;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnProviderImpl::SetNameL( const TDesC& aName )
    {
    __ASSERT_DEBUG( !iName, Panic( KErrGeneral ) );
    iName = aName.AllocL();
    }
    
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnProviderImpl::SetShortNameL( const TDesC& aShortName )
    {
    __ASSERT_DEBUG( !iShortName, Panic( KErrGeneral ) );
    iShortName = aShortName.AllocL();
    }
    
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnProviderImpl::SetVendorNameL( const TDesC& aVendorName )
    {
    __ASSERT_DEBUG( !iVendorName, Panic( KErrGeneral ) );
    iVendorName = aVendorName.AllocL();
    }
    
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnProviderImpl::SetVersion( TVersion aVersion )
    {
    iVersion = aVersion;
    }
    
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnProviderImpl::SetSupportedServices( TServices aServices )
    {
    iServices = aServices;
    }
    
// ---------------------------------------------------------------------------
// From class CMnProvider.
// ---------------------------------------------------------------------------
//
TUint32 CMnProviderImpl::SupportedFeatures( TService aService ) const
    {
    switch ( aService )
        {
        case EServiceMapView:
            return iMapViewFeatures;
            
        case EServiceNavigation:
            return iNavigationFeatures;
            
        case EServiceGeocoding:
            return iGeocodingFeatures;

        case EServiceMapImage:
            return iMapImageFeatures;

        default:
            return 0;
        }
    }
