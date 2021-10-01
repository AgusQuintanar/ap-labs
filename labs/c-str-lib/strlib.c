

int mystrlen(char *str){
    int len = 0;
    while (*str != '\0') {
        len++;
        str++;
    }
    return len;
}

char *mystradd(char *origin, char *addition){
    char *origin_end = origin + mystrlen(origin); // goes to the end of the origin str

    while (*addition != '\0') {
        *origin_end++ = *addition++;
    }

    *origin_end = '\0';
    return origin;
}

int mystrfind(char *origin, char *substr){
    int pOrigin = 0,
        pSubstr = 0,
        objective = mystrlen(substr),
        curr = 0;

    while (origin[pOrigin] != '\0') {
        curr = pOrigin;
        while (origin[pOrigin] == substr[pSubstr] && origin[pOrigin] != '\0') {
            pOrigin++;
            pSubstr++;
        }
        if (pSubstr == objective) {
            return curr;
        }
        pOrigin++;
        pSubstr = 0;
    }
    return -1;
}
