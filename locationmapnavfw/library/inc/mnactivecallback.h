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
* Description:  CMnActiveCallback class
*
*/


#ifndef MN_ACTIVECALLBACK_H_
#define MN_ACTIVECALLBACK_H_

#include <e32base.h>

class CMnMapImage;
class MMnMapImageRenderCallback;
class RMnMapImageServiceClient;

/** This is an active object, which calls MMnMapImageRenderCallback,
    when its asyncronous request is completed. Used to wrap 
    asynchronous CMnMapImage::RenderL. */
NONSHARABLE_CLASS( CMnActiveCallback ): public CActive
    {
    public:
        static CMnActiveCallback* NewL( 
            CMnMapImage& aOwner,
            RMnMapImageServiceClient& aSession );
        
        void StartL( CFbsBitmap& aBitmap, MMnMapImageRenderCallback& aCallback );
        
    protected: // From CActive
        void RunL();
        void DoCancel();
        TInt RunError( TInt aError );    
    
    private:
        CMnActiveCallback( 
            CMnMapImage& aOwner,
            RMnMapImageServiceClient& aSession );

    private:
        CMnMapImage& iOwner;
        RMnMapImageServiceClient& iSession;
        MMnMapImageRenderCallback* iCallback;
    };

#endif //MN_ACTIVECALLBACK_H_