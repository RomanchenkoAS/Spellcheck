// Implements a dictionary's functionality

//Usage: ./speller [DICTIONARY] text

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;  //sizeof(node) = 56 bytes

// Pick a number of buckets in hash table
const unsigned int N = 71549;

// Hash table
node *table[N];

// Length of the dictionary
int dict_length = 0;

// Recursive search function that goes down through the given branch
bool search(node *root, const char *word)
{
    if (strcmp(root->word, word) == 0)
    {
        return true; // Found the word
    }

    // If word is not found but it is the end of the branch
    if (root->next == NULL)
    {
        return false; // Recursion base case
    }
    else
    {
        // Else we go deeper
        if (search(root->next, word))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    char tmp[LENGTH + 1];

    // Erase the temp string just in case
    memset(tmp, '\0', sizeof(tmp));

    //lowercase the word first
    for (int i = 0; i < strlen(word); i++)
    {
        tmp[i] = tolower(word[i]);
    }

    int hash_val = hash(tmp);
    return search(table[hash_val], tmp);
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    /*x int cur_value = 0, value = 0;

        for (int i = 0; i < strlen(word); i++)
        {
            if (word[i] == 39)
            {
                cur_value = 26;
                value = value + 26;
            }
            else
            {
                cur_value = word[i] - 97;
            }
            //used Data Structures youtube course of Dr. Rob Edwards for this line
            //value = (50 * value + cur_value) % N;
            value = (value << 2) ^ cur_value;
            // printf("letter %c equals to %i\n", word[i], cur_value);
        }

        return value % N;*/
    //THE ONE form GPT that gives 0.073
    // int hash = 0;

    // for (int i = 0; word[i] != '\0'; i++)
    // {
    //     //ChatGPT advised me this one
    //     hash = (hash * prime) + word[i];


    // }
    // return (hash % N);

    //yet another function from GPT (djb2 by Dan Bernstein)
    unsigned long hash = 5381;
    int c = 0;

    while ((c = *word++))
    {
        hash = ((hash << 5) + hash) + c;  /* hash * 33 + c */
    }

    return (hash % N);
}

void insert(node *root, const char *new_word)
{
    if (root->word[0] == 0) //if current position is empty
    {
        strcpy(root->word, new_word); //fill it
        root->next = NULL;
    }
    else
    {
        if (root->next == NULL) //if next block doesnt exist, create it
        {
            node *next_node = malloc(sizeof(node));
            root->next = next_node;
            root->next->word[0] = 0; //Input a space as indication of valid node
            insert(root->next, new_word);
        }
        else
        {
            insert(root->next, new_word); //if next pointer is not null go there
        }
    }
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // allocate memory for hash table
    for (int i = 0; i < N; i++)
    {
        table[i] = malloc(sizeof(node));
        table[i]->word[0] = 0;
        table[i]->next = NULL;
    }


    // Open dictionary file
    FILE *dict = fopen(dictionary, "r");

    // Handle possible error
    if (dict == NULL)
    {
        printf("Invalid dictionary file\n");
        return false;
    }

    // Create a temporary var for storing a word and allocate memory
    char word[LENGTH + 1]; //stores every word


    // unsigned long tophash = 0;       //maximum hash and according word for tesing
    // char topword[LENGTH + 1];

    // For each string till the end of the file
    while (fgets(word, LENGTH + 1, dict))
    {
        dict_length++;  // Dictionary size

        unsigned long hash_value = 0;

        //delete \n symbol if there is one (replace to \0)
        if (word[strlen(word) - 1] == 10)
        {
            word[strlen(word) - 1] = 0;
        }

        hash_value = hash(word); //Calculate value of the word

        insert(table[hash_value], word); //put the word into the table

        //for testing - finding highest hash
        // if (hash_value > tophash)
        // {
        //     strcpy(topword , word); //NULLIFY BEFORE COPYING
        //     tophash = hash_value;
        // }

        // Nullify the string
        // for (int i = strlen(word); i > 0; i--)
        // {
        //     word[i - 1] = 0;
        // }
        memset(word, '\0', sizeof(word));
    }

    // Close the file and free any allocated memory
    fclose(dict);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return dict_length;
}
void unload_branch(node *root)
{
    node *tmp;

    tmp = root->next;

    free(root);

    if (tmp != NULL)
    {
        unload_branch(tmp);
    }
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        unload_branch(table[i]);
    }
    return true;
}