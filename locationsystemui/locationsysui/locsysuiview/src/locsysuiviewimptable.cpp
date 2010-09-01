/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  ECOM proxy table for the Location System UI View plug-in to 
*				  General Settings
*
*/


// System includes
#include <e32std.h>
#include <ecom/implementationproxy.h>

// User includes
#include "locsysuiview.h"
#include "locsysuiviewuid.hrh"

// Constants
const TImplementationProxy KLocSysUiViewPluginImpTable[] = 
	{
	IMPLEMENTATION_PROXY_ENTRY( LOCSYSUIVIEW_UID3, CLocSysUiView::NewL )
	};


// ---------------------------------------------------------------------------
// ImplementationGroupProxy
// Gate/factory function
//
// ---------------------------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy( TInt& aTableCount )
	{
	aTableCount = sizeof( KLocSysUiViewPluginImpTable ) 
        / sizeof( TImplementationProxy );
        
    return KLocSysUiViewPluginImpTable;
	}





