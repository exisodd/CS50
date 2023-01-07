#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop over row
    for (int i = 0; i < height; i++)
    {
        // Loop over each cell, where image[i][j] is one pixel
        for (int j = 0; j < width; j++)
        {
            int gray = rintf((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtBlue = gray;
            image[i][j].rgbtGreen = gray;
            image[i][j].rgbtRed = gray;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop over row
    for (int i = 0; i < height; i++)
    {
        // Loop over each cell, where image[i][j] is one pixel
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // CREATE COPY ARRAY
    RGBTRIPLE copyx[height][width];
    // Loop over row
    for (int i = 0; i < height; i++)
    {
        // Loop over each cell, where image[i][j] is one pixel
        for (int j = 0; j < width; j++)
        {
            // CURRENT PIXEL = image[i][j]
            RGBTRIPLE pixel;
            int totalRed = 0;
            int totalBlue = 0;
            int totalGreen = 0;
            int total = 0;
            // If over image border
            for (int k = -1; k < 2; k++)
            {
                for (int l = -1; l < 2; l++)
                {
                    if (i + k < 0 || i + k > height - 1)
                    {
                        continue;
                    }
                    if (j + l < 0 || j + l > width - 1)
                    {
                        continue;
                    }
                    totalRed += image[i + k][j + l].rgbtRed;
                    totalGreen += image[i + k][j + l].rgbtGreen;
                    totalBlue += image[i + k][j + l].rgbtBlue;
                    total++;
                }
            }
            // Average the colors
            pixel.rgbtRed = (int)round(totalRed * 1.0 / total);
            pixel.rgbtGreen = (int)round(totalGreen * 1.0 / total);
            pixel.rgbtBlue = (int)round(totalBlue * 1.0 / total);
            copyx[i][j] = pixel;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = copyx[i][j];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // CREATE COPY ARRAY
    RGBTRIPLE G[height][width];
    int GxK[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}};
    int GyK[3][3] = {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}};

    // Loop over row
    for (int i = 0; i < height; i++)
    {
        // Loop over each cell, where image[i][j] is one pixel
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE pixel;
            // Initialize RGB values
            int Red = 0;
            int Blue = 0;
            int Green = 0;
            int GxRed = 0;
            int GxBlue = 0;
            int GxGreen = 0;
            int GyRed = 0;
            int GyBlue = 0;
            int GyGreen = 0;

            // Loop over 3x3 neighbords
            for (int k = -1; k < 2; k++)
            {
                for (int l = -1; l < 2; l++)
                {
                    // If over image border
                    if (i + k < 0 || i + k > height - 1)
                    {
                        continue;
                    }
                    if (j + l < 0 || j + l > width - 1)
                    {
                        continue;
                    }
                    // Compute Sum of each RGB value
                    int tmultiplierx = GxK[k + 1][l + 1];
                    int tmultipliery = GyK[k + 1][l + 1];
                    int tpixelred = image[i + k][j + l].rgbtRed;
                    int tpixelgreen = image[i + k][j + l].rgbtGreen;
                    int tpixelblue = image[i + k][j + l].rgbtBlue;
                    GxRed += (GxK[k + 1][l + 1] * tpixelred);
                    GxGreen += (GxK[k + 1][l + 1] * tpixelgreen);
                    GxBlue += (GxK[k + 1][l + 1] * tpixelblue);

                    GyRed += (GyK[k + 1][l + 1] * tpixelred);
                    GyGreen += (GyK[k + 1][l + 1] * tpixelgreen);
                    GyBlue += (GyK[k + 1][l + 1] * tpixelblue);
                }
            }

            // Final values for RGB
            int red = round(sqrt(pow(GxRed, 2) + pow(GyRed, 2)));
            int green = round(sqrt(pow(GxGreen, 2) + pow(GyGreen, 2)));
            int blue = round(sqrt(pow(GxBlue, 2) + pow(GyBlue, 2)));
            // If values greater then 255, cap at 255

            // red
            if (red > 255)
            {
                pixel.rgbtRed = 255;
            }
            else
            {
                pixel.rgbtRed = red;
            }

            // green
            if (green > 255)
            {
                pixel.rgbtGreen = 255;
            }
            else
            {
                pixel.rgbtGreen = green;
            }

            // blue
            if (blue > 255)
            {
                pixel.rgbtBlue = 255;
            }
            else
            {
                pixel.rgbtBlue = blue;
            }

            // Average the colors
            G[i][j] = pixel;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = G[i][j];
        }
    }
    return;
}
