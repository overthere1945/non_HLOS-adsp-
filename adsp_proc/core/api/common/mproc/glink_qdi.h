#ifndef GLINK_QDI_H
#define GLINK_QDI_H

/**
 * @file glink_qdi.h
 *
 * Public API for the GLink
 */

/** \defgroup glink GLink
 * \ingroup SMD
 *
 * GLink reliable, in-order, datagram-based interprocessor communication
 * over a set of supported transport (Shared Memory, UART, BAM, HSIC)
 *
 * All ports preserve message boundaries across the interprocessor channel; one
 * write into the port exactly matches one read from the port.
 */
/*@{*/

/*==============================================================================
  Copyright (c) QUALCOMM Technologies Incorporated.
  All rights reserved.
  Qualcomm Confidential and Proprietary
==============================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
                        INCLUDE FILES
===========================================================================*/
#include "com_dtypes.h"
#include "stddef.h"
#include <string.h>

/*===========================================================================
                      MACRO DECLARATIONS
===========================================================================*/
/** GLink status/return codes */
typedef enum {
  GLINK_STATUS_SUCCESS                 = 0,
  GLINK_STATUS_INVALID_PARAM             = -1,
  GLINK_STATUS_NOT_INIT                  = -2,
  GLINK_STATUS_OUT_OF_RESOURCES          = -3,
  /* Failure due to lack of transport */
  GLINK_STATUS_NO_TRANSPORT              = -4 ,
  /* TX failure when there is no remote intent queued */
  GLINK_STATUS_NO_REMOTE_INTENT_FOUND    = -5,
  /* Failure of a glink_* call when channel is not fully opened yet */
  GLINK_STATUS_CH_NOT_FULLY_OPENED       = -6,
  /* Failure due to closing the same channel twice */
  GLINK_STATUS_CH_ALREADY_CLOSED         = -7,
  /* Returned when intent APIs are used over an intent less xport */
  GLINK_STATUS_API_NOT_SUPPORTED         = -8,
  /* Failure specific to QoS algorithm/implementation */
  GLINK_STATUS_QOS_FAILURE               = -9,
  /* Failure due to tx_cmd* calls */
  GLINK_STATUS_TX_CMD_FAILURE            = -10,
  /* For other Failures not covered above */
  GLINK_STATUS_FAILURE                   = -11,
  /* Failures relating to GLink operation timeout */
  GLINK_STATUS_TIMEOUT                   = -12,
  /* Failure to transmit to a specific channel */
  GLINK_STATUS_CH_TX_BUSY                = -13,
  /* OS API returned with some error */
  GLINK_STATUS_OS_ERROR                  = -14
}glink_err_type;

/*===========================================================================
                      TYPE DECLARATIONS
===========================================================================*/
/** Enums to identify link state */
typedef enum {
 GLINK_LINK_STATE_UP,  /* Called after specified link is online */
 GLINK_LINK_STATE_DOWN /* Called after SSR (before REMOTE_DISCONNECT) */
} glink_link_state_type;

/** Data structure holding the link state information */
typedef struct          glink_link_info_t {
  const char*           xport;      /* Transport name */
  const char*           remote_ss;  /* Remote subsystem name */
  glink_link_state_type link_state; /* Link state */
}glink_link_info_type;

/* Callback function invoked when link goes up/down */
typedef void (*glink_link_state_notif_cb)
(
  glink_link_info_type *link_info, /* Ptr to link information strcuture */
  void* priv                       /* Client private data */
);

/** 
 * Opaque handle returned by glink_register_link_state_cb. Client uses this
 * handle to call glink_deregister_link_state_cb() to deregister their
 * callback
 *
 * Client must not modify or try to interpret this value 
 */
typedef struct glink_link_notif_data_type * glink_link_handle_type;

/** Data structure to provide link information for the link to monitor */
typedef struct glink_link_id_t {
  unsigned int              version; /* This fields identifies the verion of 
                                        the structure. Initialized by
                                        GLINK_LINK_ID_STRUCT_INIT macro */

  const char*               xport;         /* NULL = any transport */
  const char*               remote_ss;     /* NULL = any subsystem */
  glink_link_state_notif_cb link_notifier; /* Notification callback */
  glink_link_handle_type    handle;  /* Set by glink_register_link_state_cb */

  /** bitfield for specifying various options */
  unsigned int              options; 
}glink_link_id_type;

/** 
 * Opaque handle returned by glink_open. Client uses this handle to call into
 * GLink API for any further activity related to the channel,
 *
 * Client must to modify or try to interpret this value 
 */
typedef struct glink_channel_ctx* glink_handle_type;

/** GLink logical link state notifications */
typedef enum {
  /** Channel is fully opened. Both local and remote end have
      opened channel from their respective ends, and data communication
      can now take place */
  GLINK_CONNECTED = 0,

  /** Local side has called glink_close() and remote side has acknowledged
      this end's close. Client may call glink_open() after this point */
  GLINK_LOCAL_DISCONNECTED,

  /** Remote side has called glink_close() to close the channel */
  GLINK_REMOTE_DISCONNECTED
}glink_channel_event_type;

/** Vector buffer provider type*/  
typedef void* (*glink_buffer_provider_fn)
(
  void*  iovec,   /* vector buffer */
  size_t offset,  /* offset from the beginning of the vector */
  size_t *size    /* size of the returned contiguous buffer */
);

/** Data receive notification callback type*/  
typedef void (*glink_rx_notification_cb)
(
  glink_handle_type handle,     /* handle for the glink channel */
  const void        *priv,      /* priv client data passed in glink_open */
  const void        *pkt_priv,  /* private client data assiciated with the
                                   rx intent that client queued earlier */
  const void        *ptr,       /* pointer to the received buffer */
  size_t            size,       /* size of the packet */
  size_t            intent_used /* size of the intent used for this packet */
);

/** Data receive notification callback type*/  
typedef void (*glink_rx_tracer_pkt_notification_cb)
(
  glink_handle_type handle,     /* handle for the glink channel */
  const void        *priv,      /* priv client data passed in glink_open */
  const void        *pkt_priv,  /* private client data assiciated with the
                                   rx intent that client queued earlier */
  const void        *ptr,       /* pointer to the received buffer */
  size_t            size       /* size of the packet */
);

/** Vector receive notification callback type*/  
typedef void (*glink_rxv_notification_cb)
(
  glink_handle_type        handle,    /* handle for the glink channel */
  const void               *priv,     /* priv client data passed in glink_open */
  const void               *pkt_priv, /* private client data assiciated with the
                                         rx intent that client queued earlier */
  void                     *iovec,    /* pointer to the received vector */
  size_t                   size,      /* size of the packet */
  size_t                   intent_used, /* size of the intent used for this packet */
  glink_buffer_provider_fn vprovider, /* Buffer provider for virtual space */
  glink_buffer_provider_fn pprovider  /* Buffer provider for physical space */
);

/** Data transmit notification callback type*/  
typedef void (*glink_tx_notification_cb)
(
  glink_handle_type handle,    /* handle for the glink channel */
  const void        *priv,     /* priv client data passed in glink_open */
  const void        *pkt_priv, /* private client data assiciated with the
                                  tx pkt that client queued earlier */
  const void        *ptr,      /* pointer to the transmitted buffer */
  size_t            size       /* size of the packet */
);

/** GLink channel state change notification callback type*/  
typedef void (*glink_state_notification_cb)
(
  glink_handle_type         handle, /* handle for the glink channel */
  const void                *priv,  /* priv client data passed in glink_open */
  glink_channel_event_type  event   /* Notification event */
);

/** Request for a receive intent of size req_size is queued. The request
 * originates from the remote side GLink client. Return true if intent will 
 * be queued or false if intent request will be denied. */  
typedef boolean (*glink_notify_rx_intent_req_cb)
(
  glink_handle_type         handle,   /* handle for the glink channel */
  const void                *priv,    /* priv client data passed in glink_open */
  size_t                    req_size  /* Requested size */
);

/** New intent arrival notification callback type*/  
typedef void (*glink_notify_rx_intent_cb)
(
  glink_handle_type         handle,   /* handle for the glink channel */
  const void                *priv,    /* priv client data passed in glink_open */
  size_t                    size      /* Intent size */
);

/** Control signal change notification - Invoked when remote side
 *  alters its control signals */
typedef void (*glink_notify_rx_sigs_cb)
(
  glink_handle_type  handle,   /* handle for the glink channel */
  const void         *priv,    /* priv client data passed in glink_open  */
  uint32             prev,     /* Previous remote state */
  uint32             curr      /* Current remote state */
);

/** rx_intent abort notification. This callback would be invoked for
 *  every rx_intent that is queued with GLink core at the time the
 *  local side decides to close the port */
typedef void(*glink_notify_rx_abort_cb)
(
glink_handle_type  handle,   /* handle for the glink channel */
const void         *priv,    /* priv client data passed in glink_open  */
const void         *pkt_priv /* pkt specific private data */
);

/** tx abort notification. This callback would be invoked if client 
 *   had queued a tx buffer with glink and it had not been transmitted i.e.
 *   tx_done callback has not been called for this buffer and remote side
 *   or local side closed the port*/
typedef void(*glink_notify_tx_abort_cb)
(
glink_handle_type  handle,   /* handle for the glink channel */
const void         *priv,    /* priv client data passed in glink_open  */
const void         *pkt_priv /* pkt specific private data */
);

/**  Intent buffer allocation notification (Optional, restricted). 
 *   This callback would be invoked by GLink if it has to allocate a buffer
 *   to receive a packet for a particular intent (Based on platform support).
 *   This allows the client to decide the memory storage for incoming packets.
 *   When the intent is destroyed, GLink will also call the deallocate callback.
 *   Client must ensure the validity of the output buffer till the deallocate
 *   callback is called.
 */
typedef glink_err_type (*glink_notify_allocate_cb)
(
glink_handle_type  handle,   /* handle for the glink channel */
const void         *priv,    /* priv client data passed in glink_open  */
const void         *pkt_priv, /* pkt specific private data */
size_t             intent_size, /* Intent size */
void               **buffer_ptr /* Output buffer pointer */
);

/**  Intent buffer deallocation notification (Optional, restricted). 
 *   This callback would be invoked by GLink if it has to deallocate a buffer
 *   previously allocated using the glink_notify_allocate_cb callback. Note
 *   that it is mandatory to provide both allocate and deallocate callbacks */
typedef void (*glink_notify_deallocate_cb)
(
glink_handle_type  handle,   /* handle for the glink channel */
const void         *priv,    /* priv client data passed in glink_open  */
const void         *pkt_priv, /* pkt specific private data */
size_t             intent_size, /* Intent size */
void               *buffer_ptr
);

/**
 * Data Structure for GLink logical channel open configuration
 *
 * This structure is used by the clients to open a GLink logical channel
 * when calling glink_open()
 *
 * Clients must ensure to zero this structure before setting the members
 * to ensure forwards binary compatibility.
 */
typedef struct {
  /** string name for the transport to use (Optional)*/
  const char                          *transport;

  /** string name for the remote subsystem to which the user wants to 
      connect */
  const char                          *remote_ss;

  /** string name for the channel */
  const char                          *name;

  /** bitfield for specifying various options */
  unsigned                            options; 

  /** Private data for client to maintain context. This data is passed back
      to client in the notification callbacks */
  const void                          *priv;

  /** Data receive notification callback. Optional if notify_rxv is provided */
  glink_rx_notification_cb            notify_rx;

  /** Tracer packet receive notification callback. 
   *  Optional if user doesn't need to use this */
  glink_rx_tracer_pkt_notification_cb notify_rx_tracer_pkt;
  
  /** Vector receive notification callback. Optional if notify_rx is provided */
  glink_rxv_notification_cb           notify_rxv;

  /** Data transmit notification callback */
  glink_tx_notification_cb            notify_tx_done;

  /** GLink channel state notification callback */
  glink_state_notification_cb         notify_state;

  /** Intent request from the remote side. Optional */
  glink_notify_rx_intent_req_cb       notify_rx_intent_req;

  /** New intent arrival from the remote side */
  glink_notify_rx_intent_cb           notify_rx_intent;

  /** Control signal change notification - Invoked when remote side
   *  alters its control signals. Optional */  
  glink_notify_rx_sigs_cb             notify_rx_sigs;

  /** rx_intent abort notification. This callback would be invoked for
   *  every rx_intent that is queued with GLink core at the time the
   *  local side decides to close the port. Optional */
  glink_notify_rx_abort_cb            notify_rx_abort;

  /** tx abort notification. This callback would be invoked if client
  *   had queued a tx buffer with glink and it had not been transmitted i.e.
  *   tx_done callback has not been called for this buffer and remote side
  *   or local side closed the port. Optional */
  glink_notify_tx_abort_cb            notify_tx_abort;

  /** timeout (in ms) for remote intent wait, zero for infinite wait */
  unsigned                            rm_intent_timeout;

  /* Allows the client control on intent specific buffer allocation.
   * See documentation for glink_notify_allocate_cb. 
   * Client must ensure that the bit GLINK_OPT_CLIENT_BUFFER_ALLOCATION
   * is set in options else GLink will ignore this field */
  glink_notify_allocate_cb            notify_allocate;

  /* Allows the client control on intent specific buffer allocation.
   * See documentation for glink_notify_deallocate_cb. 
   * Client must ensure that the bit GLINK_OPT_CLIENT_BUFFER_ALLOCATION
   * is set in options else GLink will ignore this field */
  glink_notify_deallocate_cb          notify_deallocate;

}glink_open_config_type;

#ifdef __cplusplus
}
#endif

#endif /* GLINK_QDI_H */