#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/audit.h>
#include <linux/string.h>
#include "my_printk.h"

#define MAX_CALL_LINK 10
static int audit_test_ko_init(void)
{
    struct task_struct * tmp = current;
    struct audit_buffer *ab;
    int cnt = 0;
    
    pr_devel("AUDIT_TEST: audit_ko_test initialized.\n");
    ab = audit_log_start(audit_context(), GFP_KERNEL, AUDIT_KERNEL);
    if (ab == NULL) {
        return 0;
    }
    audit_log_format(ab, "AUDIT_TEST: call back:");
    while(tmp->pid!=1) {
	audit_log_format(ab, "[%d, %s]", task_tgid_nr(tmp),tmp->comm);
	audit_log_format(ab, "<----");
        tmp = tmp->parent;
	//假定进程进程调用链小于10,可根据实际情况修改 
        if (cnt >= MAX_CALL_LINK ){
                break;
	}
    }
    audit_log_format(ab, "INIT");
    audit_log_end(ab);
    return 0;
}

static void audit_test_ko_exit(void)
{
    pr_devel("AUDIT_TEST: audit_ko_test removed.\n");
}

/* For each probe you need to allocate a kprobe structure */
static struct kprobe kp = {
	.symbol_name	= "common_lsm_audit",
};
static int handler_pre(struct kprobe *p, struct pt_regs *regs)
{
   
    /* 在这里可以调用内核接口函数dump_stack打印出栈的内容*/
    
    audit_test_ko_init();
    return 0;
}
 
/* 在被探测指令执行后，kprobe调用后处理例程post_handler */
static void handler_post(struct kprobe *p, struct pt_regs *regs,
                unsigned long flags)
{

}
 
/*在pre-handler或post-handler中的任何指令或者kprobe单步执行的被探测指令产生了例外时，会调用fault_handler*/
static int handler_fault(struct kprobe *p, struct pt_regs *regs, int trapnr)
{
    printk(KERN_DEBUG "fault_handler: p->addr = 0x%p, trap #%dn",
        p->addr, trapnr);
    /* 不处理错误时应该返回*/
    return 0;
} 
static int __init kprobe_init(void)
{
	int ret;
	kp.pre_handler = handler_pre;
	kp.post_handler = handler_post;
	kp.fault_handler = handler_fault;
 
	ret = register_kprobe(&kp);
	if (ret < 0) {
		printk(KERN_INFO "register_kprobe failed, returned %d\n", ret);
		return ret;
	}
	printk(KERN_INFO "Planted kprobe at %p\n", kp.addr);
	return 0;
}
 
static void __exit kprobe_exit(void)
{
	unregister_kprobe(&kp);
	printk(KERN_INFO "kprobe at %p unregistered\n", kp.addr);
	audit_test_ko_exit();
}
 
module_init(kprobe_init)
module_exit(kprobe_exit)
MODULE_LICENSE("GPL");
