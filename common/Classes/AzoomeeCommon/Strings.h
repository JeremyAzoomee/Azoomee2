#ifndef AzoomeeCommon_Strings_h
#define AzoomeeCommon_Strings_h

// For convenience
#include "Utils/StringMgr.h"
#include "Utils/StringFunctions.h"

#define _(string) StringMgr::getInstance()->getStringForKey(string)


// ---------- LOGIN SCENE TEXT -------------

#define LOGINSCENE_EMAIL_LABEL "LoginScene/email_label"
#define LOGINSCENE_PASSWORD_LABEL "LoginScene/password_label"
#define LOGINSCENE_RESET_PASSWORD_TITLE "LoginScene/resetPasswordTitle"
#define LOGINSCENE_RESET_PASSWORD_BODY "LoginScene/resetPasswordBody"

//----------- ADD CHILD SCENE ---------------------

#define ADDCHILD_WELCOME_TITLE "AddChildScene/welcomeTitle"
#define ADDCHILD_WELCOME_TITLE_MULTILINE "AddChildScene/welcomeTitleMultiline"
#define ADDCHILD_WELCOME_SUB_TITLE "AddChildScene/welcomeSubTitle"
#define ADDCHILD_ADD_ANOTHER_TITLE "AddChildScene/addAnotherTitle"
#define ADDCHILD_NAME_HEADING "AddChildScene/nameHeading"
#define ADDCHILD_AGE_TITLE "AddChildScene/ageTitle"
#define ADDCHILD_AGE_TITLE_MULTILINE "AddChildScene/ageTitleMultiline"
#define ADDCHILD_AGE_HEADING "AddChildScene/ageHeading"
#define ADDCHILD_AGE_SUB_HEADING "AddChildScene/ageSubHeading"
#define ADDCHILD_AGE_SUB_HEADING_MULTILINE "AddChildScene/ageSubHeadingMultiline"
#define ADDCHILD_OOMEE_TITLE "AddChildScene/oomeeTitle"
#define ADDCHILD_OOMEE_SUB_TITLE "AddChildScene/oomeeSubTitle"
#define ADDCHILD_OOMEE_SUB_TITLE_MULTILINE "AddChildScene/oomeeSubTitleMultiline"

//----------- OFFLINE STRINGS---------------------

#define OFFLINESCENE_OFFLINE_TITLE_LABEL "OfflineScene/offline_title_label"
#define OFFLINESCENE_FOR_LOGIN_SUB_TITLE_LABEL "OfflineScene/for_login_sub_title_label"
#define OFFLINESCENE_OFFLINE_SUB_TITLE_LABEL "OfflineScene/offline_sub_title_label"
#define OFFLINESCENE_HUB_LOGGED_IN_BODY_LABEL "OfflineScene/hub_logged_in_body_label"
#define OFFLINESCENE_ONLINE_TITLE_LABEL "OfflineScene/online_title_label"

//------------ OTHER LABELS ------------------

#define PIN_REQUEST_LABEL "OtherLabels/pin_request_label"
#define CHILD_SELECTSCENE_TITLE_LABEL "OtherLabels/child_selectscene_title_label"
#define LOADING_LABEL "OtherLabels/loading_label"
#define SAVING_LABEL "OtherLabels/saving_label"
#define KID_CODE "OtherLabels/kidCode"
#define PASSWORD_LABEL "OtherLabels/password"
#define PIN_LABEL "OtherLabels/pin"
#define SAVEQ_LABEL "OtherLabels/saveQ"
#define DELETEQ_LABEL "OtherLabels/deleteQ"
#define RESETQ_LABEL "OtherLabels/resetQ"

//------------ GENERIC BUTTONS ----------------

#define BUTTON_LOG_OUT "GenericButtons/log_out"
#define BUTTON_SIGN_UP "GenericButtons/sign_up"
#define BUTTON_SIGN_UP_MULTILINE "GenericButtons/sign_up_multiline"
#define BUTTON_LOG_IN "GenericButtons/log_in"
#define BUTTON_LOG_IN_MULTILINE "GenericButtons/log_in_multiline"
#define BUTTON_CANCEL "GenericButtons/cancel"
#define BUTTON_CONTINUE "GenericButtons/continue"
#define BUTTON_SKIP "GenericButtons/skip"
#define BUTTON_ADD_ANOTHER_LABEL "GenericButtons/add_another"
#define BUTTON_ADD_ANOTHE_CHILD "GenericButtons/add_another_child"
#define BUTTON_LETS_GO "GenericButtons/lets_go"
#define BUTTON_RETRY "GenericButtons/retry"
#define BUTTON_RESET "GenericButtons/reset"
#define BUTTON_REPORT "GenericButtons/report"
#define BUTTON_ONLINE_SAFETY_TIPS "GenericButtons/onlineSafetyTips"
#define BUTTON_BACK "GenericButtons/back"
#define BUTTON_DONE "GenericButtons/done"
#define BUTTON_EDIT "GenericButtons/edit"
#define BUTTON_CONFIRM "GenericButtons/confirm"
#define BUTTON_REJECT "GenericButtons/reject"
#define BUTTON_YES "GenericButtons/yes"
#define BUTTON_NO "GenericButtons/no"
#define BUTTON_TRY_AGAIN "GenericButtons/tryAgain"
#define BUTTON_START_TRIAL "GenericButtons/startTrial"
#define BUTTON_UPDATE "GenericButtons/update"
#define BUTTON_OK "GenericButtons/ok"
#define BUTTON_DELETE "GenericButtons/delete"
#define BUTTON_SEND "GenericButtons/send"
#define BUTTON_MANAGE "GenericButtons/manage"
#define BUTTON_RENEW_SUB "GenericButtons/renewSub"

//--------------ONLINE SAFETY SLIDES------------------

#define ONLINE_SAFETY_MAIN_TITLE "OnlineSafetySlides/mainTitle"
#define ONLINE_SAFETY_SLIDE_TITLE "OnlineSafetySlides/title"
#define ONLINE_SAFETY_SLIDE_MAIN_TEXT "OnlineSafetySlides/mainText"
#define ONLINE_SAFETY_BUTTON_TEXT "OnlineSafetySlides/buttonTitle"

//-------------CHAT MESSAGE BOXES-------------------

#define CHAT_MESSAGE_BOXES_RESET_CHAT_TITLE "ChatMessageBoxes/resetChatTitle"
#define CHAT_MESSAGE_BOXES_RESET_CHAT_BODY "ChatMessageBoxes/resetChatBody"
#define CHAT_MESSAGE_BOXES_REPORT_CHAT_TITLE "ChatMessageBoxes/reportChatTitle"

//------------TERMS AND PRIVACY-------------------

#define T_and_C_Start_Text "TandC/startText"
#define T_and_C_Privacy_Button "TandC/privacyButton"
#define T_and_C_And "TandC/and"
#define T_and_C_Terms_Button "TandC/termsButton"

//------------ CHAT ---------------------------

#define CHAT_CHAT_REPORTED "Chat/chatReported"
#define CHAT_CHAT_REPORTED_MULTILINE "Chat/chatReportedMultiline"
#define CHAT_SHARE_WITH_FRIEND "Chat/shareWithFriend"
#define CHAT_ADD_FRIENDS_BUTTON "Chat/addFriendsButton"

//------------ ART APP ------------------------

#define ART_APP_DELETE_ART "ArtApp/deleteArt"

//----------- OOMEE MAKER ----------------------

#define OOMEE_MAKER_NEW_AVATAR "OomeeMaker/newAvatar"

//----------- ME HQ ----------------------------

#define MEHQ_FIRST_USE_MESSAGE_SENDER "MeHQ/firstUseMessageSender"
#define MEHQ_FIRST_USE_MESSAGE_BODY "MeHQ/firstUseMessageBody"
#define MEHQ_MESSAGE_PLACEHOLDER_PICTURE "MeHQ/messagePlaceholderPicture"
#define MEHQ_MESSAGE_PLACEHOLDER_STICKER "MeHQ/messagePlaceholderSticker"
#define MEHQ_MESSAGE_PLACEHOLDER_LINK "MeHQ/messagePlaceholderLink"
#define MEHQ_HEADING_MESSAGES "MeHQ/headingMessages"
#define MEHQ_HEADING_GALLERY "MeHQ/headingGallery"
#define MEHQ_HEADING_FAVOURITES "MeHQ/headingFavourites"
#define MEHQ_HEADING_DOWNLOADS "MeHQ/headingDownloads"
#define MEHQ_HEADING_DOWNLOADS_EMPTY "MeHQ/headingDownloadsEmpty"
#define MEHQ_SUB_HEADING_DOWNLOADS "MeHQ/subHeadingDownloads"

//----------- USER MESSAGING -------------------

#define USER_MSG_START_TRIAL "UserMessaging/startTrial"
#define USER_MSG_REACTIVATE "UserMessaging/reactivate"
#define USER_MSG_PREMIUM "UserMessaging/premium"
#define USER_MSG_PREMIUM_MULTILINE "UserMessaging/premiumMultiline"

//----------- FORCE UPDATE ---------------------

#define FORCE_UPDATE_TITLE "ForceUpdate/title"
#define FORCE_UPDATE_SUB_TITLE "ForceUpdate/subTitle"
#define FORCE_UPDATE_BUTTON "ForceUpdate/button"
#define FORCE_UPDATE_MSG_BOX_TITLE "ForcUpdate/messageBoxTitle"
#define FORCE_UPDATE_MSG_BOX_BODY "ForceUpdate/messageBoxBody"

//----------- INPUT PLACEHOLDERS ---------------

#define INPUT_PLACEHOLDER_PASSWORD "InputPlaceholders/password"
#define INPUT_PLACEHOLDER_CHILD_NAME "InputPlaceholders/childName"
#define INPUT_PLACEHOLDER_AGE "InputPlaceholders/age"
#define INPUT_PLACEHOLDER_DAY "InputPlaceholders/day"
#define INPUT_PLACEHOLDER_MONTH "InputPlaceholders/month"
#define INPUT_PLACEHOLDER_YEAR "InputPlaceholders/year"
#define INPUT_PLACEHOLDER_KID_CODE "InputPlaceholders/kidCode"

//----------- SETTINGS -------------------------

#define SETTINGS_HEADING_MAIN "Settings/headingMain"
#define SETTINGS_HEADING_LANGUAGE "Settings/headingLanguage"
#define SETTINGS_HEADING_YOUR_KIDS "Settings/headingYourKids"
#define SETTINGS_HEADING_FRIENDSHIPS "Settings/headingFriendships"
#define SETTINGS_HEADING_ONLINE_SAFETY "Settings/headingOnlineSafety"
#define SETTINGS_HEADING_YOUR_ACCOUNT "Settings/headingYourAccount"
#define SETTINGS_HEADING_SUPPORT "Settings/headingSupport"
#define SETTINGS_SUB_HEADING_LANGUAGE "Settings/subHeadingLanguage"
#define SETTINGS_SUB_HEADING_YOUR_KIDS "Settings/subHeadingYourKids"
#define SETTINGS_SUB_HEADING_FRIENDSHIPS "Settings/subHeadingFriendships"
#define SETTINGS_SUB_HEADING_ONLINE_SAFETY "Settings/subHeadingOnlineSafety"
#define SETTINGS_SUB_HEADING_YOUR_ACCOUNT "Settings/subHeadingYourAccount"
#define SETTINGS_SUB_HEADING_SUPPORT "Settings/subHeadingSupport"
#define SETTINGS_PAGE_HEADING_YOUR_KIDS "Settings/pageHeadingYourKids"
#define SETTINGS_PAGE_HEADING_FRIENDSHIPS "Settings/pageHeadingFriendships"
#define SETTINGS_PAGE_HEADING_ONLINE_SAFETY "Settings/pageHeadingOnlineSafety"
#define SETTINGS_PAGE_HEADING_YOUR_ACCOUNT "Settings/pageHeadingYourAccount"
#define SETTINGS_PAGE_HEADING_SUPPORT "Settings/pageHeadingSupport"
#define SETTINGS_ADD_PROFILE_BUTTON "Settings/addProfileButton"
#define SETTINGS_SUPPORT_BODY "Settings/supportBody"
#define SETTINGS_ACCOUNT_PREMIUM_LABEL "Settings/accountPremiumLabel"
#define SETTINGS_ACCOUNT_FREE_LABEL "Settings/accountFreeLabel"
#define SETTINGS_FRIEND_REQUEST_BODY "Settings/friendRequestBody"
#define SETTINGS_SHARE_KID_CODE "Settings/shareKidCode"
#define SETTINGS_ADD_FRIEND "Settings/addFriend"
#define SETTINGS_CODE_UNRECOGNISED "Settings/codeUnrecognised"
#define SETTINGS_CODE_UNRECOGNISED_DETAILS "Settings/codeUnrecognisedDetails"
#define SETTINGS_CODE_ACCEPTED "Settings/codeAccepted"
#define SETTINGS_CODE_ACCEPTED_DETAILS "Settings/codeAcceptedDetails"
#define SETTINGS_FRIENDSHIP_CONFIRMED "Settings/friendshipConfirmed"
#define SETTINGS_REJECT_FRIENDSHIP_Q "Settings/rejectFriendshipQ"
#define SETTINGS_REJECTED "Settings/rejected"
#define SETTINGS_ENTER_KID_CODE "Settings/enterKidCode"
#define SETTINGS_SHARE_CODE_TEXT "Settings/shareCodeText"
#define SETTINGS_DELETE_PROFILE "Settings/deleteProfile"
#define SETTINGS_LEARN_ABOUT_SUBSCRIPTIONS "Settings/learnAboutSubscriptions"
#define SETTINGS_ACCOUNT_TYPE_TEXT "Settings/accountTypeText"
#define SETTINGS_FRIENDSHIPS_TO_APPROVE "Settings/friendshipsToApprove"
#define SETTINGS_NO_FRIENDSHIPS_TO_APPROVE "Settings/noFriendshipsToApprove"

//---------- NATIVE STRINGS -----------------------

#define NATIVE_FAV_CONTENT_LABEL "Native/favContentLabel"

#endif
