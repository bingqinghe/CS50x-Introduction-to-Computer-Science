#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // error detection
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image");
        return 1;
    }

    // successfully opening the memory card
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        fprintf(stderr, "Open file error");
        return 1;
    }

    FILE *image;
    char name[5];
    unsigned char *bf = malloc(512);
    int counter = 0;

    while (fread(bf, 512, 1, file))
    {
        // check JPEG file
        // 0xff 0xd8 0xff 0xe0-0xef
        if (bf[0] == 0xff && bf[1] == 0xd8 && bf[2] == 0xff && (bf[3] & 0xf0) == 0xe0)
        {
            // close the previous
            if (counter > 0)
            {
                fclose(image);
            }
            // new image file ###.jpg
            sprintf(name, "%03d.jpg", counter);
            // open the new image file // write
            image = fopen(name, "w");
            // check successfully open
            if (image == NULL)
            {
                fclose(file);
                free(bf); // free the memory
                fprintf(stderr, "Create image error");
                return 1;
            }

            counter++;
        }
        // successfully opened, write
        if (counter >  0)
        {
            fwrite(bf, 512, 1, image);
        }
    }

    // finish the loop
    // close the image file -> close the memory card -> free all the memory
    fclose(image);
    fclose(file);
    free(bf);
    return 0;
}
