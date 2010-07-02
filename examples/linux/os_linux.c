/* 
 * Copyright (C) cozybit, Inc. 2007-2010
 *
 * Example implementation of mdns os abstraction functions for linux processes
 */
#include "mdns_os.h"
#include "priv.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>

/* TODO: Use system-independent log function */
#define LOG printf

/* Our "thread" is really a process.  We only support one thread with this
 * function, so we'll fail if the thread is already running.
 */
void *mdns_thread_create(mdns_thread_entry entry, void *data)
{
    int ret, lock;
	/* str needs to hold a string representing the max pid in decimal */
	char str[sizeof(pid_t)*3 + 1];

	/* check the lock file */
	lock = open(MDNS_PIDFILE, O_RDWR|O_CREAT, 0640);
	if (lock < 0) {
		perror("failed to open lock file");
		return NULL;
	}
	ret = lockf(lock, F_TEST, 0);
	if (ret == -1 && (errno == EACCES || errno == EAGAIN)) {
		fprintf(stderr, "pid file is locked.  mdns already running?\n");
		return NULL;
	}
	if (ret != 0) {
		perror("failed to check lock file");
		return NULL;
	}

	/* looks good.  fork. */
	ret = fork();
	if (ret < 0) {
		perror("failed to fork mdns");
		return NULL;
	} else if (ret > 0) {
		/* parent process.  We don't really need a pointer, so we'll indicate
		 * success with 1
		 */
		return (void *)1;

	} else {
		/* child process */
		setsid();

		/* redirect stdin, out, and err */
		freopen("/dev/null", "r", stdin);
		freopen("/dev/null", "w", stdout);
		umask(027);
		ret = lockf(lock, F_TLOCK, 0);
		if (0 > ret) {
			LOG("failed to lock PID file: %s\n", strerror(errno));
			return NULL;
		}
		sprintf(str, "%d\n", getpid());
		ret = write(lock, str, strlen(str));
		if (0 > ret) {
			LOG("failed to write to PID file: %s\n", strerror(errno));
			return NULL;
		}
		signal(SIGCHLD, SIG_IGN);
		signal(SIGTSTP, SIG_IGN);
		signal(SIGTTOU, SIG_IGN);
		signal(SIGTTIN, SIG_IGN);
		/* launch the entry function */
		entry(data);
		return NULL;
	}
}

void mdns_thread_delete(void *t)
{
	/* nothing to free here. */
	return;
}
