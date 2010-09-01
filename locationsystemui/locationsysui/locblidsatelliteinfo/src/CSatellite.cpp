/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implements interface for CSatellite
*
*/


// INCLUDE FILES
#include <bautils.h>
#include <AknWaitDialog.h>
#include <data_caging_path_literals.hrh>
#include <SatInfo.rsg>
#include "SatInfoConsts.h"
#include "CSatelliteEng.h"
#include "CSatelliteDialog.h"
#include "Debug.h"
#include "CSatellite.h"

// ============================ MEMBER FUNCTIONS ==============================
// ----------------------------------------------------------------------------
// CSatellite::NewL
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
CSatellite* CSatellite::NewL()
    {
    CSatellite* self = new( ELeave ) CSatellite;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); // self
    return self;
    }

// ---------------------------------------------------------
// CSatellite::ConstructL
// ---------------------------------------------------------
//
void CSatellite::ConstructL()
    {
    iEnv = CEikonEnv::Static();
    OpenResourceFileL();    
    iEngine = CSatelliteEng::NewL();  
    }

// ----------------------------------------------------------------------------
// CSatellite::CSatellite
// C++ default constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------------------------------
//
CSatellite::CSatellite()
    {
    iLaunchView = ESignalStrengthView;
    }

// ---------------------------------------------------------
// CSatellite::~CSatellite
// ---------------------------------------------------------
//
CSatellite::~CSatellite()
    {
    CloseResourceFile();    
    delete iEngine;
    delete iWaitDialog;    
    }

// ---------------------------------------------------------
// CSatellite::NotifyL
// ---------------------------------------------------------
//
void CSatellite::NotifyL(TInt /*aError*/)
    {
    if ( iWaitDialog ) 
		{
	    iWaitDialog->ProcessFinishedL();        
	    iWaitDialog = NULL;
	    }
	else if(iDialog)
		{
		iDialog->UpdateL();
		}	
    }


// ---------------------------------------------------------
// CSatellite::NotifyError
// ---------------------------------------------------------
//               
void CSatellite::NotifyError( TInt /*aErrorCode*/ )
    {
    if ( iWaitDialog )
        {
        delete iWaitDialog;
        iWaitDialog = NULL;
        }
    }

// ---------------------------------------------------------
// CSatellite::ExecuteL
// ---------------------------------------------------------
//               
TInt CSatellite::ExecuteL( const TDesC& aNameOfRule )
    {
    TInt ret(0);
    
    iEngine->SetObserver( *this );
    iEngine->RequestSatelliteDataL( aNameOfRule );    

    iWaitDialog = new (ELeave) CAknWaitDialog( NULL, ETrue );
    if ( iWaitDialog->ExecuteLD( R_SATELLITE_WAIT_NOTE ) )
        {
        iWaitDialog = NULL;
        iDialog = CSatelliteDialog::NewL( *iEngine );
        iDialog->SetLaunchView(iLaunchView);
        ret = iDialog->ExecuteLD(R_SATELLITE_DIALOG);        
        }
    else
        {        
        delete iDialog;     
        iDialog = NULL;
        }
    iWaitDialog = NULL;
    return ret;
    }

// ----------------------------------------------------------------------------
// CSatellite::HandleForegroundEventL
// ----------------------------------------------------------------------------
//
void CSatellite::HandleForegroundEventL(TBool aForeground)
    {
    if ( aForeground )
        {
        iEngine->StartRequestingL();
        }
    else
        {
        iEngine->StopRequesting();
        }
    }


// ----------------------------------------------------------------------------
// CSatellite::OpenResourceFile
// ----------------------------------------------------------------------------
//
void CSatellite::OpenResourceFileL()
    {
    FindResourceFileL();

    BaflUtils::NearestLanguageFile( iEnv->FsSession(), iResourceFileName );
    iResourceOffset = iEnv->AddResourceFileL( iResourceFileName );
    }

// ----------------------------------------------------------------------------
// CSatellite::CloseResourceFile
// ----------------------------------------------------------------------------
//
void CSatellite::CloseResourceFile()
    {
    iEnv->DeleteResourceFile( iResourceOffset );
    }

// ----------------------------------------------------------------------------
// CSatellite::FindResourceFileL
// ----------------------------------------------------------------------------
//
void CSatellite::FindResourceFileL()
    {
    // Path and file name:
    TFileName file;
    file.Append( KSatTmp_RESOURCE_FILES_DIR ); // always safe
    file.Append( KSatInfoRscFileName ); // always safe   
    
    // Drive:
    TFileName drive;
    Dll::FileName( drive );

    // Add drive to path & file name:
    TParse parse;
    User::LeaveIfError( parse.Set( file, &drive, NULL ) );
    iResourceFileName = parse.FullName();
    }

// ----------------------------------------------------------------------------
// CSatellite::SetLaunchView
// ----------------------------------------------------------------------------
//
void CSatellite::SetLaunchView(TSatDialogView aView)
	{
	iLaunchView = aView;	
	}

// End of File
