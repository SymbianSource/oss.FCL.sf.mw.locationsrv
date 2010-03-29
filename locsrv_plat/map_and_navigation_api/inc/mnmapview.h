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
* Description:  CMnMapView class
*
*/


#ifndef MN_MAPVIEW_H
#define MN_MAPVIEW_H

#include <e32base.h>
#include <EPos_Landmarks.h>
#include <lbsposition.h>

class RFs;
class CApaServerAppExitMonitor;
class MAknServerAppExitObserver;
class CMnProvider;
class CPosLandmark;
class RMnMapViewServiceClient;

/** @brief Provides access to map view features.
 *
 *  The ShowMapL() method allows to show map of some area, which can be specified
 *  by SetMapAreaL(). ShowCurrentLocationL() opens a map with current location
 *  in the center. SelectFromMapL() allows to ask user to select some location
 *  from the map. Client can request that some landmarks should be drawn on the map,
 *  when it is shown, by using AddLandmarksToShowL.
 *  Subsequent calls to this method will add landmarks to show list. To reset the
 *  list, call @ref ResetLandmarksToShow.
 *
 *  Two overloads of AddLandmarksToShowL exist for linked and not linked landmarks.
 *  The linked landmark is the one, which has its landmark ID set.
 *  Linked landmarks are sent to Provider Application along with their
 *  landmark database URI, which allows Provider Application to get full category
 *  information of the landmark.
 *
 *  To create an instance of the class, use @ref NewL or @ref NewChainedL methods. They
 *  require instance of CMnProvider, defined in Map and Navigation Provider Discovery API.
 *
 *  Asynchronous operations can be cancelled by @ref Cancel().
 *  Only one asynchronous request can be issued at a time, otherwise client
 *  will be panicked with KMnPanicDuplicateRequest.
 *
 *  @since 3.1
 *  @lib mnclientlib.dll
 *  @ingroup MapNavAPI
 */
class CMnMapView : public CBase
    {
    public :

        /** Type for bitmap of flags defined by TOption */
        typedef TUint32 TOptions;

        /** @brief Map view options */
        enum TOption
            {
            /** Default value */
            EOptionNone,
            /** Defines that only landmarks specified by AddLandmarksToShowL
             *  are allowed to be selected by user. */
            EOptionRestrictSelection
            };

        /** @brief Defines whether current location shall be marked on map */
        enum TCurrentLocationOption
            {
            /** Current location is not shown (default). */
            ECurrentLocationDisabled,
            /** Current location is shown only if it's within visible area */
            ECurrentLocationEnabled,
            /** Current location is visible always. Provider Application selects
                appropriate zoom (and position, if area center is not specified)
                to make sure current location is visible. */
            ECurrentLocationShowAlways
            };

        /** @brief Defines type of selection result */
        enum TSelectionResultType
            {
            /** Undefined result */
            ESelectionNone,
            /** Free location has been selected.
             *  Use @ref RetrieveSelectionResultL() to
             *  get result of selection. */
            ESelectionFreeLandmark,
            /** One of landmarks passed by instance has been selected.
             *  Use @ref RetrieveSelectionResultL(TInt&) to
             *  get result of selection. */
            ESelectionLandmarkIndex,
            /** One of landmarks passed as landmark ID and database URI has been selected.
             *  Use @ref RetrieveSelectionResultL(TPosLmItemId&, HBufC*&) to
             *  get result of selection. */
            ESelectionLinkedLandmark
            };

        /** Destructor */
        virtual ~CMnMapView();

        /** @brief Creates new instance of the class. Provider Application
         *  will be executed standalone.
         *
         *  @param[in] aProvider Provider Application to be used for showing map.
         *  @return Pointer to new instance of the class.
         *
         *  @leave KErrNotSupported Selected Provider Application does not support
         *      map view services (@ref CMnProvider::EServiceMapView).
         */
        IMPORT_C static CMnMapView* NewL( CMnProvider& aProvider );

        /** @brief Creates new instance of the class. Provider Application
         *  will be executed chained (unless Provider Application defines that
         *  it should be started in background).
         *
         *  @param[in] aProvider Provider Application to be used for showing map.
         *  @return Pointer to new instance of the class.
         *
         *  @leave KErrNotSupported Selected Provider Application does not support
         *      map view services (@ref CMnProvider::EServiceMapView).
         */
        IMPORT_C static CMnMapView* NewChainedL( CMnProvider& aProvider );

        /** @brief Shows map on the screen.
         *
         *  This method completes as soon as request reaches Provider
         *  Application. Map is shown then in Provider Application's process.
         *
         *  If map area is specified by SetMapAreaL(), then this area of map
         *  will be visible, when it is shown.
         *  Otherwise, if only list of landmarks to show is specified,
         *  then area covering all landmarks is visible.
         *  If Current Location Option is set to @ref
         *  ECurrentLocationShowAlways, then it may modify visible area.
         *
         *  If neither area nor landmarks are specified, and current location
         *  option is not "always", then simply last shown map (as determined by selected
         *  provider application) is shown. If map was never shown before, then
         *  Provider Application shows map from its default position.
         */
        IMPORT_C void ShowMapL();

        /** @brief Shows map on the screen, with current location in the center.
         *
         *  This method completes as soon as request reaches Provider
         *  Application. Map is shown then in Provider Application's process.
         *
         *  Same options are applied to this method as to ShowMapL()
         *  (with the exception that area center point is ignored)
         */
        IMPORT_C void ShowCurrentLocationL();

        /** @brief Shows map on the screen and asks user to select a position.
         *
         *  If EOptionsRestrictSelection option is set, then user can select
         *  only those landmarks, which are specified with one of AddLandmarksToShowL
         *  overrides. Otherwise any location is allowed to be selected.
         *
         *  User is shown a view with default selection request text. This text
         *  can be overridden with SetCustomRequestTextL().
         *
         *  See SelectionResultType() for how to retrieve selection result.
         *
         *  This is asynchronous request and can be cancelled with Cancel().
         *
         *  This method can leave in case of memory lack or other system error.
         *  If it leaves, no asynchronous request is outstanding.
         *
         *  @param[out] aStatus Status of asynchronous operation. Contains error code,
         *                 when request is completed: KErrNone, if user has made
         *                 selection, KErrCancel - if request is cancelled, or user
         *                 refused to make selection. KErrArgument if no landmarks to
         *                 be shown are specified and ERestrictSelection is set.
         *                 Otherwise any of system-wide error codes.
         *
         *  @panic "MnClientPanic"-KMnPanicDuplicateRequest if new asynchronous request
         *          is issued before previous one is completed.
         */
        IMPORT_C void SelectFromMapL( TRequestStatus& aStatus );

        /** @brief Adds linked landmarks to the list of landmarks to be shown on map.
         *
         *  Landmarks are passed along with URL of the database, where they reside.
         *  This allows to utilize category information from landmarks.
         *  If landmarks from several databases shall be shown, call this method
         *  for every such database. All landmarks are added to a single list.
         *  To clear that list, call ResetLandmarksToShow().
         *
         *  @param[in] aDatabaseUri A database, to which belong landmarks, listed
         *                      in aLandmarkIds parameter.
         *  @param[in] aLandmarkIds IDs of landmarks to add to show list.
         *
         *  @leave KErrLocked Asynchronous request (SelectFromMapL) is outstanding.
         */
        IMPORT_C void AddLandmarksToShowL(
            const TDesC& aDatabaseUri,
            RArray<TPosLmItemId>& aLandmarkIds );

        /** @brief Adds landmarks to the list of landmarks to be shown on map.
         *
         *  Landmarks are passed as instances. This variant is
         *  for passing landmarks, which are not stored in databases.
         *
         *  All landmarks are added to a single list.
         *  To clear that list, call ResetLandmarksToShow().
         *
         *  With large amount of landmarks, it is recommended to use overload,
         *  which accepts linked landmarks, if possible.
         *
         *  @param[in] aLandmarks Array of landmarks to add to show list.
         *
         *  @leave KErrLocked Asynchronous request (SelectFromMapL) is outstanding.
         */
        IMPORT_C void AddLandmarksToShowL( RPointerArray<CPosLandmark>& aLandmarks );

        /** @brief Adds landmarks to the list of landmarks to be shown on map.
         *
         *  Landmarks are passed as packed instances (see @p PosLandmarkSeriazation from
         *  Landmarks API). This overload is for passing landmarks, which are not stored
         *  in databases. It is more efficient than
         *  AddLandmarksToShowL( RPointerArray<CPosLandmark>& ) in terms of memory usage,
         *  if client already possess landmarks in packed form.
         *
         *  All landmarks are added to a single list.
         *  To clear that list, call ResetLandmarksToShow().
         *
         *  With large amount of landmarks, it is recommended to use overload,
         *  which accepts linked landmarks, if possible.
         *
         *  @param[in] aPackedLandmarks Array of landmarks to add to show list.
         *
         *  @leave KErrLocked Asynchronous request (SelectFromMapL) is outstanding.
         */
        IMPORT_C void AddLandmarksToShowL( RArray<TPtrC8>& aPackedLandmarks );

        /** @brief Clears the list of landmarks to be shown on map. 
         *  This call has no effect during the time when asynchronous request
         *  (SelectFromMapL) is outstanding.
         */
        IMPORT_C void ResetLandmarksToShow();

        /** @brief Defines the map area to be shown.
         *  @param[in] aCentralPoint    Point on the Earth to be shown in the center
         *                              of map view. This setting is ignored, when
         *                              ShowCurrentLocationL() is called.
         *  @param aRadius      Radius in meters of the area around central point,
         *                      which should be visible, when map is shown.
         *                      If this setting is too small, then
         *                      Provider Application selects smallest possible
         *                      radius (maximum zoom).
         *                      If NaN, then Provider Application automatically
         *                      selects appropriate zoom. This setting is ignored if
         *                      ECurrentLocationShowAlways is set.
         */
        IMPORT_C void SetMapAreaL( const TCoordinate& aCentralPoint, TReal aRadius );

        /** @brief Clears map area setting. */
        IMPORT_C void ResetMapArea();

        /** @brief Specifies custom request text for SelectFromMapL() request.
         *
         *  If not specified, than default request text will be used (as
         *  defined in UI specification).
         *  New setting will have effect only for next map display request.
         *
         *  @param[in] aCustomText Text to be shown as selection request to user.
         */
        IMPORT_C void SetCustomRequestTextL(const TDesC& aCustomText);

        /** @brief Specifies that default request text shall be used
         *  starting from next SelectFromMapL() request.
         */
        IMPORT_C void SetUseDefaultRequestText();

        /** @brief Specifies how current location is shown on map.
         *  New setting will have effect only for next map display request.
         *  @param aCurrentLocationOption One of TCurrentLocationOption values.
         */
        IMPORT_C void SetCurrentLocationOption(
            TCurrentLocationOption aCurrentLocationOption );

        /** @brief Retrieves current state of current location option.
         *  @return One of TCurrentLocationOption values.
         */
        IMPORT_C TCurrentLocationOption CurrentLocationOption() const;

        /** @brief Sets map view options.
         *  New settings will have effect only for next map display request.
         *  @param aOptions Bitmap of TOption values.
         */
        IMPORT_C void SetOptions( TOptions aOptions );

        /** @brief Retrieves current map view options.
         *  @return Bitmap of TOption values.
         */
        IMPORT_C TOptions Options() const;

        /** @brief  Returns type of selection result
         *  @return One of @ref TSelectionResultType values.
         */
        IMPORT_C TSelectionResultType SelectionResultType() const;

        /** @brief Retrieves selection result as landmark instance.
         *  @return Landmark with information about selected location. Client takes ownership.
         *  @leave KErrNotFound Result is not available or is of other type.
         *      See @ref SelectionResultType() for details.
         *
         *  @see SelectionResultType()
         *  @see RetrieveSelectionResultL(TInt&)
         *  @see RetrieveSelectionResultL(TPosLmItemId&, HBufC*&)
         */
        IMPORT_C CPosLandmark* RetrieveSelectionResultL();

        /** @brief Retrieves selection result as index of given landmark instance.
         *  @param[out] aLandmarkIndex Index of a landmark passed as instance,
         *                             which has been selected.
         *  @leave KErrNotFound Result is not available or is of other type.
         *      See @ref SelectionResultType() for details.
         *
         *  @see SelectionResultType()
         *  @see RetrieveSelectionResultL()
         *  @see RetrieveSelectionResultL(TPosLmItemId&, HBufC*&)
         */
        IMPORT_C void RetrieveSelectionResultL( TInt& aLandmarkIndex );

        /** @brief Retrieves ID of selected landmark.
         *  @param[out] aLandmarkId On return, ID of selected landmark in the database,
         *                          specified by aDatabaseUri.
         *  @param[out] aDatabaseUri On return, URI of the database of selected landmark.
                                     Client takes ownership.
         *  @leave KErrNotFound Result is not available or is of other type.
         *      See @ref SelectionResultType() for details.
         *
         *  @see SelectionResultType()
         *  @see RetrieveSelectionResultL()
         *  @see RetrieveSelectionResultL(TInt&)
         */
        IMPORT_C void RetrieveSelectionResultL( TPosLmItemId& aLandmarkId, HBufC*& aDatabaseUri );

        /** @brief Cancels current outstanding asynchronous request. */
        IMPORT_C void Cancel();
        
        /** @brief Sets observer of provider application lifetime.
         * 	Client will receive exit event from provider application.
         *  By default exit events from provider application are not monitored.
         *  @param[in] aObserver Exit observer.
         *  @leave KErrAlreadyExists if observer is already set. 
         */
        IMPORT_C void SetExitObserverL( MAknServerAppExitObserver& aObserver );

        /** @brief Removes observer of provider application lifetime. */
        IMPORT_C void RemoveExitObserver();

    private :
        /** C++ constructor */
        CMnMapView();
        CMnMapView( const CMnMapView& );            //prevent default copy constructor
        CMnMapView& operator=( const CMnMapView& ); //prevent default assignment operator

        void ConstructCommonL( CMnProvider& aProvider );
        void ConstructNewL( CMnProvider& aProvider );
        void ConstructChainedL( CMnProvider& aProvider );
        void PostConstructL();

        HBufC* LoadResourceTextL(
            RFs& aFsSession,
            const TDesC& aRscFile,
            TInt aResourceId );

    private :
        TOptions                iOptions;
        TCurrentLocationOption  iCurrentLocationOption;
        TCoordinate             iCentralPoint;
        TReal                   iRadius;

        HBufC*                  iDefaultRequestText;

        RMnMapViewServiceClient*  iSession;
        CApaServerAppExitMonitor* iExitMonitor;
    };

#endif // MN_MAPVIEW_H
