# Trie data structure implemented in C

## Taiatu Iulian Marius - 312CB

For implementing this homework, I've used:
    -> T1: trie for holding the books
        The end of the key is a pointer to a struct which contains the informations about the book
    -> T2: trie for holding the authors
        The end of the key is a pointer to a trie of books similar to T1

`ADD_BOOK` - Insert a new key in trie
    I allocated a new book
    If the book isn't in the T1 trie
        Insert it in T1 using a recursive function (DFS)
    Else
        Deallocate the book
    Search the author in T2
    If the author doesn't exist in T2
        Create his trie of books
        Insert the book in his trie of books
        Insert the author in T2
    Else
        Search the book in his trie of books
        If the book doesn't exist
            Insert the book in his trie of books

`SEARCH_BOOK` - Search books based on the name or on the prefix
    If we need to search based on the prefix (autocomplete)
        Search the prefix in T1
        If the prefix doesn't exist
            No books found
        Else
            Display maximum 3 keys
            I used a similar function of displaying the trie, 
            but this time the input was the ending node of the prefix
    If we need to search based on the full name ok the book
        Search in T1 the book with the name 'book_name'

`LIST_AUTHOR` - Search authors based on the name or on the prefix
    The same algorithm used in function 'search_book', but
    this time I searched in T2 (for authors)
        
`SEARCH_BY_AUTHOR` - Search an author or a specific book of an author (based on the name or on the prefix of the book)
    If we need to search the author based on the prefix
        The same as above
    Else if we need to search the author based on the full name
        If we need to search the books of the 'author_name' based on the prefix of the book
            Search the author in T2 and return a pointer to the trie of his books and display maximum 3 books
            Search in the trie of his books using the prefix of the book
        Else if we need to search the books of the 'author_name' based on the full name of the book
            Search the author in T2 and return a pointer to the trie of his books
            Search in the trie of his books using the name of the book

`DELETE_BOOK` - Delete a book from the trie
    Search the book in the T1 trie
    If the book doesn't exist
        Display that and continue with the next command
    Else If the book doesn't exist
        Extract the 'author_name' from the 'book_info'
        Remove the book from T1, deleting the information
        Search the author in T2
            Remove the book from his trie of books, without deleting the information
            If the author no longer has books
                Remove the author from T2

Thank you! :)
