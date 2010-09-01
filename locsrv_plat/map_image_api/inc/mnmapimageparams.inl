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
* Description:  TMnMapImageParams class implementation
*
*/



#ifndef MNMAPIMAGEPARAMS_INL_
#define MNMAPIMAGEPARAMS_INL_

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void TMnMapImageParams::SetCenterPoint( const TCoordinate& aCoordinate )
    {
    iCenterPoint = aCoordinate;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void TMnMapImageParams::GetCenterPoint( TCoordinate& aCoordinate ) const
    {
    aCoordinate = iCenterPoint;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void TMnMapImageParams::SetVisibleArea( TReal32 aWidth, TReal32 aHeight )
    {
    iAreaWidth = aWidth;
    iAreaHeight = aHeight;
    }
        
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void TMnMapImageParams::GetVisibleArea( TReal32& aWidth, TReal32& aHeight ) const
    {
    aWidth = iAreaWidth;
    aHeight = iAreaHeight;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void TMnMapImageParams::SetSize( const TSize& aImageSize )
    {
    iSize = aImageSize;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
TSize TMnMapImageParams::Size() const
    {
    return iSize;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void TMnMapImageParams::SetDirection( TReal32 aDirection )
    {
    iDirection = aDirection;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
TReal32 TMnMapImageParams::Direction() const
    {
    return iDirection;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void TMnMapImageParams::SetImageType( TMapImageType aImageType )
    {
    iImageType = aImageType;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
TMnMapImageParams::TMapImageType TMnMapImageParams::ImageType() const
    {
    return iImageType;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void TMnMapImageParams::SetProjectionId( TInt aProjectionId )
    {
    iProjectionId = aProjectionId;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
TInt TMnMapImageParams::ProjectionId()
    {
    return iProjectionId;
    }

#endif /*MNMAPIMAGEPARAMS_INL_*/
