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
	{ 0xa9af3815, "netlink_kernel_release" },
	{ 0x229cb617, "__netlink_kernel_create" },
	{ 0x92c7b82b, "init_net" },
	{ 0xe2d5255a, "strcmp" },
	{ 0x656e4a6e, "snprintf" },
	{ 0xc959d152, "__stack_chk_fail" },
	{ 0x5c3c7387, "kstrtoull" },
	{ 0x85df9b6c, "strsep" },
	{ 0xe87cafd4, "kfree_skb" },
	{ 0xc5850110, "printk" },
	{ 0x5e57471d, "netlink_unicast" },
	{ 0x69acdf38, "memcpy" },
	{ 0x862470e8, "__nlmsg_put" },
	{ 0x9c4786fe, "__alloc_skb" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "E3AB40B08165BF90BF9959F");
