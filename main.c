#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>


#define MY_T_K_MEANS_SIZE_IMPL
typedef u_char size_d_tim_;

#define T_K_MEANS_IMPL
#include "k_means.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define K 15


int modify_data(u_char *data, int *sets, int n, u_char *m, int dim);


int main(int argc, char **argv) {
    char *in = NULL, *out = NULL;
    int opt, k = 0;

    while ((opt = getopt(argc, argv, "i:o:k:")) != -1) {
        switch (opt) {
            case 'i':
                in = optarg;
                break;

            case 'o':
                out = optarg;
                break;

            case 'k':
                k = atoi(optarg);
                break;

            default:
                fprintf(stderr, "Usage: %s [-k nclusters] [-i] <input file>", argv[0]);
                return -1;
        }
    }

    if (!in) return -1;
    if (!out) out = "out.png";

    if (!k) k = K;

    int width, height, n_channels;

    u_char *data = stbi_load(in, &width, &height, &n_channels, 0);
    if (!data) {
        perror("Couldn't read image!\n");
        stbi_failure_reason();
        return -1;
    }

//    printf("got image (%dx%d), %d!\n", width, height, n_channels);


    int *sets = malloc(width*height * sizeof(*sets));
    assert(sets);
    memset(sets, -1, width*height * sizeof(*sets));

    u_char *centroids = malloc(width*height*k * sizeof(*centroids));
    assert(centroids);

    gen_cents(data, width*height, centroids, k, n_channels);

    while (1) {
        k_means(data, sets, width*height, centroids, k, n_channels);
        if (k_means_adjust(data, sets, width*height, centroids, k, n_channels)) break;
//        print_data(data, sets, width*height, n_channels);
    }

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
