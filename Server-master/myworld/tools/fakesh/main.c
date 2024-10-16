#include "cmd_line.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>

#define PREFIX ":> "
#define PREFIX_SIZE (sizeof(PREFIX) - 1)

int main( int argc, char * argv[] )
{
	fd_set fds_read;
	int ret = 0;
	struct cmd_line cmd_line;

	UNUSED(argc);
	UNUSED(argv);

	cmd_init(&cmd_line, STDOUT_FILENO, 16);

	setvbuf(stdout, NULL, _IONBF, 0);
	printf(PREFIX);

	while( ret >= 0 )
	{
		char * line = NULL;
		FD_ZERO( &fds_read );
		FD_SET(STDIN_FILENO, &fds_read);

		ret = select(FD_SETSIZE, &fds_read, NULL, NULL, NULL);
		if(FD_ISSET(STDIN_FILENO, &fds_read))
		{
			if( cmd_fetch_char(&cmd_line, STDIN_FILENO, 1) < 0 )
				goto out;
			//snprintf(msg, sizeof(msg), "input : %d\n", cmd_line.last );
			//write_con(STDOUT_FILENO, msg);

			switch(cmd_line.last)
			{
			case 10: // Enter
				if( ( line = cmd_line_end(&cmd_line) ) != NULL )
				{
					printf("\necho : %s\n"PREFIX, line);
					free(line);
				}
				else
					printf("\n"PREFIX);
				break;

			case 127: // Backspace
				cmd_back_char(&cmd_line);
				break;

			case 27: // Up Down Left Right
				if( cmd_fetch_char(&cmd_line, STDIN_FILENO, 1) < 0 ||
						cmd_fetch_char(&cmd_line, STDIN_FILENO, 1) < 0 )
					goto out;
				switch(cmd_line.last)
				{
				case 65: // Up
					cmd_track_history(&cmd_line, 1);
					break;
				case 66: // Down
					cmd_track_history(&cmd_line, 0);
					break;
				case 67: // Right
					cmd_right_mov(&cmd_line);
					break;
				case 68: // Left
					cmd_left_mov(&cmd_line);
					break;
				}
				break;

			default: // Other key
				cmd_insert_char(&cmd_line);
				break;
			}
		}
	}

out:
	cmd_final(&cmd_line);
	return 0;
}
