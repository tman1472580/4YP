#include <libpmem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_PATH "/mnt/pmem/pmemfile"  // Emulated persistent memory file path
#define FILE_SIZE 100                   // Adjusted file size for shorter output

void generate_numbers(char *buffer, size_t size) {
    size_t i = 0;
    int num = 1;
    while (i < size - 2) {  // Reserve space for null terminator
        int written = snprintf(buffer + i, size - i, "%d", num++);
        if (written < 0 || (size_t)written >= size - i) {
            break;  // Stop if snprintf fails or buffer is full
        }
        i += written;
    }
    buffer[i] = '\0';  // Null-terminate the string
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

    // Step 2: Generate a concise string of numbers
    char data[FILE_SIZE];
    generate_numbers(data, FILE_SIZE);

    // Step 3: Write and flush the data to emulated persistent memory
    strcpy(pmemaddr, data);
    pmem_persist(pmemaddr, strlen(data) + 1);

    printf("Data written and flushed to emulated persistent memory: %s\n", data);

    // Cleanup
    pmem_unmap(pmemaddr, mapped_len);
    return 0;
}
