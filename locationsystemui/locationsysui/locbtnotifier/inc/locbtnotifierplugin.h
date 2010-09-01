/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CLocBtNotifierPlugIn class declaration
*
*/



#ifndef C_LOCBTNOTIFIERPLUGIN_H
#define C_LOCBTNOTIFIERPLUGIN_H


#include <e32base.h> 
#include <cfactionplugin.h>


#include "locbtnotifierconstantsmanager.h"

/**
* Forward Declarations
*/
class CAknGlobalNote;
class CLocBtNotifierPlugIn;



/**
 *  LocBtNotifier action plugin.
 *  Implementation of the Context Framework Action Plugin.
 *  Specifies the action for a change in the BtGpsPsy PS keys.
 *  ShowMsg Action shows appropriate note on change of BtGpsPsy PS Keys
 *
 *  @lib 
 *  @since S60 5.1
 */
NONSHARABLE_CLASS ( CLocBtNotifierPlugIn ) : public CCFActionPlugIn			
    {

    public:
        
        /**
        * Enumeration used to determine Battery Key State
        */
        enum TBtGpsPsyBatteryKeyState
	        {
	        /**
	        * Indicates the battery state when the plugin is loaded
	        */
	        EBatteryStateUnInitialized 		= -3,
	        /**
	        * Indicates the battery state when plugin is initialized after 
	        * the first call from the Context Framework Source plugin
	        */
	        EBatteryStateInitialized,
	        /**
	        * Indicates that the BT GPS device battery state is not applicable
	        */
	        EBatteryStateNotApplicable,
	        /**
	        * Indicates that BT GPS Device battery state is normal
	        */
	        EBatteryStateNormal,
	        /**
	        * Indicates that BT GPS Device battery state is low
	        */ 
	        EBatteryStateLow,
	        /**
	        * Indicates that BT GPS Device battery state is full
	        */
	        EBatteryStateFull 
	        };
        
        /**
        * Enumeration used to determine Antenna Key State
        */
        enum TBtGpsPsyAntennaKeyState
            {
            /**
	        * Indicates the ext Antenna state when the plugin is loaded
	        */
        	EAntennaStateUnInitialized		= -3,
        	/**
	        * Indicates Antenna state when the plugin is initialized 
	        * after the first call from the Context Framework Source plugin
	        */
        	EAntennaStateInitialized,
        	/**
        	* Indicates that the Ext Antenna State is not applicable
        	*/
        	EAntennaStateNotApplicable,
        	/**
        	* Indicates that the Ext Antenna is disconnected from the
        	* BT GPS Device
        	*/
        	EExtAntennaOff,
        	/**
        	* Indicates that the Ext Antenna is connected to the
        	* BT GPS Device
        	*/
        	EExtAntennaOn 
        	};
        
        /**
        * Enumeration used to determine Power Key State
        */
        enum TBtGpsPsyPowerKeyState
        	{
        	/**
	        * Indicates the ext Power state when the plugin is loaded
	        */
        	EPowerStateUnInitialized		= -3,
        	/**
	        * Indicates Power state when the plugin is initialized 
	        * after the first call from the Context Framework Source plugin
	        */
        	EPowerStateInitialized,
        	/**
        	* Indicates that the Ext Power State is not applicable
        	*/
        	EPowerStateNotApplicable,
        	/**
        	* Indicates that the Charger is disconnected from the
        	* BT GPS Device
        	*/
        	EExtPowerOff,
        	/**
        	* Indicates that the Charger is connected to the
        	* BT GPS Device
        	*/
        	EExtPowerOn
        	};
        
        /** 
        * Constructor
        */
        static CLocBtNotifierPlugIn* NewL();
        
        /**
        * Destructor.
        */
        ~CLocBtNotifierPlugIn();

    private:
    
        /** 
        * from base class CCFActionPlugIn
        */
        void InitializeL();
        
        /** 
        * from base class CCFActionPlugIn
        */
        TExecutionTime ExecuteL( CCFActionIndication* aActionIndication );
        
        /** 
        * from base class CCFActionPlugIn
        */
        void GetActionsL( CDesCArray& aActionList ) const;
        
        /** 
        * from base class CCFActionPlugIn
        */
        const TSecurityPolicy& SecurityPolicy() const;
        
   
    private:
        
        /**
        * Private Constructor
        */
        CLocBtNotifierPlugIn();
        
          		
    	/**
    	* Checks if there is change in Battery key state with respect to the  
    	* previous notification and proceeds to show appropriate note
    	* @param aValue - value of the action parameter
    	*/
    	void HandleBatteryKeyStateL( TDesC& aValue );
    		
    	/**
    	* Checks the Cen Rep key settings before displaying the note for
    	* any Battery key change
    	* @param aState - enum value based on the PS key that has changed
    	*/
    	void ShowBatteryNoteL( TBtGpsPsyBatteryKeyState aState );
    		
    	/**
    	* Verifies if the battery note can be shown 
    	* @param aState - enum value based on the PS key that has changed
    	* @return ETrue if note can be displayed  or
    	*         EFalse if note should not be displayed
    	*/
    	TBool IsBatteryNoteShownL( TBtGpsPsyBatteryKeyState aState );
    		
    	/**
    	* Checks if there is change in Antenna key state with respect to the  
    	* previous notification and proceeds to show appropriate note
    	* @param aValue - value of the action parameter
    	*/
    	void HandleAntennaKeyStateL( TDesC& aValue );
    		
    	/**
    	* Checks the Cen Rep key settings before displaying the note for
    	* any Antenna key change
    	* @param aState - enum value based on the PS key that has changed
    	*/
    	void ShowAntennaNoteL( TBtGpsPsyAntennaKeyState aState );
    		
    	/**
    	* Verifies if the Antenna note can be shown 
    	* @param aState - enum value based on the PS key that has changed
    	* @return ETrue if note can be displayed  or
    	*         EFalse if note should not be displayed
    	*/
    	TBool IsAntennaNoteShownL( TBtGpsPsyAntennaKeyState aState );
    		
    	/**
    	* Checks if there is change in Power key state with respect to the  
    	* previous notification and proceeds to show appropriate note
    	* @param aValue - value of the action parameter
    	*/
    	void HandlePowerKeyStateL( TDesC& aValue );
    		
    	/**
    	* Checks the Cen Rep key settings before displaying the note for
    	* any Power key change
    	* @param aState - enum value based on the PS key that has changed
    	*/
    	void ShowPowerNoteL( TBtGpsPsyPowerKeyState aState );
    		
    	/**
    	* Verifies if the Power note can be shown 
    	* @param aState - enum value based on the PS key that has changed
    	* @return ETrue if note can be displayed  or
    	*         EFalse if note should not be displayed
    	*/
    	TBool IsPowerNoteShownL( TBtGpsPsyPowerKeyState aState );
    		
    	/**
    	* Verifies if the dialog can be shown depending on the CenRep key value
    	* @param in - aKey - CenRep Key whose value needs to be checked
    	* @param out - aShown - Contains value of the key either 0 or 1
    	*/
    	void IsDialogShownL( TUint32 aKey, TBool& aShown );
    		
    	    		
    		
    private: 
    
        /**
        * Own : Avkon Global Note
        */
        CAknGlobalNote* iGlobalNote;
        
        /**
        * Own : CLocBtNotifierConstantsManager handler
        */
        CLocBtNotifierConstantsManager* iConstantsManager;
        
        /**
        * Stores the previous BluetoothGpsPsyBatteryState 
        */
        TBtGpsPsyBatteryKeyState iBatteryStatus;
        
        /**
        * Stores the previous BluetoothGpsPsyExtAntennaState 
        */
        TBtGpsPsyAntennaKeyState iAntennaStatus;
        
        /**
        * Stores the previous BluetoothGpsPsyExtPowerState 
        */
        TBtGpsPsyPowerKeyState iPowerStatus;
        
        /**
        * Pointer to the text to be displayed in the information note
        */
        TPtr16 iInfoText;
        
            
    };




#endif // C_LOCBTNOTIFIERPLUGIN_H
