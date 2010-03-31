/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Periodic info structure
*
*/


#ifndef CLPDPERIODICREQINFO_H
#define CLPDPERIODICREQINFO_H

#include <e32base.h>
#include <EPos_CPosPrivacyNotifier.h>

/**
*  Periodic info structure to Que.
*
*  @since S60 9.2tb
*/
class TLpdPeriodicReqInfo
    { 
    public :        
		/**
        * Copy constructor
        */
        TLpdPeriodicReqInfo(TPosQNRequestId aPrivacyId, TInt64 aSUPLSessionId)
	        {
	        iPrivacyId = aPrivacyId;
	        iSUPLSessionId = aSUPLSessionId;
	        };
			
		/**
        * inline function used for comparision
        */
        static inline TInt MatchSession(const TLpdPeriodicReqInfo& aFirst, const TLpdPeriodicReqInfo& aSecond)
            {
            if ( aFirst.iSUPLSessionId == aSecond.iSUPLSessionId )
                return 1;
            return 0;
            };
            
        /**
        * inline function used for comparision
        */
        static inline TInt MatchPrivacy(const TLpdPeriodicReqInfo& aFirst, const TLpdPeriodicReqInfo& aSecond)
            {
            if ( aFirst.iPrivacyId == aSecond.iPrivacyId )
                return 1;
            return 0;
            };
			
    public :
		// Periodic Notification Request Information like TPosQNRequestId and SUPLSessionId
    	TPosQNRequestId iPrivacyId;
    	TInt64 iSUPLSessionId;
 };

#endif //CLPDPERIODICREQINFO_H

// End of File
