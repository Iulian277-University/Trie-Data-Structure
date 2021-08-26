// Taiatu Iulian Marius - 312CB
#include "trie.h"
#include "book.h"

const char alphabet[ALPHABET_SIZE] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.-'?! ";

// Iterate through the alphabet and return the index of the character
int getIndexFromAlphabet(char ch_to_find)
{
    int index = -1;
    int i;
    for(i=0; i<strlen(alphabet); ++i)
    {
        if(ch_to_find == alphabet[i])
        {
            index = i;
            break;
        }
    } 
    return index;
}

// Check if a specific node has no children
bool isEmpty(TrieNode *node)
{
    int i;
    for(i = 0; i < ALPHABET_SIZE; ++i)
    {
        if(node->children[i])
            return false;
    }
    
    return true;
}

bool isLeafNode(TrieNode *root)
{
    return root->isEndOfWord != false;
}

// Create an empty trie
TrieNode *initTrie()
{
    return NULL;
}

// Create a node an set the struct members to NULL
TrieNode *newNode()
{
    TrieNode *auxNode = (TrieNode *) malloc(sizeof(TrieNode));
    if(auxNode == NULL)
        return NULL;

    auxNode->isEndOfWord = false;

    int i;
    for(i = 0; i < ALPHABET_SIZE; ++i)
        auxNode->children[i] = NULL;

    auxNode->info = NULL;

    return auxNode;
}

// Remove a key from the trie
TrieNode *removeKeyFromTrie(TrieNode **root, char *key_to_remove, int level, int type, bool delete_information)
{
    if((*root) == NULL)
        return NULL;

    if(level == strlen(key_to_remove))
    {
        if(isEmpty(*root))
        {
            destoryBooksTrie(root, delete_information);
            *root = NULL;
        }

        if(*root != NULL && (*root)->isEndOfWord == true)
        {
            if(type == 1 && delete_information == true)
                freeBook((TBook *) (*root)->info);
            (*root)->isEndOfWord = false;
        }
        return *root;
    }

    int index = getIndexFromAlphabet(key_to_remove[level]);

    if(index == -1)
        return NULL;

    (*root)->children[index] = removeKeyFromTrie(&(*root)->children[index], key_to_remove, level + 1, type, delete_information);

    if(isEmpty(*root) && (*root)->isEndOfWord == false)
    {
        if(type == 1)
            destoryBooksTrie(root, false);
        else if(type == 2)
            destoryAuthorsTrie(root, false);
            
        *root = NULL;
    }

    return *root;
}

// Delete the trie which contains books
void destoryBooksTrie(TrieNode **root, bool delete_info)
{
    if((*root) == NULL)
        return;

    // Base case
    if((*root)->isEndOfWord == true)
    {
        if(delete_info == true)
            freeBook(((TBook *) (*root)->info));
        (*root)->isEndOfWord = false;
    }

    int i;
    for(i = 0; i < ALPHABET_SIZE; ++i)
        destoryBooksTrie(&(*root)->children[i], delete_info);

    free(*root);
}

// Delete the trie which contains authors
void destoryAuthorsTrie(TrieNode **root, bool delete_info)
{
    if((*root) == NULL)
        return;

    // Base case
    if((*root)->isEndOfWord == true)
        destoryBooksTrie((TrieNode **) &((*root)->info), delete_info);

    int i;
    for(i = 0; i < ALPHABET_SIZE; ++i)
        destoryAuthorsTrie(&(*root)->children[i], delete_info);

    free(*root);
}

// Insert a key in the trie using recursion
void insertInTrieRecursively(TrieNode *root, char *key_to_insert, int i, void *info)
{
    if(i < strlen(key_to_insert))
    {
        int index = getIndexFromAlphabet(key_to_insert[i]);

        if(root->children[index] == NULL)
            root->children[index] = newNode();

        insertInTrieRecursively(root->children[index], key_to_insert, i + 1, info);
    }
    else
    {
        // Mark the end of the word
        root->isEndOfWord = true;
        // Set the info pointer specific to T1 (book struct) or T2 (author's books trie)
        root->info = info;
    }
}

// Function which calls the recursive function
void insertInTrie(TrieNode *root, char *key_to_insert, void *info)
{
    insertInTrieRecursively(root, key_to_insert, 0, info);
}

// Display the trie using recursion
void displayTrieRecursively(TrieNode *root, char str[], int level, FILE *output_file)
{
    if(isLeafNode(root))
    {
        str[level] = '\0';
        fprintf(output_file, "%s\n", str);
    }

    int i;
    for(i = 0; i < ALPHABET_SIZE; ++i)
    {
        if(root->children[i])
        {
            str[level] = alphabet[i];
            displayTrieRecursively(root->children[i], str, level+1, output_file);
        }
    }
}

// Function which calls the recursive function
void displayTrie(TrieNode *root, FILE *output_file)
{
    int level = 0;
    char str[ALPHABET_SIZE];

    displayTrieRecursively(root, str, level, output_file);
}

// Display the end of the word based on the last node of a prefix using recursion
void displayEndOfPrefixRecursively(TrieNode *root, char *str, int level, char *prefix, int *display_cnt, FILE *output_file)
{
    if(*display_cnt >= 3)
        return;

    if(isLeafNode(root))
    {
        (*display_cnt)++;
        fprintf(output_file, "%s", prefix);
        str[level] = '\0';
        fprintf(output_file, "%s\n", str);
    }

    int i;
    for(i = 0; i < ALPHABET_SIZE; ++i)
    {
        if(root->children[i])
        {
            str[level] = alphabet[i];
            displayEndOfPrefixRecursively(root->children[i], str, level+1, prefix, display_cnt, output_file);
        }
    }
}

// Function which calls the recursive function
void displayEndOfPrefix(TrieNode *root, char *prefix, FILE *output_file)
{
    int level = 0;
    int display_cnt = 0;
    char str[ALPHABET_SIZE];

    displayEndOfPrefixRecursively(root, str, level, prefix, &display_cnt, output_file);
}

// Search a specific key in the trie using recursion
void *searchInTrieRecursively(TrieNode *root, char *key_to_search, int i)
{
    if(root == NULL)
        return NULL;

    if((root->isEndOfWord == true) && (i == strlen(key_to_search)))
        return root->info;

    int index = getIndexFromAlphabet(key_to_search[i]);
    if(index == -1)
        return NULL;

    return searchInTrieRecursively(root->children[index], key_to_search, i+1);
}

// Function which calls the recursive function
void *searchInTrie(TrieNode *root, char *key_to_search)
{
    char *key_copy = key_to_search;
    return searchInTrieRecursively(root, key_copy, 0);
}

// Search a specific prefix in the trie using recursion
void *searchPrefixRecursively(TrieNode *root, char *prefix_to_search, int i)
{
    if(root == NULL)
        return NULL;

    if(i == strlen(prefix_to_search))
        return root; // Return the ending node of the prefix

    int index = getIndexFromAlphabet(prefix_to_search[i]);
    if(index == -1)
        return NULL;

    return searchPrefixRecursively(root->children[index], prefix_to_search, i+1);
}

// Function which calls the recursive function
void *searchPrefix(TrieNode *root, char *prefix_to_search)
{
    char *prefix_copy = prefix_to_search;
    return searchPrefixRecursively(root, prefix_copy, 0);
}
