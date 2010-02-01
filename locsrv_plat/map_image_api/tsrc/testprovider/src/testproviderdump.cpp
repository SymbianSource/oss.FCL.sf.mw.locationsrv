/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Utility class for map and navigation test provider
*
*/


#include <s32file.h> 
#include <e32property.h> 

#include <coemain.h>
#include <aknlists.h>
#include <barsread2.h>
#include <barsc2.h> 

#include <lbsposition.h>
#include <lbspositioninfo.h>

#include <aiwgenericparam.h>

#include <epos_cposlandmark.h>
#include <epos_cposlandmarkdatabase.h>
#include <epos_poslandmarkserialization.h>

#include <mngeocoder.h>
#include <mnmapview.h>
#include <mnnavigator.h>

#include <mnaiwservices.h>

#include <mntestprovider.rsg>

#include "debug.h"
#include "testproviderdump.h"

_LIT( KDumpFileName, "c:\\mntestprovider.dump");

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CTestProviderDump::CTestProviderDump()
    { 
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CTestProviderDump::~CTestProviderDump()
    {
    delete iList;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CTestProviderDump* CTestProviderDump::NewLC()
    {
    CTestProviderDump* self = new (ELeave) CTestProviderDump();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CTestProviderDump::ConstructL()
    {
    iList = CAiwGenericParamList::NewL();
    AddTimeStampL();    
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CTestProviderDump::AddTimeStampL()
    {
    TTime now;
    now.UniversalTime();

    TAiwGenericParam param( EGenericParamDateTime, TAiwVariant( now ) );
    iList->AppendL( param );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CTestProviderDump::AddRunModeL( TBool aIsChainedMode )
    {
    TMnAiwCommonOptionsParam options;
    options.iRunChained = aIsChainedMode;

    TPckg<TMnAiwCommonOptionsParam> pack( options );
    
    TAiwGenericParam param( EGenericParamMnCommonOptions, TAiwVariant( pack ) );
    iList->AppendL( param );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CTestProviderDump::AddLandmarkL( const CPosLandmark& aLandmark )
    {
    HBufC8* lmBuf = PosLandmarkSerialization::PackL( aLandmark );
    CleanupStack::PushL( lmBuf );

    TAiwGenericParam param( EGenericParamLandmark, TAiwVariant( *lmBuf ) );
    iList->AppendL( param );

    CleanupStack::PopAndDestroy( lmBuf );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CTestProviderDump::AddLandmarkL( const TDesC& aUri, TPosLmItemId aId )
    {
    TAiwGenericParam uri( EGenericParamLandmarkDatabase, TAiwVariant( aUri ) );
    iList->AppendL( uri );

    TAiwGenericParam id( EGenericParamLandmarkId, TAiwVariant( aId ) );
    iList->AppendL( id );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CTestProviderDump::AddLandmarkListL( const TDesC& aUri, TArray<TPosLmItemId> aItemIds )
    {
    TAiwGenericParam param( EGenericParamLandmarkDatabase, TAiwVariant( aUri ) );
    iList->AppendL( param );

    for ( TInt i = 0; i < aItemIds.Count(); i++ )
        {
        TAiwGenericParam param( EGenericParamLandmarkId, TAiwVariant( aItemIds[i] ) );
        iList->AppendL( param );
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CTestProviderDump::AddMapViewOptionsL( 
    TUint32 aOptions,
    TUint32 aCurrentLocationOption,
    TReal aRadius,
    TCoordinate aCenterPoint )
    {
    TMnAiwMapViewOptionsParam options;
    options.iOptions = aOptions;
    options.iCurrentLocationOption = aCurrentLocationOption;
    options.iRadius = aRadius;
    options.iCenterPoint = aCenterPoint;
    
    TPckg<TMnAiwMapViewOptionsParam> pack( options );
    
    TAiwGenericParam param( EGenericParamMnMapViewOptions, TAiwVariant( pack ) );
    iList->AppendL( param );
    }
    
#ifdef RD_MAPNAV_BITMAP_ACCESS
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CTestProviderDump::AddMapImageOptionsL( TMapImageOptions& aOptions )
    {
    TPckg<TMapImageOptions> pack( aOptions );
    TAiwGenericParam param( EGenericParamUnspecified, TAiwVariant( pack ) );
    iList->AppendL( param );
    }    
#endif

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CTestProviderDump::AddRequestTextL( const TDesC& aText )
    {
    TAiwGenericParam text( EGenericParamRequestText, TAiwVariant( aText ) );
    iList->AppendL( text );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CTestProviderDump::AddGeocodingOptionsL( TUint32 aOptions )
    {
    TMnAiwGeocodingOptionsParam options;
    options.iOptions = aOptions;
    
    TPckg<TMnAiwGeocodingOptionsParam> pack( options );
    
    TAiwGenericParam param( EGenericParamMnGeocodingOptions, TAiwVariant( pack ) );
    iList->AppendL( param );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CTestProviderDump::AddAddressL( const TDesC& aAddress )
    {
    TAiwGenericParam address( EGenericParamRequestText, TAiwVariant( aAddress ) );
    iList->AppendL( address );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CTestProviderDump::CommitL()
    {
    RFs& fs = CEikonEnv::Static()->FsSession();
    
    RFileWriteStream stream;
    TInt err = KErrInUse; 
    TInt attempts = 5;
    while ( err == KErrInUse && --attempts >= 0 )
        {
        err = stream.Replace( fs, KDumpFileName, EFileStream | EFileWrite );
        if ( err && attempts >= 0 )
            {
            User::After( 100*1000 ); // 0.1 sec
            }
        }
    if ( err )
        {
        LOG1("Couldn't open dump file, error %d", err);
        User::Leave( err );
        }
    CleanupClosePushL( stream );
    
    iList->ExternalizeL( stream );
    
    CleanupStack::PopAndDestroy( &stream );
    }
