/*
 * Copy me if you can.
 * by 20h
 */

#define _BSD_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <strings.h>
#include <sys/time.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <X11/Xlib.h>

char *tzbkk = "Asia/Bangkok";

static Display *dpy;

char *
smprintf(char *fmt, ...)
{
	va_list fmtargs;
	char *ret;
	int len;

	va_start(fmtargs, fmt);
	len = vsnprintf(NULL, 0, fmt, fmtargs);
	va_end(fmtargs);

	ret = malloc(++len);
	if (ret == NULL) {
		perror("malloc");
		exit(1);
	}

	va_start(fmtargs, fmt);
	vsnprintf(ret, len, fmt, fmtargs);
	va_end(fmtargs);

	return ret;
}

char*
run_command(const char* script) {
    FILE* fp;
    char path[1035];
    char* result = NULL;
    size_t result_size = 0;

    fp = popen(script, "r");
    if (fp == NULL) {
        printf("Failed to run command\n");
        exit(1);
    }

    /* Read the output a line at a time - adjust if the command output is longer */
    while (fgets(path, sizeof(path), fp) != NULL) {
        size_t len = strlen(path);
        result = realloc(result, result_size + len + 1);
        if (result == NULL) {
            printf("Failed to allocate memory\n");
            exit(1);
        }
        strcpy(result + result_size, path);
        result_size += len;
    }

    /* Close the pipe */
    pclose(fp);

    return result;
}

char *replace_str(char *str, char *orig, char *rep) {
    char *output;
    char *p;

    // Is 'orig' even in 'str'?
    if(!(p = strstr(str, orig)))
        return str;

    // Allocate memory for 'output'
    if(!(output = malloc(strlen(str) - strlen(orig) + strlen(rep) + 1)))
        return NULL;

    strncpy(output, str, p-str); // Copy characters from 'str' start to 'orig' st.
    output[p-str] = '\0';

    sprintf(output + (p - str), "%s%s", rep, p + strlen(orig));

    return output;
}

void
settz(char *tzname)
{
	setenv("TZ", tzname, 1);
}

char *
mktimes(char *fmt, char *tzname)
{
	char buf[129];
	time_t tim;
	struct tm *timtm;

	settz(tzname);
	tim = time(NULL);
	timtm = localtime(&tim);
	if (timtm == NULL)
		return smprintf("");

	if (!strftime(buf, sizeof(buf)-1, fmt, timtm)) {
		fprintf(stderr, "strftime == 0\n");
		return smprintf("");
	}

	return smprintf("%s", buf);
}

void
setstatus(char *str)
{
	XStoreName(dpy, DefaultRootWindow(dpy), str);
	XSync(dpy, False);
}

char *
loadavg(void)
{
	double avgs[3];

	if (getloadavg(avgs, 3) < 0)
		return smprintf("");

	return smprintf("%.2f %.2f %.2f", avgs[0], avgs[1], avgs[2]);
}

char *
readfile(char *base, char *file)
{
	char *path, line[513];
	FILE *fd;

	memset(line, 0, sizeof(line));

	path = smprintf("%s/%s", base, file);
	fd = fopen(path, "r");
	free(path);
	if (fd == NULL)
		return NULL;

	if (fgets(line, sizeof(line)-1, fd) == NULL) {
		fclose(fd);
		return NULL;
	}
	fclose(fd);

	return smprintf("%s", line);
}

char *
gettemperature(char *base, char *sensor)
{
	char *co;

	co = readfile(base, sensor);
	if (co == NULL)
		return smprintf("");
	return smprintf("%02.0f°C", atof(co) / 1000);
}

char *
execscript(char *cmd)
{
	FILE *fp;
	char retval[1025], *rv;

	memset(retval, 0, sizeof(retval));

	fp = popen(cmd, "r");
	if (fp == NULL)
		return smprintf("");

	rv = fgets(retval, sizeof(retval), fp);
	pclose(fp);
	if (rv == NULL)
		return smprintf("");
	retval[strlen(retval)-1] = '\0';

	return smprintf("%s", retval);
}

int
main(void)
{
	char *status;
	char *avgs;
	char *tbkk;
	char *t0;
	char *kbmap;
	char *pmdr;

	if (!(dpy = XOpenDisplay(NULL))) {
		fprintf(stderr, "dwmstatus: cannot open display.\n");
		return 1;
	}

	for (;;sleep(5)) {
		pmdr = run_command("/home/bb/scripts/pomodoro_status.sh");
		pmdr = replace_str(pmdr, "\n", " ");
		avgs = loadavg();
		tbkk = mktimes("%a %d %H:%M", tzbkk);
		kbmap = execscript("setxkbmap -query | grep layout | cut -d':' -f 2- | tr -d ' '");
		t0 = gettemperature("/sys/devices/virtual/thermal/thermal_zone0", "temp");

		status = smprintf(" %s | T:%s L:%s | %s",
				pmdr, t0, avgs, tbkk
				);
		setstatus(status);

		free(kbmap);
		free(t0);
		free(avgs);
		free(tbkk);
		free(status);
		free(pmdr);
	}

	XCloseDisplay(dpy);

	return 0;
}

