#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/kobject.h>

struct test_container {
	int member;
	struct kobject kobj;
};

struct test_container *con1, *con2, *con3;
struct kset *test_kset;
struct kobj_type test_type;


struct attribute name_attr = {
	.name = "name",
	.mode = 0666,
};

struct attribute val_attr = {
	.name = "member",
	.mode = 0666,
};

struct attribute *test_attrs[] = {
	&name_attr, 
	&val_attr,
	NULL,
};

ssize_t test_show(struct kobject *kobj, struct attribute *attr, char *buffer)
{
	struct test_container *obj = container_of(kobj, struct test_container, kobj);
	ssize_t count = 0;

	if (strcmp(attr->name, "name") == 0) {
		count = sprintf(buffer, "%s\n", kobject_name(kobj));
	} else if (strcmp(attr->name, "member") == 0) {
		count = sprintf(buffer, "%d\n", obj->member);
	}

	return count;
}

ssize_t test_store(struct kobject *kobj, struct attribute *attr, const char *buffer, size_t size)
{
	struct test_container *obj = container_of(kobj, struct test_container, kobj);

	if (strcmp(attr->name, "member") == 0) {
		sscanf(buffer, "%d", &obj->member);
	}

	return size;
}

struct sysfs_ops test_sysfsops = {
	.show = test_show,
	.store = test_store,
};

void obj_release(struct kobject *kobj)
{
	struct test_container *obj = container_of(kobj, struct test_container, kobj);
	printk(KERN_INFO "obj_release,name is: %s\n", kobject_name(&obj->kobj));
	kfree(obj);
}

static int __init testkset_init(void)
{
	printk(KERN_WARNING "test_init\n");

	test_kset = kset_create_and_add("test_kset", NULL, NULL);
	if (!test_kset) {
		return -ENOMEM;
	}

	con1 = kzalloc(sizeof(struct test_container), GFP_KERNEL);
	if (!con1) {
		kset_unregister(test_kset);
		return -ENOMEM;
	}
	con1->member = 1;

	con2 = kzalloc(sizeof(struct test_container), GFP_KERNEL);
	if (!con2) {
		kset_unregister(test_kset);
		kfree(con1);
		return -ENOMEM;
	}
	con2->member = 2;
	
	con3 = kzalloc(sizeof(struct test_container), GFP_KERNEL);
	if (!con3) {
		kset_unregister(test_kset);
		kfree(con1);
		kfree(con2);
		return -ENOMEM;
	}
	con3->member = 3;

	con1->kobj.kset = test_kset;
	con2->kobj.kset = test_kset;
	con3->kobj.kset = test_kset;
	
	test_type.release = obj_release;
	test_type.default_attrs = test_attrs;
	test_type.sysfs_ops = &test_sysfsops;

	kobject_init_and_add(&con1->kobj, &test_type, NULL, "con1");
	kobject_init_and_add(&con2->kobj, &test_type, &con1->kobj, "con2");
	kobject_init_and_add(&con3->kobj, &test_type, NULL, "con3");

	return 0;
}

static void __exit testkset_exit(void)
{
	printk(KERN_INFO "test_kset_exit\n");

	kobject_del(&con1->kobj);
	kobject_put(&con1->kobj);

	kobject_del(&con2->kobj);
	kobject_put(&con2->kobj);
	
	kobject_del(&con3->kobj);
	kobject_put(&con3->kobj);

	kset_unregister(test_kset);

	return;
}

module_init(testkset_init);
module_exit(testkset_exit);

MODULE_LICENSE("GPL");

