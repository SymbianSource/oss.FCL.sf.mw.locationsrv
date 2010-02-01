/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CMnCoordinateConverterBase class implementation
*
*/


#include <e32base.h>
#include <ecom/ecom.h>
#include <mnpluginuids.hrh>
#include "mncoordinateconverterbase.h"

// ========================== LOCAL FUNCTIONS ================================

// ========================== MEMBER FUNCTIONS ===============================

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C CMnCoordinateConverterBase::CMnCoordinateConverterBase()
    {
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
CMnCoordinateConverterBase* CMnCoordinateConverterBase::NewL( TUid aProviderUid )
    {
    TInt32 offset = static_cast<TInt32> _FOFF( CMnCoordinateConverterBase, iDtorIdKey );

    const TInt KNumUidHexDigits = 8;
    TBuf8<KNumUidHexDigits> uidText8;
    
    _LIT8( KUidHexText, "%08X" );
    uidText8.Format( KUidHexText, aProviderUid.iUid );

    TEComResolverParams resolverParams;
    resolverParams.SetDataType( uidText8 );

    TAny* ptr = REComSession::CreateImplementationL( 
        TUid::Uid( KMnMapImageConverterIf ), offset, resolverParams );

    return reinterpret_cast<CMnCoordinateConverterBase*>( ptr );
    }
    
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C CMnCoordinateConverterBase::~CMnCoordinateConverterBase()
    {
    REComSession::DestroyedImplementation( iDtorIdKey );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C const TMnMapImageParams& CMnCoordinateConverterBase::MapImageParams() const
    {
    return iImageParams;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnCoordinateConverterBase::SetMapImageParams( const TMnMapImageParams& aImageParams )
    {
    iImageParams = aImageParams;
    }
