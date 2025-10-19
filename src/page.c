#include "page.h"


struct ppage physical_page_array[128]; // 128 pages, each 2mb in length covers 256 megs of memory

struct ppage *free_physical_page_list = 0;

void init_pfa_list(void){
   for (int i = 0; i < 128; i++){
   physical_page_array[i].prev = (i == 0) ? 0 : &physical_page_array[i-1]; 
   physical_page_array[i].next = (i == 127) ? 0 : &physical_page_array[i+1];
  }


 free_physical_page_list =  &physical_page_array[0];
}


struct ppage *allocate_physical_pages(unsigned int npages) {
   if (free_physical_page_list  == 0 || npages == 0)
   return 0;

  struct ppage *head = free_physical_page_list;
  struct ppage *tail = head;

  for (unsigned int i = 1; i < npages && tail->next != 0; i++) {
     tail = tail->next;
   }

  free_physical_page_list = tail->next;
  if (free_physical_page_list)
     free_physical_page_list->prev = 0;
   
  tail->next = 0;
  return head;
}

void free_physical_pages(struct ppage *ppage_list){
  if (ppage_list == 0)
   return;

 struct ppage *tail = ppage_list;
 while (tail->next != 0) 
  tail = tail->next;


 tail->next = free_physical_page_list;
 if (free_physical_page_list)
    free_physical_page_list->prev = tail;

 free_physical_page_list = ppage_list;
 ppage_list->prev = 0;

}
