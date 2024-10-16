/*
**  @file $RCSfile: plugin.h,v $
**  general description of this module
**  $Id: plugin.h,v 1.3 2012/07/27 05:09:10 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2012/07/27 05:09:10 $
**  @version $Revision: 1.3 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef __PLUGIN__
#define __PLUGIN__

#ifdef __cplusplus
extern "C" {
#endif

struct agent_base;
struct tagPkgHead;

typedef struct plugin
{
	char *name; 		/* name of the plugin */
	char *version;      /* version */
	
	int (* init)		(struct plugin *p);
	int (* cleanup)	(struct plugin *p);
	int (* timer)		(struct plugin *p);

	/*ptr with agent head */
	int (* trigger)	(struct plugin *p, char *ptr, size_t size);
	int (* write)		(struct plugin *p, char *ptr, size_t size);

	/* ptr without agent head*/
	int (* recv)		(struct plugin *p, char *ptr, size_t size, struct tagPkgHead *ahead);
	int (* send)		(struct plugin *p, char *ptr, size_t size, struct tagPkgHead *ahead);
	
	void *data;
	/* dlopen handle */
	void *lib;
	void *base;
	
	/* tlog */
	void *log;
	/* appid */
	unsigned int id;
	
} plugin;

int  plugins_load		(struct agent_base *pstBase);
void plugins_free		(struct agent_base *pstBase);
int plugins_call_init		(struct agent_base *pstBase);
int plugins_call_timer	(struct agent_base *pstBase);
void plugins_debug		(struct agent_base *pstBase);

#ifdef __cplusplus
}
#endif

#endif /* __PLUGIN__ */

