#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xd4b13e21, "module_layout" },
	{ 0x647e04f1, "device_destroy" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0xe1cab4d6, "cdev_del" },
	{ 0xd14a7ad2, "class_destroy" },
	{ 0xfe990052, "gpio_free" },
	{ 0xd08ba4da, "device_create" },
	{ 0x9a097aa5, "__class_create" },
	{ 0x8c479ea1, "cdev_add" },
	{ 0x617bf1a9, "cdev_init" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0xa7d3ffea, "gpiod_direction_output_raw" },
	{ 0xbcf08491, "gpiod_direction_input" },
	{ 0xa7eedcc4, "call_usermodehelper" },
	{ 0x12a4e128, "__arch_copy_from_user" },
	{ 0x8da6585d, "__stack_chk_fail" },
	{ 0x6cbbfc54, "__arch_copy_to_user" },
	{ 0x86f1015f, "gpiod_get_raw_value" },
	{ 0xf9a482f9, "msleep" },
	{ 0x92997ed8, "_printk" },
	{ 0x763a8c27, "gpiod_set_raw_value" },
	{ 0xd1364350, "gpio_to_desc" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "AC223C3089B7B09F1B4315E");
