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
* Description:  The set of messages provided to Secondary Display software by
*                Location Verifier dialogs.
*
*/

#ifndef LOCVERIFIERCOVERUI_H_
#define LOCVERIFIERCOVERUI_H_

// System Includes
#include <e32base.h>

// The category UID for the commands in this header file.
// UID of the Location Verifier dialogs Dll ( locverifierdlg.dll )
//
const TUid KCatUidLocationVerifierDlgs = { 0x101f84fc };


// Dialogs shown by Location System UI / Verifer dialogs. These commands are 
// handled using the Secondary Display support in Avkon.
// Verification dialogs seek a response from the user about the decision that
// has to be taken about accepting or rejecting a Location request. 
// Notification dialogs provides a notification to the user about a particular
// action taken to a Location request.
// All the below mentioned command are expected to display a List Query dialog.
// The requestor strings for filling up the List query dialog would be supplied 
// as a packaged buffer. The buffer is packed according to the below mentioned
// format.
// 
// | Count | Length(1) | String(1) | Length(2) | String(2) | ..... | Length( Count ) | String( Count ) |
// 
// where,
// Count   - ( 4 bytes ) - The number of requestor strings. 
// Length(n) - ( 4 bytes ) - Length of the UNICODE ( UCS -2) encoded nth requestor string.
// String(n) - ( Length(n) * 2 bytes ) - String for the nth requestor. String encoded in 
// UNICODE ( UCS-2 )  format. Hence, each character takes 2 bytes. Hence, over all length of the
// string in bytes is 2 * Length(n).
// 
// Each entry is packed one after another without any delimters between them
//
//
enum TLocVerifierDlgCoverUiCmds
    {
    /**
     * Command ID for Location verification request when the network default policy for the 
     * request is accept
     * Request Type : Verification Request
     * Dialog Type : List Query Dialog
     * Hint String : "By default the network will accept this request on your behalf. Requesters:"
     */    	
    ECmdDefaultAccept 			= 1,
    
    /**
     * Command ID for Location verification request when the network default policy for 
     * the request is reject
     * Request Type : Verification Request
     * Dialog Type : List Query Dialog     
     * Hint String : "By default the network will reject this request on your behalf. Requesters:"
     */     
    ECmdDefaultReject 		  = 2,
    
    /**
     * Command ID for Location verification request when the network default policy for 
     * the request is not specified
     * Request Type : Verification Request
     * Dialog Type : List Query Dialog      
     * Hint String : "Your location was requested by:"
     */     
    ECmdDefaultNone   		  = 3,
    
    /**
     * Command ID for Location notification
     * Request Type : Notification Request
     * Dialog Type : List Query Dialog      
     * Hint String : "The network sent your location to:"
     */     
    ECmdNotifyAccept			  = 4,
    
    /**
     * Command ID for Location notification when network made "accept" decision due to timeout
     * Request Type : Notification Request
     * Dialog Type : List Query Dialog           
     * Hint String : "The network accepted this request on your behalf. Your location was sent to:"
     */     
    ECmdNotifyAcceptTimeout = 5,
    
    /**
     *Command ID for Location notification when network made "reject" decision due to timeout
     * Request Type : Notification Request
     * Dialog Type : List Query Dialog           
     * Hint String : "The network rejected this request on your behalf. Your location was not sent to:"
     */     
    ECmdNotifyRejectTimeout = 6,
    
    /**
     * Command ID for Location notification when request was rejected due to a conflict despite of user's "accept" decision.
     * Request Type : Notification Request
     * Dialog Type : List Query Dialog           
     * Hint String : "Unable to accept the request"
     */     
    ECmdNotifyAcceptFailure = 7,
    
    /**
     * Command ID for Location notification when request was accepted due to a conflict despite of user's "reject" decision.
     * Request Type : Notification Request
     * Dialog Type : List Query Dialog           
     * Hint String : "Unable to reject the request"
     */     
    ECmdNotifyRejectFailure = 8
    };

#endif      // LOCVERIFIERCOVERUI_H_            
// End of File
