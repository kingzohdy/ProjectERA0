/*
**  @file $RCSfile: tsched.h,v $
**  general description of this module
**  $Id: tsched.h,v 1.1.1.1 2011/01/04 11:02:51 niko Exp $
**  @author $Author: niko $
**  @date $Date: 2011/01/04 11:02:51 $
**  @version $Revision: 1.1.1.1 $
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
