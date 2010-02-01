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
* Description:  TMnMapSelectionResultInfo class implementation
*
*/


#include "mnmapselectionresultinfo.h"

// ======== LOCAL FUNCTIONS ========

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
inline TMnMapSelectionResultInfo::TMnMapSelectionResultInfo()
    {
    iType = CMnMapView::ESelectionNone;
    iLandmarkIndex = -1;
    iLandmarkId = KPosLmNullItemId;
    iLandmarkSize = 0;
    iDbUriSize = 0;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
inline TMnMapSelectionResultInfo::TMnMapSelectionResultInfo( TInt aLandmarkIndex )
    {
    iType = CMnMapView::ESelectionLandmarkIndex;
    iLandmarkIndex = aLandmarkIndex;
    iLandmarkId = KPosLmNullItemId;
    iLandmarkSize = 0;
    iDbUriSize = 0;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
inline TMnMapSelectionResultInfo::TMnMapSelectionResultInfo( const TDesC8& aLandmarkBuffer )
    {
    iType = CMnMapView::ESelectionFreeLandmark;
    iLandmarkIndex = -1;
    iLandmarkId = KPosLmNullItemId;
    iLandmarkSize = aLandmarkBuffer.Size();
    iDbUriSize = 0;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
inline TMnMapSelectionResultInfo::TMnMapSelectionResultInfo( TPosLmItemId aLandmarkId, const TDesC& aDbUri )
    {
    iType = CMnMapView::ESelectionLinkedLandmark;
    iLandmarkIndex = -1;
    iLandmarkId = aLandmarkId;
    iLandmarkSize = 0;
    iDbUriSize = aDbUri.Size();
    }
