/*
 * Copyright (C) cozybit, Inc. 2007-2010
 *
 * mdns on Linux
 *
 * Here we implement the mdns os abstraction and a simple command line front
 * end for mdns.
 */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include "mdns.h"
#include "mdns_os.h"

#define MDNS_PIDFILE "/var/run/mdns.pid"

/* TODO: Use system-independent log function */
#define LOG printf

static void linux_mdns_signal(int sig)
{
	switch(sig){
	case SIGHUP:
	case SIGTERM:
		mdns_halt();
		break;
	}
}

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

	/* looks good.	fork. */
	ret = fork();
	if (ret < 0) {
		perror("failed to fork mdns");
		return NULL;
	} else if (ret > 0) {
		/* parent process.	We don't really need a pointer, so we'll indicate
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

		/* On an RTOS with a single memory space, we would generally just
		 * communicate with the mdns task by invoking the API.	But on linux it
		 * runs off in its own process, so we have to send signals.	 SIGHUP and
		 * SIGTERM both invoke mdns_halt().
		 */
		signal(SIGHUP, linux_mdns_signal);
		signal(SIGTERM, linux_mdns_signal);

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

void mdns_thread_yield(void)
{
	sleep(0);
}

#define HELP_TEXT \
"Usage: mdns [options] <command>\n\n" \
"command can be one of the following:\n" \
"launch        start mdns daemon\n" \
"halt          stop mdns daemon\n" \
"\n" \
"Options\n" \
"-h             Print this help text\n" \
"-b <ipaddr>    ipaddress to bind to\n" \
"-d <domain>    domain to resolve (default is 'local')\n" \
"-n <hostname>  hostname to resolve (default is 'node')\n"

int main(int argc, char **argv)
{
	int ret = 0;
	char opt;
	in_addr_t ipaddr = 0;
	char *cmd;
	char *domain = "local";
	char *hostname = "node";

	while ((opt = getopt(argc, argv, "hb:d:n:")) != -1) {
		switch (opt) {
		case 'h':
			printf(HELP_TEXT);
			return 0;
		case 'b':
			ipaddr = inet_addr(optarg);
			break;
		case 'd':
			domain = optarg;
			break;
		case 'n':
			hostname = optarg;
			break;
		default:
			printf("Unexpected option %c\n", opt);
			return -1;
		}
	}

	if (optind >= argc) {
		printf("No command specified.\n");
		return -1;
	}

	cmd = argv[optind];
	if (strcmp(cmd, "launch") == 0) {
		ret = mdns_launch(ipaddr, domain, hostname);

	} else if (strcmp(cmd, "halt") == 0) {
		mdns_halt();
		return 0;

	} else {
		printf("No such command: %s\n", cmd);
		return -1;
	}
	return ret;
}
