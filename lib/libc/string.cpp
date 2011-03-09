#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

char*
strlast(char* s)
{
	while (*s++) {
	}
	return s-2;
}

char*
strrev(char* s)
{
	return (char*)memrev(s, strlast(s));
}

unsigned
strlen(const char* s)
{
	unsigned rc = 0;
	while (s[rc]) {
		++rc;
	}
	return rc;
}

unsigned
strnlen(const char* s, unsigned length)
{
	unsigned rc = 0;
	while (s[rc] != '\0' && length>0) {
		++rc;
		--length;
	}
	return rc;
}

char*
strcpy(char* s1, const char* s2)
{
	char* rc = s1;
	while ((*s1++ = *s2++)) {
	}
	return rc;
}

char*
strncpy(char* dest, const char* source, unsigned length)
{
	char* start = dest;
	while (length && (*dest++ = *source++)) {
		length--;
	}
	if (length) {
		*dest = '\0';
	}
	return start;
}

char*
strcat(char* s1, const char* s2)
{
	char* rc = s1;
	if (*s1) {
		while (*++s1) {
		}
	}
	while ((*s1++ = *s2++)) {
	}
	return rc;
}

char*
strdup(const char* s)
{
	if (!s) {
		return NULL;
	}
	size_t sl = strlen(s);
	char* t = (char*)malloc(sl + 1);
	memcpy(t, (const void*)s, sl + 1);
	return t;
}

int
strcmp(const char* s1, const char* s2)
{
	while ((*s1) && (*s1 == *s2)) {
		++s1;
		++s2;
	}
	return (*s1 - *s2);
}

int
stricmp(const char* s1, const char* s2)
{
	while ((*s1) && (tolower(*s1) == tolower(*s2))) {
		++s1;
		++s2;
	}
	return (*s1 - *s2);
}

int
strncmp(const char* s1, const char* s2, int n)
{
	while ((--n > 0) && (*s1) && (*s1 == *s2)) {
		++s1;
		++s2;
	}
	return (*s1 - *s2);
}

int
strnicmp(const char* s1, const char* s2, int n)
{
	while ((--n > 0) && (*s1) && (tolower(*s1) == tolower(*s2))) {
		++s1;
		++s2;
	}
	return (*s1 - *s2);
}

char*
strchr(const char* s, int c)
{
	do {
		if (*s == (char) c) {
			return (char*) s;
		}
	} while (*s++);
	return NULL;
}

char*
strrchr(const char* s, int c)
{
	size_t i = 0;
	while (s[i++]) {
	}
	do {
		if (s[--i] == (char)c)
			return (char*) s + i;
	} while (i);
	return NULL;
}

char*
strstr(const char* s1, const char* s2)
{
	const char* p1 = s1;
	const char* p2;
	while (*s1) {
		p2 = s2;
		while (*p2 && (*p1 == *p2)) {
			++p1;
			++p2;
		}
		if (! *p2) {
			return (char*) s1;
		}
		++s1;
		p1 = s1;
	}
	return NULL;
}

char*
strpadr (char* s, const size_t fl, const char c)
{
	const ptrdiff_t sl = strlen(s);
	const ptrdiff_t paste_cnt = fl - sl;
	if (paste_cnt>0) {
		for (ptrdiff_t i=sl; i>=0; --i) {
			s[i+paste_cnt]=s[i];
		}
		for (ptrdiff_t i=0; i<paste_cnt; ++i) {
			s[i]=c;
		}
	} else if (paste_cnt<0) {
		for(size_t i = 0; i<=fl; ++i) {
			s[i]=s[i-paste_cnt];
		}
	}
	return s;
}

int
strlike (char* str, char* mask)
{
	if (!(str && mask)) {
		return false;
	}
	while (*mask) {
		if (*mask == '*') {
			while (*mask == '*') {
				mask++;
			}
			if (*mask == '\0') {
				return true;
			}
			while (*str) {
				if (strlike(str, mask)) {
					return true;
				}
				str++;
			}
			return false;
		} else if (*mask == *str || (*mask == '?' && *str != '\0')) {
			str++;
			mask++;
		} else {
			return false;
		}
	}
	return *str == '\0';
}

char*
strtok (char* s1, const char* s2)
{
	static char* tmp = NULL;
	const char* p = s2;

	if (s1 != NULL) {
		tmp = s1;
	} else {
		if (tmp == NULL) {
			return NULL;
		}
		s1 = tmp;
	}

	while (*p && *s1) {
		if (*s1 == *p) {
			++s1;
			p = s2;
			continue;
		}
		++p;
	}

	if (! *s1) {
		return (tmp = NULL);
	}

	tmp = s1;
	while (*tmp) {
		p = s2;
		while (*p) {
			if (*tmp == *p++) {
				*tmp++ = '\0';
				return s1;
			}
		}
		++tmp;
	}
	return (tmp = NULL);
}

