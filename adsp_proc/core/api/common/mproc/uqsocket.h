#ifndef _UQSOCKET_H_
#define _UQSOCKET_H_
/******************************************************************************
  @file    qsocket.h
  @brief   Generic Socket-like interface

  DESCRIPTION

  Provides a generic socket like interface currently used for IPC Router.
  If this becomes a generic interface the documentation must change 
  appropriately

  Certain Operating systems might need special setup. Please refer
  to qsocket_<os>.h if it exists
  
  ---------------------------------------------------------------------------
  Copyright (c) 2014 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
  ---------------------------------------------------------------------------

 *******************************************************************************/
#include "stdlib.h"
#include "stdint.h"
#include "qsocket.h"

#ifdef __cplusplus
extern "C" {
#endif


/*============================================================================
                        FUNCTION PROTOTYPES
============================================================================*/

/*===========================================================================
  FUNCTION  qsocket
===========================================================================*/
/*!
@brief

  This function returns a socket descriptor

@param[in]   domain    The address family of the socket (Currently only
                       AF_IPC_ROUTER is supported)
@param[in]   type      Type of socket. See SOCKET TYPES for more info
@param[in]   protocol  Family specific protocol (Currently unused)

@return
  Positive socket descriptor upon success, negative error code on failure.

@note

  - Dependencies
    - None

  - Side Effects
    - None
*/
/*=========================================================================*/
int uqsocket(int domain, int type, int protocol);

/*===========================================================================
  FUNCTION  uqconnect
===========================================================================*/
/*!
@brief

  Connect the socket to the specified destination endpoint. Considering 
  IPC Router is a connection-less protocol, this does not result in
  an actual connection, but just associates the socket to the remote
  endpoint. Thus, if the remote endpoint terminates, the sender will
  detect a failure.

@param[in]   fd        Socket descriptor
@param[in]   addr      Address of the destination endpoint
@param[in]   addrlen   Length of the address

@return
  0 on success, negative error code on failure.

@note

  - Dependencies
    - None

  - Side Effects
    - None
*/
/*=========================================================================*/
int uqconnect(int fd, struct qsockaddr *addr, qsocklen_t addrlen);

/*===========================================================================
  FUNCTION  uqbind
===========================================================================*/
/*!
@brief

  Binds a socket to a specific name.
  Since all ports in IPC Router are ephemeral ports, it is not allowed
  to bind to a specified physical port ID (Like UDP/IP).

@param[in]   fd        Socket descriptor
@param[in]   addr      Name to bind with the socket
@param[in]   addrlen   Length of the address

@return
  0 on success, negative error code on failure.

@note

  - Dependencies
    - Note that the 'type' field of the NAME _must_ be allocated and reserved
      by the Qualcomm Interface Control board else it is highly likely that
      the users might be in conflict with names allocated to other users.

  - Side Effects
    - None
*/
/*=========================================================================*/
int uqbind(int fd, struct qsockaddr *addr, qsocklen_t addrlen);

/*===========================================================================
  FUNCTION  uqsendto
===========================================================================*/
/*!
@brief

  Sends a message to a specific remote address

@param[in]   fd        Socket descriptor
@param[in]   buf       Pointer to the buffer
@param[in]   len       Length of the buffer
@param[in]   flags     Flags - See SOCKET SEND/RECV FLAGS for more info
@param[in]   addr      Address of the destination
@param[in]   addrlen   Length of the address

@return
  Transmitted length on success, negative error code on failure.
  If the destination is flow controlled, and QMSG_DONTWAIT is
  set in flags, then the function can return QEAGAIN

  If the address is of type IPCR_ADDR_NAME, the message is multicast
  to all ports which has bound to the specified name.

@note
  - Dependencies
    - None

  - Retry logic (After receiving an error of QEAGAIN)
    - If QMSG_DONTWAIT is set in the flags, the user must retry
      after blocking and receiving the:
      * QPOLLOUT event (In the case of a connected socket)'
      * QPOLLIN event in the case of a unconnected socket and
        uqrecvfrom() returns a zero length message with the unblocked
        destination in the output address.

  - Side Effects
    - Function can block.
*/
/*=========================================================================*/
int uqsendto(int fd, void *buf, size_t len, int flags, 
             struct qsockaddr *addr, qsocklen_t addrlen);

/*===========================================================================
  FUNCTION  uqsend
===========================================================================*/
/*!
@brief

  Sends a message to the connected address

@param[in]   fd        Socket descriptor
@param[in]   buf       Pointer to the buffer
@param[in]   len       Length of the buffer
@param[in]   flags     Flags - See SOCKET SEND/RECV FLAGS for more info

@return
  Transmitted length on success, negative error code on failure.
  If the destination is flow controlled, and QMSG_DONTWAIT is
  set in flags, then the function can return QEAGAIN

  If the address is of type IPCR_ADDR_NAME, the message is multicast
  to all ports which has bound to the specified name.

@note
  - Dependencies
    - None

  - Retry logic (After receiving an error of QEAGAIN)
    - If QMSG_DONTWAIT is set in the flags, the user must retry
      after blocking and receiving the:
      * QPOLLOUT event (In the case of a connected socket)'
      * QPOLLIN event in the case of a unconnected socket and
        uqrecvfrom() returns a zero length message with the unblocked
        destination in the output address.

  - Dependencies
    - uqconnect() must have been called to associate this socket
      with a destination address

  - When the connected endpoint terminates (normally or abnormally), the
    function would unblock and return QENOTCONN/QEDESTADDRREQ.

  - Side Effects
    - Function can block.
*/
/*=========================================================================*/
int uqsend(int fd, void *buf, size_t len, int flags);

/*===========================================================================
  FUNCTION  uqrecvfrom
===========================================================================*/
/*!
@brief

  Receives a message from a remote address

@param[in]   fd        Socket descriptor
@param[in]   buf       Pointer to the buffer
@param[in]   len       Length of the buffer
@param[in]   flags     Flags - See SOCKET SEND/RECV FLAGS for more info
@param[out]  addr      Address of the sender
@param[inout]addrlen   Input: Size of the passed in buffer for address
                       Output: Size of the address filled by the framework.
                       (Can be NULL if addr is also NULL).

@return
  Received packet size in bytes, negative error code in failure.

@note

  - Dependencies
    - None

  - Retry logic
    - If QMSG_DONTWAIT is set in the flags, the user must retry
      after blocking and receiving the QPOLLIN event using qpoll()

  - Side Effects
    - None
*/
/*=========================================================================*/
int uqrecvfrom(int fd, void *buf, size_t len, int flags, 
                struct qsockaddr *addr, qsocklen_t *addrlen);

/*===========================================================================
  FUNCTION  uqrecv
===========================================================================*/
/*!
@brief

  Receives a message from the connected address address

@param[in]   fd        Socket descriptor
@param[in]   buf       Pointer to the buffer
@param[in]   len       Length of the buffer
@param[in]   flags     Flags - See SOCKET SEND/RECV FLAGS for more info

@return
  Received packet size in bytes, negative error code in failure.

@note

  - Dependencies
    - uqconnect() must have been called to associate this socket
      with a destination address

  - Retry logic
    - If QMSG_DONTWAIT is set in the flags, the user must retry
      after blocking and receiving the QPOLLIN event using qpoll()

  - When the connected endpoint terminates (normally or abnormally), the
    function would unblock and return QENOTCONN/QEDESTADDRREQ.

  - Side Effects
    - None
*/
/*=========================================================================*/
int uqrecv(int fd, void *buf, size_t len, int flags);

/*===========================================================================
  FUNCTION  uqgetsockopt
===========================================================================*/
/*!
@brief

  Gets an option value

@param[in]   fd        Socket descriptor
@param[in]   level     Level of the option. Currently the only supported
                       level is QSOL_IPC_ROUTER
@param[in]   optname   Option name to get
@param[in]   optval    Buffer to place the option
@param[inout]optlen    In: Length of the buffer passed into uqgetsockopt
                       Out: Length of the filled in options

@return
  0 on success, negative error code on failure.

@note
  - Side Effects
    - None
*/
/*=========================================================================*/
int uqgetsockopt(int fd, int level, int optname, void *optval, qsocklen_t *optlen);

/*===========================================================================
  FUNCTION  uqsetsockopt
===========================================================================*/
/*!
@brief

  Sets an option on a socket

@param[in]   fd        Socket descriptor
@param[in]   level     Level of the option. Currently the only supported
                       level is QSOL_IPC_ROUTER
@param[in]   optname   Option name to get
@param[in]   optval    Buffer to place the option
@param[in]   optlen    Length of the buffer passed into uqsetsockopt

@return
  0 on success, negative error code on failure.

@note
  - Side Effects
    - None
*/
/*=========================================================================*/
int uqsetsockopt(int fd, int level, int optname, void *optval, qsocklen_t optlen);

/*===========================================================================
  FUNCTION  uqpoll
===========================================================================*/
/*!
@brief

  Blocks on requested events on the provided socket descriptors

@param[in]   pfd       Array of poll info (See uqpollfd for more info)
@param[in]   num       Number of sockets to wait on (Len of pfd array)
@param[in]   timeout_ms Timeout in milli-seconds:
                        -1 = Infinite
                        0  = poll, return immediately if there are no events
                        val > 0, timeout

@return
  Total number of socket descriptors which have events on them on success
  0 if there were no events, and the function unblocked after the timeout.
  Negative error code on failure.

@note

  - Dependencies
    - None

  - Side Effects
    - Blocks waiting for events
*/
/*=========================================================================*/
int uqpoll(struct qpollfd *pfd, qnfds_t num, int timeout_ms);

/*===========================================================================
  FUNCTION  uqclose
===========================================================================*/
/*!
@brief

  Closes the socket

@param[in]   fd        Socket descriptor

@return
  0 on success, negative error code on failure

@note

  - Dependencies
    - None

  - Side Effects
    - None
*/
/*=========================================================================*/
int uqclose(int fd);

/*===========================================================================
  FUNCTION  qshutdown
===========================================================================*/
/*!
@brief

  Shuts down a socket

@param[in]   fd        Socket descriptor
@param[in]   how       QSHUTRD (or 0) - Stop receiving packets
                       QSHUTWR (or 1) - Stop transmitting packets
                       QSHUTRDWR (or 2) - Stop both receiving and transmitting

@return
  0 on success  negative error code on failure.

@note

  - Dependencies
    - None

  - Side Effects
    - shutting down both RD and WR will have the same effect as qclose()
*/
/*=========================================================================*/
int uqshutdown(int fd, int how);


#ifdef __cplusplus
}
#endif

#endif
