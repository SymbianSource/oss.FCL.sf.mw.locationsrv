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
* Description:  Location settings can be modified 
*                and read with help of this API.
*
*/



#ifndef MLOCPSYSETTINGS_H_
#define MLOCPSYSETTINGS_H_

// INCLUDES
#include <lbscommon.h> // TPositionModuleId

// FORWARD DECLARATIONS
class MLocPsyEngObserver;
class CLocPsyInfo;
class MAknServerAppExitObserver;

// CLASS DECLARATION

/**
*  This location application engine API is only used by location application.
*  Location settings can be modified and read with help of this API.
*
*  @lib locpsysettingseng.lib
*  @since 3.1
*/
class MLocPsySettings
    {
    public:  // Constructors and destructor
        
        /**
        * Destructor.
        */
        virtual ~MLocPsySettings(){};

    public: // New functions
        /**
        * Lists all PSYs
        * @since 3.1
        * @return number of PSYs
        */
        virtual TInt PSYCount() = 0;  

        /**
        * Get psy information
        * @since 3.1
        * @param aIndex 
        * @return psy information
        */
        virtual CLocPsyInfo* GetPsyL(TInt aIndex ) = 0;  
        
        /**
        * Enable psy
        * @since 3.1
        * @param psy
        * @return nothing. Leaves on Error.
        */
        virtual void EnablePsyL( TInt aIndex ) = 0;

        /**
        * Disable psy
        * @since 3.1
        * @param psy
        * @return nothing. Leaves on Error.
        */
        virtual void DisablePsyL( TInt aIndex ) = 0;


        /**
        * Start psy configuration UI
        * @since 3.1
        * @param aIndex 
        * @return none
        */
		virtual void ConfigurePsyL(TInt aIndex, 
								   MAknServerAppExitObserver& aObserver ) = 0;
		
        /**
        * Close psy configuration UI
        * @since 3.1
        * @param none 
        * @return none
        */
		virtual void ClosePsyConfigurationApp() = 0;

        /**
        * Get psy information
        * @since 3.1
        * @param aIndex 
        * @return psy information
        */
        virtual TBool IsPsyConfigurable( TInt aIndex ) = 0;
        
        /**
        * Get psy information
        * @since 3.1
        * @param aIndex 
        * @return psy information
        */
        virtual TBool IsPsyEnabled( TInt aIndex ) = 0;

        /**
        * Sets settings observer
        * @since 3.1
        * @param aObserver A reference to observer
        */
        virtual void SetObserver(MLocPsyEngObserver& aObserver) = 0;

        /**
        * Removes settings observer        
        * @since 3.1
        */        
        virtual void RemoveModelObserver() = 0;
    };

#endif      // MLOCPSYSETTINGS_H_ 
            
// End of File
