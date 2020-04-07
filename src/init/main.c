#include <arch/interrupt.h>
#include <xbook/memcache.h>
#include <xbook/debug.h>
#include <xbook/softirq.h>
#include <xbook/clock.h>
#include <xbook/vmarea.h>
#include <xbook/task.h>
#include <xbook/unit.h>
#include <xbook/rawblock.h>
#include <xbook/schedule.h>
#include <xbook/sharemem.h>
#include <xbook/msgqueue.h>
#include <xbook/sem.h>

int kernel_main(void)
{
    printk(KERN_INFO "welcom to xbook kernel.\n");
    /* init memory cache for kernel */
    init_mem_caches();
    init_vmarea();
    /* init irq description */
    init_irq_description();
    /* init softirq */
    init_softirq();
    
    /* init ipc */
    init_share_mem();
    init_msg_queue();
    init_sem();
    
    init_tasks();
    init_clock();
    
    enable_intr(); /* 打开中断 */
    
    /* init unit drivers */
    init_unit();

    /* init raw block */
    init_raw_block();

    kernel_pause();

    return 0;    
}

