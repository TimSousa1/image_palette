#ifndef T_K_MEANS_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#include <raylib.h>

#ifndef D_RADIUS 
#define D_RADIUS 3
#endif

#ifndef K_RADIUS
#define K_RADIUS 7
#endif

#ifndef MY_T_K_MEANS_SIZE_IMPL
#define MY_T_K_MEANS_SIZE_IMPL
typedef int size_d_tim_;
#endif

// gen rand data with 'dim' dimensions. dim+1 is the set the point belongs to. ex: for dim = 2, n = 2  -> arr = [x, y, set, x, y, set]
// range is an array with 'dim' points specifying the limit of random values. if range = [100, 200], x -- [0, 100] and y -- [0, 200]
int gen_data(size_d_tim_ *arr, int n, int dim, size_d_tim_ *range);
int gen_cents(size_d_tim_ *data, int n, size_d_tim_ *m, int k,  int dim);

int print_data(size_d_tim_ *arr, int *sets, int n, int dim);
// colors is the color of each set
int draw_data(size_d_tim_ *arr, int *sets, int n, int dim, Color *colors);

int print_cents(size_d_tim_ *arr, int n, int dim);
int draw_cents(size_d_tim_ *arr, int n, int dim, Color *colors);

// set points' sets
int k_means(size_d_tim_ *arr, int *sets, int n, size_d_tim_ *m, int k, int dim);
// get new centroids
int k_means_adjust(size_d_tim_ *arr, int *sets, int n, size_d_tim_ *m, int k, int dim);

// generate random 'n' colors
int gen_colors(Color *colors, int n);
#endif

#ifdef T_K_MEANS_IMPL
int gen_data(size_d_tim_ *data, int n, int dim, size_d_tim_ *range) {
    if (!data) return -1;
    if (!range) return -1;

    for (int i = 0; i < n; i++){
        for (int j = 0; j < dim; j++){
            data[j] = rand() % range[j];
        }
        data += dim;
    }

    return 0;
}


int gen_cents(size_d_tim_ *data, int n, size_d_tim_ *m, int k,  int dim) {
    for (int i = 0; i < k; i++) {
        int ix = rand() % n;
        for (int j = 0; j < dim; j++) {
            m[j] = data[j + ix*(dim)];
        }
        m += dim;
    }

    return 0;
}


int print_cents(size_d_tim_ *m, int k, int dim) {
    printf("centroids:\n");
    for (int i = 0; i < k; i++) {
        printf("[");

        for (int j = 0; j < dim-1; j++) {
            printf("%d, ", m[j]);
        }
        printf("%d]\n", m[dim-1]);
        m += dim;
    }

    return 0;

}


int draw_cents(size_d_tim_ *arr, int n, int dim, Color *colors) {
    if (dim != 2) return -1;

    for (int i = 0; i < n; i++) {
        DrawCircle(arr[0], arr[1], K_RADIUS, colors[i]);
        arr += dim;
    }

    return 0;
}


int print_data(size_d_tim_ *arr, int *sets, int n, int dim) {
    printf("data:\n");
    for (int i = 0; i < n; i++) {
        printf("[");

        for (int j = 0; j < dim-1; j++) {
            printf("%d, ", arr[j]);
        }
        printf("%d | %d]\n", arr[dim-1], sets[i]);
        arr += dim;
    }

    return 0;
}


int draw_data(size_d_tim_ *arr, int *sets, int n, int dim, Color *colors) {
    if (dim != 2) return -1;

    for (int i = 0; i < n; i++) {
        DrawCircle(arr[0], arr[1], D_RADIUS, colors[sets[i]]);
        arr += dim;
    }

    return 0;
}


int k_means(size_d_tim_ *arr, int *sets, int n, size_d_tim_ *m, int k, int dim) {
    for (int i = 0; i < n; i++) { // for each point
        int set_ix = -1;
        long min_d = LONG_MAX; // change later
        size_d_tim_ *m_t = m;

        for (int g = 0; g < k; g++) { // for each centroid
            long d = 0; // change later
            for (int j = 0; j < dim; j++) { // for each dim
                d += (m_t[j] - arr[j])*(m_t[j] - arr[j]);
            }

            if (d < min_d) {
                min_d = d;
                set_ix = g;
            }

            m_t += dim;
        }

        sets[i] = set_ix;
        arr += dim;
    }

    return 0;
}


// return value: bool -> if new centroids are the same
int k_means_adjust(size_d_tim_ *arr, int *sets, int n, size_d_tim_ *m, int k, int dim) {
    int *sum = (int *) calloc(k*dim, sizeof(*sum));
    int *sum_n = (int *) calloc(k, sizeof(*sum_n));

    // iterators
    int *sum_t = sum;
    int *sum_n_t = sum_n;

    for (int i = 0; i < n; i++) { // for each point
        int ix = sets[i];
        for (int j = 0; j < dim; j++) { // for each dim
            sum_t[ix*dim + j] += arr[j];
        }
        sum_n_t[ix]++;
        arr += dim;
    }

    /* for (int g = 0; g < k; g++) {
        for (int j = 0; j < dim; j++) { // for each dim
            printf("sum: %d | sum_n: %d\n", sum_t[g*dim + j], sum_n_t[g]);
        }
    } */

    int eq = 1;
    for (int g = 0; g < k; g++) { // for each centroid
        for (int j = 0; j < dim; j++) { // for each dim
            if (sum_n_t[g] == 0) continue;
            int mid = sum_t[j] / sum_n_t[g];
            if (mid != m[j] && eq) eq = 0;
            m[j] = mid;

//            printf("%d ", sum_t[j]);
        }
        m += dim;
        sum_t += dim;
    }

    free(sum);
    free(sum_n);

    return eq;
}


int gen_colors(Color *colors, int n) {
    if (!colors) return -1;

    for (int i = 0; i < n; i++) {
        colors[i].r = rand() % 200 + 55;
        colors[i].g = rand() % 200 + 55;
        colors[i].b = rand() % 200 + 55;
        colors[i].a = 255;
    }

    return 0;
}

#endif
