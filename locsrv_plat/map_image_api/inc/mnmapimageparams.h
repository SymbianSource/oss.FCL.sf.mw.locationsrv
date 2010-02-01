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
* Description:  TMnMapImageParams class
*
*/


#ifndef MNMAPIMAGEPARAMS_H_
#define MNMAPIMAGEPARAMS_H_

#include <lbsposition.h>

/** Container class for map image rendering parameters.
 *  It stores following parameters:
 *   - center coordinate of map image,
 *   - size of the image,
 *   - image scale
 *   - image direction
 *   - type of image 
 *  @since 5.0 */
class TMnMapImageParams
    {
    public:

        /** Types of map images */
        enum TMapImageType
            {
            /** Type is unknown or unspecified */
            ETypeUnspecified        = 0x00,
            /** Scalable graphical maps (e.g. street maps) */
            ETypeVectorMap          = 0x01,
            /** Image built of satellite photographs of Earth */
            ETypeSatelliteImage     = 0x02
            };
            
        /** Type for bitmap of image types defined in @ref TMapImageType */
        typedef TUint32 TMapImageTypes;

    public:
        IMPORT_C TMnMapImageParams();

        /** Sets a world coordinate as a center of desired image.
         *  @param[in] aCoordinate World location to be center of rendered map.*/
        inline void SetCenterPoint( const TCoordinate& aCoordinate );

        /** Returns world location, selected by client as the map center.
         *  @param aCoordinate Coordinate of map's center. */
        inline void GetCenterPoint( TCoordinate& aCoordinate ) const;

        /** Sets the width and heigth of the world area to be covered by the map image.
         *  These two parameters define an ellipse, covering the world area, 
         *  which is <i>required</i> to be visible on the map. Actual visible area
         *  can be larger, if given image size and area have different aspect ratios.
         *  @param[in] aWidth The length in meters of the line, orthogonal to the image 
         *      direction and going through center point
         *  @param[in] aHeight The length in meters of the line, parallel to the image 
         *      direction and going through center point */
        inline void SetVisibleArea( TReal32 aWidth, TReal32 aHeight );
        
        /** Returns the width and heigth of the world area to be covered by the map image.
         *  These two parameters define an ellipse, covering the world area, 
         *  which is <i>required</i> to be visible on the map. Actual visible area
         *  can be larger, if given image size and area have different aspect ratios.
         *  @param[out] aWidth The length in meters of the line, orthogonal to the image 
         *      direction and going through center point
         *  @param[out] aHeight The length in meters of the line, parallel to the image 
         *      direction and going through center point */
        inline void GetVisibleArea( TReal32& aWidth, TReal32& aHeight ) const;

        /** Sets size of desired map image in pixels.
         *  @param aImageSize Width and height of the map image. */
        inline void SetSize( const TSize& aImageSize );

        /** Returns size of desired map image in pixels.
         *  @return Width and height of the map image. */
        inline TSize Size() const;

        /** Sets required direction of the map.
         *  Image direction is defined as clockwise angle between two vectors:
         *  - a.  vector from center point towards true North Pole parallel to center’s longitude.
         *        If center is positioned at North Pole, then this vector is directed
         *        towards South Pole parallel to center’s longitude.
         *  - b.  vector from center point (see @ref SetCenterPoint()) towards center of image’s top side.
         * 
         *  This direction parameter can be thought of as "true heading" of the image.
         *
         * @param aDirection Direction in degrees. */
        inline void SetDirection( TReal32 aDirection );

        /** Returns selected direction of the map. For details of direction definition,
         *  see @ref SetDirection().
         *  @return Direction in degrees.*/
        inline TReal32 Direction() const;

        /** Sets desired type of map image. If set, then rendering will
         *  provide (if supported) maps of specified type. Default value is
         *  ETypeUnspecified, which allows provider to select best suitable image
         *  for given scale, center etc.*/
        inline void SetImageType( TMapImageType aImageType );

        /** Returns type of last rendered map image. (It always matches the value
         *  specified by client in @ref SetImageType except if ETypeUnspecified
         *  is given by client.) */
        inline TMapImageType ImageType() const;

        /** Sets internal ID of a projection used by renderer.
         *  It is useful if provider supports several projections.
         *  Currently must not be used by clients, only for internal purposes
         *  of provider application.
         *  @param aProjectionId Renderer'd internal id of used projection */
        inline void SetProjectionId( TInt aProjectionId );

        /** Sets internal ID of a projection used by renderer.
         *  It is meaningful if provider supports several projections.
         *  Currently must not be used by clients, only for internal purposes
         *  of provider application.
         *  @return Renderer'd internal id of used projection. */
        inline TInt ProjectionId();

    private:
        TCoordinate     iCenterPoint;
        TReal32         iAreaWidth;
        TReal32         iAreaHeight;
        TSize           iSize;
        TReal32         iDirection;

        TMapImageType   iImageType;
        TInt            iProjectionId;

        TInt8           reserved[32];
    };

#include <mnmapimageparams.inl>

#endif /*MNMAPIMAGEPARAMS_H_*/
