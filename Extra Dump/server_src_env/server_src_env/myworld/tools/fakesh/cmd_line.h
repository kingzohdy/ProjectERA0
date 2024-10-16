#ifndef CMD_LINE_H
#define CMD_LINE_H

#include <string.h>
#include <unistd.h>

#define BUF_SIZE 4096
#define UNUSED(var) ((void)(var))


static inline int write_con(int fd, const char * msg)
{
	return write(fd, msg, strlen(msg)*sizeof(msg[0]));
}


struct dlist_entry
{
	struct dlist_entry * prev;
	struct dlist_entry * next;
};

struct cmd_line_node
{
	struct dlist_entry entry;
	char cmd[1];
};

struct cmd_line_history
{
	struct dlist_entry list;
	struct dlist_entry * current;
	int n;
	int max;
};

struct cmd_line
{
	char line[BUF_SIZE];
	int pos;
	int len;
	int last;
	int fd;
	int do_not_track;
	struct cmd_line_history history;
};

void cmd_init( struct cmd_line * cmd_line, int fd, int max_history );

void cmd_final( struct cmd_line * cmd_line );

int cmd_fetch_char( struct cmd_line * cmd_line, int fd, int count );

char * cmd_line_end( struct cmd_line * cmd_line );

void cmd_track_history( struct cmd_line * cmd_line, int up );

void cmd_insert_char( struct cmd_line * cmd_line );

void cmd_del_char( struct cmd_line * cmd_line );

void cmd_back_char( struct cmd_line * cmd_line );

void cmd_left_mov( struct cmd_line * cmd_line );

void cmd_right_mov( struct cmd_line * cmd_line );

#endif // CMD_LINE_H
