#ifndef AzoomeeCommon_Strings_h
#define AzoomeeCommon_Strings_h

// For convenience
#include "Utils/StringMgr.h"


// ---------- GET VERSION -------------

#define APP_VERSION_NUMBER cocos2d::FileUtils::getInstance()->getStringFromFile("res/Version.txt")

// ---------- LOGIN SCENE TEXT -------------

#define LOGINSCENE_SIGNUP_LOGIN_LABEL "LoginScene/signup_login_label"
#define LOGINSCENE_EMAIL_LABEL "LoginScene/email_label"
#define LOGINSCENE_PASSWORD_LABEL "LoginScene/password_label"

//---------- ONBOARDING SCENE --------------

#define ONBOARDINGSCENE_EMAIL_LABEL "OnboardingScene/email_label"
#define ONBOARDINGSCENE_PASSWORD_LABEL "OnboardingScene/password_label"
#define ONBOARDINGSCENE_PIN_LABEL "OnboardingScene/pin_label"
#define ONBOARDINGSCENE_PIN_SUB_LABEL "OnboardingScene/pin_sub_label"

//---------- ONBOARDING SUCCESS SCENE --------------

#define ONBOARDINGSUCCESSSCENE_TITLE_LABEL "OnboardingSuccessScene/title_label"
#define ONBOARDINGSUCCESSSCENE_SUB_LABEL "OnboardingSuccessScene/sub_label"
#define ONBOARDINGSUCCESSSCENE_CHECKEMAIL_LABEL "OnboardingSuccessScene/checkemail_label"

//----------- CHILD ACCOUNT SCENE ------------

#define CHILDACCOUNTSCENE_REQUEST_NAME_LABEL "ChildAccountScene/request_name_label"
#define CHILDACCOUNTSCENE_EDIT_NAME_LABEL "ChildAccountScene/edit_name_label"
#define CHILDACCOUNTSCENE_REQUEST_DOB_LABEL "ChildAccountScene/request_dob_label"
#define CHILDACCOUNTSCENE_REQUEST_DOB_SUB_LABEL "ChildAccountScene/request_dob_sub_label"

#define CHILDACCOUNTSCENE_EDIT_DOB_LABEL "ChildAccountScene/edit_dob_label"
#define CHILDACCOUNTSCENE_REQUEST_OOMEE_LABEL "ChildAccountScene/request_oomee_label"
#define CHILDACCOUNTSCENE_REQUEST_OOMEE_SUB_LABEL "ChildAccountScene/request_oomee_sub_label"
#define CHILDACCOUNTSCENE_EDIT_OOMEE_LABEL "ChildAccountScene/edit_oomee_label"

#define CHILDACCOUNTSCENE_DOB_DAY_PLACEHOLDER "ChildAccountScene/dob_day_placeholder"
#define CHILDACCOUNTSCENE_DOB_MONTH_PLACEHOLDER "ChildAccountScene/dob_month_placeholder"
#define CHILDACCOUNTSCENE_DOB_YEAR_PLACEHOLDER "ChildAccountScene/dob_year_placeholder"

//----------- CHILD ACCOUNT SUCCESS SCENE ------------

#define CHILDACCOUNTSUCCESSSCENE_REQUEST_NAME_LABEL "ChildAccountSuccessScene/title_label"

//----------- OFFLINE STRINGS---------------------

#define OFFLINESCENE_OFFLINE_TITLE_LABEL "OfflineScene/offline_title_label"
#define OFFLINESCENE_FOR_LOGIN_SUB_TITLE_LABEL "OfflineScene/for_login_sub_title_label"
#define OFFLINESCENE_OFFLINE_SUB_TITLE_LABEL "OfflineScene/offline_sub_title_label"
#define OFFLINESCENE_HUB_LOGGED_IN_BODY_LABEL "OfflineScene/hub_logged_in_body_label"
#define OFFLINESCENE_ONLINE_TITLE_LABEL "OfflineScene/online_title_label"

//------------ OTHER LABELS ------------------

#define PIN_REQUEST_LABEL "OtherLabels/pin_request_label"
#define PREVIEW_MESSAGEBOX_TITLE_LABEL "OtherLabels/preview_messagebox_title_label"
#define PREVIEW_MESSAGEBOX_BODY_LABEL "OtherLabels/preview_messagebox_body_label"
#define CHILD_SELECTSCENE_TITLE_LABEL "OtherLabels/child_selectscene_title_label"
#define LOADING_LABEL "OtherLabels/loading_label"

//------------ GENERIC BUTTONS ----------------

#define BUTTON_EXIT_APP "GenericButtons/exit_app"
#define BUTTON_LOG_OUT "GenericButtons/log_out"
#define BUTTON_SIGN_UP "GenericButtons/sign_up"
#define BUTTON_SIGN_UP_MULTILINE "GenericButtons/sign_up_multiline"
#define BUTTON_LOG_IN "GenericButtons/log_in"
#define BUTTON_LOG_IN_MULTILINE "GenericButtons/log_in_multiline"
#define BUTTON_CANCEL "GenericButtons/cancel"
#define BUTTON_SKIP "GenericButtons/skip"
#define BUTTON_START_EXPLORING "GenericButtons/start_exploring"
#define BUTTON_ADD_ANOTHER_LABEL "GenericButtons/add_another"
#define BUTTON_LETS_GO "GenericButtons/lets_go"
#define BUTTON_RETRY "GenericButtons/retry"


#endif
