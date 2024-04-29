/********************************************************************************
  * @file    socket.h
  * @author  ImagineOn cxOS development team
  *          Copyright (c) 2020 ImagineOn GmbH. All rights reserved.
  * @brief   POSIX Socket wrapper
  * @copyright  ImagineoOn GmbH
  ********************************************************************************/


#ifndef _SOCKET_H
#define _SOCKET_H

#ifdef __cplusplus
extern "C" {
#endif

#include <unistd.h>
#include <arpa/inet.h>

/**
 * @defgroup tcpip TCP/IP
 * @brief TCP/IP Socket Functions
 * @details a socket interface as per the [Open Group Base Specifications](https://pubs.opengroup.org/onlinepubs/9699919799/)
 * @{
 */
#ifndef EXCLUDE_FROM_DOCS

#define AF_UNSPEC       0
#define AF_INET         2
#define TCP_NODELAY     0x01                    // don't delay send to coalesce packets
#define F_GETFL         3                       // Get file descriptors
#define F_SETFL         4                       // Set a subset of file descriptors (e.g. O_NONBlOCK)

#define SOCK_STREAM     1
#define SOCK_DGRAM      2
#define SOCK_RAW        3

#define AF_UNSPEC       0
#define AF_INET         2
#define AF_INET6        AF_UNSPEC
#define PF_INET         AF_INET
#define PF_INET6        AF_INET6
#define PF_UNSPEC       AF_UNSPEC

#define IPPROTO_IP      0
#define IPPROTO_ICMP    1
#define IPPROTO_TCP     6
#define IPPROTO_UDP     17
#define IPPROTO_UDPLITE 136
#define IPPROTO_RAW     255

/* Flags we can use with send and recv. */
#define MSG_PEEK       0x01    /* Peeks at an incoming message */
#define MSG_WAITALL    0x02    /* Unimplemented: Requests that the function block until the full amount of data requested can be returned */
#define MSG_OOB        0x04    /* Unimplemented: Requests out-of-band data. The significance and semantics of out-of-band data are protocol-specific */
#define MSG_DONTWAIT   0x08    /* Nonblocking i/o for this operation only */
#define MSG_MORE       0x10    /* Sender will send more */
#define MSG_NOSIGNAL   0x20    /* Uninmplemented: Requests not to send the SIGPIPE signal if an attempt to send is made on a stream-oriented socket that is no longer connected. */

#define SOL_SOCKET      0xfff                   // Define the socket option category

#define SO_REUSEADDR    0x0004 /* Allow local address reuse */
#define SO_BROADCAST    0x0020 /* permit to send and to receive broadcast messages (see IP_SOF_BROADCAST option) */
#define SO_REUSEPORT    0x0200 /* Unimplemented: allow local address & port reuse */
#define SO_RCVTIMEO     0x1006 /* receive timeout */


/*
 * Additional options, not kept in so_options.
 */
#define SO_DEBUG        0x0001 ///< Unimplemented: turn on debugging info recording
#define SO_ACCEPTCONN   0x0002 ///<  socket has had listen()
#define SO_DONTROUTE    0x0010 ///<  Unimplemented: just use interface addresses
#define SO_USELOOPBACK  0x0040 ///<  Unimplemented: bypass hardware when possible
#define SO_LINGER       0x0080 ///< linger on close if data present
#define SO_DONTLINGER   ((int)(~SO_LINGER))
#define SO_OOBINLINE    0x0100 ////< Unimplemented: leave received OOB data in line
#define SO_REUSEPORT    0x0200 ///< Unimplemented: allow local address & port reuse
#define SO_SNDBUF       0x1001 ///<  Unimplemented: send buffer size
#define SO_RCVBUF       0x1002 ///<  receive buffer size
#define SO_SNDLOWAT     0x1003 ///<  Unimplemented: send low-water mark
#define SO_RCVLOWAT     0x1004 ///<  Unimplemented: receive low-water mark
#define SO_SNDTIMEO     0x1005 ///<  send timeout
#define SO_RCVTIMEO     0x1006 ///<  receive timeout
#define SO_ERROR        0x1007 ///<  get error status and clear
#define SO_TYPE         0x1008 ///<  get socket type /
#define SO_CONTIMEO     0x1009 ///<  Unimplemented: connect timeout
#define SO_NO_CHECK     0x100a ///<  don't create UDP checksum
#define SO_BINDTODEVICE 0x100b ///<  bind to device

#ifndef O_NONBLOCK
#define O_NONBLOCK      1                       // nonblocking I/O
#endif

#ifndef SHUT_RD
#define SHUT_RD   0
#define SHUT_WR   1
#define SHUT_RDWR 2
#endif

#if BYTE_ORDER == BIG_ENDIAN
#define htons(x)   ((uint16_t)(x))
#define ntohs(x)   ((uint16_t)(x))
#define htonl(x)   ((uint32_t)(x))
#define ntohl(x)   ((uint32_t)(x))
#else /* BYTE_ORDER != BIG_ENDIAN */
#define htons(x) ((uint16_t)((((x) & (uint16_t)0x00ffU) << 8) | (((x) & (uint16_t)0xff00U) >> 8)))
#define ntohs(x) htons(x)
#define htonl(x) ((((x) & (uint32_t)0x000000ffUL) << 24) | \
                     (((x) & (uint32_t)0x0000ff00UL) <<  8) | \
                     (((x) & (uint32_t)0x00ff0000UL) >>  8) | \
                     (((x) & (uint32_t)0xff000000UL) >> 24))
#define ntohl(x) htonl(x)
#endif /* BYTE_ORDER == BIG_ENDIAN */

#define DNS_MAX_NAME_LENGTH             256

typedef uint8_t sa_family_t;
typedef uint16_t in_port_t;
typedef uint32_t socklen_t;
typedef uint32_t in_addr_t;

#endif

#ifndef EXCLUDE_FROM_DOCS
struct ip4_addr {
	uint32_t addr;
};

typedef struct ip4_addr ip4_addr_t;
typedef ip4_addr_t ip_addr_t;
#endif

struct in_addr {
	in_addr_t s_addr;
};

struct sockaddr_in
{
	uint8_t sin_len;
	sa_family_t sin_family;
	in_port_t sin_port;
	struct in_addr sin_addr;
#define SIN_ZERO_LEN 8
	char sin_zero[SIN_ZERO_LEN];
};

struct sockaddr
{
	uint8_t sa_len;
	sa_family_t sa_family;
	char sa_data[14];
};

struct sockaddr_storage
{
	uint8_t s2_len;
	sa_family_t ss_family;
	char s2_data1[2];
	uint32_t s2_data2[3];
};

///
/// \param domain
/// \param type
/// \param protocol
/// \return
int socket (int domain, int type, int protocol);

///
/// \param s
/// \param name
/// \param namelen
/// \return
int connect (int s, const struct sockaddr *name, socklen_t namelen);

///
/// \param s
/// \param name
/// \param namelen
/// \return
int bind (int s, const struct sockaddr *name, socklen_t namelen);

///
/// \param s
/// \param backlog
/// \return
int listen (int s, int backlog);

///
/// \param s
/// \param addr
/// \param addrlen
/// \return
int accept (int s, struct sockaddr *addr, socklen_t *addrlen);

///
/// \param s
/// \param buf
/// \param len
/// \param flags
/// \return
int recv (int s, void *buf, size_t len, int flags);

///
/// \param maxfdp1
/// \param readset
/// \param writeset
/// \param exceptset
/// \param timeout
/// \return
int select(int maxfdp1, fd_set *readset, fd_set *writeset, fd_set *exceptset,
		   struct timeval *timeout);

///
/// \param s
/// \param data
/// \param size
/// \param flags
/// \return
int send (int s, const void *data, size_t size, int flags);

///
/// \param s
/// \param how
/// \return
int shutdown (int s, int how);

///
/// \param s
/// \param data
/// \param size
/// \param flags
/// \param to
/// \param tolen
/// \return
ssize_t sendto (int s, const void *data, size_t size, int flags, const struct sockaddr *to, socklen_t tolen);

///
/// \param s
/// \param mem
/// \param len
/// \param flags
/// \param from
/// \param fromlen
/// \return
ssize_t recvfrom (int s, void *mem, size_t len, int flags, struct sockaddr *from, socklen_t *fromlen);

///
/// \param cp
/// \return
in_addr_t inet_addr (const char *cp);

///
/// \param s
/// \param cmd
/// \param val
/// \return
int fcntl (int s, int cmd, ...);

///
/// \param af
/// \param src
/// \param dst
/// \return
int inet_pton (int af, const char *src, void *dst);

///
/// \param af
/// \param src
/// \param dst
/// \param size
/// \return
const char *inet_ntop (int af, const void *src, char *dst, socklen_t size);

///
/// \param socket
/// \param level
/// \param option_name
/// \param option_value
/// \param option_len
/// \return
int setsockopt (int socket, int level, int option_name, const void *option_value, socklen_t option_len);

///
/// \param s
/// \param level
/// \param optname
/// \param optval
/// \param optlen
/// \return
int
getsockopt (int s, int level, int optname, void *optval, socklen_t *optlen);

///
/// \param s
/// \param name
/// \param namelen
/// \return
int
getsockname (int s, struct sockaddr *name, socklen_t *namelen);


/** @} */

#ifdef __cplusplus
}
#endif

#endif //_SOCKET_H