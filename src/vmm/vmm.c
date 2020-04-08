#include <xbook/vmm.h>
#include <xbook/debug.h>
#include <xbook/vmspace.h>

void vmm_init(vmm_t *vmm)
{
    vmm->page_storage = copy_kernel_page_storge();
    if (vmm->page_storage == NULL) {
        panic(KERN_EMERG "task_init_vmm: kmalloc for page_storege failed!\n");
    }
    vmm->vmspace_head = NULL;
    /* 其它参数 */
    
}

void dump_vmm(vmm_t *vmm)
{
    printk(KERN_DEBUG "code: start=%x, end=%x\n", vmm->code_start, vmm->code_end);
    printk(KERN_DEBUG "data: start=%x, end=%x\n", vmm->data_start, vmm->data_end);
    printk(KERN_DEBUG "heap: start=%x, end=%x\n", vmm->heap_start, vmm->heap_end);
    printk(KERN_DEBUG "map: start=%x, end=%x\n", vmm->map_start, vmm->map_end);
    printk(KERN_DEBUG "share: start=%x, end=%x\n", vmm->share_start, vmm->share_end);
    printk(KERN_DEBUG "stack: start=%x, end=%x\n", vmm->stack_start, vmm->stack_end);
    printk(KERN_DEBUG "arg: start=%x, end=%x\n", vmm->arg_start, vmm->arg_end);
    printk(KERN_DEBUG "env: start=%x, end=%x\n", vmm->env_start, vmm->env_end);
}


/**
 * vmm_release_space - 释放掉进程空间管理
 * @vmm: 虚拟内存管理
 * 
 * 额外需要释放共享空间
 * 
 * @return: 成功返回0， 失败返回-1
 */
int vmm_release_space(vmm_t *vmm)
{
    if (vmm == NULL)
        return -1; 
    /* 释放虚拟空间地址描述 */
    vmspace_t *space = (vmspace_t *)vmm->vmspace_head;
    vmspace_t *p;
    //dump_vmspace(vmm);
    
    while (space != NULL) {
        p = space;
        space = space->next;
        vmspace_free(p); /* 释放空间 */
    }
    vmm->vmspace_head = NULL;
    return 0;
}

int vmm_exit(vmm_t *vmm)
{
    if (vmm == NULL)
        return -1; 
    if (vmm->vmspace_head == NULL)
        return -1;
    /*dump_vmm(vmm);
    dump_vmspace(vmm);*/
    /* 取消虚拟空间的地址映射 */
    vmspace_t *space = (vmspace_t *)vmm->vmspace_head;
    while (space != NULL) {
        /* 由于内存区域可能不是连续的，所以需要用安全的方式来取消映射 */
        unmap_pages_safe(space->start, space->end - space->start, space->flags & VMS_MAP_SHARED);
        space = space->next;
    }
    /* 释放虚拟空间描述 */
    if (vmm_release_space(vmm)) {
        return -1;
    }
    free_page(v2p(vmm->page_storage));
    return 0;
}