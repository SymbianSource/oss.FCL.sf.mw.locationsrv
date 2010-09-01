/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of Manager class for the BtGpsPsy string constants
*
*/



// System Includes
#include <e32std.h>
#include <bautils.h>
#include <barsc.h>
#include <centralrepository.h>


// User Includes
#include "locbtnotifierconstantsmanager.h"
#include "locbtnotifierprivatecrkeys.h"

// Resource Definitions
#include <locbtnotifierresources.rsg>

// CONSTANTS

// Resource File Name
_LIT(KResourceFileName, "\\resource\\apps\\locbtnotifierresources.*");
			


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CLocBtNotifierConstantsManager::NewL
// Two Phased constructor 
// ---------------------------------------------------------------------------
//
CLocBtNotifierConstantsManager* CLocBtNotifierConstantsManager::NewL()
    {
    CLocBtNotifierConstantsManager* self = new( ELeave ) CLocBtNotifierConstantsManager;
    CleanupStack::PushL( self );
    self->ConstructL();    
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// CLocBtNotifierConstantsManager::~CLocBtNotifierConstantsManager
// Destructor
// -----------------------------------------------------------------------------
CLocBtNotifierConstantsManager::~CLocBtNotifierConstantsManager()
    {
    delete iBatteryLowDialogText;
	delete iBatteryFullDialogText;
    delete iExtAntennaConnectedDialogText;
    delete iExtAntennaDisconnectedDialogText;
    delete iExtPowerConnectedDialogText;
    delete iExtPowerDisconnectedDialogText;
    }
    
// -----------------------------------------------------------------------------
// CLocBtNotifierConstantsManager::ConstructL
// -----------------------------------------------------------------------------
void CLocBtNotifierConstantsManager::ConstructL()
    {
    //File session for resource file
    RFs fileSession;

    //Open file session
    User::LeaveIfError( fileSession.Connect() );
    CleanupClosePushL( fileSession );

    TFileName* localisedRscName = GetLocalisableResourceNameL( fileSession );
    CleanupStack::PushL( localisedRscName );

	//Construct RResourceFile
	RResourceFile localisationResourceFile;
    localisationResourceFile.OpenL( fileSession, *localisedRscName );
    CleanupClosePushL( localisationResourceFile );

    //Check the signature
    localisationResourceFile.ConfirmSignatureL( 0 );
    
    //Read constants from resource file
    iBatteryLowDialogText = GetTextL( localisationResourceFile, R_BATTERY_LOW );
		
    iBatteryFullDialogText = GetTextL( localisationResourceFile, R_BATTERY_FULL );
		
    iExtAntennaConnectedDialogText = GetTextL( localisationResourceFile, 
                                              R_EXT_ANTENNA_CONNECTED );
		
    iExtAntennaDisconnectedDialogText = GetTextL( localisationResourceFile, 
                                                 R_EXT_ANTENNA_DISCONNECTED );
		
    iExtPowerConnectedDialogText = GetTextL( localisationResourceFile, 
                                            R_EXT_POWER_CONNECTED );
		
    iExtPowerDisconnectedDialogText = GetTextL( localisationResourceFile,
                                               R_EXT_POWER_DISCONNECTED );
        
        
    CleanupStack::PopAndDestroy( 3,&fileSession );
    }
    
// ---------------------------------------------------------------------------
// CLocBtNotifierConstantsManager::CLocBtNotifierConstantsManager
// Private Constructor
// ---------------------------------------------------------------------------
//
 CLocBtNotifierConstantsManager::CLocBtNotifierConstantsManager()
    {
    
    }
    
// -----------------------------------------------------------------------------
// CLocBtNotifierConstantsManager::GetLocalisableResourceNameL
// -----------------------------------------------------------------------------
TFileName* CLocBtNotifierConstantsManager::GetLocalisableResourceNameL( RFs& aFs )
    {
	TFileName* fileName = new ( ELeave ) TFileName;
	CleanupStack::PushL( fileName );
	
	TFindFile findFile( aFs );
	CDir* dir;
	User::LeaveIfError( findFile.FindWildByDir( KResourceFileName, KNullDesC(),dir ) );
	CleanupStack::PushL( dir );
	  
	TParse fileNameParser;
	fileNameParser.Set( findFile.File(), NULL, NULL );
	
	fileName->Copy( fileNameParser.DriveAndPath() );
	fileName->Append( ( *dir )[0].iName );
	CleanupStack::PopAndDestroy( dir );
	
	BaflUtils::NearestLanguageFile( aFs, *fileName );
	
	CleanupStack::Pop( fileName );
	return fileName;
    }
    
// -----------------------------------------------------------------------------
// CLocBtNotifierConstantsManager::GetTextL
// -----------------------------------------------------------------------------
HBufC* CLocBtNotifierConstantsManager::GetTextL( RResourceFile& aResourceFile, 
                                                TInt aId )
    {
    // Read from resource file
    HBufC8* buffer = aResourceFile.AllocReadLC( aId );
    HBufC* retBuf;
    
    //Resource parser
    TResourceReader resourceReader;
    resourceReader.SetBuffer( buffer );
    
    retBuf = resourceReader.ReadHBufCL();
    User::LeaveIfNull( retBuf );
    
    CleanupStack::PopAndDestroy( buffer );
    
    return retBuf;
    }


// -----------------------------------------------------------------------------
// CLocBtNotifierConstantsManager::GetBatteryLowDialogText
// -----------------------------------------------------------------------------
TPtr CLocBtNotifierConstantsManager::GetBatteryLowDialogText()
    {
	return iBatteryLowDialogText->Des();
    }
    

// -----------------------------------------------------------------------------
// CLocBtNotifierConstantsManager::GetBatteryFullDialogText
// -----------------------------------------------------------------------------
TPtr CLocBtNotifierConstantsManager::GetBatteryFullDialogText()
    {
	return iBatteryFullDialogText->Des();
    }

    
// -----------------------------------------------------------------------------
// CLocBtNotifierConstantsManager::GetExtAntennaConnectedDialogText
// -----------------------------------------------------------------------------
TPtr CLocBtNotifierConstantsManager::GetExtAntennaConnectedDialogText()
    {
	return iExtAntennaConnectedDialogText->Des();
    }
    

// -----------------------------------------------------------------------------
// CLocBtNotifierConstantsManager::GetExtAntennaDisconnectedDialogText
// -----------------------------------------------------------------------------
TPtr CLocBtNotifierConstantsManager::GetExtAntennaDisconnectedDialogText()
    {
	return iExtAntennaDisconnectedDialogText->Des();
    }
    

// -----------------------------------------------------------------------------
// CLocBtNotifierConstantsManager::GetExtPowerConnectedDialogText
// -----------------------------------------------------------------------------
TPtr CLocBtNotifierConstantsManager::GetExtPowerConnectedDialogText()
    {
	return iExtPowerConnectedDialogText->Des();
    }
    

// -----------------------------------------------------------------------------
// CLocBtNotifierConstantsManager::GetExtPowerDisconnectedDialogText
// -----------------------------------------------------------------------------
TPtr CLocBtNotifierConstantsManager::GetExtPowerDisconnectedDialogText()
    {
	return iExtPowerDisconnectedDialogText->Des();
    }
    

 

