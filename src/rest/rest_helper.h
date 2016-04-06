#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>






struct string {
    char *ptr;
    size_t len;
};

struct WriteThis {
    const char *readptr;
    long sizeleft;
};

static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *userp) {
    struct WriteThis *pooh = (struct WriteThis *) userp;

    if (size * nmemb < 1)
        return 0;

    if (pooh->sizeleft) {
        *(char *) ptr = pooh->readptr[0]; /* copy one single byte */
        pooh->readptr++;                 /* advance pointer */
        pooh->sizeleft--;                /* less data left */
        return 1;                        /* we return 1 byte at a time! */
    }

    return 0;                          /* no more data left to deliver */
}


void init_string(struct string *s) {
    s->len = 0;
    s->ptr = malloc(s->len + 1);
    if (s->ptr == NULL) {
        fprintf(stderr, "malloc() failed\n");
        exit(EXIT_FAILURE);
    }
    s->ptr[0] = '\0';
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s) {
    size_t new_len = s->len + size * nmemb;
    s->ptr = realloc(s->ptr, new_len + 1);
    if (s->ptr == NULL) {
        fprintf(stderr, "realloc() failed\n");
        exit(EXIT_FAILURE);
    }
    memcpy(s->ptr + s->len, ptr, size * nmemb);
    s->ptr[new_len] = '\0';
    s->len = new_len;

    return size * nmemb;
}


void rest_call(const char* url, const char* verb, const char* body,long* status, char* response){

    CURL *curl;
    CURLcode res;


    struct WriteThis pooh;

    pooh.readptr = body;
    pooh.sizeleft = (long) strlen(body);

    curl = curl_easy_init();
    if (curl) {
        struct string s;
        init_string(&s);

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);

        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, verb);
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
        curl_easy_setopt(curl, CURLOPT_READDATA, &pooh);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
            exit(1);
        }
        else {
            long http_code = 0;
            curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);


            strcpy(response,s.ptr);
            *status=http_code;
            free(s.ptr);
        }


        /* always cleanup */
        curl_easy_cleanup(curl);

    }

}