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
* Description:  CMnMapImageServiceBase class
*
*/


#ifndef MN_MAPIMAGESERVICEBASE_H_
#define MN_MAPIMAGESERVICEBASE_H_

#include <mnservicebase.h>
#include <mnmapimage.h>
#include <mnmapimageparams.h>

class CFbsBitmap;

/** Base class for MapImage service implementations (@p KMnAppMapImageService). 
 *
 *  Provider Application
 *  must derive its MapImage service class from this one. This base implementation
 *  hides IPC communication details and provides native methods for accessing
 *  various parameters, specified by client.
 *
 *  Provider Application must return pointer to instance of its class, derived
 *  from this class, in response to @p CAknAppService::CreateServiceL() call,
 *  if given parameter is KMnAppMapImageService. 
 *
 *  This class defines handlers of particular requests from client as pure 
 *  virtual methods, which derived class must implement (@ref HandleRenderingL). 
 *  The parameters, which client application may specify in CMnMapImage are 
 *  accessible via other methods of this class. Provider Application must obey 
 *  these parameters.
 *
 *  See parent @ref CMnServiceBase class documentation for 
 *  other details.
 *
 *  @since 5.0
 *  @lib mnservicelib.lib
 *  @ingroup MnProviderAPI
 */
class CMnMapImageServiceBase : public CMnServiceBase
    {
    public :

        /** Returns drawing parameters of map image.
         *  @param aImageParams Parameters of map image. */
        IMPORT_C const TMnMapImageParams& MapImageParams() const;
        
        /** Returns drawing parameters of map image.
         *  @param aImageParams Parameters of map image. */
        IMPORT_C TMnMapImageParams& MapImageParams();

        /** @internal */
        /*  Sets drawing parameters of map image. Implementation can read
         *  these by calling MapImageParams().
         *  @param aImageParams Parameters of map image. */
        void SetMapImageParams( const TMnMapImageParams& aImageParams );

        /** Returns origin and size of needed image in pixels.
         *  Size part of that rectangle is the same as specified in MapImageParams().
         *  @return Rectangle within destination bitmap (see @ref TargetBitmap() ), 
         *          where the map should be drawn to. */
    	IMPORT_C TRect TargetRect() const;
    	
        /** Returns rendering options set by client. Rendering process
         *  must obey these options. If it is not possible, it should complete
         *  rendering with KErrNotSupported.
         *  @returns Options defined in CMnMapImage::TShowOptions. */
    	IMPORT_C CMnMapImage::TShowOptions ShowOptions() const;
    	
        /** Returns reference to target bitmap object. This is the bitmap, 
         *  which the map image should be rendered to. 
         *  It is guaranteed that bitmap's size in pixels is sufficient 
         *  for drawing. 
         *  This method should be called only during processing of rendering
         *  request, i.e. not before HandleRenderingL() or after 
         *  CompleteRendering() */
        IMPORT_C CFbsBitmap& TargetBitmap() const;
        
        /** Must be called by client when rendering is successfully completed. 
         *  (If there were an error, then CMnServiceBase::CompleteRequest() 
         *  should be called instead.)
         * 
         *  Before completion, actual type of image drawn should be set in 
         *  MapImageParams() in case original value was TMnMapImageParams::ETypeUnspecified.
         * 
         *  @panic "MnPanicServer"-KMnPanicInvalidResult if image type in
         *                         @p MapImageParams() is TMnMapImageParams::ETypeUnspecified.
         *  @panic "MnPanicServer"-KMnPanicAttemptToCompleteNoRequest if
         *         there is no ongoing asynchronous request. */
    	IMPORT_C void CompleteRendering();
    
    protected :
        /** C++ constructor */
        IMPORT_C CMnMapImageServiceBase();
        /** Destructor */
        IMPORT_C ~CMnMapImageServiceBase();

        /** This method must be called from derived class' ConstructL to
         *  allow internal initialization of this class. */
        IMPORT_C void BaseConstructL();

        /** Handles client's request for rendering map image. 
         *
         *  Derived class must implement this method if KMnSvcFeatureRender
         *  feature is specified in SERVICE_INFO.
         *
         *  Implementation should draw map image to bitmap object returned by 
         *  @ref TargetBitmap(). Drawing must be done to the area of the bitmap, 
         *  specified by @ref TargetRect(). 
         *  
         *  Map image parameters are specified in object of TMnMapImageParams and 
         *  can be accessed via @ref MapImageOptions(). Center of the drawing must correspond
         *  to world point given by @ref TMnMapImageParams::GetCentralPoint(), 
         *  scale must be as specified by @ref TMnMapImageParams::Scale(). Image 
         *  must be oriented as required by @ref TMnMapImageParams::Direction(). 
         *  Image type requested is defined in @ref TMnMapImageParams::ImageType().
         *  Additional options defined by @ref ShowOptions() must also be obeyed 
         *  (if possible).
         * 
         *  If any of those requirements can not be satisfied, rendering must be 
         *  completed with KErrNotSupported. If any of arguments are considered 
         *  generally invalid, then error code should be KErrArgument.
         * 
         *  This is asynchronous request.
         *  Implementation should complete this request by calling CompleteRendering()
         *  if rendering ended successfully or by calling CMnServiceBase::CompleteRequest()
         *  in case of error. 
         * 
         *  This request can be canceled by client, in which case base method 
         *  DoCancel() will be called.
         * 
         *  If this function leaves then client's request is automatically
         *  completed with error.
         * 
         *  @leave KErrNotSupported Should leave if this feature is not supported.
         */
        virtual void HandleRenderingL() =0;

    protected: // From CAknAppServiceBase

        /** @internal */
        IMPORT_C virtual void ServiceL( const RMessage2& aMessage );
        
    private:
        void StartAsyncRequestL( const RMessage2& aMessage );
    
        void UnpackOptionsL( const RMessage2& aMessage );    
        void UnpackImageParamsL( const RMessage2& aMessage );
        void UnpackBitmapHandleL( const RMessage2& aMessage );
        void HandleCancelRequestL( const RMessage2& aMessage );

    private:    
        TMnMapImageParams iMapImageParams;
        CMnMapImage::TShowOptions iShowOptions;
        TPoint iTargetOrigin;
        CFbsBitmap* iBitmap;
    };


#endif /*MN_MAPIMAGESERVICEBASE_H_*/
