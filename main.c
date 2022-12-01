#include "RadixTree.h"
#include <time.h>


int main()
{
    int wordSize;
    FILE* input = stdin;

    fscanf(input,"%d\n", &wordSize);
    RadixTrie* T = newTree(wordSize);

    int position;
    char inputChar;

    clock_t start, end;
    double execution_time;
    start = clock();

    char* command = malloc(sizeof(char)*wordSize+1);

    while(!feof(input))
    {
        position = 0;
        while ((inputChar = getchar()) != '\n' && !feof(input))
        {
            if(position < T -> wordSize)
            {
                command[position++] = (char) inputChar;
            }
        }
        command[position] = '\0';
        insert(T, command, T->wordSize);
    }


    end = clock();

    printPreorder(T->root);

    execution_time = ((double)(end - start))/CLOCKS_PER_SEC;
    printf("The program inserted %d words in %f\n", T->currentSize, execution_time);

    freeTrie(T->root);
    free(T);
    free(command);
}