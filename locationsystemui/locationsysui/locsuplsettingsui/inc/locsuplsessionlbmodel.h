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
* Description:  List box model for the SUPL session UI
*
*/

#ifndef C_LOCSUPLSESSIONLBMODEL_H_
#define C_LOCSUPLSESSIONLBMODEL_H_

// System Include
#include <e32base.h>
#include <bamdesca.h>  // MDesCArray

// Forward Declaration
class CLocSUPLSettingsUiEngine;
class CTriggerParams;

// Class Declaration
/**
 * Model for the SUPL Sessions UI container. 
 *
 * The model interacts with the SUPL Settings Engine inorder to obtain the latest
 * SUPL UI session array
 *
 * @lib locsuplsettings.lib
 * @since S60 v3.1
 */
class CLocSUPLSessionLBModel : public CBase,
						      public MDesCArray
	{
	public:
        /**
         * Two-phased constructor
         *
         * @param  aEngine                      Reference the SUPL Settings Engine
         * @return CLocSUPLSessionLBModel*     Reference to the object created
         */
        static CLocSUPLSessionLBModel* NewL( 
                        CLocSUPLSettingsUiEngine&     aEngine );

        /**
         * Two-phased constructor
         *
         * @param  aEngine                      Reference the SUPL Settings Engine         
         * @return CLocSUPLSessionLBModel*     Reference to the object created
         */
        static CLocSUPLSessionLBModel* NewLC( 
                        CLocSUPLSettingsUiEngine&     aEngine );
        
        /**
         * Destructor.
         */
        virtual ~CLocSUPLSessionLBModel();
    
    public: // New functions
    
        /**
         * Returns the session id array to get session attributes
         */
   		RArray<TInt64>& SessionIds();		        
		
        /**
         * Refresh the session id array list from the supl settings
         */
		void UpdateSessionIdsL();

        /**
         * Return the session name identified by session id
         */		
		TPtrC SessionName( TInt aIndex );
		
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
        CLocSUPLSessionLBModel( CLocSUPLSettingsUiEngine&     aEngine );
        
        /**
         * Second Phase constructor
         */
        void ConstructL();
        
        /**
         * Appends the session name and type to the buffer passed.
         * 
         * @param aPtr Buffer to which session name and type will be
         *             appended.
         * @param aIndex index to get session entry 
         */
		void AppendSuplSessionDetail( TDes& aPtr, TInt aIndex ) const;
		
    private:
        /**
        * Enumerations for Supl sessions type
        */
        enum	TSUPLSessionType       
            {
        /**
         * Supl session is of Terminal Initiaon Type
         */
            ESUPLSessionTIType = 0,
        /**
         * Supl session is of Network Initiaon Type
         */
            ESUPLSessionNIType
            };

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
         * SUPL Sessions Id
         * Owns.
         */
        RArray< TInt64 > 				iSessionIdList;
        
        /**
         * SUPL Sessions Name
         * Owns.
         */
        RPointerArray< HBufC >			iSessionNameList;
        
        /**
         * SUPL Sessions Type.
         * Owns.
         */
        RArray< TSUPLSessionType >					iSessionTypeList;
        
        /**
         * Unnamed session SUPL Sessions Name
         * Owns.
         */
        HBufC*			iUnnamedSuplActiveSessions;
	};

#endif // C_LOCSUPLSESSIONLBMODEL_H_

