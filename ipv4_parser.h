#ifndef __IPV4_PARSER_H__
#define __IPV4_PARSER_H__
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


// The following function will try to validate/parse the IPV4 address specify
// in the buffer. The return value can be zero or negative value.
// 0 means it finds/parses a valid IPV4 address initiate inside of the buffer,
// negative value means it cannot find a valid IPV4 address. The string must be
// a valid string but the validation/parsing will only continue when it finds 
// enough character to trust it as a valid ipv4 address. So a string:
// 127.1.2.4a will be a valid IPV4 address, and index will point to the character
// which is the first character that should not belong to IPV4 address. However,
// 1.a.3.4 will return negative value.

int is_ipv4_address( const char* , int* index );

int parse_ipv4_address( const char* , int* index , int res[4] );

#ifdef __cplusplus
};
#endif // __cplusplus


#endif // __IPV4_PARSER_H__