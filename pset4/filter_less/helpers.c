#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    float gray;
    // r=g=b
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            gray = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.00);
            // change the r g b value to the same
            image[i][j].rgbtBlue = gray;
            image[i][j].rgbtGreen = gray;
            image[i][j].rgbtRed = gray;
        }
    }
    return;
}

// Convert image to sepia
// sepiaRed = .393 * originalRed + .769 * originalGreen + .189 * originalBlue
// sepiaGreen = .349 * originalRed + .686 * originalGreen + .168 * originalBlue
// sepiaBlue = .272 * originalRed + .534 * originalGreen + .131 * originalBlue
// make sure RGB is 0-255
int changeLimit(int rgb)
{
    if (rgb > 255)
    {
        rgb = 255;
    }
    return rgb;
}

void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // use as temp
    int sepiaRed;
    int sepiaGreen;
    int sepiaBlue;
    // change r g b by the formular
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            sepiaRed = changeLimit(round(.393 * image[i][j].rgbtRed + .769 * image[i][j].rgbtGreen + .189 * image[i][j].rgbtBlue));
            sepiaGreen = changeLimit(round(.349 * image[i][j].rgbtRed + .686 * image[i][j].rgbtGreen + .168 * image[i][j].rgbtBlue));
            sepiaBlue = changeLimit(round(.272 * image[i][j].rgbtRed + .534 * image[i][j].rgbtGreen + .131 * image[i][j].rgbtBlue));

            image[i][j].rgbtRed = sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtBlue = sepiaBlue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // use swap
    int temp[3];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            // left
            temp[0] = image[i][j].rgbtRed;
            temp[1] = image[i][j].rgbtGreen;
            temp[2] = image[i][j].rgbtBlue;
            // left = right // left + right = width - 1
            image[i][j].rgbtRed = image[i][width - 1 - j].rgbtRed;
            image[i][j].rgbtGreen = image[i][width - 1 - j].rgbtGreen;
            image[i][j].rgbtBlue = image[i][width - 1 - j].rgbtBlue;
            // right = original-left
            image[i][width - 1 - j].rgbtRed = temp[0];
            image[i][width - 1 - j].rgbtGreen = temp[1];
            image[i][width - 1 - j].rgbtBlue = temp[2];
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // copy of original for each pixel
    RGBTRIPLE temp[height][width];
    // count sum or surround
    int sumRed = 0;
    int sumGreen = 0;
    int sumBlue = 0;
    // average box count;
    float counter = 0.00;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // reset the variables each pixel
            sumRed = 0;
            sumGreen = 0;
            sumBlue = 0;
            counter = 0.00;

            // up self down
            for (int x = -1; x < 2; x++)
            {
                // out of height boundry
                if (i + x < 0 || i + x > height - 1)
                {
                    continue;
                }
                // left self right
                for (int y = -1; y < 2; y++)
                {
                    // out of width boundry
                    if (j + y < 0 || j + y > width - 1)
                    {
                        continue;
                    }

                    sumRed += image[i + x][j + y].rgbtRed;
                    sumGreen += image[i + x][j + y].rgbtGreen;
                    sumBlue += image[i + x][j + y].rgbtBlue;
                    counter++;
                }
            }

            // average
            temp[i][j].rgbtRed = round(sumRed / counter);
            temp[i][j].rgbtGreen = round(sumGreen / counter);
            temp[i][j].rgbtBlue = round(sumBlue / counter);
        }
    }
    // output
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = temp[i][j].rgbtRed;
            image[i][j].rgbtGreen = temp[i][j].rgbtGreen;
            image[i][j].rgbtBlue = temp[i][j].rgbtBlue;
        }
    }
    return;
}
