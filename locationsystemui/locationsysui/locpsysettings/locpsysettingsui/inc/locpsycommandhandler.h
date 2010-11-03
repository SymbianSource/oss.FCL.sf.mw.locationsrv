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
* Description:  Interface class that provides the command handling functionality.
*				 The class implementing it should be prepared to handle all commands
*				 except those internal to the calling UI
*
*/


#ifndef MLOCPSYCOMMANDHANDLER_H_
#define MLOCPSYCOMMANDHANDLER_H_

// System Include 
#include <e32base.h>

// Class Definition
/**
 *	Interface class that provides the command handling functionality.
 */
class MLocPsyCommandHandler
	{
	public:
		/**
		 * Command handlikng function
		 *
		 * @param aCommand Command that needs to be handled
		 */
		 virtual void HandleCommandL( TInt aCommand )	= 0;
	};

#endif //MLOCPSYCOMMANDHANDLER_H_

