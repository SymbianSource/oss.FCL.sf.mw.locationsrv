/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Cover UI command definitions for SUPL queries
*
*/

#ifndef EPOS_OMASUPLQUERYCOVERUI_H_
#define EPOS_OMASUPLQUERYCOVERUI_H_

// System Includes
#include <e32base.h>

// The category UID for the commands in this header file.
// UID of the SUPL Query Notifier Dll ( epos_omasuplnotifier.dll )
//
const TUid KCatUidSuplQuery = { 0x10009BF6 };


// Dialogs shown by SUPL Notifers. These commands are handled using the
// Secondary Display support in Avkon.
//
enum TEposSUPLSettingsCoverUiCmds
    {
    /**
     * Command for displaying a confirmation query dialog, which seeks confirmation from the 
     * user as to whether he wants to use SUPL. The dialog is displayed when the user is
     * in his home network.
     * Dialog Type : Message Query Dialog     
     * Hint String : "Connection to network positioning server is requested. Proceed?"
     *               "Server Settings ?" Link     
     */
    ECmdSuplHomeNetwork = 1,
    
    /**
     * Command for displaying a confirmation query dialog, which seeks confirmation from the 
     * user as to whether he wants to use SUPL. The dialog is displayed when the user is
     * not in his home network.
     * Dialog Type : Message Query Dialog with link     
     * Hint String : "Connection to network positioning server is requested. You are outside home network. Proceed?"
     *               "Server Settings ?" Link
     */
    ECmdSuplRoamingNetwork = 2,    
    
    /**
     * Command for displaying a timeout query dialog, which informs the user about the 
     * timeout of SUPL Request. The dialog is displayed when the SUPL connection times out.
     * Dialog Type : Query Dialog     
     * Hint String : "Positioning server SUPL request timed out. Network connection not started."
     */
    ECmdSuplTimeOut = 3    
    };

#endif      // EPOS_OMASUPLQUERYCOVERUI_H_            
// End of File
