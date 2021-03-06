/* RTX includes */
#include "osdep_service.h"
//#include "cmsis_rtos_service.h"
#include "platform_stdlib.h"
#include "cmsis_os.h"
#include "memory.h"
//#include "hal_timer.h"
//#include "tcm_heap.h"
//#include <core_cmFunc.h>
//#include <stdlib.h>//malloc(), free()
//#include <string.h>//memcpy(), memcmp(), memset()
//#include <rt_HAL_CM.h>
//#include <RTX_CM_lib.h>
/********************* os depended utilities ********************/

#define SECTION_RTX_SERVICE_STUBS          SECTION(".rom.rtx_service.stubs")

#ifndef USE_MUTEX_FOR_SPINLOCK
#define USE_MUTEX_FOR_SPINLOCK 1
#endif

#define USE_HEAP_INFO	0

#define OS_TICK        1000
#define OS_TICK_RATE_MS (1000/OS_TICK)

extern void hal_misc_delay_us_rtl8195blp (u32 time_us);

extern void rt_tsk_lock   (void);
extern void rt_tsk_unlock (void);
extern u32 os_time;

//-----------------------------------------------------------------------
// Private Variables
//-----------------------------------------------------------------------
static unsigned long CriticalNesting = 0;

//-----------------------------------------------------------------------
// Misc Function
//-----------------------------------------------------------------------
#if 0
int osdep_print = 0;
#define _func_enter_ do{\
						if(osdep_print)\
							printf("enter %s\r\n", __FUNCTION__);\
					}while(0)
#define _func_exit_ do{\
						if(osdep_print)\
							printf("exit %s\r\n", __FUNCTION__);\
				     }while(0)
#else
#define _func_enter_
#define _func_exit_
#endif

static void _rtx_enter_critical(_lock *plock, _irqL *pirqL);
static void _rtx_exit_critical(_lock *plock, _irqL *pirqL);
static u32 _rtx_ms_to_systime(u32 ms);

void save_and_cli()
{
_func_enter_;
#if defined(__CC_ARM)
	_rtx_enter_critical(NULL, NULL);
#else
	__disable_irq();
#endif
_func_exit_;
}

void restore_flags()
{
_func_enter_;
#if defined(__CC_ARM)
	_rtx_exit_critical(NULL, NULL);
#else
	__enable_irq();
#endif
_func_exit_;
}

void cli()
{
_func_enter_;
	__disable_irq();
_func_exit_;
}

/* Not needed on 64bit architectures */
static unsigned int __div64_32(u64 *n, unsigned int base)
{
	u64 rem = *n;
	u64 b = base;
	u64 res, d = 1;
	unsigned int high = rem >> 32;
_func_enter_;
	/* Reduce the thing a bit first */
	res = 0;
	if (high >= base) {
		high /= base;
		res = (u64) high << 32;
		rem -= (u64) (high * base) << 32;
	}

	while ((u64)b > 0 && b < rem) {
		b = b+b;
		d = d+d;
	}

	do {
		if (rem >= b) {
			rem -= b;
			res += d;
		}
		b >>= 1;
		d >>= 1;
	} while (d);
_func_exit_;
	*n = res;
	return rem;
}

/********************* os depended service ********************/

#define _rtx_memcpy     _memcpy
#define _rtx_memset     _memset

typedef void *(*malloc_t)(u32 size);
typedef void (*mfree_t)(void *p);

static malloc_t pmalloc_func = NULL;
static mfree_t pmfree_func = NULL;

void rtw_set_mem_func (void *malloc, void *mfree)
{
    pmalloc_func = (malloc_t)malloc;
    pmfree_func = (mfree_t)mfree;
}

u8* _rtx_malloc(u32 sz)
{
_func_enter_;
	void *p = NULL;
	p = (void *)pmalloc_func(sz);
_func_exit_;
	return p;
}

u8* _rtx_zmalloc(u32 sz)
{
_func_enter_;
	u8 *pbuf = _rtx_malloc(sz);

	if (pbuf != NULL){
		_rtx_memset(pbuf, 0, sz);
	}
_func_exit_;
	return pbuf;	
}

void _rtx_mfree(u8 *pbuf, u32 sz)
{
    pmfree_func(pbuf);
}

static int _rtx_memcmp(void *dst, void *src, u32 sz)
{
_func_enter_;
//under Linux/GNU/GLibc, the return value of memcmp for two same mem. chunk is 0
	if (!(_memcmp(dst, src, sz)))
		return _SUCCESS;
_func_exit_;
	return _FAIL;
}

static void _rtx_init_sema(_sema *sem, int init_val)
{
_func_enter_;
	rtx_sema_t *p_sem = (rtx_sema_t *)_rtx_zmalloc(sizeof(rtx_sema_t));
	if(p_sem == NULL){
		goto err_exit;
	}
#ifdef CMSIS_OS_RTX
	p_sem->def.semaphore = p_sem->data;
#endif
	*sem = (_sema)p_sem;
	p_sem->id = osSemaphoreCreate(&p_sem->def, init_val);
	if (p_sem->id == NULL){
		goto err_exit;
	}
_func_exit_;
	return;
err_exit:
	DBG_MISC_ERR("error");
	if(p_sem)
		_rtx_mfree((u8 *)p_sem, sizeof(rtx_sema_t));
	*sem = NULL;
	return;
}

static void _rtx_free_sema(_sema *sema)
{
_func_enter_;
	if(*sema){
		rtx_sema_t *p_sem = (rtx_sema_t *)(*sema);
		osSemaphoreDelete(p_sem->id);
		if(p_sem)
			_rtx_mfree((u8 *)p_sem, sizeof(rtx_sema_t));
		*sema = NULL;
	}else
		DBG_MISC_ERR("NULL pointer get");
_func_exit_;
}

static void _rtx_up_sema(_sema *sema)
{
	if(*sema){
		rtx_sema_t *p_sem = (rtx_sema_t *)(*sema);
		osStatus status = osSemaphoreRelease(p_sem->id);
		if ( status != osOK){
			DBG_MISC_ERR("error %d", status);
		}
	}else
		DBG_MISC_ERR("NULL pointer get");
_func_exit_;
}

static void _rtx_up_sema_from_isr(_sema *sema)
{
_func_enter_;
	if(*sema){
		rtx_sema_t *p_sem = (rtx_sema_t *)*sema;
		osStatus status = osSemaphoreRelease(p_sem->id);
		if (status != osOK){
			DBG_MISC_ERR("error %d", status);
		}
	}else
		DBG_MISC_ERR("NULL pointer get");
_func_exit_;
}

static u32 _rtx_down_sema(_sema *sema, u32 timeout_ms)
{
	if(*sema){
		rtx_sema_t *p_sem = (rtx_sema_t *)*sema;
		if(timeout_ms == RTW_MAX_DELAY) {
			timeout_ms = osWaitForever;
		} else {
			timeout_ms = _rtx_ms_to_systime(timeout_ms);
		}

		if (osSemaphoreWait(p_sem->id, (timeout_ms != 0)?(timeout_ms):(osWaitForever)) >= 0)
			return _TRUE;
	}
	return _FALSE;
}

static void _rtx_mutex_init(_mutex *mutex)
{
_func_enter_;
	rtx_mutex_t *p_mut = (rtx_mutex_t *)_rtx_zmalloc(sizeof(rtx_mutex_t));
	if(p_mut == NULL)
		goto err_exit;
#ifdef CMSIS_OS_RTX
	p_mut->def.mutex = (void *)p_mut->data;
#endif
	*mutex = (_mutex)p_mut;
	p_mut->id = osMutexCreate(&p_mut->def);
	if (p_mut->id == NULL)
		goto err_exit;
_func_exit_;
	return;
err_exit:
	DBG_MISC_ERR("error");
	if(p_mut)
		_rtx_mfree((u8 *)p_mut, sizeof(rtx_mutex_t));
	*mutex = NULL;
	return;
}

static void _rtx_mutex_free(_mutex *pmutex)
{
_func_enter_;
	if(*pmutex){
		rtx_mutex_t *p_mut = (rtx_mutex_t *)(*pmutex);
		osMutexDelete(p_mut->id);
		if(p_mut)
			_rtx_mfree((u8 *)p_mut, sizeof(rtx_mutex_t));
	}
_func_exit_;
}

static void _rtx_mutex_get(_mutex *pmutex)
{
_func_enter_;
	if(*pmutex){
		rtx_mutex_t *p_mut = (rtx_mutex_t *)(*pmutex);
		if (osMutexWait(p_mut->id, 60 * 1000 / OS_TICK_RATE_MS) != osOK)
			DBG_MISC_ERR("%s(%p) failed, retry\n",  __FUNCTION__, p_mut);
	}
_func_exit_;
} 

static int _rtx_mutex_get_timeout(_mutex *pmutex, u32 timeout_ms)
{
_func_enter_;
	if(*pmutex){
		rtx_mutex_t *p_mut = (rtx_mutex_t *)(*pmutex);
		if(timeout_ms == RTW_MAX_DELAY) {
			timeout_ms = osWaitForever;
		} else {
			timeout_ms = _rtx_ms_to_systime(timeout_ms);
		}
		if(osMutexWait(p_mut->id, timeout_ms / OS_TICK_RATE_MS) == osOK){	
			return _SUCCESS;
		}
	}
_func_exit_;
	DBG_MISC_ERR("%s(%p) failed, retry\n",  __FUNCTION__, pmutex);
	return _FAIL;
}

static void _rtx_mutex_put(_mutex *pmutex)
{
_func_enter_;
	if(*pmutex){
		rtx_mutex_t *p_mut = (rtx_mutex_t *)(*pmutex);
		if (osMutexRelease(p_mut->id) != osOK)
			DBG_MISC_ERR("\r\ninternal counter of mutex is 0 or calling task is not the owner of the mutex");
	}
_func_exit_;
}

static void _rtx_enter_critical(_lock *plock, _irqL *pirqL)
{
_func_enter_;
	CriticalNesting++;
	if(CriticalNesting == 1){
		rt_tsk_lock();//tsk_lock & tsk_unlock should not be called nested
	}
_func_exit_;
}

static void _rtx_exit_critical(_lock *plock, _irqL *pirqL)
{
_func_enter_;
	if(CriticalNesting == 0){
		DBG_MISC_ERR("die here");
		HALT();
	}
	CriticalNesting--;
	if(CriticalNesting == 0){
		rt_tsk_unlock();
	}
_func_exit_;
} 

static void _rtx_enter_critical_from_isr(_lock *plock, _irqL *pirqL)
{
_func_enter_;
	__disable_irq();
_func_exit_;
}

static void _rtx_exit_critical_from_isr(_lock *plock, _irqL *pirqL)
{
_func_enter_;
	__enable_irq();
_func_exit_;
}

static int _rtx_enter_critical_mutex(_mutex *pmutex, _irqL *pirqL)
{
_func_enter_;
	while(_rtx_mutex_get_timeout(pmutex, 60 * 1000 / OS_TICK_RATE_MS) != _SUCCESS)
		DBG_MISC_ERR("\n\r[%p] %s(%p) failed, retry\n", osThreadGetId(), __FUNCTION__, pmutex);
_func_exit_;
	return _SUCCESS;
}

static void _rtx_exit_critical_mutex(_mutex *pmutex, _irqL *pirqL)
{
_func_enter_;
	_rtx_mutex_put(pmutex);
_func_exit_;
}

static void _rtx_spinlock_init(_lock *plock)
{
_func_enter_;
#if USE_MUTEX_FOR_SPINLOCK
	_rtx_mutex_init(plock);
#endif
_func_exit_;
}

static void _rtx_spinlock_free(_lock *plock)
{
_func_enter_;
#if USE_MUTEX_FOR_SPINLOCK
	if(plock != NULL){
		_rtx_mutex_free(plock);
	}
#endif
_func_exit_;
}

static void _rtx_spinlock(_lock *plock)
{
_func_enter_;
#if USE_MUTEX_FOR_SPINLOCK
	_rtx_mutex_get(plock);
#endif
_func_exit_;
}

static void _rtx_spinunlock(_lock *plock)
{
_func_enter_;
#if USE_MUTEX_FOR_SPINLOCK
	_rtx_mutex_put(plock);
#endif
_func_exit_;
}

static void _rtx_spinlock_irqsave(_lock *plock, _irqL *irqL)
{
_func_enter_;
	_rtx_enter_critical(plock, irqL);
#if USE_MUTEX_FOR_SPINLOCK
	_rtx_spinlock(plock);
#endif
_func_exit_;
}

static void _rtx_spinunlock_irqsave(_lock *plock, _irqL *irqL)
{
_func_enter_;
#if USE_MUTEX_FOR_SPINLOCK
	_rtx_spinunlock(plock);
#endif
	_rtx_exit_critical(plock, irqL);
_func_exit_;
}

static int _rtx_init_xqueue( _xqueue* queue, const char* name, u32 message_size, u32 number_of_messages )
{
_func_enter_;
	rtx_mbox_t *mbox = (rtx_mbox_t *)_rtx_zmalloc(sizeof(rtx_mbox_t));
	if (mbox == NULL ){
		goto err_exit;
	}
#ifdef CMSIS_OS_RTX
	mbox->os_mailQ_q = (uint32_t)_rtx_zmalloc((4+number_of_messages)*sizeof(uint32_t));
	mbox->os_mailQ_m = (uint32_t)_rtx_zmalloc((3 + (message_size+3)/4)*number_of_messages);
	if((mbox->os_mailQ_q == 0) || (mbox->os_mailQ_m == 0))
		goto err_exit;
	mbox->os_mailQ_p[0] = (void *)mbox->os_mailQ_q;
	mbox->os_mailQ_p[1] = (void *)mbox->os_mailQ_m;
	mbox->def.pool = mbox->os_mailQ_p;
    mbox->def.queue_sz = number_of_messages;
    mbox->def.item_sz = message_size;
#endif
	*queue = (_xqueue)mbox;
	mbox->id = osMailCreate(&mbox->def, NULL);
	if(mbox->id == NULL)
		goto err_exit;
_func_exit_;
	return _SUCCESS;
err_exit:
	DBG_MISC_ERR("%s error\r\n", __FUNCTION__);
	if(mbox){
		if(mbox->os_mailQ_q)
			_rtx_mfree((u8 *)mbox->os_mailQ_q, ((4+number_of_messages)*sizeof(uint32_t)));
		if(mbox->os_mailQ_m)
			_rtx_mfree((u8 *)mbox->os_mailQ_m, ((3 + (message_size+3)/4)*number_of_messages));
		_rtx_mfree((u8 *)mbox, sizeof(rtx_mbox_t));
		*queue = NULL;
	}
	return _FAIL;
}

static int _rtx_push_to_xqueue( _xqueue* queue, void* message, u32 timeout_ms )
{
_func_enter_;
	void *mptr;
	rtx_mbox_t *mbox;
	if(timeout_ms == RTW_MAX_DELAY) {
	      timeout_ms = osWaitForever;
	} else {
	      timeout_ms = _rtx_ms_to_systime(timeout_ms);
	}
	
	if (*queue != NULL){
		mbox = (rtx_mbox_t *)(*queue);
		mptr = osMailAlloc(mbox->id, timeout_ms);       // Allocate memory
		_rtx_memcpy(mptr, message, mbox->def.item_sz);
		if(osMailPut(mbox->id, mptr) != osOK ){
			DBG_MISC_ERR("%s error\n", __FUNCTION__);
			return _FAIL;
		}
    	}
_func_exit_;
	return _SUCCESS;
}

static int _rtx_pop_from_xqueue( _xqueue* queue, void* message, u32 timeout_ms )
{
_func_enter_;
	if(timeout_ms == RTW_WAIT_FOREVER) {
		timeout_ms = osWaitForever;
	} else {
		timeout_ms = _rtx_ms_to_systime(timeout_ms);
	}
	if (*queue != NULL){
		rtx_mbox_t *mbox = (rtx_mbox_t *)(*queue);
		osEvent  evt;
		evt = osMailGet(mbox->id,  timeout_ms );
		if (evt.status == osEventMail) {
			_rtx_memcpy(message, evt.value.p, mbox->def.item_sz);
			osMailFree(mbox->id, evt.value.p);
_func_exit_;
			return _SUCCESS;
		}
	}
	DBG_MISC_ERR("[%p] %s error", osThreadGetId(), __FUNCTION__);
	return _FAIL;
}

static int _rtx_deinit_xqueue( _xqueue* queue )
{
_func_enter_;
	if(*queue != NULL){
		rtx_mbox_t *mbox = (rtx_mbox_t *)(*queue);
		
		if(mbox->os_mailQ_q)
			_rtx_mfree((u8 *)mbox->os_mailQ_q, ((4+mbox->def.queue_sz)*sizeof(uint32_t)));
		if(mbox->os_mailQ_m)
			_rtx_mfree((u8 *)mbox->os_mailQ_m, ((3 + (mbox->def.item_sz+3)/4)*mbox->def.queue_sz));
		_rtx_mfree((u8 *)mbox, sizeof(rtx_mbox_t));
		*queue = NULL;
	}
_func_exit_;
    return 0;
}

static u32 _rtx_get_current_time(void)
{
_func_enter_;
    return os_time;
_func_exit_;
}

static u32 _rtx_systime_to_ms(u32 systime)
{
	return systime * OS_TICK_RATE_MS;
}

static u32 _rtx_systime_to_sec(u32 systime)
{
	return systime / OS_TICK;
}

static u32 _rtx_ms_to_systime(u32 ms)
{
	return ms / OS_TICK_RATE_MS;
}

static u32 _rtx_sec_to_systime(u32 sec)
{
	return sec * OS_TICK;
}

static void _rtx_msleep_os(int ms)
{
_func_enter_;
	osDelay(ms);
_func_exit_;
}

static void _rtx_usleep_os(int us)
{
_func_enter_;
    hal_misc_delay_us_rtl8195blp(us);
_func_exit_;
}

static void _rtx_mdelay_os(int ms)
{
_func_enter_;
	osDelay(ms);
_func_exit_;
}

static void _rtx_udelay_os(int us)
{
_func_enter_;
    hal_misc_delay_us_rtl8195blp(us);
_func_exit_;
}

static void _rtx_yield_os(void)
{
_func_enter_;
	osThreadYield();
_func_exit_;
}

static void _rtx_ATOMIC_SET(ATOMIC_T *v, int i)
{
	save_and_cli();
	atomic_set(v,i);
	restore_flags();
}

static int _rtx_ATOMIC_READ(ATOMIC_T *v)
{
    int ret;

	save_and_cli();
    ret = atomic_read(v);
	restore_flags();

	return ret;
}

static void _rtx_ATOMIC_ADD(ATOMIC_T *v, int i)
{
	save_and_cli();
	v->counter += i;
	restore_flags();
}

static void _rtx_ATOMIC_SUB(ATOMIC_T *v, int i)
{
	save_and_cli();
	v->counter -= i;
	restore_flags();
}

static void _rtx_ATOMIC_INC(ATOMIC_T *v)
{
	save_and_cli();
	v->counter++;
	restore_flags();
}

static void _rtx_ATOMIC_DEC(ATOMIC_T *v)
{
	save_and_cli();
	v->counter--;
	restore_flags();
}

static int _rtx_ATOMIC_ADD_RETURN(ATOMIC_T *v, int i)
{
	int temp;

	save_and_cli();
	temp = v->counter;
	temp += i;
	v->counter = temp;
	restore_flags();

	return temp;
}

static int _rtx_ATOMIC_SUB_RETURN(ATOMIC_T *v, int i)
{
	int temp;

	save_and_cli();
	temp = v->counter;
	temp -= i;
	v->counter = temp;
	restore_flags();

	return temp;
}

static int _rtx_ATOMIC_INC_RETURN(ATOMIC_T *v)
{
	return _rtx_ATOMIC_ADD_RETURN(v, 1);
}

static int _rtx_ATOMIC_DEC_RETURN(ATOMIC_T *v)
{
	return _rtx_ATOMIC_SUB_RETURN(v, 1);
}

static u64 _rtx_modular64(u64 n, u64 base)
{
	unsigned int __base = (base);
	unsigned int __rem;
_func_enter_;
	if (((n) >> 32) == 0) {
		__rem = (unsigned int)(n) % __base;
		(n) = (unsigned int)(n) / __base;
	}
	else
		__rem = __div64_32(&(n), __base);
_func_exit_;
	return __rem;
}

/* Refer to ecos bsd tcpip codes */
static int _rtx_arc4random(void)
{
_func_enter_;
	u32 res = _rtx_get_current_time();
	static unsigned long seed = 0xDEADB00B;
	seed = ((seed & 0x007F00FF) << 7) ^
	    ((seed & 0x0F80FF00) >> 8) ^ // be sure to stir those low bits
	    (res << 13) ^ (res >> 9);    // using the clock too!
_func_exit_;
	return (int)seed;
}

static int _rtx_get_random_bytes(void *buf, u32 len)
{
#if 1 //becuase of 4-byte align, we use the follow code style.
	unsigned int ranbuf;
	unsigned int *lp;
	int i, count;
	count = len / sizeof(unsigned int);
	lp = (unsigned int *) buf;
_func_enter_;
	for(i = 0; i < count; i ++) {
		lp[i] = _rtx_arc4random();  
		len -= sizeof(unsigned int);
	}

	if(len > 0) {
		ranbuf = _rtx_arc4random();
		_rtx_memcpy(&lp[i], &ranbuf, len);
	}
_func_exit_;
	return 0;
#else
	unsigned long ranbuf, *lp;
	lp = (unsigned long *)buf;
	while (len > 0) {
		ranbuf = _rtx_arc4random();
		*lp++ = ranbuf; //this op need the pointer is 4Byte-align!
		len -= sizeof(ranbuf);
	}
	return 0;
#endif
}

static u32 _rtx_GetFreeHeapSize(void)
{
	return 0;
}


#if defined(CONFIG_USE_TCM_HEAP) && (CONFIG_USE_TCM_HEAP == 1)
void *tcm_heap_malloc(int size);
#endif
static int _rtx_create_task(struct task_struct *ptask, const char *name,
	u32  stack_size, u32 priority, thread_func_t func, void *thctx)
{
_func_enter_;
	rtx_thread_data_t *thread_hdl = NULL;
	u32 stacksize = stack_size * 4; //sizeof(DWORD)
	if(!func)
		goto err_exit;
	thread_hdl = (rtx_thread_data_t *)_rtx_zmalloc(sizeof(rtx_thread_data_t));
	if(thread_hdl == NULL)
		goto err_exit;
#ifdef CMSIS_OS_RTX
	if(priority > osPriorityRealtime){
		DBG_MISC_ERR("[%s]priority is higher than osPriorityRealtime", name);
		priority = osPriorityRealtime;
	}
	thread_hdl->def.pthread = (os_pthread)func;
	thread_hdl->def.tpriority = (osPriority)priority; 
	thread_hdl->def.stacksize = stacksize;
#endif
	ptask->task = (_thread_hdl_)thread_hdl;
	ptask->task_name = name;
	ptask->blocked = 0;
	ptask->callback_running = 0;

	_rtx_init_sema(&ptask->wakeup_sema, 0);
	_rtx_init_sema(&ptask->terminate_sema, 0);
	//rtw_init_queue(&wq->work_queue);

	thread_hdl->id = osThreadCreate(&thread_hdl->def, thctx);
	if(thread_hdl->id == NULL)
		goto err_exit;
	return _SUCCESS;
err_exit:
	if(thread_hdl){
		_rtx_free_sema(&ptask->wakeup_sema);
		_rtx_free_sema(&ptask->terminate_sema);
		_rtx_memset((u8 *)ptask, 0, sizeof(*ptask));
		_rtx_mfree((u8 *)thread_hdl, sizeof(rtx_thread_data_t));
	}
	DBG_MISC_ERR("Create Task \"%s\" Failed! \n", ptask->task_name);
	return _FAIL;
}

static void _rtx_delete_task(struct task_struct *ptask)
{
_func_enter_;
	rtx_thread_data_t *thread_hdl = (rtx_thread_data_t *)ptask->task;
	if (!thread_hdl){
		DBG_MISC_ERR("_rtx_delete_task(): ptask is NULL!\n");
		return;
	}

	ptask->blocked = 1;

	_rtx_up_sema(&ptask->wakeup_sema);
	_rtx_down_sema(&ptask->terminate_sema, TIMER_MAX_DELAY);

	osThreadTerminate(thread_hdl->id);
	_rtx_mfree((u8 *)thread_hdl, sizeof(rtx_thread_data_t));
	
	//rtw_deinit_queue(&wq->work_queue);
	_rtx_free_sema(&ptask->wakeup_sema);
	_rtx_free_sema(&ptask->terminate_sema);

	ptask->task = NULL;

	DBG_MISC_INFO("Delete Task \"%s\"\n", ptask->task_name);
_func_exit_;
}

void _rtx_wakeup_task(struct task_struct *ptask)
{
_func_enter_;
	if(ptask)
		_rtx_up_sema(&ptask->wakeup_sema);
_func_exit_;
}

static void _rtx_thread_enter(char *name)
{
_func_enter_;
	DBG_MISC_INFO("\n\rRTKTHREAD %s\n", name);
_func_exit_;
}

static void _rtx_thread_exit(void)
{
_func_enter_;
	osThreadId id = osThreadGetId();
	osThreadTerminate(id);
_func_exit_;
}

/*****************************************************
************timer data block, defined in rt_CMSIS.c*********

// Timer definitions
#define osTimerInvalid  0
#define osTimerStopped  1
#define osTimerRunning  2

// Timer structures

typedef struct os_timer_cb_ {                   // Timer Control Block
  struct os_timer_cb_ *next;                    // Pointer to next active Timer, (u8 *)data[0:3]
  uint8_t             state;                    // Timer State, (u8 *)data[4]
  uint8_t              type;                    // Timer Type (Periodic/One-shot), (u8 *)data[5]
  uint16_t         reserved;                    // Reserved, (u8 *)data[6:7]
  uint16_t             tcnt;                    // Timer Delay Count, (u8 *)data[8:9]
  uint16_t             icnt;                    // Timer Initial Count, (u8 *)data[10:11]
  void                 *arg;                    // Timer Function Argument, (u8 *)data[12:15]
  osTimerDef_t       *timer;                    // Pointer to Timer definition, (u8 *)data[16:19]
} os_timer_cb;
*****************************************************/
_timerHandle _rtx_timerCreate( const signed char *pcTimerName, 
							  osdepTickType xTimerPeriodInTicks, 
							  u32 uxAutoReload, 
							  void * pvTimerID, 
							  TIMER_FUN pxCallbackFunction )
{
_func_enter_;
	rtx_tmr_t *tmr = (rtx_tmr_t *)_rtx_zmalloc(sizeof(rtx_tmr_t));
	os_timer_type type = (uxAutoReload == _TRUE)?osTimerPeriodic:osTimerOnce;
	if(tmr == NULL)
		goto err_exit;
#ifdef CMSIS_OS_RTX
	tmr->def.ptimer = (os_ptimer)pxCallbackFunction;
	tmr->def.timer = (void *)tmr->data;
#endif
	if(pvTimerID == NULL)
		pvTimerID = (void *)tmr;
	tmr->id = osTimerCreate(&tmr->def, type, pvTimerID);
	if(tmr->id == NULL)
		goto err_exit;
_func_exit_;
	return (_timerHandle)tmr;
err_exit:
	DBG_MISC_ERR("error");
	if(tmr)
		_rtx_mfree((u8 *)tmr, sizeof(rtx_tmr_t));
	return NULL;
}

u32 _rtx_timerDelete( _timerHandle xTimer, 
							   osdepTickType xBlockTime )
{
_func_enter_;
	rtx_tmr_t *tmr = (rtx_tmr_t *) xTimer;
	osStatus status = osTimerDelete(tmr->id);
	_rtx_mfree((u8 *)tmr, sizeof(rtx_tmr_t));
	if(status != osOK){
		DBG_MISC_ERR("error %d", status);
		return _FAIL;
	}
_func_exit_;
	return _SUCCESS;
}

u32 _rtx_timerIsTimerActive( _timerHandle xTimer )
{
_func_enter_;
	rtx_tmr_t *tmr = (rtx_tmr_t *) xTimer;
	u8 *data = (u8 *)tmr->data;
_func_exit_;
	switch(data[4]){
		case 2U:
			return _TRUE;
		default:
			return _FALSE;	
	}
}

u32  _rtx_timerStop( _timerHandle xTimer, 
							   osdepTickType xBlockTime )
{
_func_enter_;
	rtx_tmr_t *tmr = (rtx_tmr_t *) xTimer;
	if(_rtx_timerIsTimerActive(xTimer) == _TRUE){
		osStatus status = osTimerStop(tmr->id);
_func_exit_;
		if(status != osOK){
			DBG_MISC_ERR("error %d\n", status);
			return _FAIL;
		}
	}
	return _SUCCESS;
}

u32  _rtx_timerChangePeriod( _timerHandle xTimer, 
							   osdepTickType xNewPeriod, 
							   osdepTickType xBlockTime )
{
_func_enter_;
	rtx_tmr_t *tmr = (rtx_tmr_t *) xTimer;
	osStatus ret;

	if(xNewPeriod == 0)
		xNewPeriod += 1;
	xNewPeriod = _rtx_systime_to_ms(xNewPeriod);
	ret = osTimerStart(tmr->id, xNewPeriod);
_func_exit_;
	if(ret == osOK)
		return _SUCCESS;
	
	DBG_MISC_ERR("%s error\n", __FUNCTION__);
	return _FAIL;
}

//void _rtx_acquire_wakelock()
//{
//#if defined(configUSE_WAKELOCK_PMU) && (configUSE_WAKELOCK_PMU == 1)
//    acquire_wakelock(WAKELOCK_WLAN);
//#endif
//}

//void _rtx_release_wakelock()
//{
//#if defined(configUSE_WAKELOCK_PMU) && (configUSE_WAKELOCK_PMU == 1)
//    release_wakelock(WAKELOCK_WLAN);
//#endif
//}

u8 _rtx_get_scheduler_state(void)
{
_func_enter_;
	int32_t state = osKernelRunning();
	u8 state_out = OS_SCHEDULER_NOT_STARTED;
	switch(state){
		case 1:		state = OS_SCHEDULER_RUNNING;		break;
		case 0:		state = OS_SCHEDULER_SUSPENDED;		break;
		default:	break;
	}
_func_exit_;
	return state_out;
}

SECTION_RTX_SERVICE_STUBS const struct osdep_service_ops osdep_service = {
	.rtw_vmalloc = _rtx_malloc,		//rtw_vmalloc
	.rtw_zvmalloc = _rtx_zmalloc,		//rtw_zvmalloc
	.rtw_vmfree = _rtx_mfree,			//rtw_vmfree
	.rtw_malloc = _rtx_malloc, //rtw_malloc
	.rtw_zmalloc = _rtx_zmalloc, //rtw_zmalloc
	.rtw_mfree = _rtx_mfree, //rtw_mfree
	.rtw_memcpy = _rtx_memcpy, //rtw_memcpy
	.rtw_memcmp = _rtx_memcmp, //rtw_memcmp
	.rtw_memset = _rtx_memset, //rtw_memset
	.rtw_init_sema = _rtx_init_sema, //rtw_init_sema
	.rtw_free_sema = _rtx_free_sema, //rtw_free_sema
	.rtw_up_sema = _rtx_up_sema, //rtw_up_sema
	.rtw_up_sema_from_isr =_rtx_up_sema_from_isr,//rtw_up_sema_from_isr
	.rtw_down_timeout_sema = _rtx_down_sema, //rtw_down_sema
	.rtw_mutex_init = _rtx_mutex_init, //rtw_mutex_init
	.rtw_mutex_free = _rtx_mutex_free, //rtw_mutex_free
	.rtw_mutex_get = _rtx_mutex_get, //rtw_mutex_get
	.rtw_mutex_get_timeout = _rtx_mutex_get_timeout, //rtw_mutex_get_timeout
	.rtw_mutex_put = _rtx_mutex_put, //rtw_mutex_put
	.rtw_enter_critical = _rtx_enter_critical,		//rtw_enter_critical
	.rtw_exit_critical = _rtx_exit_critical,		//rtw_exit_critical
	.rtw_enter_critical_from_isr = _rtx_enter_critical_from_isr,		//rtw_enter_critical_from_isr
	.rtw_exit_critical_from_isr = _rtx_exit_critical_from_isr,		//rtw_exit_critical_from_isr
	.rtw_enter_critical_bh = NULL,		//rtw_enter_critical_bh
	.rtw_exit_critical_bh = NULL,		//rtw_exit_critical_bh
	.rtw_enter_critical_mutex = _rtx_enter_critical_mutex,		//rtw_enter_critical_mutex
	.rtw_exit_critical_mutex = _rtx_exit_critical_mutex,		//rtw_exit_critical_mutex
	.rtw_spinlock_init = _rtx_spinlock_init, //rtw_spinlock_init
	.rtw_spinlock_free = _rtx_spinlock_free, //rtw_spinlock_free
	.rtw_spin_lock = _rtx_spinlock, //rtw_spin_lock
	.rtw_spin_unlock = _rtx_spinunlock, //rtw_spin_unlock
	.rtw_spinlock_irqsave = _rtx_spinlock_irqsave,		//rtw_spinlock_irqsave
	.rtw_spinunlock_irqsave = _rtx_spinunlock_irqsave,	//rtw_spinunlock_irqsave
	.rtw_init_xqueue = _rtx_init_xqueue,//rtw_init_xqueue
	.rtw_push_to_xqueue = _rtx_push_to_xqueue,//rtw_push_to_xqueue
	.rtw_pop_from_xqueue = _rtx_pop_from_xqueue,//rtw_pop_from_xqueue
	.rtw_deinit_xqueue = _rtx_deinit_xqueue,//rtw_deinit_xqueue
	.rtw_get_current_time = _rtx_get_current_time, //rtw_get_current_time
	.rtw_systime_to_ms = _rtx_systime_to_ms, //rtw_systime_to_ms
	.rtw_systime_to_sec = _rtx_systime_to_sec, //rtw_systime_to_sec
	.rtw_ms_to_systime = _rtx_ms_to_systime, //rtw_ms_to_systime
	.rtw_sec_to_systime = _rtx_sec_to_systime, //rtw_sec_to_systime
	.rtw_msleep_os = _rtx_msleep_os, //rtw_msleep_os
	.rtw_usleep_os = _rtx_usleep_os, //rtw_usleep_os
	.rtw_mdelay_os = _rtx_mdelay_os, //rtw_mdelay_os
	.rtw_udelay_os = _rtx_udelay_os, //rtw_udelay_os
	.rtw_yield_os = _rtx_yield_os, //rtw_yield_os
	
	.ATOMIC_SET = _rtx_ATOMIC_SET, //ATOMIC_SET
	.ATOMIC_READ = _rtx_ATOMIC_READ, //ATOMIC_READ
	.ATOMIC_ADD = _rtx_ATOMIC_ADD, //ATOMIC_ADD
	.ATOMIC_SUB = _rtx_ATOMIC_SUB, //ATOMIC_SUB
	.ATOMIC_INC = _rtx_ATOMIC_INC, //ATOMIC_INC
	.ATOMIC_DEC = _rtx_ATOMIC_DEC, //ATOMIC_DEC
	.ATOMIC_ADD_RETURN = _rtx_ATOMIC_ADD_RETURN, //ATOMIC_ADD_RETURN
	.ATOMIC_SUB_RETURN = _rtx_ATOMIC_SUB_RETURN, //ATOMIC_SUB_RETURN
	.ATOMIC_INC_RETURN = _rtx_ATOMIC_INC_RETURN, //ATOMIC_INC_RETURN
	.ATOMIC_DEC_RETURN = _rtx_ATOMIC_DEC_RETURN, //ATOMIC_DEC_RETURN

	.rtw_modular64 = _rtx_modular64, 		//rtw_modular64
	.rtw_get_random_bytes = _rtx_get_random_bytes,	//rtw_get_random_bytes
	.rtw_getFreeHeapSize = _rtx_GetFreeHeapSize,		//rtw_getFreeHeapSize

	.rtw_create_task = _rtx_create_task,		//rtw_create_task
	.rtw_delete_task = _rtx_delete_task,		//rtw_delete_task
	.rtw_wakeup_task = _rtx_wakeup_task,		//rtw_wakeup_task

	.rtw_thread_enter = _rtx_thread_enter,		//rtw_thread_enter
	.rtw_thread_exit = _rtx_thread_exit,		//rtw_thread_exit

	.rtw_timerCreate = _rtx_timerCreate,           //rtw_timerCreate,
	.rtw_timerDelete = _rtx_timerDelete,           //rtw_timerDelete,
	.rtw_timerIsTimerActive = _rtx_timerIsTimerActive,    //rtw_timerIsTimerActive,
	.rtw_timerStop = _rtx_timerStop,             //rtw_timerStop,         
	.rtw_timerChangePeriod = _rtx_timerChangePeriod,      //rtw_timerChangePeriod  

	.rtw_acquire_wakelock = NULL,  // rtw_acquire_wakelock
	.rtw_release_wakelock = NULL,  // rtw_release_wakelock
//	.rtw_wakelock_timeout = NULL,  //rtw_wakelock_timeout

	.rtw_get_scheduler_state = _rtx_get_scheduler_state  // rtw_get_scheduler_state
};

