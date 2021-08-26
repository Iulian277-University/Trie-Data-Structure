// Taiatu Iulian Marius - 312CB
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trie.h"
#include "book.h"

#define LINE_LENGTH 100

int main(int argc, char *argv[])
{
    char *input_file_name = argv[1];
    char *output_file_name = argv[2];

    FILE *input_file = NULL;
    input_file = fopen(input_file_name, "r");
    if(input_file == NULL) // Couldn't open the input file
        return -1;

    FILE *output_file = NULL;
    output_file = fopen(output_file_name, "w");
    if(output_file == NULL) // Couldn't open the output file
    {
        fclose(input_file);
        return -1;
    }

    char *line = NULL;
    line = (char *) calloc(LINE_LENGTH, sizeof(char));
    if(line == NULL) // Allocation error
    {
        fclose(input_file);
        fclose(output_file);
        return -1;
    }

    char *command = NULL;
    command = (char *) calloc(LINE_LENGTH, sizeof(char));
    if(command == NULL) // Allocation error
    {
        fclose(input_file);
        fclose(output_file);
        free(line);
        return -1;
    }

    TrieNode *T1, *T2;
    T1 = newNode(), T2 = newNode(); // Books and Authors

    // Read from input file line by line
    while(!feof(input_file))
    {
        if(fgets(line, LINE_LENGTH, input_file) == NULL)
            break;

        if(line[strlen(line) - 1] == '\n')
            line[strlen(line) - 1] = '\0';

        char *token = strtok(line, " ");
        strcpy(command, token);

        if(strcmp(command, "add_book") == 0)
        {
            char *book_name = (char *) malloc(BOOK_NAME_LENGTH * sizeof(char));
            token = strtok(NULL, ":");
            strcpy(book_name, token);

            char *author_name = (char * ) malloc(AUTHOR_NAME_LENGTH * sizeof(char));
            token = strtok(NULL, ":");
            strcpy(author_name, token);

            int rating = -1;
            token = strtok(NULL, ":");
            rating = atoi(token);

            int no_of_pages = -1;
            token = strtok(NULL, ":");
            no_of_pages = atoi(token);

            TBook *book_info = allocBook(book_name, author_name, rating, no_of_pages);
            // Search the book in T1
            if(searchInTrie(T1, book_name) == NULL)
                insertInTrie(T1, book_name, (void *) book_info);
            else // The book already exists in T1
            {
                freeBook(book_info);
                free(book_name);
                free(author_name);
                continue;
            }

            void *books_trie_author = searchInTrie(T2, author_name);
            // Search the author in T2
            if(books_trie_author == NULL)
            {
                TrieNode *author_books = newNode();
                insertInTrie(author_books, book_name, (void *) book_info);
                insertInTrie(T2, author_name, (void *) author_books);
            }
            else // The author already exists in T2
            {
                // Search the book in the trie of books specific to that author
                if(searchInTrie(books_trie_author, book_name) == NULL)
                    insertInTrie(books_trie_author, book_name, (void *) book_info);
            }

            free(book_name);
            free(author_name);
        }
        else if(strcmp(command, "search_book") == 0)
        {
            char *book_name = (char *) malloc(BOOK_NAME_LENGTH * sizeof(char));
            token = strtok(NULL, ":");
            strcpy(book_name, token);

            // Search books based on the prefix
            if(book_name[strlen(book_name) - 1] == '~')
            {
                book_name[strlen(book_name) - 1]  = '\0';

                void *prefix_ending_node = searchPrefix(T1, book_name);
                if(prefix_ending_node == NULL) // No prefix found
                    fprintf(output_file, "Nicio carte gasita.\n");
                else // Display maximum 3 keys
                    displayEndOfPrefix(prefix_ending_node, book_name, output_file);
            }
            else
            {
                // Search in T1 based on the full name of the book
                void *book_found = searchInTrie(T1, book_name);
                if(book_found)
                {
                    fprintf(output_file, "Informatii recomandare: ");
                    showBook(*((TBook *) book_found), output_file);
                }
                else
                    fprintf(output_file, "Cartea %s nu exista in recomandarile tale.\n", book_name);
            }
                
            free(book_name);
        }
        else if(strcmp(command, "list_author") == 0)
        {
            char *author_name = (char *) malloc(BOOK_NAME_LENGTH * sizeof(char));
            token = strtok(NULL, ":");
            strcpy(author_name, token);

            // Search authors based on the prefix
            if(author_name[strlen(author_name) - 1] == '~')
            {
                author_name[strlen(author_name) - 1]  = '\0';

                void *prefix_ending_node = searchPrefix(T2, author_name);
                if(prefix_ending_node == NULL) // No prefix found
                    fprintf(output_file, "Niciun autor gasit.\n");
                else // Display maximum 3 keys
                    displayEndOfPrefix(prefix_ending_node, author_name, output_file);
            }
            else
            {
                // Search in T2 based on the full name of the author
                void *author_books = searchInTrie(T2, author_name);
                if(author_books == NULL)
                    fprintf(output_file, "Autorul %s nu face parte din recomandarile tale.\n", author_name);
                else
                    displayTrie(author_books, output_file);
            }

            free(author_name);
        }
        else if(strcmp(command, "search_by_author") == 0)
        {
            char *author_name = (char *) malloc(AUTHOR_NAME_LENGTH * sizeof(char));
            token = strtok(NULL, ":");
            strcpy(author_name, token);

            // Search authors based on the prefix
            if(author_name[strlen(author_name) - 1] == '~')
            {
                author_name[strlen(author_name) - 1]  = '\0';

                void *prefix_ending_node = searchPrefix(T2, author_name);
                if(prefix_ending_node == NULL) // No prefix found
                    fprintf(output_file, "Niciun autor gasit.\n");
                else // Display maximum 3 keys
                    displayEndOfPrefix(prefix_ending_node, author_name, output_file);
            }
            else
            {
                // Search based on the full name of the author
                char *book_name = (char *) malloc(BOOK_NAME_LENGTH * sizeof(char));
                token = strtok(NULL, ":");
                strcpy(book_name, token);

                // Search books of the 'author_name' based on the prefix
                if(book_name[strlen(book_name) - 1] == '~')
                {
                    book_name[strlen(book_name) - 1]  = '\0';
                    
                    void *author_books = searchInTrie(T2, author_name);
                    // 'author_books': NULL or Pointer to author's books trie
                    if(author_books == NULL) // The author doesn't have trie books
                        fprintf(output_file, "Autorul %s nu face parte din recomandarile tale.\n", author_name);
                    else
                    {
                        // Search books based on the prefix
                        void *prefix_ending_book_node = searchPrefix(author_books, book_name);
                        if(prefix_ending_book_node == NULL)
                            fprintf(output_file, "Nicio carte gasita.\n");
                        else // Display maximum 3 keys
                            displayEndOfPrefix(prefix_ending_book_node, book_name, output_file);
                    }
                }
                else
                {
                    // Search books of the 'author_name' based on the full name of the book
                    void *author_books = searchInTrie(T2, author_name);
                    if(author_books == NULL) // The author doesn't have trie books
                        fprintf(output_file, "Autorul %s nu face parte din recomandarile tale.\n", author_name);
                    else
                    {
                        void *book_found = searchInTrie(author_books, book_name);
                        if(book_found == NULL) // The book doesn't exist
                            fprintf(output_file, "Cartea %s nu exista in recomandarile tale.\n", book_name);
                        else
                        {
                            // Display the book information
                            fprintf(output_file, "Informatii recomandare: ");
                            showBook(*((TBook *) book_found), output_file);
                        }
                    }
                }

                free(book_name);
            }
            
            free(author_name);
        }
        else if(strcmp(command, "delete_book") == 0)
        {
            char *book_name = (char *) malloc(BOOK_NAME_LENGTH * sizeof(char));
            token = strtok(NULL, ":");
            strcpy(book_name, token);

            // Search the book in the T1 trie
            void *book_found = searchInTrie(T1, book_name);
            if(book_found == NULL) // The book doesn't exist
                fprintf(output_file, "Cartea %s nu exista in recomandarile tale.\n", book_name);
            else
            {
                // Extract the author_name from the book info
                char *author_name = (char *) malloc(AUTHOR_NAME_LENGTH * sizeof(char));
                strcpy(author_name, ((TBook *) book_found)->author);

                // Remove the book from T1, deleting the infromation
                T1 = removeKeyFromTrie(&T1, book_name, 0, 1, true);

                // Search the author in T2
                void *author_books = searchInTrie(T2, author_name);
                if(author_books != NULL)
                {
                    // Remove the book from the author's books trie, without deleting the information
                    author_books = removeKeyFromTrie((TrieNode **) &author_books, book_name, 0, 1, false);
                    if(author_books == NULL) // The author no longer has books
                        T2 = removeKeyFromTrie(&T2, author_name, 0, 2, false); // Remove the author from T2
                }
                free(author_name);
            }
            free(book_name);
        }
        
    }

    destoryBooksTrie(&T1, true);
    destoryAuthorsTrie(&T2, false);

    fclose(input_file);
    fclose(output_file);
    fclose(stdin);
    fclose(stdout);
    fclose(stderr);

    free(line);
    free(command);

    return 0;
}
