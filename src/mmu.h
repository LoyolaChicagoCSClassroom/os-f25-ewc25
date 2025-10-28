#ifndef MMU_H
#define MMU_H

#include <stdint.h>
#include "page.h"

#define PAGE_SIZE 4096u
#define PAGE_MASK (~(PAGE_SIZE - 1u))
#define PDIR_ENTRIES 1024u
#define PTBL_ENTRIES 1024u

#define PDIR_INDEX(v) (((uint32_t)(v) >> 22) & 0x3FFu)
#define PTBL_INDEX(v) (((unit32_t)(v) >> 12) & 0x3FFu)
#define PAGE_ALIGN_DOWN(x) ((void *)((uint32_t)(x) & PAGE_MASK))

struct page_directory_entry {
   uint32_t present	: 1;
   uint32_t rw	        : 1;
   uint32_t user        : 1;
   uint32_t writethru   : 1;
   uint32_t cache_disabled: 1;
   uint32_t accessed    : 1;
   uint32_t reserved    : 1;
   uint32_t pagesize    : 1;
   uint32_t global      : 1;
   uint32_t available   : 3;
   uint32_t frame       : 20;
 } __attribute__((packed));

struct page_entry {
  uint32_t present        : 1;
  uint32_t rw             : 1;
  uint32_t user	          : 1;
  uint32_t writethru	  : 1;
  uint32_t cache_disabled : 1;
  uint32_t accessed       : 1;
  uint32_t dirty          : 1;
  uint32_t pat            : 1;
  uint32_t global         : 1;
  uint32_t available      : 3;
  uint32_t frame          : 20;
} __attribute__((packed));


 extern struct page_directory_entry kernel_page_directory[PDIR_ENTRIES];

 void *map_pages(void *vaddr, struct ppage *pglist, struct page_directory_entry *pd);

 void loadPageDirectory(struct page_directory_entry *pd);
 void enablePaging(void);

 #endif








