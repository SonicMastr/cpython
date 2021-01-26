#include <psp2/kernel/threadmgr.h>

#define SCE_KERNEL_PRIO_USER_NORMAL 96
#define SEM_VALUE_MAX   (2147483647)

#if !defined(THREAD_STACK_SIZE)
#define THREAD_STACK_SIZE       0x10000
#endif

#define VITA_STACKSIZE(x)        (x ? x : THREAD_STACK_SIZE)

typedef struct {
	volatile int counter;
} atomic_t;

static inline void atomic_add(int i, atomic_t *v)
{
	unsigned long tmp;
	int result;

	__asm__ __volatile__("@ atomic_add\n"
"1:	ldrex	%0, [%2]\n"
"	add	%0, %0, %3\n"
"	strex	%1, %0, [%2]\n"
"	teq	%1, #0\n"
"	bne	1b"
	: "=&r" (result), "=&r" (tmp)
	: "r" (&v->counter), "Ir" (i)
	: "cc");
}

/*
 * Initialization.
 */
  static void
PyThread__init_thread(void)
{
  return;
}

typedef struct {
  void (*func)(void*);
  void *arg;
} callobj;

static int bootstrap(SceSize args, void *call){
  callobj *obj = *(callobj**) call;
  void (*func)(void*) = obj->func;
  void *arg = obj->arg;

  (*func)(arg);
  free(obj);
  return 0;
}

/*
 * Thread support.
 */
 static atomic_t thread_count = { 0 };

  long
PyThread_start_new_thread(void (*func)(void *), void *arg)
{
  char name[SCE_UID_NAMELEN];
  callobj* obj;

  dprintf(("PyThread_start_new_thread called\n"));

  atomic_add(1, &thread_count);
  PyOS_snprintf(name, sizeof(name),
      "python thread (%d)", thread_count.counter );

  obj = (callobj *)malloc(sizeof(callobj));

  obj->func = func;
  obj->arg = arg;


  SceUID thid = sceKernelCreateThread(name, (SceKernelThreadEntry)bootstrap, SCE_KERNEL_PRIO_USER_NORMAL, VITA_STACKSIZE(_pythread_stacksize), 0, 0, NULL);

  if(thid < 0) {
    return -1;
  }

  int success = sceKernelStartThread(thid, sizeof(obj), &obj);

  if(success != 0) {
    return -1;
  }

  return (long)thid;
}

  long
PyThread_get_thread_ident(void)
{
  int id = sceKernelGetThreadId();
  return (id < 0) ? -1 : (long) id;
}

  void
PyThread_exit_thread(void)
{
  dprintf(("PyThread_exit_thread called\n"));
  sceKernelExitDeleteThread(0);
}

/*
 * Lock support.
 */

static atomic_t lock_count = { 0 };
PyThread_type_lock PyThread_allocate_lock(void)
{
  SceUID* lock = (SceUID*)malloc(sizeof(SceUID));

  char name[SCE_UID_NAMELEN];

  dprintf(("PyThread_allocate_lock called\n"));

  atomic_add(1, &lock_count);
  PyOS_snprintf(name, sizeof(name), "python lock (%d)", lock_count.counter);

  *lock = sceKernelCreateSema(name, 0, 1, SEM_VALUE_MAX, NULL);
  if (*lock < 0) {
    perror("sem_init");
    free(lock);
    return NULL;
  }

  dprintf(("PyThread_allocate_lock() -> %p\n", (SceUID)(*lock)));
  return (PyThread_type_lock) lock;
}

  void
PyThread_free_lock(PyThread_type_lock lock)
{
  SceUID* thelock = (SceUID*) lock;

  if (thelock == NULL)
    return;

  dprintf(("PyThread_free_lock(%p) called\n", (SceUID)(*thelock)));
  sceKernelDeleteSema(*thelock);
  free(thelock);
}

  int
PyThread_acquire_lock(PyThread_type_lock lock, int waitflag)
{
  int success, status = 0;
  SceUID* thelock = (SceUID*) lock;

  dprintf(("PyThread_acquire_lock(%p, %d) called\n", (SceUID)(*thelock), waitflag));

  if (waitflag) /* blocking */
    status = sceKernelWaitSema(*thelock, 1, NULL);
  else /* non-blocking */
    status = sceKernelPollSema(*thelock, 1);

  success = (status >= 0) ? 1 : 0;
  dprintf(("PyThread_acquire_lock(%p, %d) -> %d : (0x%08x)\n", (SceUID)(*thelock), waitflag, success, status));
  return success;
}

  void
PyThread_release_lock(PyThread_type_lock lock)
{
  SceUID* thelock = (SceUID*) lock;
  dprintf(("PyThread_release_lock(%p) called\n", (SceUID)(*thelock)));
  sceKernelSignalSema(*thelock, 1);
}

/* minimum/maximum thread stack sizes supported */
#define THREAD_MIN_STACKSIZE    0x1000          /* 32kB */
#define THREAD_MAX_STACKSIZE    0x2000000       /* 32MB */

/* set the thread stack size.
 * Return 0 if size is valid, -1 otherwise.
 */
  static int
_pythread_vita_set_stacksize(size_t size)
{
  /* set to default */
  if (size == 0) {
    _pythread_stacksize = 0;
    return 0;
  }

  /* valid range? */
  if (size >= THREAD_MIN_STACKSIZE && size < THREAD_MAX_STACKSIZE) {
    _pythread_stacksize = size;
    return 0;
  }

  return -1;
}

#define THREAD_SET_STACKSIZE(x) _pythread_vita_set_stacksize(x)
