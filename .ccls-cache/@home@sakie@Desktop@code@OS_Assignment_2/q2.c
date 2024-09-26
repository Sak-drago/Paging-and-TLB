#include <stdio.h>
#include <stdlib.h>

struct B_B{
  int base;
  int bound;
};


int Add_Translation(struct B_B seg, int offset){
  if(offset>= seg.bound)return -1;
  return seg.base+offset;
}


int main(){
  
  struct{
  struct B_B Code; 
  struct B_B Heap;
  struct B_B Stack;
  }Add_Space;

  Add_Space.Code.base = 0x8000;
  Add_Space.Code.bound = 0x0800;
  Add_Space.Heap.base = 0x8800;
  Add_Space.Heap.bound = 0x0C00;
  Add_Space.Stack.base = 0x7000;
  Add_Space.Stack.bound = 0x0800;

  int address;
  printf("Enter the address (Hex): \n"); 
  scanf("%x", &address);

  int choice;
  printf("Choose which segment to translate: \n");
  printf("1. Code\n");
  printf("2. Heap\n");
  printf("3. Stack\n");
  scanf("%i", &choice);

  int addPhysical = -1;
  if(choice == 1){
    addPhysical = Add_Translation(Add_Space.Code, address);
  }
  else if(choice == 2){
    addPhysical = Add_Translation(Add_Space.Code, address);
  }
  else if(choice == 3){
    addPhysical = Add_Translation(Add_Space.Code, address);
  }
  else{
    printf("Invalid Choice\n");
  }

  if(addPhysical!= -1){
    printf("Physical Address: 0x%X", addPhysical);
  }
  else{
    printf("Segmentation Fault\n");
    abort();
  }
  return 0;
}
