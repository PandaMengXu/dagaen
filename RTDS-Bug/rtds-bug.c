/* This is a module to test for long hypercalls in Xen. */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/time.h>

#include <asm/xen/hypercall.h>
#include <xen/xen.h>

static int __init lock_init (void) {
	unsigned long start, end, max = 0;
	int counter;

	printk(KERN_INFO "Testing for Hypercall Bug...\n");
	/* Must be running on Xen */
	if (!xen_domain()) {
		printk(KERN_INFO "NOT Running on Xen, exiting.\n");
		return -ENODEV;
	}

	for(counter = 0; counter < 1000000; ++counter) {
		start = jiffies;
		HYPERVISOR_xen_version(0, NULL);
		end = jiffies_to_usecs(jiffies - start);

		if(end > max) max = end;
	}

	printk(KERN_INFO "Max hyercall length: %lu us\n", max);

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
