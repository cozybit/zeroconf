/*******************************************************************
 *
 *    DESCRIPTION: PEER STATION API
 *
 *    AUTHOR:
 *
 *    HISTORY:
 *
 *    DATE:9/22/2006
 *
 *******************************************************************/
  #ifndef PEER_STA_API_H
  #define PEER_STA_API_H
  /** include files **/
  #include "wltypes.h"
  #include "queue.h"
  #include "IEEE_types.h"
  /** local definitions **/
  
  /* default settings */
  
  /** external functions **/
  
  /** external data **/
  
  /** internal functions **/
  
  /** public data **/
  
  /* Data structure associated with the Peer Station DataBase*/
typedef struct _STADataBase_t {
 unsigned char       in_use;
 IEEEtypes_MacAddr_t PeerStaAddr;
 Queue_t             tx_q;
 Queue_t             tx_free_q;
 /*  Function implemented by the use APP to receive a packet*/
 void            (*receive)(uint8 * buffer);

} STADataBase_t;


  /** private data **/
// tx element type
typedef struct  _peerstatx_e_t {
    struct _peerstatx_e_t *next;          // for queuing
	uint32         sta_index;
    uint8 *        tx_buffer;
} peerstatx_e_t;
  
  /** public functions **/

  /**
   * Initialization routine
   * 
   * @return 
   */
  uint32 	peersta_Init(void); 

  /**
   * This Functions adds an entry in the Peer Station DataBase
   *    Inputs to the functions are Mac address of the peer station,
   *    desired number of tx buffers for the queuing
   * 
   * @param MacAddress
   * @param num_tx_bufs
   * @return SUCCESS/FALIURE
   */
  uint32 	peersta_AddSTAEntry(IEEEtypes_MacAddr_t MacAddress,
								uint32 num_tx_bufs,
								void  (*receive)(uint8 *buffer)); 
  /**
   * This function removes an entry from the Peer Station DataBase
   * 
   * @param MacAddress
   * @return 
   */
  uint32 	peersta_RemoveSTAEntry(IEEEtypes_MacAddr_t MacAddress);  
  /**
   * Application uses the function to obtain a buffer for the
   *    purpose of transmitting data
   * 
   * @param MacAddress
   * @return 
   */
  uint8 * 	peersta_GetTxFreeBuffer(IEEEtypes_MacAddr_t MacAddress);
  /**
   * Application uses this function to tx a buffer to the peer station
   * 
   * @param buffer
   * @return 
   */
  Status_e 	peersta_SendTxBuffer(uint8 *buffer);

  /** private functions **/
  
 #endif
