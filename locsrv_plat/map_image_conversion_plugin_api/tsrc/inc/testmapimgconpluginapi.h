/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Ctestmapimgconpluginapi class
*
*/



#ifndef TESTMAPIMGCONPLUGINAPI_H
#define TESTMAPIMGCONPLUGINAPI_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>



// Logging path
_LIT( KtestmapimgconpluginapiLogPath, "\\logs\\testframework\\testmapimgconpluginapi\\" ); 
// Log file
_LIT( KtestmapimgconpluginapiLogFile, "testmapimgconpluginapi.txt" ); 
_LIT( KtestmapimgconpluginapiLogFileWithTitle, "testmapimgconpluginapi_[%S].txt" );


// FORWARD DECLARATIONS
class Ctestmapimgconpluginapi;


/**
*  Ctestmapimgconpluginapi test class for STIF Test Framework TestScripter.
*
*  @lib ?library
*  @since s60 v5.0
*/
NONSHARABLE_CLASS(Ctestmapimgconpluginapi) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static Ctestmapimgconpluginapi* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~Ctestmapimgconpluginapi();

    public: // Functions from base classes

        /**
        * From CScriptBase Runs a script line.
        * @param aItem Script line containing method name and parameters
        * @return Symbian OS error code
        */
        virtual TInt RunMethodL( CStifItemParser& aItem );


    private:

        /**
        * C++ default constructor.
        */
        Ctestmapimgconpluginapi( CTestModuleIf& aTestModuleIf );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
        * Frees all resources allocated from test methods.
        */
        void Delete();

        /**
        * Test methods are listed below. 
        */

        /**
        * This test case will test MapImageParams method of CMnCoordinateConverterBase
        * class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CMnCoordinateConverterBase_MapImageParams( CStifItemParser& aItem );
        
        /**
        * This test case will test GetImageCoordinate method of CMnCoordinateConverterBase
        * class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CMnCoordinateConverterBase_GetImageCoordinate( CStifItemParser& aItem );
        
        /**
        * This test case will test GetWorldCoordinate method of CMnCoordinateConverterBase
        * class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CMnCoordinateConverterBase_GetWorldCoordinate( CStifItemParser& aItem );
		
		


    };

#endif      // TESTMAPIMGCONPLUGINAPI_H

// End of File
