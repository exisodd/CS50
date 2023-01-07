#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int index(char c);

int main(int argc, string argv[])
{
    int total = 0;
    for (int i = 0; i < argc; i++)
    {
        total++;
    }
    if (total != 2) {
        printf("Program must use exactly 1 command line argument.\n");
        return 1;
    }

    string key = argv[1];
    int n = strlen(key);
    char keyu[26];
    char keyl[26];

    // 26 length-key validation
    if (n != 26)
    {
        printf("Key must be of length 26.\n");
        return 1;
    }

    // Alpha key validation
    for (int i = 0; i < n; i++)
    {
        if (isalpha(key[i]) == 0)
        {
            printf("Key must only contain alphabetic characters.\n");
            return 1;
        }
    }

    // Duplicate character validation
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (i != j)
            {
                if (key[i] == key[j])
                {
                    printf("Key must not contain duplicate chracters.\n");
                    return 1;
                }
            }
        }
    }

    // Make duplicate key into capital letters
    for (int i = 0; i < n; i++)
    {
        keyu[i] = toupper(key[i]);
    }

    // Make duplicate key into lowercase letters
    for (int i = 0; i < n; i++)
    {
        keyl[i] = tolower(key[i]);
    }

    // KEY IS VALID
    // Convert text into cipher text
    string text = get_string("plaintext: ");
    int length = strlen(text);
    printf("ciphertext: ");
    // char ciphertexts[length];
    for (int i = 0; i < length; i++)
    {
        if (isalpha(text[i]))
        {
            // If text alpha, then check if upper or lower
            int ind = index(text[i]);
            if (isupper(text[i]))
            {
                printf("%c", keyu[ind]);
            }
            else
            {
                printf("%c", keyl[ind]);
            }
        }
        // If char not alpha, insert into string
        else
        {
            printf("%c", text[i]);
        }
    }
    printf("\n");
    return 0;
}

int index(char c)
{
    // Returns index of character
    int i = c;
    if isupper(c)
    {
        i -= 65;
    }
    else
    {
        i -= 97;
    }
    return i;
}