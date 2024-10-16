/*
**  script.h
**  script system functions header.
**
**  Jim, 2004.2.18.
**
*/
#ifndef __script_h
#define __script_h

#ifdef __cplusplus
extern "C"
{
#endif/* __cplusplus */

#ifndef IN
#define IN
#endif
#ifndef OUT
#define OUT
#endif
#ifndef INOUT
#define INOUT
#endif

    
//Jim, change for test [2/18/2004]
//#define SCRIPT_PATH         "script"	/* directory of script files */
#define SCRIPT_PATH         "."         /* directory of script files */

    
/*
** game flow control values
*/
#define GAME_CONTINUE       0
#define GAME_OVER           2


typedef int (*fnScriptDecoder)(unsigned long code);

int init_script(IN void *var_ptr, IN int size);
void set_script_decoder(IN fnScriptDecoder func);
void free_script(void);

int load_script(IN char* fname, IN void* code_ptr, IN int size);
int exec_script(IN char *fname, IN int proc_no);

int get_script_number(INOUT int *var_idx);
char get_script_char(void);
char* get_script_string();
int get_script_variable(IN int idx, OUT int *val);
int set_script_variable(IN int idx, IN int val);

void debug_log(int before_update, int after_update, char *fmt, ...);

extern void* get_script_location(IN char *fname);



#ifdef __cplusplus
}
#endif/* __cplusplus */

#endif/* __script_h */
