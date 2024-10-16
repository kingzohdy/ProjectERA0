/*
**  @file $RCSfile: tsched.h,v $
**  general description of this module
**  $Id: tsched.h,v 1.3 2012/07/27 05:09:05 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2012/07/27 05:09:05 $
**  @version $Revision: 1.3 $
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
