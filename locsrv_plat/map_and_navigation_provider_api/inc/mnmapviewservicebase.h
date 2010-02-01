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
* Description:  CMnMapViewServiceBase class
*
*/


#ifndef MN_MAPSERVICEBASE_H
#define MN_MAPSERVICEBASE_H

#include "mnservicebase.h"
#include "mnmapview.h"

class TMnMapOptions;
class CPosLandmark;

/** @brief Base class for MapView service implementations (KMnAppMapViewService). 
 *
 *  Provider Application
 *  must derive its MapView service class from this one. This base implementation
 *  hides IPC communication details and provides native methods for accessing
 *  various map parameters, specified by client.
 *
 *  Provider Application must return pointer to instance of its class, derived
 *  from this class, in response to CAknAppService::CreateServiceL() call,
 *  if given parameter is KMnAppMapViewService. 
 *
 *  This class defines handlers of particular requests from client as pure 
 *  virtual methods, which derived class must implement. The parameters, which
 *  client application may specify in CMnMapView are accessible via other methods
 *  of this class. Provider Application must obey these parameters and use them
 *  as specified in @ref CMnMapView.
 *
 *  See parent @ref CMnServiceBase class documentation for 
 *  other details.
 *
 *  @since 3.1
 *  @lib mnservicelib.lib
 *  @ingroup MnProviderAPI
 */
class CMnMapViewServiceBase : public CMnServiceBase
    {
    public :

        /** @brief Returns map displaying options specified by client application.
         *
         *  These options must be utilized in request handlers as specified in 
         *  @ref CMnMapView. 
         *  
         *  @return Options specified by client.
         */
        IMPORT_C CMnMapView::TOptions Options();
        
        /** @brief Returns Current Location option specified by client application.
         *
         *  This option must be utilized in request handlers as specified in 
         *  @ref CMnMapView. 
         *  
         *  @return Current Location options specified by client.
         */
        IMPORT_C CMnMapView::TCurrentLocationOption CurrentLocationOption();

        /** @brief Returns array of landmarks as set by client to be shown on map.
         *
         *  All these landmarks shall be shown on map.
         *
         *  @return Reference to array of landmarks.
         */          
        IMPORT_C TArray<const CPosLandmark*> LandmarksToShow();

        /** @brief Returns array of linked landmarks from specified database.
         *
         *  All these landmarks shall be shown on map.
         *
         *  @param[in] aDbIndex Index of landmark database, which contains returned
         *                    landmarks. Must be valid for array returned from
         *                    @ref LandmarksToShowDatabases().
         *  @return Array of IDs of landmarks from database, specified by
         *          aDbIndex.
         *
         *  @panic "MnPanicServer"-KMnPanicArrayIndexOutOfBounds 
         *      aDbIndex not valid for the list of databases.
         */          
        IMPORT_C TArray<TPosLmItemId> LinkedLandmarksToShow( TInt aDbIndex );
        
        /** @brief Returns databases, where linked landmarks belong to.
         *  @return Array of landmark database URIs.
         */
        IMPORT_C TArray<const HBufC*> LandmarksToShowDatabases();
        
        /** @brief Returns text, which shall be shown in Select From Map 
         *  request. 
         *
         *  This text is used in HandleSelectFromMapL().
         *  
         *  @return Selection request text to be shown to user in 
         *  HandleSelectFromMapL().
         */
        IMPORT_C TPtrC SelectionRequestText();
        
        /** @brief Determines whether central point is specified by client.
         *
         *  If central point is not set, @ref GetAreaCentralPoint returns
         *  coordinate with latitude and longitude set to NaN.
         *
         *  @return ETrue - if central point is set, EFalse - otherwise,
         *          and during Show Current Location request.
         */
        IMPORT_C TBool IsAreaCentralPointSet();
        
        /** @brief Returns central point specified by client.
         *
         *  If central point is not set, this
         *  coordinate has latitude and longitude set to NaN.
         *
         *  @param[out] aCentralPoint Central point of map view area, 
         *                            specified by client.
         */
        IMPORT_C void GetAreaCentralPoint( TCoordinate& aCentralPoint );
        
        /** @brief Returns radius, in meters, of map view area, specified by client.
         *
         *  Implementation has to obey this setting as long as this is within
         *  allowed range. If it is too low and cannot be used, the implementation
         *  must select lowest possible radius. If this value is KAutomaticRadius, 
         *  it is considered not set.
         */
        IMPORT_C TReal AreaRadius();
        
        /** @brief Completes selection request.
         *
         *  Implementation must call this method to signal completion
         *  of Select From Map request. 
         *  This override is used if free location from map has been selected by user.
         *
         *  @param[in] aLandmark Landmark containing information about 
         *             user-selected location. As much information as possible
         *             shall be set.
         *
         *  @leave KErrArgument CMnMapView::EOptionRestrictSelection was specified
         *      by client. 
         *  @see Options().
         */
        IMPORT_C void CompleteSelectionRequestL( const CPosLandmark& aLandmark );

        /** @brief Completes selection request.
         *
         *  Implementation must call this method to signal completion
         *  of Select From Map request. 
         *  This override is used if one of not linked landmarks has been selected by user.
         *
         *  @param[in] aLandmarkIndex Index of not linked landmark containing information about 
         *             user-selected location. As much information as possible
         *             shall be set.
         *
         *  @panic "MnPanicServer"-KMnPanicArrayIndexOutOfBounds
         *      aLandmarkIndex is negative or greater than LandmarksToShow().Count().
         */
        IMPORT_C void CompleteSelectionRequest( TInt aLandmarkIndex );

        /** @brief Completes selection request.
         *
         *  Implementation must call this method to signal completion
         *  of Select From Map request. 
         *  This override is used if one of linked landmarks, passed by client
         *  has been selected by user.
         *
         *  @param aLandmarkId ID of landmark selected by user.
         *  @param aDbIndex Index of database where landmark specified by
         *                      aLandmarkId belongs to.
         *
         *  @panic "MnPanicServer"-KMnPanicArrayIndexOutOfBounds
         *      aDbIndex is negative or greater than LandmarksToShowDatabases().Count().
         */
        IMPORT_C void CompleteSelectionRequest( TPosLmItemId aLandmarkId, TInt aDbIndex );
    
    protected :
    
        /** C++ constructor */
        IMPORT_C CMnMapViewServiceBase();
        /** Destructor */
        IMPORT_C virtual ~CMnMapViewServiceBase();

        /** This method must be called from derived class' ConstructL to
         *  allow internal initialization of this class. */
        IMPORT_C void BaseConstructL();
        
        /** @brief Handles client's request for showing map. 
         *
         *  Derived class must implement this method obeying 
         *  specification of @ref CMnMapView::ShowMapL and if @ref KMnSvcFeatureShowMap
         *  feature is specified in its SERVICE_INFO.
         *
         *  Landmarks returned by LandmarksToShow and LinkedLandmarksToShow shall be 
         *  shown on map.
         *
         *  This is synchronous request.
         *  Client's request will be completed when this method exits, therefore
         *  it should exit immediately after map view is shown.
         *  
         *  @leave KErrNotSupported Should leave if this feature is not supported.
         */
        virtual void HandleShowMapL() =0;

        /** @brief Handles client's request for showing map from current location. 
         *
         *  Derived class must implement this method obeying specification of 
         *  @ref CMnMapView::ShowCurrentLocationL and if @ref KMnSvcFeatureShowMap
         *  feature is specified in its SERVICE_INFO.
         *  
         *  Landmarks returned by LandmarksToShow and LinkedLandmarksToShow shall be 
         *  shown on map.
         *
         *  This is synchronous request.
         *  Client's request will be completed when this method exits, therefore
         *  it should exit immediately after map view is shown.
         *  
         *  @leave KErrNotSupported Should leave if this feature is not supported.
         */
        virtual void HandleShowCurrentLocationL() =0;

        /** @brief Handles client's request for selecting a location from map. 
         *
         *  Derived class must implement this method obeying specification of 
         *  @ref CMnMapView::SelectFromMapL and if KMnSvcFeatureSelectFromMap
         *  feature is specified in its SERVICE_INFO.
         *  
         *  Landmarks returned by LandmarksToShow and LinkedLandmarksToShow shall be 
         *  shown on map.
         *
         *  This is asynchronous request.
         *  It must be completed (in successful case) with one of 
         *  CompleteSelectionRequest overloads.
         *  
         *  @leave KErrNotSupported Should leave if this feature is not supported.
         */
        virtual void HandleSelectFromMapL() =0;

    protected: // From CAknAppServiceBase

        /** \internal */
        IMPORT_C void ServiceL( const RMessage2& aMessage );

    private: // Internal methods

        void CleanSelectionResult();

        void HandleAddLandmarkIdsToShowL( const RMessage2& aMessage );
        void HandleAddLandmarksToShowL( const RMessage2& aMessage );
        void HandleGetSelectionResultLandmarkL( const RMessage2& aMessage );
        void HandleGetSelectionResultDbUriL( const RMessage2& aMessage );
        
        void ResetLandmarks();        
        TInt FindDatabase( const TDesC& aUri );

        void UnpackOptionsL( const RMessage2& aMsg );
        void UnpackSelectionRequestTextL( const RMessage2& aMsg );

        void VerifySelectionRequestL();

    private:
        class CLinkedLandmarksList : public CBase
            {
            public:
                ~CLinkedLandmarksList();
                
                HBufC*                  iDatabaseUri;
                RArray< TPosLmItemId >  iLandmarkIds;
            };

    private:

        CMnMapView::TOptions                iOptions;
        CMnMapView::TCurrentLocationOption  iCurrentLocationOption;
        
        TCoordinate                         iCentralPoint;
        TReal                               iRadius;
        HBufC*                              iSelectionRequestText;
        
        RPointerArray< const CPosLandmark >     iLandmarks;
        RPointerArray< CLinkedLandmarksList >   iLinkedLandmarksLists;

        RPointerArray< const HBufC >        iTempDatabaseList;
        
        HBufC8*                             iResultLandmark;
        TInt                                iResultDbIndex;
    };

#endif // MN_MAPSERVICEBASE_H


