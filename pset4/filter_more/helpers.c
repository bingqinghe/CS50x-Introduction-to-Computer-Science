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

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // copy for calculate
    RGBTRIPLE copy[height][width];
    int Gx[3][3] =
    {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };
    int Gy[3][3] =
    {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };
    float GxRed = 0;
    float GxGreen = 0;
    float GxBlue = 0;
    float GyRed = 0;
    float GyGreen = 0;
    float GyBlue = 0;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // reset
            GxRed = 0;
            GxGreen = 0;
            GxBlue = 0;
            GyRed = 0;
            GyGreen = 0;
            GyBlue = 0;
            // initial coord
            int row[] = {i - 1, i, i + 1};
            int col[] = {j - 1, j, j + 1};
            // up down
            for (int x = 0; x < 3; x++)
            {
                // left right
                for (int y = 0; y < 3; y++)
                {
                    int currentRow = row[x];
                    int currentCol = col[y];
                    if (currentRow >= 0 && currentRow < height && currentCol >= 0 && currentCol < width)
                    {
                        // Gx * Gx // Gy * Gy
                        RGBTRIPLE temp = image[currentRow][currentCol];
                        GxRed += Gx[x][y] * temp.rgbtRed;
                        GxGreen += Gx[x][y] * temp.rgbtGreen;
                        GxBlue += Gx[x][y] * temp.rgbtBlue;
                        GyRed += Gy[x][y] * temp.rgbtRed;
                        GyGreen += Gy[x][y] * temp.rgbtGreen;
                        GyBlue += Gy[x][y] * temp.rgbtBlue;
                    }
                }
            }
            // do the sqrt
            int sqrtRed = round(sqrt(GxRed * GxRed + GyRed * GyRed));
            int sqrtGreen = round(sqrt(GxGreen * GxGreen + GyGreen * GyGreen));
            int sqrtBlue = round(sqrt(GxBlue * GxBlue + GyBlue * GyBlue));
            // rgb should be 0-255
            copy[i][j].rgbtRed = sqrtRed > 255 ? 255 : sqrtRed;
            copy[i][j].rgbtGreen = sqrtGreen > 255 ? 255 : sqrtGreen;
            copy[i][j].rgbtBlue = sqrtBlue > 255 ? 255 : sqrtBlue;
        }
    }

    //copy back to the original
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = copy[i][j].rgbtRed;
            image[i][j].rgbtGreen = copy[i][j].rgbtGreen;
            image[i][j].rgbtBlue = copy[i][j].rgbtBlue;
        }
    }
    return;
}
