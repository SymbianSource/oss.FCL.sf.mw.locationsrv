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
* Description:  Static utility class for file operations.
*
*/


// INCLUDE FILES
#include "locfileutils.h"
#include <bautils.h>
#include <f32file.h>

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// LocFileUtils::GetFileWithCorrectDriveL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void LocFileUtils::GetFileWithCorrectDriveL(
    const TFileName& aDrive,
	TFileName& aFileName )
    { 
    TParse parse;
	User::LeaveIfError( parse.Set( aDrive, NULL, NULL ) );
    User::LeaveIfError( parse.Set( parse.Drive(), &aFileName, NULL ) );
	aFileName = parse.FullName();
    }

// -----------------------------------------------------------------------------
// LocFileUtils::GetNearestLanguageFileL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void LocFileUtils::GetNearestLanguageFileL(
    RFs& aSession,
    const TFileName& aDrive,
    TFileName& aFileName )
    {
    GetFileWithCorrectDriveL( aDrive, aFileName );
    BaflUtils::NearestLanguageFile( aSession, aFileName );
    }

//  End of File
