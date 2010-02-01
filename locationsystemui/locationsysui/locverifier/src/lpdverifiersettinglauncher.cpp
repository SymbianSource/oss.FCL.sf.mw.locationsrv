/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Responsible for launching SUPL session details
*
*/


// INCLUDE FILES
#include "lpdverifiersettinglauncher.h"

// CONSTANTS
const TInt KSettingsParamBufferLength = 64;

// ================= MEMBER FUNCTIONS =======================
// ----------------------------------------------------------------------------
// CLmkLocSettingsLauncher::NewL
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
CLpdVerifierSettingsLauncher* CLpdVerifierSettingsLauncher:: NewL()
	{
	CLpdVerifierSettingsLauncher* self = new (ELeave) CLpdVerifierSettingsLauncher();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(); // self
	return self;
	}

// ----------------------------------------------------------------------------
// CLmkLocSettingsLauncher::CLmkLocSettingsLauncher
// ----------------------------------------------------------------------------
//
CLpdVerifierSettingsLauncher::CLpdVerifierSettingsLauncher()
	: CActive( EPriorityStandard )
	{
	}

// ----------------------------------------------------------------------------
// CLpdVerifierSettingsLauncher::~CLpdVerifierSettingsLauncher
// ----------------------------------------------------------------------------
//
CLpdVerifierSettingsLauncher::~CLpdVerifierSettingsLauncher()
	{
	Cancel();
    delete iClientLibrary;
    delete iLaunchParams;
	}

// ----------------------------------------------------------------------------
// CLpdVerifierSettingsLauncher::ConstructL
// ----------------------------------------------------------------------------
//
void CLpdVerifierSettingsLauncher::ConstructL()
	{
	iClientLibrary  = CLocSettingsUiClient::NewL();    
	iLaunchParams = HBufC::NewL( KSettingsParamBufferLength );
  CActiveScheduler::Add( this );
	}

// ----------------------------------------------------------------------------
// CLpdVerifierSettingsLauncher::LaunchL
// ----------------------------------------------------------------------------
//
void CLpdVerifierSettingsLauncher::LaunchL( TInt aSessionId )
	{
	Cancel();
	iLaunchParams->Des().Zero();
	iLaunchParams->Des().AppendNum( aSessionId );
	iClientLibrary->LaunchSettingsUiAsEmbeddedAppL( TUid::Uid( KLocSUPLSettingsUID ), *iLaunchParams, iStatus );
	SetActive();
	}

// ----------------------------------------------------------------------------
// CLpdVerifierSettingsLauncher::RunL
// ----------------------------------------------------------------------------
//
void CLpdVerifierSettingsLauncher::RunL()
    {
    switch( iStatus.Int())
        {
        case KErrNone:
            {
            break;
            }
        case KErrCancel:
            {
            break;
            }
        default:
            {
            break;    
            }
        }
    }

// ----------------------------------------------------------------------------
// CLpdVerifierSettingsLauncher::DoCancel
// ----------------------------------------------------------------------------
//
void CLpdVerifierSettingsLauncher::DoCancel()
    {
    iClientLibrary->CancelLaunchedSettingsUi();
    }

// End of File

