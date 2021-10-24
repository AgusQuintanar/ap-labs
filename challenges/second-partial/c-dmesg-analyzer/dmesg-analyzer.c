#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define REPORT_FILE "report.txt"

#define MAX_LOG_TYPES 10000
#define MAX_LOGS 100

#define MAX_LOG_TYPE_SIZE 1000
#define MAX_LOG_DESCRIPTION_SIZE 1000

void analizeLog(char *logFile, char *report);
char* substring(const char *src, int pos, int len);
struct LogType getLogType(char *line);
unsigned long hash(char *str);
void createReport();
void insert(char *name, char *log);

struct Log {
   char *type,
        *description;
};

struct LogType {
   char *name,
        logs[MAX_LOGS*MAX_LOG_TYPE_SIZE];
};


struct LogType* logsTable[MAX_LOG_TYPES];


unsigned long hash(char *str) {
    // djb2 by Dan Bernstein (http://www.cse.yorku.ca/~oz/hash.html) 
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash %= MAX_LOG_TYPES;
}

void insert(char *name, char *log) {
    int hashIndex = hash(name);

    while(logsTable[hashIndex] != NULL) {
        if(strcmp(logsTable[hashIndex]->name, name) == 0) { // if key already exists
            strcat(logsTable[hashIndex]->logs, "\t");
            strcat(logsTable[hashIndex]->logs, log);
            return;
        }
        ++hashIndex;
        hashIndex %= MAX_LOG_TYPES;
    }
	
    struct LogType *logType = (struct LogType*) malloc(sizeof(struct LogType));
    logType->name = name;
    strcat(logType->logs, "\t"); 
    strcat(logType->logs, log); 

    logsTable[hashIndex] = logType;
}


char* substring(const char *src, int pos, int len) {
    char *dest = (char*)malloc(sizeof(char) * (len + 1));
    strncpy(dest, (src + pos), len);
    return dest;
}

struct Log getLog(char *line) {
    char currChar;
    int logTypeStartIndex = 0, 
        logTypeLen = 0, 
        logTypeStartFound = 0,
        colonFound = 0;

    for (size_t i=0; i<strlen(line); i++) {
        currChar = line[i];

        if (currChar == ']') {
            logTypeStartFound = 1;
        }
        else if (logTypeStartFound) {
            if (currChar != ' ') {
                logTypeStartIndex = i;
                for (size_t j=i; j<strlen(line); j++) {
                    currChar = line[j];
                    if (currChar == ':') colonFound = 1;
                    else if (currChar == ' ' && colonFound) break;
                    logTypeLen++;
                }
                break;
            }
        }
    }

    struct Log log;
    if (colonFound == 0) { // no log type found
        log.type = "General";
        log.description = line;
    }
    else {
        log.type = substring(line, logTypeStartIndex, logTypeLen-1);

        char *timestamp = substring(line, 0, logTypeStartIndex-1),
             *message = substring(line, logTypeStartIndex+logTypeLen, strlen(line));

        log.description = (char*) malloc(MAX_LOG_DESCRIPTION_SIZE);

        strcpy(log.description, timestamp);
        strcat(log.description, message);
    }

    return log;
} 

void createReport(char *report) {
    int fptr = open(report, O_CREAT | O_WRONLY, 0600);

    if (fptr < 0) {
        printf("Error: tcould not create DMESG report\n");
        return;
    }

    for(int i = 0; i<MAX_LOG_TYPES; i++) {
        if(logsTable[i] != NULL){
            const int SIZE = MAX_LOG_TYPES;
            char log_type[SIZE];
            snprintf(log_type, SIZE, "%s: \n%s\n", logsTable[i]->name, logsTable[i]->logs);

            write(fptr, log_type, strlen(log_type));
        }
    }

    close(fptr);
}

void analizeLog(char *logFile, char *report) {
    printf("Generating Report from: [%s] log file\n", logFile);

    FILE *fptr;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    if ((fptr = fopen(logFile, "r")) == NULL) {
        printf("Error: file not found or cannot open\n");
        return;
    }

    while ((read = getline(&line, &len, fptr)) != -1) {
        if (strlen(line) == 0 || strcasecmp(line, "\n") == 0) continue;
        struct Log log = getLog(line);
        insert(log.type, log.description);
    }

    createReport(report);

    printf("Report is generated at: [%s]\n", report);
}

int main(int argc, char **argv) {
    if (argc < 2) {
	printf("Usage:./dmesg-analizer logfile.txt\n");
	return 1;
    }

    analizeLog(argv[1], REPORT_FILE);

    return 0;
}
