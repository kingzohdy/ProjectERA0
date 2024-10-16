/******************************************************************************************
FileName            : timer.h
Author              : ouyang(ouyang jiangping)//Author name here
Version             : 
Date Of Creation    : 2009年11月27日
Description         :
	最小堆实现的定时器
	最小堆指的是满足除了根节点以外的每个节点都不小于其父节点的堆。这样，堆中的最小值就存放在根
	节点中，并且在以某个结点为根的子树中，各节点的值都不小于该子树根节点的值。
	一个最小堆，一般支持以下几种操作：

	Insert(TimerHeap, Timer): 在堆中插入一个值，并保持最小堆性质，具体对应于定时器的实现，则是把定时器插入到定时器堆中。
	根据最小堆的插入算法分析，可以知道该操作的时间复杂度为 O(lgn) 。

	Minimum(TimerHeap): 获取最小堆的中最小值；在定时器系统中，则是返回定时器堆中最先可能终止的定时器。
	由于是最小堆，只需返回堆的 root 即可。此时的算法复杂度为 O(1) 。

	ExtractMin(TimerHeap): 在定时器到期后，执行相关的动作，它的算法复杂度为 O(1) 。

	最小堆本质上是一种最小优先级队列 (min-priority queue) 。定时可以作为最小优先级队列的一个应用，
	该优先级队列把定时器的时间间隔值转化为一个绝对时间来处理，ExtractMin 操则是在所有等待的定时器中，
	找出最先超时的定时器。在任何时候，一个新的定时器实例都可通过 Insert 操作加入到定时器队列中去。

Others              : 
Function List       : 
1.  ......
Modification History: 
1.Date  :
Author  :
Modification  :
******************************************************************************************/

#ifndef _EVS_TIMER_H_
#define _EVS_TIMER_H_

#include <stdio.h>
#include <assert.h>
#include <sys/time.h>
#include "string.h"
#include <stdlib.h>

typedef long							timer_id_t;

#ifndef EINVAL
#define EINVAL		22
#endif /* EINVAL */

static const  long     EINVALIDOP						=1;
static const  long     ENOTFOUND						=2;
static const  long     MAXINT32						=0x7FFFFFFFL;

#define HEAP_PARENT(X)	(X == 0 ? 0 : (((X) - 1) / 2))
#define HEAP_LEFT(X)	(((X)+(X))+1)
#define DEFAULT_MAX_TIMED_OUT_PER_POLL  (64)

#define ASSERT_RETURN(expr,retval)    \
	do { \
	if (!(expr)) { assert(expr); return retval; } \
	} while (0)


void time_val_normalize( struct timeval *t);

#define TIME_VAL_MSEC(t)	((t).tv_sec * 1000 + (t).tv_usec)

#define TIME_VAL_EQ(t1, t2)	((t1).tv_sec==(t2).tv_sec && (t1).tv_usec==(t2).tv_usec)

#define TIME_VAL_GT(t1, t2)	((t1).tv_sec>(t2).tv_sec || \
	((t1).tv_sec==(t2).tv_sec && (t1).tv_usec>(t2).tv_usec))

#define TIME_VAL_GTE(t1, t2)	(TIME_VAL_GT(t1,t2) || \
	TIME_VAL_EQ(t1,t2))

#define TIME_VAL_LT(t1, t2)	(!(TIME_VAL_GTE(t1,t2)))

#define TIME_VAL_LTE(t1, t2)	(!TIME_VAL_GT(t1, t2))

//时间加
#define TIME_VAL_ADD(t1, t2)	    do {			    \
	(t1).tv_sec += (t2).tv_sec;	    \
	(t1).tv_usec += (t2).tv_usec;	    \
	time_val_normalize(&(t1)); \
} while (0)

//时间减
#define TIME_VAL_SUB(t1, t2)	    do {			    \
	(t1).tv_sec -= (t2).tv_sec;	    \
	(t1).tv_usec -= (t2).tv_usec;	    \
	time_val_normalize(&(t1)); \
} while (0)


/** 
* 前置申明下
*/
struct timer_entry;
struct timer_heap_t;

#define TIMER_USER_DATA   256

struct timer_heap_t *ht;

/**
* 回调函数 调用一个已过期的计时器
* @param timer_heap    定时器头部.
* @param entry         已过期定时器.
*/
typedef void timer_heap_callback(struct timer_heap_t *timer_heap,struct timer_entry *entry);


/**
* The implementation of timer heap.
*/
struct timer_heap_t
{
	/** 内存分配器,算暂时丢一边哈 */
	//cpool *pool;

	//用来管理enter的节点
	struct timer_entry *_timer_entry_head;
	struct timer_entry *_timer_entry_end;

	/** 堆的最大个数 */
	size_t max_size;

	/** 当前使用的节点个数. */
	size_t cur_size;

	/** 每次处理到期的最大节点个数 */
	unsigned max_entries_per_poll;

	/** 弄了个信号量锁，暂时用于多线程里面，锁的key最好自己定义下，
	*   多进程要用的话，需要把cpool指向共享内存，等有需求的时候再来弄吧
    */
	//CSemaphore *lock;

	/** 是否自动删除锁 */
	char auto_delete_lock;

	/**
	* 指向头节点的指针，节点的组织方式为一个数组，数组按二叉数的方式来
	* 组织，是个B-tree结构。._timer_id没有使用的时候是-1
	*/
	struct timer_entry **heap;

	/**
	* 一个“指针”数组，使每个timer_entry对应一个ids,其实就是个数据，这个
	* 值指向那个timer_entry._timer_id，一一对应.可以是查找算法控制在O（1）
	* _timer_id的含义 负值*视为“指针”的<freelist_>，>0 值被视为已经使用
	* 并放到<heap_>数组
	*/
	timer_id_t *timer_ids;

	/**
	* 相当于一个指针，指向空闲表的第一个元素
	* 在<timer_ids_>阵列，这是一个有组织的堆栈
	*/
	timer_id_t timer_ids_freelist;

	/**到期回调函数 */
	timer_heap_callback *callback;

};

/**
* 定时器节点定义
*/
struct timer_entry
{
	/** 
	* 前一个没用的指针 NULL表示自己本身在用
	*/
	char *timer_entry_pre_;
	/** 
	* 用户数据项,考虑改掉这个指针
	*/
	char user_data[TIMER_USER_DATA];

	/** 
	* 该计时器在链表中的id或序号
	*/
	long id;

	/** 
	* 到期时间的回调函数 
	*/
	timer_heap_callback *cb;

	/** 
	* 内部独特的计时器ID，它是由定时器堆分配 
	* 应用程序不操作该id
	*/
	timer_id_t _timer_id;

	/** 
	* 到期计时器
	*/
	 struct timeval _timer_value;
};


inline void lock_timer_heap(  );

inline void unlock_timer_heap(  );

void copy_node(struct timer_heap_t *ht, long slot, struct timer_entry *moved_node );

timer_id_t pop_freelist(struct timer_heap_t *ht  );

void push_freelist (struct timer_heap_t *ht, timer_id_t old_id);

void reheap_down(struct timer_heap_t *ht,struct timer_entry *moved_node,
								size_t slot, size_t child);

void reheap_up(struct timer_heap_t *ht, struct timer_entry *moved_node,
							  size_t slot, size_t parent);

struct timer_entry * remove_node(struct timer_heap_t *ht, size_t slot);

void grow_heap(struct timer_heap_t *ht);

void insert_node(struct timer_heap_t *ht,struct timer_entry *new_node);

long schedule_entry(struct timer_heap_t *ht, struct timer_entry *entry, 
								  const  struct timeval *future_time );

long cancel( struct timer_heap_t *ht,struct timer_entry *entry, 
						  long dont_call);

/**
* 计算计时器堆内存大小
*
* @param count     计时器数
* @return          内存的字节大小
*/
size_t timer_heap_mem_size(size_t count);

/**
* 创建一个定时器堆
*
* @param count     初始化定时器个数，定时器不够用时，系统会自动扩展一倍
* @param ht        返回堆头指针
*
* @return         成功，或者相应的错误代码
*/
long timer_heap_create(struct timer_heap_t *ht,size_t count/*,struct timer_heap_t **ht*/);

/**
* 销毁计时器堆，把一些共享内存，锁等销毁
*
* @param ht        堆头.
*/
//void timer_heap_destroy(struct timer_heap_t *ht );


/**
* 设置堆的锁，锁的key如果要进程间信号量锁的化，需要指定key
*
* @param ht        The timer heap.
* @param lock      锁对象用于同步
* @param auto_del  如果非零，堆销毁时也同步销毁锁对象
*                  .
*/
//void timer_heap_set_lock( CSemaphore *lock,
//									 bool auto_del );

/**
* 设置一次处理超时的节点个数
*
* @param ht        The timer heap.
* @param count     Number of entries.
*
* @return          The old number.
*/
unsigned long timer_heap_set_max_timed_out_per_poll(struct timer_heap_t *ht, unsigned count );

/**
* 初始化计时器项
*
* @param entry     计时器节点初始化
* @param id        任意编号分配给用户/此项目的所有者，应用程序可以使用这个
*				   编号来区分多个计时器项，共享相同的回调和USER_DATA
* @param user_data 用户对象数据
* @param len 	   用户对象数据长度
* @param cb        到期回调函数
*
* @return          返回计时器本身
*/
struct timer_entry* timer_entry_init(struct timer_entry *entry,
											 long id,
											 void *user_data,
											 unsigned int len,
											 timer_heap_callback *cb );

/**
* 设置一个到期进度表，哈哈，就是个延时
*
* @param ht        头指针
* @param entry     注册个schedule节点 
* @param delay     过期的时间间隔.
* @return          EVS_SYSRET::RET_SUCC, or the appropriate error code.
*/
long timer_heap_schedule(struct timer_heap_t *ht, struct timer_entry *entry, 
											const  struct timeval *delay);

/**
* 取消以前注册的计时器
*
* @param ht        头指针
* @param entry     取消的节点.
* @return          
*/
long timer_heap_cancel(struct timer_heap_t *ht, struct timer_entry *entry);

/**
* 获取计时器条目数
*
* @param ht        头指针
* @return          计时器的项目数
*/
size_t timer_heap_count( struct timer_heap_t *ht);

/**
* 获取计时器条目数
*
* @param ht        头指针
* @return          当前计数器的总数
*/
size_t timer_head_max(struct timer_heap_t *ht);
/**
* 检查链表是否有到期的节点，调用该函数前，需要先调用timer_heap_count()
* 确定有schedule了节点哈。
*
* @param ht        The timer heap.
* @param  struct timeval   最早的计时器进入的时间期限
*
* @return          EVS_SYSRET::RET_SUCC, or ENOTFOUND if no entry is scheduled.
*/
long timer_heap_earliest_time(struct timer_heap_t *ht,  struct timeval *  timeval);

/**
* 调用到期的schedule节点
*
* @param ht         The timer heap.
* @param next_delay If this parameter is not NULL, it will be filled up with
*		     the time delay until the next timer elapsed, or 
*		     MAXINT32 in the sec part if no entry exist.
*
* @return           The number of timers expired.
*/
unsigned long timer_heap_poll(struct timer_heap_t *ht,  struct timeval *next_delay);


/**
* 调用到期的schedule节点
*
* @param ht         The timer heap.
* @param entry      user's data handle
*
* @return           The number of timers expired.
*/
long  timer_entry_pop_back(struct timer_heap_t *ht,struct timer_entry *entry);
long  timer_entry_push_back(struct timer_heap_t *ht,struct timer_entry *entry);
#endif	/* __TIMER_H__ */
