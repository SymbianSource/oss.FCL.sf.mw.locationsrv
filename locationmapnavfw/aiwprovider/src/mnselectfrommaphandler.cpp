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
* Description:  CMnSelectFromMapHandler class implementation
*
*/


#include <e32base.h>

#include <AiwGenericParam.hrh>
#include <AiwCommon.hrh>

#include <EPos_CPosLandmark.h>

#include <mnmapview.h>

#include "mnaiwdebug.h"
#include "mnaiwinternal.h"
#include "mnselectfrommaphandler.h"

// ================= LOCAL FUNCTIONS =======================

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnSelectFromMapHandler::CMnSelectFromMapHandler( CMnProvider& aProvider )
:   CMnMapViewCmdHandlerBase( aProvider )
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnSelectFromMapHandler* CMnSelectFromMapHandler::NewL( 
    CMnProvider& aProvider,
    TRunMode aRunMode )
    {
    CMnSelectFromMapHandler* self = 
        new (ELeave) CMnSelectFromMapHandler( aProvider );
    CleanupStack::PushL( self );        
    self->ConstructL( aRunMode );
    CleanupStack::Pop( self );        
    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnSelectFromMapHandler::ConstructL( TRunMode aRunMode )
    {
    LOG("CMnSelectFromMapHandler::ConstructL in");

    CMnMapViewCmdHandlerBase::ConstructL( aRunMode );

    LOG("CMnSelectFromMapHandler::ConstructL out");
    }

// ================= INTERNAL FUNCTIONS =======================

// -----------------------------------------------------------------------------
//  From class CMnAiwCommandHandlerBase
// -----------------------------------------------------------------------------
//
void CMnSelectFromMapHandler::DoStartL()
    {
    LOG("CMnSelectFromMapHandler::DoStartL in");
    
    MapView().SelectFromMapL( iStatus );
    
    LOG("CMnSelectFromMapHandler::DoStartL out");
    }

// -----------------------------------------------------------------------------
//  From class CActive
// -----------------------------------------------------------------------------
//
void CMnSelectFromMapHandler::RunL()
    {
    LOG("CMnSelectFromMapHandler::RunL in");
    
    TInt result = iStatus.Int();
    if ( result == KErrNone )
        {
        // read selection result and put to AIW
        switch ( MapView().SelectionResultType() )
            {
            case CMnMapView::ESelectionFreeLandmark:
                LOG("CMnSelectFromMapHandler::RunL selected free");
                {
                CPosLandmark* lm = MapView().RetrieveSelectionResultL();
                CleanupStack::PushL( lm );
                WriteLandmarkL( *lm );
                CleanupStack::PopAndDestroy( lm );
                }
                break;
                
            case CMnMapView::ESelectionLandmarkIndex:
                LOG("CMnSelectFromMapHandler::RunL selected index");
                {
                TInt landmarkIndex( KErrNotFound );
                MapView().RetrieveSelectionResultL( landmarkIndex );
                LOG1("CMnSelectFromMapHandler::RunL selected index %d", landmarkIndex);

                // find this landmark in input
                TAiwGenericParam param;
                if ( FindLandmarkData( landmarkIndex, param ) )
                    {
                    // give it back to client
                    AiwParameters().OutList().AppendL( param );
                    }
                else    
                    {
                    result = KErrNotFound;
                    }
                }
                break;
                
            case CMnMapView::ESelectionLinkedLandmark:
                LOG("CMnSelectFromMapHandler::RunL selected linked");
                {
                TPosLmItemId lmId( KPosLmNullItemId );
                HBufC* dbUri = NULL;
                MapView().RetrieveSelectionResultL( lmId, dbUri );
                CleanupStack::PushL( dbUri );
                WriteLandmarkL( lmId, *dbUri );
                CleanupStack::PopAndDestroy( dbUri );
                }
                break;
                
            default:
                // Unknown result type
                break;
            }
        }
    Done( result );
    LOG("CMnSelectFromMapHandler::RunL out");
    }
    
// -----------------------------------------------------------------------------
//  From class CActive
// -----------------------------------------------------------------------------
//
void CMnSelectFromMapHandler::DoCancel()
    {
    MapView().Cancel();
    }

// -----------------------------------------------------------------------------
//  From class CMnAiwCommandHandlerBase
// -----------------------------------------------------------------------------
//
void CMnSelectFromMapHandler::HandleAiwParamL( const TAiwGenericParam& aParam )
    {
    switch ( aParam.SemanticId() )
        {
        case EGenericParamRequestText:
            if ( aParam.Value().TypeId() == EVariantTypeDesC )
                {
                iMapView->SetCustomRequestTextL( aParam.Value().AsDes() );
                }
            else
                {
                User::Leave( KErrArgument );
                }
            break;

        default:
            CMnMapViewCmdHandlerBase::HandleAiwParamL( aParam );
            break;
        }
    }
