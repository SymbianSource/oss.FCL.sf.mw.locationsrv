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
* Description:  Location Psy info class.
*
*/



#ifndef CLOCPSYINFO_H__
#define CLOCPSYINFO_H__

//  INCLUDES
#include <e32std.h>
#include <lbscommon.h>

// CLASS DECLARATION

/**
* Location Psy info class.
* A data container class which holds information about psy name and id.
*/
class CLocPsyInfo: public CBase
    {
    public: // interface
        /**
        * Returns the Location Psy id of this PSY module.
        */
        IMPORT_C TPositionModuleId PsyId() const;

        /**
         * Returns the name of psy module.
         */
        IMPORT_C const TDesC& PsyName() const;
        
        /**
        * C++ default constructor.
        */
        CLocPsyInfo();

        /**
        * Destructor.
        */        
        virtual ~CLocPsyInfo();
	
	public: // Non-exported functions
		/**
		 * Set the PsyId
		 */
		void SetPsyId(const TPositionModuleId& aPsyId);

		/**
		 * Set the Psy Name
		 */
		void SetPsyNameL(const TDesC& aBuf);
        
    private:  // Unimplemented functions
        /// Private, unimplemented copy constructor
        CLocPsyInfo(const CLocPsyInfo&);
        /// Private, unimplemented assignment operator
        CLocPsyInfo& operator=(const CLocPsyInfo&);

    public: // data members
        // Own: unique psy id
        TPositionModuleId iPsyId;
        // Own: PSY name
        HBufC* iPsyName;        
    };

#endif      // CLOCPSYINFO_H__
            
// End of File
