#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define ALPHABETSIZE 64

struct Node
{
    char* label;
    int labelSize;
    int valid;
    struct Node* child[ALPHABETSIZE];
};

typedef struct Node TrieNode;

struct RadixTree
{
    TrieNode* root;
    int wordSize;
    int currentSize;
};

typedef struct RadixTree RadixTrie;

int findPointer(char c)
{
    if(c==45)
        return 0;
    else if(48<=c && c<=57)
        return c-47;
    else if(65<=c && c<=90)
        return c-54;
    else if(97<=c && c<=122)
        return c-59;
    else if(c==95)
        return 37;
    else
        return -1;
}

TrieNode* newNode(char* label, int labelSize)
{
    char* BLOCK = malloc(sizeof(TrieNode) + labelSize * sizeof(char) + 1);
    TrieNode* newNode = (TrieNode*) &BLOCK[0];
    newNode->label = &BLOCK[sizeof(TrieNode)];
    memcpy(newNode->label, label, labelSize * sizeof(char) );
    newNode->label[labelSize] = '\0';

    newNode->labelSize = labelSize;
    newNode->valid = 1;

    memset(newNode->child, 0, sizeof(TrieNode*) * ALPHABETSIZE);

    return newNode;
}

RadixTrie* newTree(int wordSize)
{
    RadixTrie* T = malloc(sizeof(RadixTrie));
    T->root = newNode("", 0);
    T->currentSize = 0;
    T->wordSize = wordSize;
    return T;
}

int matchingChars(char* string, int strlen, TrieNode* node)
{
    int matches = 0;
    int minLength = 0;

    if(node->labelSize >= strlen)
    {
        minLength = strlen;
    }
    else if(node->labelSize < strlen)
    {
        minLength = node->labelSize;
    }

    if(minLength > 0)
    {
        for(int i = 0; i < minLength; i++)
        {
            if(string[i] == node->label[i])
            {
                matches++;
            }
            else
            {
                return matches;
            }
        }
    }
    return matches;
}

char* substring(char* original, int start, int length)
{
    char* substring = malloc(length + 1);
    memcpy(substring, &original[start], length);
    substring[length] = '\0';

    return substring;
}

void insertProcess(char* string, int strlen, TrieNode* node, RadixTrie* T)
{
    int matches = matchingChars(string, strlen, node);
    if((matches == 0) || (node == T->root) || ((matches > 0) && (matches < strlen) && (matches >= node->labelSize)))
    {
        int inserted = 0;

        int newPartLength = strlen - matches;
        char* newWordPart = substring(string, matches, newPartLength);

        if(node->child[findPointer(newWordPart[0])])
        {
            if(node->child[findPointer(newWordPart[0])]->label[0] == newWordPart[0])
            {
                inserted = 1;
                insertProcess(newWordPart, newPartLength, node->child[findPointer(newWordPart[0])], T);
            }
        }
        if(inserted == 0)
        {
            node->child[findPointer(newWordPart[0])] = newNode(newWordPart, newPartLength);
        }
        free(newWordPart);
    }
    else if(matches < strlen)
    {
        int commonRootLength = matches;
        char* commonRoot = substring(string, 0, commonRootLength);

        int previousLevelLength = node->labelSize - matches;
        char* previousLevel = substring(node->label, matches, previousLevelLength);

        int newLabelLength = strlen - matches;
        char* newLabel = substring(string, matches, newLabelLength);

        memset(node->label, '\0', node->labelSize);
        memcpy(node->label, commonRoot, commonRootLength);
        node->labelSize = commonRootLength;

        TrieNode* newNodePrevLabel = newNode(previousLevel, previousLevelLength);

        memcpy(newNodePrevLabel->child, node->child, sizeof(TrieNode*)*ALPHABETSIZE);

        memset(node->child, 0, sizeof(TrieNode*) * ALPHABETSIZE);

        node->child[findPointer(newNodePrevLabel->label[0])] = newNodePrevLabel;

        node->child[findPointer(newLabel[0])] = newNode(newLabel, newLabelLength);

        free(commonRoot);
        free(previousLevel);
        free(newLabel);
    }
    else if(matches > node->labelSize)
    {
        int newLabelLength = strlen;
        char* newLabel = substring(node->label, node->labelSize, newLabelLength);
        node->child[findPointer(newLabel[0])] = newNode(newLabel, newLabelLength);
    }
}

void freeTrie(TrieNode* node)
{
    if(!node)
    {
        return;
    }

    for(int i = 0; i < ALPHABETSIZE; i++)
    {
        freeTrie(node->child[i]);
    }

    free(node);
}

void insert(RadixTrie* T, char* string, int len)
{
    insertProcess(string, len, T->root, T);
    T->currentSize++;
}

void printPreorder(TrieNode* node)
{
    if (node == NULL)
    {
        return;
    }

    // Display node value
    printf("%s\n", node->label);
    for (int i = 0; i < ALPHABETSIZE; ++i)
    {
        if(node->child[i])
        {
            printPreorder(node->child[i]);
        }
    }
}