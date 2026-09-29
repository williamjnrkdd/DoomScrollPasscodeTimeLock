#include <stdio.h>
#include <stdlib.h>

#include "qrencode.h"

#define SCALE 10
#define BORDER 4


int code_to_qrcode_image(
    const char *code_text,
    unsigned char **pixels,
    int *width,
    int *height
) {
    if (!code_text || !pixels || !width || !height) {
        return EXIT_FAILURE;
    }

    *pixels = NULL;
    *width = 0;
    *height = 0;

    /*
     * Generate QR matrix.
     */
    QRcode *qr = QRcode_encodeString(
        code_text,
        0,
        QR_ECLEVEL_M,
        QR_MODE_8,
        1
    );

    if (!qr) {
        fprintf(stderr, "QR generation failed\n");
        return EXIT_FAILURE;
    }

    int modules = qr->width;

    int image_modules =
        modules + (2 * BORDER);

    int image_width =
        image_modules * SCALE;

    int image_height =
        image_width;

    /*
     * One byte per pixel.
     *
     * 0   = black
     * 255 = white
     */
    size_t pixel_count =
        (size_t)image_width *
        (size_t)image_height;

    unsigned char *image =
        malloc(pixel_count);

    if (!image) {
        fprintf(stderr, "Unable to allocate QR image\n");

        QRcode_free(qr);

        return EXIT_FAILURE;
    }

    /*
     * Start completely white.
     */
    for (size_t i = 0; i < pixel_count; i++) {
        image[i] = 255;
    }

    /*
     * Render QR modules.
     */
    for (int y = 0; y < image_height; y++) {

        int module_y =
            (y / SCALE) - BORDER;

        for (int x = 0; x < image_width; x++) {

            int module_x =
                (x / SCALE) - BORDER;

            if (module_x < 0 ||
                module_x >= modules ||
                module_y < 0 ||
                module_y >= modules) {

                continue;
            }

            unsigned char qr_pixel =
                qr->data[
                    module_y * modules +
                    module_x
                ];

            if (qr_pixel & 1) {

                image[
                    (size_t)y *
                    (size_t)image_width +
                    (size_t)x
                ] = 0;
            }
        }
    }

    QRcode_free(qr);

    *pixels = image;
    *width = image_width;
    *height = image_height;

    return EXIT_SUCCESS;
}