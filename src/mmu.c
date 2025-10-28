#include <stdint.h>
#include "page.h"
#include "mmu.h"


struct page_directory_entry kernel_page_directory[PDIR_ENTRIES]
   __attribute__((aligned(4096)));


static struct page_entry page_table_pool[4][PTBL_ENTRIES]
   __attribute__((aligned(4096)));
static uint32_t page_table_pool_used = 0;
static struct page_entry *pd_slot_table[PDIR_ENTRIES];


static struct page_entry *get_or_create_page_table(struct page_directory_entry *pd, uint32_t pd_index){
  if (pd_slot_table[pd_index])
     return pd_slot_table[pd_index];
  if (page_table_pool_used >= (sizeof(page_table_pool) / sizeof(page_table_pool[0])))
     return 0;

  struct page_entry *pt = page_table_pool[page_table_pool_used++];

  for(uint32_t i = 0; i < PTBL_ENTRIES; i++) {
   pt[i].present =0;
   pt[i].rw =1;
   pt[i].user = 0;
   pt[i].writethru = 0;
   pt[i].cache_disabled =0;
   pt[i].accessed = 0;
   pt[i].dirty = 0;
   pt[i].pat = 0;
   pt[i].global = 0;
   pt[i].available = 0;
   pt[i].frame = 0;
 }

  struct page_directory_entry *pde = &pd[pd_index];
  pde->present = 1;
  pde->rw = 1;
  pde->user = 0;
  pde->writethru = 0;
  pde->cache_disabled = 0;
  pde->accessed = 0;
  pde->reserved = 0;
  pde->pagesize = 0;
  pde->global = 0;
  pde->available = 0;
  pde->frame = ((uint32_t)pt) >> 12;

  pd_slot_table[pd_index] = pt;
  return pt;
 }


void *map_pages(void *vaddr, struct ppage *pglist, struct page_directory_entry *pd) {
	uint32_t v = (uint32_t)vaddr;
	struct ppage *curr = pglist;

	while (curr) {
	uint32_t pd_index = (v >> 22) & 0x3FF;
	uint32_t pt_index = (v >> 12) & 0x3FF;
	
	struct page_entry *pt = get_or_create_page_table(pd, pd_index);
	if (!pt)
	    break;
	
	struct page_entry *pte = &pt[pt_index];
	pte->present = 1;
	pte->rw = 1;
	pte->user = 0;
	pte->writethru = 0;
	pte->accessed = 0;
	pte->dirty = 0;
	pte->pat = 0;
	pte->global = 0;
	pte->available = 0;
	pte->frame = ((uint32_t)curr->physical_addr) >> 12;

	v += PAGE_SIZE;
	curr = curr->next;
   }

  return vaddr;

}



void loadPageDirectory(struct page_directory_entry *pd) {
	asm volatile("mov %0, %%cr3" : : "r"(pd) : "memory");
}

void enablePaging(void) {
	asm volatile(
	"mov %%cr0, %%eax\n\t"
	"or $0x80000001, %%eax\n\t"
	"mov %%eax, %%cr0\n\t"
	:
	:
	: "eax", "memory"
     );
}





















