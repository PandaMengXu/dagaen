/* This is a module to test for long hypercalls in Xen. */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/time.h>

#include <asm/xen/hypercall.h>
#include <xen/xen.h>

#define MHZ 2100

#if defined(__i386__)
static __inline__ unsigned long long rdtsc(void)
{
  unsigned long long int x;
     __asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
     return x;
}
#elif defined(__x86_64__)
static __inline__ unsigned long long rdtsc(void)
{
  unsigned hi, lo;
  __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
  return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
}
#endif

static int __init lock_init (void) {
	unsigned long long start, end, max = 0;
    unsigned long long start_tsc, end_tsc, max_tsc = 0, lat_tsc;
	int counter;

	printk(KERN_INFO "Testing for Hypercall Bug...\n");
	/* Must be running on Xen */
	if (!xen_domain()) {
		printk(KERN_INFO "NOT Running on Xen, exiting.\n");
		return -ENODEV;
	}

	for(counter = 0; counter < 50000000; ++counter) {
	//for(counter = 0; counter < 1000; ++counter) {
		start = jiffies;
        start_tsc = rdtsc();
		HYPERVISOR_xen_version(0, NULL);
        end_tsc = rdtsc();
		end = jiffies_to_usecs(jiffies - start);

        lat_tsc = ( end_tsc - start_tsc ) / MHZ;
		if(end > max) max = end;
        if ( lat_tsc > max_tsc )
        {
            printk("prev max: %llu us, new max: %llu us\n", max_tsc, lat_tsc);
            max_tsc = lat_tsc;
        }
	}

	printk(KERN_INFO "Max hyercall length: %llu us\n", max);
	printk(KERN_INFO "Max hyercall length (use tsc counter): %llu us\n", max_tsc);

	return 0;
}

static void __exit lock_cleanup(void) {
	printk(KERN_INFO "Unloading Hypercall Bug Testing Module.\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dagaen Golomb <dgolomb@seas.upenn.edu>");
MODULE_DESCRIPTION("RTDS Hypercall Bug Proof-of-Concept.");
module_init(lock_init);
module_exit(lock_cleanup);
