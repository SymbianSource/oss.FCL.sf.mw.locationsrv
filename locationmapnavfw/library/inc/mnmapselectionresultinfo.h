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
* Description:  TMnMapSelectionResultInfo class
*
*/


#ifndef MN_MAPSELECTIONRESULTINFO_H
#define MN_MAPSELECTIONRESULTINFO_H

#include <mnmapview.h>
#include <EPos_Landmarks.h>

class CPosLandmark;

/** \internal 
 *  Contains information about result of CMnMapView::SelectFromMapL request 
 */
class TMnMapSelectionResultInfo
    {
    public:
        /** Default constructor */
        inline TMnMapSelectionResultInfo();
        /** Initializes for CMnMapView::ESelectionLandmarkIndex result */
        inline TMnMapSelectionResultInfo( TInt aLandmarkIndex );
        /** Initializes for CMnMapView::ESelectionFreeLandmark result */
        inline TMnMapSelectionResultInfo( const TDesC8& aLandmarkBuffer );
        /** Initializes for CMnMapView::ESelectionLinkedLandmark result */
        inline TMnMapSelectionResultInfo( TPosLmItemId aLandmarkId, const TDesC& aDbUri );
        
        /** Type of result */
        CMnMapView::TSelectionResultType    iType;
        /** Index of landmark selected */
        TInt                                iLandmarkIndex;
        /** ID of landmark selected */
        TPosLmItemId                        iLandmarkId;
        /** Size of buffer required to receive landark from server */ 
        TInt                                iLandmarkSize;
        /** Size of buffer required to receive dafabase URI server */ 
        TInt                                iDbUriSize;
    };

#endif // MN_MAPSELECTIONRESULTINFO_H

