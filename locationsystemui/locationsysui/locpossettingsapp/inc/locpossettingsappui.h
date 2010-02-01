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
* Description:  Application to launch Positioning Settings for Location Centre
*
*/


#ifndef C_LOCPOSSETTINGSAPPUI_H
#define C_LOCPOSSETTINGSAPPUI_H

// System Includes
#include <eikapp.h>
#include <eikdoc.h>
#include <e32std.h>
#include <coeccntx.h>
#include <aknappui.h>

// User Includes
#include "locsysuiview.h"

/**
 *  Application UI class
 *  Provides support for the following features:
 *  - EIKON control architecture
 *
 *  @lib locpossettingsapp.exe
 *  @since S60 v 5.0
 */
class CLosPosSettingsAppUi : public CAknViewAppUi
    {
public:   
    /**
     * Second phase of the two phase constructor.
     */       
    void ConstructL();
        
    /**
     * Destructor.
     */
    virtual ~CLosPosSettingsAppUi();
    
public: // Inherited from Base classes    
    /**
     * Inherited from CAknViewAppUi
     */
    void HandleCommandL(TInt aCommand); 
    };

#endif // C_LOCPOSSETTINGSAPPUI_H
