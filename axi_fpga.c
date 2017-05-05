#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include <asm/io.h>				//virt_to_phys
#include <linux/ioport.h>		//request_mem_region
#include <linux/slab.h>			//kmalloc and kfree
#include <linux/mm.h>			//remap_pfn_range
#include <linux/device.h>		//class_create
#include "axi_fpga.h"

unsigned int *base_vir_addr=NULL;
struct resource *base_vir_mem_addr=NULL;
dev_t axi_fpga_dev_num;
struct cdev *p_axi_fpga_dev=NULL;
static int axi_fpga_major = 0;
struct class *axi_fpga_class = NULL;

/*
void reset_lwhps2fpga(void)
{
	char *p = "lwhps2fpga_vir_addr";
	unsigned int *lwhps2fpga_vir_addr;
	struct resource *lwhps2fpga_vir_mem_addr;
	unsigned int value = 0xdeadbeef;
	unsigned int i=0;
	
	lwhps2fpga_vir_mem_addr = request_mem_region(RESET_MANAGER_BASE_ADDR + BRGMODRST_ADDR, 4, p);
	if(!lwhps2fpga_vir_mem_addr)
	{
		printk("lwhps2fpga_vir_addr request_mem_region failed!\n");
	}
	
	lwhps2fpga_vir_addr = ioremap(RESET_MANAGER_BASE_ADDR + BRGMODRST_ADDR, 4);
	if(!lwhps2fpga_vir_addr)
	{
		printk("lwhps2fpga_vir_addr ioremap failed!\n");
	}
	
	value = ioread32(lwhps2fpga_vir_addr);
	printk("Original value in RESET_MANAGER_BASE_ADDR + BRGMODRST_ADDR is 0x%x\n", value);
	
	value |= 0x00000002;
	iowrite32(value, lwhps2fpga_vir_addr);
	
	//usleep(500);
	for(i=0;i<10000;i++)
	{
		i = i;
	}
	
	value &= 0xfffffffd;
	iowrite32(value, lwhps2fpga_vir_addr);	
    
    iounmap(lwhps2fpga_vir_addr);
    release_mem_region(RESET_MANAGER_BASE_ADDR + BRGMODRST_ADDR, 4);
}
*/

static int axi_fpga_dev_open(struct inode *inode, struct file *filp)
{
	//printk(KERN_EMERG "axi fpga dev is open!\n");
	return 0;
}

static ssize_t axi_fpga_dev_mmap(struct file *filp, struct vm_area_struct *vma)  
{  
	int ret;  
	//printk(KERN_EMERG "In axi_fpga_dev_mmap!\n");
	
	vma->vm_flags |= VM_IO;
	vma->vm_flags |= (VM_DONTEXPAND | VM_DONTDUMP);
	vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);	//禁止了相关页的cache和写缓冲(write buffer)
	
	ret = remap_pfn_range(vma,  
				vma->vm_start,  
				BASE_PHYS_ADDR >> PAGE_SHIFT,
				//vma->vm_pgoff,
				vma->vm_end-vma->vm_start,  
				vma->vm_page_prot);  
    
	if(ret != 0)
	{   
		printk("axi_fpga_dev_mmap error!\n");
		return -11;  
	}  
    
	return 0;  
}

static int axi_fpga_dev_release(struct inode *node, struct file* filp)
{
	//printk(KERN_EMERG "axi fpga dev is released!\n");
	return 0;
}

static const struct file_operations axi_fpga_dev_fops =
{
	.owner = THIS_MODULE,
	.open = axi_fpga_dev_open,
	.mmap = axi_fpga_dev_mmap,
	.release = axi_fpga_dev_release,
};

static int __init axi_fpga_dev_init(void)
{
	int ret;
	char *p = "axi_fpga_vir_mem";
	
	printk("In axi fpga driver!\n");
	
	//reset_lwhps2fpga();
	
	if(axi_fpga_major)
	{
		axi_fpga_dev_num = MKDEV(axi_fpga_major, 0);
		ret = register_chrdev_region(axi_fpga_dev_num, 1, "axi_fpga_dev");
	}
	else
	{
		ret = alloc_chrdev_region(&axi_fpga_dev_num, 0, 1, "axi_fpga_dev");
	}
	if(ret < 0)
	{
		printk("alloc axi_fpga_dev fail!\n");
		return ret;
	}
	
	p_axi_fpga_dev = kmalloc(sizeof(struct cdev), GFP_KERNEL);
	if(!p_axi_fpga_dev)
	{
		printk("kmalloc cdev fail!\n");
		return 1;
	}
	
	cdev_init(p_axi_fpga_dev, &axi_fpga_dev_fops);
	p_axi_fpga_dev->owner = THIS_MODULE;
	
	ret = cdev_add(p_axi_fpga_dev, axi_fpga_dev_num, 1);
	if(ret)
	{
		printk("add axi_fpga_dev fail!\n");
		return ret;
	}
#if 1	
	base_vir_mem_addr = request_mem_region(BASE_PHYS_ADDR, TOTAL_LEN, p);
	if(!base_vir_mem_addr)
	{
		printk("request_mem_region failed!\n");
		return 1;
	}
	printk("request_mem_region OK!\n");
	
	base_vir_addr = ioremap(BASE_PHYS_ADDR, TOTAL_LEN);
	if(!base_vir_addr)
	{
		printk("ioremap fail!\n");
		printk("!!!*base_vir_addr = 0x%x\n", (unsigned int)base_vir_addr);
		return -1;
	}
	printk("AXI fpga dev virtual address is 0x%x\n", (unsigned int)base_vir_addr);
	
	printk("*base_vir_addr = 0x%x\n", ioread32(base_vir_addr));
	//printk("*(base_vir_addr + 0x100) = 0x%x\n", ioread32(base_vir_addr + 0x100));
#endif	
	axi_fpga_class = class_create(THIS_MODULE, "axi_fpga_dev");
	if (IS_ERR(axi_fpga_class))
	{
        	printk("Err:failed in creating axi fpga class.\n");
        	return -1;
	}
	device_create(axi_fpga_class, NULL, axi_fpga_dev_num, NULL, "axi_fpga_dev");
	
	return 0;
}

static void __exit axi_fpga_dev_exit(void)
{
    unregister_chrdev_region(axi_fpga_dev_num, 1);
	cdev_del(p_axi_fpga_dev);
    kfree(p_axi_fpga_dev);
    iounmap(base_vir_addr);
	release_mem_region(BASE_PHYS_ADDR, TOTAL_LEN);
    device_destroy(axi_fpga_class, axi_fpga_dev_num);
    class_destroy(axi_fpga_class);
	printk("Bye Bye axi fpga driver!\n");
}

module_init(axi_fpga_dev_init);
module_exit(axi_fpga_dev_exit);

MODULE_LICENSE("Dual BSD/GPL");
