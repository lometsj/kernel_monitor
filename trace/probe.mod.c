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
	{ 0xd9726f80, "module_layout" },
	{ 0x36c11c94, "kmalloc_caches" },
	{ 0xeb233a45, "__kmalloc" },
	{ 0xf4487d2, "send_usrmsg" },
	{ 0x108be812, "unregister_kprobe" },
	{ 0xe73fe9c, "register_kprobe" },
	{ 0xe2d5255a, "strcmp" },
	{ 0xc5850110, "printk" },
	{ 0xa916b694, "strnlen" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0xcbd4898c, "fortify_panic" },
	{ 0x69ecc112, "kmem_cache_alloc_trace" },
	{ 0x37a0cba, "kfree" },
};

MODULE_INFO(depends, "transfer");


MODULE_INFO(srcversion, "B9494FEE9AAAE82AEFB89E7");
