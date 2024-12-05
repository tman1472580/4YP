#include <libpmem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#define FILE_PATH "/mnt/pmem/pmemfile"  // Emulated persistent memory file path
#define FILE_SIZE 1024 * 1024           // 1MB file size (adjustable for your test)

void generate_numbers(char *buffer, size_t size) {
    size_t i = 0;
    int num = 1;
    while (i < size - 1) {
        i += snprintf(buffer + i, size - i, "%d", num++);
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

    // Step 2: Generate a continuous string of numbers
    char *data = malloc(FILE_SIZE);
    if (!data) {
        perror("malloc");
        pmem_unmap(pmemaddr, mapped_len);
        exit(1);
    }
    generate_numbers(data, FILE_SIZE);

    // Step 3: Write and flush the data to emulated persistent memory
    strcpy(pmemaddr, data);
    pmem_persist(pmemaddr, strlen(data) + 1);

    printf("Data written and flushed to emulated persistent memory. Simulating failure...\n");

    // Simulate failure
    exit(1);

    // Cleanup (won't be reached in this simulation)
    free(data);
    pmem_unmap(pmemaddr, mapped_len);
    return 0;
}
