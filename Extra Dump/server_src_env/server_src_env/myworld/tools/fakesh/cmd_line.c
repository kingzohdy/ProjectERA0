#include "cmd_line.h"
#include <string.h>
#include <stdlib.h>
#include <termios.h>


static const int kDummyLeftKey      = 0x1b | (0x5b << 8) | (0x44 << 16);
static const int kDummyRightKey     = 0x1b | (0x5b << 8) | (0x43 << 16);
static const int kDummyBackspaceKey = 0x7f;
static const int kDummySpaceKey     = 0x20;



static inline struct dlist_entry * dlist_head( struct dlist_entry * list )
{
	return list->next;
}

static inline struct dlist_entry * dlist_tail( struct dlist_entry * list )
{
	return list->prev;
}

static inline void dlist_push_back( struct dlist_entry * list, 
		                           struct dlist_entry * x )
{
	x->prev = list->prev;
	list->prev->next = x;
	x->next = list;
	list->prev = x;
}

static inline void dlist_remove( struct dlist_entry * x )
{
	x->next->prev = x->prev;
	x->prev->next = x->next;
}



static void history_init( struct cmd_line_history * history, int max )
{
	history->list.prev = &history->list;
	history->list.next = &history->list;
	history->current = &history->list;
	history->n = 0;
	history->max = max;
}

static void history_final( struct cmd_line_history * history )
{
	struct dlist_entry * phe = NULL;
	struct dlist_entry * i = dlist_head(&history->list);
	while(i != &history->list)
	{
		phe = i;
		i = i->next;
		dlist_remove(phe);
		free(phe);
	}
}

static inline void history_fallback( struct cmd_line_history * history )
{
	history->current = &history->list;
}

static int history_record( struct cmd_line_history * history, const char * line )
{
	struct cmd_line_node * node = NULL;
	if( !line || !line[0] )
		return -1;
	if( history->n >= history->max )
	{
		struct dlist_entry * head = dlist_head(&history->list);
		history->n--;
		dlist_remove(head);
		free(head);
	}
	node = calloc( sizeof(struct cmd_line_node) + strlen(line) + 1, 1 );
	strcpy(node->cmd, line);
	dlist_push_back(&history->list, &node->entry);
	return ++(history->n);
}

static const char * history_watch( struct cmd_line_history * history, int up )
{
	if( up )
		history->current = history->current->prev;
	else
		history->current = history->current->next;
	return history->current == &history->list ? 
		NULL : (((struct cmd_line_node*)history->current)->cmd);
}

static struct termios old_setting;

static void set_tty_mode()
{
	struct termios new_setting;
	tcgetattr(0, &old_setting);
	new_setting = old_setting;
	new_setting.c_lflag &= (~ICANON);
	new_setting.c_lflag &= (~ECHO);
	new_setting.c_cc[VTIME] = 0;
	new_setting.c_cc[VMIN] = 1;
	tcsetattr(0, TCSANOW, &new_setting);
}

static void reset_tty_mode()
{
	tcsetattr(0, TCSANOW, &old_setting);
}


void cmd_init( struct cmd_line * cmd_line, int fd, int max_history )
{
	set_tty_mode();
	bzero(cmd_line, sizeof(*cmd_line));
	cmd_line->fd = fd;
	history_init(&cmd_line->history, max_history);
}

void cmd_final( struct cmd_line * cmd_line )
{
	history_final(&cmd_line->history);
	reset_tty_mode();
}

int cmd_fetch_char( struct cmd_line * cmd_line, int fd, int count )
{
	cmd_line->last = 0;
	if( count <= 0 || count > (int)sizeof(cmd_line->last) )
		return -1;
	return read(fd, &cmd_line->last, count);
}

char * cmd_line_end( struct cmd_line * cmd_line )
{
	char * line = NULL;
	if( cmd_line->len > 0 )
	{
		cmd_line->line[cmd_line->len] = 0;
		line = strdup(cmd_line->line);
		bzero(cmd_line->line, cmd_line->len);
		cmd_line->len = 0;
		cmd_line->pos = 0;
		if( !cmd_line->do_not_track )
			history_record(&cmd_line->history, line);
		history_fallback(&cmd_line->history);
		cmd_line->do_not_track = 0;
	}
	return line;
}

void cmd_track_history( struct cmd_line * cmd_line, int up )
{
	int i = 0;
	const char * last = NULL;
	last = history_watch(&cmd_line->history, up);
	if(last)
	{
		i = cmd_line->len - cmd_line->pos;
		while(i--)
			write(cmd_line->fd, &kDummyRightKey, 3);
		i = cmd_line->len;
		while(i--)
			write(cmd_line->fd, &kDummyBackspaceKey, 1);
		write_con(cmd_line->fd, last);
		strcpy(cmd_line->line, last);
		cmd_line->len = strlen(cmd_line->line);
		cmd_line->pos = cmd_line->len;
		cmd_line->do_not_track = 1;
	}
}

void cmd_insert_char( struct cmd_line * cmd_line )
{
	int i = 0;
	memmove(cmd_line->line + cmd_line->pos + 1, 
					cmd_line->line + cmd_line->pos,
					cmd_line->len - cmd_line->pos);
	cmd_line->line[cmd_line->pos++] = (char)(cmd_line->last & 0x000000ff);
	cmd_line->len++;
	write(cmd_line->fd, &cmd_line->last, 1);
	if( cmd_line->pos < cmd_line->len )
	{
		write_con(cmd_line->fd, cmd_line->line + cmd_line->pos);
		i = cmd_line->len - cmd_line->pos;
		while(i--)
			write(cmd_line->fd, &kDummyLeftKey, 3);
	}
	cmd_line->do_not_track = 0;
}

void cmd_del_char( struct cmd_line * cmd_line )
{
	if( cmd_line->pos < cmd_line->len )
	{
		memmove(cmd_line->line + cmd_line->pos, 
				    cmd_line->line + cmd_line->pos + 1,
						cmd_line->len - cmd_line->pos);
		cmd_line->len--;
		write(cmd_line->fd, &cmd_line->last, 1);
	}
}

void cmd_back_char( struct cmd_line * cmd_line )
{
	int i = 0;
	if( cmd_line->pos > 0 )
	{
		memmove(cmd_line->line + cmd_line->pos - 1, 
				    cmd_line->line + cmd_line->pos,
						cmd_line->len + 1 - cmd_line->pos);
		cmd_line->pos--;
		cmd_line->len--;
		write(cmd_line->fd, &cmd_line->last, 1);
		if( cmd_line->pos < cmd_line->len )
		{
			write_con(cmd_line->fd, cmd_line->line + cmd_line->pos);
			write(cmd_line->fd, &kDummySpaceKey, 1);
			i = cmd_line->len + 1 - cmd_line->pos;
			while(i--)
				write(cmd_line->fd, &kDummyLeftKey, 3);
		}
	}
}

void cmd_left_mov( struct cmd_line * cmd_line )
{
	if( cmd_line->pos > 0 )
	{
		cmd_line->pos--;
		write(cmd_line->fd, &kDummyLeftKey, 3);
	}
}

void cmd_right_mov( struct cmd_line * cmd_line )
{
	if( cmd_line->pos < cmd_line->len )
	{
		cmd_line->pos++;
		write(cmd_line->fd, &kDummyRightKey, 3);
	}
}


