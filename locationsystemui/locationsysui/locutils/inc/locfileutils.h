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


#ifndef LOCFILEUTILS_H_
#define LOCFILEUTILS_H_

//  INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class RFs;

// CLASS DECLARATION

/**
*  Static utility class for file operations.
*  @lib locutils.lib
*  @since 2.6
*/
class LocFileUtils
    {
    public: // New functions

        /**
        * Sets correct drive to aFileName. In normal use aDrive is the drive where
        * calling dll resides, and it has been set by calling Dll::FileName().
        *
        * @param aDrive file name where drive is extracted from
        * @param aFileName name which will be updated
        */
        IMPORT_C static void GetFileWithCorrectDriveL( const TFileName& aDrive,
                                                       TFileName& aFileName );

        /**
        * Uses GetFileWithCorrectDriveL and then
        * BaflUtils::NearestLanguageFile() to locate correct file and language
        * version. To be used with resource files.
        * Leaves if file is not found.
        *
        * @param aSession opened file server session
        * @param aDrive file name where drive is extracted from        
        * @param aFileName name which will be updated
        */
        IMPORT_C static void GetNearestLanguageFileL( RFs& aSession,
                                                      const TFileName& aDrive,
                                                      TFileName& aFileName );
    };

#endif // LOCFILEUTILS_H_
            
// End of File
