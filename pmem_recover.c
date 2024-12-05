#include <libpmem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_PATH "/mnt/pmem/pmemfile"  // Emulated persistent memory file path
#define FILE_SIZE 100                   // Adjusted file size for shorter output

int main() {
    size_t mapped_len;
    int is_pmem;

    // Step 1: Map the emulated persistent memory file
    char *pmemaddr = pmem_map_file(FILE_PATH, FILE_SIZE, 0, 0666, &mapped_len, &is_pmem);
    if (pmemaddr == NULL) {
        perror("pmem_map_file");
        exit(1);
    }

    if (!is_pmem) {
        printf("Warning: Using emulated persistent memory.\n");
    }

    // Step 2: Read and display the data
    printf("Recovered data: %s\n", pmemaddr);

    // Cleanup
    pmem_unmap(pmemaddr, mapped_len);
    return 0;
}
