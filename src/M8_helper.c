
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "M8_helper.h"

/*
    Reads a file and returns the content as a string.

    TODO: Make more generic with malloc size.
          (Take another argument)
*/
char *M8_read_file(char *filename) {
    int ch;
    FILE *fp;
    char *s = malloc(2*256*2*sizeof(char)); // Max 256 instructions * 2 hex chars * approx spaces
    int index = 0;

    fp = fopen(filename,"r");

    assert(fp!=NULL); // TODO: Change to nice error message

    while( ( ch = fgetc(fp) ) != EOF ){
        s[index++] = (char) ch;
    }
    s[index] = '\0';

    fclose(fp);
    return s;

}