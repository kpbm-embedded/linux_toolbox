// SPDX-License-Identifier: GPL-2.0
/*
 * boxinit: /init for the example test boots (scripts/klinux examples test). Built with the kernel's own
 * tools/include/nolibc (no C library needed), so any kernel.org cross compiler can build it.
 *
 * Mounts proc, sysfs, devtmpfs, debugfs and configfs, runs the commands in /test.cmds one per line, prints
 * "BOXTEST: PASS" or "BOXTEST: FAIL ..." on the console and powers off. Commands:
 *   insmod <file.ko> [params]   load a module (finit_module)       rmmod <name>      unload it
 *   write <path> <text>         write text (no newline added)      cat <path>        print a file
 *   expect <path> <text>        file contains text                 exists <path>     file exists
 *   missing <path>              file does not exist                gt <path> <n>     file holds a number > n
 *   klog <text>                 kernel log contains text (waits up to 2 s)
 *   fail <cmd ...>              the command must fail (e.g. "fail write /dev/x y")
 *   sleep <ms>                  wait                                # comment
 */
#include <linux/reboot.h>

#define MAXBUF 16384

static int failed;

static void out(const char *s) { write(1, s, strlen(s)); }

static int contains(const char *hay, const char *needle)
{
	size_t n = strlen(needle);

	for (; *hay; hay++)
		if (!strncmp(hay, needle, n))
			return 1;
	return n == 0;
}

static int slurp(const char *path, char *buf, int size)
{
	int fd = open(path, O_RDONLY), len = 0, r;

	if (fd < 0)
		return -1;
	while (len < size - 1 && (r = read(fd, buf + len, size - 1 - len)) > 0)
		len += r;
	close(fd);
	buf[len] = 0;
	return len;
}

/* Search the kernel log (/dev/kmsg records "pri,seq,ts,flags;text"). */
static int klog_has(const char *text)
{
	static char rec[1024];
	int tries, fd, r;

	for (tries = 0; tries < 20; tries++) {
		fd = open("/dev/kmsg", O_RDONLY | O_NONBLOCK);
		if (fd < 0)
			return 0;
		while ((r = read(fd, rec, sizeof(rec) - 1)) > 0 || r == -EPIPE || errno == EPIPE) {
			if (r <= 0)
				continue;
			rec[r] = 0;
			if (contains(strchr(rec, ';') ? strchr(rec, ';') + 1 : rec, text)) {
				close(fd);
				return 1;
			}
		}
		close(fd);
		usleep(100000);
	}
	return 0;
}

/* Run one command line; returns 0 on success. */
static int run(char *line)
{
	static char buf[MAXBUF];
	static char argline[512];
	char *cmd = line, *arg = NULL, *rest = NULL;
	int fd, r;

	while (*cmd == ' ')
		cmd++;
	if (!*cmd || *cmd == '#')
		return 0;
	arg = strchr(cmd, ' ');
	if (arg) {
		*arg++ = 0;
		while (*arg == ' ')
			arg++;
		strncpy(argline, arg, sizeof(argline) - 1);	/* klog text with its spaces */
		rest = strchr(arg, ' ');
		if (rest) {
			*rest++ = 0;
			while (*rest == ' ')
				rest++;
		}
	}
	if (!rest)
		rest = "";
	if (!strcmp(cmd, "fail")) {	/* the rest of the line is a command that must fail */
		static char sub[512];

		if (!arg)
			return -1;
		strcpy(sub, argline);
		return run(sub) == 0 ? -1 : 0;
	}
	if (!arg && strcmp(cmd, "cat"))
		return -1;
	if (!strcmp(cmd, "insmod")) {
		fd = open(arg, O_RDONLY);
		if (fd < 0)
			return -1;
		r = my_syscall3(__NR_finit_module, fd, rest, 0);
		close(fd);
		return r;
	}
	if (!strcmp(cmd, "rmmod"))
		return my_syscall2(__NR_delete_module, arg, O_NONBLOCK);
	if (!strcmp(cmd, "write")) {
		fd = open(arg, O_WRONLY);
		if (fd < 0)
			return -1;
		r = write(fd, rest, strlen(rest));
		close(fd);
		return r < 0 ? -1 : 0;
	}
	if (!strcmp(cmd, "cat")) {
		if (slurp(arg, buf, sizeof(buf)) < 0)
			return -1;
		out(buf);
		return 0;
	}
	if (!strcmp(cmd, "expect")) {
		if (slurp(arg, buf, sizeof(buf)) < 0)
			return -1;
		if (contains(buf, rest))
			return 0;
		out("BOXTEST: got: ");
		out(buf);
		out("\n");
		return -1;
	}
	if (!strcmp(cmd, "gt")) {
		if (slurp(arg, buf, sizeof(buf)) < 0)
			return -1;
		return atol(buf) > atol(rest) ? 0 : -1;
	}
	if (!strcmp(cmd, "exists") || !strcmp(cmd, "missing")) {
		fd = open(arg, O_RDONLY);
		if (fd >= 0)
			close(fd);
		return (fd >= 0) == !strcmp(cmd, "exists") ? 0 : -1;
	}
	if (!strcmp(cmd, "klog"))
		return klog_has(argline) ? 0 : -1;
	if (!strcmp(cmd, "sleep")) {
		usleep(atol(arg) * 1000);
		return 0;
	}
	out("BOXTEST: unknown command\n");
	return -1;
}

int main(void)
{
	static char cmds[MAXBUF], copy[1024];
	char *line, *next;
	int n = 0;

	mount("proc", "/proc", "proc", 0, NULL);
	mount("sysfs", "/sys", "sysfs", 0, NULL);
	mount("devtmpfs", "/dev", "devtmpfs", 0, NULL);
	mount("debugfs", "/sys/kernel/debug", "debugfs", 0, NULL);
	mount("configfs", "/sys/kernel/config", "configfs", 0, NULL);
	out("BOXTEST: start\n");
	if (slurp("/test.cmds", cmds, sizeof(cmds)) < 0) {
		out("BOXTEST: FAIL no /test.cmds\n");
		failed = 1;
	}
	for (line = cmds; !failed && line && *line; line = next) {
		next = strchr(line, '\n');
		if (next)
			*next++ = 0;
		strncpy(copy, line, sizeof(copy) - 1);
		if (!*line || *line == '#')
			continue;
		n++;
		out("BOXTEST: $ ");
		out(copy);
		out("\n");
		if (run(line)) {
			out("BOXTEST: FAIL at: ");
			out(copy);
			out("\n");
			failed = 1;
		}
	}
	if (!failed)
		printf("BOXTEST: PASS (%d commands)\n", n);
	reboot(LINUX_REBOOT_CMD_POWER_OFF);
	return 0;
}
