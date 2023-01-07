#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cs50.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 2)
    {
        printf("Program requires 1 command-line argument.\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");

    if (file == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *image = fopen("000.jpg", "w");
    BYTE buffer[512];
    int num = 0;

    // Read all blocks of bytes in card
    while (fread(buffer, 1, 512, file) == 512)
    {
        // Check if block is start of JPEG signature
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // 512 Block confirmed to be beginning of the JPEG file
            // Make JPEG name

            // If first JPEG found,
            if (num == 0)
            {
                fwrite(buffer, 512, 1, image);
            }
            // second or more+ JPEG
            else
            {
                // Close already opened image
                fclose(image);
                char name[8];
                sprintf(name, "%03i.jpg", num);
                image = fopen(name, "w");
                // Write to new file first block of bytes
                fwrite(buffer, 512, 1, image);
            }

            num++;
        }
        else
        {
            // Continue writing to JPEG if start of file has been found
            fwrite(buffer, 512, 1, image);
        }
    }

    fclose(image);
    fclose(file);
}