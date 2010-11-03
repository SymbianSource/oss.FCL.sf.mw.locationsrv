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
* Description:  Internal helpers for Map and Navigation FW
*
*/


#ifndef MN_UTILS_H
#define MN_UTILS_H

/** \inernal 
 *  This class contains internal utils for the subsystem
 */
class MnUtils
    {
    public:
        /** Looks for localized version of specified resource file 
         *  on all available drives
         *  \param[in] aFsSession opened session with File Server
         *  \param[in] aRscFile z: -based name of SC file
         *  \param[out] aFileName filename of localized version of SC file
         *                        or SC filename if not found
         *  \leave KErrNotFound if file is not found on any drives
         */
        IMPORT_C static void FindLocalizedResourceFileL( 
            RFs& aFsSession, 
            const TDesC& aRscFile,
            TFileName& aFileName );

        /** Looks for specified resource file on all available drives
         *  on all available drives
         *  \param[in] aFsSession opened session with File Server
         *  \param[in] aFilePathAndName path and name of resource file
         *  \param[out] aFileName filename with drive set
         *  \leave KErrNotFound if file is not found on any drives
         */
        IMPORT_C static void FindResourceFileL( 
            RFs& aFsSession,
            const TDesC& aFilePathAndName,
            TFileName& aRscFile );
    };
    
#endif // MN_UTILS_H
