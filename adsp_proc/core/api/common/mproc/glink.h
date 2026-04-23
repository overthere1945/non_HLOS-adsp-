#ifndef GLINK_H
#define GLINK_H

/**
 * @file glink.h
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
#include "glink_qdi.h"

/** List of possible subsystems */
/**
  "apss"   Application Processor Subsystem
  "mpss"   Modem subsystem
  "lpass"  Low Power Audio Subsystem
  "dsps"   Sensors Processor
  "wcnss"  Wireless Connectivity Subsystem
  "rpm"    Resource Power Manager processor
*/

/** Max allowed channel name length */
#define GLINK_CH_NAME_LEN 32

/* Bit position of DTR/CTS/CD/RI bits in control sigs 32 bit signal map */
#define SMD_DTR_SIG_SHFT 31
#define SMD_CTS_SIG_SHFT 30
#define SMD_CD_SIG_SHFT  29
#define SMD_RI_SIG_SHFT  28

/* set bits between the mentioned positions */
#define GLINK_MASK_PREP(_from_pos, _to_pos) \
  (((uint32)(1 << ((_to_pos) - (_from_pos))) - 1) << (_from_pos))

/* set bits of a 32b word */
#define GLINK_SET_BITS(_word_32, _from_pos, _to_pos, _mask) \
  ((_word_32) | (GLINK_MASK_PREP(_from_pos, _to_pos) & ((_mask) << (_from_pos))))

/* set value of a 32b word */
#define GLINK_SET_VAL(_word_32, _from_pos, _to_pos, _val) \
  (((_word_32) & (~GLINK_MASK_PREP(_from_pos, _to_pos))) | ((_val) << (_from_pos)))

/* hub type bit positions */
#define GLINK_HUB_POS_START (8)
#define GLINK_HUB_POS_END   (16)

/* Type of hub to be used */
/* default hub works on DDR */
#define GLINK_HUB_TYPE_DEFAULT       (0)

/* micro hub works on top of TCM */
#define GLINK_HUB_TYPE_MICRO         (1)

/** Version number for the glink_link_id_type structure */
#define GLINK_LINK_ID_VER  0x00000001

/** Macro to initialize the link identifier structure with default values.
 * It memsets the header to 0 and initializes the header field */
#define GLINK_LINK_ID_STRUCT_INIT(link_id)                           \
                          (link_id).xport = 0;                       \
                          (link_id).remote_ss = 0;                   \
                          (link_id).link_notifier = 0;               \
                          (link_id).handle = 0;                      \
                          (link_id).version = GLINK_LINK_ID_VER;     \
                          (link_id).options = GLINK_HUB_TYPE_DEFAULT;

/** Macro to set right link id options for hub info
 * should be invoked after `GLINK_LINK_ID_STRUCT_INIT`
 * any one from GLINK_HUB_TYPE_* */
#define GLINK_LINK_ID_SET_HUB(link_id, hub_type) (link_id).options =       \
  GLINK_SET_VAL((link_id).options, GLINK_HUB_POS_START, GLINK_HUB_POS_END, \
                (hub_type))

/* GLink tx options */
/* Flag for no options */
#define GLINK_TX_NO_OPTIONS      ( 0 )

/* Whether to block and request for remote rx intent in
 * case it is not available for this pkt tx */
#define GLINK_TX_REQ_INTENT      0x00000001

/* If the tx call is being made from single threaded context. GLink tries to
 * flush data into the transport in glink_tx() context, or returns error if
 * it is not able to do so */
#define GLINK_TX_SINGLE_THREADED 0x00000002

/* This option is to turn on tracer packet */
#define GLINK_TX_TRACER_PKT      0x00000004

/* ======================= glink open cfg options ==================*/

/* Specified transport is just the initial transport and migration is possible
 * to higher-priority transports.  Without this flag, the open will fail if
 * the transport does not exist. */
#define GLINK_OPT_INITIAL_XPORT            0x00000001

/* Specified if the client wishes to provide their own allocation/deallocation
 * methods. See glink_notify_allocate_cb and glink_notify_deallocate_cb for 
 * more info. */
#define GLINK_OPT_CLIENT_BUFFER_ALLOCATION 0x00000002

/* Clients must call this macro to initialize the open config structure
 * before setting its individual members. This will ensure that the 
 * default optional values in the structure are appropriately initialized. */
#define GLINK_OPEN_CONFIG_INIT(cfg) \
  memset(&(cfg), 0, sizeof(cfg))

/** Macro to set right open options for hub info
 * should be invoked after `GLINK_OPEN_CONFIG_INIT`
 * any one from GLINK_HUB_TYPE_* */
#define GLINK_OPEN_CONFIG_SET_HUB(cfg, hub_type) (cfg).options =       \
  GLINK_SET_VAL((cfg).options, GLINK_HUB_POS_START, GLINK_HUB_POS_END, \
                (hub_type))

/* Identifiers for the instance of a given processor on a given chiplet.
For single instance processors GLINK_PROC_NUM_DEFAULT should be used. */
#define GLINK_PROC_NUM_0            0
#define GLINK_PROC_NUM_1            1
#define GLINK_PROC_NUM_2            2
#define GLINK_PROC_NUM_3            3
#define GLINK_PROC_NUM_DEFAULT      GLINK_PROC_NUM_0

/* Identifiers for a chiplet. For single instance processors
GLINK_CHIPLET_ID_DEFAULT should be used. */
#define GLINK_CHIPLET_ID_0          0
#define GLINK_CHIPLET_ID_DEFAULT    GLINK_CHIPLET_ID_0

/* Identifiers for PD within a processor instance. GLINK_PD_DEFAULT should be
used in most of the cases unless there are explicit connections to PDs. */
#define GLINK_PD_ROOT               0
#define GLINK_PD_DEFAULT            GLINK_PD_ROOT

/* Get the remote ss name based on proc name, proc number, pd number and
chiplet number */
#define GLINK_GET_REMOTE_SS_NAME(proc_name, proc_num, pd_num, chiplet_num) \
  glink_get_remote_ss_name(GLINK_HUB_TYPE_DEFAULT, proc_name, proc_num,    \
                           pd_num, chiplet_num)

/*===========================================================================
                      GLINK PUBLIC API
===========================================================================*/
/*===========================================================================
  FUNCTION      glink_get_remote_ss_name
===========================================================================*/
/** 
 *  Get the remote ss name based on proc name, number and chiplet number
 * 
 * @param[in]    hub_type    Type of link layer hub to be used
 * @param[in]    proc_name   Name of the remote proc
 * @param[in]    proc_num    Number of the remote proc
 * @param[in]    pd_num      Number of the PD
 * @param[in]    proc_name   Chiplet number of the remote proc
 *
 * @return       remote_ss string.
 *
 * @sideeffects  None.
 */
/*=========================================================================*/
const char *glink_get_remote_ss_name
(
  uint32     hub_type,
  const char *proc_name,
  int        proc_num,
  int        pd_num,
  int        chiplet_num
);

/** 
 * Regsiters a client specified callback to be invoked when the specified
 * transport (link) is up/down.
 *
 * @param[in]    link_id  Pointer to the configuration structure for the
 *                        xport(link) to be monitored. See glink.h
 * @param[in]    priv     Callback data returned to client when callback
 *                        is invoked.
 *
 * @return       Standard GLink error codes
 *
 * @sideeffects  Puts the callback in a queue which gets scanned when a 
 *               transport(link) comes up OR an SSR happnes.
 */
glink_err_type glink_register_link_state_cb
(
  glink_link_id_type *link_id,
  void* priv
);

/** 
 * Degsiter the link UP/DOWN notification callback associated with the
 * provided handle.
 *
 * @param[in]    handle  Callback handler returned by 
 *                       glink_register_link_state_cb
 *
 * @return       Standard GLink error codes
 *
 * @sideeffects  Removes the callback in a queue which gets scanned when a 
 *               transport(link) comes up OR an SSR happnes.
 */
glink_err_type glink_deregister_link_state_cb
(
  glink_link_handle_type handle
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
glink_err_type glink_open
(
  glink_open_config_type *cfg_ptr,
  glink_handle_type      *handle
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
glink_err_type glink_close
(
  glink_handle_type handle
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
glink_err_type glink_tx
(
  glink_handle_type handle,
  const void        *pkt_priv,
  const void        *data,
  size_t            size,
  uint32            options
);

/** 
 * Transmit the provided vector buffer over GLink.
 *
 * @param[in]    handle    GLink handle associated with the logical channel
 *
 * @param[in]   *pkt_priv  Per packet private data
 *
 * @param[in]   *iovec     Pointer to the vector buffer to be transmitted
 *
 * @param[in]   size       Size of buffer
 *
 * @param[in]   vprovider  Buffer provider for virtual space
 *
 * @param[in]   pprovider  Buffer provider for physical space
 *
 * @param[in]   options    Flags specifying how transmission for this buffer 
 *                         would be handled. See GLINK_TX_* flag definitions.
 *
 * @return       Standard GLink error codes
 *
 * @sideeffects  Causes remote host to wake-up and process rx pkt
 */
glink_err_type glink_txv
(
  glink_handle_type        handle,
  const void               *pkt_priv,
  void                     *iovec,
  size_t                   size,
  glink_buffer_provider_fn vprovider,
  glink_buffer_provider_fn pprovider,
  uint32                   options
);

/** 
 * Queue an Rx intent for the logical GLink channel.
 *
 * @param[in]    handle   GLink handle associated with the logical channel
 *
 * @param[in]   *pkt_priv Per packet private data
 *
 * @param[in]   size      Size of buffer
 *
 * @return       Standard GLink error codes
 *
 * @sideeffects  GLink XAL allocates rx buffers for receiving packets
 */
glink_err_type glink_queue_rx_intent
(
  glink_handle_type handle,
  const void        *pkt_priv,
  size_t            size
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
glink_err_type glink_rx_done
(
  glink_handle_type handle,
  const void        *ptr,
  boolean           reuse
);

/** 
 * Set the 32 bit control signal field. Depending on the transport, it may
 * take appropriate actions on the set bit-mask, or transmit the entire 
 * 32-bit value to the remote host.
 *
 * @param[in]   handle     GLink handle associated with the logical channel
 *
 * @param[in]   sig_value  32 bit signal word
 *
 * @return       Standard GLink error codes
 *
 * @sideeffects  None
 */
glink_err_type glink_sigs_set
(
  glink_handle_type handle,
  uint32            sig_value
);

/** 
 * Get the local 32 bit control signal bit-field.
 *
 * @param[in]   handle      GLink handle associated with the logical channel
 *
 * @param[out]  *sig_value  Pointer to a 32 bit signal word to get sig value
 *
 * @return      Standard GLink error codes
 *
 * @sideeffects  None
 */
glink_err_type glink_sigs_local_get
(
  glink_handle_type handle,
  uint32            *sig_value
);

/** 
 * Get the remote 32 bit control signal bit-field.
 *
 * @param[in]   handle      GLink handle associated with the logical channel
 *
 * @param[out]  *sig_value  Pointer to a 32 bit signal word to get sig value
 *
 * @return      Standard GLink error codes
 *
 * @sideeffects  None
 */
glink_err_type glink_sigs_remote_get
(
  glink_handle_type handle,
  uint32            *sig_value
);

#ifdef __cplusplus
}
#endif

#endif /* GLINK_H */
