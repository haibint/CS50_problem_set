/**
 * Copies a BMP piece by piece, just because.
 */

// Progress: resize horizontally was successful, but once I try to resize vertically, the image get wrong.

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize n infile outfile\n");
        return 1;
    }

    // remember filenames
    int n = atoi(argv[1]);
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;   //one padding is just one byte.

    //update the outfile's BITMAPFILEHEADER
    BITMAPFILEHEADER new_bf;
    BITMAPINFOHEADER new_bi;

    //I will need to inherite all the properties from old Bitmap headers.

    new_bi.biSize = bi.biSize; // this is a fixed value of the size of bitmap header infomation size.
    new_bi.biWidth = bi.biWidth * n;
    // create new_padding
    int new_padding = (4 - (new_bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    printf("new_padding=%d\n", new_padding );

    new_bi.biHeight = bi.biHeight * n;  // we are not going to change the height for now.
    new_bi.biPlanes = bi.biPlanes;
    new_bi.biBitCount = bi.biBitCount;
    new_bi.biCompression = bi.biCompression;
    new_bi.biSizeImage = ((-new_bi.biHeight)*new_bi.biWidth*3) + (new_padding*(-new_bi.biHeight)); // we are not going to change the height for now.
    new_bi.biXPelsPerMeter = bi.biXPelsPerMeter;
    new_bi.biYPelsPerMeter = bi.biYPelsPerMeter;
    new_bi.biClrUsed = bi.biClrUsed;
    new_bi.biClrImportant = bi.biClrImportant;

    // determine padding for scanlines

    new_bf.bfType = bf.bfType;
    new_bf.bfSize = 54 + new_bi.biSizeImage;
    new_bf.bfReserved1 = bf.bfReserved1;
    new_bf.bfReserved2 = bf.bfReserved2;
    new_bf.bfOffBits = bf.bfOffBits;

    printf("bfType_old=%d bfType_new=%d\n",  bf.bfType, new_bf.bfType);
    printf("bfsize_old=%d bfsize_new=%d\n",  bf.bfSize, new_bf.bfSize);
    printf("bfReserved1_old=%d bfReserved1_new=%d\n",  bf.bfReserved1, new_bf.bfReserved1);
    printf("bfReserved2_old=%d bfReserved2_new=%d\n",  bf.bfReserved2, new_bf.bfReserved2);
    printf("bfOffBits_old=%d bfOffBits_new=%d\n",  bf.bfOffBits, new_bf.bfOffBits);

    printf("bisize_old=%d bisize_new=%d\n", bi.biSize, new_bi.biSize);
    printf("biWidth_old=%d biWidth_new=%d\n", bi.biWidth, new_bi.biWidth);
    printf("biHeight_old=%d biHeight_new=%d\n", bi.biHeight, new_bi.biHeight);
    printf("biPlanes_old=%d biPlanes_new=%d\n", bi.biPlanes, new_bi.biPlanes);
    printf("biBitCount_old=%d biBitCount_new=%d\n", bi.biBitCount, new_bi.biBitCount);
    printf("biCompression_old=%d biCompression_new=%d\n", bi.biCompression, new_bi.biCompression);
    printf("biSizeImage_old=%d biSizeImage_new=%d\n", bi.biSizeImage, new_bi.biSizeImage);
    printf("biXPelsPerMeter_old=%d biXPelsPerMeter_new=%d\n", bi.biXPelsPerMeter, new_bi.biXPelsPerMeter);
    printf("biYPelsPerMeter_old=%d biYPelsPerMeter_new=%d\n", bi.biYPelsPerMeter, new_bi.biYPelsPerMeter);
    printf("biClrUsed_old=%d biClrUsed_new=%d\n", bi.biClrUsed, new_bi.biClrUsed);
    printf("biClrImportant_old=%d biClrImportant_new=%d\n", bi.biClrImportant, new_bi.biClrImportant);




    // write outfile's BITMAPFILEHEADER
    fwrite(&new_bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&new_bi, sizeof(BITMAPINFOHEADER), 1, outptr);


    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
      	printf("NEW LINE\n");

        // iterate over pixels in scanline
        for (int j = 0; j < bi.biWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

	    printf( "%hhu %hhu %hhu -- ", triple.rgbtRed, triple.rgbtGreen, triple.rgbtBlue );

            //for each pixel in a row, write RGB triple to outfile, repeat n times.
            for (int m = 0; m < n; m++)
            {
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            }
        }

        // skip over padding(from the original image), if any
        // fseek(inptr, padding, SEEK_CUR);

	printf( "New padding: %d\n", new_padding );

        // then add it back (to demonstrate how)
        for (int k = 0; k < new_padding; k++)   /* A.M. */
        {
	  printf( "0/");
	  fputc(0x00, outptr);
        }
	printf("\n");

	printf("SECOND\n");

        // by now, the cursor is at the end of a scanline. you need to write the same line for n times. I want the cursor in fread to go back.
        for (int l=0; l<n-1; l++)
        {
            fseek(inptr, -bi.biWidth*sizeof(RGBTRIPLE), SEEK_CUR);
            for (int j = 0; j < bi.biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;
                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

		printf( "%hhu %hhu %hhu -- ", triple.rgbtRed, triple.rgbtGreen, triple.rgbtBlue );

                //for each pixel in a row, write RGB triple to outfile, repeat n times.
                for (int m = 0; m < n; m++)
                {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
	    }

	    for (int k = 0; k < new_padding; k++)
                {
		  printf( "0.");
		  fputc(0x00, outptr);
                }
		printf("\n");
        }

        fseek(inptr, padding, SEEK_CUR);
    }
    printf("%i\n", padding);
    printf("%i\n", new_padding);
    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
