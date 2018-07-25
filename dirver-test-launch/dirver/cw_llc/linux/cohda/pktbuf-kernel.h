/**
 * @addtogroup cohda_pktbuf_kernel Cohda's kernel pkacket buffer library
 * @{
 *
 * Don't #include this file directly, use @ref pktbuf.h instead
 *
 * @file
 * Collection of kernel space wrappers for sk_buffs
 *
 */
#ifndef __INCLUDE__LINUX__COHDA__PKTBUF_KERNEL_H__
#define __INCLUDE__LINUX__COHDA__PKTBUF_KERNEL_H__

#include <linux/types.h>
#include <linux/skbuff.h>
#include <net/sock.h> // for gfp_any()

#define PktBufQueue sk_buff_head
#define PktBuf sk_buff


/// @brief skb_queue_head_init
static inline void PktBuf_QueueInit(struct PktBufQueue *pQ)
{
  skb_queue_head_init(pQ);
}

/// @brief skb_queue_len
static inline uint32_t PktBuf_QueueLen(const struct PktBufQueue *pQ)
{
  return skb_queue_len(pQ);
}

/// @brief skb_queue_empty
static inline bool PktBuf_QueueEmpty(const struct PktBufQueue *pQ)
{
  return skb_queue_empty(pQ);
}

/// @brief skb_peek
static inline struct PktBuf *PktBuf_Peek(struct PktBufQueue *pQ)
{
  return skb_peek(pQ);
}

/// @brief skb_peek_tail
static inline struct PktBuf *PktBuf_PeekTail(struct PktBufQueue *pQ)
{
  return skb_peek_tail(pQ);
}

/// @brief skb_dequeue
static inline struct PktBuf *PktBuf_Dequeue(struct PktBufQueue *pQ)
{
  return skb_dequeue(pQ);
}

/// @brief skb_dequeue_tail
static inline struct PktBuf *PktBuf_DequeueTail(struct PktBufQueue *pQ)
{
  return skb_dequeue_tail(pQ);
}

/// @brief skb_insert
static inline int PktBuf_Insert(struct PktBuf *pOld,
                                struct PktBuf *pNew,
                                struct PktBufQueue *pQ)
{
  skb_insert(pOld, pNew, pQ);
  return 0;
}

/// @brief skb_append
static inline int PktBuf_Append(struct PktBuf *pOld,
                                struct PktBuf *pNew,
                                struct PktBufQueue *pQ)
{
  skb_append(pOld, pNew, pQ);
  return 0;
}

/// @brief skb_unlink
static inline int PktBuf_Unlink(struct PktBuf *pPkb,
                                struct PktBufQueue *pQ)
{
  skb_unlink(pPkb, pQ);
  return 0;
}

/// @brief skb_queue_head
static inline int PktBuf_QueueHead(struct PktBufQueue *pQ, struct PktBuf *pNew)
{
  skb_queue_head(pQ, pNew);
  return 0;
}

/// @brief skb_queue_tail
static inline int PktBuf_QueueTail(struct PktBufQueue *pQ, struct PktBuf *pNew)
{
  skb_queue_tail(pQ, pNew);
  return 0;
}

/// @brief skb_queue_purge
static inline void PktBuf_QueuePurge(struct PktBufQueue *pQ)
{
  skb_queue_purge(pQ);
}

/// skb_queue_walk_safe
#define PktBuf_QueueWalkSafe skb_queue_walk_safe

/// @brief alloc_skb
static inline struct PktBuf *PktBuf_Alloc(int Len)
{
  return alloc_skb(Len, gfp_any());
}

/// @brief kfree_skb
static inline void PktBuf_Free(struct PktBuf *pPkb)
{
  kfree_skb(pPkb);
}

/// @brief skb_clone
static inline struct PktBuf *PktBuf_Clone(struct PktBuf *pParent)
{
  return skb_clone(pParent, gfp_any());
}

/// @brief skb_share_check
static inline struct PktBuf *PktBuf_ShareCheck(struct PktBuf *pOld)
{
  return skb_share_check(pOld, gfp_any());
}

/// @brief skb_put
static inline unsigned char *PktBuf_Put(struct PktBuf *pPkb, int Len)
{
  return skb_put(pPkb, Len);
}

/// @brief skb_push
static inline unsigned char *PktBuf_Push(struct PktBuf *pPkb, int Len)
{
  return skb_push(pPkb, Len);
}

/// @brief skb_pull
static inline unsigned char *PktBuf_Pull(struct PktBuf *pPkb, int Len)
{
  return skb_pull(pPkb, Len);
}

/// @brief skb_trim
static inline void PktBuf_Trim(struct PktBuf *pPkb, int Len)
{
  return skb_trim(pPkb, Len);
}

/// @brief skb_headroom
static inline int PktBuf_Headroom(struct PktBuf *pPkb)
{
  return skb_headroom(pPkb);
}

/// @brief skb_tailroom
static inline int PktBuf_Tailroom(struct PktBuf *pPkb)
{
  return skb_tailroom(pPkb);
}

/// @brief skb_reserve
static inline void PktBuf_Reserve(struct PktBuf *pPkb, int Len)
{
  return skb_reserve(pPkb, Len);
}

/// @brief skb_copy
static inline struct PktBuf *PktBuf_Copy(struct PktBuf *pSrc)
{
  return skb_copy(pSrc, gfp_any());
}

/// @brief skb_copy_expand
static inline struct PktBuf *PktBuf_CopyExpand(struct PktBuf *pSrc, int Pre, int Post)
{
  return skb_copy_expand((struct sk_buff *) pSrc, Pre, Post, gfp_any());
}

/// @brief skb_copy_bits
static inline int PktBuf_CopyBits(const struct PktBuf *pPkb, int Offset, void *pDst, int Len)
{
  return skb_copy_bits(pPkb, Offset, pDst, Len);
}

/// @brief skb_store_bits
static inline int PktBuf_StoreBits(struct PktBuf *pPkb, int Offset, void *pSrc, int Len)
{
  return skb_store_bits(pPkb, Offset, pSrc, Len);
}

/// @brief skb_mac_header
static inline unsigned char *PktBuf_GetMacHdr(struct PktBuf *pPkb)
{
  return skb_mac_header(pPkb);
}
/// @brief skb_set_mac_header
static inline void PktBuf_SetMacHdr(struct PktBuf *pPkb, int Len)
{
  skb_set_mac_header(pPkb, Len);
}
/// @brief skb_reset_mac_header
static inline void PktBuf_ResetMacHdr(struct PktBuf *pPkb)
{
  skb_reset_mac_header(pPkb);
}

/// @brief skb_network_header
static inline unsigned char *PktBuf_GetNetworkHdr(struct PktBuf *pPkb)
{
  return skb_network_header(pPkb);
}
/// @brief skb_set_mac_header
static inline void PktBuf_SetNetworkHdr(struct PktBuf *pPkb, int Len)
{
  skb_set_network_header(pPkb, Len);
}
/// @brief skb_reset_network_header
static inline void PktBuf_ResetNetworkHdr(struct PktBuf *pPkb)
{
  skb_reset_network_header(pPkb);
}

/// @brief skb_transport_header
static inline unsigned char *PktBuf_GetTransportHdr(struct PktBuf *pPkb)
{
  return skb_transport_header(pPkb);
}
/// @brief skb_set_mac_header
static inline void PktBuf_SetTransportHdr(struct PktBuf *pPkb, int Len)
{
  return skb_set_transport_header(pPkb, Len);
}
/// @brief skb_reset_network_header
static inline void PktBuf_ResetTransportHdr(struct PktBuf *pPkb)
{
  skb_reset_transport_header(pPkb);
}

/// @brief skb_orphan
static inline void PktBuf_Orphan(struct PktBuf *pPkb)
{
  skb_orphan(pPkb);
}


/**
 @todo
skb_recv_datagram
skb_copy_datagram_iovec
skb_free_datagram
*/

#endif // #ifndef __INCLUDE__LINUX__COHDA__PKTBUF_KERNEL_H__
// Close the doxygen group
/**
 * @}
 */
