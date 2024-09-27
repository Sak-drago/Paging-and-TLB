#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>

#define P_SIZE 4096
#define P_TABLE_ENTRIES 1024
#define P_DIRECTORY_ENTRIES 1024

typedef struct {
  bool valid;  
  uint32_t add_physical;
} PTE;

typedef struct {
  PTE entries[P_TABLE_ENTRIES];
} PageTable;

typedef struct {
  PageTable* directory[P_DIRECTORY_ENTRIES];
} PageDirectory;

PageDirectory page_directory;
int hit = 0;
int miss = 0;

// Initialize the Page Directory
void inital_dir() {
  for (int i = 0; i < P_DIRECTORY_ENTRIES; i++) {
    page_directory.directory[i] = NULL;
  }
}

uint32_t load(uint32_t add_virtual) {
  uint32_t pd_i = (add_virtual >> 22) & 0x3FF;  // Page Directory Index
  uint32_t pt_i = (add_virtual >> 12) & 0x3FF;  // Page Table Index
  uint32_t offset = add_virtual & 0xFFF;        // Offset within the page

  if (page_directory.directory[pd_i] == NULL) {
    page_directory.directory[pd_i] = (PageTable*) malloc(sizeof(PageTable));
    for (int i = 0; i < P_TABLE_ENTRIES; i++) 
      page_directory.directory[pd_i]->entries[i].valid = false;
    
    printf("Page Fault: Created Page Table at Page Directory Index %d\n", pd_i);
    miss++;
  }

  PageTable* page_table = page_directory.directory[pd_i];

  if (!page_table->entries[pt_i].valid) {
    page_table->entries[pt_i].add_physical = (pd_i * P_DIRECTORY_ENTRIES + pt_i) * P_SIZE;
    page_table->entries[pt_i].valid = true;
    printf("Page Fault: Loaded Page at Page Table %d for Page Directory %d\n", pt_i, pd_i);
    miss++;
  } else {
    hit++;
    printf("Page Hit: PD Index: %d, PT Index: %d\n", pd_i, pt_i);
  }

  uint32_t add_physical = page_table->entries[pt_i].add_physical + offset;
  return add_physical;  // Returning the simulated physical address
}

void store(uint32_t add_virtual, uint32_t value) {
  uint32_t pd_i = (add_virtual >> 22) & 0x3FF;  // Page Directory Index
  uint32_t pt_i = (add_virtual >> 12) & 0x3FF;  // Page Table Index
  uint32_t offset = add_virtual & 0xFFF;        // Offset within the page

  if (page_directory.directory[pd_i] == NULL) {
    page_directory.directory[pd_i] = (PageTable*) malloc(sizeof(PageTable));
    for (int i = 0; i < P_TABLE_ENTRIES; i++) 
      page_directory.directory[pd_i]->entries[i].valid = false;
    
    printf("Page Fault: Created Page Table at Page Directory Index %d\n", pd_i);
    miss++;
  }
  
  PageTable* page_table = page_directory.directory[pd_i];

  if (!page_table->entries[pt_i].valid) {
    page_table->entries[pt_i].add_physical = (pd_i * P_DIRECTORY_ENTRIES + pt_i) * P_SIZE;
    page_table->entries[pt_i].valid = true;
    printf("Page Fault: Loaded Page at Page Table %d for Page Directory %d\n", pt_i, pd_i);
    miss++;
  } else {
    hit++;
    printf("Page Hit: PD Index: %d, PT Index: %d\n", pd_i, pt_i);
  }

  uint32_t add_physical = page_table->entries[pt_i].add_physical + offset;
  printf("Stored value %d at VA: 0x%X (PA: 0x%X)\n", value, add_virtual, add_physical);
}

void summary() {
  uint32_t total = hit + miss;
  printf("========================\n");
  printf("Summary:\n");
  printf("Total Hits: %d\n", hit);
  printf("Total Misses: %d\n", miss);
  printf("Total Accesses: %d\n", total);
}

int main() {
  inital_dir();
  
  uint32_t va_1 = 0xFFF;  // Example virtual addresses
  uint32_t va_2 = 0x1000;

  store(va_1, 300);
  store(va_2, 2000);

  printf("Loaded Value: %d\n", load(va_1));
  printf("Loaded Value: %d\n", load(va_2));

  summary();
  return 0;
}

