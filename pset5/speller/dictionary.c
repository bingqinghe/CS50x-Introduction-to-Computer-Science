// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <cs50.h>
#include <stdlib.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 30;

// Hash table
node *table[N];
int total = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // TODO
    node *sample = table[hash(word)];
    // case insensitive
    if (strcasecmp(sample->word, word) == 0)
    {
        return true;
    }

    while (sample->next)
    {
        sample = sample->next;
        if (strcasecmp(sample->word, word) == 0)
        {
            return true;
        }
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO
    int index = (int) tolower(word[0]) - 97;
    return index;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // TODO
    // open dictionary read only
    FILE *file = fopen(dictionary, "r");
    char *word_pointer = malloc(LENGTH);
    if (word_pointer == NULL)
    {
        return false;
    }

    //keep reading
    while (fscanf(file, "%s", word_pointer) != EOF)
    {
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            return false;
        }

        // copy value & update
        strcpy(new_node->word, word_pointer);
        total++;
        new_node->next = table[hash(word_pointer)];
        table[hash(word_pointer)] = new_node;
    }
    fclose(file);
    free(word_pointer);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return total;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // TODO
    node *current;
    node *temp;

    for (int i = 0; i < N; i++)
    {
        if (table[i] == NULL)
        {
            continue;
        }
        current = table[i];
        temp = current;

        // free one by one
        while (current->next)
        {
            current = current->next;
            free(temp);
            temp = current;
        }
        free(current);
    }
    return true;
}
