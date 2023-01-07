// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>

#include "dictionary.h"

int word_count = 0;

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 675;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    int i = hash(word);

    // Loop over linked list at location of hash table
    for (node *tmp = table[i]->next; tmp != NULL; tmp = tmp->next)
    {
        // Word has been found
        if (strcasecmp(word, tmp->word) == 0)
        {
            return true;
        }
    }

    // No word has been found, so return false
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    int total = 0;
    int length = strlen(word);
    if (length == 1)
    {
        total += (toupper(word[0]) - 'A');
    }
    else
    {
        for (int i = 0; i < 2; i++)
        {
            int power = pow(26, 1 - i);
            total += (toupper(word[i]) - 'A') * power;
        }
    }

    return total;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO

    // Set all table buckets to point to NULL
    // Set node for all table buckets (first node)
    node *tab = NULL;
    for (int i = 0; i < 26; i++)
    {
        for (int j = 0; j < 26; j++)
        {
            tab = malloc(sizeof(node));
            if (tab == NULL)
            {
                return false;
            }
            // Bucket's word
            char tword[3];
            tword[0] = i + 'A';
            tword[1] = j + 'A';
            tword[2] = '\0';
            // Store word into first node
            strcpy(tab->word, tword);
            // Set pointer of first node to NULL
            tab->next = NULL;
            // Set table's node to that pointer
            table[i * 26 + j] = tab;
        }
    }

    // Free tab
    free(tab);

    // Open dictionary
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        return false;
    }

    char word[LENGTH + 1];
    // Create word buffer to load words into on loop
    while (fscanf(dict, "%s", word) != EOF)
    {
        // Find index of word in hash table
        int i = hash(word);

        // Create new node for each word
        node *n = malloc(sizeof(node));

        if (n == NULL)
        {
            return false;
        }

        // Copy word into n->word
        strcpy(n->word, word);
        // Point node to the first word in the linked list
        n->next = table[i]->next;

        // Index into hash table and insert word at the start of the linked list
        table[i]->next = n;
        word_count++;
    }
    
    // Close dictionary
    fclose(dict);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return word_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    // Loop over all buckets
    for (int i = 0; i < N; i++)
    {
        // Loop over linked list at bucket location
        node *tmp = NULL;
        // Set cursor to point to the same as the first node in table
        node *cursor = table[i];
        // On each loop, Set temp to cursor, move cursor to next before freeing tmp
        while (cursor != NULL)
        {
            tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
    }
    return true;
}
