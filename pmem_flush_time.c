#include <libpmem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FILE_PATH "/mnt/pmem/pmemfile"  // Emulated persistent memory file path
#define FILE_SIZE (1024 * 1024)         // 1MB file size

void generate_numbers(char *buffer, size_t size) {
    size_t i = 0;
    int num = 1;
    while (i < size - 1) {
        int written = snprintf(buffer + i, size - i, "%d", num++);
        if (written < 0 || (size_t)written >= size - i) {
            break;  // Stop if snprintf fails or buffer is full
        }
        i += written;
    }
    buffer[size - 1] = '\0';  // Null-terminate the string
}

int main() {
    size_t mapped_len;
    int is_pmem;

    // Step 1: Map the emulated persistent memory file
    char *pmemaddr = pmem_map_file(FILE_PATH, FILE_SIZE, PMEM_FILE_CREATE, 0666, &mapped_len, &is_pmem);
    if (pmemaddr == NULL) {
        perror("pmem_map_file");
        exit(1);
    }

    if (!is_pmem) {
        printf("Warning: Using emulated persistent memory.\n");
    }

    // Step 2: Generate a string of numbers
    char data[FILE_SIZE];
    generate_numbers(data, FILE_SIZE);

    // Step 3: Write data to emulated persistent memory
    strcpy(pmemaddr, data);

    // Step 4: Measure time taken to flush data
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    pmem_persist(pmemaddr, strlen(data) + 1);
    clock_gettime(CLOCK_MONOTONIC, &end);

    // Calculate elapsed time in nanoseconds
    long seconds = end.tv_sec - start.tv_sec;
    long nanoseconds = end.tv_nsec - start.tv_nsec;
    double elapsed = seconds + nanoseconds*1e-9;

    printf("Data flushed to emulated persistent memory.\n");
    printf("Time taken to flush: %.9f seconds\n", elapsed);

    // Cleanup
    pmem_unmap(pmemaddr, mapped_len);
    return 0;
}
