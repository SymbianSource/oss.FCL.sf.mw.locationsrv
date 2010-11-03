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
* Description:  The set of messages provided to Secondary Display software by
*                SUPL Settings UI.
*
*/

#ifndef LOCSUPLSETTINGSCOVERUI_H_
#define LOCSUPLSETTINGSCOVERUI_H_

// System Includes
#include <e32base.h>

// The category UID for the commands in this header file.
// UID of the SUPL Settings UI Dll ( locsuplsettings.dll )
//
const TUid KCatUidLocationSuplSettingsUi = { 0x10275091 };


// Dialogs shown by Location System UI / SUPL Settings UI. These commands are handled using the
// Secondary Display support in Avkon.
//
enum TLocSUPLSettingsCoverUiCmds
    {
    /**
     * Command for displaying a confirmation query dialog, which seeks confirmation from the 
     * user as to whether he wants to configure SUPL Settings.
     * Dialog Type : Text Query Dialog     
     * Hint String : "Secure positioning requires an access point. Define access point?"
     */
    ECmdSuplSettingsMissing = 1,

    /**
     * Command for displaying a confirmation query dialog, which seeks confirmation from the 
     * user as to whether he wants to enable SUPL .
     * Dialog Type : Text Query Dialog     
     * Hint String : "Secure positioning is not enabled. Do you want to enable?"
     */
    ECmdSuplStateMissing
    };

#endif      // LOCSUPLSETTINGSCOVERUI_H_            
// End of File
