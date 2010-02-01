/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Application to launch Positioning Settings for Location Centre
*
*/


#ifndef C_LOCPOSSETTINGSAPP_H
#define C_LOCPOSSETTINGSAPP_H

// System Includes
#include <aknapp.h>

/**
 *  Application class
 *  Application class creates document class object and return application UID
 *
 *  @lib locpossettingsapp.exe
 *  @since S60 v 5.0
 */
 class CLocPosSettingsApp : public CAknApplication
    {
 private:
    /**
    * From CApaApplication, returns application's UID (KCLocPosSettingsAppUid3).
    * @return The value of KCLocPosSettingsAppUid3.
    */
    TUid AppDllUid() const;
        
    /**
    * From CApaApplication, creates CLocPosSettingsAppDocument object.
    * @return A pointer to the created document object.
    */
    CApaDocument* CreateDocumentL();
    };

#endif // C_LOCPOSSETTINGSAPP_H
