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
* Description: This  is a header file for engine class of Location position settings
*
*/


#ifndef CLOCPSYENG_H_
#define CLOCPSYENG_H_

//  INCLUDES
#include <e32base.h>

// FORWARD DECLARATION

class CLocPsySettings;
class MLocPsySettings;

/**
* Enumerations for model creation
*/
enum TLocModelType
    {
    ELocPsySettings, 
    };

// CLASS DECLARATION
/**
 *  Implements entry point class to LocEng.dll
 *
 *  @lib locpsysettingseng.dll
 *  @since 3.1
 */
class CLocPsyEng : public CBase
    {
    private:
       /**
        * C++ default constructor.
        */
        CLocPsyEng();
        
    public:
       /**
        * Two-phased constructor.
        *
        * @return CLocPsyEng object.
        */
        IMPORT_C static CLocPsyEng* NewL();
        
       /**
        * Destructor.
        */
        IMPORT_C virtual ~CLocPsyEng();
    
    public: 
       /**
        * Settings model getter.
        * Model is NOT created if it does not exist.
        * Ownership not transferred.
        * @return pointer to the settings model
        */
        IMPORT_C MLocPsySettings* SettingsModel();
        /**
        * Model creator.
        * This method creates and initializes models.
        * Model creation method is used if no separate 
        * handle to model needed. 
        * @param aModel model identificator.
        */
        IMPORT_C void CreateModelL( TLocModelType aModel );

        /**
        * Engine can contain multiple models.
        * With help of this function certain model can be deleted.
        * @param aModelType Model type to be deleted
        */
        IMPORT_C void RemoveModel(TLocModelType aModelType );
                       
    private:    // data
        // OWn: Settings model
        CLocPsySettings* iSettings; 
    };

#endif // CLOCPSYENG_H_

// End of File
