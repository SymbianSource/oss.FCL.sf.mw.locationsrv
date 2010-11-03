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
* Description:  The declaration for document class of Settings UI Server
*
*/


#ifndef C_LOCSETTINGSUIDOC_H
#define C_LOCSETTINGSUIDOC_H

#include <AknDoc.h>
#include <eikapp.h>

//Forward declarations
class CEikAppUi;
class CLocSysUiEngine;


/**
 *  The Document class for Location Settings UI Application Server
 *
 *  @lib 
 *  @since S60 v3.1
 */
NONSHARABLE_CLASS(CLocSettingsUISrvDoc) : public CAknDocument
    {
public:
    /**
     * Constructor
     *
     * @since S60 v3.1
     * @param aApp Reference to the CEikApplication object that is creating
     * this document class instance.
     * @return None
     */
    CLocSettingsUISrvDoc(CEikApplication &aApp);

    /**
     * Destructor
     */
    virtual ~CLocSettingsUISrvDoc();
    
    /**
     * Set the Engine Instance
     * Once set the ownership is transferred to the AppUi.
     *
     * @since S60 v3.1
     * @param Pointer to the instance of the Engine.
     * @return None
     */
    void SetEngineInstance(CLocSysUiEngine* aEngine);
    
    /**
     * Accessor method to get the Engine Instance
     * 
     *
     * @since S60 v3.1
     * @return Pointer to the Engine Instance.
     */
    CLocSysUiEngine* Engine();
    
protected:
    /**
     * method to create AppUi object.
     * The AppUi object returned is partially constructed. The Framework
     * will call the ConstructL on AppUi object.
     *
     * @since S60 v3.1
     * @param None
     * @return An Instance of the AppUi class.
     */
    CEikAppUi* CreateAppUiL();

private: // data
    /**
     * The Engine instance created by the CLocSettingsUISrvLaunchAO.
     * It is stored in the Document class since the destruction of the 
     * Engine needs to be delayed.
     * Owns
     */
    CLocSysUiEngine* iEngine;
    };

#endif // C_LOCSETTINGSUIDOC_H
