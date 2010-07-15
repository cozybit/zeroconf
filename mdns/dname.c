/* DNS Name helper functions
 */
#include "mdns.h"
#include "mdns_port.h"
#include "mdns_private.h"
#include "mdns_message.h"

/* Change the dname foo.local to foo-2.local.  If it already says foo-X.local,
 * make it foo-(X+1).local.  If it says foo-9.local, just leave it alone and
 * return -1.  name must be a valid dns name and must contain at least 2 extra
 * bytes.
 */
int dname_increment(char *name)
{
	int len = name[0], newlen;

	if (name[len - 1] == '-' && name[len] >= '2' && name[len] < '9') {
		name[len] += 1;
		return 0;
	}

	if (name[len - 1] == '-' && name[len] == '9')
		return -1;

	newlen = len + 2;
	if (newlen > MDNS_MAX_LABEL_LEN) {
		name[MDNS_MAX_LABEL_LEN - 1] = '-';
		name[MDNS_MAX_LABEL_LEN] = '2';
	} else {
		name[0] = newlen;
		/* don't forget to move trailing 0 */
		memmove(&name[len + 3], &name[len + 1], strlen(name) - len);
		name[len + 1] = '-';
		name[len + 2] = '2';
	}
	DBG("Derived new name: ");
	debug_print_name(NULL, name);
	DBG("\n");
	return 0;
}

#ifdef MDNS_TESTS
static int c_to_dname(char *dst, char *src)
{
	char *label = dst;
	int len = 0;

	dst++;
	while (*src != 0x00) {
		if (len > 63)
			/* label is too long */
			return -1;

		if (*src == '.') {
			*label = len;
			len = 0;
			label = dst;
			dst++;
			src++;
			continue;
		}
		*dst++ = *src++;
		len++;
	}
	*dst = 0;
	*label = len;
	return 0;
}

static void increment_name_tests(void)
{
	int ret, i;
	char n1[MDNS_MAX_NAME_LEN];
	char n2[MDNS_MAX_NAME_LEN];

	/* simple test case */
	ret = c_to_dname(n1, "foo.local");
	FAIL_UNLESS(ret == 0, "Failed to convert name to mdns name");
	ret = dname_increment(n1);
	FAIL_UNLESS(ret == 0, "Failed to increment foo to foo-2");
	ret = c_to_dname(n2, "foo-2.local");
	FAIL_UNLESS(ret == 0, "Failed to convert name to mdns name");	
	ret = strcmp(n1, n2);
	FAIL_UNLESS(ret == 0, "Failed to increment foo to foo-2");

	/* maximum name */
	ret = c_to_dname(n1, "thisnameisthemaxlabellength000000000000000000000000000000000000.local");
	FAIL_UNLESS(ret == 0, "Failed to convert longest possible name to mdns name");
	ret = dname_increment(n1);
	FAIL_UNLESS(ret == 0, "Failed to increment longest possible name");
	ret = c_to_dname(n2, "thisnameisthemaxlabellength0000000000000000000000000000000000-2.local");
	FAIL_UNLESS(ret == 0, "Failed to convert longest possible name to mdns name");
	ret = strcmp(n1, n2);
	FAIL_UNLESS(ret == 0, "Failed to increment longest possible name");

	/* several increments */
	ret = c_to_dname(n1, "myname.local");
	FAIL_UNLESS(ret == 0, "Failed to convert name to mdns name");
	for (i = 0; i < 6; i++)
		dname_increment(n1);
	ret = c_to_dname(n2, "myname-7.local");
	FAIL_UNLESS(ret == 0, "Failed to convert name to mdns name");	
	ret = strcmp(n1, n2);
	FAIL_UNLESS(ret == 0, "Failed to increment foo to foo-6");

	/* max increments */
	for (i = 0; i < 2; i++)
		dname_increment(n1);
	ret = c_to_dname(n2, "myname-9.local");
	FAIL_UNLESS(ret == 0, "Failed to convert name to mdns name");	
	ret = strcmp(n1, n2);
	FAIL_UNLESS(ret == 0, "Failed to increment foo to foo-9");

	/* over increment */
	ret = dname_increment(n1);
	FAIL_UNLESS(ret == -1, "Should have failed to over-increment.");
}

void dname_tests(void)
{
	increment_name_tests();
}
#endif
