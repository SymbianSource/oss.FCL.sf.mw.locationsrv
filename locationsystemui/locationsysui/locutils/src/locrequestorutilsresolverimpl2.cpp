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
* Description:  Requestor utilities, supports privacy rules.
*
*/


// INCLUDE FILES

#include <e32base.h>
#include <barsread.h>

#include <lbs/epos_cposrequestor.h>
#include <lbs/epos_cposservicerequestor.h>
#include <lbs/epos_cposcontactrequestor.h>

// Virtual Phonebook Engine API
#include <CVPbkContactManager.h>
#include <VPbkContactStoreUris.h>
#include <CVPbkContactStoreUriArray.h>
#include <TVPbkContactStoreUriPtr.h>
#include <MVPbkContactLinkArray.h>
#include <MVPbkContactOperationBase.h>
#include <MVPbkContactLink.h>
#include <MVPbkStoreContact.h>
#include <TVPbkFieldVersitProperty.h>
#include <VPbkFieldType.hrh>
#include <MVPbkContactStoreList.h>
#include <MVPbkContactStore.h>
#include <TVPbkFieldTypeMapping.h>
#include <CVPbkFieldTypeSelector.h>

// Virtual Phonebook Presentation API
#include <CPbk2StoreConfiguration.h>
#include <Pbk2ContactNameFormatterFactory.h>
#include <CPbk2SortOrderManager.h>
#include <MPbk2ContactNameFormatter.h>
#include <CVPbkFieldTypeRefsList.h>
#include <MVPbkContactFieldData.h>

#include <cntdef.h>

#include "locrequestorutilsresolverimpl2.h"
#include "locutilsdebug.h"
#include "locfileutils.h"
#include <locutils.rsg>

// This is a non localisable resource file hence we can directly refer to the
// file as .rsc and we don't need to use the BaflUtils to load this resource
// file.
_LIT(KLocUtilsRscFile, "\\resource\\locutils.rsc");
// ============= CLocRequestorUtilsResolver MEMBER FUNCTIONS =================

// ---------------------------------------------------------------------------
// CLocRequestorUtilsResolver::NewL
// 2 Phase Constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CLocRequestorUtilsResolver* CLocRequestorUtilsResolver::NewL()
	{
	CLocRequestorUtilsResolver* self = 
							new(ELeave) CLocRequestorUtilsResolver();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
    return self;
	}

// ---------------------------------------------------------------------------
// destructor
// ---------------------------------------------------------------------------
//
CLocRequestorUtilsResolver::~CLocRequestorUtilsResolver()
	{
    delete iCntsOperation;
    delete iCntLinkArray;
    delete iContact;
    delete iPbkContactEngine;
    delete iPbkSortManager;
    delete iPbkCntFormatter;

    iEnv->DeleteResourceFile( iResourceOffset );
	}

// ---------------------------------------------------------------------------
// The method resolves requestors with the contact database
// ---------------------------------------------------------------------------
//
EXPORT_C void CLocRequestorUtilsResolver::ProcessRequestorsL( 
	RPointerArray<CPosRequestor>& aRequestors )
	{
    // The Contact Stores are all opened and setup for Search Operation
    // Let us start with the first requestor now.
	// The iCount member denotes the current requestor being processed.
	// Initialise the value to zero.
    iCount = 0;

	iRequestors = &aRequestors;
	// Initialize the state machine.
	iState = KNoProcessing;
	// Start the Active Object.
	ScheduleAORun();
	
	iWait.Start();
	}
	
// -----------------------------------------------------------------------------
// CLocRequestorUtilsResolver::PhoneNumberAsName
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CLocRequestorUtilsResolver::PhoneNumberAsName(
    const CPosRequestor& aRequestor )
    {
    if ( aRequestor.RequestorIdFormat() != CPosRequestor::EIdFormatPhoneNumber )
        { // id is of some other format
        return EFalse;
        }
    else
    	{
    	return ETrue;
    	}
    }

// -----------------------------------------------------------------------------
// CLocRequestorUtilsResolver::RequestorIdValid
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CLocRequestorUtilsResolver::RequestorIdValid(
    const CPosRequestor& aRequestor )
    {
    TBool rc(ETrue);
    if ( aRequestor.RequestorIdFormat() == CPosRequestor::EIdFormatUnknown )
        {
        rc = EFalse;
        }
    return rc;
    }


// -----------------------------------------------------------------------------
// CLocRequestorUtilsResolver::RequestorNameL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C HBufC* CLocRequestorUtilsResolver::RequestorNameL(
    const CPosRequestor& aRequestor )
    {
	return aRequestor.RequestorIdString().AllocL();
    }


// ---------------------------------------------------------------------------
// This is the callback method triggered when contacts matching succeeds.
// ---------------------------------------------------------------------------
//
void CLocRequestorUtilsResolver::FindCompleteL(MVPbkContactLinkArray* aResults)
    {
    iCntLinkArray = aResults;
    // Set State to process the matched contacts
   	iState = KMatchedContacts;
    // Schedule to run the Active Object again.
    ScheduleAORun();
    }


// ---------------------------------------------------------------------------
// This is the callback method triggered when contacts matching fails
// ---------------------------------------------------------------------------
//
void CLocRequestorUtilsResolver::FindFailed(TInt /*aError*/)
    {
    // Search in one of the contact stores failed.
    // No operation to be done in this phase.
    // All actions are done after the find is completed fully in 
    // FindCompleteL
    }

// ---------------------------------------------------------------------------
// This is the callback method triggered when retrieving contact details
// from a Contact Link
// ---------------------------------------------------------------------------
//
void CLocRequestorUtilsResolver::VPbkSingleContactOperationComplete(
    MVPbkContactOperationBase& /*aOperation*/,
    MVPbkStoreContact* aContact)
    {
    iContact = aContact;
    // Set State to process the retrieved contacts Information.
    iState = KRetrievedContactInfo;
    // Schedule to run the Active Object again.
    ScheduleAORun();
    }


// ---------------------------------------------------------------------------
// This is the callback method triggered when there is an error in 
// retrieving contact details from a Contact Link
// ---------------------------------------------------------------------------
//
void CLocRequestorUtilsResolver::VPbkSingleContactOperationFailed(
    MVPbkContactOperationBase& /*aOperation*/, 
    TInt /*aError*/)
    {
    // In this case we cannot get the Contact card name.
    // Hence the requestor detail cannot be updated.
    // So let us move to the next state.
    // The state machine takes care of *not* updating the requestor
    // information.
    // Set State to process the retrieved contacts Information.
    iState = KRetrievedContactInfo;
    // Schedule to run the Active Object again.
    ScheduleAORun();
    }


// ---------------------------------------------------------------------------
// This is the callback method triggered when closing / opening the
// contacts stores.
// ---------------------------------------------------------------------------
//
void CLocRequestorUtilsResolver::OpenComplete()
    {
    // Set State to Start Requestor resolving.
    iState = KResolveRequestor;
    // Schedule to run the Active Object again.
    ScheduleAORun();
    }


// ---------------------------------------------------------------------------
// This is the callback method triggered when closing / opening one
// contact store.
// ---------------------------------------------------------------------------
//
void CLocRequestorUtilsResolver::StoreReady(
    MVPbkContactStore& /*aContactStore*/)
    {
    //Nothing to do here
    }


// ---------------------------------------------------------------------------
// This is the callback method triggered when an opened contact store becomes
// unavailable.
// ---------------------------------------------------------------------------
//
void CLocRequestorUtilsResolver::StoreUnavailable(
    MVPbkContactStore& /*aContactStore*/,
    TInt /*aReason*/)
    {
    //Nothing to do here
    }


// ---------------------------------------------------------------------------
// This is the callback method triggered when any event happens with the
// opened contact stores.
// ---------------------------------------------------------------------------
//
void CLocRequestorUtilsResolver::HandleStoreEventL(
    MVPbkContactStore& /*aContactStore*/, 
    TVPbkContactStoreEvent /*aStoreEvent*/)
    {
    // We don't worry about contact , group events with any contact store.
    // Also backup / restore is taken care of in the verifier dialogs and
    // so we don't need to worry about backup / restore events with this store
    // as well.
    // Hence no processing is needed here.
    // Just overriden since the base class method is pure virtual.
    }


// -----------------------------------------------------------------------------
// CLocRequestorUtilsResolver::RunL
// RunL runs the full state machine operations
// -----------------------------------------------------------------------------
//
void CLocRequestorUtilsResolver::RunL()
    {
    // No need to check iStatus since this is a dummy Active Object and the
    // asynchronous operation was completed locally.
    switch( iState )
        {
        case KNoProcessing:
            {
            MVPbkContactStoreList& cntStoreList = 
                            iPbkContactEngine->ContactStoresL();
            cntStoreList.OpenAllL(*this);
            }
            break;

        case KResolveRequestor:
            {
            TInt count = iRequestors->Count();
            if ( iCount < count )
                {
                // Till there are more requestors to resolve continue this
                // operation.
               	ResolveRequestorNameL( *(*iRequestors)[iCount] );
                }
            else
                {
                // Set State to complete requestors resolving process.
                iState = KResolvingCompleted;
                ScheduleAORun();
                }
            }
            break;

        case KMatchedContacts:
            {
            // Once the Find operation is completed the control reaches here
            // and the result is stored in iCntLinkArray
            if ( iCntLinkArray && iCntLinkArray->Count() == 1 )
                {
                delete iCntsOperation;
                iCntsOperation = NULL;

                SetContactNameToRequestorL(0);
                }
            else
                {
                delete iCntsOperation;
                delete iCntLinkArray;

                iCntLinkArray = NULL;
                iCntsOperation = NULL;

                // There is no match or more than 1 match in contact stores.
                // So no more operation to be done on this requestor.
                // Move onto the next requestor and start resolving process again.
                iCount++;
                // Set State to Start Requestor resolving.
            	iState = KResolveRequestor;
                // Schedule to run the Active Object again.
            	ScheduleAORun();
                }
            }
            break;
            
        case KRetrievedContactInfo:
            {
            if ( iContact )
                {
                HBufC* name = 
                    iPbkCntFormatter->GetContactTitleOrNullL(
                        iContact->Fields(),
                        MPbk2ContactNameFormatter::EUseSeparator);
                if ( name )
                    {
                	CleanupStack::PushL(name);
                	// Set the requestor Type as Generic Name otherwise clipping
                	// would be done as if this is a number.
                	// Number clipping is done from beginning while text clipping
                	// is done from the end. Hence this is not trivial.
                	(*iRequestors)[iCount]->SetRequestorIdL( 
                	    CPosRequestor::EIdFormatGenericName,
                		*name );
                	CleanupStack::PopAndDestroy( name );
                    }
                }

            // Delete all information related to the current requestor resolving 
            // operation

            delete iContact;
            delete iCntLinkArray;
            delete iCntsOperation;

            iContact = NULL;
            iCntLinkArray = NULL;
            iCntsOperation = NULL;

            // Start next requestor resolving operation.
            iCount++;
            iState = KResolveRequestor;
            ScheduleAORun();
            }
            break;
            
        case KResolvingCompleted:
            {
            // All the handles regarding the requestor resolving namely iContact,
            // iCntLinkArray and iCntsOperation have already been cleaned up.
            // The only remaining item is the Contact Sotre handle.
            MVPbkContactStoreList& cntStoreList = 
                            iPbkContactEngine->ContactStoresL();
            // Although CloseAll takes a handle to the observer of type 
            // MVPbkContactStoreListObserver does not provide any callback mechanism.
            // It merely uses it to match in the observer array and remove it from the
            // array.
            cntStoreList.CloseAll(*this);
            CompleteRequest();
            }
            break;
        }

    }


// -----------------------------------------------------------------------------
// CLocRequestorUtilsResolver::RunError
// Handle RunL error cases.
// -----------------------------------------------------------------------------
//
TInt CLocRequestorUtilsResolver::RunError(TInt /*aError*/)
    {
    switch ( iState )
        {
        case KNoProcessing:
            // Leave occured while opening Contact Stores. Just complete the
            // resolving request.
            CompleteRequest();
            break;
        case KResolveRequestor:
            // ResolvRequestorNameL caused the leave. So current requestor 
            // cannot be resolved. 
        case KMatchedContacts:
            // RetrieveContactL caused a leave. Contact Name cannot be retrieved
        case KRetrievedContactInfo:
            // Settings the Requestor Id name failed.
            delete iContact;
            delete iCntLinkArray;
            delete iCntsOperation;

            iContact = NULL;
            iCntLinkArray = NULL;
            iCntsOperation = NULL;
            //So just set requestor count to next and trigger
            // the AO in the same state.
            iCount++;
            iState = KResolveRequestor;
            ScheduleAORun();
            break;
        case KResolvingCompleted:
            // Leave occured while opening Contact Stores. Just complete the
            // resolving request.
            CompleteRequest();
            break;        
        }
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CLocRequestorUtilsResolver::DoCancel
// Handle Cancel operation.
// -----------------------------------------------------------------------------
//
void CLocRequestorUtilsResolver::DoCancel()
    {
    //Nothing to be done here.
    }

// ---------------------------------------------------------------------------
// CLocRequestorUtilsResolver::CLocRequestorUtilsResolver
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CLocRequestorUtilsResolver::CLocRequestorUtilsResolver()
    :CActive(EPriorityStandard)
	{
	CActiveScheduler::Add(this);
	}

// ---------------------------------------------------------------------------
// CLocRequestorUtilsResolver::ConstructL
// Symbian Second Phase COnstructor
// ---------------------------------------------------------------------------
//
void CLocRequestorUtilsResolver::ConstructL()
	{
    iEnv = CEikonEnv::Static();
    
    // Retrieve the URI list of contact stores to be used for searching
    CPbk2StoreConfiguration* cntStoreCfg = CPbk2StoreConfiguration::NewL();
    CleanupStack::PushL(cntStoreCfg);

    CVPbkContactStoreUriArray* uriArray = 
        cntStoreCfg->SearchStoreConfigurationL();

    CleanupStack::PushL(uriArray);

    // Create the Virtual Phonebook Contact Manager class.
    // This class provides Search functionality.
	iPbkContactEngine = CVPbkContactManager::NewL(*uriArray);

    // Contact Store Configuration and URI Array classes are not
    // needed anymore.
	CleanupStack::PopAndDestroy(2, cntStoreCfg);

    // The Sort Manager is a parameter to the Contact Formatter Constructor
    iPbkSortManager = 
        CPbk2SortOrderManager::NewL(iPbkContactEngine->FieldTypes());
    
    // The Phonebook Formatter is a paramter to the Search method in the
    // contact manager.
    iPbkCntFormatter = 
        Pbk2ContactNameFormatterFactory::CreateL(
            iPbkContactEngine->FieldTypes(),
            *iPbkSortManager);

    TFileName* resourceFile = new( ELeave ) TFileName;
    CleanupStack::PushL( resourceFile );

    resourceFile->Append( KLocUtilsRscFile );

    TFileName* dllDrive = new( ELeave ) TFileName;
    CleanupStack::PushL( dllDrive );
    Dll::FileName( *dllDrive );    

    LocFileUtils::GetFileWithCorrectDriveL( *dllDrive, *resourceFile );
    iResourceOffset = iEnv->AddResourceFileL( *resourceFile );
    
    CleanupStack::PopAndDestroy( 2, resourceFile );
	}


// ---------------------------------------------------------------------------
// CLocRequestorUtilsResolver::ResolveRequestorNameL
// ---------------------------------------------------------------------------
//
void CLocRequestorUtilsResolver::ResolveRequestorNameL(
	CPosRequestor& aRequestor )
	{
    // There are 2 types of Requestors
    // 1. Contact
    // 2. Service and
    // There are 9 types of Requestor id Formats
    // 1. Generic Name
    // 2. Phone Number
    // 3. URL
    // 4. E-mail
    // 5. SIP URL
    // 6. IMS Public Identity
    // 7. MIN
    // 8. MDN
    // 9. Unknown
    // The following are the different cases for resolving.

    switch(aRequestor.RequestorIdFormat())
    	{
    	case CPosRequestor::EIdFormatGenericName:
			// The requestor is identified by name.
			// Hence there is no need to change the requestor String.
			// So complete the resolving operation for this requestor.
			iState = KResolveRequestor;
			// Increment the count of resolved requestors.
			iCount++;
			ScheduleAORun();
    		break;

    	case CPosRequestor::EIdFormatPhoneNumber:
    		ResolveRequestorByPhoneNumberL(aRequestor);
    		break;

    	case CPosRequestor::EIdFormatUrl:
    	    {
			// The requestor is identified by URL.
			CVPbkFieldTypeRefsList* fieldTypeList = 
			                        CVPbkFieldTypeRefsList::NewL();
			CleanupStack::PushL(fieldTypeList);

            const MVPbkFieldTypeList& fieldTypes =
                iPbkContactEngine->FieldTypes();

            // create last name field to the contact
            TResourceReader reader;
            iEnv->CreateResourceReaderLC(reader,
                R_VPBK_URL_SELECTOR);
            CVPbkFieldTypeSelector* urlSelector = 
                CVPbkFieldTypeSelector::NewL(reader, fieldTypes);
            CleanupStack::PushL(urlSelector);

            for (TInt i = 0; i < fieldTypes.FieldTypeCount(); ++i)
                {
                const MVPbkFieldType& fieldType = fieldTypes.FieldTypeAt(i);
                if (urlSelector->IsFieldTypeIncluded(fieldType))
                    {
        			fieldTypeList->AppendL(fieldType);
                    break;
                    }
                }

            // urlSelector, R_VPBK_URL_SELECTOR buffer 
            CleanupStack::PopAndDestroy(2); 

			ResolveRequestorByOtherFieldsL( aRequestor, fieldTypeList );
			CleanupStack::PopAndDestroy(fieldTypeList);
    	    }
    		break;

    	case CPosRequestor::EIdFormatEmail:
            {
			CVPbkFieldTypeRefsList* fieldTypeList = 
			                        CVPbkFieldTypeRefsList::NewL();
			CleanupStack::PushL(fieldTypeList);

            const MVPbkFieldTypeList& fieldTypes =
                iPbkContactEngine->FieldTypes();

            // create last name field to the contact
            TResourceReader reader;
            iEnv->CreateResourceReaderLC(reader,
                R_VPBK_EMAIL_SELECTOR);
            CVPbkFieldTypeSelector* emailSelector = 
                CVPbkFieldTypeSelector::NewL(reader, fieldTypes);
            CleanupStack::PushL(emailSelector);

            for (TInt i = 0; i < fieldTypes.FieldTypeCount(); ++i)
                {
                const MVPbkFieldType& fieldType = fieldTypes.FieldTypeAt(i);
                if (emailSelector->IsFieldTypeIncluded(fieldType))
                    {
        			fieldTypeList->AppendL(fieldType);
                    break;
                    }
                }

            // emailSelector, R_VPBK_EMAIL_SELECTOR buffer 
            CleanupStack::PopAndDestroy(2); 

			ResolveRequestorByOtherFieldsL( aRequestor, fieldTypeList );
			CleanupStack::PopAndDestroy(fieldTypeList);
    	    }
    		break;

    	case CPosRequestor::EIdFormatSIPUrl:
    	    {
			CVPbkFieldTypeRefsList* fieldTypeList = 
			                        CVPbkFieldTypeRefsList::NewL();
			CleanupStack::PushL(fieldTypeList);

            const MVPbkFieldTypeList& fieldTypes =
                iPbkContactEngine->FieldTypes();

            // create last name field to the contact
            TResourceReader reader;
            iEnv->CreateResourceReaderLC(reader,
                R_VPBK_SIPURL_SELECTOR);
            CVPbkFieldTypeSelector* sipUrlSelector = 
                CVPbkFieldTypeSelector::NewL(reader, fieldTypes);
            CleanupStack::PushL(sipUrlSelector);

            for (TInt i = 0; i < fieldTypes.FieldTypeCount(); ++i)
                {
                const MVPbkFieldType& fieldType = fieldTypes.FieldTypeAt(i);
                if (sipUrlSelector->IsFieldTypeIncluded(fieldType))
                    {
        			fieldTypeList->AppendL(fieldType);
                    break;
                    }
                }

            // sipUrlSelector, R_VPBK_SIPURL_SELECTOR buffer 
            CleanupStack::PopAndDestroy(2); 

			ResolveRequestorByOtherFieldsL( aRequestor, fieldTypeList );
			CleanupStack::PopAndDestroy(fieldTypeList);
    	    }
    		break;

    	case CPosRequestor::EIdFormatIMSPublicIdentity:
    	case CPosRequestor::EIdFormatMIN:
    	case CPosRequestor::EIdFormatMDN:
    	// In this case we match by any field in the Contacts
    	    {
			const MVPbkFieldTypeList& gblFieldTypeList = 
			                        iPbkContactEngine->FieldTypes();
			
			ResolveRequestorByOtherFieldsL( aRequestor, 
			    const_cast<MVPbkFieldTypeList *>(&gblFieldTypeList) );
    	    }
    		break;
    	
    	default:
			// If the requestor format is unknown then it won't be resolved.
			// This case is for any future additions to the format types.
    		// Nothing to do here as of now.
			// So complete the resolving operation for this requestor.
			iState = KResolveRequestor;
			// Increment the count of resolved requestors.
			iCount++;
			ScheduleAORun();
    		break;
    	}
	}


// ---------------------------------------------------------------------------
// CLocRequestorUtilsResolver::ResolveRequestorByPhoneNumberL
// ---------------------------------------------------------------------------
//
void CLocRequestorUtilsResolver::ResolveRequestorByPhoneNumberL(
	CPosRequestor& aRequestor )
	{
	
	// The requestor is identified by Phone Number.
    // Use the MatchPhoneNumberL of CPbkPhoneEngine
	iCntsOperation = iPbkContactEngine->MatchPhoneNumberL(
                            				aRequestor.RequestorIdString(),
                            				KMaxPhoneMatchLength,
                            				*this);
	}

	
// ---------------------------------------------------------------------------
// CLocRequestorUtilsResolver::ResolveRequestorByOtherFieldsL
// Searches the Contacts Database Synchronously using the field and
// information specified in the requestor.
// ---------------------------------------------------------------------------
//
void CLocRequestorUtilsResolver::ResolveRequestorByOtherFieldsL(
	CPosRequestor& aRequestor,
	MVPbkFieldTypeList* aFieldTypeList)
	{
    // If the format is URL or E-mail etc then use FindL of CVPbkContactManager
	iCntsOperation = iPbkContactEngine->FindL(aRequestor.RequestorIdString(),
				                              *aFieldTypeList,
				                              *this);
	}


// ---------------------------------------------------------------------------
// CLocRequestorUtilsResolver::SetContactNameToRequestor
// ---------------------------------------------------------------------------
//

void CLocRequestorUtilsResolver::SetContactNameToRequestorL(
	TInt aCntLinkArrayIndex)
	{
    iCntsOperation = iPbkContactEngine->RetrieveContactL(
                        iCntLinkArray->At(aCntLinkArrayIndex),
                        *this);
	}


// ---------------------------------------------------------------------------
// CLocRequestorUtilsResolver::ScheduleAORun
// Schedule to Run this dummy Active Object once again.
// ---------------------------------------------------------------------------
//
void CLocRequestorUtilsResolver::ScheduleAORun()
    {
    // This results in the RunL getting called.
    if ( !IsActive() )
        {
        SetActive();
        TRequestStatus* status = &iStatus;
        User::RequestComplete(status, KErrNone);
        }
    }


// ---------------------------------------------------------------------------
// CLocRequestorUtilsResolver::ScheduleAORun
// Schedule to Run this dummy Active Object once again.
// ---------------------------------------------------------------------------
//
void CLocRequestorUtilsResolver::CompleteRequest()
    {
    // Stop the ActiveScheduler Wait Loop to complete the requestor resolving
    // operation.
	iWait.AsyncStop();
    }


//  End of File
