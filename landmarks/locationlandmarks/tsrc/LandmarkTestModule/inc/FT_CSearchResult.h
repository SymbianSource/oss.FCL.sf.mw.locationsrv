/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Symbian Foundation License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.symbianfoundation.org/legal/sfl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description: 
*   ?description_line
*
*/



#ifndef CSEARCHRESULT_H
#define CSEARCHRESULT_H

//  INCLUDES
#include <e32base.h>
#include <EPos_CPosLandmark.h>

#include "Ft_CSearchResultBase.h"

class CSearchResult : public CSearchResultBase
    {

    public:  // Constructors and destructor
        
         /**
        * Two-phased constructor.
        */
        static CSearchResult* NewL(const TDesC& aDes); 

        /**
        * Destructor.
        */
        ~CSearchResult();      

    public: 

         TPtrC TextCriteria() const;
        
         const RArray<TUint>& PositionFields() const;

    private: 
        
        CSearchResult();

        void ConstructL(const TDesC& aDes);
        

    private:

         // By default, prohibit copy constructor
        CSearchResult(const CSearchResult&);
        // Prohibit assigment operator
        CSearchResult& operator=(const CSearchResult&);

        void ParsePositionFields(const TDesC& aDes);

        void Print();
        
        TBool Equals32(const TReal aValue1, const TReal aValue2, const TReal aAccuracy);
        
    private: // Data
        
        enum TValueIndex
			{
            ETextCriteria           =0,	
            EAttributes             =1,	
            EPositionFields         =2,
            ELmSearchResult         =3,
            ERedefined              =4
			};
        
        HBufC*                      iTextCriteria;
        
        RArray<TUint>               iPositionFields;
        
    };

#endif      // CSEARCHRESULT_H
            
// End of File
