/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  The declaration for AppUi class of Settings UI Server
*
*/


#ifndef C_LOCSETTINGSUISRVAPPUI_H
#define C_LOCSETTINGSUISRVAPPUI_H

#include <aknViewAppUi.h>

/**
 *  The AppUi class for the Location Settings UI Application Server.
 *
 *  @lib 
 *  @since S60 v3.1
 */
NONSHARABLE_CLASS(CLocSettingsUISrvAppUi) : public CAknViewAppUi
    {
public:
    /**
     * Destructor
     */
    virtual ~CLocSettingsUISrvAppUi();

    /**
     * Constructor
     */
    CLocSettingsUISrvAppUi();

    /**
     * Symbian 2nd Phase Constructor
     * Leaves in case of Error. Will be called by Framework.
     * Hence there is no 2 Phase Constructor. Also the ConstructL
     * has to be kept public.
     *
     * @since S60 v3.1
     * @return None
     */
    void ConstructL();
    
    /**
     * Exposes the Full Screen application setting
     */
    void SetFullScreenState( TBool aFullScreenState );
    
protected:

    /**
     * from base class CEikAppUi
     * Overriding the base class implementation to implement Exit
     * functionality.
     *
     * @since S60 v3.1
     * @param aCommand the Command Id of the command to be executed.
     * @returns None
     */
    virtual void HandleCommandL(TInt aCommand);
    };

#endif // C_LOCSETTINGSUISRVAPPUI_H
