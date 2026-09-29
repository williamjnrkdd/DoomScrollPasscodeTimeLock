#ifndef QRCODE_IMAGE_GENERATOR_H
#define QRCODE_IMAGE_GENERATOR_H

#include <stddef.h>

int code_to_qrcode_image(
    const char *code_text,
    unsigned char **pixels,
    int *width,
    int *height
);

#endif