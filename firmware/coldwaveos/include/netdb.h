//
// Created by Sebastian Floss on 06.07.19.
// Copyright (c) 2019 ImagineOn GmbH. All rights reserved.
//

#ifndef _COLDWAVEOS_NETDB_H
#define _COLDWAVEOS_NETDB_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <sys/socket.h>

/**
* @defgroup tcpip TCP/IP
* @brief TCP/IP Socket Functions
* @details a socket interface as per the [Open Group Base Specifications](https://pubs.opengroup.org/onlinepubs/9699919799/)
* @{
*/

#ifndef EXCLUDE_FROM_DOCS

#define EAI_NONAME      200
#define EAI_SERVICE     201
#define EAI_FAIL        202
#define EAI_MEMORY      203
#define EAI_FAMILY      204

#define HOST_NOT_FOUND  210
#define NO_DATA         211
#define NO_RECOVERY     212
#define TRY_AGAIN       213

/* input flags for struct addrinfo */
#define AI_PASSIVE      0x01
#define AI_CANONNAME    0x02
#define AI_NUMERICHOST  0x04
#define AI_NUMERICSERV  0x08
#define AI_V4MAPPED     0x10
#define AI_ALL          0x20
#define AI_ADDRCONFIG   0x40

/* application accessible error code set by the DNS API functions */
extern int h_errno;

#endif


struct hostent {
    char *h_name;      /* Official name of the host. */
    char **h_aliases;   /* A pointer to an array of pointers to alternative host names,
                           terminated by a null pointer. */
    int h_addrtype;  /* Address type. */
    int h_length;    /* The length, in bytes, of the address. */
    char **h_addr_list; /* A pointer to an array of pointers to network addresses (in
                           network byte order) for the host, terminated by a null pointer. */
#define h_addr h_addr_list[0] /* for backward compatibility */
};

struct addrinfo {
    int ai_flags;      /* Input flags. */
    int ai_family;     /* Address family of socket. */
    int ai_socktype;   /* Socket type. */
    int ai_protocol;   /* Protocol of socket. */
    socklen_t ai_addrlen;    /* Length of socket address. */
    struct sockaddr *ai_addr;       /* Socket address of socket. */
    char *ai_canonname;  /* Canonical name of service location. */
    struct addrinfo *ai_next;       /* Pointer to next in list. */
};

/**
 * Returns an entry containing addresses of address family AF_INET
 * for the host with name name.
 * Due to dns_gethostbyname limitations, only one address is returned.
 *
 * @param name the hostname to resolve
 * @return an entry containing addresses of address family AF_INET
 *         for the host with name name
 */
struct hostent *
gethostbyname (const char *name);

/**
 * Thread-safe variant of gethostbyname: instead of using a static
 * buffer, this function takes buffer and errno pointers as arguments
 * and uses these for the result.
 *
 * @param name the hostname to resolve
 * @param ret pre-allocated struct where to store the result
 * @param buf pre-allocated buffer where to store additional data
 * @param buflen the size of buf
 * @param result pointer to a hostent pointer that is set to ret on success
 *               and set to zero on error
 * @param h_errnop pointer to an int where to store errors (instead of modifying
 *                 the global h_errno)
 * @return 0 on success, non-zero on error, additional error information
 *         is stored in *h_errnop instead of h_errno to be thread-safe
 */
int
gethostbyname_r (const char *name, struct hostent *ret, char *buf,
                 size_t buflen, struct hostent **result, int *h_errnop);

/**
 * Frees one or more addrinfo structures returned by getaddrinfo(), along with
 * any additional storage associated with those structures. If the ai_next field
 * of the structure is not null, the entire list of structures is freed.
 *
 * @param ai struct addrinfo to free
 */
void
freeaddrinfo (struct addrinfo *ai);

/**
* Translates the name of a service location (for example, a host name) and/or
	* a service name and returns a set of socket addresses and associated
	* information to be used in creating a socket with which to address the
* specified service.
* Memory for the result is allocated internally and must be freed by calling
* freeaddrinfo()!
*
* Due to a limitation in dns_gethostbyname, only the first address of a
	* host is returned.
* Also, service names are not supported (only port numbers)!
*
* @param nodename descriptive name or address string of the host
*                 (may be NULL -> local address)
* @param servname port number as string of NULL
* @param hints structure containing input values that set socktype and protocol
* @param res pointer to a pointer where to store the result (set to NULL on failure)
* @return 0 on success, non-zero on failure

*/
int
getaddrinfo (const char *nodename,
             const char *servname,
             const struct addrinfo *hints,
             struct addrinfo **res);

/** @} */

#ifdef __cplusplus
}
#endif

#endif //_COLDWAVEOS_NETDB_H
