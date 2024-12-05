#include <string.h>
#include <libpmem.h>
#include <stdio.h>
#include <stdlib.h>

#define FILE_PATH "/mnt/pmem/pmemfile"  // Path to the persistent memory file
#define FILE_SIZE 1024 * 1024          // 1MB file size

int main() {
    size_t mapped_len; // Variable to store the mapped length
    int is_pmem;       // Flag to check if mapping is to persistent memory

    // Step 1: Map the persistent memory file
    char *pmemaddr = pmem_map_file(FILE_PATH, FILE_SIZE, PMEM_FILE_CREATE, 0666, &mapped_len, &is_pmem);

    // Check if the mapping succeeded
    if (pmemaddr == NULL) {
        perror("pmem_map_file");
        exit(1);
    }

    // Step 2: Check if the file is on real persistent memory
    if (!is_pmem) {
        printf("Warning: Not real persistent memory, using emulation.\n");
    } else {
        printf("File is mapped to real persistent memory.\n");
    }

    // Step 3: Write some data to the mapped memory
    const char *data = "Persistent Memory Test";
    sprintf(pmemaddr, "%s", data);

    // Step 4: Persist the data
    pmem_persist(pmemaddr, strlen(data) + 1);

    printf("Data written and persisted: %s\n", pmemaddr);

    // Step 5: Unmap the memory
    pmem_unmap(pmemaddr, mapped_len);

    return 0;
}
