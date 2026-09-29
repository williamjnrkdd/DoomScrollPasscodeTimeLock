#include <stdio.h>
#include <stdlib.h>

#include "qrencode.h"
#include "png.h"

#define SCALE 10       // pixels per QR module
#define BORDER 4       // quiet-zone modules

int code_to_qrcode_png(char* code_text, char* target_file){
    QRcode *qr = QRcode_encodeString(
        code_text,
        0,                  /* version: 0 = automatic */
        QR_ECLEVEL_M,      /* error correction */
        QR_MODE_8,
        1                   /* case sensitive */
    );

    if (!qr) {
        fprintf(stderr, "QR generation failed\n");
        return EXIT_FAILURE;
    }

    int modules = qr->width;
    int image_modules = modules + 2 * BORDER;

    int width = image_modules * SCALE;
    int height = width;

    FILE *fp = fopen(target_file, "wb");
    if (!fp) {
        perror(target_file);
        QRcode_free(qr);
        return EXIT_FAILURE;
    }

    png_structp png = png_create_write_struct(
        PNG_LIBPNG_VER_STRING,
        NULL,
        NULL,
        NULL
    );

    if (!png) {
        fclose(fp);
        QRcode_free(qr);
        return EXIT_FAILURE;
    }

     png_infop info = png_create_info_struct(png);

    if (!info) {
        png_destroy_write_struct(&png, NULL);
        fclose(fp);
        QRcode_free(qr);
        return EXIT_FAILURE;
    }

    if (setjmp(png_jmpbuf(png))) {
        png_destroy_write_struct(&png, &info);
        fclose(fp);
        QRcode_free(qr);
        return EXIT_FAILURE;
    }

    png_init_io(png, fp);

    png_set_IHDR(
        png,
        info,
        width,
        height,
        8,
        PNG_COLOR_TYPE_GRAY,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT
    );

    png_write_info(png, info);

    /* Allocate one row */
    png_bytep row = malloc(width);

    if (!row) {
        png_destroy_write_struct(&png, &info);
        fclose(fp);
        QRcode_free(qr);
        return EXIT_FAILURE;
    }

    for (int y = 0; y < height; y++) {

        int module_y = y / SCALE - BORDER;

        for (int x = 0; x < width; x++) {

            int module_x = x / SCALE - BORDER;

            unsigned char pixel = 255;   /* white */

            if (module_x >= 0 &&
                module_x < modules &&
                module_y >= 0 &&
                module_y < modules) {

                unsigned char qr_pixel =
                    qr->data[module_y * modules + module_x];

                /*
                 * libqrencode stores the module color
                 * in bit 0.
                 */
                if (qr_pixel & 1)
                    pixel = 0;           /* black */
            }

            row[x] = pixel;
        }

        png_write_row(png, row);
    }

    free(row);

    png_write_end(png, NULL);
    png_destroy_write_struct(&png, &info);

    fclose(fp);
    QRcode_free(qr);

    printf("Created %s (%dx%d)\n", target_file, width, height);

    return EXIT_SUCCESS;
}