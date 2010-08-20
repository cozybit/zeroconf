/*
 * Copyright (C) cozybit, Inc. 2007-2010. All Rights Reserved.
 *
 * Use and redistribution subject to licensing terms.
 *
 * DNS Name helper functions
 */
#include "mdns.h"
#include "mdns_port.h"
#include "mdns_private.h"
#include "mdns_message.h"

/* d points to a dname pointer in a message.  return the offset in the
 * packet.
 */
#define POINTER(d) ((((*(d) & ~0xC0) << 8) | *((d) + 1)) & 0xFFFFU)
#define IS_POINTER(c) ((c) & 0xC0)

/* write the normal c string "label" to "dst" with appropriate dns length and
 * null termination.  Return a pointer to the byte after the last one written,
 * or 0 if the label was too long.
 */
char *dname_put_label(char *dst, char *label)
{
	int len;
	char *p = dst;

	len = (uint8_t)strlen(label);
	*p++ = len;
	strcpy(p, label);
	return p + len;
}

/* Find the size of a dname.  Note that it may contain pointers.  Note that the
 * size does include the number of bytes taken up by a pointer, but not the
 * size of the data it points to.  It also includes the byte taken up by the
 * terminating 0, if any.  return -1 for invalid names.
 */
int dname_size(char *dname)
{
	char *start = dname;
	while (*dname != 0x00) {
		if (*dname  > MDNS_MAX_LABEL_LEN)
			return -1;

		if (IS_POINTER(*dname)) { /* pointer */
			dname++;
			break;
		}

		/* we've found a valid label length */
		dname += *dname + 1;
	}
	return dname - start + 1;
}

/* compare l1 to l2.  return <0, 0, >0 like strcmp */
static int label_cmp(char *l1, char *l2)
{
	int i, min;
	char *p1, *p2;

	min = *l1 > *l2 ? *l2 : *l1;
	p1 = l1 + 1;
	p2 = l2 + 1;

	for (i = 0; i < min; i++) {
		if (*p1 > *p2)
			return 1;
		if (*p2 > *p1)
			return -1;
		p1++;
		p2++;
	}
	if (*l1 > *l2)
		return 1;
	if (*l2 > *l1)
		return -1;
	return 0;
}

/* compare the dname name n1 from the raw packet p1 to the dname n2 from the
 * raw packet p2.  Because the dnames may contain pointers, we need the
 * pointers to the raw packets.  Return 0 if the dnames are identical, >0 if n1
 * is lexicographically greater than n2, or <0 if n2 is greater than n1.  If
 * you are absolutely certain that a name doesn't contain pointers, you can
 * pass NULL as it's raw packet.
 */
int dname_cmp(char *p1, char *n1, char *p2, char *n2)
{
	int ret;

	while (1) {
		/* advance both names to the next valid pointers */
		while (IS_POINTER(*n1))
			n1 = p1 + POINTER(n1);
		while (IS_POINTER(*n2))
			n2 = p2 + POINTER(n2);

		if (*n1 == 0 && *n2 == 0)
			break;

		if (*n1 != 0 && *n2 != 0) {
			/* both n1 and n2 are pointing to labels by now */
			ret = label_cmp(n1, n2);
			if (ret != 0)
				return ret;
		}

		if (*n1 != 0 && *n2 == 0)
			/* we're done with n2 and n1 still has chars */
			return 1;

		if (*n1 == 0 && *n2 != 0)
			/* we're done with n2 and n1 still has chars */
			return -1;

		if (*n1 == 0 && *n2 == 0)
			/* we arrived at the end of both strings and they're still the
			 * same
			 */
			return 0;

		/* no conclusion yet.  advance to the next label */
		n1 += *n1 + 1;
		n2 += *n2 + 1;
	}
	return 0;
}

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

/* convert the ascii name from a list of labels separated by "sep".  Return the
 * length of the modified name, or -1 if there was an error.  name is not
 * allowed to begin with a separator.  dnames can't conceivable be bigger than
 * UINT16_MAX, if they are, this is an error.  If dest is NULL, the operation
 * is done in-place, otherwise the result is written to dest.
 */
int dnameify(char *name, char sep, char *dest)
{
	char *src;
	int len, labellen;

	/* now change all of the colons to lengths starting from the last
	 * char
	 */
	len = strlen(name);
	if (dest == NULL) {
		dest = name + len;
		src = dest - 1;
	} else {
		dest += len;
		src = name + len - 1;
	}
	len = 0;
	labellen = 0;

	while (1) {
		if (*src == sep && labellen == 0)
			return -1;

		if (dest == name || *src == sep ) {
			/* This is the beginning of a label.  Update its length and start
			 * looking at the next one.
			 */
			*dest = labellen;

			if (UINT16_MAX - len < labellen)
				return -1;

			len += labellen + 1;
			labellen = 0;

			if (dest == name)
				break;
			dest--;
			src--;
			continue;
		}
		/* move the char down */
		*dest-- = *src--;
		labellen++;
	}
	return len;
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

/* dname_length tests and test data */
char p0[] = {3, 'f', 'o', 'o', 5, 'l', 'o', 'c', 'a', 'l', 0};

/* p1 contains the name foo.local in a query and a pointer to that name at
 * offset 27.
 */
char p1[] = {0xBD, 0x38, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00,
			 0x00, 0x03, 0x66, 0x6F, 0x6F, 0x05, 0x6C, 0x6F, 0x63, 0x61, 0x6C,
			 0x00, 0x00, 0x01, 0x00, 0x01, 0xC0, 0x0C, 0x00, 0x01, 0x00, 0x01,
			 0x00, 0x00, 0x00, 0xFF, 0x00, 0x04, 0xC0, 0xA8, 0x01, 0x51, };

char p2[] = {64, 't', 'h', 'i', 's', 'n', 'a', 'm', 'e', 'i', 's', 't', 'h',
			 'e', 'm', 'a', 'x', 'l', 'a', 'b', 'e', 'l', 'l', 'e', 'n', 'g',
			 't', 'h', 'p', 'l', 'u', 's', '1', '0', '0', '0', '0', '0', '0',
			 '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
			 '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 5,
			 'l', 'o', 'c', 'a', 'l', 0};

char p3[] = {5, 'f', 'o', 'o', '-', '2', 5, 'l', 'o', 'c', 'a', 'l', 0};

char p4[] = {5, 'k', '1', '=', 'v', '1', 5, 'k', '2', '=', 'k', '3', 0};

char p5[] = {5, 'k', '1', '=', 'v', '1', 5, 'k', '2', '=', 'k', '2', 0};

static void dname_size_tests(void)
{
	int ret;
	test_title("dname_size");
	ret = dname_size(p0);
	FAIL_UNLESS(ret == 11, "Failed to find length of foo.local");
	ret = dname_size(&p1[27]);
	FAIL_UNLESS(ret == 2, "Failed to find length of name with pointer");
	ret = dname_size(&p2[0]);
	FAIL_UNLESS(ret == -1, "should have failed to parse max + 1 label");
}

static void dname_cmp_tests(void)
{
	int ret;
	test_title("dname_cmp");
	ret = dname_cmp(NULL, p0, NULL, p0);
	FAIL_UNLESS(ret == 0, "Failed to cmp a dname to itself");
	ret = dname_cmp(NULL, p0, p1, &p1[27]);
	FAIL_UNLESS(ret == 0,
				"Failed to cmp identical dnames, one direct one indirect.");
	ret = dname_cmp(p1, &p1[27], p1, &p1[27]);
	FAIL_UNLESS(ret == 0, "Failed to cmp identical indirect dnames.");
	ret = dname_cmp(p1, &p1[27], NULL, p0);
	FAIL_UNLESS(ret == 0,
				"Failed to cmp identical dnames, one indirect one direct.");
	ret = dname_cmp(p1, &p1[27], NULL, p3);
	FAIL_UNLESS(ret == -1,
				"foo-2.local should be greater than foo.local");
	ret = dname_cmp(NULL, p3, p1, &p1[27]);
	FAIL_UNLESS(ret == 1,
				"foo-2.local should be greater than foo.local");
	ret = dname_cmp(NULL, p4, NULL, p5);
	FAIL_UNLESS(ret == 1,
				"k1=v1.k2=v3 should be greater than k1=v1.k2=v2");

	ret = dname_cmp(NULL, p5, NULL, p4);
	FAIL_UNLESS(ret == -1,
				"k1=v1.k2=v3 should be less than k1=v1.k2=v2");
}

static void increment_name_tests(void)
{
	int ret, i;
	char n1[MDNS_MAX_NAME_LEN];
	char n2[MDNS_MAX_NAME_LEN];

	test_title("dname_increment");
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
	dname_size_tests();
	dname_cmp_tests();
	increment_name_tests();
}
#endif
