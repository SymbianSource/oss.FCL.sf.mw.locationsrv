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


//  INCLUDES

#include "FT_CPosTp136.h"
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLmDatabaseManager.h>
#include <EPos_CPosLandmarkParser.h>
           
// ================= CONSTANTS =======================

    _LIT(KLMXFile, "c:\\system\\test\\testdata\\Tp136.xml");
    _LIT(KGPXFile, "c:\\system\\test\\testdata\\Tp136.gpx");
    
    _LIT(KEmpty, "");

    _LIT(K1, "Category nr1 added in TP48");
        
    _LIT(K2, "L�kare");
    _LIT(K3, "Sjukhus");
    _LIT(K4, "Restaurant");
    _LIT(K5, "Shop");
    
    _LIT(K6, "Fris�r");
    
    _LIT(K7, "Dagis");
    _LIT(K8, "BurgerKing");
    _LIT(K9, "Hotel");
    _LIT(K10, "Car park");
    
    _LIT(K11, "Garage");
    _LIT(K12, "Bus stop");
    _LIT(K13, "BurgerKing");
    
    _LIT(K14, "Category nr2 added in TP48");
    _LIT(K15_1, "OutSight");
    const TInt KNrOfLandmarks = 8;
                                                    
// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp136::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp136::CloseTest()
    {
    // Release ECOM stuff
    ReleaseLandmarkResources();
    
    delete iOperation;iOperation=NULL;
    }

// ---------------------------------------------------------
// CPosTp136::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp136::StartL()
    {
    iErrorsFound = 0;
    
    // LMX
    TestParseL(KLMXFile, KLmxMimeType);
    TestImportL(KLMXFile, KLmxMimeType);
    
    // GPX
    TestParseL(KGPXFile, KGpxMimeType);
    TestImportL(KGPXFile, KGpxMimeType);
    
    if (iErrorsFound != KErrNone)
        {
        iLog->Log(_L("Errors found in TP136"));
        User::Leave(-1);
        }
	}

// ---------------------------------------------------------
// CPosTp136::TestParseL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp136::TestParseL(const TDesC& aFile, const TDesC8& aMime)
    {
    iLog->Log(_L("----- TestParseL -----"));
    iLog->Log(_L("FILE: %S"), &aFile);
    
    RemoveAllLmDatabasesL();
    
        const TDesC* categoryNames[KNrOfLandmarks][4] = {{&KEmpty, &KEmpty, &KEmpty, &KEmpty},
                                            {&K1, &KEmpty, &KEmpty, &KEmpty},
                                            {&K2, &K3, &K4, &K5},
                                            {&KEmpty, &KEmpty, &KEmpty, &KEmpty},
                                            {&K6, &KEmpty, &KEmpty, &KEmpty},
                                            {&K7, &K8, &K9, &K10},
                                            {&K11, &K12, &K13, &KEmpty},
                                            {&K14, &K15_1, &KEmpty, &KEmpty}};

    CPosLandmarkParser* parser = CPosLandmarkParser::NewL(aMime);
    CleanupStack::PushL(parser);
    
    RFile file; 
    TInt err = file.Open(iFileSession, aFile, EFileRead);
    if (err != KErrNone) 
    {
    	iLog->Log(_L("Error when opening file"));
        User::Leave(err);
    }
    CleanupClosePushL(file);
    parser->SetInputFileHandleL(file);
    iOperation = parser->ParseContentL();
    TRequestStatus status = KPosLmOperationNotComplete;
    TReal32 progress=0;
    
    CDesCArrayFlat* landmarkNames = new (ELeave) CDesCArrayFlat(KNrOfLandmarks);
    landmarkNames->AppendL(_L("Billigt"));
    landmarkNames->AppendL(_L("LOG9"));
    landmarkNames->AppendL(_L("Kalles Hundg�rd"));
    landmarkNames->AppendL(_L("MLFW"));
    landmarkNames->AppendL(_L("TE, Lund"));
    landmarkNames->AppendL(_L("asdf"));
    landmarkNames->AppendL(_L("TP48LandmarkWithAllXMLFields"));
    landmarkNames->AppendL(_L("TP48Landmark With Empty XML Fields"));
    
    TInt counter=0;
    while (progress != 1)
        {
        iOperation->NextStep(status, progress);
        // Wait for NextStep to complete
        User::WaitForRequest(status);
        CPosLandmark* lm = parser->LandmarkLC();
        TPtrC name;
        lm->GetLandmarkName(name);
        TPtrC expName(landmarkNames->MdcaPoint(counter));
        iLog->Log(_L("Landmark %d, Actual name '%S', Expected '%S'"), counter, &name, &expName);
        if (expName.CompareC(name) != KErrNone)
            {
            iLog->Log(_L("ERROR: Wrong Landmark Name"));
        	iErrorsFound++;
            }
        
        RArray<TPosLmItemId> arr;
        CleanupClosePushL(arr);
        
        lm->GetCategoriesL(arr);
        for (TInt i=0;i<arr.Count();i++)
            {
            CPosLandmarkCategory* cat = parser->LandmarkCategoryLC(arr[i]);
            TPtrC catName;
            cat->GetCategoryName(catName);
            iLog->Log(_L("Category %d, Actual '%S', Expected '%S'"), i, &catName, categoryNames[counter][i]);
            if (catName.CompareC(*categoryNames[counter][i]) != KErrNone)
            	{
            	iLog->Log(_L("ERROR: Wrong Category Name when comparing"));
                iErrorsFound++;
            	}
            CleanupStack::PopAndDestroy(cat);
            }
        
        CleanupStack::PopAndDestroy(&arr);
        CleanupStack::PopAndDestroy(lm);
        counter++;
        }
        
    TUint32 nr = parser->NumOfParsedLandmarks();
    if (nr != KNrOfLandmarks) 
    {
    	iLog->Log(_L("ERROR: Wrong number of landmarks parsed"));
        iErrorsFound++;
    }
    
    iLog->Log(_L("-------------------\n"));
    delete iOperation;iOperation=NULL;
       
    CleanupStack::PopAndDestroy(&file);
    
    landmarkNames->Reset();
    delete landmarkNames;
    landmarkNames = NULL;   
    CleanupStack::PopAndDestroy(parser);    
    }

// ---------------------------------------------------------
// CPosTp136::TestImportL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp136::TestImportL(const TDesC& aFile, const TDesC8& aMime)
    {
    iLog->Log(_L("----- TestImportL -----"));    
    iLog->Log(_L("FILE: %S"), &aFile);

    RemoveAllLmDatabasesL();

    const TDesC* categoryNames[KNrOfLandmarks][4] = {{&KEmpty, &KEmpty, &KEmpty, &KEmpty},
                                        {&K1, &KEmpty, &KEmpty, &KEmpty},
                                        {&K2, &K3, &K4, &K5},
                                        {&KEmpty, &KEmpty, &KEmpty, &KEmpty},
                                        {&K6, &KEmpty, &KEmpty, &KEmpty},
                                        {&K7, &K8, &K9, &K10},
                                        {&K11, &K12, &K13, &KEmpty},
                                        {&K14, &K15_1, &KEmpty, &KEmpty}};
    
    CPosLandmarkDatabase* database = OpenDatabaseLC();
   
    CPosLandmarkParser* parser = CPosLandmarkParser::NewL(aMime);
    CleanupStack::PushL(parser);
    
    CPosLmCategoryManager* categoryManager = CPosLmCategoryManager::NewL(*database);
    CleanupStack::PushL(categoryManager);
    
    RFile file; 
    TInt err = file.Open(iFileSession, aFile, EFileRead);
    if (err != KErrNone) 
    {
    	iLog->Log(_L("Error when opening file"));
        User::Leave(err);
    }
    CleanupClosePushL(file);
    
    parser->SetInputFileHandleL(file);
    iOperation = database->ImportLandmarksL(*parser, CPosLandmarkDatabase::EIncludeCategories);
    
    ExecuteAndDeleteLD(iOperation);
    iOperation=NULL;
    
    CPosLmItemIterator* iter = database->LandmarkIteratorL();
    CleanupStack::PushL(iter);
    
    CDesCArrayFlat* landmarkNames = new (ELeave) CDesCArrayFlat(KNrOfLandmarks);
    landmarkNames->AppendL(_L("Billigt"));
    landmarkNames->AppendL(_L("LOG9"));
    landmarkNames->AppendL(_L("Kalles Hundg�rd"));
    landmarkNames->AppendL(_L("MLFW"));
    landmarkNames->AppendL(_L("TE, Lund"));
    landmarkNames->AppendL(_L("asdf"));
    landmarkNames->AppendL(_L("TP48LandmarkWithAllXMLFields"));
    landmarkNames->AppendL(_L("TP48Landmark With Empty XML Fields"));
    
    TPosLmItemId id = iter->NextL();
    TInt counter = 0;
    while (id != KPosLmNullItemId)
        {
        CPosLandmark* lm = database->ReadLandmarkLC(id);
        RArray<TPosLmItemId> arr;
        CleanupClosePushL(arr);
        
        TPtrC name;
        lm->GetLandmarkName(name);
        TPtrC expName(landmarkNames->MdcaPoint(counter));
        iLog->Log(_L("Landmark %d, Actual name '%S', Expected '%S'"), counter, &name, &expName);
        if (expName.CompareC(name) != KErrNone)
            {
            iLog->Log(_L("ERROR: Wrong Landmark Name"));
            iErrorsFound++;
            }
        
        lm->GetCategoriesL(arr);
        if (arr.Count() == 0) iLog->Log(_L("NOLL"));
        else
            {
            TBuf<100> buf;
            buf.Format(_L("nr of categories %d"), arr.Count());
            iLog->Log(buf);
            }
            
        for (TInt i=0;i<arr.Count();i++)
            {
            CPosLandmarkCategory* cat = categoryManager->ReadCategoryLC(arr[i]);
            TPtrC catName;
            cat->GetCategoryName(catName);
            iLog->Log(_L("Category %d, Actual '%S', Expected '%S'"), i, &catName, categoryNames[counter][i]);
            if (catName.CompareC(*categoryNames[counter][i]) != KErrNone) 
                {
                iLog->Log(_L("ERROR: Wrong Category Name"));
                iErrorsFound++;
                }
            CleanupStack::PopAndDestroy(cat);
            }

        CleanupStack::PopAndDestroy(&arr);
        CleanupStack::PopAndDestroy(lm);
        id = iter->NextL();
        counter++;
        }
    TUint32 nr = parser->NumOfParsedLandmarks();
    if (nr != KNrOfLandmarks)
    {
    	iLog->Log(_L("ERROR: Wrong number of landmarks parsed"));
        iErrorsFound++;
    }
    
    CleanupStack::PopAndDestroy(iter);
    landmarkNames->Reset();
    delete landmarkNames;
    landmarkNames = NULL;  
    
    iLog->Log(_L("-------------------\n"));

    CleanupStack::PopAndDestroy(&file);
    CleanupStack::PopAndDestroy(categoryManager);
    CleanupStack::PopAndDestroy(parser);
    CleanupStack::PopAndDestroy(database);
    }
	
//  End of File
