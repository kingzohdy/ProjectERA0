/*
**  @file $RCSfile: tsched.h,v $
**  general description of this module
**  $Id: tsched.h,v 1.2 2009/07/24 05:19:00 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2009/07/24 05:19:00 $
**  @version $Revision: 1.2 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TSCHED_H
#define TSCHED_H

#ifdef WIN32

/* Scheduling algorithms.  */
#define SCHED_OTHER	0
#define SCHED_FIFO	1
#define SCHED_RR	2

/* The official definition.  */
struct sched_param
{
	int __sched_priority;
};

#endif

#endif /* TSCHED_H */
