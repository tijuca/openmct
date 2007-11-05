#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/unistd.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <asm/current.h>
#include <linux/fcntl.h>
#include <asm/uaccess.h>
#include <linux/reboot.h>

#if defined(CONFIG_MCT_SG100)

#define CPLD_GPIO_ADDR  0xb4000400
#define CPLD_PASSWD_RESET_BIT   0x01

#elif defined(CONFIG_MCT_SG600) || defined(CONFIG_MCT_SA100)

#define CPU_PIODI_ADDR  0xfffef504
#define CPU_GPIO_PASSWD_RESET_BIT       0x00000020
#define LOCALBUS_DATAIN_ADDR    0xb2000000
#define LOCALBUS_DATAIN_PASSWD_RESET_BIT 0x04

#elif defined(CONFIG_MCT_SG4100)

#define CPU_PIODI_ADDR  0xff1ff504
#define CPU_GPIO_PASSWD_RESET_BIT       0x00000008

#elif defined(CONFIG_MCT_SA200)

#include<asm/tx4925.h>
#define CPU_PIODI_ADDR  0xff1ff504
#define CPU_GPIO_PASSWD_RESET_BIT       0x00000008

#endif

static int mct_flash_reset_thread(void *data)
{
        int ret;
        struct task_struct *tsk = current;
        char *argv[2], *envp[3];
        struct file *file = NULL;
        mm_segment_t old_fs;
        int     normal_interval;
#if defined(CONFIG_MCT_SG100) || defined(CONFIG_MCT_SG600) || defined(CONFIG_MCT_SA100)
        unsigned short  val;
#elif defined(CONFIG_MCT_SG4100) || defined(CONFIG_MCT_SA200)
        unsigned long   val;
#endif

        DECLARE_WAITQUEUE(wait, tsk);
        daemonize();
        exit_files(current);
        reparent_to_init();
        strcpy(current->comm, "resetd");

        printk("MCT: Recovery thread starting.\n");

        for (;;) {
                set_current_state(TASK_INTERRUPTIBLE);
                schedule_timeout(6 * HZ);
        	normal_interval = 1;
#if defined(CONFIG_MCT_SG100)
	        val = (*(volatile unsigned short*) CPLD_GPIO_ADDR);
        	if ( (val & CPLD_PASSWD_RESET_BIT) == 0) {
#elif defined(CONFIG_MCT_SG600)
	        val = (*(volatile unsigned short*) LOCALBUS_DATAIN_ADDR);
	        if ( (val & LOCALBUS_DATAIN_PASSWD_RESET_BIT) == 0) { 
#elif defined(CONFIG_MCT_SA100)
	        val = *(volatile unsigned long *) CPU_PIODI_ADDR;
	        if ( (val & CPU_GPIO_PASSWD_RESET_BIT) == 0) {
#elif defined(CONFIG_MCT_SG4100) || defined(CONFIG_MCT_SA200)
	        val = *(volatile unsigned long *) CPU_PIODI_ADDR;
	        if ( (val & CPU_GPIO_PASSWD_RESET_BIT) == 0) {
#endif
                        old_fs = get_fs();
                        set_fs(KERNEL_DS);
                        file = filp_open("/squashfs/jffs2/.reset", O_WRONLY|O_CREAT, 0644);
                        if (file) {
                                filp_close(file, NULL);
                        }
                        set_fs(old_fs);
                        machine_restart(NULL);
                }
        }

        return 0;
}

static int __init init_module(void)
{
  /* Start kernel thread detecting reset button */
  kernel_thread(mct_flash_reset_thread, NULL, 0);

  return 0;
}

static void __exit cleanup_module(void)
{ }

MODULE_LICENSE("GPL");
