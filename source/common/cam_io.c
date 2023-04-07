#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include "cam_log.h"

int32_t _mkdir_recursive(const char *dir)
{
	char tmp[256];
	char *p = NULL;
	size_t len;
	int rc;

	len = snprintf(tmp, sizeof(tmp), "%s", dir);
	if (len < 0 || len >= sizeof(tmp)) {
		CLOG_ERROR("%s: %s invalid", __func__, dir);
		return -1;
	}

	len = strlen(tmp);
	if (tmp[len - 1] == '/')
		tmp[len - 1] = 0;
	for (p = tmp + 1; *p; p++) {
		if (*p == '/') {
			*p = 0;
			rc = mkdir(tmp, 0755);
			if (rc != 0 && errno != EEXIST) {
				CLOG_ERROR("%s: %s created failed, %s",
				       __func__, tmp, strerror(errno));
				break;
			}
			*p = '/';
		}
	}
	rc = mkdir(tmp, 0755);
	if (rc == 0) {
		CLOG_INFO("%s created!", tmp);
	}

	return rc;
}

void timespec_diff(struct timespec *start, struct timespec *stop,
		   struct timespec *result)
{
	if ((stop->tv_nsec - start->tv_nsec) < 0) {
		result->tv_sec = stop->tv_sec - start->tv_sec - 1;
		result->tv_nsec = stop->tv_nsec - start->tv_nsec + 1000000000;
	} else {
		result->tv_sec = stop->tv_sec - start->tv_sec;
		result->tv_nsec = stop->tv_nsec - start->tv_nsec;
	}

	return;
}
