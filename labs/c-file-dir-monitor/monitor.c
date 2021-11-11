#define _XOPEN_SOURCE 500
#define __USE_XOPEN_EXTENDED 1


#include <stdio.h>
#include "logger.h"
#include <sys/inotify.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ftw.h>
#include "logger.h"
#include <inttypes.h>


#define EVENT_SIZE      (sizeof (struct inotify_event))
#define MAX_NUM_EVENTS  (1024 * (EVENT_SIZE + 16))

#define SIZE 1024

struct Item* getItem(int key);
char* get(int key);
void put(int key, const char* value);

int inotify;
char *events[MAX_NUM_EVENTS];

char* renamedEvent[2];

int monitor();
int handleEvent(struct inotify_event *event);
int createDirRecord(const char *fpath, const struct stat *sb, int tflag);

int createDirRecord(const char *fpath, const struct stat *sb, int tflag) {
    if (tflag == FTW_D) { // && ftwbuf->level <= 2) { // only directories in the first two levels
        int wd = inotify_add_watch(
            inotify, fpath, IN_CREATE | IN_DELETE | IN_MOVED_FROM | IN_MOVED_TO
        );
        put(wd, fpath);
    }
    return 0;
}

int monitor(){
    while(1) {
        int totalLen = read(inotify, events, MAX_NUM_EVENTS);
        for (int i=0; i<totalLen;) {
            struct inotify_event *event = ( struct inotify_event * ) &events[i];     
            handleEvent(event);
            i += EVENT_SIZE + event->len;
        }
    }
    return 0;
}

int handleEvent(struct inotify_event *event){
    char *path = get(event->wd);
    // printf("mask [ %"PRIu32" ]\n", event->mask);

    if(event->mask & IN_CREATE) {
        if(event->mask & IN_ISDIR) {
            infof("[Directory Create] - %s/%s\n", path, event->name);
            char *dir = malloc(strlen(path) + strlen(event->name) + 1);
            strcpy(dir, path);
            strcat(dir, "/");
            strcat(dir, event->name);
            createDirRecord(dir, NULL, FTW_D);
        }
        else infof("[File Create] -: %s/%s\n", path, event->name);
    }
    else if(event->mask & IN_DELETE) {
        if(event->mask & IN_ISDIR) {
            infof("[Directory Removal] - %s/%s\n", path, event->name);
            inotify_rm_watch(inotify, event->wd);
        }
        else {
            infof("[File Removal] -: %s/%s\n", path, event->name);
        }
    }
    else if (event->mask & IN_MOVED_FROM) {
        renamedEvent[0] = event->name;
        renamedEvent[1] = path;
    }
    else if (event->mask & IN_MOVED_TO) {
        if(strcmp(renamedEvent[1], path)==0){
            infof("[File Rename] - %s/%s -> %s/%s\n", path, renamedEvent[0] , path, event->name);
        }
        renamedEvent[0] = NULL;
        renamedEvent[1] = NULL;
    }
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2){
        errorf("Error: You need to specify a valid dir.\nExample usage: ./monitor $HOME\n");
        return 1;
    }

    inotify = inotify_init();

    if (ftw(argv[1], createDirRecord, 20) == -1){
        errorf("An error ocurrer while executing nftw().\n");
        return 1;
    }

    monitor();
    return 0;
}

struct Item {
    struct Item *next;
    long key;
    char* value;
};

static struct Item *dictionary[SIZE];

unsigned hash(long key) {
    return (key + 101) % SIZE;
}

char* get(int key) {
    struct Item *item = getItem(key);
    return (item == NULL) ? NULL : item->value; 
}

struct Item* getItem(int key) {
    struct Item *item;
    for (item = dictionary[hash(key)]; item != NULL; item = item->next) {
        if (key == item->key) {
            return item; 
        }
    }
    return NULL; 
}

void put(int key, const char *value){ 
    struct Item *item;
    unsigned hashval;
    if ((item = getItem(key)) == NULL) { 
        item = malloc(sizeof(*item));
        hashval = hash(key);
        item->next = dictionary[hashval];
        item->key = key;
        item->value = malloc(strlen(value)+1);
        strcpy(item->value, value);
        dictionary[hashval] = item;
    }
    else {
        free(item->value);
        item->value = malloc(strlen(value)+1);
        strcpy(item->value, value);
    }
}