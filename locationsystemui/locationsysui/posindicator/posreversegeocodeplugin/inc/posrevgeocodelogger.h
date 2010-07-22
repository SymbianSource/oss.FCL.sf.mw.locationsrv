/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: Implementation of logger.
*
*/

#ifndef POSREVGEOCODELOGGER_H
#define POSREVGEOCODELOGGER_H

#include <flogger.h>
#include <f32file.h>

//#ifdef _DEBUG
#define FUNC( x ) {  TBuf8<150> buffer; \
                     const unsigned char* string = (unsigned char*)(x); \
                     buffer.Zero(); \
                     buffer.Copy(string); \
                     RFileLogger::Write( _L("lbs"),_L("PosRevGeocode.log"),EFileLoggingModeAppend,buffer); }

#define LOG ( x,y ) { TBuf8<150> buffer; \
                      const unsigned char* string = (unsigned char*)(x); \
                      buffer.Zero(); \
                      buffer.Copy(string); \
                      buffer.format(y); \
                      RFileLogger::Write( _L("lbs"),_L("PosRevGeocode.log"),EFileLoggingModeAppend,buffer); }

/*#else
#define FUNC( x )
#define LOG ( x,y )
#endif*/

#endif /* POSREVGEOCODELOGGER_H */
