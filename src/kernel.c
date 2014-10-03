#include <igx.h>
#include <kcons.h>

void printvc()
{
  kprintf("IGX v. %u.%u.%u\n",_IGX_VER,_IGX_REV,_IGX_BLD);
  kprintf("\nCopyright (c) 2004 Benjamin Collins <bencollins@tamu.edu>\n");
  kprintf("See LICENSE.asc for licensing information.\n");
}

/*! \brief Kernel entry point. 
 *
 * The entry point initalizes the necessary hardware, sets up its own
 * memory management (which will later be handed off to user-space),
 * and other tasks needed to get the system prepped to be a standalone
 * system.
 */

void _start(long * mb_info)
{
  /* because new/delete aren't functional until memory is initialized,
     these declarations have to serve as initializations, to (i.e.,
     the constructors get called), which means the order is important.
  */
  _kcons_init();
  printvc();
  
}
