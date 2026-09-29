#include <stdio.h>
#include <stdlib.h>

#include <SDL.h>

#include "qrcode_display.h"

#define WINDOW_WIDTH  700
#define WINDOW_HEIGHT 700


int display_qrcode(
    const unsigned char *pixels,
    int width,
    int height
) {
    if (!pixels || width <= 0 || height <= 0) {
        fprintf(stderr, "Invalid QR image\n");
        return EXIT_FAILURE;
    }

    printf("Starting SDL...\n");

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(
            stderr,
            "SDL_Init failed: %s\n",
            SDL_GetError()
        );

        return EXIT_FAILURE;
    }

    printf(
        "SDL video driver: %s\n",
        SDL_GetCurrentVideoDriver()
    );

    int num_drivers = SDL_GetNumVideoDrivers();

    printf("Compiled-in video drivers:\n");

    for (int i = 0; i < num_drivers; i++) {
        printf("  %s\n", SDL_GetVideoDriver(i));
}

    /*
     * Create window.
     */
    SDL_Window *window =
        SDL_CreateWindow(
            "DoomScroll QR Unlock",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            0
        );

    if (!window) {
        fprintf(
            stderr,
            "SDL_CreateWindow failed: %s\n",
            SDL_GetError()
        );

        SDL_Quit();
        return EXIT_FAILURE;
    }

    printf("SDL window created.\n");

    /*
     * Software renderer.
     *
     * We don't need GPU acceleration for a QR code.
     */
    SDL_Renderer *renderer =
        SDL_CreateRenderer(
            window,
            -1,
            SDL_RENDERER_SOFTWARE
        );

    if (!renderer) {
        fprintf(
            stderr,
            "SDL_CreateRenderer failed: %s\n",
            SDL_GetError()
        );

        SDL_DestroyWindow(window);
        SDL_Quit();

        return EXIT_FAILURE;
    }

    printf("SDL renderer created.\n");

    /*
     * Make sure nearest-neighbour scaling is used.
     * This is important for QR codes.
     */
    SDL_SetHint(
        SDL_HINT_RENDER_SCALE_QUALITY,
        "0"
    );

    /*
     * Create an 8-bit indexed surface using our
     * existing pixel buffer.
     *
     * 0 = black
     * 1 = white
     */
    SDL_Surface *surface =
        SDL_CreateRGBSurfaceWithFormatFrom(
            (void *)pixels,
            width,
            height,
            8,
            width,
            SDL_PIXELFORMAT_INDEX8
        );

    if (!surface) {
        fprintf(
            stderr,
            "SDL_CreateRGBSurfaceWithFormatFrom failed: %s\n",
            SDL_GetError()
        );

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();

        return EXIT_FAILURE;
    }

    /*
     * Create black/white palette.
     */
    SDL_Color colors[2];

    colors[0].r = 0;
    colors[0].g = 0;
    colors[0].b = 0;
    colors[0].a = 255;

    colors[1].r = 255;
    colors[1].g = 255;
    colors[1].b = 255;
    colors[1].a = 255;

    if (SDL_SetPaletteColors(
            surface->format->palette,
            colors,
            0,
            2
        ) != 0) {

        fprintf(
            stderr,
            "SDL_SetPaletteColors failed: %s\n",
            SDL_GetError()
        );

        SDL_FreeSurface(surface);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();

        return EXIT_FAILURE;
    }

    printf("SDL surface created.\n");

    /*
     * Convert surface to texture.
     */
    SDL_Texture *texture =
        SDL_CreateTextureFromSurface(
            renderer,
            surface
        );

    if (!texture) {
        fprintf(
            stderr,
            "SDL_CreateTextureFromSurface failed: %s\n",
            SDL_GetError()
        );

        SDL_FreeSurface(surface);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();

        return EXIT_FAILURE;
    }

    printf("SDL texture created.\n");

    /*
     * We don't need the surface anymore.
     * SDL_CreateTextureFromSurface() has copied
     * the pixel data into the texture.
     */
    SDL_FreeSurface(surface);

    /*
     * Calculate QR display rectangle.
     */
    int display_size =
        WINDOW_WIDTH < WINDOW_HEIGHT
            ? WINDOW_WIDTH
            : WINDOW_HEIGHT;

    display_size -= 40;

    SDL_Rect destination;

    destination.x =
        (WINDOW_WIDTH - display_size) / 2;

    destination.y =
        (WINDOW_HEIGHT - display_size) / 2;

    destination.w = display_size;
    destination.h = display_size;

    printf(
        "QR image: %dx%d\n",
        width,
        height
    );

    printf(
        "Displaying QR. Close the window or press ESC.\n"
    );

    /*
     * Main window loop.
     */
    int running = 1;

    while (running) {

        SDL_Event event;

        while (SDL_PollEvent(&event)) {

            switch (event.type) {

                case SDL_QUIT:
                    running = 0;
                    break;

                case SDL_KEYDOWN:

                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        running = 0;
                    }

                    break;

                default:
                    break;
            }
        }

        /*
         * White background.
         */
        SDL_SetRenderDrawColor(
            renderer,
            255,
            255,
            255,
            255
        );

        SDL_RenderClear(renderer);

        /*
         * Draw QR.
         */
        if (SDL_RenderCopy(
                renderer,
                texture,
                NULL,
                &destination
            ) != 0) {

            fprintf(
                stderr,
                "SDL_RenderCopy failed: %s\n",
                SDL_GetError()
            );

            break;
        }

        SDL_RenderPresent(renderer);

        /*
         * Don't hammer the CPU.
         */
        SDL_Delay(16);
    }

    printf("Closing QR window.\n");

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return EXIT_SUCCESS;
}