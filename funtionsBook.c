// Taiatu Iulian Marius - 312CB
#include "trie.h"
#include "book.h"

// Allocate memory for a book
TBook *allocBook(char *book_name, char *author_name, int rating, int no_of_pages)
{
    TBook *pointer_to_book = (TBook *) malloc(sizeof(TBook));
    pointer_to_book->name = (char *) malloc(BOOK_NAME_LENGTH * sizeof(char));
    pointer_to_book->author = (char *) malloc(AUTHOR_NAME_LENGTH * sizeof(char));

    strcpy(pointer_to_book->name, book_name);
    strcpy(pointer_to_book->author, author_name);
    pointer_to_book->rating = rating;
    pointer_to_book->no_of_pages = no_of_pages;

    return pointer_to_book;
}

// Deallocate memory for a book
void freeBook(TBook *book)
{
    free(book->name);
    free(book->author);
    free(book);
}

// Display a book
void showBook(TBook book, FILE *output_file)
{
    fprintf(output_file, "%s, %s, %d, %d\n", book.name, book.author, book.rating, book.no_of_pages);
}
