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
* Description:  Declaration of Manager class for the BtGpsPsy string constants
*
*/



#ifndef C_LOCBTNOTIFIERCONSTANTSMANAGER_H
#define C_LOCBTNOTIFIERCONSTANTSMANAGER_H


#include <e32base.h>
#include <f32file.h>


/**
*  Forward Declarations
*/
class RResourceFile;


/**
 * Manager class for the BtGpsPsy string constants
 * This class reads all the pre-defined constants
 * from resource file on construction
 * 
 *  
 *  @since S60 5.1
 */
NONSHARABLE_CLASS ( CLocBtNotifierConstantsManager ): public CBase
    {

    public:
        
        /** 
        * Constructor
        */
        static CLocBtNotifierConstantsManager* NewL();
        
        
        /**
        * Destructor.
        */
        virtual ~CLocBtNotifierConstantsManager();

    private:
    
        /**
        * Second Phase of construction
        */
        void ConstructL();
        
        /**
        * Private Constructor
        */
        CLocBtNotifierConstantsManager();
        
        /**
        * Get localisable resource name
        * This function leaves if no localisable resource is found
        * @param   aFs File Session handle
        * @return  Resource filename
        */
        TFileName* GetLocalisableResourceNameL( RFs& aFs );
        
        /**
        * Get text from resource file
        * @param aResourceFile  A handle to the localisation resource file
        * @param aId            Resource id as mentioned in rss file
        * @return               Text associated with the resource id mentioned
        */
        HBufC* GetTextL( RResourceFile& aResourceFile, TInt aId );
        
    public:
    
        /**
        * Returns the Battery Low Dialog text
        * @return BatteryLowDialogText
        */
        TPtr GetBatteryLowDialogText();
        
        /**
        * Returns the Battery Full Dialog text
        * @return BatteryFullDialogText
        */
        TPtr GetBatteryFullDialogText();
        
        /**
        * Returns the Ext. Antenna Connected Dialog text
        * @return ExtAntennaConnectedDialogText
        */
        TPtr GetExtAntennaConnectedDialogText();
        
        /**
        * Returns the Ext. Antenna Disconnected Dialog text
        * @return ExtAntennaDisconnectedDialogText
        */
        TPtr GetExtAntennaDisconnectedDialogText();
        
        /**
        * Returns the Ext. Power Connected Dialog text
        * @return ExtPowerConnectedDialogText
        */
        TPtr GetExtPowerConnectedDialogText();
        
        /**
        * Returns the Ext. Power Disconnected Dialog text
        * @return ExtPowerDisconnectedDialogText
        */
        TPtr GetExtPowerDisconnectedDialogText();
        
        
        
    private:
    
        /**
        * Text for battery low dialog
        */
        HBufC*   iBatteryLowDialogText;
		
		/**
		* Text for battery full dialog
		*/
		HBufC*   iBatteryFullDialogText;
		
		/**
		* Text for ext. antenna connected dialog
		*/
		HBufC*   iExtAntennaConnectedDialogText;
		
		/**
		* Text for ext. antenna disconnected dialog 
		*/    
		HBufC*   iExtAntennaDisconnectedDialogText;
		
		/**
		* Text for ext. power connected dialog
		*/
		HBufC*   iExtPowerConnectedDialogText;
		
		/**
		* Text for ext. power disconnected dialog
		*/
		HBufC*   iExtPowerDisconnectedDialogText;



    };



#endif // C_LOCBTNOTIFIERCONSTANTSMANAGER_H
