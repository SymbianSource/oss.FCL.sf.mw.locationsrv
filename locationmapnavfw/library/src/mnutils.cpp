/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  MnUtils class implementation
*
*/


#include <e32base.h>
#include <f32file.h>
#include <barsc2.h>
#include <barsread2.h>
#include <bautils.h>

#include "mnutils.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void MnUtils::FindLocalizedResourceFileL(
    RFs& aFsSession,
    const TDesC& aRscFile,
    TFileName& aFileName )
    {
    TFileName fileName( aRscFile );

    TParse parse;
    parse.Set( aRscFile, NULL, NULL );

    // look on system and given drives first
    BaflUtils::NearestLanguageFile( aFsSession, fileName );

    const TInt KLangPartOfExtensionLen = 2;
    TBool found = ( fileName.Right( KLangPartOfExtensionLen ) !=
                    aRscFile.Right( KLangPartOfExtensionLen ) );

    if ( !found )
        {
        // look on other drives
        TDriveList drives;
        aFsSession.DriveList( drives );

        for ( TInt drive = EDriveA; drive <= EDriveZ; drive++ )
            {
            if ( drives[drive] && !( drives[drive] & KDriveAttRemote ) ) // avoid remote drives
                {
                TDriveInfo drvInfo;
                TInt err = aFsSession.Drive( drvInfo, drive );

                if ( !err && drvInfo.iType != EMediaNotPresent )
                    {
                    TChar drv;
                    aFsSession.DriveToChar( drive, drv );
    
                    fileName.Zero();
                    fileName.Append( drv );
                    fileName.Append( KDriveDelimiter );
                    fileName.Append( parse.Path() );
                    fileName.Append( parse.NameAndExt() );
    
                    // look on c: and this drive
                    BaflUtils::NearestLanguageFile( aFsSession, fileName );
    
                    const TInt KLangPartOfExtensionLen = 2;
                    found = ( fileName.Right( KLangPartOfExtensionLen ) !=
                              aRscFile.Right( KLangPartOfExtensionLen ) );
                    if ( found ) break;
                    }
                }    
            }
        }

    if ( !found )
        {
        // localized file not found, try using SC
        FindResourceFileL( aFsSession, aRscFile, fileName );
        }
        
    aFileName.Copy( fileName );        
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void MnUtils::FindResourceFileL(
    RFs& aFsSession,
    const TDesC& aFilePathAndName,
    TFileName& aRscFile )
    {
    TParse parse;
    parse.Set( aFilePathAndName, NULL, NULL );

    TFindFile finder( aFsSession );
    if ( finder.FindByDir( parse.NameAndExt(), parse.Path() ) == KErrNone )
        {
        aRscFile.Copy( finder.File() );
        }
    else
        {
        User::Leave( KErrNotFound );
        }
    }
