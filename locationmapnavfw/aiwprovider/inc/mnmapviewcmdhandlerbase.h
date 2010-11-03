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
* Description:  CMnMapViewCmdHandlerBase class declaration
*
*/


#ifndef MN_MAPVIEWCMDHANDLERBASE_H
#define MN_MAPVIEWCMDHANDLERBASE_H

#include "mnaiwcommandhandlerbase.h"

class CMnMapView;

/** \internal
 *  Base class for MapView-related AIW commands
 */
class CMnMapViewCmdHandlerBase : public CMnAiwCommandHandlerBase
    {
    public:
        ~CMnMapViewCmdHandlerBase();
        
    protected:
        CMnMapViewCmdHandlerBase( CMnProvider& aProvider );
            
        void ConstructL( TRunMode aRunMode );
        
        CMnMapView& MapView();

        /** Reads contents of EGenericParamMnMapOptions parameter */
        void ReadMapOptionsL( 
            const TAiwGenericParam& aParam,
            TMnAiwMapViewOptionsParam& aMapOptions );

	public: // from CMnAiwCommandHandlerBase

        void SetExitObserverL( MMnProviderExitCallback& aExitCallback );

    protected: // from CMnAiwCommandHandlerBase
    
        void HandleAiwParamL( const TAiwGenericParam& aParam );
        void HandleAiwParamLandmarksL( RArray<TPtrC8>& aLandmarkDataList );
        void HandleAiwParamLandmarksListL( const TDesC& aDbUri, RArray<TPosLmItemId>& aLmIds );
        void ResetParametersL();
        
    protected:
        CMnMapView* iMapView;            
    };
    
#endif //  MN_MAPVIEWCMDHANDLERBASE_H
    