/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Implementation of TOMASuplSettingsMap
*
*/




#include <e32def.h>
#include "epos_tomasuplsettingsmap.h"


TInt TOMASuplSettingsMap::Map(const TDesC8& aSettingName, TSuplSettingType& suplSettingType )
    {
    if( aSettingName == KSlpAddr || aSettingName == KSlpAddrFull )
        {
        suplSettingType = ESlp;
        }
    else if( aSettingName == KIapIdName || aSettingName == KIapIdFull )
        {
        suplSettingType = EPrefConRef;	
        }
    else if( aSettingName == KProviderId || aSettingName == KProviderIdFull )
         {
         suplSettingType = EProviderId; 
         }
    else if( aSettingName == KName || aSettingName == KNameFull )
         {
         suplSettingType = EName; 
         }
    else if( aSettingName == KToConRef || aSettingName == KToConRefFull )
         {
         suplSettingType = EToConRef; 
         }
    else if( aSettingName == KConRef || aSettingName == KConRefFull )
         {
         suplSettingType = EConRef; 
         }
    else if( aSettingName == KAddrType || aSettingName == KAddrTypeFull )
         {
         suplSettingType = EAddrType; 
         }
    else
        {
        suplSettingType = ENone;
        return KErrNotFound;	
        }
    return KErrNone;
    }


TInt TOMASuplSettingsMap::Get( const TDesC8& aUri, RChildList& aList, 
                                    RArray<TPtrC8>& aChildren )
    {
    TInt rc = KErrNone;

    if( aUri == KRootFull )
        {
        TSettingMap settingName;       
        settingName.iSettingName.Set( KNameFull );
        aList.Append(settingName);
        aChildren.Append( TPtrC8( KName.iBuf, KName.iTypeLength ) );
        
        TSettingMap settingIapId;		
        settingIapId.iSettingName.Set( KIapIdFull );
        aList.Append(settingIapId);
        aChildren.Append( TPtrC8( KIapIdName.iBuf, KIapIdName.iTypeLength ) );
        
        TSettingMap settingProviderId;       
        settingProviderId.iSettingName.Set( KProviderIdFull );
        aList.Append(settingProviderId);
        aChildren.Append( TPtrC8( KProviderId.iBuf, KProviderId.iTypeLength ) );

        TSettingMap settingSlp;     
        settingSlp.iSettingName.Set( KSlpAddrFull );
        aList.Append( settingSlp );
        aChildren.Append( TPtrC8( KSlpAddr.iBuf, KSlpAddr.iTypeLength ) );
        
        TSettingMap settingToConRef;       
        settingToConRef.iSettingName.Set( KToConRefFull );
        aList.Append(settingToConRef);
        aChildren.Append( TPtrC8( KToConRef.iBuf, KToConRef.iTypeLength ) );
        
        TSettingMap settingConRef;       
        settingConRef.iSettingName.Set( KConRefFull );
        aList.Append(settingConRef);
        aChildren.Append( TPtrC8( KConRef.iBuf, KConRef.iTypeLength ) );
        
        TSettingMap settingAddrType;       
        settingAddrType.iSettingName.Set( KAddrTypeFull );
        aList.Append(settingAddrType);
        aChildren.Append( TPtrC8( KAddrType.iBuf, KAddrType.iTypeLength ) );        
        }
    else
        {
        rc = KErrNotFound;	
        }

    return rc;		
    }


TInt TOMASuplSettingsMap::FileName( const TDesC8& aUri, const TDesC*& aFileName )
    {
    if( aUri == KRootFull || aUri == KRootNode )
        {
        aFileName = &KSuplFile;	
        }
    else if( aUri == KSlpAddrFull || aUri == KSlpAddr )
        {
        aFileName = &KSlpFile;	
        }
    else if( aUri == KIapIdFull || aUri == KIapIdName )
        {
        aFileName = &KIAPIDFile;
        }
    else if( aUri == KProviderId || aUri == KProviderIdFull )
        {
        aFileName = &KProviderIdFile; 
        }
    else if( aUri == KName || aUri == KNameFull )
        {
        aFileName = &KNameFile; 
        }
    else if( aUri == KToConRef || aUri == KToConRefFull )
        {
        aFileName = &KToConRefFile; 
        }
    else if( aUri == KConRef || aUri == KToConRefFull )
        {
        aFileName = &KToConRefFile; 
        }
    else if( aUri == KAddrType || aUri == KAddrTypeFull )
        {
        aFileName = &KAddrTypeFile; 
        }
    else
        {
        return KErrNotFound;	
        }

    return KErrNone;
    }
