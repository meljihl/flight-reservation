#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <microhttpd.h>
#include <glob.h>
#include "commons/helper.h"
#include <sqlite3.h>

#define PORT 8888

struct response {
    char *response_body;
    int response_code;
    //headers?
};
typedef struct response http_restponse;

void handle_request(char const *url, char const *method, char const *data, size_t *pInt, http_restponse *pRestponse);

void handle_root(http_restponse *pRestponse);

void handle_get_countries(http_restponse *pRestponse);

void handle_get_country(char *country, http_restponse *pRestponse);


int answer_to_connection(void *cls, struct MHD_Connection *connection,
                         const char *url,
                         const char *method, const char *version,
                         const char *upload_data,
                         size_t *upload_data_size, void **con_cls) {


    struct MHD_Response *response;
    int ret;

    http_restponse localResponse;
    handle_request(url, method, upload_data, upload_data_size, &localResponse);

    response = MHD_create_response_from_buffer(strlen(localResponse.response_body),
                                               (void *) localResponse.response_body, MHD_RESPMEM_MUST_FREE);


    ret = MHD_queue_response(connection, localResponse.response_code, response);
    //free(page); no need since MHD_RESPMEM_MUST_FREE
    //if (localResponse.response_body != NULL)
    //free(localResponse.response_body);

    MHD_destroy_response(response);

    return ret;
}

//state machine
void handle_request(char const *url, char const *method, char const *data, size_t *pInt, http_restponse *pRestponse) {
    const char *notFound = "not found";


    if (strcmp(url, "/") == 0) {
        if (strcmp(method, "GET") == 0) {
            //here we don't use / ignore data since it's a GET
            handle_root(pRestponse);
            return;
        }
    }
    else if (strcmp(url, "/countries") == 0) {
        if (strcmp(method, "GET") == 0) {
            //here we don't use / ignore data since it's a GET
            handle_get_countries(pRestponse);
            return;
        }

    }
    struct re_groups groups = get_regex_groups("^/countries/([^/]+)$", url);
    if (groups.data != NULL) {
        if (strcmp(method, "GET") == 0) {
            //here we don't use / ignore data since it's a GET
            handle_get_country(groups.data[0], pRestponse);
            free_groups(groups);
            return;
        }

    }

    

    pRestponse->response_code = 404;

    pRestponse->response_body = malloc(sizeof(char) * (strlen(notFound) + 2));
    memset(pRestponse->response_body, 0, sizeof(pRestponse->response_body));
    strcpy(pRestponse->response_body, notFound);
}


//example handle
void handle_root(http_restponse *pRestponse) {
    const char *root = "vous etes bien a la racine!";
    pRestponse->response_code = 200;
    pRestponse->response_body = malloc(512 * sizeof(char));
    memset(pRestponse->response_body, '\0', sizeof(pRestponse->response_body));
    strcpy(pRestponse->response_body, root);
}

void handle_get_countries(http_restponse *pRestponse) {
    const char *res = "France;Italy;England";
    pRestponse->response_code = 200;
    pRestponse->response_body = malloc(512 * sizeof(char));
    memset(pRestponse->response_body, '\0', sizeof(pRestponse->response_body));
    strcpy(pRestponse->response_body, res);
}

void handle_get_country(char *country, http_restponse *pRestponse) {


    pRestponse->response_body = malloc(512 * sizeof(char));
    memset(pRestponse->response_body, '\0', sizeof(pRestponse->response_body));

    if (strcmp("France", country) == 0) {
        pRestponse->response_code = 200;

        sprintf(pRestponse->response_body, "Population:%d\nCapital:%s\nCurrency:%s", 66917469, "Paris", "EUR");
        pRestponse->response_code = 200;
    }
    else if (strcmp("Italy", country) == 0) {
        pRestponse->response_code = 200;
        sprintf(pRestponse->response_body, "Population:%d\nCapital:%s\nCurrency:%s", 60808000, "Rome", "EUR");

    }
    else if (strcmp("England", country) == 0) {
        pRestponse->response_code = 200;
        sprintf(pRestponse->response_body, "Population:%d\nCapital:%s\nCurrency:%s", 53012456, "London", "GBP");

    }
    else {
        pRestponse->response_code = 404;
    }


}


int main() {
    // Opening database
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_open("reservation.db", &db);

    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    }else{
        fprintf(stderr, "Opened database successfully\n");
    }

    struct MHD_Daemon *daemon;

    daemon = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY, PORT, NULL, NULL,
                              &answer_to_connection, NULL, MHD_OPTION_END);


    if (NULL == daemon) return 1;
    printf("~~ listening on %d\nWaiting for connections...\n", PORT);

    getchar();

    MHD_stop_daemon(daemon);
    sqlite3_close(db);

    return 0;
}
