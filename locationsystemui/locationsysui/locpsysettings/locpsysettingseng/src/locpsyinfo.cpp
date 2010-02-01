/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Location Psy info class.
*
*/



// INCLUDE FILES
#include    "locpsyinfo.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CLocPsyInfo::PsyId
// -----------------------------------------------------------------------------
//
EXPORT_C TPositionModuleId CLocPsyInfo::PsyId() const 
    { 
    return iPsyId; 
    }

// -----------------------------------------------------------------------------
// CLocPsyInfo::PsyName
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& CLocPsyInfo::PsyName() const 
    { 
    return *iPsyName; 
    }

// -----------------------------------------------------------------------------
// CLocPsyInfo::SetPsyId
// -----------------------------------------------------------------------------
//
void CLocPsyInfo::SetPsyId(const TPositionModuleId& aPsyId)
	{
	iPsyId = aPsyId;
	}

// -----------------------------------------------------------------------------
// CLocPsyInfo::SetPsyNameL
// -----------------------------------------------------------------------------
//	
void CLocPsyInfo::SetPsyNameL(const TDesC& aBuf)
	{
	delete iPsyName;
    iPsyName = NULL;
    iPsyName = aBuf.AllocL();
	}

// -----------------------------------------------------------------------------
// CLocPsyInfo::CLocPsyInfo
// -----------------------------------------------------------------------------
//	
CLocPsyInfo::CLocPsyInfo()
    {    
    }

// -----------------------------------------------------------------------------
// CLocPsyInfo::~CLocPsyInfo
// -----------------------------------------------------------------------------
//
CLocPsyInfo::~CLocPsyInfo()
    {
    delete iPsyName;
    }
//  End of File  
