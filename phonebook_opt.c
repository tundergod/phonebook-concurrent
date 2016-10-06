#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "phonebook_opt.h"
#include "debug.h"
#include "tpool.h"

entry *findName(char lastname[], entry *pHead)
{
    size_t len = strlen(lastname);
    while (pHead != NULL) {
        if (strncasecmp(lastname, pHead->lastName, len) == 0
                && (pHead->lastName[len] == '\n' ||
                    pHead->lastName[len] == '\0')) {
            pHead->lastName = (char *) malloc(sizeof(char) *
                                              MAX_LAST_NAME_SIZE);
            memset(pHead->lastName, '\0', MAX_LAST_NAME_SIZE);
            strcpy(pHead->lastName, lastname);
            pHead->dtl = (pdetail) malloc(sizeof(detail));
            return pHead;
        }
        dprintf("find string = %s\n", pHead->lastName);
        pHead = pHead->pNext;
    }
    return NULL;
}

append_a *new_append_a(char *ptr, char *eptr, int ntd,
                       entry *start)
{
    append_a *app = (append_a *) malloc(sizeof(append_a));

    app->ptr = ptr;
    app->eptr = eptr;
    app->nthread = ntd;
    app->entryStart = start;

    app->pHead = (app->pLast = app->entryStart);
    return app;
}


void append(void *arg)
{
    int count = 0;
    append_a *app = (append_a *) arg;
    entry *j = app->entryStart;
    for (char *i = app->ptr; i < app->eptr; i += MAX_LAST_NAME_SIZE * app->nthread,j += app->nthread,count++) {
        app->pLast->pNext = j;
        app->pLast = app->pLast->pNext;

        app->pLast->lastName = i;
        app->pLast->pNext = NULL;
    }
}

void show_entry(entry *pHead)
{
    while (pHead) {
        printf("%s\n", pHead->lastName);
        pHead = pHead->pNext;
    }
}
