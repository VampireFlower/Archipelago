#include <types.h>
#include <OSContext.h>

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define OS_THREAD_SPECIFIC_MAX  2

typedef int OSPriority;

struct OSThread;
struct OSMutex;
struct OSMutexQueue;

typedef struct OSThread OSThread;

typedef struct OSThreadQueue
{
    OSThread *head;
    OSThread *tail;
} OSThreadQueue;

typedef struct OSThreadLink
{
    OSThread *next;
    OSThread *prev;
} OSThreadLink;

typedef struct OSMutexQueue
{
    struct OSMutex *head;
    struct OSMutex *tail;
} OSMutexQueue;

typedef struct OSMutexLink
{
    struct OSMutex *next;
    struct OSMutex *prev;
} OSMutexLink;

typedef struct OSThread
{
    /*0x000*/ struct OSContext context;
    /*0x2C8*/ ushort state; // OS_THREAD_STATE_*
    /*0x2CA*/ ushort attr; // OS_THREAD_ATTR_*
    /*0x2CC*/ int suspend; // suspended if the count is greater than zero
    /*0x2D0*/ OSPriority priority; // effective scheduling priority
    /*0x2D4*/ OSPriority base; // base scheduling priority
    /*0x2D8*/ void* val; // exit value
    /*0x2DC*/ OSThreadQueue* queue; // queue thread is on
    /*0x2E0*/ OSThreadLink link; // queue link
    /*0x2E8*/ OSThreadQueue queueJoin; // list of threads waiting for termination (join)
    /*0x2F0*/ struct OSMutex* mutex; // mutex trying to lock
    /*0x2F4*/ OSMutexQueue queueMutex; // list of mutexes owned
    /*0x2FC*/ OSThreadLink linkActive; // link of all threads for debugging
    /*0x304*/ byte* stackBase; // high address
    /*0x308*/ uint* stackEnd; // low address
    /*0x30C*/ int error;
    /*0x310*/ void* specific[OS_THREAD_SPECIFIC_MAX]; // thread local storage
} OSThread;

enum OS_THREAD_STATE : ushort
{
    OS_THREAD_STATE_READY = 1,
    OS_THREAD_STATE_RUNNING = 2,
    OS_THREAD_STATE_WAITING = 4,
    OS_THREAD_STATE_MORIBUND = 8,
};

#define OS_PRIORITY_MIN 0  // highest
#define OS_PRIORITY_MAX 31 // lowest
#define OS_PRIORITY_IDLE OS_PRIORITY_MAX

#define OS_THREAD_ATTR_DETACH 0x0001u

#define OS_THREAD_STACK_MAGIC 0xDEADBABE

/*80039840*/ void      OSInitThreadQueue(OSThreadQueue* queue);
/*80039850*/ OSThread* OSGetCurrentThread();
/*8003985c*/ void      __OSSwitchThread(OSThread* nextThread);
/*800398b8*/ int       OSIsThreadSuspended(OSThread* thread);
/*800398d4*/ int       OSIsThreadTerminated(OSThread* thread);
/*80039908*/ int       __OSIsThreadActive(OSThread* thread);
/*8003994c*/ int       OSDisableScheduler();
/*8003998c*/ int       OSEnableScheduler();
/*800399cc*/ void      SetRun(OSThread* thread);
/*80039a38*/ void      UnsetRun(OSThread* thread);
/*80039adc*/ OSThread* SetEffectivePriority(OSThread* thread, int priority);
/*80039c9c*/ void      UpdatePriority(OSThread* thread);
/*80039d64*/ OSThread* SelectThread(int yield);
/*80039fbc*/ void      OSYieldThread(void);
/*80039ff8*/ int       OSCreateThread(OSThread* thread, void* (*func)(void*), void* param, void* stack, uint stackSize, int priority, ushort attr);
/*8003a1e0*/ void      OSExitThread(void * val);
/*8003a2c4*/ void      OSCancelThread(OSThread* thread);
/*8003a480*/ int       OSJoinThread(OSThread* thread, void * val);
/*8003a5c0*/ void      OSDetachThread(OSThread* thread);
/*8003a660*/ int       OSResumeThread(OSThread* thread);
/*8003a8e8*/ int       OSSuspendThread(OSThread* thread);
/*8003aa58*/ void      OSSleepThread(OSThreadQueue* queue);
/*8003ab44*/ void      OSWakeupThread(OSThreadQueue* queue);
/*8003ac48*/ int       OSSetThreadPriority(OSThread* thread, int priority);
/*8003ad08*/ int       OSGetThreadPriority(OSThread* thread);
/*8003ad10*/ OSThread* OSSetIdleFunction(void (*idleFunction)(void*), void* param, void* stack, uint stackSize);
/*8003ada0*/ OSThread* OSGetIdleFunction();
/*8003adbc*/ int       CheckThreadQueue(OSThreadQueue* queue);
/*8003ae58*/ BOOL      IsMember(OSThreadQueue* queue, OSThread * thread);
/*8003ae84*/ int       OSCheckActiveThreads();
/*8003b5d4*/ void      OSClearStack(byte);
/*8003b680*/ void      OSSetThreadSpecific(int i, void* val);
/*8003b6b0*/ void*     OSGetThreadSpecific(int i);


/*800000dc*/ extern OSThreadQueue __OSActiveThreadQueue;


#define IsSuspended(suspend) (suspend > 0)


#ifdef __cplusplus
}
#endif