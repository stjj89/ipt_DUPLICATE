/*
 * Shared library add-on to iptables to add customized DUPLICATE
 * target module support.
 *
 * Written by Samuel Tan <samueltan@gmail.com>
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <xtables.h>
#include <linux/netfilter_ipv4/ipt_DUPLICATE.h>
#include <linux/version.h>

static void DUPLICATE_help(void)
{
    printf(
"DUPLICATE target options:\n"
"--times NUM                Duplicate each packet NUM times (1 by default)\n");

    printf("(*) See man page or read the INCOMPATIBILITES file for compatibility issues.\n");
}

static const struct option DUPLICATE_opts[] = {
    { "times",        1, NULL, '1' },
    { .name = NULL }
};

static void DUPLICATE_init(struct xt_entry_target *t)
{
    struct ipt_duplicate_info *duplicate = (struct ipt_duplicate_info *)t->data;

    duplicate->times = 1; /* duplicate packet once by default */    
}

static int DUPLICATE_parse(int c, char **argv, int invert, unsigned int *flags,
                        const void *entry, struct xt_entry_target **target)
{
    struct ipt_duplicate_info *duplicate = (struct ipt_duplicate_info *)(*target)->data;
    unsigned int val;

    switch(c) {
    case '1':
        if (!xtables_strtoui(optarg, NULL, &val, 0, UINT32_MAX))
            xtables_error(PARAMETER_PROBLEM,
                   "cannot parse --times `%s'", optarg);
        if (val < 1)
            xtables_error(PARAMETER_PROBLEM,
                   "Argument passed to --times cannot be less than 1");
        duplicate->at_byte = val;
        return 1;
    default:
        /* Fall through */
        break;
    }
    
    return 0;
}

static void DUPLICATE_print(const void *ip, const struct xt_entry_target *target,
                         int numeric)
{
    const struct ipt_duplicate_info *duplicate
        = (const struct ipt_duplicate_info *)target->data;
    
    printf("times %d ", duplicate->times);
}

static void DUPLICATE_save(const void *ip, const struct xt_entry_target *target)
{
    const struct ipt_duplicate_info *duplicate
        = (const struct ipt_duplicate_info *)target->data;
    
    printf("--times %d ", duplicate->times);
}

static struct xtables_target duplicate_tg_reg = {
    .name               = "DUPLICATE",
    .version            = XTABLES_VERSION,
    .family             = NFPROTO_IPV4,
    .size               = XT_ALIGN(sizeof(struct ipt_duplicate_info)),
    .userspacesize      = XT_ALIGN(sizeof(struct ipt_duplicate_info)),
    .help               = DUPLICATE_help,
    .init               = DUPLICATE_init,
    .parse              = DUPLICATE_parse,
    .print              = DUPLICATE_print,
    .save               = DUPLICATE_save,
    .extra_opts         = DUPLICATE_opts,
};

void _init(void)
{
    xtables_register_target(&duplicate_tg_reg);
}
