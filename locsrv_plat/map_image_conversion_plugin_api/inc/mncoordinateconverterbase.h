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
* Description:  CMnCoordinateConverterBase class
*
*/


#ifndef MN_COORDINATECONVERTERBASE_H
#define MN_COORDINATECONVERTERBASE_H

#include <mnmapimageparams.h>

/** This is the base service interface to access coordinate conversion 
 *  functionality of MapImage service capable provider applications.
 *
 *  This interface must be implemented by Map and Navigation provider applications,
 *  which support @c KMnAppServiceMapImage service.
 *
 *  @lib mnclientlib.lib
 *  @since 5.0 */
class CMnCoordinateConverterBase : public CBase
    {
    public:
        static CMnCoordinateConverterBase* NewL( TUid aProviderUid );

        /** Base destructor */
        IMPORT_C ~CMnCoordinateConverterBase();
        
        /** Returns drawing parameters of map image.
         *  @param aImageParams Parameters of map image. */
        IMPORT_C const TMnMapImageParams& MapImageParams() const;
        
        /** @internal */
        /*  Sets drawing parameters of map image. Implementation can read
         *  these by calling MapImageParams().
         *  @param aImageParams Parameters of map image. */
        void SetMapImageParams( const TMnMapImageParams& aImageParams );

        /** Calculates coordinate in pixels of the point corresponding to given
         *  world coordinate. 
         * 
         *  Given world coordinate is allowed to be beyond 
         *  image borders. In that case negative values in result value
         *  describe point to the left of left border and/or above top border of the
         *  image and values bigger than width or height describe point to the right
         *  of right image border or below its bottom border.
         * 
         *  If such calculation is not possible (e.e given world coordinate is not 
         *  part of the image) this function must return @c KErrUnknown.
         * 
         *  @param[in] aWorldCoordinate World coordinate (in WGS-84) 
         *  @param[out] aImagePoint Coordinate of corresponding pixel in image 
         *                          relative to image top left corner. 
         *  @return Error code of the operation:
         *      - @c KErrNone if calculation is successful
         *      - @c KErrArgument if map image parameters in are invalid.
         *      - @c KErrUnknown if calculation is not possible */
        virtual TInt GetImageCoordinate( 
            const TCoordinate& aWorldCoordinate, 
            TPoint& aImagePoint ) = 0;
        
        /** Calculates world coordinate of the given image point. 
         * 
         *  Given image point coordinate must be within the rectangle 
         *  ( TPoint( 0, 0 ), @ref Size() ).
         * 
         *  If such calculation is not possible (e.g. given image coordinate does not 
         *  match any world coordinate) this function must return @c KErrUnknown.
         * 
         *  @param[in] aImagePoint Coordinate of a point on map image in pixels, 
         *             relative to image top left corner. 
         *  @param[out] aWorldCoordinate On successful return, corresponding
         *                               world coordinate (in WGS-84).
         *  @return Error code of the operation:
         *      - @c KErrNone if calculation is successful
         *      - @c KErrArgument if image coordinate is out of bounds
         *      - @c KErrArgument if map image parameters in are invalid.
         *      - @c KErrUnknown if calculation is not possible  */
        virtual TInt GetWorldCoordinate( 
            const TPoint& aImagePoint, 
            TCoordinate& aWorldCoordinate ) = 0;
                    
    protected:    
        IMPORT_C CMnCoordinateConverterBase();
        
    private:
        TUid                iDtorIdKey;
        TMnMapImageParams   iImageParams;
    };

#endif /*MN_COORDINATECONVERTERBASE_H*/
