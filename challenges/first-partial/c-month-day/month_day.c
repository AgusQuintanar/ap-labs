#include <stdio.h>
#include <stdlib.h>

void month_day(int year, int yearday, int *pmonth, int *pday) {
    if (year < 0) {
        printf("The year provided is not valid.\n");
        exit(0);
    }

    int isLeap = year%4 == 0 && year%100 != 0 || year%400 == 0;

    if (yearday > (isLeap ? 366 : 365)) {
        printf("The year day provided is not valid.\n");
        exit(0);
    }

    char daysPerMonth[12] = {
        31, (isLeap ? 29 : 28), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
    };

    int daysPassed = 0,
        month = 0;

    while (month++ < 12) {
        if (yearday <= daysPassed + daysPerMonth[month-1]) {
            *pday = yearday - daysPassed;
            *pmonth = month;
            return;
        }
        daysPassed += daysPerMonth[month-1];
    }
}

void printFormattedDate(int month, int day, int year) {
    char *month_abbrvs[12] = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun","Jul", "Aug", "Sept","Oct", "Nov", "Dec"
    };
    printf("%s %02d, %d\n", month_abbrvs[month-1], day, year);
}


int main(int argc, char **argv) {
    int year,
        yearday,
        pmonth,
        pday;

    if (argc != 3) {
        printf("Input provided is not valid.\n");
        printf("Expected usage: \"./month_day <year> <yearday>\"\n");
        printf("Ej. \"./month_day 2019 33\"\n");
        return -1;
    }

    char *end;

    year = strtol(argv[1], &end, 10);
    if (*end != '\0') {
        printf("Year value should be of <int> type.\n");
        return -1;
    }

    yearday = strtol(argv[2], &end, 10);
    if (*end != '\0') {
        printf("YearDay value should be of <int> type.\n");
        return -1;
    }
 
    
    month_day(year, yearday, &pmonth, &pday);
    printFormattedDate(pmonth, pday, year);
    return 0;
}
