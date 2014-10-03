#include <igx.h>
#include <kcons.h>

#ifndef _KMEM_
#define _KMEM_

#define _KMEM_PG_DIR_SZ    0x2
#define _KMEM_PG_TBL_SZ    0x400
#define _KMEM_PG_SZ        0x1000
#define _KMEM_LIMIT        0x400000;

extern void * _kmem_loc;
extern void * _kmem_pg_dir;
extern void * _kmem_pg_tbl;
extern void **lstack_top;
extern void *lstack_base;
extern ulong sz_low;
extern ulong sz_high;

/* claim 0x400000 bytes of memory for the kernel that means that the
   kernel gets 0x101000 -> 0x401000
   
   Forget trees.  The kernel memory allocator is just going to be a
   stack of addresses of _kmem_page_sz-byte pages that the kernel can use.
*/

void bcopy(const void *src, void *dest, int n);
void memclear(const void *p, int n);
void *align_up(void *p);
void pg_mem_setup(void *pg_dir_loc, void *pg_tbl_loc);
void enable_paging();
void init_pg_stack(void *base);
void init_map(long *mb);

#endif
