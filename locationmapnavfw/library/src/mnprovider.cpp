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
* Description:  CMnProvider class implementation
*
*/


#include <e32base.h>

#include "mnprovider.h"
#include "mnproviderimpl.h"

// ======== LOCAL FUNCTIONS ========

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
CMnProvider::CMnProvider()
    {
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
CMnProvider::CMnProvider( CMnProvider& )
    {
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C CMnProvider::~CMnProvider()
    {
    }


// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C TUid CMnProvider::Uid() const
    {
    return iUid;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C TVersion CMnProvider::Version() const
    {
    return iVersion;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnProvider::GetName( TPtrC& aName ) const
    {
    if ( iName )
        {
        aName.Set( iName->Des() );
        }
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnProvider::GetShortName( TPtrC& aShortName ) const
    {
    if ( iShortName )
        {
        aShortName.Set( iShortName->Des() );
        }
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnProvider::GetVendorName( TPtrC& aVendorName ) const
    {
    if ( iVendorName )
        {
        aVendorName.Set( iVendorName->Des() );
        }
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C CMnProvider::TServices CMnProvider::SupportedServices() const
    {
    return iServices;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C TUint32 CMnProvider::SupportedFeatures( TService /*aService*/ ) const
    {
    return 0;
    }
