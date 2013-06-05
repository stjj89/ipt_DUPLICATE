/*
 * Target module which is used to duplicate packets
 */

/* Written by Samuel Tan <samueltan@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/socket.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <linux/icmp.h>
#include <net/icmp.h>
#include <net/ip.h>
#include <net/tcp.h>
#include <net/udp.h>
#include <net/udplite.h>
#include <net/route.h>
#include <net/dst.h>
#include <net/checksum.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include <linux/netfilter_ipv4/ipt_DUPLICATE.h>
#ifdef CONFIG_BRIDGE_NETFILTER
#include <linux/netfilter_bridge.h>
#endif

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Samuel Tan <samueltan@gmail.com>");
MODULE_DESCRIPTION("Xtables: packet duplication target for IPv4");

static unsigned int
duplicate_tg(struct sk_buff *skb, const struct xt_target_param *par)
{
    struct sk_buff *copied_skb;
    struct iphdr *iph = ip_hdr(skb);
    const struct ipt_duplicate_info *duplicate = par->targinfo;

    /* Duplicate packet */
    for (int i = 0; i < duplicate->times; i++) {
        copied_skb = skb_copy(skb, GFP_ATOMIC);
        
        // If ladder calling the right IP Architecture function calls
        // to send the cloned skb the right way
    }
    

    return XT_CONTINUE;  
}

static struct xt_target duplicate_tg_reg __read_mostly = {
    .name       = "DUPLICATE",
    .family     = NFPROTO_IPV4,
    .target     = duplicate_tg,
    .targetsize = sizeof(struct ipt_duplicate_info),
    .table      = "mangle",
    .hooks      =   (1 << NF_INET_PRE_ROUTING) |
                    (1 << NF_INET_FORWARD) |
                    (1 << NF_INET_LOCAL_OUT) |
                    (1 << NF_INET_POST_ROUTING),
    .me         = THIS_MODULE,
};

static int __init duplicate_tg_init(void)
{
    return xt_register_target(&duplicate_tg_reg);
}

static void __exit duplicate_tg_exit(void)
{
    xt_unregister_target(&duplicate_tg_reg);
}

module_init(duplicate_tg_init);
module_exit(duplicate_tg_exit);
