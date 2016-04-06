//
// Created by nherbaut on 30/04/15.
//

#ifndef PROJECT_HELPER_H
#define PROJECT_HELPER_H

#include <string.h>
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <regex.h>

/*!
this structure contains an array of regex matched groups
*/
struct re_groups{
    char** data;
    size_t size;
};

/**
this function returns matched groups for a regex given the buffer
**/
struct re_groups get_regex_groups(const char* regexchar, const char *buffer) {
    regex_t regex;
    int i;
    char** groups = NULL;

    if (regcomp(&regex, regexchar, REG_EXTENDED) != 0) {
        perror("failed to compile regex");
    }
    int nsub = regex.re_nsub;
    regmatch_t matchptr[nsub];

    int err;
    if (err = regexec(&regex, buffer, nsub + 1, matchptr, 0) != 0) {

        regfree(&regex);

    } else {

        groups = malloc((regex.re_nsub + 1) * sizeof(void *));
        memset(groups, 0, regex.re_nsub + 1);

        for (i = 1; i <= nsub; i++) {
            groups[i - 1] = malloc(
                    (matchptr[i].rm_eo - matchptr[i].rm_so + 2) * sizeof(char));
            memset(groups[i - 1], 0, matchptr[i].rm_eo - matchptr[i].rm_so + 2);
            memcpy(groups[i - 1], buffer + matchptr[i].rm_so,
                   matchptr[i].rm_eo - matchptr[i].rm_so);
        }
    }
    regfree(&regex);
    struct re_groups res;
    res.data = groups;
    res.size = nsub;

    return res;

}

/*!
this helper function free a re_groups structure for you
*/
void free_groups(struct re_groups agroups) {
    int i = 0;
    if (agroups.data == NULL)
        return;
    for (i = 0; i < agroups.size; i++) {
        free(agroups.data[i]);
    }

    free(agroups.data);
}


#endif //PROJECT_HELPER_H
