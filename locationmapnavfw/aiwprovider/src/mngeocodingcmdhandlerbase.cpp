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
* Description:  CMnGeocodingCmdHandlerBase class implementation
*
*/


#include <AiwGenericParam.hrh>
#include <AiwCommon.hrh>

#include <mngeocoder.h>

#include "mnaiwdebug.h"
#include "mngeocodingcmdhandlerbase.h"

// ================= LOCAL FUNCTIONS =======================

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnGeocodingCmdHandlerBase::CMnGeocodingCmdHandlerBase( CMnProvider& aProvider )
:   CMnAiwCommandHandlerBase( aProvider )
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnGeocodingCmdHandlerBase::~CMnGeocodingCmdHandlerBase()
    {
    delete iGeocoder;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnGeocodingCmdHandlerBase::SetExitObserverL( 
	MMnProviderExitCallback& aExitCallback )
	{
	iGeocoder->SetExitObserverL( *this );
	CMnAiwCommandHandlerBase::SetExitObserverL( aExitCallback );
	}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnGeocodingCmdHandlerBase::ConstructL( TRunMode aRunMode )
    {
    CMnAiwCommandHandlerBase::ConstructL();
    
    if ( aRunMode == ERunModeChained )
        {
        iGeocoder = CMnGeocoder::NewChainedL( Provider() );
        }
    else
        {
        iGeocoder = CMnGeocoder::NewL( Provider() );
        }

    iRunMode = aRunMode;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnGeocodingCmdHandlerBase::ReadGeocodingOptionsL( 
    const TAiwGenericParam& aParam,
    TMnAiwGeocodingOptionsParam& aGeoOptions )
    {
    TPtrC8 data( aParam.Value().AsData() ); 
    if ( data.Size() == sizeof( TMnAiwGeocodingOptionsParam ) )
        {
        TMnAiwGeocodingOptionsParam options;
        TPckg<TMnAiwGeocodingOptionsParam> optionsPack( options );
        optionsPack.Copy( data );
        aGeoOptions = options;
        }
    else
        {
        User::Leave( KErrArgument );
        }
    }
    
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnGeocoder& CMnGeocodingCmdHandlerBase::Geocoder()
    {
    return *iGeocoder;
    }
    
// -----------------------------------------------------------------------------
//  From class CMnAiwCommandHandlerBase
// -----------------------------------------------------------------------------
//
void CMnGeocodingCmdHandlerBase::HandleAiwParamL( const TAiwGenericParam& aParam )
    {
    switch ( aParam.SemanticId() )
        {
        case EGenericParamMnGeocodingOptions:
            //LOG("CMnGeocodingCmdHandlerBase::HandleAiwParam, GeocodingOptions");
            {
            TMnAiwGeocodingOptionsParam geoOptions;
            ReadGeocodingOptionsL( aParam, geoOptions );
            iGeocoder->SetOptions( ( CMnGeocoder::TOptions ) geoOptions.iOptions );
            break;
            }
        default:
            CMnAiwCommandHandlerBase::HandleAiwParamL( aParam );
            break;
        }
    }
    
// -----------------------------------------------------------------------------
//  From class CMnAiwCommandHandlerBase
// -----------------------------------------------------------------------------
//
void CMnGeocodingCmdHandlerBase::HandleAiwParamLandmarksL( RArray<TPtrC8>& aLandmarkDataList )
    {
    // if already set or not single landmarks are given
    if ( iLandmarkData.Size() || aLandmarkDataList.Count() != 1 )
        {
        User::Leave( KErrArgument );
        }
    iLandmarkData.Set( aLandmarkDataList[0] );
    }    

// -----------------------------------------------------------------------------
//  From class CMnAiwCommandHandlerBase
// -----------------------------------------------------------------------------
//
void CMnGeocodingCmdHandlerBase::ResetParametersL()
    {
    iGeocoder->SetOptions( CMnGeocoder::EOptionNone );
    iLandmarkData.Set( NULL, 0 );
    }
