#ifndef __MAN__H
#define __MAN__H

void man_inspect();

// For the 'pci' command
void man_pci();

// For the 'write' command
void man_write();

// For the 'read' command
void man_read();

// For the 'ls' command
void man_ls();

// For the 'rm' command
void man_rm();

// For the 'cls' command
void man_cls();

// For the 'bg' command
void man_bg();

// For the 'logo' command
void man_logo();

// For the 'mkdir' command
void man_mkdir();

// For the 'exit' command
void man_exit();

// For the 'set-xy' command
void man_set_xy();

// For the 'song' command
void man_song();

// For the 'format' command
void man_format();

// For the 'table' command
void man_table();

// For the '3d' command
void man_3d();

// For the 'rle' command
void man_rle();

// For the 'fs' command
void man_fs();

// For the 'exe' command
void man_exe();

// For the 'sect' command
void man_sect();

// For the 'edit' command
void man_edit();

void man_main(const char* functionName);

int man_list();
#endif
