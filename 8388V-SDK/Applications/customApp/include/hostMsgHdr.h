
#ifndef _HOSTMSGHDR_H
#define _HOSTMSGHDR_H

typedef PACK_START  struct host_MsgHdr_t
{
    UINT16 Msg;          /*Command number - host_Msg_e*/
    UINT16 Size;         /*Size of the data structure*/
    UINT16 SeqNum;       /*Command sequence number*/
    UINT16 Result;       /*Result code - host_Result_e*/
}
PACK_END host_MsgHdr_t;


#endif

