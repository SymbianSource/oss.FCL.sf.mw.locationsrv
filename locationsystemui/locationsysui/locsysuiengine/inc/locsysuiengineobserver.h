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
* Description:  Observer to the Location System UI Engine. 
*                Notfies changes in the Location System UI related UI components
*
*/


#ifndef M_LOCSYSUIENGINEOBSERVER_H_
#define M_LOCSYSUIENGINEOBSERVER_H_

// System Include
#include <e32base.h>

// Class Declaration

/**
 * Observer class to Location System UI Engine.
 * It provides notfication about the user termination of the Location UIs.
 *
 * @lib locsysuiengine.lib
 * @since S60 v3.2
 */
class MLocSysUiEngineObserver
	{
	public:
		/**
		 * Notifies the termination of the Location UI.
		 *
		 * @since S60 v3.2
		 * @param aErrorCode The termination reason. KErrNone for normal 
		 *					 terminations. In case of error or pre-mature
		 *					 aborting System wide Error codes.
		 */
		 virtual void LocationUIDismissed( TInt aErrorCode )			= 0;
	};

#endif // M_LOCSYSUIENGINEOBSERVER_H_


