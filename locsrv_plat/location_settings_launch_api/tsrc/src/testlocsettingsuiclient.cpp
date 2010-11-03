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
* Description:   Test Module DLL to Location Settings Launch API
*
*/




// INCLUDE FILES
#include <Stiftestinterface.h>
#include "testlocsettingsuiclient.h"
#include "testlocsettingsuiclientao.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CTestLocSettingsUiClient::CTestLocSettingsUiClient
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CTestLocSettingsUiClient::CTestLocSettingsUiClient( 
    CTestModuleIf& aTestModuleIf ):
        CScriptBase( aTestModuleIf )
    {
    }

// -----------------------------------------------------------------------------
// CTestLocSettingsUiClient::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CTestLocSettingsUiClient::ConstructL()
    {
    iLog = CStifLogger::NewL( KtestlocsettingsuiclientLogPath, 
                          KtestlocsettingsuiclientLogFile,
                          CStifLogger::ETxt,
                          CStifLogger::EFile,
                          EFalse );
                          
	iLocSettingsUiClientAO = CTestLocSettingsUiClientAO::NewL(iLog);

    }

// -----------------------------------------------------------------------------
// CTestLocSettingsUiClient::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CTestLocSettingsUiClient* CTestLocSettingsUiClient::NewL( 
    CTestModuleIf& aTestModuleIf )
    {
    CTestLocSettingsUiClient* self = new (ELeave) CTestLocSettingsUiClient( aTestModuleIf );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;

    }

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
//
CTestLocSettingsUiClient::~CTestLocSettingsUiClient()
    { 

    // Delete resources allocated from test methods
    Delete();

    // Delete logger
    delete iLog; 

    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

// -----------------------------------------------------------------------------
// LibEntryL is a polymorphic Dll entry point.
// Returns: CScriptBase: New CScriptBase derived object
// -----------------------------------------------------------------------------
//
EXPORT_C CScriptBase* LibEntryL( 
    CTestModuleIf& aTestModuleIf ) // Backpointer to STIF Test Framework
    {

    return ( CScriptBase* ) CTestLocSettingsUiClient::NewL( aTestModuleIf );

    }


//  End of File
