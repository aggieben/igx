#include <igx.h>
#include <kmem.h>

void * _kmem_loc;
void * _kmem_pg_dir;
void * _kemm_pg_tbl;
void **lstack_top;
void *lstack_base;
ulong sz_low;
ulong sz_high;

void *align_up(void *p)
{
  ulong tp = (ulong)p;
  if(tp & 0xfffff000)
  { 
    tp += 0x1000; 
  }  
  else   
    return p;   
  return (void*)(tp & 0xfffff000);  
}

void pg_mem_setup(void *pg_dir_loc, void *pg_tbl_loc)
{
  short PDE_off, PTE_off;
  ulong *pgd = (ulong*)pg_dir_loc, *pgt = (ulong*)pg_tbl_loc;
  ulong i,j;
  /* 
     since kmem is only going to be 0x400000 bytes, there needs only
     to be 1 PDE and 1024 PTEs

     0x0 - 0x100000 will always be mapped to itself, along with
     0x100000 - 0x500000 (4MB) for the kernel.

     need a programmatic way to set up these structures up to the
     _kmem_limit.
  */
  for(i=0;i<_KMEM_PG_DIR_SZ;i+=4)
  {
    /* pgt + i*0x100 is used to access the appropriate page table,
       each of which is 0x400 (1024) entries (words) in size */
    *(pgd + PDE_off++) = (((ulong)(pgt + i*0x400) & 0xfffff000) | 0x3);
    for(j=0;j<_KMEM_PG_TBL_SZ;j++)
    {
      /* j*0x1000 gives the address of each page */
      *(pgt + i*0x400 + PTE_off++) = (((ulong)(j*0x1000)&0xfffff000)|0x3);
    }
  }
}

void enable_paging()
{
  /* write cr3 */
  asm("movl\t%0,\t%%ebx\n"
      "movl\t%%ebx,\t%%cr3"
      :/* outputs */
      :"g" (_kmem_pg_dir)
      :"ebx"
      );
  asm("movl\t%0,\t%%ebx\n"
      "movl\t%%ebx,\t%%cr3"
      :/* outputs */
      :"i" (0x80000000) /* bit 31=1 */
      :"ebx"
      );
  // paging now enabled :-)
}

void _kmem_init(long *mb)
{
  kprintf("initializing paging...\n");

  _kmem_pg_dir = align_up(&_end);
  kmemclear(_kmem_pg_dir,_KMEM_PG_DIR_SZ);

  _kmem_pg_tbl = (void*)((ulong)_kmem_pg_dir + _KMEM_PG_DIR_SZ);
  kmemclear(_kmem_pg_tbl,_KMEM_PG_TBL_SZ*_KMEM_PG_DIR_SZ);

  _kmem_loc = (void*)((ulong)_kmem_pg_tbl + _KMEM_PG_TBL_SZ*_KMEM_PG_DIR_SZ);

  init_pg_stack(_kmem_loc);
  kprintf("initializing memory map...\n");
  init_map(mb);
  
  /* ok, so now all pages are in the stack. now set up the kernel's
     page directory and page table. */
  kprintf("enabling paging...\n");
  pg_mem_setup(_kmem_pg_dir,_kmem_pg_tbl);
  enable_paging();
  kprintf("kmem initialized.\n");
}

void init_pg_stack(void *base)
{
  lstack_base = base;
  lstack_top = (void**)lstack_base;
}

void push(void *word)
{
  *(lstack_top) = word;
  lstack_top++;
}

void *pop()
{
  return *(lstack_top--);
}

void init_map(long *mb)
{
  ulong i=(ulong)_kmem_loc;
  ulong j=0x100000;
  ulong num_pages;
  ulong *pagep;

  if(*mb & 1)
  {
    /* mem_lower / mem_upper present */
    sz_low = *(mb + 1);
    sz_high = *(mb + 2); 
    kprintf("lowmem detected: ");
    kprintf(tostring(sz_low,10));
    kprintf(" KB\nhighmem detected: ");
    kprintf(tostring(sz_high,10));
    kprintf(" KB\n");
  }

  j += _KMEM_LIMIT;
  kprintf("kmem starts @ ");
  kprintf(ktostring((ulong)_kmem_loc,16));
  kputchar('\n');
  kprintf("kmem ends @ ");
  kprintf(ktostring((ulong)j,16));
  kputchar('\n');

  /* figure out how many pages there are */
  while(i<j)
  {
    j-=_KMEM_PG_SZ;
    i+=sizeof(ulong);
  }
  /* # pages is now i-_kmem_loc */
  num_pages = (ulong*)i - (ulong*)_kmem_loc;
  kprintf(ktostring(num_pages,10));
  kprintf(" kernel pages found\n");

  pagep = (ulong*)((ulong)_kmem_loc + num_pages);
  j = 0x100000 + sz_high*1024;
  while((ulong)pagep < j)
  {
    push(pagep);
    pagep+=_KMEM_PG_SZ;
  }

}

void *calloc(ulong nm, ulong sz)
{
  return (void*)0;
}
