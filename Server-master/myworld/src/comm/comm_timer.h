/******************************************************************************************
FileName            : timer.h
Author              : ouyang(ouyang jiangping)//Author name here
Version             : 
Date Of Creation    : 2009��11��27��
Description         :
	��С��ʵ�ֵĶ�ʱ��
	��С��ָ����������˸��ڵ������ÿ���ڵ㶼��С���丸�ڵ�Ķѡ����������е���Сֵ�ʹ���ڸ�
	�ڵ��У���������ĳ�����Ϊ���������У����ڵ��ֵ����С�ڸ��������ڵ��ֵ��
	һ����С�ѣ�һ��֧�����¼��ֲ�����

	Insert(TimerHeap, Timer): �ڶ��в���һ��ֵ����������С�����ʣ������Ӧ�ڶ�ʱ����ʵ�֣����ǰѶ�ʱ�����뵽��ʱ�����С�
	������С�ѵĲ����㷨����������֪���ò�����ʱ�临�Ӷ�Ϊ O(lgn) ��

	Minimum(TimerHeap): ��ȡ��С�ѵ�����Сֵ���ڶ�ʱ��ϵͳ�У����Ƿ��ض�ʱ���������ȿ�����ֹ�Ķ�ʱ����
	��������С�ѣ�ֻ�践�ضѵ� root ���ɡ���ʱ���㷨���Ӷ�Ϊ O(1) ��

	ExtractMin(TimerHeap): �ڶ�ʱ�����ں�ִ����صĶ����������㷨���Ӷ�Ϊ O(1) ��

	��С�ѱ�������һ����С���ȼ����� (min-priority queue) ����ʱ������Ϊ��С���ȼ����е�һ��Ӧ�ã�
	�����ȼ����аѶ�ʱ����ʱ����ֵת��Ϊһ������ʱ��������ExtractMin �����������еȴ��Ķ�ʱ���У�
	�ҳ����ȳ�ʱ�Ķ�ʱ�������κ�ʱ��һ���µĶ�ʱ��ʵ������ͨ�� Insert �������뵽��ʱ��������ȥ��

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

//ʱ���
#define TIME_VAL_ADD(t1, t2)	    do {			    \
	(t1).tv_sec += (t2).tv_sec;	    \
	(t1).tv_usec += (t2).tv_usec;	    \
	time_val_normalize(&(t1)); \
} while (0)

//ʱ���
#define TIME_VAL_SUB(t1, t2)	    do {			    \
	(t1).tv_sec -= (t2).tv_sec;	    \
	(t1).tv_usec -= (t2).tv_usec;	    \
	time_val_normalize(&(t1)); \
} while (0)


/** 
* ǰ��������
*/
struct timer_entry;
struct timer_heap_t;

#define TIMER_USER_DATA   256

struct timer_heap_t *ht;

/**
* �ص����� ����һ���ѹ��ڵļ�ʱ��
* @param timer_heap    ��ʱ��ͷ��.
* @param entry         �ѹ��ڶ�ʱ��.
*/
typedef void timer_heap_callback(struct timer_heap_t *timer_heap,struct timer_entry *entry);


/**
* The implementation of timer heap.
*/
struct timer_heap_t
{
	/** �ڴ������,����ʱ��һ�߹� */
	//cpool *pool;

	//��������enter�Ľڵ�
	struct timer_entry *_timer_entry_head;
	struct timer_entry *_timer_entry_end;

	/** �ѵ������� */
	size_t max_size;

	/** ��ǰʹ�õĽڵ����. */
	size_t cur_size;

	/** ÿ�δ����ڵ����ڵ���� */
	unsigned max_entries_per_poll;

	/** Ū�˸��ź���������ʱ���ڶ��߳����棬����key����Լ������£�
	*   �����Ҫ�õĻ�����Ҫ��cpoolָ�����ڴ棬���������ʱ������Ū��
    */
	//CSemaphore *lock;

	/** �Ƿ��Զ�ɾ���� */
	char auto_delete_lock;

	/**
	* ָ��ͷ�ڵ��ָ�룬�ڵ����֯��ʽΪһ�����飬���鰴�������ķ�ʽ��
	* ��֯���Ǹ�B-tree�ṹ��._timer_idû��ʹ�õ�ʱ����-1
	*/
	struct timer_entry **heap;

	/**
	* һ����ָ�롱���飬ʹÿ��timer_entry��Ӧһ��ids,��ʵ���Ǹ����ݣ����
	* ֵָ���Ǹ�timer_entry._timer_id��һһ��Ӧ.�����ǲ����㷨������O��1��
	* _timer_id�ĺ��� ��ֵ*��Ϊ��ָ�롱��<freelist_>��>0 ֵ����Ϊ�Ѿ�ʹ��
	* ���ŵ�<heap_>����
	*/
	timer_id_t *timer_ids;

	/**
	* �൱��һ��ָ�룬ָ����б�ĵ�һ��Ԫ��
	* ��<timer_ids_>���У�����һ������֯�Ķ�ջ
	*/
	timer_id_t timer_ids_freelist;

	/**���ڻص����� */
	timer_heap_callback *callback;

};

/**
* ��ʱ���ڵ㶨��
*/
struct timer_entry
{
	/** 
	* ǰһ��û�õ�ָ�� NULL��ʾ�Լ���������
	*/
	char *timer_entry_pre_;
	/** 
	* �û�������,���Ǹĵ����ָ��
	*/
	char user_data[TIMER_USER_DATA];

	/** 
	* �ü�ʱ���������е�id�����
	*/
	long id;

	/** 
	* ����ʱ��Ļص����� 
	*/
	timer_heap_callback *cb;

	/** 
	* �ڲ����صļ�ʱ��ID�������ɶ�ʱ���ѷ��� 
	* Ӧ�ó��򲻲�����id
	*/
	timer_id_t _timer_id;

	/** 
	* ���ڼ�ʱ��
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
* �����ʱ�����ڴ��С
*
* @param count     ��ʱ����
* @return          �ڴ���ֽڴ�С
*/
size_t timer_heap_mem_size(size_t count);

/**
* ����һ����ʱ����
*
* @param count     ��ʼ����ʱ����������ʱ��������ʱ��ϵͳ���Զ���չһ��
* @param ht        ���ض�ͷָ��
*
* @return         �ɹ���������Ӧ�Ĵ������
*/
long timer_heap_create(struct timer_heap_t *ht,size_t count/*,struct timer_heap_t **ht*/);

/**
* ���ټ�ʱ���ѣ���һЩ�����ڴ棬��������
*
* @param ht        ��ͷ.
*/
//void timer_heap_destroy(struct timer_heap_t *ht );


/**
* ���öѵ���������key���Ҫ���̼��ź������Ļ�����Ҫָ��key
*
* @param ht        The timer heap.
* @param lock      ����������ͬ��
* @param auto_del  ������㣬������ʱҲͬ������������
*                  .
*/
//void timer_heap_set_lock( CSemaphore *lock,
//									 bool auto_del );

/**
* ����һ�δ���ʱ�Ľڵ����
*
* @param ht        The timer heap.
* @param count     Number of entries.
*
* @return          The old number.
*/
unsigned long timer_heap_set_max_timed_out_per_poll(struct timer_heap_t *ht, unsigned count );

/**
* ��ʼ����ʱ����
*
* @param entry     ��ʱ���ڵ��ʼ��
* @param id        �����ŷ�����û�/����Ŀ�������ߣ�Ӧ�ó������ʹ�����
*				   ��������ֶ����ʱ���������ͬ�Ļص���USER_DATA
* @param user_data �û���������
* @param len 	   �û��������ݳ���
* @param cb        ���ڻص�����
*
* @return          ���ؼ�ʱ������
*/
struct timer_entry* timer_entry_init(struct timer_entry *entry,
											 long id,
											 void *user_data,
											 unsigned int len,
											 timer_heap_callback *cb );

/**
* ����һ�����ڽ��ȱ����������Ǹ���ʱ
*
* @param ht        ͷָ��
* @param entry     ע���schedule�ڵ� 
* @param delay     ���ڵ�ʱ����.
* @return          EVS_SYSRET::RET_SUCC, or the appropriate error code.
*/
long timer_heap_schedule(struct timer_heap_t *ht, struct timer_entry *entry, 
											const  struct timeval *delay);

/**
* ȡ����ǰע��ļ�ʱ��
*
* @param ht        ͷָ��
* @param entry     ȡ���Ľڵ�.
* @return          
*/
long timer_heap_cancel(struct timer_heap_t *ht, struct timer_entry *entry);

/**
* ��ȡ��ʱ����Ŀ��
*
* @param ht        ͷָ��
* @return          ��ʱ������Ŀ��
*/
size_t timer_heap_count( struct timer_heap_t *ht);

/**
* ��ȡ��ʱ����Ŀ��
*
* @param ht        ͷָ��
* @return          ��ǰ������������
*/
size_t timer_head_max(struct timer_heap_t *ht);
/**
* ��������Ƿ��е��ڵĽڵ㣬���øú���ǰ����Ҫ�ȵ���timer_heap_count()
* ȷ����schedule�˽ڵ����
*
* @param ht        The timer heap.
* @param  struct timeval   ����ļ�ʱ�������ʱ������
*
* @return          EVS_SYSRET::RET_SUCC, or ENOTFOUND if no entry is scheduled.
*/
long timer_heap_earliest_time(struct timer_heap_t *ht,  struct timeval *  timeval);

/**
* ���õ��ڵ�schedule�ڵ�
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
* ���õ��ڵ�schedule�ڵ�
*
* @param ht         The timer heap.
* @param entry      user's data handle
*
* @return           The number of timers expired.
*/
long  timer_entry_pop_back(struct timer_heap_t *ht,struct timer_entry *entry);
long  timer_entry_push_back(struct timer_heap_t *ht,struct timer_entry *entry);
#endif	/* __TIMER_H__ */
