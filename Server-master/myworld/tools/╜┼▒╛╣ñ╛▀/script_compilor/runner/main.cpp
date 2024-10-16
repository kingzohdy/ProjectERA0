/*
**   script runner main functions
**
**   Jim, 2004.2.18.
**
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "script.h"
#include "decoder.h"


#define VAR_SIZE    40960       //40K bytes
#define CODE_SIZE   204800000   //20M bytes
#define MAX_SCRIPT  1024        /* max script files */



static unsigned char script_variable_buffer[VAR_SIZE];
static unsigned char script_code_buffer[CODE_SIZE];

/*added by kent 2004.7.26, to test logic expression script*/
#ifdef TEST_LOGIC_EXP
	FILE *testfp;
#endif


void* get_script_location(IN char *fname);

static int register_script_location(char *fname, void *ptr);
static void init_script_location();

/*added by kent 2004.7.26, to test script:
IF (1 > 0 OR 2 > 0) AND NOT (3 > 0 or 4 > 0 or not 5 > 0 and 6 > 0 and (7 > 0 or 8 > 0) and 9 > 0)
	CLEAR_TALK 'A'
ELSEIF (1 > 0 OR 2 < 0) AND NOT (3 < 0 or 4 < 0 or not 5 < 0 and 6 < 0 and (7 < 0 or 8 < 0) and 9 < 0)
	ADD_TALK 'V' script_aaa_proc_0_else
ELSEIF (1 < 0 OR 2 > 0) AND NOT (3 < 0 or 4 > 0 or not 5 < 0 and 6 > 0 and (7 < 0 or 8 > 0) and 9 < 0)
	CLEAR_ANI
ELSE
	CLEAR_MAGIC 6
ENDIF
*/
#ifdef TEST_LOGIC_EXP
void testiflogicexp(IN char *script_filename, IN int script_proc_no)
{
	int i;
	int j;
	int num[9];

	testfp = fopen("./testlogicexp", "w+");
	
	for (i = 0; i < 511; i++)
	{
		for (j = 0; j < 9; j++)
		{
			if (((i>>j) & 1) > 0)
			{
				num[8 - j] = 1;
				set_script_variable(9 - j, 1);
			}
			else
			{
				num[8 - j] = 0;
				set_script_variable(9 - j, 0);
			}
		}
		if (((num[0] > 0) || (num[1] > 0)) && !((num[2] > 0) || (num[3] > 0) || !(num[4] > 0) && (num[5] > 0) && ((num[6] > 0) || (num[7] > 0)) && (num[8] > 0)) )
			//printf("true\n");
			fputs("IF true CLEAR_TALK\n", testfp);
		else if (((num[0] > 0) || (num[1] < 0)) && !((num[2] < 0) || (num[3] < 0) || !(num[4] < 0) && (num[5] < 0) && ((num[6] < 0) || (num[7] < 0)) && (num[8] < 0)) )
			//printf("false\n");
			fputs("1ELSEIF true ADD_TALK\n", testfp);
		else if (((num[0] < 0) || (num[1] > 0)) && !((num[2] < 0) || (num[3] > 0) || !(num[4] < 0) && (num[5] > 0) && ((num[6] < 0) || (num[7] > 0)) && (num[8] < 0)) )
			fputs("2ELSEIF true CLEAR_ANI\n", testfp);
		else
			fputs("ELSE CLEAR_MAGIC\n", testfp);
		exec_script(script_filename, script_proc_no);
	}

	return;
}
#endif

int main(int argc, char *argv[])
{
    char script_filename[255];
    int  script_proc_no;
    
    if(argc != 3)
    {
        printf("Usage: %s script_filename proc_no\n\n", argv[0]);
        return -1;
    }
    
    strcpy(script_filename, argv[1]);
    script_proc_no = atoi(argv[2]);
    
    if(0 >= init_script(script_variable_buffer, VAR_SIZE))
    {
        printf("init script system error\n");
        return -2;
    }
    set_script_decoder(my_script_decoder);
    
    int load_size;
    unsigned char *ptr = script_code_buffer;
    int free_size = CODE_SIZE;
    
    int i;
    char fname[][32] = 
    {
        "aaa",
         "bbb",
         "ccc"
    };
    
    for(i=0; i<sizeof(fname)/sizeof(fname[0]); i++)
    {
        load_size = load_script(fname[i], ptr, free_size );
        if(load_size <= 0)
        {
            printf("load script file %s error\n", fname[i]);
            goto _error_exit;
        }
        
        int reg_idx;
        reg_idx = register_script_location(fname[i], ptr);
        if(reg_idx < 0)
        {
            printf("register script %s location error", fname[i]);
            goto _error_exit;
        }
        
        ptr += load_size;
        free_size -= load_size;
    }

#ifdef TEST_LOGIC_EXP
	testiflogicexp(script_filename, script_proc_no);
#endif

#ifndef TEST_LOGIC_EXP
    exec_script(script_filename, script_proc_no);
#endif
    
    free_script();
    return 0;
    
_error_exit:
    free_script();
    return -1;
    
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
typedef struct  stScriptLoc
{
    char fname[32];
    void *ptr;
} TScriptLoc;


static TScriptLoc script_loc_list[MAX_SCRIPT];
static int script_loc_index = 0;

void* get_script_location(IN char *fname)
{
    int i;
    void *ptr = NULL;

    for(i=0; i<script_loc_index; i++)
    {
        if(0 == strcmp(fname, script_loc_list[i].fname))
        {
            ptr = script_loc_list[i].ptr;
            break;
        }
    }

    return ptr;
}


static int register_script_location(char *fname, void *ptr)
{
    if(script_loc_index < MAX_SCRIPT)
    {
        strncpy(script_loc_list[script_loc_index].fname, fname, 31);
        script_loc_list[script_loc_index].ptr = ptr;
        script_loc_index ++;
        return script_loc_index-1;
    }
    else
    {
        return -1;
    }
}


static void init_script_location()
{
    memset(script_loc_list, 0, sizeof(script_loc_list));
    script_loc_index = 0;
}

