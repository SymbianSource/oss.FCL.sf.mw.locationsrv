/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Observer to the Views Engine. Notfies changes in the Settings UI
*				 and E-Com events
*
*/


#ifndef M_LOCSETTINGSUIECOMNOTIFIER_H_
#define M_LOCSETTINGSUIECOMNOTIFIER_H_

// System Include
#include <e32base.h>

// Class Declaration

/**
 * Observer class to CLocSettingsUiEngine.
 * Inaddtion to it it also provides notfication regarding the changes the 
 * plug-in loading and unloading states
 *
 * @lib locsysuiview.lib
 * @since S60 v3.2
 */
class MLocSettingsUiEcomNotifer
	{
	public:
		/** 
		 * Notifies any change in the state of the underlying Settings UI
		 * session
		 *
		 * @since S60 v3.1
		 * @param aErrorCode Error Code denoting the state change
		 */
		 virtual void EComSessionStateChanged( TInt aErrorCode )	= 0;
	};

#endif // M_LOCSETTINGSUIENGINEOBSERVER_H_
