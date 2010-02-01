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
* Description:  CTestProviderCoordConverter class
*
*/


#include <e32base.h>
#include "testprovidercoordconverter.h"

// ========================== MEMBER FUNCTIONS ===============================

// ---------------------------------------------------------------------------
// Two phase constructor
//---------------------------------------------------------------------------
CTestProviderCoordConverter* CTestProviderCoordConverter::NewLC()
    {
    CTestProviderCoordConverter* self = new (ELeave) CTestProviderCoordConverter;
    CleanupStack::PushL(self);
    return self;
    }

// ---------------------------------------------------------------------------
// C++ constructor.
// ---------------------------------------------------------------------------
CTestProviderCoordConverter::CTestProviderCoordConverter()
    {
    }

// ---------------------------------------------------------------------------
// C++ destructor
// ---------------------------------------------------------------------------
CTestProviderCoordConverter::~CTestProviderCoordConverter()
    {
    }

// ---------------------------------------------------------------------------
// Implementation for GetImageCoordinate declared in base class.
// ---------------------------------------------------------------------------
TInt CTestProviderCoordConverter::GetImageCoordinate( 
    const TCoordinate& /*aWorldCoordinate*/, 
    TPoint& /*aImagePoint*/ )
    {
    return KErrNone;    
    }


// ---------------------------------------------------------------------------
// Implementation for GetWorldCoordinate declared in base class.
// ---------------------------------------------------------------------------
TInt CTestProviderCoordConverter::GetWorldCoordinate( 
    const TPoint& /*aImagePoint*/, 
    TCoordinate& /*aWorldCoordinate*/ )
    {
    return KErrNone;    
    }

