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
* Description:  CMnNavigateToHandler class implementation
*
*/


#include <AiwGenericParam.hrh>
#include <AiwCommon.hrh>

#include <EPos_CPosLandmark.h>
#include <epos_poslandmarkserialization.h>

#include <mnnavigator.h>

#include "mnaiwdebug.h"
#include "mnaiwinternal.h"
#include "mnnavigatetohandler.h"

// ================= LOCAL FUNCTIONS =======================

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnNavigateToHandler::CMnNavigateToHandler( CMnProvider& aProvider )
:   CMnAiwCommandHandlerBase( aProvider )
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnNavigateToHandler::~CMnNavigateToHandler()
    {
    delete iNavigator;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnNavigateToHandler* CMnNavigateToHandler::NewL(
    CMnProvider& aProvider,
    TRunMode aRunMode )
    {
    CMnNavigateToHandler* self = 
        new (ELeave) CMnNavigateToHandler( aProvider );
    CleanupStack::PushL( self );        
    self->ConstructL( aRunMode );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnNavigateToHandler::SetExitObserverL( 
	MMnProviderExitCallback& aExitCallback )
	{
	iNavigator->SetExitObserverL( *this );
	CMnAiwCommandHandlerBase::SetExitObserverL( aExitCallback );
	}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnNavigateToHandler::ConstructL( TRunMode aRunMode )
    {
    LOG("CMnNavigateToHandler::ConstructL in");

    CMnAiwCommandHandlerBase::ConstructL();
    
    if ( aRunMode == ERunModeChained )
        {
        iNavigator = CMnNavigator::NewChainedL( Provider() );
        }
    else
        {
        iNavigator = CMnNavigator::NewL( Provider() );
        }
        
    iRunMode = aRunMode;
    
    LOG("CMnNavigateToHandler::ConstructL out");
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnNavigateToHandler::DoStartL()
    {
    LOG("CMnNavigateToHandler::DoStartL in");
    if ( iLandmarkData.Size() )
        {
        CPosLandmark* lm = PosLandmarkSerialization::UnpackL( iLandmarkData );
        CleanupStack::PushL( lm );
        
        iNavigator->NavigateToL( *lm );
        
        CleanupStack::PopAndDestroy( lm );
        }
    else    
        {
        User::Leave( KErrArgument );
        }
        
    LOG("CMnNavigateToHandler::DoStartL out");
    }

// -----------------------------------------------------------------------------
//  From class CMnAiwCommandHandlerBase
// -----------------------------------------------------------------------------
//
void CMnNavigateToHandler::HandleAiwParamLandmarksL( RArray<TPtrC8>& aLandmarkDataList )
    {
    __ASSERT_DEBUG( aLandmarkDataList.Count() > 0, Panic( KErrGeneral ) );
    
    // if already set or not single landmarks are given
    if ( !iLandmarkData.Length() )
        {
        iLandmarkData.Set( aLandmarkDataList[0] );
        }
    }

// -----------------------------------------------------------------------------
//  From class CMnAiwCommandHandlerBase
// -----------------------------------------------------------------------------
//
void CMnNavigateToHandler::ResetParametersL()
    {
    iLandmarkData.Set( NULL, 0 );
    }
