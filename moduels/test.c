char modname[] __attribute__((section(".modname"))) = "mymodule";
 
/* Example dependencies */
char moddeps[] __attribute__((section(".moddeps"))) = "relocator\0video\0vbe\0multiboot\0gfxterm";
 
GRUB_MOD_LICENSE("GPLv3+");

static grub_extcmd_t cmd;
 
GRUB_MOD_INIT(mymodule)
{
    cmd = grub_register_extcmd("mycommand" /* name of the command */,
            grub_cmd_mycommand /* function to call when the command is run */,
            0 /* flags */,
            0 /* summary */,
            N_("In-depth description of this command") /* description */,
            0 /* argument parser */);
}
 
GRUB_MOD_FINI(mymodule)
{
    grub_unregister_extcmd(cmd);
}