#include <assert.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Computes a histogram of a chunk of the data sequence.
int *compute_histogram(float a, float b, float *data, int data_count,
                       int bin_count)
{
    // This is the size of a chunk.
    int local_data_count = data_count / omp_get_num_threads();

    // The local histogram which will later be summed into the global one.
    int *histogram = calloc(bin_count, sizeof(*histogram));

    const float bin_interval = (b - a) / (float)bin_count;

    // Finds where this chunk starts inside the data sequence.
    int start = omp_get_thread_num() * local_data_count;

    for (int i = 0; i < local_data_count; ++i)
    {
        // Computes the bin which this element is part of and increments its
        // count.
        int bin_index = (data[start + i] - a) / bin_interval;
        ++histogram[bin_index];
    }

    return histogram;
}

// Prints a textual representation of the histogram.
void print_histogram(float a, float b, int *histogram, int data_count,
                     int bin_count)
{
    int max_bin_count = 0;

    // Get the maximum bin counting value.
    for (int i = 0; i < bin_count; ++i)
    {
        if (histogram[i] > max_bin_count)
            max_bin_count = histogram[i];
    }

    const float bin_interval = (b - a) / (float)bin_count;

    // Pretty prints the histogram vector.
    printf(
        "sequence range: [%.2f, %.2f]\n"
        "adjusted sequence length: %d\n"
        "number of bins: %d\n"
        "bin interval: %.2f\n",
        a, b, data_count, bin_count, bin_interval);

    for (int i = 0; i < bin_count; ++i)
    {
        float bin_index = (float)i * bin_interval + a;
        printf("[%.2f, %.2f) |", bin_index, bin_index + bin_interval);
        int row_width = ((float)histogram[i] / (float)max_bin_count) * 40.f;
        for (int j = 0; j < row_width; ++j)
        {
            printf("#");
        }
        printf(" %d\n", histogram[i]);
    }

    fflush(stdout);
}

// Generate a sequence of n size that contains elements in the range [a, b].
float *generate_data_sequence(float a, float b, int n)
{
    // Holds the sequence of the generated random numbers in the range [a, b].
    float *data = calloc(n, sizeof(*data));

    // Generate the sequence of random numbers in ther range [a, b].
    for (int i = 0; i < n; ++i)
    {
        data[i] = (float)rand() / (float)RAND_MAX * (b - a) + a;
    }

    return data;
}

int main(int argc, char **argv)
{
    assert(argc >= 2);
    const int thread_count = strtol(argv[1], NULL, 10);

    // Selects a "random" seed for the rand engine.
    srand((unsigned int)time(NULL));

    float a, b; // Min and max range values.
    int data_count; // Length of the random numbers sequence.
    int bin_count; // Number of bins in the histogram.

    printf("Type in the values for: <a> <b> <data_count> <bin_count>: ");
    fflush(stdout);
    scanf("%f %f %d %d", &a, &b, &data_count, &bin_count);

    // Chunk size for each thread.
    int local_data_count = data_count / thread_count;
    data_count = local_data_count * thread_count;

    float *data = generate_data_sequence(a, b, data_count);

    // The main histogram that sums all local histograms.
    int *global_histogram = calloc(bin_count, sizeof(*global_histogram));

    // Reduce each local histogram into the global one.
    // This is what you've been breath holding for.
#pragma omp parallel num_threads(thread_count) reduction(+ : global_histogram[:bin_count])
    {
        int *local_histogram =
            compute_histogram(a, b, data, data_count, bin_count);
        for (int i = 0; i < bin_count; ++i)
            global_histogram[i] += local_histogram[i];
        free(local_histogram);
    }

    // Now, let's pretty print some information.
    print_histogram(a, b, global_histogram, data_count, bin_count);

    // Load the jump program.
    //
    // You have to let it all go, Neo.
    // Fear. Doubt.
    // And just believe.
    //
    // Free your mind.
    free(data);
    free(global_histogram);
    // Wow...

    return 0;
}
