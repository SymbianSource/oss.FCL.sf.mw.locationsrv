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
* Description:  CMnServiceBase class
*
*/


#ifndef MN_SERVICEBASE_H
#define MN_SERVICEBASE_H

#include <AknServerApp.h>

class CPosLandmark;

/** @brief Base class for all Map and Navigation services.
 *
 *  Its only purpose is to provide common base functionality for Map and Navigation
 *  service classes.
 *
 *  There are both synchronous and asynchronous request handlers defined in derived
 *  service base classes. Implementations of handlers shall behave as follows.
 *  <ul>
 *  <li>For Synchronous requests:</li>
 *  <ul>
 *  <li>Exit normally to report successful completion</li>
 *  <li>Leave with error code to report error</li>
 *  </ul>
 *  <li>For Asynchronous requests:</li>
 *  <ul>
 *  <li>Exit normally as soon as possible. Call dedicated Complete method
 *      to report successful completion</li>
 *  <li>Leave with error code to report error found during execution of
 *      Handle method</li>
 *  <li>Call CompleteRequest method with error code to report error
 *      found after execution of Handle method</li>
 *  <li>Call CancelRequest method to report that user has cancelled
 *      execution</li>
 *  </ul>
 *  </ul>
 *
 *  Following error codes shall be used by implementation to report specific cases:
 *  - KErrCancel - If user has cancelled operation.
 *  - KErrArgument - If given argument cannot be used for operation. For example, landmark has
 *          no coordinate or address and cannot be shown on map or address information
 *          is not sufficient for geocoding.
 *  - KErrNotSupported - If service feature is not supported, or cannot be executed with given parameters.
 *          For example, map content not available for specified area etc.
 *
 *  @since 3.1
 *  @lib mnservicelib.lib
 *  @ingroup MnProviderAPI
 */
class CMnServiceBase : public CAknAppServiceBase
    {
    public :

        /** @brief Completes current asynchronous request.
         *
         *  Called by implementations to complete client's request. Used
         *  by service implementations to report errors during execution
         *  of asynchronous requests. To report successful completion,
         *  special methods defined by service base classes should be used
         *  (e.g. @ref CMnMapViewServiceBase::CompleteSelectionRequest)
         *
         *  For synchronous requests, handler should
         *  just leave if error is detected.
         *
         *  @param aResult Error code.
         *
         *  @panic "MnPanicServer"-KMnPanicAttemptToCompleteNoRequest
         *      This method called during execution of synchronous request.
         */
        IMPORT_C void CompleteRequest( TInt aResult );

    protected :
        /** C++ constructor */
        CMnServiceBase();
        /** Destructor */
        ~CMnServiceBase();

        /** \internal */
        void BaseConstructL();

        /** @brief Reports that client cancelled request.
         *
         *  Called by framework to report that request was
         *  cancelled by client application. Service implementations
         *  must implement this method. It is not needed to call
         *  @ref CompleteRequest() in this case.
         */
        virtual void DoCancel() =0;

    protected: // internal methods

        /** \internal
         *  Completes client's message
         */
        void Complete( const RMessage2& aMsg, TInt aResult );

        /** \internal
         *  Informs derived implementation (by calling DoCancel())
         *  that current async request has been cancelled by client and
         *  completes it with KErrCancel.
         */
        void HandleCancelRequestL( const RMessage2& aMsg );

        /** \internal
        * Copies an 8-bit buffer from the address space of the client and puts
        * the result in the returned argument.
        *
        * @param aMessage the message from the client.
        * @param aClientBufferParam index of message parameter to read as buffer
        * @return a copy of the client buffer.
        */
        static HBufC8* CopyClientBuffer8LC(
            const RMessage2& aMessage,
            const TInt aClientBufferParam );

        /** \internal
        * Copies a 16-bit buffer from the address space of the client and puts
        * the result in the returned argument.
        *
        * @param aMessage the message from the client.
        * @param aClientBufferParam index of message parameter to read as buffer
        * @return a copy of the client buffer.
        */
        static HBufC* CopyClientBufferLC(
            const RMessage2& aMessage,
            const TInt aClientBufferParam );

        /** \internal
         *  Unpacks landmark from client's message.
         *  @param  aMsg client's message
         *  @param  aParamIndex index of landmark parameter in message
         *  @return new copy instance of client's landmark
         */
        CPosLandmark* UnpackLandmarkLC( const RMessage2& aMsg, TInt aParamIndex );

        /** @internal */
        TInt CurrentAsyncRequest();
        /** @internal */
        TBool IsAsyncRequestActive();
        /** @internal */
        void PrepareAsyncRequestL( const RMessage2& aMessage );

    protected:  // from CApaAppServiceBase
        /** @internal */
        IMPORT_C void ServiceError( const RMessage2 &aMessage, TInt aError );

    protected:
        RMessage2   iMessage;

    private:
        TInt        iCurrentRequest;
    };

#endif // MN_SERVICEBASE_H

