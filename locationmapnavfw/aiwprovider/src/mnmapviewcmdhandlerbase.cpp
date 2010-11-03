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
* Description:  CMnMapViewCmdHandlerBase class implementation
*
*/


#include <AiwGenericParam.hrh>
#include <AiwCommon.hrh>

#include <mnmapview.h>

#include "mnaiwdebug.h"
#include "mnmapviewcmdhandlerbase.h"

// ================= LOCAL FUNCTIONS =======================

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnMapViewCmdHandlerBase::CMnMapViewCmdHandlerBase( CMnProvider& aProvider )
:   CMnAiwCommandHandlerBase( aProvider )
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnMapViewCmdHandlerBase::~CMnMapViewCmdHandlerBase()
    {
    delete iMapView;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnMapViewCmdHandlerBase::SetExitObserverL( MMnProviderExitCallback& aExitCallback )
	{
	iMapView->SetExitObserverL( *this );
	CMnAiwCommandHandlerBase::SetExitObserverL( aExitCallback );
	}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnMapViewCmdHandlerBase::ConstructL( TRunMode aRunMode )
    {
    LOG("CMnMapViewCmdHandlerBase::ConstructL in");
    CMnAiwCommandHandlerBase::ConstructL();
    
    if ( aRunMode == ERunModeChained )
        {
        iMapView = CMnMapView::NewChainedL( Provider() );
        }
    else
        {
        iMapView = CMnMapView::NewL( Provider() );
        }

    iRunMode = aRunMode;

    LOG("CMnMapViewCmdHandlerBase::ConstructL out");
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnMapViewCmdHandlerBase::ReadMapOptionsL( 
    const TAiwGenericParam& aParam,
    TMnAiwMapViewOptionsParam& aMapOptions )
    {
    TPtrC8 data( aParam.Value().AsData() ); 
    if ( data.Size() == sizeof( TMnAiwMapViewOptionsParam ) )
        {
        TMnAiwMapViewOptionsParam options;
        TPckg<TMnAiwMapViewOptionsParam> optionsPack( options );
        optionsPack.Copy( data );
        aMapOptions = options;
        }
    else
        {
        User::Leave( KErrArgument );
        }
    }
    
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnMapView& CMnMapViewCmdHandlerBase::MapView()
    {
    return *iMapView;
    }
    
// -----------------------------------------------------------------------------
//  From class CMnAiwCommandHandlerBase
// -----------------------------------------------------------------------------
//
void CMnMapViewCmdHandlerBase::HandleAiwParamL( const TAiwGenericParam& aParam )
    {
    switch ( aParam.SemanticId() )
        {
        case EGenericParamMnMapViewOptions:
            {
            TMnAiwMapViewOptionsParam mapOptions;
            ReadMapOptionsL( aParam, mapOptions );

            iMapView->SetOptions( mapOptions.iOptions );
            iMapView->SetMapAreaL( mapOptions.iCenterPoint, mapOptions.iRadius );
            iMapView->SetCurrentLocationOption( 
                ( CMnMapView::TCurrentLocationOption) mapOptions.iCurrentLocationOption );

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
void CMnMapViewCmdHandlerBase::HandleAiwParamLandmarksL( RArray<TPtrC8>& aLandmarkDataList )
    {
    iMapView->AddLandmarksToShowL( aLandmarkDataList );
    }
    
// -----------------------------------------------------------------------------
//  From class CMnAiwCommandHandlerBase
// -----------------------------------------------------------------------------
//
void CMnMapViewCmdHandlerBase::HandleAiwParamLandmarksListL( 
    const TDesC& aDbUri, 
    RArray<TPosLmItemId>& aLmIds )
    {
    iMapView->AddLandmarksToShowL( aDbUri, aLmIds );
    }

// -----------------------------------------------------------------------------
//  From class CMnAiwCommandHandlerBase
// -----------------------------------------------------------------------------
//
void CMnMapViewCmdHandlerBase::ResetParametersL()
    {
    iMapView->ResetLandmarksToShow();
    iMapView->ResetMapArea();
    iMapView->SetUseDefaultRequestText();
    iMapView->SetOptions( CMnMapView::EOptionNone );
    iMapView->SetCurrentLocationOption( CMnMapView::ECurrentLocationDisabled );
    }
