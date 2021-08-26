// Taiatu Iulian Marius - 312CB
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BOOK_NAME_LENGTH 50
#define AUTHOR_NAME_LENGTH 40

#ifndef _BOOK_
#define _BOOK_

typedef struct book
{
    char *name;
    char *author;
    int rating;
    int no_of_pages;
}TBook;

TBook *allocBook(char *book_name, char *author_name, int rating, int no_of_pages);
void freeBook(TBook *book);
void showBook(TBook book, FILE *output_file);

#endif
