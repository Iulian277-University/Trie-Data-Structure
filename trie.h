// Taiatu Iulian Marius - 312CB
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ALPHABET_SIZE 70

extern const char alphabet[ALPHABET_SIZE];

#ifndef _TRIE_
#define _TRIE_
    
typedef struct trieNode
{
    struct trieNode *children[ALPHABET_SIZE];
    bool isEndOfWord;
    void *info;
}TrieNode;

TrieNode *initTrie();
TrieNode *newNode();
TrieNode *removeKeyFromTrie(TrieNode **root, char *key_to_remove, int level, int type, bool delete_information);

bool isEmpty(TrieNode *root);
bool isLeafNode(TrieNode *root);

int getIndexFromAlphabet(char ch_to_find);

void destoryBooksTrie(TrieNode **root, bool delete_info);
void destoryAuthorsTrie(TrieNode **root, bool delete_info);

void insertInTrieRecursively(TrieNode *root, char *key_to_insert, int i, void *info);
void insertInTrie(TrieNode *root, char *key_to_insert, void *info);

void displayTrieRecursively(TrieNode *root, char *str, int level, FILE *output_file);
void displayTrie(TrieNode *root, FILE *output_file);

void displayEndOfPrefixRecursively(TrieNode *root, char *str, int level, char *prefix, int *display_cnt, FILE *output_file);
void displayEndOfPrefix(TrieNode *root, char *prefix, FILE *output_file);

void *searchInTrieRecursively(TrieNode *root, char *key_to_search, int i);
void *searchInTrie(TrieNode *root, char *key_to_search);

void *searchPrefixRecursively(TrieNode *root, char *key_to_search, int i);
void *searchPrefix(TrieNode *root, char *key_to_search);

#endif
