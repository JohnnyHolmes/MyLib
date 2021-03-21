#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include "my_printk.h"
static int __init audit_test_ko_init(void){
    int ret;

    pr_devel("XUPENG: audit_ko_test initialized.\n");
    struct task_struct * tmp = current;
    int cnt = 0;
    while(tmp->pid!=1) {
        pr_devel("XUPENG: tmp->pid = %d, tmp->comm=%s.\n", tmp->pid, tmp->comm);
	tmp = tmp->parent;
	if(cnt>=10)
		break;
    }
    return 0;
}

static void __exit audit_test_ko_exit(void){
    pr_devel("XUPENG: audit_ko_test removed.\n");
}

module_init(audit_test_ko_init);
module_exit(audit_test_ko_exit);
MODULE_LICENSE("GPL");

