#include <libpmem.h>
#include <stdio.h>
#include <stdlib.h>

#define FILE_PATH "/mnt/pmem/pmemfile"  // Emulated persistent memory file path

int main() {
    size_t mapped_len;
    int is_pmem;

    // Step 1: Map the emulated persistent memory file
    char *pmemaddr = pmem_map_file(FILE_PATH, 0, PMEM_FILE_EXCL, 0666, &mapped_len, &is_pmem);
    if (pmemaddr == NULL) {
        perror("pmem_map_file");
        exit(1);
    }

    if (!is_pmem) {
        printf("Warning: Using emulated persistent memory.\n");
    }

    // Step 2: Print the recovered data
    printf("Recovered Data:\n%s\n", pmemaddr);

    // Cleanup
    pmem_unmap(pmemaddr, mapped_len);
    return 0;
}
