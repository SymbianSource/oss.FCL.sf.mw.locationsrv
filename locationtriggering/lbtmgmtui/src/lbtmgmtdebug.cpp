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
* Description:  Debug File.
*
*/



// INCLUDE FILES
#include "lbtmgmtdebug.h"
#include <flogger.h>
#include <e32svr.h>
#include <bautils.h>
// CONSTANTS

/// Folder where the log resides
_LIT( KLbtMgmtLogFolder, "epos" );

/// The name of the log file
_LIT( KLbtMgmtLogFileName, "lbtmgmtui.txt" );

/// The format in which the time is formatted in log
_LIT( KLbtMgmtLogTimeFormat, "%02d.%02d:%02d:%06d ");

/// The length of the string produced by KLbtMgmtLogTimeFormat
const TInt KLbtMgmtLogTimeFormatLength = 16;

/// How many characters a log line can contain
const TInt KLbtMgmtLogLineLength = 256;

// ========================== OTHER EXPORTED FUNCTIONS =========================



// -----------------------------------------------------------------------------
// Debug
// Generates a log file if c:\logs\epos\ folder exists
// -----------------------------------------------------------------------------
//
void Debug( TRefByValue<const TDesC> aText, ... )
    {    
    VA_LIST args;
    VA_START( args, aText );
    
    TBuf<KLbtMgmtLogLineLength> buf;
    buf.FormatList( aText, args );

    #ifdef _DEBUG
    RDebug::Print(buf);
    #endif
	
    RFileLogger logger;
    
    TInt ret=logger.Connect();
    if (ret==KErrNone)
        {
        logger.SetDateAndTime( EFalse,EFalse );
        logger.CreateLog( KLbtMgmtLogFolder, KLbtMgmtLogFileName, EFileLoggingModeAppend );       
        TBuf<KLbtMgmtLogTimeFormatLength> timeStamp;
        TTime now;
        now.HomeTime();
        TDateTime dateTime;
        dateTime = now.DateTime();
        timeStamp.Format( KLbtMgmtLogTimeFormat, 
            dateTime.Hour(), dateTime.Minute(),
            dateTime.Second(), dateTime.MicroSecond() );
        buf.Insert( 0, timeStamp );

        logger.Write(buf);
        }

    logger.Close();

    VA_END( args );
    }
	
//  End of File  
