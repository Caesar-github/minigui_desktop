/*
 * Copyright (c) 2018 rockchip
 *
 */

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <linux/iio/types.h>

#define LOG_TAG			"sysfs "

#define IIO_MAX_NAME_LENGTH	30
#define IIO_SYSFS_PATH		"/sys/bus/iio/devices/"
#define IIO_TYPE_DEVICE		"iio:device"

static int _write_sysfs_int(const char *filename, const char *basedir, int val,
			    bool verify)
{
	int ret = 0;
	FILE *sysfsfp;
	int test;
	char *temp = (char *)malloc(strlen(basedir) + strlen(filename) + 2);

	if (!temp)
		return -ENOMEM;

	ret = sprintf(temp, "%s/%s", basedir, filename);
	if (ret < 0)
		goto error_free;

	sysfsfp = fopen(temp, "w");
	if (!sysfsfp) {
		ret = -errno;
		printf(LOG_TAG"failed to open %s", temp);
		goto error_free;
	}

	ret = fprintf(sysfsfp, "%d", val);
	if (ret < 0) {
		if (fclose(sysfsfp))
			printf(LOG_TAG"%s: Failed to close dir", __func__);

		goto error_free;
	}

	if (fclose(sysfsfp)) {
		ret = -errno;
		goto error_free;
	}

	if (verify) {
		sysfsfp = fopen(temp, "r");
		if (!sysfsfp) {
			ret = -errno;
			printf(LOG_TAG"failed to open %s", temp);
			goto error_free;
		}

		if (fscanf(sysfsfp, "%d", &test) != 1) {
			ret = errno ? -errno : -ENODATA;
			if (fclose(sysfsfp))
				printf(LOG_TAG"%s: Failed to close dir", __func__);

			goto error_free;
		}

		if (fclose(sysfsfp)) {
			ret = -errno;
			goto error_free;
		}

		if (test != val) {
			printf(LOG_TAG"Possible failure in int write %d to %s/%s",
				 val, basedir, filename);
			ret = -1;
		}
	}

error_free:
	free(temp);
	return ret;
}

int write_sysfs_int(const char *filename, const char *basedir, int val)
{
	return _write_sysfs_int(filename, basedir, val, false);
}

int write_sysfs_int_and_verify(const char *filename, const char *basedir,
			       int val)
{
	return _write_sysfs_int(filename, basedir, val, true);
}

static int _write_sysfs_string(const char *filename, const char *basedir,
			       const char *val, bool verify)
{
	int ret = 0;
	FILE  *sysfsfp;
	char *temp = (char *)malloc(strlen(basedir) + strlen(filename) + 2);

	if (!temp) {
		printf(LOG_TAG"Memory allocation failed");
		return -ENOMEM;
	}

	ret = sprintf(temp, "%s/%s", basedir, filename);
	if (ret < 0)
		goto error_free;

	sysfsfp = fopen(temp, "w");
	if (!sysfsfp) {
		ret = -errno;
		printf(LOG_TAG"Could not open %s", temp);
		goto error_free;
	}

	ret = fprintf(sysfsfp, "%s", val);
	if (ret < 0) {
		if (fclose(sysfsfp))
			printf(LOG_TAG"%s: Failed to close dir", __func__);

		goto error_free;
	}

	if (fclose(sysfsfp)) {
		ret = -errno;
		goto error_free;
	}

	if (verify) {
		sysfsfp = fopen(temp, "r");
		if (!sysfsfp) {
			ret = -errno;
			printf(LOG_TAG"Could not open file to verify");
			goto error_free;
		}

		if (fscanf(sysfsfp, "%s", temp) != 1) {
			ret = errno ? -errno : -ENODATA;
			if (fclose(sysfsfp))
				printf(LOG_TAG"%s: Failed to close dir", __func__);

			goto error_free;
		}

		if (fclose(sysfsfp)) {
			ret = -errno;
			goto error_free;
		}

		if (strcmp(temp, val) != 0) {
			printf(LOG_TAG"Possible failure in string write of %s "
				 "Should be %s written to %s/%s", temp, val,
				 basedir, filename);
			ret = -1;
		}
	}

error_free:
	free(temp);

	return ret;
}

int write_sysfs_string_and_verify(const char *filename, const char *basedir,
				  const char *val)
{
	return _write_sysfs_string(filename, basedir, val, true);
}

int write_sysfs_string(const char *filename, const char *basedir,
		       const char *val)
{
	return _write_sysfs_string(filename, basedir, val, false);
}

int read_sysfs_posint(const char *filename, const char *basedir)
{
	int ret;
	FILE  *sysfsfp;
	char *temp = (char *)malloc(strlen(basedir) + strlen(filename) + 2);

	if (!temp) {
		printf(LOG_TAG"Memory allocation failed");
		return -ENOMEM;
	}

	ret = sprintf(temp, "%s/%s", basedir, filename);
	if (ret < 0)
		goto error_free;

	sysfsfp = fopen(temp, "r");
	if (!sysfsfp) {
		ret = -errno;
		goto error_free;
	}

	errno = 0;
	if (fscanf(sysfsfp, "%d\n", &ret) != 1) {
		ret = errno ? -errno : -ENODATA;
		if (fclose(sysfsfp))
			printf(LOG_TAG"%s: Failed to close dir", __func__);

		goto error_free;
	}

	if (fclose(sysfsfp))
		ret = -errno;

error_free:
	free(temp);

	return ret;
}

int read_sysfs_float(const char *filename, const char *basedir, float *val)
{
	int ret = 0;
	FILE  *sysfsfp;
	char *temp = (char *)malloc(strlen(basedir) + strlen(filename) + 2);

	if (!temp) {
		printf(LOG_TAG"Memory allocation failed");
		return -ENOMEM;
	}

	ret = sprintf(temp, "%s/%s", basedir, filename);
	if (ret < 0)
		goto error_free;

	sysfsfp = fopen(temp, "r");
	if (!sysfsfp) {
		ret = -errno;
		goto error_free;
	}

	errno = 0;
	if (fscanf(sysfsfp, "%f\n", val) != 1) {
		ret = errno ? -errno : -ENODATA;
		if (fclose(sysfsfp))
			printf(LOG_TAG"%s: Failed to close dir", __func__);

		goto error_free;
	}

	if (fclose(sysfsfp))
		ret = -errno;

error_free:
	free(temp);

	return ret;
}

int read_sysfs_string(const char *filename, const char *basedir, char *str)
{
	int ret = 0;
	FILE  *sysfsfp;
	char *temp = (char *)malloc(strlen(basedir) + strlen(filename) + 2);

	if (!temp) {
		printf(LOG_TAG"Memory allocation failed");
		return -ENOMEM;
	}

	ret = sprintf(temp, "%s/%s", basedir, filename);
	if (ret < 0)
		goto error_free;

	sysfsfp = fopen(temp, "r");
	if (!sysfsfp) {
		ret = -errno;
		goto error_free;
	}

	errno = 0;
	if (fscanf(sysfsfp, "%s\n", str) != 1) {
		ret = errno ? -errno : -ENODATA;
		if (fclose(sysfsfp))
			printf(LOG_TAG"%s: Failed to close dir", __func__);

		goto error_free;
	}

	if (fclose(sysfsfp))
		ret = -errno;

error_free:
	free(temp);

	return ret;
}

static int calc_digits(int num)
{
	int count = 0;

	while (num != 0) {
		num /= 10;
		count++;
	}

	return count;
}

int iio_find_type_by_name(const char *name, const char *type)
{
	const struct dirent *ent;
	int number, numstrlen, ret;

	FILE *namefp;
	DIR *dp;
	char thisname[IIO_MAX_NAME_LENGTH];
	char *filename;

	dp = opendir(IIO_SYSFS_PATH);
	if (!dp) {
		fprintf(stderr, "No industrialio devices available");
		return -ENODEV;
	}

	while (ent = readdir(dp), ent) {
		if (strcmp(ent->d_name, ".") != 0 &&
		    strcmp(ent->d_name, "..") != 0 &&
		    strlen(ent->d_name) > strlen(type) &&
		    strncmp(ent->d_name, type, strlen(type)) == 0) {
			errno = 0;
			ret = sscanf(ent->d_name + strlen(type), "%d", &number);
			if (ret < 0) {
				ret = -errno;
				printf(LOG_TAG"failed to read element number");
				goto error_close_dir;
			} else if (ret != 1) {
				ret = -EIO;
				printf(LOG_TAG"failed to match element number");
				goto error_close_dir;
			}

			numstrlen = calc_digits(number);
			/* verify the next character is not a colon */
			if (strncmp(ent->d_name + strlen(type) + numstrlen,
			    ":", 1) != 0) {
				filename = (char *)malloc(strlen(IIO_SYSFS_PATH)
						+ strlen(type) + numstrlen + 6);
				if (!filename) {
					ret = -ENOMEM;
					goto error_close_dir;
				}

				ret = sprintf(filename, "%s%s%d/name",
					      IIO_SYSFS_PATH, type, number);
				if (ret < 0) {
					free(filename);
					goto error_close_dir;
				}

				namefp = fopen(filename, "r");
				if (!namefp) {
					free(filename);
					continue;
				}

				free(filename);
				errno = 0;
				if (fscanf(namefp, "%s", thisname) != 1) {
					ret = errno ? -errno : -ENODATA;
					goto error_close_dir;
				}

				if (fclose(namefp)) {
					ret = -errno;
					goto error_close_dir;
				}

				if (strcmp(name, thisname) == 0) {
					if (closedir(dp) == -1)
						return -errno;

					return number;
				}
			}
		}
	}
	if (closedir(dp) == -1)
		return -errno;

	return -ENODEV;

error_close_dir:
	if (closedir(dp) == -1)
		printf(LOG_TAG"%s: Failed to close directory", __func__);

	return ret;
}

