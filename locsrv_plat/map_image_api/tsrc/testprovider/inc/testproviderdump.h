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
* Description: Utility class for map and navigation test provider
*
*/

#ifndef MNTP_DUMP_H
#define MNTP_DUMP_H

#include <e32base.h>
#include <e32property.h>

#include <epos_landmarks.h>

#ifdef RD_MAPNAV_BITMAP_ACCESS
#include <mnmapimage.h>
#endif

const TInt KPropCategoryUid = 0x0313200B;
const TInt KPropKeyErrorCode = 1;
const TInt KPropKeyAction = 2;
const TInt KPropKeyProcessId = 3;

enum 
    {
    EServerActionNone,
    EServerActionBaseComplete,
    EServerActionCompleteGeocoding,
    EServerActionCompleteSelectionFree,
    EServerActionCompleteSelectionIndex,
    EServerActionCompleteSelectionNegativeIndex,
    EServerActionCompleteSelectionLinked,
    EServerActionCompleteSelectionWrongLinked,
    EServerActionCompleteSelectionNegativeDbIndex,
    EServerActionDelayCompletion,
    EServerActionExit,
    EServerActionDisableDump,
    EServerActionCompleteRendering,
    };

class CPosLandmark;
class CAiwGenericParamList;
class RProperty;

#ifdef RD_MAPNAV_BITMAP_ACCESS
struct TMapImageOptions
    {
    TMnMapImageParams iParams;
    CMnMapImage::TShowOptions iShowOptions;
    TPoint iTargetOrigin;
    TSize iBitmapSize;
    TInt iBitmapHandle;
    };     
#endif

class CTestProviderDump : public CBase
    {
    public:
        static CTestProviderDump* NewLC();

        void AddTimeStampL();
        void AddRunModeL( TBool aIsChainedMode );

        void AddLandmarkL( const CPosLandmark& aLandmark );
        void AddLandmarkL( const TDesC& aUri, TPosLmItemId aId );
        void AddLandmarkListL( const TDesC& aUri, TArray<TPosLmItemId> aItemIds );
        
        void AddGeocodingOptionsL( TUint32 aOptions );
        void AddMapViewOptionsL( 
            TUint32 aOptions,
            TUint32 aCurrentLocationOption,
            TReal aRadius,
            TCoordinate iCenterPoint );

#ifdef RD_MAPNAV_BITMAP_ACCESS
        void AddMapImageOptionsL( TMapImageOptions& aOptions );
#endif
        void AddRequestTextL( const TDesC& aText );
        void AddAddressL( const TDesC& aAddress );

        void CommitL();
       
        ~CTestProviderDump();

    protected:

        void ConstructL();
        CTestProviderDump();

    private:
        CAiwGenericParamList* iList;
    };
    
class CTestCommandListener : public CActive
    {
    public:
        CTestCommandListener( TCallBack aCallback ) : CActive ( CActive::EPriorityStandard )
            {
            iCallback = aCallback;
            iProperty.Attach( TUid::Uid( KPropCategoryUid ), KPropKeyAction );
            CActiveScheduler::Add( this );
            };
            
        ~CTestCommandListener()
            {
            iProperty.Close();
            };
          
        void Start()
            {
            iProperty.Subscribe( iStatus );
            SetActive();
            };
            
        void RunL()
            {
            Start();
            iCallback.CallBack();
            };
            
        void DoCancel()
            {
            iProperty.Cancel();
            };
            
        RProperty& Property()
            {
            return iProperty;
            };
            
    protected:
        RProperty iProperty;
        TCallBack iCallback;
    };

#endif // MNTP_DUMP_H

