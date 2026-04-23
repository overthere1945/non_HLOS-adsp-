#ifndef UGLINK_H
#define UGLINK_H

/**
 * @file uglink.h
 *
 * Public API for the uGLink
 */

/** \defgroup glink uGLink
 * 
 * Glink for uImage
 */
/*@{*/

/*==============================================================================
     Copyright (c) 2014-2020 QUALCOMM Technologies Incorporated.
     All rights reserved.
     Qualcomm Confidential and Proprietary
==============================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

$Header: //components/rel/core.qdsp6/8.2.3/api/common/mproc/uglink.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/07/20   bm      uGlink
===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
                        INCLUDE FILES
===========================================================================*/
#include "glink.h"

#define GLINK_MAX_HOST_NAME 10

typedef void (*glink_link_notify_cb_type)(glink_link_info_type *, void*);

typedef struct 
{
  char remote_ss[GLINK_MAX_HOST_NAME];
  glink_link_notify_cb_type cb; 
  void* priv;
  uint8 cb_called;
}glink_link_notify_type;

glink_err_type uglink_register_notify_clients
(
 glink_link_notify_type* glink_notify,
 void* dumb_data 
);

void uglink_notify_clients
(
 void* ctx_ptr,
 glink_link_state_type link_state
);

/** 
 * Client uses this to signal to GLink layer that it is done with the received 
 * data buffer. This API should be called to free up the receive buffer, which,
 * in zero-copy mode is actually remote-side's transmit buffer.
 *
 * @param[in]   handle   GLink handle associated with the logical channel
 *
 * @param[in]   *ptr     Pointer to the received buffer
 *
 * @param[in]   reuse    Reuse intent
 *
 * @return       Standard GLink error codes
 *
 * @sideeffects  GLink XAL frees the Rx buffer
 */
glink_err_type uglink_rx_done
(
  glink_handle_type handle,
  const void        *ptr,
  boolean           reuse
);

/** 
 * Transmit the provided buffer over GLink.
 *
 * @param[in]    handle    GLink handle associated with the logical channel
 *
 * @param[in]   *pkt_priv  Per packet private data
 *
 * @param[in]   *data      Pointer to the data buffer to be transmitted
 *
 * @param[in]   size       Size of buffer
 *
 * @param[in]   options    Flags specifying how transmission for this buffer 
 *                         would be handled. See GLINK_TX_* flag definitions.
 *
 * @return       Standard GLink error codes
 *
 * @sideeffects  Causes remote host to wake-up and process rx pkt
 */
glink_err_type uglink_tx
(
  glink_handle_type handle,
  const void        *pkt_priv,
  const void        *data,
  size_t            size,
  uint32            options
);

/** 
 * Closes the GLink logical channel specified by the handle.
 *
 * @param[in]    handle   GLink handle associated with the logical channel
 *
 * @return       Standard GLink error codes
 *
 * @sideeffects  Closes local end of the channel and informs remote host
 */
glink_err_type uglink_close
(
  glink_handle_type handle
);

/** 
 * Opens a logical GLink based on the specified config params
 *
 * @param[in]    cfg_ptr  Pointer to the configuration structure for the
 *                        GLink. See glink.h
 * @param[out]   handle   GLink handle associated with the logical channel
 *
 * @return       Standard GLink error codes
 *
 * @sideeffects  Allocates channel resources and informs remote host about
 *               channel open.
 */
glink_err_type uglink_open
(
  glink_open_config_type *cfg_ptr,
  glink_handle_type      *handle
);

/** 
 * Inits glink
 *
 * @param[in]    None 
 *  
 * @param[out]   None
 *
 * @return       None
 *
 * @sideeffects  Inits uimage link 
 *  
 */
void uglink_init
(
  void
);

#ifdef __cplusplus
}
#endif

#endif //UGLINK_H
