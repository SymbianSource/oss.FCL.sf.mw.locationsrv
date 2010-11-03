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
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include <locsettingsuiclient.h>

#include "testlocsettingsuiclient.h"
#include "testlocsettingsuiclientao.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CTestLocSettingsUiClient::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CTestLocSettingsUiClient::Delete() 
    {
    if(iLocSettingsUiClientAO)
    	{
    	delete iLocSettingsUiClientAO;	
    	iLocSettingsUiClientAO = NULL;
    	}	
    }

// -----------------------------------------------------------------------------
// CTestLocSettingsUiClient::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CTestLocSettingsUiClient::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  

        ENTRY( "TestNewL", CTestLocSettingsUiClient::TestNewL ),
        ENTRY( "TestNewLC", CTestLocSettingsUiClient::TestNewLC ),
        ENTRY( "TestLaunchSettingsL", CTestLocSettingsUiClient::TestLaunchSettingsL ),
        ENTRY( "TestCancelLaunchedSettingsUi", CTestLocSettingsUiClient::TestCancelLaunchedSettingsUi ),
        ENTRY( "TestCancelLaunchedSettingsUiErr", CTestLocSettingsUiClient::TestCancelLaunchedSettingsUiErr ),
        ENTRY( "TestClosePosSettings", CTestLocSettingsUiClient::TestClosePosSettings ),
        ENTRY( "TestClosePosSettingsErr", CTestLocSettingsUiClient::TestClosePosSettingsErr ),

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// ---------------------------------------------------------
// CTestLocSettingsUiClient::TestNewL
//
// (other items are commented in a header).
// ---------------------------------------------------------
//
TInt CTestLocSettingsUiClient::TestNewL( CStifItemParser& /*aItem*/ )
	{
	return iLocSettingsUiClientAO->TestNewL();
	}
	
// ---------------------------------------------------------
// CTestLocSettingsUiClient::TestNewLC
//
// (other items are commented in a header).
// ---------------------------------------------------------
//
TInt CTestLocSettingsUiClient::TestNewLC( CStifItemParser& /*aItem*/ )
	{
	return iLocSettingsUiClientAO->TestNewLC();
	}

// ---------------------------------------------------------
// CTestLocSettingsUiClientAO::TestLaunchPosSettingsAsEmbeddedAppL
//
// (other items are commented in a header).
// ---------------------------------------------------------
//
TInt CTestLocSettingsUiClient::TestLaunchSettingsL(
								 CStifItemParser& aItem )
	{
	TInt err = iLocSettingsUiClientAO->TestLaunchSettingsL(aItem);
//	CActiveScheduler::Start();
	return err;
	}

// ---------------------------------------------------------
// CTestLocSettingsUiClientAO::TestCancelLaunchedSettingsUi
//
// (other items are commented in a header).
// ---------------------------------------------------------
//
TInt CTestLocSettingsUiClient::TestCancelLaunchedSettingsUi(CStifItemParser& aItem)
	{
	TInt err = iLocSettingsUiClientAO->TestCancelLaunchedSettingsUi(aItem);
	return err;	
	}
	
// ---------------------------------------------------------
// CTestLocSettingsUiClientAO::TestCancelLaunchedSettingsUi
//
// (other items are commented in a header).
// ---------------------------------------------------------
//
TInt CTestLocSettingsUiClient::TestCancelLaunchedSettingsUiErr( CStifItemParser& /*aItem*/ )
	{
	TInt err = iLocSettingsUiClientAO->TestCancelLaunchedSettingsUiErr();
	return err;	
	}
	
// ---------------------------------------------------------
// CTestLocSettingsUiClientAO::TestClosePosSettings
//
// (other items are commented in a header).
// ---------------------------------------------------------
//
TInt CTestLocSettingsUiClient::TestClosePosSettings( CStifItemParser& /*aItem*/ )
	{
	TInt err = iLocSettingsUiClientAO->TestClosePosSettings();
	return err;	;		
	}

// ---------------------------------------------------------
// CTestLocSettingsUiClientAO::TestClosePosSettings
//
// (other items are commented in a header).
// ---------------------------------------------------------
//
TInt CTestLocSettingsUiClient::TestClosePosSettingsErr( CStifItemParser& /*aItem*/ )
	{
	TInt err = iLocSettingsUiClientAO->TestClosePosSettingsErr();
	return err;	;		
	}

//  End of File
