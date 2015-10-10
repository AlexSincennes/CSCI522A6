#ifndef USOCKETTYPES_H
#define USOCKETTYPES_H
/*=========================================================================*\
* Socket compatibilization module for Unix
* LuaSocket toolkit
*
* RCS ID: $Id: usocket.h,v 1.7 2005/10/07 04:40:59 diego Exp $
\*=========================================================================*/

/*=========================================================================*\
* BSD include files
\*=========================================================================*/
/* error codes */
#include <errno.h>
/* close function */
#include <unistd.h>
/* fnctnl function and associated constants */
#include <fcntl.h>
/* struct sockaddr */
#include <sys/types.h>
/* socket function */
#include <sys/socket.h>
/* struct timeval */
#include <sys/time.h>
/* gethostbyname and gethostbyaddr functions */
#include <netdb.h>
/* sigpipe handling */
//#include <signal.h>
#ifdef SN_TARGET_PS3
#include <sys/select.h>
#endif

/* IP stuff*/
#include <netinet/in.h>
#include <arpa/inet.h>
/* TCP options (nagle algorithm disable) */
#include <netinet/tcp.h>

typedef int t_socket;
typedef t_socket *p_socket;

#define SOCKET_INVALID (-1)

#endif /* USOCKETTYPES_H */