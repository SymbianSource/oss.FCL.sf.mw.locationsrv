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
* Description:  CMnShowOnMapHandler class implementation
*
*/


#include <AiwGenericParam.hrh>
#include <AiwCommon.hrh>

#include <mnmapview.h>
#include <mnprovider.h>

#include "mnaiwdebug.h"
#include "mnaiwinternal.h"
#include "mnshowonmaphandler.h"

// ================= LOCAL FUNCTIONS =======================

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnShowOnMapHandler::CMnShowOnMapHandler( CMnProvider& aProvider )
:   CMnMapViewCmdHandlerBase( aProvider  )
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnShowOnMapHandler* CMnShowOnMapHandler::NewL( 
    CMnProvider& aProvider, TRunMode aRunMode )
    {
    CMnShowOnMapHandler* self = new (ELeave) CMnShowOnMapHandler( aProvider );
    CleanupStack::PushL( self );        
    self->ConstructL( aRunMode );
    CleanupStack::Pop( self );
    return self;
    }


// ================= INTERNAL FUNCTIONS =======================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnShowOnMapHandler::ConstructL( TRunMode aRunMode )
    {
    LOG("CMnShowOnMapHandler::ConstructL in");

    CMnMapViewCmdHandlerBase::ConstructL( aRunMode );
    
    LOG("CMnShowOnMapHandler::ConstructL out");
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnShowOnMapHandler::DoStartL()
    {
    LOG("CMnShowOnMapHandler::DoStartL in");
    MapView().ShowMapL();
    LOG("CMnShowOnMapHandler::DoStartL out");
    }
