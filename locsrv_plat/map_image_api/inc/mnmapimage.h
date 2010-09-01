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
* Description:  CMnMapImage class
*
*/


#ifndef MN_MAPIMAGE_H_
#define MN_MAPIMAGE_H_

#include <e32base.h>
#include <lbsposition.h>
#include <mnmapimageparams.h>

class CApaServerAppExitMonitor;
class MAknServerAppExitObserver;
class CMnProvider;
class RMnMapImageServiceClient;
class CMnMapImage;
class CFbsBitmap;
class CMnCoordinateConverterBase;

/** Abstract class to be inherited by classes, which want to receive
 *  notification, when image rendering is completed.
 *  @see CMnMapImage 
 *  @since 5.0 */
class MMnMapImageRenderCallback
    {
    public:
        /** This callback is called when CMnMapImage has completed rendering
         *  map image. It is not called if rendering was cancelled by client.
         *  @param aStatus Result of the rendering operation:
         *                      - @p KErrNone - rendering was completed successfully.
         *                      - @p KErrNotSupported - rendering is not possible in specified area
         *                           or one of mandatory options are not supported (see @ref TShowOption).
         *                      - Other system-wide error codes also possible.
         *  @param aMapImage Instance of CMnMapImage, through which the request has
         *                   been issued. */
        virtual void HandleRenderingCompletedL( TInt aStatus, CMnMapImage& aMapImage ) = 0;
    };

/** Provides access to map image rendering features.
 *
 *  The RenderL() method allows to get bitmap image of some area, which is specified
 *  by SetCenterPoint() and SetScale(). Size of the bitmap is defined by SetSize().
 *
 *  To create an instance of the class, use @ref NewL method. It
 *  requires instance of CMnProvider, defined in Map and Navigation Provider Discovery API.
 *
 *  Only one asynchronous request can be issued at a time, otherwise client
 *  will be panicked with KMnPanicDuplicateRequest.
 *
 *  @since 5.0
 *  @lib mnclientlib.dll
 *  @ingroup MapNavAPI
 */
class CMnMapImage : public CBase
	{
	public:

        /** Creates new instance of the class. Provider Application
         *  will be executed standalone.
         *
         *  @param[in] aProvider Provider Application to be used for map rendering.
         *  @return Pointer to new instance of the class.
         *
         *  @leave KErrNotSupported Selected Provider Application does not support
         *      map image services (@ref CMnProvider::EServiceMapImage). */
		IMPORT_C static CMnMapImage* NewL( CMnProvider& aProvider );

        /** Destructor */
        virtual ~CMnMapImage();

		enum TShowOption
			{
            /** No options set, just plain image is requested. */
            EShowOptionNone             = 0x00,

            /** Texts such as street, city, country names should be drawn */
            EShowOptionText             = 0x01,

            /** POIs should be marked on the map */
            EShowOptionPois             = 0x02,

            /** Landmarks should be marked on the map */
            EShowOptionLandmarks        = 0x04,

            /** Current location should be marked on the map.
             *  Client must have Location capability, otherwise
             *  this option will be ignored. */
            EShowOptionCurrentLocation  = 0x08,

            /** Country borders should be shown */
            EShowOptionCountryBorders   = 0x10,

            /** If this flag is set, it is required to draw
             *  items specified by other flags, otherwise all
             *  of them are optional. */
            EShowOptionsMandatory       = 0xF000
			};

        /** Type for bitmap of options defined in @ref TShowOption */
		typedef TUint32 TShowOptions;

        /** Returns what types of map images are supported by provider
         *  application given at construction (see NewL()).
         *  @return Bitmap of values defined by @ref TMapImageType. */
		IMPORT_C TMnMapImageParams::TMapImageTypes SupportedImageTypes();

        /** Returns map image parameters object.
         *  Client can modify it, but all modification will be
         *  applied only for next and following RenderL requests.
         *  @return Modifiable map image parameters object */
        IMPORT_C TMnMapImageParams& MapImageParams();

        /** @overload
         *  @return Non-modifiable map image parameters object */
        IMPORT_C const TMnMapImageParams& MapImageParams() const;

        /** Sets all map image drawing parameters at once.
         *  New options will be applied only for next and following
         *  RenderL requests.
         *  @param aImageParams Image parameters to take into use for
         *                      following RenderL requests */
        IMPORT_C void SetMapImageParams( TMnMapImageParams& aImageParams );

        /** Specifies the origin of image in the target bitmap, see
         *  RenderL(). By default it is TPoint( 0, 0 ), i.e. left top
         *  corner of the bitmap.
         *  This setting takes effect only from next RenderL request.
         *  @param[in] aOrigin Origin of the map in the target bitmap.*/
		IMPORT_C void SetTargetOrigin( const TPoint& aOrigin );

        /** Returns current setting of map image origin in target bitmap.
         *  @returns Origin of map image in the target bitmap.*/
		IMPORT_C TPoint TargetOrigin();

        /** Sets additional options to be used in rendering. Renderer will
         *  take them into account if possible.
         *  This setting takes effect only from next RenderL request.
         *  @param[in] aOptions Bitmap of options defined in @ref TOption. */
		IMPORT_C void SetShowOptions( const TShowOptions& aOptions );

        /** Returns additional options set to be used in rendering.
         *  @return Bitmap of options defined in @ref TShowOption. */
		IMPORT_C TShowOptions ShowOptions() const;

        /** Intiates rendering of map image.
         *  This is asynchronous request, which completion is informed via
         *  aStatus parameter. Client should use active object to wait for completion.
         *
         *  This function may leave if some error occurs during initiating of the request.
         *  If so, then no asynchronous request will be left ongoing.
         *
         *  @param[in,out] aBitmap Bitmap object, which the map image will be drawn into.
         *                 Its size in pixels must be sufficient to fit the image
         *                 defined by @ref SetSize() taking @ref SetTargetOrigin()
         *                 into account as well.
         *  @param[out] aStatus This status will be completed, when rendering is finished.
         *                      - @p KErrNone - rendering was completed successfully.
         *                      - @p KErrNotSupported - rendering is not possible in specified area
         *                           or one of mandatory options are not supported (see @ref TShowOption).
         *                      - Other system-wide error codes also possible.
         *
         *  @leave KErrArgument
         *      - If target bitmap size is too small.
         *      - If requested image type is not supported by provider application
         *
         *  @panic "MnPanicClient"-KMnPanicDuplicateRequest if request is already
         *         ongoing. */
		IMPORT_C void RenderL( CFbsBitmap& aBitmap, TRequestStatus& aStatus );

        /** Intiates rendering of map image.
         *  This is asynchronous request, which completion is informed via aCallback.
         *
         *  Client must provide callback as class derived from MMnMapImageRenderCallback.
         *  This is asynchronous request also. An active object will be executed to handle
         *  completion of rendering request, so client must have active scheduler installed.
         *
         *  This function may leave if some error occurs during initiating of the request.
         *  If so, then no asynchronous request will be left ongoing.
         *
         *  @param[in,out] aBitmap Bitmap object, which the map image will be drawn into.
         *                 Its size in pixels must be sufficient to fit the image
         *                 defined by @ref SetSize() taking @ref SetTargetOrigin()
         *                 into account as well.
         *  @param[in] aCallback Callback object to call when rendering is completed.
         *                 See @ref MMnMapImageRenderCallback::HandleRenderingCompletedL for details
         *                 on possible operation result error codes.
         *
         *  @leave KErrArgument
         *      - If target bitmap size is too small.
         *      - If requested image type is not supported by provider application
         *
         *  @panic "MnPanicClient"-KMnPanicDuplicateRequest if request is already
         *         ongoing. */
        IMPORT_C void RenderL( CFbsBitmap& aBitmap, MMnMapImageRenderCallback& aCallback );

        /** Cancels current ongoing asynchronous rendering request.
         *  Note that callback, set in @ref RenderL(CFbsBitmap&, MMnMapImageRenderCallback&)
         *  is <i>not</i> called, if request is cancelled. */
		IMPORT_C void CancelRendering();

        /** Calculates coordinate in pixels of the point corresponding to given
         *  world coordinate.
         *
         *  Given world coordinate is allowed to be beyond
         *  image borders. In that case negative values in result value
         *  describe point to the left of left border and/or above top border of the
         *  image and values bigger than width or height describe point to the right
         *  of right image border or below its bottom border.
         *
         *  Sometimes such calculation is not possible (e.e given world coordinate
         *  is not part of the image) and this function returns @c KErrUnknown.
         *
         *  Map image parameters are necessary be set in order for this function
         *  to operate successfully. Default values are not sufficient, so client
         *  is responsible to override them before calling this method. Note though,
         *  that it is not required to call RenderL before calling this method.
         *
         *  @param[in] aWorldCoordinate World coordinate (in WGS-84)
         *  @param[out] aImagePoint On successful return, coordinate of corresponding
         *                          pixel in image relative to image top left corner.
         *  @return Error code of the operation:
         *      - @c KErrNone if calculation is successful
         *      - @c KErrArgument if map image parameters in are invalid.
         *      - @c KErrUnknown if calculation is not possible */
		IMPORT_C TInt GetImageCoordinate(
            const TCoordinate& aWorldCoordinate,
            TPoint& aImagePoint ) const;

        /** Calculates world coordinate of the given image point.
         *
         *  Given image point coordinate must be within the rectangle
         *  ( TPoint( 0, 0 ), @ref Size() ).
         *
         *  Map image parameters are necessary be set in order for this function
         *  to operate successfully. Default values are not sufficient, so client
         *  is responsible to override them before calling this method. Note though,
         *  that it is not required to call RenderL before calling this method.
         *
         *  Sometimes such calculation is not possible (e.g. given image coordinate
         *  does not match any world coordinate) and this function returns @c KErrUnknown.
         *
         *  @param[in] aImagePoint Coordinate of a point on map image in pixels,
         *             relative to image top left corner.
         *  @param[out] aWorldCoordinate On successful return, corresponding
         *                               world coordinate (in WGS-84).
         *  @return Error code of the operation:
         *      - @c KErrNone if calculation is successful
         *      - @c KErrArgument if image coordinate is out of bounds
         *      - @c KErrArgument if map image parameters in are invalid.
         *      - @c KErrUnknown if calculation is not possible */
		IMPORT_C TInt GetWorldCoordinate(
            const TPoint& aImagePoint,
            TCoordinate& aWorldCoordinate ) const;
         
        /** Sets observer of provider application lifetime.
         *  Client will receive exit event from provider application.
         *  By default exit events from provider application are not monitored.
         *  @param[in] aObserver Exit observer.
         *  @leave KErrAlreadyExists if observer is already set. 
         */
        IMPORT_C void SetExitObserverL( MAknServerAppExitObserver& aObserver );

        /** Removes observer of provider application lifetime. */
        IMPORT_C void RemoveExitObserver();

	protected:
		CMnMapImage();
		void ConstructL( CMnProvider& aProvider );
        
    private:
        TUint32 iProviderFeatures;
    
        TPoint iTargetOrigin;    
        TMnMapImageParams iMapImageParams;
        TShowOptions iShowOptions;
        
        RMnMapImageServiceClient* iSession;
        CApaServerAppExitMonitor* iExitMonitor;
        CActive* iActiveCallback;
        
        CMnCoordinateConverterBase* iCoordConverter;
	};

#endif /*MN_MAPIMAGE_H_*/
