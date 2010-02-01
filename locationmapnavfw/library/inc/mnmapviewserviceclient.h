/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  RMnMapViewServiceClient class
*
*/


#ifndef MN_MAPVIEWSERVICECLIENT_H
#define MN_MAPVIEWSERVICECLIENT_H

#include <AknServerApp.h>
#include <EPos_Landmarks.h>

#include "mnmapselectionresultinfo.h"
#include "mnserviceclientbase.h"

class CPosLandmark;

/** \internal */
NONSHARABLE_CLASS( RMnMapViewServiceClient ) : public RMnServiceClientBase
    {        
    public:
        /** C++ constructor */
        RMnMapViewServiceClient();
        /** C++ destructor */
        ~RMnMapViewServiceClient();
        
        void AddLandmarksToShowL( 
            const TDesC& aDatabaseUri, RArray<TPosLmItemId>& aLandmarkIds );
            
        void AddLandmarksToShowL( RPointerArray<CPosLandmark>& aLandmarks );
        void AddLandmarksToShowL( RArray<TPtrC8>& aPackedLandmarks );
        
        void ResetLandmarksToShow();
        
        void InitSelectionRequestTextL( HBufC*& aSelectionRequestText );
        void SetSelectionRequestTextL( const TDesC& aSelectionRequestText );
        void SetSelectionRequestText( HBufC*& aSelectionRequestText );
        void ClearSelectionRequestText();
     
        void ShowMapL( TMnMapOptions aOptions );
        
        void ShowCurrentLocationL( TMnMapOptions aOptions );
        
        void SelectFromMap( TMnMapOptions aOptions, TRequestStatus& aStatus );
        
        CMnMapView::TSelectionResultType SelectionResultType();

        CPosLandmark* GetSelectedLandmarkL();
        void GetSelectedLandmarkL( TInt& aLandmarkIndex );
        void GetSelectedLandmarkL( TPosLmItemId& aLandmarkId, HBufC*& aDbUri );

    private: // from RApaAppServiceBase
        TUid ServiceUid() const ;
        
    private:
        TMnMapOptions               iOptions;
        TPtrC8                      iOptionsPtr;
        
        HBufC*                      iSelectionRequestText;
        TBool                       iSelectionRequestTextOwned;
        
        TMnMapSelectionResultInfo           iResultInfo;
        TPckg<TMnMapSelectionResultInfo>    iResultInfoPtr;
    };
    
#endif // MN_MAPVIEWSERVICECLIENT_H
