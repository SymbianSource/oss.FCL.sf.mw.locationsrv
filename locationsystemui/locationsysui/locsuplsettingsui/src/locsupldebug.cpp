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
* Description:  Utility class for Debug traces
*
*/


// INCLUDE FILES
#include "locsupldebug.h"
#include <flogger.h>
#include <e32svr.h>

// ========================== EXPORTED FUNCTIONS =========================

#ifdef _LOCSUPLDEBUG

/// Folder where the log resides
_LIT( KLogFolder, "epos" );

/// The name of the log file
_LIT( KLogFileName, "mslp.txt" );

/// How many characters a log line can contain
const TInt KLogLineLength = 256;

// -----------------------------------------------------------------------------
//  void Debug( TRefByValue<const TDesC> aText, ... )
// -----------------------------------------------------------------------------
//
void Debug( TRefByValue<const TDesC> aText, ... )
    {
    RDebug::Print(aText);

    VA_LIST args;
    VA_START( args, aText );

    TBuf<KLogLineLength> buf;
    buf.FormatList( aText, args );
    RFileLogger logger;
    TInt ret=logger.Connect();
    if (ret==KErrNone)
        {
        logger.SetDateAndTime( EFalse,EFalse );
        logger.CreateLog( KLogFolder, KLogFileName, EFileLoggingModeAppend );
        logger.Write(buf);
        }
    logger.Close();

    VA_END( args );
    }

#endif

//  End of file
