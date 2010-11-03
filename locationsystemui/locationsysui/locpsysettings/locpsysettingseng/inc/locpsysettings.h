/*
* Copyright (c) 2002,2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementes settings extension API
*
*/



#ifndef CLOCPSYSETTINGS_H__
#define CLOCPSYSETTINGS_H__

//  INCLUDES
#include <e32base.h>
#include <lbs/epos_mposmodulesobserver.h> 
#include <lbs/epos_cposmoduleidlist.h>

#include "locpsyinfo.h"
#include "locpsysettingsinterface.h"

/// FORWARD DECLARATION
class CPosModules;
class CPosModuleIdList;
class MLocPsyEngObserver;
class CLocPsyConfigUiServiceClientImpl;

// CLASS DECLARATION

/**
*  An location engine API for location application.
*  Provides access to location settings.
*  @lib loceng.lib
*  @since 2.1
*/
class CLocPsySettings : public CBase, 
                        public MLocPsySettings, 
						public MPosModulesObserver
    {
    private:
        /**
        * C++ default constructor.
        */
        CLocPsySettings();
        
    public:
        /**
        * Two-phased constructor.
        */
        static CLocPsySettings* NewL();

        /**
        * Destructor.
        */
        virtual ~CLocPsySettings();

    private:
        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    public:
        /**
         * Accessor function for obtaining the number of PSYs
         * 
         * @return TInt Number of PSYs
         */    
        inline TInt PSYCount();
        
        /**
         * Accessor function for obtaining the PSY information
         * 
         * @return CLocPsyInfo* PSY information
         */         
        CLocPsyInfo* GetPsyL( TInt aIndex );
        
        /**
         * Enable a particular PSY
         * 
         * @param TInt Index of the PSY
         */          
		void EnablePsyL( TInt aIndex );
		
        /**
         * Disable a particular PSY
         * 
         * @param TInt Index of the PSY
         */ 		
		void DisablePsyL( TInt aIndex );
		
        /**
         * Launch the configuration UI for a particular PSY
         * 
         * @param TInt Index of the PSY
         */ 		
		void ConfigurePsyL( TInt                        aIndex,
		                    MAknServerAppExitObserver&  aObserver );
		    
        /**
         * Accessor function to determine whether the PSY is enabled
         * 
         * @param  TInt     Index of the PSY
         * @return TBool    Boolean value to indicate whether the PSY 
         *                  has been enabled / disabled
         */ 		                    
		TBool IsPsyEnabled( TInt aIndex );
	
        /**
         * Accessor function to determine whether the PSY is configurable
         * 
         * @param  TInt     Index of the PSY
         * @return TBool    Boolean value to indicate whether the PSY 
         *                  can be configured
         */		
		TBool IsPsyConfigurable( TInt aIndex );
		
        /**
         * Close the launched configuration
         *
         */ 		
		void ClosePsyConfigurationApp();
	
        /**
         * Accessor function for setting the PSY Settings UI Engine 
         * Observer
         * 
         * @param aObserver Observer to the PSY Settings UI Engine
         */ 		
        void SetObserver(MLocPsyEngObserver& aObserver);
        
        /**
         * Accessor function for removing the PSY Settings UI Engine 
         * Observer
         *
         */        
        void RemoveModelObserver();

    public:
        /**
         * MPosModulesObserver
         */
        virtual void HandleSettingsChangeL(TPosModulesEvent aEvent);
		
    private:
        /// Own: Pointer to modules db
        CPosModules* iModuleDb;
        /// Own: Pointer to modules id list
        CPosModuleIdList* iPsyList;
        /// Own: psy info
        CLocPsyInfo* iPsy;
        /// Ref: Observer interface to location application engine
        MLocPsyEngObserver* iObserver;
        /// Own: Handle to the PSY configuration UI application. 
        ///      Used to close the configuration application if the PSY in uninstalled.
        CLocPsyConfigUiServiceClientImpl* iServClient;
    };

TInt CLocPsySettings::PSYCount()
    {
    return iPsyList->Count();
    }


#endif      // __CLOCPSYSETTINGS_H__   
            
// End of File
