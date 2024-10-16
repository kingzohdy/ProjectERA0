
#include "comm_timer.h"

//using namespace evslib;

/* CTimer()
{
}
 ~CTimer()
{
}*/

inline void  lock_timer_heap(  )
{
	/*if (ht->lock) {
		ht->lock->Lock();
	}*/
}

inline void  unlock_timer_heap(  )
{
	/*if (ht->lock) {
		ht->lock->Release();
	}*/
}


void  copy_node(struct timer_heap_t *ht, long slot, struct timer_entry *moved_node )
{
	// <moved_node>插入在堆中新的位置
	ht->heap[slot] = moved_node;

	// 更新并行的相应插槽<timer_ids_>的值.一一对应
	ht->timer_ids[moved_node->_timer_id] = slot;
}

 timer_id_t  pop_freelist(struct timer_heap_t *ht  )
{
	// 取一个空闲节点
	timer_id_t new_id = ht->timer_ids_freelist;

	// 在空闲表中的值<timer_ids_>是负的 需要取反，
	// 并且指向下一个空闲下标
	ht->timer_ids_freelist =
		-ht->timer_ids[ht->timer_ids_freelist];

	return new_id;

}

void  push_freelist (struct timer_heap_t *ht, timer_id_t old_id)
{
	// 放进空闲表中，把空闲指针指向头
	ht->timer_ids[old_id] = -ht->timer_ids_freelist;
	ht->timer_ids_freelist = old_id;
}


void  reheap_down(struct timer_heap_t *ht,struct timer_entry *moved_node,
						size_t slot, size_t child)
{
	// 降序
	while (child < ht->cur_size)
	{
		// 选择较小的两个孩子
		if (child + 1 < ht->cur_size
			&& TIME_VAL_LT(ht->heap[child + 1]->_timer_value, ht->heap[child]->_timer_value))
			child++;

		// 执行<copy>如果孩子有一个比<moved_node>大
		if (TIME_VAL_LT(ht->heap[child]->_timer_value, moved_node->_timer_value))
		{
			copy_node(ht,slot, ht->heap[child]);
			slot = child;
			child = HEAP_LEFT(child);
		}
		else
			// We've found our location in the heap.
			break;
	}

	copy_node(ht, slot, moved_node);
}
//加一个使用的节点,B-tree算法,1,2,3的顺序
void  reheap_up(struct timer_heap_t *ht, struct timer_entry *moved_node,
					  size_t slot, size_t parent)
{
	//升序
	while (slot > 0)
	{
		//如果父节点大于<moved_node>我们需要复制下来
		if (TIME_VAL_LT(moved_node->_timer_value, ht->heap[parent]->_timer_value))
		{
			copy_node(ht, slot, ht->heap[parent]);
			slot = parent;
			parent = HEAP_PARENT(slot);
		}
		else
			break;
	}

	//插入其适当堆中的地方和新的节点
	//更新的并行相应插槽<timer_ids>数组。
	copy_node(ht,slot, moved_node);
}


struct timer_entry *  remove_node(struct timer_heap_t *ht, size_t slot)
{
	struct timer_entry *removed_node = ht->heap[slot];

	//回收一个已经使用过的节点
	push_freelist(ht, removed_node->_timer_id );
	// 当前使用的节点减一
	ht->cur_size--;

	// 把_timeer_id设置成-1
	removed_node->_timer_id = -1;

	// 如果slot比当前使用的个数还要小
	// 不是删除末节点的情况下
	if (slot < ht->cur_size)
	{
		long parent;
		struct timer_entry *moved_node = ht->heap[ht->cur_size];

		// 结束节点移动到该位置，删除，更新
		// 并行相应插槽<timer_ids>数组
		copy_node(ht, slot, moved_node);

		// 如果<moved_node-> time_value_“是大于或等于其有需要时提出降低堆
		parent = HEAP_PARENT (slot);

		if (TIME_VAL_GTE(moved_node->_timer_value, ht->heap[parent]->_timer_value))
			reheap_down(ht, moved_node, slot, HEAP_LEFT(slot));
		else
			reheap_up(ht, moved_node, slot, parent);
	}

	return removed_node;
}

void  grow_heap(struct timer_heap_t *ht)
{
	// 扩大2倍
	size_t new_size = ht->max_size * 2;
	timer_id_t *new_timer_ids;
	size_t i;

	// First grow the heap itself.

	struct timer_entry **new_heap = 0;

	new_heap = (struct timer_entry**) 
		//pool_alloc(ht->pool, sizeof(struct timer_entry*) * new_size);
		malloc(sizeof(struct timer_entry*) * new_size);
	memcpy(new_heap, ht->heap, ht->max_size * sizeof(struct timer_entry*));
	//delete [] this->heap_;
	free(ht->heap);
	ht->heap = new_heap;

	// Grow the array of timer ids.

	new_timer_ids = 0;
	new_timer_ids = (timer_id_t*)
		//pool_alloc(ht->pool, new_size * sizeof(timer_id_t));
		malloc(new_size * sizeof(timer_id_t));

	memcpy( new_timer_ids, ht->timer_ids, ht->max_size * sizeof(timer_id_t));
	//delete [] timer_ids_;
	free(ht->timer_ids);
	ht->timer_ids = new_timer_ids;

	// And add the new elements to the end of the "freelist".
	for (i = ht->max_size; i < new_size; i++)
		ht->timer_ids[i] = -((timer_id_t) (i + 1));

	ht->max_size = new_size;
}

void  insert_node(struct timer_heap_t *ht,struct timer_entry *new_node)
{
	if (ht->cur_size + 2 >= ht->max_size)
		grow_heap( ht);

	reheap_up(ht, new_node, ht->cur_size, HEAP_PARENT(ht->cur_size));
	ht->cur_size++;
}


long  schedule_entry(struct timer_heap_t *ht, struct timer_entry *entry, 
								  const  struct timeval *future_time )
{
	if (ht->cur_size < ht->max_size)
	{
		// 获取一个空闲的entry
		// Set the entry
		entry->_timer_id = pop_freelist(ht );
		entry->_timer_value = *future_time;
		insert_node(ht, entry);
		return 0;
	}
	else
		return -1;
}


long  cancel(struct timer_heap_t *ht, struct timer_entry *entry, 
				  long dont_call)
{
	long timer_node_slot;

	// Check to see if the timer_id is out of range
	if (entry->_timer_id < 0 || (size_t)entry->_timer_id > ht->max_size)
		return 0;

	timer_node_slot = ht->timer_ids[entry->_timer_id];

	if (timer_node_slot < 0) // Check to see if timer_id is still valid.
		return 0;

	if (entry != ht->heap[timer_node_slot])
	{
		assert(entry == ht->heap[timer_node_slot]);
		return 0;
	}
	else
	{
		remove_node(ht, timer_node_slot);

		if (dont_call == 0)
			// Call the close hook.
			(*ht->callback)(ht, entry);
		return 1;
	}
}


/*
* 计算内存大小
*/
size_t  timer_heap_mem_size(size_t count)
{
	return /* size of the timer heap itself: */
		sizeof(struct timer_heap_t) + 
		/* size of each entry: */
		(count+2) * (sizeof(struct timer_entry*)+sizeof(timer_id_t)) +
		/* lock, pool etc: */
		132;
}

/*
* 创建一个新的计时器堆
*/
long  timer_heap_create( struct timer_heap_t *ht,size_t size)
{
	size_t i;
	/* Magic? */
	size += 2;

	/* malloc一个头结构，要记得free */
	ht =(struct timer_heap_t*)malloc(sizeof(struct timer_heap_t));

	if (!ht)
		return -1;

	ht->_timer_entry_head = NULL;
	ht->_timer_entry_end = NULL;
	
	/* Initialize timer heap sizes */
	ht->max_size = size;
	ht->cur_size = 0;
	ht->max_entries_per_poll = DEFAULT_MAX_TIMED_OUT_PER_POLL;
	ht->timer_ids_freelist = 1;
	//ht->pool = pool;

	/* Lock. */
	//ht->lock = NULL;
	ht->auto_delete_lock = 0;

	// 创建一个节点数组
	ht->heap = (struct timer_entry**)
		//pool_alloc(pool, sizeof(struct timer_entry*) * size);
		malloc(sizeof(struct timer_entry*) * size);

	if (!ht->heap)
		return -1;

	// 创建和节点并行的链表,用来管理使用和没有使用的struct timer_entry
	ht->timer_ids = (timer_id_t *)
		//pool_alloc( pool, sizeof(timer_id_t) * size);
		malloc(sizeof(timer_id_t) * size);
	if (!ht->timer_ids)
		return -1;

	// 初始化“空闲表”，它使用负值区分开来“到<heap_>指针数组”
	// 空闲表元素。
	for (i=0; i<size; ++i)
		ht->timer_ids[i] = -((timer_id_t) (i + 1));

	//*p_heap = ht;
	return 0;
}
//void  PrintBuf()
//{
//	for (long i=0; i<ht->max_size; ++i)
//	{
//		printf("i:%d, timer_ids:%d \n",i,(long )ht->timer_ids[i]);
//		if(ht->heap[i]!=NULL)
//		printf("i:%d, _timer_value:%d,heap_timer_ids:%d,id:%d \n",i,ht->heap[i]->_timer_value,ht->heap[i]->_timer_id,ht->heap[i]->id);
//
//	}
//
//}

/*void  timer_heap_destroy(struct timer_heap_t *ht)
{	
	
	if (ht->lock && ht->auto_delete_lock) {
		ht->lock = NULL;
	}
}*/

/*void  timer_heap_set_lock(  CSemaphore *lock,
									bool auto_del )
{
	//if (ht->lock && ht->auto_delete_lock)

	ht->lock = lock;
	ht->auto_delete_lock = auto_del;
}*/


unsigned long   timer_heap_set_max_timed_out_per_poll(struct timer_heap_t *ht,unsigned count )
{
	unsigned old_count = ht->max_entries_per_poll;
	ht->max_entries_per_poll = count;
	return old_count;
}

 struct timer_entry*  timer_entry_init(struct timer_entry *entry,
											long id,
											void *user_data,
											unsigned int len,
											timer_heap_callback *cb )
{
	assert(entry && cb);

	entry->_timer_id = -1;
	entry->id = id;
	if(len>TIMER_USER_DATA)
	{
		memcpy(entry->user_data,user_data,TIMER_USER_DATA);
	}
	else
	{
		memcpy(entry->user_data,user_data,len);
	}
	entry->cb = cb;

	return entry;
}

long  timer_heap_schedule(struct timer_heap_t *ht, struct timer_entry *entry, 
										   const  struct timeval *delay)
{
	long status;
	struct timeval expires;

	ASSERT_RETURN(ht && entry && delay, EINVAL);
	ASSERT_RETURN(entry->cb != NULL, EINVAL);

	/* check下该节点是否已经被用了，没有用是-1 */
	ASSERT_RETURN(entry->_timer_id < 1, EINVALIDOP);

	/*取现在的时间，加上到期时间增量*/
	memset(&expires,0,sizeof(expires));
	gettimeofday(&expires,NULL);
	TIME_VAL_ADD(expires, *delay);

	lock_timer_heap( );
	status = schedule_entry(ht, entry, &expires);
	unlock_timer_heap( );

	return status;
}

long  timer_heap_cancel(struct timer_heap_t *ht, struct timer_entry *entry)
{
	long count;

	ASSERT_RETURN(ht && entry, EINVAL);

	lock_timer_heap( );
	count = cancel(ht, entry, 1);
	unlock_timer_heap( );

	return count;
}

unsigned long   timer_heap_poll(struct timer_heap_t *ht,  struct timeval *next_delay )
{
	 struct timeval now;
	unsigned count;

	ASSERT_RETURN(ht, 0);

	if (!ht->cur_size && next_delay) {
		next_delay->tv_sec = next_delay->tv_usec = MAXINT32;
		return 0;
	}

	count = 0;
	gettimeofday(&now,NULL);

	lock_timer_heap( );
	while ( ht->cur_size && 
		TIME_VAL_LTE(ht->heap[0]->_timer_value, now) &&
		count < ht->max_entries_per_poll ) 
	{
		struct timer_entry *node = remove_node(ht,0);
		++count;

		unlock_timer_heap( );
		if (node->cb)
			(*node->cb)(ht, node);
		lock_timer_heap( );
	}
	if (ht->cur_size && next_delay) {
		*next_delay = ht->heap[0]->_timer_value;
		TIME_VAL_SUB(*next_delay, now);
		if (next_delay->tv_sec < 0 || next_delay->tv_usec < 0)
			next_delay->tv_sec = next_delay->tv_usec = 0;
	} else if (next_delay) {
		next_delay->tv_sec = next_delay->tv_usec = MAXINT32;
	}
	unlock_timer_heap( );

	return count;
}

size_t  timer_heap_count(struct timer_heap_t *ht)
{
	ASSERT_RETURN(ht, 0);

	return ht->cur_size;
}

size_t  timer_head_max(struct timer_heap_t *ht)
{
	return ht->max_size;
}
long  timer_heap_earliest_time(struct timer_heap_t *ht,  struct timeval *  timeval)
{
	assert(ht->cur_size != 0);
	if (ht->cur_size == 0)
		return ENOTFOUND;

	lock_timer_heap( );
	* timeval = ht->heap[0]->_timer_value;
	unlock_timer_heap( );

	return 0;
}

void  time_val_normalize( struct timeval *t)
{

	if (t->tv_usec >= 1000000) {
		t->tv_sec += (t->tv_usec / 1000000);
		t->tv_usec = (t->tv_usec % 1000000);
	}
	else if (t->tv_usec <= -1000000) {
		do {
			t->tv_sec--;
			t->tv_usec += 1000000;
		} while (t->tv_usec <= -1000000);
	}

	if (t->tv_sec >= 1 && t->tv_usec < 0) {
		t->tv_sec--;
		t->tv_usec += 1000000;

	} else if (t->tv_sec < 0 && t->tv_usec > 0) {
		t->tv_sec++;
		t->tv_usec -= 1000000;
	}
}

long  timer_entry_pop_back(struct timer_heap_t *ht,struct timer_entry *entry)
{
	size_t i = 0;
	if(ht->_timer_entry_head == ht->_timer_entry_end)
	{
		//不够用的话，需要自己来扩充,汗杯具
		for(i=0;i<timer_head_max(ht);i++)
		{
			struct timer_entry *new_entry;
			new_entry=malloc(sizeof(struct timer_entry));
			if(!new_entry)
			{
				entry = NULL;
				return -1;
			}
			memset(new_entry,0,sizeof(struct timer_entry));
			if(ht->_timer_entry_head == NULL ||
			   ht->_timer_entry_end == NULL)
			{
				ht->_timer_entry_head = ht->_timer_entry_end = new_entry;
			}
			else
			{
				ht->_timer_entry_head->timer_entry_pre_ = (char *)new_entry;
				ht->_timer_entry_head = new_entry;
			}
		}
	}
	if(ht->_timer_entry_head == NULL ||
	   ht->_timer_entry_end == NULL  ||
	   ht->_timer_entry_head == ht->_timer_entry_end )
	{
		entry = NULL;
		return -1;
	}

	entry = ht->_timer_entry_end;
	ht->_timer_entry_end = (struct timer_entry *)ht->_timer_entry_end->timer_entry_pre_;
	
	return 0;
}

long  timer_entry_push_back(struct timer_heap_t *ht,struct timer_entry *entry)
{

	ht->_timer_entry_head->timer_entry_pre_ = (char *)entry;
	ht->_timer_entry_head = entry;
	
	return 0;
}
