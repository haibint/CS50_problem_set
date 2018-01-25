// Problem, the first picture is cut off when skipping the gabage info before any jpg picture.

#include <stdio.h>
#include <stdlib.h>

#define BLOCK_SIZE 512


int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "This program takes only one command line arguement.\n");
        return 1;
    }

    // remember filenames
    char* infile = argv[1];

    // open input file
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    unsigned char block[BLOCK_SIZE];
    int picture_count = 0;
    int first_jpg = 1;

    while (fread(block, BLOCK_SIZE, 1, inptr) == 1)
    {
        // current working out file.
        char filename[8];   // declare filename array.
        sprintf(filename, "%03d.jpg", picture_count);  // change filename according piture_count.
        FILE* outptr = fopen(filename, "a"); // change output file pointer.
        if (outptr == NULL)
        {
            fprintf(stderr, "Could not open %s.\n", filename);
            return 2;
        }

        if (block[0] == 0xff && block[1] == 0xd8 && block[2] == 0xff && (block[3] & 0xe0) == 0xe0) // if it looks like it is a jpeg.
        {
            if (first_jpg == 1)
            {
                fwrite(&block, BLOCK_SIZE, 1, outptr);
                first_jpg =0;
                fclose(outptr);
                continue;
            }
            fclose(outptr);
            picture_count++;
            sprintf(filename, "%03d.jpg", picture_count);
            outptr = fopen(filename, "w");    // Here we change the pointer to a new file.
            fwrite(&block, BLOCK_SIZE, 1, outptr);
            fclose(outptr);
        }
        else if (first_jpg == 0)
        {
            fwrite(&block, BLOCK_SIZE, 1, outptr);
            fclose(outptr);
        }
        else if (first_jpg == 1)
        {
            fclose(outptr);
        }
    }
    fclose(inptr);
    return 0;
}