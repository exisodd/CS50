#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int num;
    do
    {
        num = get_int("Height: ");
    }
    while (num < 1 || num > 8);

    for (int i = 0; i < num; i++)
    {
        // printf("current i: %i", i);
        for (int k = num - 1; k >= 0; k--)
        {
            // printf("current k: %i", k);
            if (k <= i)
            {
                printf("#");
            }
            else
            {
                printf(" ");
            }

        }
        printf("  ");
        for (int j = 0; j < num; j++)
        {
            if (j <= i)
            {
                printf("#");
            }
            // else
            // {
            //     printf(" ");
            // }
        }
        printf("\n");
    }
}