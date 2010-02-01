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
* Description:  List box model for the SUPL server UI
*
*/

#ifndef C_LOCSUPLSERVERLBMODEL_H_
#define C_LOCSUPLSERVERLBMODEL_H_

// System Include
#include <e32base.h>
#include <bamdesca.h>  // MDesCArray

// Forward Declaration
class CLocSUPLSettingsUiEngine;
class CServerParams;

// Class Declaration
/**
 * Model for the SUPL Settings UI container. 
 *
 * The model interacts with the SUPL Settings Engine inorder to obtain the latest
 * SUPL UI Settings values
 *
 * @lib locsuplsettings.lib
 * @since S60 v3.1
 */
class CLocSUPLServerLBModel : public CBase,
						      public MDesCArray
	{
	public:
        /**
        * Enumerations for List box Entries
        */
        enum	TSUPLSettingsItemId        
            {
            ESUPLSettingsServerAddress = 0
            };
            	
	public:
        /**
         * Two-phased constructor
         *
         * @param  aEngine                      Reference the SUPL Settings Engine
         * @return CLocSUPLServerLBModel*     Reference to the object created
         */
        static CLocSUPLServerLBModel* NewL( 
                        CLocSUPLSettingsUiEngine&     aEngine );

        /**
         * Two-phased constructor
         *
         * @param  aEngine                      Reference the SUPL Settings Engine         
         * @return CLocSUPLServerLBModel*     Reference to the object created
         */
        static CLocSUPLServerLBModel* NewLC( 
                        CLocSUPLSettingsUiEngine&     aEngine );
        
        /**
         * Destructor.
         */
        virtual ~CLocSUPLServerLBModel();
    
    public: // New functions
    
        /**
         * Returns the slp id array to get server attributes
         */
   		RArray<TInt64>& SlpIds();		        
		
        /**
         * Refresh the slp id array list from the supl settings
         */
		void UpdateSlpIdsL();
		
    protected:
        /**
         * Inherited from MDesCArray
         */
        TInt MdcaCount() const;

        /**
         * Inherited from MDesCArray
         */
        TPtrC16 MdcaPoint( TInt aIndex ) const;
        				
    private:
        /**
         * C++ constructor.
         */
        CLocSUPLServerLBModel( CLocSUPLSettingsUiEngine&     aEngine );
        
        /**
         * Second Phase constructor
         */
        void ConstructL();
        
        /**
         * Appends the server address to the end of the buffer passed.
         * 
         * @param aPtr Buffer to which server attributes will be
         *             appended.
         * @param aIndex index to get server entry 
         */
		void AppendSuplServerDetail( TDes& aPtr, TInt aIndex ) const;
		
    private:
        /**
         * Format conversion buffer
         * Owns
         */
        HBufC16*                    	iBuffer;
        
        /**
         * Reference the SUPL Settings Engine
         */   
        CLocSUPLSettingsUiEngine&   	iEngine;
        
        /** 
         * Boolean value to indicate whethere SUPL settings usage
         * is present.
         */
        TBool                       	iSuplUsagePresent;
                
        /**
         * Boolean value to indicate whether SUPL address field
         * is present.
         */
        TBool                       	iServerAddressPresent;     

        /**
         * SUPL Servers SlpId
         * Owns.
         */
        RArray< TInt64 > 				iSlpIdList;
        
        /**
         * SUPL Server Addresses 
         * Owns.
         */
        RPointerArray< HBufC >			iSlpAddressList;
        
        /**
         * SUPL Servers with its attributes.
         * Owns.
         */
        RArray< TBool>					iEnableFlagList;
	};

#endif // C_LOCSUPLSERVERLBMODEL_H_

