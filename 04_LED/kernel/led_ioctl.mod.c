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
	{ 0x92997ed8, "_printk" },
	{ 0x418c10ec, "__register_chrdev" },
	{ 0x403f9529, "gpio_request_one" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0x1bbb3529, "gpio_to_desc" },
	{ 0x3c58fa2d, "gpiod_set_raw_value" },
	{ 0xfe990052, "gpio_free" },
	{ 0x474e54d2, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "5D325A0DC3DC9E36F657BE9");
