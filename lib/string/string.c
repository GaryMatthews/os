#include <string.h>
#include <stdint.h>

/* TODO: - use unsigned lengths */

size_t strlen( const char *input ){
	int i;

	for ( i = 0; input[i] != '\0'; i++ );

	return i;
}

int strcmp( const char *s1, const char *s2 ){
	int ret = 0;
	unsigned i;

	for ( i = 0;; i++ ){
		ret += s1[i] != s2[i];

		if ( !s1[i] || !s2[i] )
			break;
	}

	return ret;
}

int strncmp( const char *s1, const char *s2, size_t len ){
	int ret = 0;
	unsigned i;

	for ( i = 0; i < len; i++ ){
		ret += s1[i] != s2[i];

		if ( !s1[i] || !s2[i] )
			break;
	}

	return ret;
}

/*
char *strdup( const char *s ){
	char *ret = kmalloc( strlen( s ) + 1 );

	return memcpy( ret, s, strlen( s ) + 1 );
    }*/

char *strcpy( char *dest, const char *src ){
	char *ret = dest;
	int i;

	for ( i = 0; src[i]; i++ )
		dest[i] = src[i];

	dest[i] = 0;

	return ret;
}

char *strncpy( char *dest, const char *src, size_t len ){
	char *ret = dest;
	unsigned i;

	for ( i = 0; src[i] && i < len; i++ )
		dest[i] = src[i];

	dest[i] = 0;

	return ret;
}

char *strcat( char *dest, const char *src ){
	char *ret = dest;
	unsigned i;

	for ( i = 0; dest[i]; i++ );
	strcpy( dest + i, src );

	return ret;
}

char *strncat( char *dest, const char *src, size_t n ){
	char *ret = dest;

	unsigned i;

	for ( i = 0; dest[i]; i++ );
	strncpy( dest + i, src, n );

	return ret;
}

/* Basic memory stuff */
void *memset( void *dest, int value, size_t count ){
	uint8_t *ret_dest = dest;

	while ( count-- ){
		*(ret_dest++) = value;
	}

	return dest;
}

void *memsetw( void *dest, unsigned char value, size_t count ){
	uint8_t *ret_dest = dest;

	while ( count-- ){
		*(ret_dest++) = value;
	}

	return dest;
}

void *memcpy( void *dest, const void *src, size_t count ){
	asm volatile("cld; rep movsb"
	            : "=c"((int){0})
	            : "D"(dest), "S"(src), "c"(count)
	            : "flags", "memory");
	return dest;
/*
	uint8_t *byte_ret;
	const uint8_t *byte_src;

	uint32_t *long_ret = dest;
	const uint32_t *long_src = src;

	while ( count > 4 ){
		*(long_ret++) = *(long_src++);
		count -= 4;
	}

	byte_ret = (uint8_t *)long_ret;
	byte_src = (const uint8_t *)long_src;

	while ( count ){
		*(byte_ret++) = *(byte_src++);
		count--;
	}

	return dest;
*/
}

void *memmove( void *dest, const void *src, size_t count ){
	uint8_t *ret_dest = dest;
	const uint8_t *src_dest = src;

	while ( count-- ){
		*(ret_dest++) = *(src_dest++);
	}

	return dest;
}

int memcmp( const void *s1, const void *s2, size_t len ){
	int ret = 0;
	unsigned i;

	const uint8_t *foo = s1;
	const uint8_t *bar = s2;

	for ( i = 0; i < len; i++ ){
		ret += foo[i] != bar[i];
	}

	return ret;
}

char *strchr(char *str, int c) {
    while(*str) {
        if(*str == c)
            return str;
        str++;
    }
    return NULL;
}

void itoa(char *buf, unsigned long int n, int base) {
    unsigned long int tmp;
    int i, j;

    tmp = n;
    i = 0;

    do {
        tmp = n % base;
        buf[i++] = (tmp < 10) ? (tmp + '0') : (tmp + 'a' - 10);
    } while (n /= base);
    buf[i--] = 0;

    for (j = 0; j < i; j++, i--) {
        tmp = buf[j];
        buf[j] = buf[i];
        buf[i] = tmp;
    }
}
