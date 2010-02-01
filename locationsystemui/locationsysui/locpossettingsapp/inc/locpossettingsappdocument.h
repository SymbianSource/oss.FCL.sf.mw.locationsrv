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
* Description:  Application to launch Positioning Settings for Location Center
*
*/


#ifndef C_LOCPOSSETTINGSAPPDOCUMENT_H
#define C_LOCPOSSETTINGSAPPDOCUMENT_H

// System Includes
#include <AknDoc.h>

// Forward Declarations
class  CEikAppUi;

/**
 *  Positioning Settings Application Document Class
 *
 *  @lib locpossettingsapp.exe
 *  @since S60 v 5.0
 */
class CLocPosSettingsAppDocument : public CAknDocument
    {
public:
    /**
     * Two-phased constructor.
     * @param CEikApplication& EikApplication reference
     * @return pointer to CLocPosSettingsAppDocument
     */
    static CLocPosSettingsAppDocument* NewL(CEikApplication& aApp);
    
   /**
    * Destructor.
    */
    virtual ~CLocPosSettingsAppDocument();

private:
	/**
	 * Overloaded C++ constructor
	 */
    CLocPosSettingsAppDocument(CEikApplication& aApp);

	/**
	 * Second phase of the two phase constuctor
	 */
    void ConstructL();

private: // Inherited from Base classes
   /**
    * From CEikDocument, create CLocPosSettingsAppUi "App UI" object.
    */
    CEikAppUi* CreateAppUiL();
  };

#endif // C_LOCPOSSETTINGSAPPDOCUMENT_H
