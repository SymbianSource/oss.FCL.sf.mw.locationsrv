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
* Description: 
*     Implements interface for Loc event
*
*/


// INCLUDE FILES
#include "locpsyeng.h"
#include "locpsysettings.h"
#include "locpsyengdebug.h"

// CONSTANTS
/// Unnamed namespace for local definitions
#ifdef _DEBUG

_LIT( KLocPanicText, "CLocPsyEng" );
// LOCAL CONSTANTS AND MACROS
enum TPanicCode
    {
    KLocErrModelTypeNotSupported = -70
    };
// ==================== LOCAL FUNCTIONS ====================

void Panic(TPanicCode aReason)
    {
    User::Panic( KLocPanicText, aReason );
    }
#endif

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CLocPsyEng::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CLocPsyEng* CLocPsyEng::NewL()
    {
    CLocPsyEng* self = new (ELeave) CLocPsyEng();
    // Nothing to do for the second stage construction.
    return self;
    }

// -----------------------------------------------------------------------------
// CLocPsyEng::CLocPsyEng
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CLocPsyEng::CLocPsyEng() 
    {
    }

// Destructor

EXPORT_C CLocPsyEng::~CLocPsyEng()
    {    
    delete iSettings;
    }

// -----------------------------------------------------------------------------
// CLocPsyEng::SettingsModel
// Settings model getter.
// -----------------------------------------------------------------------------
//
EXPORT_C MLocPsySettings* CLocPsyEng::SettingsModel()
    {
    return iSettings;
    }

// -----------------------------------------------------------------------------
// CLocPsyEng::CreateModelL() 
// Creates a given model type.
// -----------------------------------------------------------------------------
//
EXPORT_C void CLocPsyEng::CreateModelL( TLocModelType aModel )
    {
    switch( aModel )
        {
        case ELocPsySettings:
            {
            if ( !iSettings )
                {
                iSettings = CLocPsySettings::NewL();
                }
            break;
            }            
        default:
            {
            // Not supported model type
            __ASSERT_DEBUG(EFalse, Panic(KLocErrModelTypeNotSupported));
            break;
            }            
        }
    }

// -----------------------------------------------------------------------------
// CLocPsyEng::RemoveModel
// Removes a given model type.
// -----------------------------------------------------------------------------
//
EXPORT_C void CLocPsyEng::RemoveModel( TLocModelType aModelType )
    {
    switch ( aModelType )
        {
        case ELocPsySettings:
            {
            delete iSettings;
            iSettings = NULL;
            break;
            }           
        default:
            {
            // Not supported model type
            __ASSERT_DEBUG(EFalse, Panic(KLocErrModelTypeNotSupported));
            }
            break;
        }
    }

// End of File
