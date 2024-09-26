#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>

#define P_SIZE 4096
#define P_TABLE_ENTRIES 1024
#define P_DIRECTORY_ENTRIES 1024


typedef struct{
  bool valid;  
  uint32_t add_physical;
}PTE;

typedef struct{
  PTE entries[P_TABLE_ENTRIES];
}PageTable;

typedef struct{
  PageTable* directory[P_DIRECTORY_ENTRIES];
}PageDirectory;

PageDirectory page_directory;
int hit = 0;
int miss = 0;

//initialising it
void inital_dir(){
  for(int i = 0;i< P_DIRECTORY_ENTRIES;i++){
    page_directory.directory[i] = NULL;
  }
}

uint32_t load(uint32_t add_virtual){
  uint32_t pd_i = (add_virtual >> 22) & 0x3FF;
  uint32_t pt_i = (add_virtual >> 12) & 0x3FF;
  uint32_t offset = add_virtual & 0xFFF;
  if(page_directory.directory[pd_i] == NULL){
    page_directory.directory[pd_i] = (PageTable*) malloc(sizeof(PageTable));
    for(int i = 0;i<P_TABLE_ENTRIES;i++) page_directory.directory[pd_i]->entries[i].valid = false;
    printf("Page Fault: Not Existed. Created Page Table at Page Directory Index %d\n", pd_i);
    miss++;
  }

  else{
    PageTable* page_table = page_directory.directory[pd_i];
    if(!page_table->entries[pt_i].valid){
      page_table->entries[pt_i].add_physical = (pd_i*P_DIRECTORY_ENTRIES+pt_i)*P_SIZE;
      page_table->entries[pt_i].valid = true;
      printf("Page Fault: Not Loaded. Loaded Page at Page Table %d, for Page Directory %d\n",pt_i,pd_i);
      miss++;
    }
  }
}

int main(){
  
  return 0;
}
