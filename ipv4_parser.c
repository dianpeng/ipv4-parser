#include "ipv4_parser.h"
#include <assert.h>
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

enum {
    D21,
    D22,
    D23,
    D31,
    D32,
    REQUIRE_DOT
};

static int check_ipv4_address_component( const char* ipv4 , int index , int* err ) {
    int state;
    //digit 1
    switch( ipv4[index] ) {
    case '1': state = D21; break;
    case '2': state = D22; break;
    case '3': case '4': case '5': case '6': case '7': case '8':case '9':
        state = D23; break;
    default: *err = index; return -1;
    }
    //digit 2
    switch(state) {
    case D21:
        switch(ipv4[index+1]) {
        case '0':case '1':case '2':case '3':case '4':
        case '5':case '6':case '7':case '8':case '9':
            state = D31; break;
        case '.': return index+2;
        default: *err = index + 1 ; return -1;
        } 
        break;
    case D22:
        switch(ipv4[index+1]) {
        case '0':case '1':case '2':case '3':case '4':
            state = D31; break;
        case '5':
            state = D32; break;
        case '6':case '7':case '8':case '9':
            state = REQUIRE_DOT;
            break;
        case '.':
            return index+2;
        default: *err= index+1; return -1;
        }
        break;
    case D23:
        switch(ipv4[index+1]) {
        case '0':case '1':case '2':case '3':case '4':
        case '5':case '6':case '7':case '8':case '9':
            state = REQUIRE_DOT;
            break;
        case '.':
            return index+2;
        default: *err = index+1; return -1;
        }
        break;
    }
    //digit 3
    switch(state) {
    case D31:
        switch(ipv4[index+2]) {
        case '0':case '1':case '2':case '3':case '4':
        case '5':case '6':case '7':case '8':case '9':
            state = REQUIRE_DOT;
            break;
        case '.': return index+3;
        default: *err = index+2; return -1;
        }
        break;
    case D32:
        switch(ipv4[index+2]) {
        case '0':case '1':case '2': case '3':case '4':case '5':
            state = REQUIRE_DOT;
            break;
        case '.': return index+3;
        default: *err = index+2; return -1;
        }
        break;
    case REQUIRE_DOT:
        if(ipv4[index+2] == '.') {
            return index+3;
        } else {
            *err = index+2; return -1;
        }
        break;
    default: assert(0);
    }
    assert(state == REQUIRE_DOT);
    if( ipv4[index+3] != '.' ) {
        *err = index+3;
        return -1;
    }
    return index+4;
}

// last component is slightly different in terms of previous state

static int check_ipv4_address_last_component( const char* ipv4 , int index , int* err ) {
    int state;
    //digit 1
    switch( ipv4[index] ) {
    case '1': state = D21; break;
    case '2': state = D22; break;
    case '3': case '4': case '5': case '6': case '7': case '8':case '9':
        state = D23; break;
    default: *err = index; return -1;
    }
    //digit 2
    switch(state) {
    case D21:
        switch(ipv4[index+1]) {
        case '0':case '1':case '2':case '3':case '4':
        case '5':case '6':case '7':case '8':case '9':
            state = D31; break;
        default: *err = index + 1; return 0;
        } 
        break;
    case D22:
        switch(ipv4[index+1]) {
        case '0':case '1':case '2':case '3':case '4':
            state = D31; break;
        case '5':
            state = D32; break;
        case '6':case '7':case '8':case '9':
            *err = index+2;
            return 0;
        default: *err= index+1; return 0;
        }
        break;
    case D23:
        switch(ipv4[index+1]) {
        case '0':case '1':case '2':case '3':case '4':
        case '5':case '6':case '7':case '8':case '9':
            *err = index+2;
            return 0;
        default: *err = index+1; return 0;
        }
        break;
    }
    //digit 3
    switch(state) {
    case D31:
        switch(ipv4[index+2]) {
        case '0':case '1':case '2':case '3':case '4':
        case '5':case '6':case '7':case '8':case '9':
            state = REQUIRE_DOT;
            break;
        default: *err = index+2 ; return 0;
        }
        break;
    case D32:
        switch(ipv4[index+2]) {
        case 0: return index+2;
        case '0':case '1':case '2': case '3':case '4':case '5':
            state = REQUIRE_DOT;
            break;
        default: *err = index+2; return 0;
        }
        break;
    default: assert(0);
    }
    ++(*err);
    return 0;
}


int is_ipv4_address( const char* ipv4 , int* err ) {
    int index = check_ipv4_address_component(ipv4,0,err);
    if( index == -1 ) return -1;
    index = check_ipv4_address_component(ipv4,index,err);
    if( index == -1 ) return -1;
    index = check_ipv4_address_component(ipv4,index,err);
    if( index == -1 ) return -1;
    return check_ipv4_address_last_component(ipv4,index,err);
}

static int parse_ipv4_address_component( const char* ipv4 , int index , int* err , int* comp ) {
    int ret;
    int state;
    //digit 1
    switch( ipv4[index] ) {
    case '1': ret = 1; state = D21; 
        ret = 1; break;
    case '2': ret = 2; state = D22;
        ret = 2; break;
    case '3': case '4': case '5':
    case '6': case '7': case '8':case '9':
        ret = ipv4[index]-'0'; state = D23; break;
    default: *err = index; return -1;
    }
    //digit 2
    switch(state) {
    case D21:
        switch(ipv4[index+1]) {
        case '0':case '1':case '2':case '3':case '4':
        case '5':case '6':case '7':case '8':case '9':
            ret*=10 ; ret += ipv4[index+1]-'0'; state = D31; break;
        case '.': *comp = ret; return index+2;
        default: *err = index + 1 ; return -1;
        } 
        break;
    case D22:
        switch(ipv4[index+1]) {
        case '0':case '1':case '2':case '3':case '4':
            ret*=10 ; ret += ipv4[index+1]-'0';state = D31; break;
        case '5':
            ret*=10; ret += 5;
            state = D32; break;
        case '6':case '7':case '8':case '9':
            ret += ipv4[index+1]-'0';
            state = REQUIRE_DOT;
            break;
        case '.':
            *comp = ret;
            return index+2;
        default: *err= index+1; return -1;
        }
        break;
    case D23:
        switch(ipv4[index+1]) {
        case '0':case '1':case '2':case '3':case '4':
        case '5':case '6':case '7':case '8':case '9':
            ret *= 10 ; ret += ipv4[index+1]-'0';
            state = REQUIRE_DOT;
            break;
        case '.':
            *comp = ret;
            return index+2;
        default: *err = index+1; return -1;
        }
        break;
    }
    //digit 3
    switch(state) {
    case D31:
        switch(ipv4[index+2]) {
        case '0':case '1':case '2':case '3':case '4':
        case '5':case '6':case '7':case '8':case '9':
            ret *= 10 ; ret += ipv4[index+2]-'0';
            state = REQUIRE_DOT;
            break;
        case '.':
            *comp = ret;
            return index+3;
        }
        break;
    case D32:
        switch(ipv4[index+2]) {
        case '0':case '1':case '2':
        case '3':case '4':case '5':
            ret *= 10 ; ret += ipv4[index+2]-'0';
            state = REQUIRE_DOT;
            break;
        case '.':
            *comp = ret;
            return index+3;
        default: *err = index+2; return -1;
        }
        break;
    case REQUIRE_DOT:
        if(ipv4[index+2] == '.') {
            *comp = ret;
            return index+3;
        } else {
            *err = index+2; return -1;
        }
        break;
    default: assert(0);
    }
    assert(state == REQUIRE_DOT);
    if( ipv4[index+3] != '.' ) {
        *err = index+3;
        return -1;
    }
    *comp = ret;
    return index+4;
}

static int parse_ipv4_address_last_component( const char* ipv4 , int index , int* err , int* comp ) {
    int ret;
    int state;
    //digit 1
    switch( ipv4[index] ) {
    case '1': ret = 1; state = D21; 
        ret = 1; break;
    case '2': ret = 2; state = D22;
        ret = 2; break;
    case '3': case '4': case '5':
    case '6': case '7': case '8':case '9':
        ret = ipv4[index]-'0'; state = D23; break;
    default: *err = index; return -1;
    }
    //digit 2
    switch(state) {
    case D21:
        switch(ipv4[index+1]) {
        case '0':case '1':case '2':case '3':case '4':
        case '5':case '6':case '7':case '8':case '9':
            ret*=10 ; ret += ipv4[index+1]-'0'; state = D31; break;
        default: *comp = ret; *err = index + 1 ; return 0;
        } 
        break;
    case D22:
        switch(ipv4[index+1]) {
        case '0':case '1':case '2':case '3':case '4':
            ret*=10 ; ret += ipv4[index+1]-'0';state = D31; break;
        case '5':
            ret*=10; ret += 5;
            state = D32; break;
        case '6':case '7':case '8':case '9':
            ret += ipv4[index+1]-'0';
            *comp =ret;
            *err = index+2;
            return 0;
        default: *comp = ret; *err= index+1; return 0;
        }
        break;
    case D23:
        switch(ipv4[index+1]) {
        case '0':case '1':case '2':case '3':case '4':
        case '5':case '6':case '7':case '8':case '9':
            ret *= 10 ; ret += ipv4[index+1]-'0';
            *comp =ret;
            *err = index+2;
            return 0;
        default: *comp = ret; *err = index+1; return 0;
        }
        break;
    }
    //digit 3
    switch(state) {
    case D31:
        switch(ipv4[index+2]) {
        case 0: *comp = ret; return index+3;
        case '0':case '1':case '2':case '3':case '4':
        case '5':case '6':case '7':case '8':case '9':
            ret *= 10 ; ret += ipv4[index+2]-'0';
            state = REQUIRE_DOT;
            break;
        default: 
            *comp = ret; *err = index+2; return 0;
        }
        break;
    case D32:
        switch(ipv4[index+2]) {
        case '0':case '1':case '2':
        case '3':case '4':case '5':
            ret *= 10 ; ret += ipv4[index+2]-'0';
            state = REQUIRE_DOT;
            break;
        default: 
            *comp = ret; *err = index+2; return -1;
        }
        break;
    default: assert(0);
    }
    assert(state == REQUIRE_DOT);
    *err = index+3;
    *comp = ret;
    return 0;
}

int parse_ipv4_address( const char* ipv4 , int* err , int res[4] ) {
    int component;
    int index ;
    index = parse_ipv4_address_component(ipv4,0,err,&component);
    if( index == -1 )  return -1;
    res[0] = component;
    index = parse_ipv4_address_component(ipv4,index,err,&component);
    if( index == -1 )  return -1;
    res[1] = component ;
    index = parse_ipv4_address_component(ipv4,index,err,&component);
    if( index == -1 )  return -1;
    res[2] = component;
    index = parse_ipv4_address_last_component(ipv4,index,err,&component);
    res[3] = component;
    return index;
}

#ifdef __cplusplus
};
#endif // __cplusplus
