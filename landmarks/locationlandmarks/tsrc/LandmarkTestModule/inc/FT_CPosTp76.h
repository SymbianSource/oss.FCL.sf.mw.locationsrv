/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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


#ifndef CPOSTp76_H
#define CPOSTp76_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"

// CLASS DECLARATION

/**
*  Test procedure 76
*/
class CPosTp76 :public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        CPosTp76() : CLandmarkTestProcedureBase(76) {};
        
        /**
        * Destructor.
        */
        ~CPosTp76() {};

    public: // Functions from base classes

        /**
        * From CUtfwTestBase
        * Retrieves the test module name
        * @param aName The name of the test module.
        */
        void GetName(
            TDes&   aName
            ) const;

		void CloseTest();
        /**
        * From CLandmarkTestProcedureBase
        * Runs the test in the module
        */
        void StartL();

	private:
        /**
        * Use a corrupt global categories resource file
        */ 
		void TestCorruptGlobalCategoriesFileL();

    };

#endif      // CPOSTp76_H
            
// End of File
