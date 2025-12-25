#include <linux/module.h>
#include <linux/export-internal.h>
#include <linux/compiler.h>

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



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x1bbb3529, "gpio_to_desc" },
	{ 0x3c58fa2d, "gpiod_set_raw_value" },
	{ 0xfe990052, "gpio_free" },
	{ 0x8b970f46, "device_destroy" },
	{ 0x6775d5d3, "class_destroy" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0x92997ed8, "_printk" },
	{ 0x418c10ec, "__register_chrdev" },
	{ 0x59c02473, "class_create" },
	{ 0x2c9a4c10, "device_create" },
	{ 0x47229b5c, "gpio_request" },
	{ 0xeec1c378, "gpiod_direction_output_raw" },
	{ 0x2cc6ea32, "gpiod_direction_input" },
	{ 0x309c5214, "gpiod_get_raw_value" },
	{ 0x6cbbfc54, "__arch_copy_to_user" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x474e54d2, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "C553E8998070D60D3229D07");
