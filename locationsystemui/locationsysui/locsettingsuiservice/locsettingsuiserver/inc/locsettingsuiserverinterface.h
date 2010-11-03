/*
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Interface header for the Location Settings Ui server
*
*/


#ifndef R_LOCSETTINGSUISERVERINTERFACE_H
#define R_LOCSETTINGSUISERVERINTERFACE_H

/**
 * Server Name
 */
 _LIT( KLocSettingsUiServerName, "!LocSettingsUiServer" );

/**
 * Server exe
 */
 _LIT( KLocSettingsUiServerExe, "locsettingsuiserver" );
 
/** 
 * Server UID
 */
 const TInt  KLocSettingsUiServerUid    = 0x10281861;
 
/**
 * Service UID
 */
 const TInt  KLocSettingsUiServiceId    = 0x10281862;
 
/**
 * Message Types
 */
 enum TLocSettingsUiMsgType
    {
    // Using a random value. Just to avoid clashing with the Server message
    // types
    ELaunchSettings = 0x1000,
    ELaunchSettingsWithString,
    ECancelSettings,
    ELaunchPosSettings,
    ECancelPosSettings    
    };
  
/**
 * parameter Slots in a IPC message
 */
 enum TLocSettingsUiSrvParams
    {
    KSettingsUID = 0,
    KSettingsUIParam
    };

/**
 * Number of Message slots
 */ 
 const TInt KDefaultMessageSlots = 4;

/**
 * Location UI Server Panic Text
 */
 _LIT( KLocSettingsUIPanicTxt, "Location Settings UI Server" );

/** 
 * Server Panic Codes
 */
 enum TLocSetitngsUiPanicCodes
    {
    ELocSettingsUiSrvInvalidArguments = 1,
    ELocSettingsUiSrvUnknownMessage,
    ELocSettingsUiSrvBadDescriptor
    };
 
#endif // R_LOCSETTINGSUISERVERINTERFACE_H
