#if 0

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kthread.h>


static struct task_struct *tsk;

static int thread_function(void *data)
{
	int time_count = 0;

	do{
		printk(KERNEL_INFO "thread_function %d times", ++time_count);
		msleep(1000);
	}while(!kthread_should_stop() && time_count <= 30);

	return time_count;
	
}


static int hello_init(void)
{
	printk(KERNEL_INFO "Hello kernel\n");


	tsk = kthread_run(thread_function, NULL, "mythread%d", 1);

	if(IS_ERR(tsk){
		printk(KERNEL_INFO "create kthread failed\n");
	} else
		printk(KERNEL_INFO "create kthread ok\n ");

	return 0;

}

static void hello_exit(void)
{
	printk(KERN_INFO "Bye kernel\n");

	if(!IS_ERR(tsk)){
		int ret = kthread_stop(tsk);
		printk(KERN_INFO "thread_function has run %d\n ",ret);
	}

	
	
}


module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");

#endif


#if 0
///eg 1

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/wait.h>
#include <linux/time.h>
#include <linux/jiffies.h>
#include <linux/delay.h>




MODULE_LICENSE("GPL");

static struct task_struct * _tsk1;
static struct task_struct * _tsk2;

static int tc = 0;

static wait_queue_head_t log_wait_queue;

static int thread_function1(void *data)
{
	do{
		printk(KERN_INFO "IN thread_function_1 :%d times\n", tc);

		wait_event_interruptible(log_wait_queue, tc == 10);
		tc = 0; //必须加这一行，内核才会进行调度。内核线程不像应用程序会主动调度，我们需要显式的使用调度函数，想要在thread_function_1中去重置tc的值是不可能的，因为线程不会被调度，该线程会一直占用CPU

		printk(KERN_INFO "Thread_function_1 has been woke up!\n");
			printk("KKKK1111 should_stop = %d\n",kthread_should_stop());

	}while(!kthread_should_stop());
	printk("KKKK2222 should_stop = %d\n",kthread_should_stop());

	return tc;
		
}

static int thread_function2(void * data)
{
	do{
		printk(KERN_INFO "IN thread_function_2 thread_funciton :%d times\n", ++tc);

		
		if(tc == 10 && waitqueue_active(&log_wait_queue)){
			wake_up_interruptible(&log_wait_queue);
		}

		msleep_interruptible(1000);
	}while(!kthread_should_stop());

	return tc;

}

static int hello_init(void)
{
	printk(KERN_INFO "Hello world kernel\n");

	init_waitqueue_head(&log_wait_queue);

	_tsk1 = kthread_run(thread_function1, NULL, "mythread1");

	if(IS_ERR(_tsk1)){
		printk(KERN_INFO "first create kthread failed\n");
	} else {
		printk(KERN_INFO "first create kthread OK\n");
	}

	_tsk2 = kthread_run(thread_function2, NULL, "mythread2");
	if(IS_ERR(_tsk2)){
		printk(KERN_INFO "second create kthread fail\n");
	}else {
		printk(KERN_INFO "second create kthread OK\n");
	}

	return 0;

}


static void hello_exit(void)
{
	printk(KERN_INFO "Bye kernel\n");

	if(!IS_ERR(_tsk1)){
		kthread_stop(_tsk1);
		printk(KERN_INFO "First thread function exit\n");
	}

	if(!IS_ERR(_tsk2)){
		kthread_stop(_tsk2);
		printk(KERN_INFO "Second thread function exit\n");
	}

}


module_init(hello_init);
module_exit(hello_exit);

MODULE_AUTHOR("WYF");
#endif


#if 0
///eg 3
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/param.h>
#include <linux/jiffies.h>
#include <asm/system.h>
#include <linux/completion.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/kthread.h>


MODULE_LICENSE("GPL");

static DECLARE_COMPLETION(my_completion);

static struct task_struct *task;

int flag = 0;


int my_function(void *arg)
{
	printk("in %s()\n", __FUNCTION__);

	allow_signal(SIGKILL);//使得线程可以接收SIGKILL信号

	mdelay(2000);
	printk("my_function complete()\n");
	printk("should stop : %d\n",kthread_should_stop());

	//while(!signal_pending(current) && !kthread_should_stop()){//使得线程可以可以被杀死，也可以再rmmod的时候结束
		printk("jiffies is %lu\n",jiffies);
		set_current_state(TASK_INTERRUPTIBLE);

		schedule_timeout(HZ * 5);

		printk("ssssshould stop :%d\n", kthread_should_stop());
		printk("HZ = %d\n",HZ);
	//}

	printk("KKKK should_stop = %d\n",kthread_should_stop());
	printk("Leaving my_function\n");

	flag = 1;//flag很关键

	return 0;
}


static int __init hello_init(void)
{
	task = kthread_run(my_function, NULL, "my_function");
		//printk("KKKKsssss should_stop = %d\n",kthread_should_stop());

	printk("<1> init wait for completion()\n");

	return 0;

}


static void __exit hello_exit(void)
{
	int ret ;
	if(!flag){
		if(!IS_ERR(task)){
			//	printk("KKKK22 should_stop = %d\n",kthread_should_stop());
			ret = kthread_stop(task);
			printk("Fikrst thread function stop ret = %d\n",ret);
				//printk("KKKK33 should_stop = %d\n",kthread_should_stop());
		}
	}

	printk("task_struct:0x%p", task);

	printk("Bye kernel\n");

}


module_init(hello_init);
module_exit(hello_exit);

#endif

#include <linux/init.h>
#include <linux/module.h>
#include <linux/wait.h>
#include <linux/completion.h>

#include <linux/jiffies.h>
#include <asm/system.h>

#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/kthread.h>


MODULE_LICENSE("GPL");


static struct completion comp;
static struct task_struct *tsk1;
static struct task_struct *tsk2;

static int tc = 0;

static int thread_function_1(void *data)
{
	do{
		printk(KERN_INFO "In thread_function1 :%d times\n", tc);

		wait_for_completion(&comp);

		//tc = 0; ///在哪里都行

		printk(KERN_INFO "Has been woke up\n");
	}while(!kthread_should_stop());

	return tc;
}

static int thread_function_2(void *data)
{
	do{
		printk(KERN_INFO"In thread_funciton_2 :%d times\n",++tc);

		if(tc == 10){
			complete(&comp);
			tc = 0;
		}

		msleep_interruptible(1000);

	}while(!kthread_should_stop());

	return tc;
}

static int hello_init(void)
{
	printk(KERN_INFO "Hello world \n");

	init_completion(&comp);

	tsk1 = kthread_run(thread_function_1, NULL, "mythred1");
	if(IS_ERR(tsk1)){
		printk(KERN_INFO"First create kthread failed\n");
	}else{
		printk(KERN_INFO "First create kthread ok\n");
	}

	tsk2 = kthread_run(thread_function_2, NULL, "mythread2");
	if(IS_ERR(tsk2)){
		printk(KERN_INFO"Second create kthread failed");
	}else{
		printk("Second create kthread OK\n");
	}

	return 0;
}

static void hello_exit(void)
{
	printk(KERN_INFO"Bye kernel..\n");

	if(!IS_ERR(tsk1)){
		int ret = kthread_stop(tsk1);
		printk(KERN_INFO "First thread function1 has stop ret = %d\n",ret);
	}

	if(!IS_ERR(tsk2)){
		int ret = kthread_stop(tsk2);
		printk(KERN_INFO "Second thread function has stop ret = %d\n",ret);
	}

}

module_init(hello_init);
module_exit(hello_exit);

