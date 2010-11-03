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
* Description:  List box model for the SUPL Settings UI
*
*/

#ifndef C_LOCSUPLSETTINGSLBMODEL_H_
#define C_LOCSUPLSETTINGSLBMODEL_H_

// System Include
#include <e32base.h>
#include <bamdesca.h>  // MDesCArray

// Forward Declaration
class CLocSUPLSettingsUiEngine;


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
class CLocSUPLSettingsLBModel : public CBase,
						        public MDesCArray
	{
	public:
        /**
        * Enumerations for List box Entries
        */
        enum	TSUPLSettingsItemId        
            {
            ESUPLSettingsServersDetail = 0,
            ESUPLSettingsActiveSessions
            };
            	
	public:
        /**
         * Two-phased constructor
         *
         * @param  aEngine                      Reference the SUPL Settings Engine
         * @return CLocSUPLSettingsLBModel*     Reference to the object created
         */
        static CLocSUPLSettingsLBModel* NewL( 
                        CLocSUPLSettingsUiEngine&     aEngine );

        /**
         * Two-phased constructor
         *
         * @param  aEngine                      Reference the SUPL Settings Engine         
         * @return CLocSUPLSettingsLBModel*     Reference to the object created
         */
        static CLocSUPLSettingsLBModel* NewLC( 
                        CLocSUPLSettingsUiEngine&     aEngine );
        
        /**
         * Destructor.
         */
        virtual ~CLocSUPLSettingsLBModel();
        
        
        /**
         * Resolves the list box index to the SUPL settings item.
         *
         * @param  aListboxIndex        List box item index.
         * @return TSUPLSettingsItemId  SUPL settings item id.
         */
        TSUPLSettingsItemId ResolveSettingsItem( TInt aListboxIndex ) const;
        
        /**
         * Returns the Active session count.
         *
         * @return TInt  SUPL Active sessions count.
         */
        TInt ActiveSessionCount( ) const;
                
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
        CLocSUPLSettingsLBModel( CLocSUPLSettingsUiEngine&     aEngine );
        
        /**
         * Second Phase constructor
         */
        void ConstructL();
        

        /**
         * Packs the Server Address to the end of the buffer passed.
         * 
         * @param aPtr Buffer to which the Access point needs to be 
         *             appended.
         */
        void AppendSuplServerDetail( TDes16& aPtr ) const;  
     
        /**
         * Packs the SUPL access points count to the end of the buffer passed.
         * 
         * @param aPtr Buffer to which the SUPL access points count needs to be 
         *             appended.
         */    
        void AppendSuplActiveSessionsDetailL( TDes16& aPtr ) const;
                 
    private:
        /**
         * Format conversion buffer
         * Owns
         */
        HBufC16*                    iBuffer;
        
        /**
         * Title Text Buffer for Server Detail Heading
         * Owns
         */
        HBufC16*                    iServerDetailTitle;
        
        
        /**
         * SUPL Active Sessions count.
         */
        mutable TInt iSessionCount;
         
        /**
         * Default Server Address 
         * Owns
         */
        HBufC16*                    iSuplServerDetail;
        

        /**
         * Reference the SUPL Settings Engine
         */   
        CLocSUPLSettingsUiEngine&   iEngine;
        
                
        /**
         * Boolean value to indicate whether SUPL address field
         * is present.
         */
        TBool                       iServerAddressPresent;               
	};

#endif // C_LOCSUPLSETTINGSLBMODEL_H_

