/*
 * iotkit 'C' Library to communicate with IoT Cloud via REST APIs
 * Copyright (c) 2014, Intel Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU Lesser General Public License,
 * version 2.1, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 */

/**
 * @file iotkit.c
 * @brief Implementation of iotkit library
 *
 * Provides features for communication with IoT Cloud server
 */

#include "iotkit.h"

static bool isInitialized = false;

void iotkit_init() {

    parseConfiguration("../config/config.json");
    parseAuthorizationToken();
    parseDeviceToken();
    parseComponentsList();

    if(!isInitialized) {
        CURLcode code = rest_init(configurations.isSecure);
        if(code) {
            fprintf(stderr, "Unable to initialize CURL %d\n", code);
        } else {
            isInitialized = true;
            puts("Library initialized successfully");
        }
    } else {
        puts("Library is already initialized and doesn't need to be re-initialized");
    }
}

void iotkit_cleanup() {
    if(isInitialized) {
        rest_cleanup();
        isInitialized = false;
    } else {
        puts("Library is not initialized and doesn't need to cleanup resources");
    }
}


/** Parses configuration JSON
* @param[in] config file path to the JSON
* @return returns client query description object upon successful parsing and NULL otherwise
*/
void parseConfiguration(char *config_file_path) {
    char *out;
    int i = 0;
    cJSON *json, *jitem, *child1, *child2;
    bool status = true;

    FILE *fp = fopen(config_file_path, "rb");
    if (fp == NULL) {
        fprintf(stderr,"Error can't open file %s\n", config_file_path);
    }
    else {
        fseek(fp, 0, SEEK_END);
        long size = ftell(fp);
        rewind(fp);

        // read the file
        char *buffer = (char *)malloc(size+1);
        fread(buffer, 1, size, fp);

        // parse the file
        json = cJSON_Parse(buffer);
        if (!json) {
            fprintf(stderr,"Error before: [%s]\n",cJSON_GetErrorPtr());
        }
        else {
            #if DEBUG
                out = cJSON_Print(json, 2);
                printf("%s\n", out);
                free(out);
            #endif

            if (!isJsonObject(json)) {
                fprintf(stderr,"Invalid JSON format for %s file\n", config_file_path);
                return;
            }

            jitem = cJSON_GetObjectItem(json, "isSecure");
            if (!isJsonBooleanTrue(jitem)) {
                fprintf(stderr,"Invalid JSON format for json property %s\n", jitem->string);
                return;
            }
            configurations.isSecure = true;

            jitem = cJSON_GetObjectItem(json, "host");
            if (!isJsonString(jitem)) {
                fprintf(stderr,"Invalid JSON format for json property %s\n", jitem->string);
                return;
            }
            configurations.base_url = strdup(jitem->valuestring);


            jitem = cJSON_GetObjectItem(json, "apipath");
            if (!isJsonObject(jitem)) {
                fprintf(stderr,"Invalid JSON format for json property %s\n", jitem->string);
                return;
            }

            child1 = cJSON_GetObjectItem(jitem, "alert_management");
            if (!isJsonObject(child1)) {
                fprintf(stderr,"Invalid JSON format for json property %s\n", child1->string);
                return;
            }

            child2 = cJSON_GetObjectItem(child1, "create_new_alert");
            if (!isJsonString(child2)) {
                fprintf(stderr,"Invalid JSON format for json property %s\n", child2->string);
                return;
            }
            configurations.create_new_alert = strdup(child2->valuestring);

            child2 = cJSON_GetObjectItem(child1, "get_list_of_alerts");
            if (!isJsonString(child2)) {
                fprintf(stderr,"Invalid JSON format for json property %s\n", child2->string);
                return;
            }
            configurations.get_list_of_alerts = strdup(child2->valuestring);

            child2 = cJSON_GetObjectItem(child1, "get_alert_information");
            if (!isJsonString(child2)) {
                fprintf(stderr,"Invalid JSON format for json property %s\n", child2->string);
                return;
            }
            configurations.get_alert_information = strdup(child2->valuestring);

            child2 = cJSON_GetObjectItem(child1, "reset_alert");
            if (!isJsonString(child2)) {
                fprintf(stderr,"Invalid JSON format for json property %s\n", child2->string);
                return;
            }
            configurations.reset_alert = strdup(child2->valuestring);

            child2 = cJSON_GetObjectItem(child1, "update_alert_status");
            if (!isJsonString(child2)) {
                fprintf(stderr,"Invalid JSON format for json property %s\n", child2->string);
                return;
            }
            configurations.update_alert_status = strdup(child2->valuestring);

            child2 = cJSON_GetObjectItem(child1, "add_comment_to_alert");
            if (!isJsonString(child2)) {
                fprintf(stderr,"Invalid JSON format for json property %s\n", child2->string);
                return;
            }
            configurations.add_comment_to_alert = strdup(child2->valuestring);


            child1 = cJSON_GetObjectItem(jitem, "account_management");
            if (!isJsonObject(child1)) {
                fprintf(stderr,"Invalid JSON format for json property %s\n", child1->string);
                return;
            }

            child2 = cJSON_GetObjectItem(child1, "create_an_account");
            if (!isJsonString(child2)) {
                fprintf(stderr,"Invalid JSON format for json property %s\n", child2->string);
                return;
            }
            configurations.create_an_account = strdup(child2->valuestring);

            child2 = cJSON_GetObjectItem(child1, "get_account_information");
            if (!isJsonString(child2)) {
                fprintf(stderr,"Invalid JSON format for json property %s\n", child2->string);
                return;
            }
            configurations.get_account_information = strdup(child2->valuestring);

            child2 = cJSON_GetObjectItem(child1, "get_account_activation_code");
            if (!isJsonString(child2)) {
                fprintf(stderr,"Invalid JSON format for json property %s\n", child2->string);
                return;
            }
            configurations.get_account_activation_code = strdup(child2->valuestring);

            child2 = cJSON_GetObjectItem(child1, "renew_account_activation");
            if (!isJsonString(child2)) {
                fprintf(stderr,"Invalid JSON format for json property %s\n", child2->string);
                return;
            }
            configurations.renew_account_activation = strdup(child2->valuestring);

            child2 = cJSON_GetObjectItem(child1, "update_an_account_name");
            if (!isJsonString(child2)) {
                fprintf(stderr,"Invalid JSON format for json property %s\n", child2->string);
                return;
            }
            configurations.update_an_account_name = strdup(child2->valuestring);

            child2 = cJSON_GetObjectItem(child1, "delete_an_account_name");
            if (!isJsonString(child2)) {
                fprintf(stderr,"Invalid JSON format for json property %s\n", child2->string);
                return;
            }
            configurations.delete_an_account_name = strdup(child2->valuestring);

            child2 = cJSON_GetObjectItem(child1, "add_an_user_to_account");
            if (!isJsonString(child2)) {
                fprintf(stderr,"Invalid JSON format for json property %s\n", child2->string);
                return;
            }
            configurations.add_an_user_to_account = strdup(child2->valuestring);

            child2 = cJSON_GetObjectItem(child1, "get_user_associated_with_account");
            if (!isJsonString(child2)) {
                fprintf(stderr,"Invalid JSON format for json property %s\n", child2->string);
                return;
            }
            configurations.get_user_associated_with_account = strdup(child2->valuestring);

            child2 = cJSON_GetObjectItem(child1, "update_user_associated_with_account");
            if (!isJsonString(child2)) {
                fprintf(stderr,"Invalid JSON format for json property %s\n", child2->string);
                return;
            }
            configurations.update_user_associated_with_account = strdup(child2->valuestring);

            child1 = cJSON_GetObjectItem(jitem, "advanced_data_inquiry");
            if (!isJsonString(child1)) {
                fprintf(stderr,"Invalid JSON format for json property %s\n", child1->string);
                return;
            }
            configurations.advanced_data_inquiry = strdup(child1->valuestring);

            child1 = cJSON_GetObjectItem(jitem, "aggregated_report_interface");
            if (!isJsonString(child1)) {
                fprintf(stderr,"Invalid JSON format for json property %s\n", child1->string);
                return;
            }
            configurations.aggregated_report_interface = strdup(child1->valuestring);

            child1 = cJSON_GetObjectItem(jitem, "authorization");
            if (!isJsonObject(child1)) {
                fprintf(stderr,"Invalid JSON format for json property %s\n", child1->string);
                return;
            }

            child2 = cJSON_GetObjectItem(child1, "new_auth_token");
            if (!isJsonString(child2)) {
                fprintf(stderr,"Invalid JSON format for json property %s\n", child2->string);
                return;
            }
            configurations.new_auth_token = strdup(child2->valuestring);

            child2 = cJSON_GetObjectItem(child1, "auth_token_info");
            if (!isJsonString(child2)) {
                fprintf(stderr,"Invalid JSON format for json property %s\n", child2->string);
                return;
            }
            configurations.auth_token_info = strdup(child2->valuestring);
            printf("Read auth_token_info is %s\n", configurations.auth_token_info);

            child2 = cJSON_GetObjectItem(child1, "me_info");
            if (!isJsonString(child2)) {
                fprintf(stderr,"Invalid JSON format for json property %s\n", child2->string);
                return;
            }
            configurations.me_info = strdup(child2->valuestring);
            printf("Read me_info is %s\n", configurations.me_info);


            child1 = cJSON_GetObjectItem(jitem, "cmpcatalog");
            if (!isJsonObject(child1)) {
                fprintf(stderr,"Invalid JSON format for json property %s\n", child1->string);
                return;
            }

            child2 = cJSON_GetObjectItem(child1, "list_components");
            if (!isJsonString(child2)) {
                fprintf(stderr,"Invalid JSON format for json property %s\n", child2->string);
                return;
            }
            configurations.list_components = strdup(child2->valuestring);

            child2 = cJSON_GetObjectItem(child1, "get_component_details");
            if (!isJsonString(child2)) {
                fprintf(stderr,"Invalid JSON format for json property %s\n", child2->string);
                return;
            }
            configurations.get_component_details = strdup(child2->valuestring);

            child2 = cJSON_GetObjectItem(child1, "create_an_cmp_catalog");
            if (!isJsonString(child2)) {
                fprintf(stderr,"Invalid JSON format for json property %s\n", child2->string);
                return;
            }
            configurations.create_an_cmp_catalog = strdup(child2->valuestring);

            child2 = cJSON_GetObjectItem(child1, "update_an_cmp_catalog");
            if (!isJsonString(child2)) {
                fprintf(stderr,"Invalid JSON format for json property %s\n", child2->string);
                return;
            }
            configurations.update_an_cmp_catalog = strdup(child2->valuestring);


            child1 = cJSON_GetObjectItem(jitem, "device_management");
            if (!isJsonObject(child1)) {
                fprintf(stderr,"Invalid JSON format for json property %s\n", child1->string);
                return;
            }

            child2 = cJSON_GetObjectItem(child1, "list_all_devices");
            if (!isJsonString(child2)) {
                fprintf(stderr,"Invalid JSON format for json property %s\n", child2->string);
                return;
            }
            configurations.list_all_devices = strdup(child2->valuestring);

            child2 = cJSON_GetObjectItem(child1, "get_device_info");
            if (!isJsonString(child2)) {
                fprintf(stderr,"Invalid JSON format for json property %s\n", child2->string);
                return;
            }
            configurations.get_device_info = strdup(child2->valuestring);

            child2 = cJSON_GetObjectItem(child1, "get_my_device_info");
            if (!isJsonString(child2)) {
                fprintf(stderr,"Invalid JSON format for json property %s\n", child2->string);
                return;
            }
            configurations.get_my_device_info = strdup(child2->valuestring);

            child2 = cJSON_GetObjectItem(child1, "create_a_device");
            if (!isJsonString(child2)) {
                fprintf(stderr,"Invalid JSON format for json property %s\n", child2->string);
                return;
            }
            configurations.create_a_device = strdup(child2->valuestring);

            child2 = cJSON_GetObjectItem(child1, "update_a_device");
            if (!isJsonString(child2)) {
                fprintf(stderr,"Invalid JSON format for json property %s\n", child2->string);
                return;
            }
            configurations.update_a_device = strdup(child2->valuestring);

            child2 = cJSON_GetObjectItem(child1, "activate_a_device");
            if (!isJsonString(child2)) {
                fprintf(stderr,"Invalid JSON format for json property %s\n", child2->string);
                return;
            }
            configurations.activate_a_device = strdup(child2->valuestring);

            child2 = cJSON_GetObjectItem(child1, "delete_a_device");
            if (!isJsonString(child2)) {
                fprintf(stderr,"Invalid JSON format for json property %s\n", child2->string);
                return;
            }
            configurations.delete_a_device = strdup(child2->valuestring);

            child2 = cJSON_GetObjectItem(child1, "add_a_component");
            if (!isJsonString(child2)) {
                fprintf(stderr,"Invalid JSON format for json property %s\n", child2->string);
                return;
            }
            configurations.add_a_component = strdup(child2->valuestring);

            child2 = cJSON_GetObjectItem(child1, "delete_a_component");
            if (!isJsonString(child2)) {
                fprintf(stderr,"Invalid JSON format for json property %s\n", child2->string);
                return;
            }
            configurations.delete_a_component = strdup(child2->valuestring);

            child2 = cJSON_GetObjectItem(child1, "list_all_tags_for_devices");
            if (!isJsonString(child2)) {
                fprintf(stderr,"Invalid JSON format for json property %s\n", child2->string);
                return;
            }
            configurations.list_all_tags_for_devices = strdup(child2->valuestring);

            child2 = cJSON_GetObjectItem(child1, "list_all_attributes_for_devices");
            if (!isJsonString(child2)) {
                fprintf(stderr,"Invalid JSON format for json property %s\n", child2->string);
                return;
            }
            configurations.list_all_attributes_for_devices = strdup(child2->valuestring);


            child1 = cJSON_GetObjectItem(jitem, "invitation_management");
            if (!isJsonObject(child1)) {
                fprintf(stderr,"Invalid JSON format for json property %s\n", child1->string);
                return;
            }

            child2 = cJSON_GetObjectItem(child1, "get_list_of_invitation");
            if (!isJsonString(child2)) {
                fprintf(stderr,"Invalid JSON format for json property %s\n", child2->string);
                return;
            }
            configurations.get_list_of_invitation = strdup(child2->valuestring);

            child2 = cJSON_GetObjectItem(child1, "get_invitation_list_send_to_specific_user");
            if (!isJsonString(child2)) {
                fprintf(stderr,"Invalid JSON format for json property %s\n", child2->string);
                return;
            }
            configurations.get_invitation_list_send_to_specific_user = strdup(child2->valuestring);

            child2 = cJSON_GetObjectItem(child1, "create_invitation");
            if (!isJsonString(child2)) {
                fprintf(stderr,"Invalid JSON format for json property %s\n", child2->string);
                return;
            }
            configurations.create_invitation = strdup(child2->valuestring);

            child2 = cJSON_GetObjectItem(child1, "delete_invitations");
            if (!isJsonString(child2)) {
                fprintf(stderr,"Invalid JSON format for json property %s\n", child2->string);
                return;
            }
            configurations.delete_invitations = strdup(child2->valuestring);


            child1 = cJSON_GetObjectItem(jitem, "data");
            if (!isJsonObject(child1)) {
                fprintf(stderr,"Invalid JSON format for json property %s\n", child1->string);
                return;
            }

            child2 = cJSON_GetObjectItem(child1, "submit_data");
            if (!isJsonString(child2)) {
                fprintf(stderr,"Invalid JSON format for json property %s\n", child2->string);
                return;
            }
            configurations.submit_data = strdup(child2->valuestring);

            child2 = cJSON_GetObjectItem(child1, "retrieve_data");
            if (!isJsonString(child2)) {
                fprintf(stderr,"Invalid JSON format for json property %s\n", child2->string);
                return;
            }
            configurations.retrieve_data = strdup(child2->valuestring);


            child1 = cJSON_GetObjectItem(jitem, "rule_management");
            if (!isJsonObject(child1)) {
                fprintf(stderr,"Invalid JSON format for json property %s\n", child1->string);
                return;
            }

            child2 = cJSON_GetObjectItem(child1, "create_a_rule");
            if (!isJsonString(child2)) {
                fprintf(stderr,"Invalid JSON format for json property %s\n", child2->string);
                return;
            }
            configurations.create_a_rule = strdup(child2->valuestring);

            child2 = cJSON_GetObjectItem(child1, "update_a_rule");
            if (!isJsonString(child2)) {
                fprintf(stderr,"Invalid JSON format for json property %s\n", child2->string);
                return;
            }
            configurations.update_a_rule = strdup(child2->valuestring);


            cJSON_Delete(json);
        }

        // free buffers
        fclose(fp);
        free(buffer);
    }

    return ;
}


bool prepareUrl(char **full_url, char *url_prepend, char *url_append, KeyValueParams *urlParams) {
    int urlSize;
    char *url_post = NULL;

    if(!url_prepend || !url_append) {
        fprintf(stderr, "prepareUrl: Parameter cannot be NULL");
        return false;
    }

//    url_post = url_append;

    char *start = url_append;
    char *end = NULL;

    while(start != NULL && strstr(start, "{") != NULL) {

        start = strstr(start, "{");
        end = strstr(start, "}");

        char strtokensize = end - start;
        char *strtoken = (char *)malloc(sizeof(char) * strtokensize);
        strncpy(strtoken, start + 1, strtokensize -1);
        strtoken[strtokensize - 1] = '\0';

        if(strcmp(strtoken, "data_account_id") == 0) {
            int url_post_size = 0;

            if(configurations.data_account_id == NULL){
                puts("Data Account ID not Found");
                return false;
            }

            url_post_size = (start - url_append) + strlen(configurations.data_account_id) + strlen(end);

            if(url_post == NULL) {
                url_post = (char *)malloc(sizeof(char) * url_post_size);
                strncpy(url_post, url_append, (start - url_append));
                url_post[start - url_append] = '\0';
            } else {
                url_post_size += strlen(url_post);
                url_post = (char *)realloc(url_post, sizeof(char) * url_post_size);
                strncat(url_post, url_append, (start - url_append));
            }

            strcat(url_post, configurations.data_account_id);
        } else if(strcmp(strtoken, "device_id") == 0) {
            int url_post_size = 0;

            if(configurations.device_id == NULL){
                puts("Device ID not Found");
                return false;
            }

            url_post_size = (start - url_append) + strlen(configurations.device_id) + strlen(end);

            if(url_post == NULL) {
                url_post = (char *)malloc(sizeof(char) * url_post_size);
                strncpy(url_post, url_append, (start - url_append));
                url_post[start - url_append] = '\0';
            } else {
                url_post_size += strlen(url_post);
                url_post = (char *)realloc(url_post, sizeof(char) * url_post_size);
                strncat(url_post, url_append, (start - url_append));
            }

            strcat(url_post, configurations.device_id);
        } else if(strcmp(strtoken, "user_account_id") == 0) {
            int url_post_size = 0;

            if(configurations.user_account_id == NULL){
                puts("User Account ID not Found");
                return false;
            }

            url_post_size = (start - url_append) + strlen(configurations.user_account_id) + strlen(end);

            if(url_post == NULL) {
                url_post = (char *)malloc(sizeof(char) * url_post_size);
                strncpy(url_post, url_append, (start - url_append));
                url_post[start - url_append] = '\0';
            } else {
                url_post_size += strlen(url_post);
                url_post = (char *)realloc(url_post, sizeof(char) * url_post_size);
                strncat(url_post, url_append, (start - url_append));
            }

            strcat(url_post, configurations.user_account_id);
        } else {
            int url_post_size = 0;
            KeyValueParams *traverseParams = urlParams;
            char *paramValue = NULL;

            while(traverseParams != NULL) {
                if(strcmp(traverseParams->name, strtoken) == 0) {
                    paramValue = traverseParams->value;
                    break;
                }

                traverseParams = traverseParams->next;
            }

            if(!paramValue) {
                paramValue = (char *)malloc(sizeof(char) * strlen(strtoken)) + 2;
                strcpy(paramValue, "{");
                strcat(paramValue, strtoken);
                strcat(paramValue, "}");
            }

            url_post_size = (start - url_append) + strlen(paramValue) + strlen(end);

            if(url_post == NULL) {
                url_post = (char *)malloc(sizeof(char) * url_post_size);
                strncpy(url_post, url_append, (start - url_append));
                url_post[start - url_append] = '\0';
            } else {
                url_post_size += strlen(url_post);
                url_post = (char *)realloc(url_post, sizeof(char) * url_post_size);
                strncat(url_post, url_append, (start - url_append));
            }

            strcat(url_post, paramValue);
        }

        url_append = end + 1;
        start = url_append;
    }

    if(url_post == NULL) {
        url_post = url_append;
    } else if(end != NULL) {
        strcat(url_post, url_append);
    }

    urlSize = configurations.isSecure ? strlen(HTTPS_PROTOCOL) : strlen(HTTP_PROTOCOL);
    urlSize += strlen(url_prepend) + strlen(url_post) + 1;
    *full_url = (char *)malloc(sizeof(char) * urlSize);
    if(configurations.isSecure){
        strcpy(*full_url, HTTPS_PROTOCOL);
    } else {
        strcpy(*full_url, HTTP_PROTOCOL);
    }

    strcat(*full_url, url_prepend);
    strcat(*full_url, url_post);


    #if DEBUG
        printf("URL prepared is %s\n", *full_url);
    #endif

    return true;
}


char *getConfigAuthorizationToken() {
    char *authorization;
    int authorizationSize = strlen(HEADER_AUTHORIZATION_BEARER) + strlen(configurations.authorization_key) + 1;

    authorization = (char *)malloc(sizeof(char) * authorizationSize);
    strcpy(authorization, HEADER_AUTHORIZATION_BEARER);
    strcat(authorization, configurations.authorization_key);

    return authorization;
}

char *getDeviceAuthorizationToken() {
    char *authorization;
    int authorizationSize = strlen(HEADER_AUTHORIZATION_BEARER) + strlen(configurations.deviceToken) + 1;

    authorization = (char *)malloc(sizeof(char) * authorizationSize);
    strcpy(authorization, HEADER_AUTHORIZATION_BEARER);
    strcat(authorization, configurations.deviceToken);

    return authorization;
}

#if DEBUG

    void main() {
        puts("Iotkit Library to communicate with IoT Cloud via REST APIs");

        iotkit_init();

//        testGetAccountInformation();
//        testAddAnUserToAccount();
//        testAdvancedDataInquiry();
//        testAggregatedReportInterface();
//        testCreateNewAlert();
//        testGetListOfAlerts();
//        testGetAlertInformation();
//        testResetAlert();
//        testUpdateAlertStatus();
//        testAddCommentToAlert();
//        testGetUserAssociatedWithAccount();
//        testUpdateUserAssociatedWithAccount();

//        testGetNewAuthorizationToken();
//        testValidateAuthorizationToken();
//        testGetAuthorizationTokenMeInfo();

//        testListAllComponentCatalogs();
//        testGetComponentCatalogDetails();

//        testSubmitData();

//        testValidateDeviceToken();
//        testListAllDevices();
//        testGetMyDeviceInfo();
//        testGetOneDeviceInfo();
//        testDeleteADevice();
//        testDeleteComponent();

//        testCreateADevice();
//        testUpdateADevice();
//        testActivateADevice("CEUztvgI");

//        testDeviceActivationStatus();

//        testAddComponent();
//        testSensorRegistrationStatus();
//        testGetSensorId();
//        testListAllTagsForDevices();
//        testListAllAttributesForDevices();

//        testGetListOfInvitation();
//        testGetInvitationListSendToSpecificUser();
//        testCreateInvitation();
//        testDeleteInvitation();
//        testCreateAnComponentCatalog();
//        testUpdateAnComponentCatalog();

//        testRetrieveData();

//        testCreateAnRule();
        testUpdateAnRule();

        iotkit_cleanup();
    }

#endif
