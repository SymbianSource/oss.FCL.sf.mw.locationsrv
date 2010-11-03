/*
 * Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
 * Description:  Implements the CLocSUPLServerEditor class
 *
 */

#include <aknappui.h>
#include <AknIconArray.h>       // Icon Array
#include <aknlocationed.h>
#include <aknlistquerydialog.h>
#include <AknPopupFieldText.h> 
#include <aknnotewrappers.h>
#include <AknsUtils.h>
#include <AknsSkinInstance.h>
#include <aknselectionlist.h>
#include <akntitle.h>       	// CAknTitlePane
#include <AknWaitDialog.h> 
#include <aknuniteditor.h> 
#include <avkon.rsg>
#include <avkon.hrh>
#include <e32math.h>
#include <eikapp.h>
#include <eikaufty.h>
#include <eikcapc.h>
#include <eikedwin.h>
#include <eikenv.h>
#include <eikmenup.h>
#include <eikmenub.h>
#include <eikserverapp.h>
#include <eikspane.h>       	// Status pane
#include <barsread.h>           // For TResourceReader
#include <bautils.h> 
#include <data_caging_path_literals.hrh>
#include <gulicon.h>            // Gul Icon
#include <StringLoader.h>
#include <hlplch.h> 

#include <epos_csuplsettingsconstants.h>
#include <epos_csuplsettingparams.h>

#include <locsuplsettingsui.rsg>
#include "locsuplsettings.hrh"
#include "locsuplservereditor.h"
#include "locsuplsettingsuiengine.h"
#include "locsupldebug.h"

// ============================ LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CLocSUPLServerEditor::CLocSUPLServerEditor()
// -----------------------------------------------------------------------------
//
CLocSUPLServerEditor::CLocSUPLServerEditor(TBool aIsEditable,
        CLocSUPLSettingsUiEngine& aEngine, TInt64 aSlpId) :
    iIsEditMode(aIsEditable), iEngine(aEngine), iSlpId(aSlpId),
            iIsAccessPointDefined(EFalse)
    {
    if (aIsEditable)
        {
        iIsNewServer = ETrue;
        iSlpId = NULL;
        }
    }

// -----------------------------------------------------------------------------
// CLocSUPLServerEditor::ConstructL()
// -----------------------------------------------------------------------------
//
void CLocSUPLServerEditor::ConstructL()
    {
    DEBUG(+CLocSUPLServerEditor::ConstructL);
    CAknForm::ConstructL( R_SUPLSERVER_EDITOR_MENU_BAR);

    // Save original title
    CEikStatusPane* statusPane = iAvkonAppUi->StatusPane();
    iTitlePane = (CAknTitlePane*) statusPane->ControlL(TUid::Uid(
            EEikStatusPaneUidTitle));
    iOriginalTitle = iTitlePane->Text()->AllocL();

    // Allocate memory for Server address buffer & Access point buffer
    iServerAddress = HBufC16::NewL(KMaxHSLPAddrLen);
    iIap = HBufC16::NewL(KMaxIAPLen);

    DEBUG(-CLocSUPLServerEditor::ConstructL);
    }

// -----------------------------------------------------------------------------
// CLocSUPLServerEditor::NewL()
// -----------------------------------------------------------------------------
//
CLocSUPLServerEditor* CLocSUPLServerEditor::NewL(TBool aIsEditable,
        CLocSUPLSettingsUiEngine& aEngine, TInt64 aSlpId)
    {
    CLocSUPLServerEditor* self = NewLC(aIsEditable, aEngine, aSlpId);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CLocSUPLServerEditor::NewLC()
// -----------------------------------------------------------------------------
//
CLocSUPLServerEditor* CLocSUPLServerEditor::NewLC(TBool aIsEditable,
        CLocSUPLSettingsUiEngine& aEngine, TInt64 aSlpId)
    {
    CLocSUPLServerEditor* self = new (ELeave) CLocSUPLServerEditor(
            aIsEditable, aEngine, aSlpId);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CLocSUPLServerEditor::~CLocSUPLServerEditor()
// -----------------------------------------------------------------------------
//
CLocSUPLServerEditor::~CLocSUPLServerEditor()
    {
    DEBUG(+CLocSUPLServerEditor::~CLocSUPLServerEditor);
    // Restore title pane. Ownership of iTitlePane transferred.
    if (iTitlePane && iOriginalTitle)
        {
        iTitlePane->SetText(iOriginalTitle);
        }
    delete iServerAddress;
    iServerAddress = NULL;

    delete iIap;
    iIap = NULL;
    DEBUG(-CLocSUPLServerEditor::~CLocSUPLServerEditor);
    }

// -----------------------------------------------------------------------------
// CLocSUPLServerEditor::DynInitMenuPaneL()
// -----------------------------------------------------------------------------
//
void CLocSUPLServerEditor::DynInitMenuPaneL(TInt aResourceId,
        CEikMenuPane* aMenuPane)
    {
    DEBUG(+CLocSUPLServerEditor::DynInitMenuPaneL);
    CAknForm::DynInitMenuPaneL(aResourceId, aMenuPane);

    if (aResourceId == R_AVKON_FORM_MENUPANE)
        {
        // We do not want to allow the user to modify our Form therefore
        // Disable the Label, Add, Delete, and Save Field
        aMenuPane->SetItemDimmed(EAknFormCmdLabel, ETrue);
        aMenuPane->SetItemDimmed(EAknFormCmdAdd, ETrue);
        aMenuPane->SetItemDimmed(EAknFormCmdDelete, ETrue);
        aMenuPane->SetItemDimmed(EAknFormCmdSave, ETrue);

        if (IsEditable())
            {
            aMenuPane->SetItemDimmed(EAknFormCmdEdit, ETrue);
            aMenuPane->SetItemDimmed(ELocServerEditorDisable, ETrue);
            aMenuPane->SetItemDimmed(ELocServerEditorEnable, ETrue);
            aMenuPane->SetItemDimmed(ELocServerEditorDelete, ETrue);

            switch (IdOfFocusControl())
                {
                case ELocSuplServerIdServerAddr:
                    {
                    aMenuPane->SetItemDimmed(ELocServerEditorDefine, ETrue);
                    break;
                    }
                case ELocSuplServerIdAccessPoint:
                    {
                    break;
                    }
                case ELocSuplServerIdUsageInHomeNw:
                    {
                    aMenuPane->SetItemDimmed(ELocServerEditorDefine, ETrue);

                    TBool editableFlag = ETrue;
                    if (iSlpId)
                        TRAP_IGNORE(iEngine.GetEditableFlagL(iSlpId,
                                editableFlag));

                    break;
                    }
                default:
                    break;
                }
            }
        else
            {
            TBool enabledFlag;
            TRAP_IGNORE(iEngine.GetServerEnabledFlagL(iSlpId, enabledFlag));

            if (enabledFlag)
                aMenuPane->SetItemDimmed(ELocServerEditorEnable, ETrue);
            else
                aMenuPane->SetItemDimmed(ELocServerEditorDisable, ETrue);

            TBool editableFlag = ETrue;
            if (iSlpId)
                TRAP_IGNORE(iEngine.GetEditableFlagL(iSlpId, editableFlag));
            if (editableFlag == EFalse)
                {
                switch (IdOfFocusControl())
                    {
                    case ELocSuplServerIdServerAddr:
                    case ELocSuplServerIdUsageInHomeNw:
                        {
                        aMenuPane->SetItemDimmed(EAknFormCmdEdit, ETrue);
                        break;
                        }
                    default:
                        break;
                    }
                }

            aMenuPane->SetItemDimmed(ELocServerEditorDefine, ETrue);
            aMenuPane->SetItemDimmed(ELocServerEditorRemove, ETrue);
            }
        }
    DEBUG(-CLocSUPLServerEditor::DynInitMenuPaneL);
    }

// -----------------------------------------------------------------------------
// CLocSUPLServerEditor::ProcessCommandL()
// -----------------------------------------------------------------------------
//
void CLocSUPLServerEditor::ProcessCommandL(TInt aCommandId)
    {
    DEBUG1( + CLocSUPLServerEditor::ProcessCommandL aCommandId=%d, aCommandId );
    CAknForm::ProcessCommandL(aCommandId);

    switch (aCommandId)
        {
        case EAknFormCmdEdit:
            {
            iIsEditMode = ETrue;
            SetCursorToEndL();
            HandleMSKCaptionL();
            break;
            }
        case ELocServerEditorDelete:
        case ELocServerEditorRemove:
            {
            if (DeleteConfirmationQueryL())
                {
                TRAP_IGNORE(iEngine.RemoveServerL(iSlpId));
                // Return back to server view
                TryExitL( EAknSoftkeyBack);
                }
            break;
            }
        case ELocServerEditorDisable:
            {
            TRAP_IGNORE(iEngine.SetServerEnabledFlagL(iSlpId, EFalse));
            break;
            }
        case ELocServerEditorEnable:
            {
            TRAP_IGNORE(iEngine.SetServerEnabledFlagL(iSlpId, ETrue));
            break;
            }
        case ELocServerEditorMSKChange:
            {
            CEikEdwin* usageInHomeNWPopupFieldText = (CEikEdwin*) Control(
                    ELocSuplServerIdUsageInHomeNw);
            HBufC* string;
            string = StringLoader::LoadLC(R_LOC_SERVER_USAGEINHOMENETWORK_NO,
                    iCoeEnv);
            if (iIsNewServer)
                {
                TBuf<KMaxUsageTextSize> des;
                usageInHomeNWPopupFieldText->GetText(des);
                if (!des.CompareC(string->Des()))
                    {
                    SetUsageinHomeNetworkL( ETrue);
                    }
                else
                    {
                    SetUsageinHomeNetworkL( EFalse);
                    }
                }
            else
                {
                TBool editableFlag;
                TRAP_IGNORE(iEngine.GetEditableFlagL(iSlpId, editableFlag));
                if (editableFlag)
                    {
                    TBuf<KMaxUsageTextSize> des;
                    usageInHomeNWPopupFieldText->GetText(des);
                    if (!des.CompareC(string->Des()))
                        {
                        SetUsageinHomeNetworkL( ETrue);
                        }
                    else
                        {
                        SetUsageinHomeNetworkL( EFalse);
                        }
                    }
                CleanupStack::PopAndDestroy(string);
                }
            break;
            }
        case EAknCmdHelp:
            {
            HlpLauncher::LaunchHelpApplicationL(iCoeEnv->WsSession(),
                    iAvkonAppUi->AppHelpContextL());
            break;
            }
        case EEikCmdExit:
            {
            // The Exit is handled by the Application UI
            TryExitL( EEikCmdExit);
            break;
            }
        default:
            break;
        }
    DEBUG(-CLocSUPLServerEditor::ProcessCommandL);
    }

// ---------------------------------------------------------------------------
// Derived from CEikDialog
// ---------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// CLocSUPLServerEditor::OfferKeyEventL
// Receives and handles key events
// -----------------------------------------------------------------------------
//
TKeyResponse CLocSUPLServerEditor::OfferKeyEventL(const TKeyEvent& aKeyEvent,
        TEventCode aType)
    {
    DEBUG(CLocSUPLServerEditor::OfferKeyEventL);
    if (aType == EEventKey)
        {
        if (aKeyEvent.iCode == EKeyBackspace && IsEditable() == EFalse)
            {
            ProcessCommandL( ELocServerEditorDelete);
            return EKeyWasConsumed;
            }

        if (!iIsNewServer && IsEditable())
            {
            // check if current opened server is editable
            // if its not editable then return with no action on it.			
            TBool editableFlag = ETrue;
            if (iSlpId)
                TRAP_IGNORE(iEngine.GetEditableFlagL(iSlpId, editableFlag));

            if (editableFlag == EFalse)
                {
                if ((aKeyEvent.iScanCode != EStdKeyUpArrow
                        || aKeyEvent.iScanCode != EStdKeyDownArrow) == EFalse)
                    {
                    switch (IdOfFocusControl())
                        {
                        case ELocSuplServerIdServerAddr:
                        case ELocSuplServerIdUsageInHomeNw:
                            {
                            return EKeyWasConsumed;
                            }
                        default:
                            break;
                        }
                    }
                }
            }
        //ou1cimx1#613257. changes made for form to go to edit mode by press of OK/Enter Key from physical keyboard
        if (aKeyEvent.iCode == EKeyOK || aKeyEvent.iCode == EKeyEnter
                || aKeyEvent.iScanCode == EStdKeyEnter)
            {
            TInt focusControl(IdOfFocusControl());
            if (!IsEditable())
                {
                if (focusControl == ELocSuplServerIdUsageInHomeNw
                        || focusControl == ELocSuplServerIdServerAddr)
                    {
                    ProcessCommandL( EAknFormCmdEdit);
                    return EKeyWasConsumed;
                    }
                }
            }
        }
    TKeyResponse response = CAknForm::OfferKeyEventL(aKeyEvent, aType);
    return response;
    }

// ---------------------------------------------------------------------------
// CLocSUPLServerEditor ::OkToExitL()
// ---------------------------------------------------------------------------
//
TBool CLocSUPLServerEditor::OkToExitL(TInt aButtonId)
    {
    DEBUG1( + CLocSUPLServerEditor::OkToExitL aButtonId=%d, aButtonId );
    TBool retVal = ETrue;

    switch (aButtonId)
        {
        case EAknFormCmdEdit: // when MSK is pressed
            {
            //Handle MSK operation based on focused line.            
            if (IsEditable() == EFalse)
                {
                ProcessCommandL( EAknFormCmdEdit);
                HandleMSKCaptionL();
                }
            //dont close editor            
            retVal = EFalse;
            break;
            }
        case ELocServerEditorNoCmd:
            {
            //dont close editor            
            retVal = EFalse;
            break;
            }
        case ELocServerEditorMSKChange:
            {
            ProcessCommandL( ELocServerEditorMSKChange);
            //dont close editor            
            retVal = EFalse;
            break;
            }
        case EAknSoftkeyOptions: //when LSK is pressed
            {
            //display menu specific to editor
            DisplayMenuL();
            //don't close editor
            retVal = EFalse;
            break;
            }
        case EAknSoftkeyBack:
            {
            break;
            }
        case EAknSoftkeyDone: // when RSK is pressed in edit mode
            {
            if (SaveFormDataL())
                {
                if (iIsNewServer)
                    {
                    // iap name is always set as KNullDesC since defining the access point is 
                    // not allowed while creating a new server manually
                    TRAPD(err, iEngine.AddNewServerL(iServerAddress->Des(),
                            KNullDesC, iUsageInHomeNw));
                    if (err == KErrNone)
                        {
                        iIsNewServer = EFalse;
                        }
                    else if (err == KErrAlreadyExists)
                        {
                        ShowServerValidationInfoNoteL
                                ( R_LOC_SERVER_ALREADY_EXIST_INFO_NOTE);
                        iIsEditMode = ETrue;
                        SetEditableL( iIsEditMode);
                        SetInitialCurrentLine();
                        SetCursorToEndL();
                        //dont close editor            
                        retVal = EFalse;
                        }
                    else if (err == KErrArgument)
                        {
                        ShowServerValidationInfoNoteL
                                ( R_LOC_SERVER_INFO_NOTE);
                        iIsEditMode = ETrue;
                        SetEditableL( iIsEditMode);
                        SetInitialCurrentLine();
                        SetCursorToEndL();
                        //dont close editor            
                        retVal = EFalse;
                        }
                    }
                else
                    { // save the existing slp entry info
                    if (iSlpId)
                        {
                        TRAPD(err, iEngine.SetServerAddressL(iSlpId,
                                iServerAddress->Des()));
                        if (err == KErrNone)
                            {
                            if (iIap->Length() > 0)
                                iEngine.SetIapNameL(iSlpId, iIap->Des());
                            iEngine.SetUsageInHomwNwFlagL(iSlpId,
                                    iUsageInHomeNw);

                            iIsEditMode = EFalse;
                            SetEditableL( iIsEditMode);
                            }
                        else if (err == KErrAlreadyExists)
                            {
                            ShowServerValidationInfoNoteL
                                    ( R_LOC_SERVER_ALREADY_EXIST_INFO_NOTE);
                            iIsEditMode = ETrue;
                            SetEditableL( iIsEditMode);
                            SetInitialCurrentLine();
                            SetCursorToEndL();
                            }
                        else if (err == KErrArgument)
                            {
                            ShowServerValidationInfoNoteL
                                    ( R_LOC_SERVER_INFO_NOTE);
                            iIsEditMode = ETrue;
                            SetEditableL( iIsEditMode);
                            SetInitialCurrentLine();
                            SetCursorToEndL();
                            //dont close editor            
                            retVal = EFalse;
                            }
                        }
                    HandleMSKCaptionL();
                    //dont close editor            
                    retVal = EFalse;
                    }
                }
            else
                {
                //if control's state is changed and manadatory contain is not added then show the popup message
                if (iIsModified)
                    {
                    ShowServerValidationInfoNoteL( R_LOC_SERVER_INFO_NOTE);
                    iIsEditMode = ETrue;
                    SetEditableL( iIsEditMode);
                    SetInitialCurrentLine();
                    SetCursorToEndL();
                    //dont close editor            
                    retVal = EFalse;
                    }
                else
                    {
                    retVal = ETrue;
                    }
                }
            break;
            }
        default: // close editor in all other cases
            {
            break;
            }
        }
    DEBUG1( - CLocSUPLServerEditor::OkToExitL retVal=%d, retVal );
    return retVal;
    }

// -----------------------------------------------------------------------------
// CLocSUPLServerEditor::SaveFormDataL()
// Deals with the occasion of a change from edit->view mode and the user wishes 
// to save the changes.
// -----------------------------------------------------------------------------
//
TBool CLocSUPLServerEditor::SaveFormDataL()
    {
    DEBUG(+CLocSUPLServerEditor::SaveFormDataL);
    TBool retVal = EFalse;

    //Retrieve server address text 
    CEikEdwin* serverAddress = static_cast<CEikEdwin*> (ControlOrNull(
            ELocSuplServerIdServerAddr));
    if (serverAddress)
        {
        HBufC* address = serverAddress->GetTextInHBufL();
        if (address)
            {
            iServerAddress->Des().Copy(address->Des());
            }
        else
            iServerAddress->Des().Copy(KNullDesC);
        delete address;
        }

    //Retrieve Access Point 
    CEikEdwin* accessPoint = static_cast<CEikEdwin*> (ControlOrNull(
            ELocSuplServerIdAccessPoint));
    if (accessPoint)
        {
        HBufC* apText = accessPoint->GetTextInHBufL();
        if (apText)
            {
            iIap->Des().Copy(apText->Des());
            }
        delete apText;

        HBufC* apString = StringLoader::LoadL(R_LOC_SERVER_NONE);
        if (apString->CompareC(iIap->Des()) == 0)
            {
            iIap->Des().Copy(KNullDesC);
            }
        delete apString;
        }

    //Retrieve Usage In Home Nw Field value
    CEikEdwin* usageInHomeNWPopupFieldText = (CEikEdwin*) Control(
            ELocSuplServerIdUsageInHomeNw);
    if (usageInHomeNWPopupFieldText)
        {
        TBuf<KMaxUsageTextSize> des;
        HBufC* string;
        string = StringLoader::LoadLC(R_LOC_SERVER_USAGEINHOMENETWORK_NO,
                iCoeEnv);
        usageInHomeNWPopupFieldText->GetText(des);
        if (!des.CompareC(string->Des()))
            {
            iUsageInHomeNw = EFalse;
            }
        else
            {
            iUsageInHomeNw = ETrue;
            }
        CleanupStack::PopAndDestroy(string);
        }

    if (iServerAddress->Length() > 0)
        {
        retVal = ETrue;
        }

    DEBUG(-CLocSUPLServerEditor::SaveFormDataL);
    return retVal;
    }

// -----------------------------------------------------------------------------
// LocSUPLServerEditor::PreLayoutDynInitL()
// Initialises the dialog's controls before the dialog is sized and layed out.
// -----------------------------------------------------------------------------
//
void CLocSUPLServerEditor::PreLayoutDynInitL()
    {
    DEBUG(+CLocSUPLServerEditor::PreLayoutDynInitL);
    CAknForm::PreLayoutDynInitL();

    DEBUG(-CLocSUPLServerEditor::PreLayoutDynInitL);
    }

// -----------------------------------------------------------------------------
// CLocSUPLServerEditor::PostLayoutDynInitL
// Initialises the dialog's controls after the dialog
// has been sized but before it has been activated.
// -----------------------------------------------------------------------------
//
void CLocSUPLServerEditor::PostLayoutDynInitL()
    {
    DEBUG(+CLocSUPLServerEditor::PostLayoutDynInitL);
    CAknForm::PostLayoutDynInitL();
    SetEditableL( iIsEditMode);
    LoadFormValuesFromDataL();

    CEikButtonGroupContainer* cba = CEikButtonGroupContainer::Current();
    CleanupStack::PushL(cba);
    if (IsEditable())
        {
        cba->SetCommandSetL(R_SUPLSERVER_EDITOR_OPTIONS_NOMSK_DONE);
        }
    else
        {
        TBool editableFlag = ETrue;
        if (iSlpId)
            TRAP_IGNORE(iEngine.GetEditableFlagL(iSlpId, editableFlag));

        if (editableFlag)
            cba->SetCommandSetL(R_SUPLSERVER_EDITOR_OPTIONS_EDIT_BACK);
        else
            cba->SetCommandSetL(R_SUPLSERVER_EDITOR_OPTIONS_NOMSK_BACK);
        }
    CleanupStack::Pop(cba);

    DEBUG(-CLocSUPLServerEditor::PostLayoutDynInitL);
    }

// -----------------------------------------------------------------------------
// CLocSUPLServerEditor::LineChangedL()
// -----------------------------------------------------------------------------
//
void CLocSUPLServerEditor::LineChangedL(TInt /*aControlId*/)
    {
    HandleMSKCaptionL();
    }

// -----------------------------------------------------------------------------
// CLocSUPLServerEditor::SetInitialCurrentLine()
// Selects desired line on startup
// -----------------------------------------------------------------------------
//
void CLocSUPLServerEditor::SetInitialCurrentLine()
    {
    CAknForm::SetInitialCurrentLine();

    // set focus to server address field
    if (ControlOrNull(ELocSuplServerIdServerAddr))
        {
        TRAP_IGNORE(TryChangeFocusToL(ELocSuplServerIdServerAddr));
        }
    }

// -----------------------------------------------------------------------------
// CLocSUPLServerEditor::HandleResourceChange()
// Handles layout switch
// -----------------------------------------------------------------------------
//
void CLocSUPLServerEditor::HandleResourceChange(TInt aType)
    {
    if (aType == KEikDynamicLayoutVariantSwitch)
        {
        CAknForm::HandleResourceChange(aType);
        }
    }

// -----------------------------------------------------------------------------
// CLocSUPLServerEditor::ExecuteLD
// Executes the Dialog
// -----------------------------------------------------------------------------
//
TInt CLocSUPLServerEditor::ExecuteLD()
    {
    if (!iIsAccessPointDefined)
        {
        return CAknForm::ExecuteLD(R_SUPLSERVER_EDITOR_FORM_DIALOG_NOAP);
        }
    else
        {
        return CAknForm::ExecuteLD(R_SUPLSERVER_EDITOR_FORM_DIALOG);
        }
    }

// -----------------------------------------------------------------------------
// CLocSUPLServerEditor::HandleControlStateChangeL
// -----------------------------------------------------------------------------
//
void CLocSUPLServerEditor::HandleControlStateChangeL(TInt aControlId)
    {
    DEBUG(+CLocSUPLServerEditor::HandleControlStateChangeL);
    iIsModified = ETrue;
    switch (aControlId)
        {
        case ELocSuplServerIdServerAddr:
            {
            //Retrieve server address text 
            CEikEdwin* serverAddress =
                    static_cast<CEikEdwin*> (ControlOrNull(
                            ELocSuplServerIdServerAddr));
            if (serverAddress)
                {
                HBufC* address = serverAddress->GetTextInHBufL();
                if (address)
                    {
                    CleanupStack::PushL(address);
                    // update the title pane caption
                    ChangeTitlePaneTextL(*address);
                    CleanupStack::PopAndDestroy(address);
                    }
                else
                    {
                    // update the title pane caption
                    TBuf<1> nullDes;
                    nullDes.Copy(KNullDesC);
                    ChangeTitlePaneTextL(nullDes);
                    }
                }
            break;
            }
        default:
            break;
        }
    DEBUG(-CLocSUPLServerEditor::HandleControlStateChangeL);
    }

// ---------------------------------------------------------------------------
// CLocSUPLServerEditor ::LoadFormValuesFromDataL()
// Called when the form is  executed and when the user chooses to discard changes
// in QuerySaveChangesL (via  DoNotSaveFormDataL).
// ---------------------------------------------------------------------------
// 
void CLocSUPLServerEditor::LoadFormValuesFromDataL()
    {
    DEBUG(+CLocSUPLServerEditor::LoadFormValuesFromDataL);
    if (IsEditable() && iIsNewServer) //if create new server
        {

        CEikEdwin* usageInHNWPopupFieldText = (CEikEdwin*) Control(
                ELocSuplServerIdUsageInHomeNw);
        if (usageInHNWPopupFieldText)
            {
            HBufC* usageString = StringLoader::LoadL(
                    R_LOC_SERVER_USAGEINHOMENETWORK_YES);
            usageInHNWPopupFieldText->SetTextL(usageString);
            delete usageString;
            }
        usageInHNWPopupFieldText->DrawNow();

        // update the title pane caption
        HBufC* serverTitle = StringLoader::LoadL(R_LOC_SERVER_EDITOR_TITLE);
        if (serverTitle)
            {
            ChangeTitlePaneTextL(*serverTitle);
            }
        delete serverTitle;
        serverTitle = NULL;
        }
    else
        {
        CServerParams *params = CServerParams::NewL();
        CleanupStack::PushL(params);
        TRAP_IGNORE(iEngine.GetSlpInfoFromIdL(iSlpId, params));

        HBufC* hslpAddr = HBufC::NewL(KMaxHSLPAddrLen);
        HBufC* iapName = HBufC::NewL(KMaxIAPLen);
        TInt64 slpId;
        TBool enabledFlag, simChangeFlag, usageInHomeNwFlag, editFlag;

        TInt errParams = params->Get(slpId, hslpAddr->Des(), iapName->Des(),
                enabledFlag, simChangeFlag, usageInHomeNwFlag, editFlag);
        if (errParams != KErrNone)
            {
            // Error has occured 
            }

        CEikEdwin* serverAddress = static_cast<CEikEdwin*> (ControlOrNull(
                ELocSuplServerIdServerAddr));
        if (serverAddress)
            {
            serverAddress->SetTextL(hslpAddr);
            if (editFlag == EFalse)
                serverAddress->SetReadOnly(ETrue);
            serverAddress->DrawDeferred();
            iServerAddress->Des().Copy(hslpAddr->Des());
            }

        if (iIsAccessPointDefined)
            {
            CEikEdwin* apSelector = static_cast<CEikEdwin*> (ControlOrNull(
                    ELocSuplServerIdAccessPoint));
            if (apSelector)
                {
                apSelector->SetTextL(iapName);
                apSelector->DrawNow();
                iIap->Des().Copy(iapName->Des());
                }
            }

        CEikEdwin* usageInHNWPopupFieldText = (CEikEdwin*) Control(
                ELocSuplServerIdUsageInHomeNw);
        if (usageInHNWPopupFieldText)
            {
            if (!usageInHomeNwFlag)
                SetUsageinHomeNetworkL( EFalse);
            else
                SetUsageinHomeNetworkL( ETrue);

            UpdatePageL( ETrue);
            iUsageInHomeNw = usageInHomeNwFlag;
            }

        delete iapName;
        delete hslpAddr;
        CleanupStack::PopAndDestroy(params);

        // update the title pane caption
        ChangeTitlePaneTextL(*iServerAddress);

        }
    DEBUG(-CLocSUPLServerEditor::LoadFormValuesFromDataL);
    }

// ---------------------------------------------------------------------------
// CLocSUPLServerEditor::ChangeTitlePaneTextL()
// Set the Title Text
// ---------------------------------------------------------------------------
//
void CLocSUPLServerEditor::ChangeTitlePaneTextL(TDesC& aText)
    {
    DEBUG(+CLocSUPLServerEditor::ChangeTitlePaneTextL);
    // Obtain the title from the Status Pane
    CAknTitlePane
            * title =
                    static_cast<CAknTitlePane*> (iEikonEnv->AppUiFactory()-> StatusPane()->ControlL(
                            TUid::Uid(EEikStatusPaneUidTitle)));

    // Set the Title's buffer    
    HBufC* buf = aText.AllocL();
    title->SetText(buf); // Takes ownership of buf
    title->DrawNow();
    DEBUG(-CLocSUPLServerEditor::ChangeTitlePaneTextL);
    }

// -----------------------------------------------------------------------------
// CLocSUPLServerEditor::HandleMSKCaptionL()
// It changes caption of MSK key
// -----------------------------------------------------------------------------
//
void CLocSUPLServerEditor::HandleMSKCaptionL()
    {
    DEBUG(+CLocSUPLServerEditor::HandleMSKCaptionL);
    CEikButtonGroupContainer* cba = CEikButtonGroupContainer::Current();
    CleanupStack::PushL(cba);
    //attribute specific cba
    if (IsEditable())
        {
        switch (IdOfFocusControl())
            {
            case ELocSuplServerIdServerAddr:
                {
                cba->SetCommandSetL(R_SUPLSERVER_EDITOR_OPTIONS_NOMSK_DONE);
                break;
                }
            case ELocSuplServerIdAccessPoint:
                {
                break;
                }
            case ELocSuplServerIdUsageInHomeNw:
                {
                TBool editableFlag = ETrue;
                if (iSlpId)
                    {
                    TRAP_IGNORE(
                            iEngine.GetEditableFlagL(iSlpId, editableFlag));
                    }
                if (editableFlag)
                    cba->SetCommandSetL(
                            R_SUPLSERVER_EDITOR_OPTIONS_CHANGE_DONE);
                else
                    cba->SetCommandSetL(
                            R_SUPLSERVER_EDITOR_OPTIONS_NOMSK_DONE);
                break;
                }
            default:
                break;
            }
        }
    else
        {
        TBool editableFlag = ETrue;
        TBool changeMSK = ETrue;

        if (iSlpId)
            TRAP_IGNORE(iEngine.GetEditableFlagL(iSlpId, editableFlag));

        if (editableFlag == EFalse && (IdOfFocusControl()
                == ELocSuplServerIdServerAddr || IdOfFocusControl()
                == ELocSuplServerIdUsageInHomeNw))
            changeMSK = EFalse;

        if (changeMSK)
            cba->SetCommandSetL(R_SUPLSERVER_EDITOR_OPTIONS_EDIT_BACK);
        else
            cba->SetCommandSetL(R_SUPLSERVER_EDITOR_OPTIONS_NOMSK_BACK);
        }
    cba->DrawDeferred();
    CleanupStack::Pop(cba);
    DEBUG(-CLocSUPLServerEditor::HandleMSKCaptionL);
    }

// -----------------------------------------------------------------------------
// CLocSUPLServerEditor::UpdateIapL()
// It changes IAP value in AP field
// -----------------------------------------------------------------------------
//
void CLocSUPLServerEditor::UpdateIapL(const TDesC& aIapName)
    {
    DEBUG(+CLocSUPLServerEditor::UpdateIapL);
    if (aIapName.Length() > 0)
        {
        iIap->Des().Copy(aIapName);
        CEikEdwin* apSelector = static_cast<CEikEdwin*> (ControlOrNull(
                ELocSuplServerIdAccessPoint));
        if (apSelector)
            {
            apSelector->SetTextL(iIap);
            }
        apSelector->DrawNow();
        HandleMSKCaptionL();
        if (iIsNewServer)
            {
            iIsModified = ETrue;
            }
        }
    DEBUG(-CLocSUPLServerEditor::UpdateIapL);
    }

// -----------------------------------------------------------------------------
// CLocSUPLServerEditor::DeleteConfirmationQueryL
// Pops the delete confirmation query note
// ----------------------------------------------------------------------------
//
TInt CLocSUPLServerEditor::DeleteConfirmationQueryL()
    {
    HBufC* noteText = NULL;
    noteText = StringLoader::LoadL(R_LOC_DETAIL_CONF_DELETE_SERVER);
    CAknQueryDialog* query = new (ELeave) CAknQueryDialog(*noteText);
    TInt ret = query->ExecuteLD(R_LOC_SUPL_DELETE_QUERY);
    delete noteText; //noteText    
    noteText = NULL;
    return ret;
    }

// -----------------------------------------------------------------------------
// CLocSUPLServerEditor::ShowServerValidationInfoNoteL
// Shows Validation Info note for server address entry
// ----------------------------------------------------------------------------
//
void CLocSUPLServerEditor::ShowServerValidationInfoNoteL(TInt aResourceId)
    {
    // Show the information note to add server address before closing
    HBufC* noteText = StringLoader::LoadL(aResourceId);
    CAknInformationNote* dialog = new (ELeave) CAknInformationNote(
            R_AKN_INFORMATION_NOTE_WAIT);
    dialog->ExecuteLD(*noteText);
    delete noteText; //noteText    
    noteText = NULL;
    }

// ----------------------------------------------------------------------------------
// CLocSUPLServerEditor::SetCursorToEndL
// Moves the cursor position at end if focus is on server address field in edit mode
// ----------------------------------------------------------------------------------
//
void CLocSUPLServerEditor::SetCursorToEndL()
    {
    if (IdOfFocusControl() == ELocSuplServerIdServerAddr)
        {
        CEikEdwin* serverAddress = static_cast<CEikEdwin*> (ControlOrNull(
                ELocSuplServerIdServerAddr));
        if (serverAddress)
            {
            HBufC* address = serverAddress->GetTextInHBufL();
            if (address)
                {
                serverAddress->SetCursorPosL(address->Length(), EFalse);
                serverAddress->DrawDeferred();
                }
            delete address;
            }
        }
    }
// ----------------------------------------------------------------------------------
// CLocSUPLServerEditor::HandleDialogPageEventL
// ----------------------------------------------------------------------------------
//
void CLocSUPLServerEditor::HandleDialogPageEventL(TInt aEventID)
    {
    DEBUG(+CLocSUPLServerEditor::HandleDialogPageEventL);

    CAknForm::HandleDialogPageEventL(aEventID);
    CEikEdwin* iapEditor = static_cast<CEikEdwin*> (ControlOrNull(
            ELocSuplServerIdAccessPoint));
    CEikEdwin* usageInHomeNWPopupFieldText = (CEikEdwin*) Control(
            ELocSuplServerIdUsageInHomeNw);
    TInt focusControl(IdOfFocusControl());

    if (!IsEditable())
        {
        if (focusControl == ELocSuplServerIdUsageInHomeNw || focusControl
                == ELocSuplServerIdServerAddr)
            {
            ProcessCommandL( EAknFormCmdEdit);
            }
        else
            return;
        }
    else if (aEventID == MEikDialogPageObserver::EDialogPageTapped)
        {
        if (focusControl == ELocSuplServerIdUsageInHomeNw)
            {
            if (usageInHomeNWPopupFieldText)
                {
                HBufC* string;
                string = StringLoader::LoadLC(
                        R_LOC_SERVER_USAGEINHOMENETWORK_NO, iCoeEnv);
                TBuf<KMaxUsageTextSize> des;
                usageInHomeNWPopupFieldText->GetText(des);
                if (!des.CompareC(string->Des()))
                    {
                    SetUsageinHomeNetworkL( ETrue);
                    }
                else
                    {
                    SetUsageinHomeNetworkL( EFalse);
                    }
                if (iIsNewServer)
                    {
                    iIsModified = ETrue;
                    }
                CleanupStack::PopAndDestroy(string);
                }
            return;
            }
        }
    DEBUG(-CLocSUPLServerEditor::HandleDialogPageEventL);
    }

// -----------------------------------------------------------------------------
// CLocSUPLServerEditor::SetUsageinHomeNetwork
// Set usage in network field on or off
// -----------------------------------------------------------------------------
//
void CLocSUPLServerEditor::SetUsageinHomeNetworkL(TBool aOnOff)
    {
    HBufC* string;
    CEikEdwin* usageInHomeNWPopupFieldText = (CEikEdwin*) Control(
            ELocSuplServerIdUsageInHomeNw);
    if (aOnOff)
        {
        // load string 'ON'
        string = StringLoader::LoadLC(R_LOC_SERVER_USAGEINHOMENETWORK_YES,
                iCoeEnv);
        }
    else
        {
        // load string 'OFF'
        string = StringLoader::LoadLC(R_LOC_SERVER_USAGEINHOMENETWORK_NO,
                iCoeEnv);
        }
    // set Usage in Network field string as 'ON'/'OFF'
    usageInHomeNWPopupFieldText->SetTextL(string);
    UpdatePageL( ETrue);
    CleanupStack::PopAndDestroy(string);
    }

// -----------------------------------------------------------------------------
// CLocSUPLServerEditor::SetAccessPointEnabled
// 
// -----------------------------------------------------------------------------
//
void CLocSUPLServerEditor::SetAccessPointEnabled(TBool aEnabled)
    {
    iIsAccessPointDefined = aEnabled;
    }

// End of file
