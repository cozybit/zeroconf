/* 
 * Copyright (C) cozybit, Inc. 2007-2010
 *
 * mdns_os.h: operating system abstraction layer for mdns
 *
 */

#ifndef __MDNS_OS_H__
#define __MDNS_OS_H__

/*
 * mdsn_thread_entry: Thread entry point function
 */
typedef void (*mdns_thread_entry)(void *data);

/*
 * mdns_thread_create: Create a thread
 *
 * mdns_thread_create should create and launch the thread.  mdns launches only
 * a single thread, so the implementation of this function can be fairly
 * simple.
 *
 * entry: thread entry point function
 * data: data to be passed to entry when thread is launched
 *
 * Returns: NULL on failure; a pointer to an opaque type that represents the
 * thread on success.  This return type is passed to the other thread
 * functions.
 *
 */
void *mdns_thread_create(mdns_thread_entry entry, void *data);

/*
 * mdns_thread_delete: Delete a thread
 *
 * t: pointer to thread to be deleted
 *
 */
void mdns_thread_delete(void *t);

/*
 * mdns_thread_yield: yield to other runnable threads
 *
 * Some mdns routines need to yeild after sending commands to the mdns thread.
 * This allows the mdns thread to run and respond to the command.
 */
void mdns_thread_yield(void);

/*
 * mdns_log: printf-like function to write log messages
 *
 * The mdns daemon will write log messages depending on its build-time
 * log-level.  See mdns.h for details.
 */
void mdns_log(const char *fmt, ...);

#endif /* __MDNS_OS_H__ */
