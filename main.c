#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>


#define MY_T_K_MEANS_SIZE_IMPL
typedef u_char size_d_tim_;

#define T_K_MEANS_IMPL
#ifdef VISUALIZE
#define T_K_MEANS_RAYLIB_VISUALIZATION
#endif
#include "k_means.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define K 15

int modify_data(u_char *data, int *sets, int n, u_char *m, int dim);

#ifdef VISUALIZE

#include <raylib.h>
#define WIDTH 1920
#define HEIGHT 1080
#define FPS 60

Texture2D draw_image(u_char *data, int width, int height);

#endif


int main(int argc, char **argv) {
    char *in = NULL, *out = NULL;
    int opt;
    uint64_t k;

    while ((opt = getopt(argc, argv, "i:o:k:")) != -1) {
        switch (opt) {
            case 'i':
                in = optarg;
                break;

            case 'o':
                out = optarg;
                break;

            case 'k':
                if (atoi(optarg) < 0) {
                    perror("K must be a positive value!\n");
                    return -1;
                }
                k = (uint64_t) strtoul(optarg, 0, 10);
                break;

            default:
                fprintf(stderr, "Usage: %s [-k nclusters] [-i] <input file>", argv[0]);
                return -1;
        }
    }

    if (!in) return -1;
    if (!out) out = "out.png";

    if (!k) k = K;
    if (k > UINT_MAX) return -1;

    int width, height, n_channels;

    u_char *data = stbi_load(in, &width, &height, &n_channels, 0);
    if (!data) {
        perror("Couldn't read image!\n");
        stbi_failure_reason();
        return -1;
    }

    int *sets = malloc(width*height * sizeof(*sets));
    assert(sets);
    memset(sets, 0, width*height * sizeof(*sets));

    u_char *centroids = malloc(width*height*k * sizeof(*centroids));
    assert(centroids);

    gen_cents(data, width*height, centroids, k, n_channels);


#ifndef VISUALIZE

    while (1) {
        k_means(data, sets, width*height, centroids, k, n_channels);
        if (k_means_adjust(data, sets, width*height, centroids, k, n_channels)) break;
//        print_data(data, sets, width*height, n_channels);
    }

#endif

#ifdef VISUALIZE
    InitWindow(WIDTH, HEIGHT, "Color k means visualizer");
    SetTargetFPS(60);

    u_char *buf = malloc(width*height * n_channels * sizeof(*buf));
    int iteration = 0;
    k_means(data, sets, width*height, centroids, k, n_channels);
    k_means_adjust(data, sets, width*height, centroids, k, n_channels);
    
    modify_data(buf, sets, width*height, centroids, n_channels);

    while (!WindowShouldClose()) {
        if (iteration) {
            // copy image to buf

            k_means(data, sets, width*height, centroids, k, n_channels);
            if (k_means_adjust(data, sets, width*height, centroids, k, n_channels)) printf("FOUND SOLUTION");
//            print_cents(centroids, k, n_channels);
            // modify bufs contents
            modify_data(buf, sets, width*height, centroids, n_channels);
        }
        BeginDrawing();
        Texture2D texture;
        {
            ClearBackground(BLACK);
            texture = draw_image(buf, width, height);
        }
        EndDrawing();
        UnloadTexture(texture);
        if (IsKeyPressed(KEY_SPACE)) iteration = 1;
        else iteration = 0;
    }

    CloseWindow();
#endif

    modify_data(data, sets, width*height, centroids, n_channels);
    stbi_write_png(out, width, height, n_channels, data, width*n_channels);

    stbi_image_free(data);
    free(sets);
    return 0;
}


int modify_data(u_char *data, int *sets, int n, u_char *m, int dim) {
    for (int i = 0; i < n; i++) {
        int ix = sets[i];
        for (int j = 0; j < dim; j++) {
            data[j] = m[ix*(dim) + j];
        }
        data += dim;
    }

    return 0;
}

#ifdef VISUALIZE
Texture2D draw_image(u_char *data, int width, int height) {
    Image img = {
        .data = data,
        .width = width,
        .height = height,
        .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8,
        .mipmaps = 1
    };

    Texture2D texture = LoadTextureFromImage(img);

    DrawTexture(texture, 0, 0, WHITE);

    return texture;
}
#endif
