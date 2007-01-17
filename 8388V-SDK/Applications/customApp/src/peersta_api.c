 /*******************************************************************
 *
 *    DESCRIPTION: Peer STA API implementation
 *
 *    AUTHOR:
 *
 *    HISTORY:
 *
 *    DATE:10/2/2006
 *
 *******************************************************************/
 
 /** include files **/
 #include "wltypes.h"
 #include "dbg_Facility.h"
 #include "mli.h"
 #include "peersta_api.h"
 #include <string.h>
 /** local definitions **/
 #define NUM_PEERS 2
 
 STADataBase_t STADb[NUM_PEERS];

 #define NUM_TX_ELEMENTS 4
 peerstatx_e_t    peerstatx_e[NUM_TX_ELEMENTS];
 int next_tx_element = 0;
 int num_stas = 0;
 uint8 peerstaIE[6 + 6*NUM_PEERS];
void 	peersta_TxFreeBuffer(wcb_t *buffer);

 /* default settings */
 
 /** external functions **/
 
 /** external data **/
 
 /** internal functions **/

 
/**
 * finds the index in the STA database for a given 
 * Mac Address
 * 
 * @param MacAddr
 * @return 
 */
int get_staindex_from_macaddr(IEEEtypes_MacAddr_t MacAddr)
 {
	 int i;
	 for (i = 0; i < num_stas; i++) {
		 if(!memcmp((void *)STADb[i].PeerStaAddr,(void *)MacAddr,sizeof(IEEEtypes_MacAddr_t)))
		 {
           return i;
		 }
	 }
     return -1;
 }

/**
 * Updates the IE for the MacAddresses with the data
 * pending 
 */
void update_peersta_IE(void)
 {
	const uint8 oui[4] = { 0x00, 0x50, 0xf2, 0x05 };
    int i;
	peerstaIE[0] = 221;
	memcpy(&peerstaIE[2], oui, sizeof(oui));
	peerstaIE[1] = 5;
	peerstaIE[6] = 0;
	for(i = 0; i < num_stas; i++)
	{
       if(STADb[i].tx_q.q_len) {
         memcpy((void *)&peerstaIE[peerstaIE[1] + 2], (void *) STADb[i].PeerStaAddr, sizeof(IEEEtypes_MacAddr_t));
         peerstaIE[1] += 6;
		 peerstaIE[6]++;
	   }
	}
	mli_updateIEinBeacon(peerstaIE);
 }

 /**
  * Empties the tx_q for a given index
  * 
  * @param index
  */
 void empty_tx_q(int index)
 {
	 peerstatx_e_t *data_tx_e;

	 while((data_tx_e = getq(&STADb[index].tx_q)) != NULL)
	 {
	  if(STADb[index].tx_q.q_len == 0) {
	    ((wcb_t *)data_tx_e->tx_buffer)->TxPacketType = MLI_CUSTOM_DATA_LAST_PKT;
	  } else {
	    ((wcb_t *)data_tx_e->tx_buffer)->TxPacketType = MLI_CUSTOM_DATA_MORE_PKT;
	  }
	  mli_TxDataEnqueue((wcb_t *)data_tx_e->tx_buffer,peersta_TxFreeBuffer);
	 }
 } 


 /**
  * processes the rxed packets
  * 
  * @param io_pPkt
  * @return 
  */
 MLI_CUSTOM_DATA_T peersta_rx_handler(RxPD_t *io_pPkt)
{
	char *x;
	int index;
	IEEEtypes_MacAddr_t PeerAddr;
	if(io_pPkt != NULL) {
		x = (char *)((char *)io_pPkt + sizeof(RxPD_t) + 6);
        memcpy((void *)PeerAddr,x,sizeof(IEEEtypes_MacAddr_t));
        index = get_staindex_from_macaddr(PeerAddr);
        if(index >= 0) {
           STADb[index].receive((uint8*)io_pPkt + sizeof(RxPD_t));
		   empty_tx_q(index);
		   update_peersta_IE();
		}
	}
 	return MLI_CUSTOM_DATA_NOT_MY_PKT;
}


 
 /** public data **/
 
 /** private data **/
 
 /** public functions **/
 
/**
  * Initialization routine
  * 
  * @return 
  */

 uint32 	peersta_Init(void)
 {
	 memset((void*)&STADb[0],0x00, NUM_PEERS*sizeof(STADataBase_t));
	 num_stas = 0;
	 next_tx_element = 0;

	 // Install  Rx Data Handler
	 mli_installCustomRxDataHandler(peersta_rx_handler);
	 update_peersta_IE();
	 
	 return 0;
 }
 
 /**
  * Adds entry in the peer sta database
  * 
  * @param MacAddress
  * @param num_tx_bufs
  * @param receive
  * @return 
  */
 uint32 	peersta_AddSTAEntry(IEEEtypes_MacAddr_t MacAddress, 
							   uint32 num_tx_bufs,
							   void  (*receive)(uint8 *buffer))
 {
	 int i;

     if((get_staindex_from_macaddr(MacAddress) < 0) && (num_stas < NUM_PEERS)) {
	  memcpy((void *)STADb[num_stas].PeerStaAddr, (void *)MacAddress, sizeof(IEEEtypes_MacAddr_t));
      STADb[num_stas].in_use = 1;
	  
	  for(i = 0 ; i < num_tx_bufs; i++) {
	   peerstatx_e[next_tx_element + i].tx_buffer = mli_GetRawTxBuffer();
	   peerstatx_e[next_tx_element + i].sta_index = num_stas;
	  }

          // Chain tx elements as a link list
      for (i=0; i < num_tx_bufs; i++)
      {
       // last element points to null
       if (i == (num_tx_bufs-1))
          peerstatx_e[next_tx_element + i].next = 0;
        else
          peerstatx_e[next_tx_element + i].next = &peerstatx_e[next_tx_element + i + 1];
      }
      // Init tx_freeq to contain pointers to all tx elements
      STADb[num_stas].tx_free_q.q_head = (qp)&peerstatx_e[next_tx_element];
      STADb[num_stas].tx_free_q.q_tail = (qp)&peerstatx_e[next_tx_element + num_tx_bufs - 1];
      STADb[num_stas].tx_free_q.q_len  = num_tx_bufs;
      STADb[num_stas].tx_free_q.q_max  = num_tx_bufs;
      STADb[num_stas].tx_free_q.q_min  = num_tx_bufs;

	  memset(&STADb[num_stas].tx_q, 0, sizeof(STADb[num_stas].tx_q));

      STADb[num_stas].receive = receive;
      next_tx_element += num_tx_bufs;
	  num_stas++;
	  return 0;
     }
   return 1;
 }
 /**
  * This function removes an entry from the Peer Station DataBase
  * 
  * @param MacAddress
  * @return 
  */
 uint32 	peersta_RemoveSTAEntry(IEEEtypes_MacAddr_t MacAddress)
 {
     int index;  
	 index = get_staindex_from_macaddr(MacAddress);
	 if(index > 0)
	   STADb[index].in_use = 0;
	 return SUCCESS;
 }
 /**
  * Application uses the function to obtain a buffer for the
  *    purpose of transmitting data
  * 
  * @param sta_handle
  * @return 
  */
 uint8 * 	peersta_GetTxFreeBuffer(IEEEtypes_MacAddr_t MacAddress)
 {
    uint8* buffer = NULL;
    peerstatx_e_t *data_tx_e;
    int index;
	index = get_staindex_from_macaddr(MacAddress);
    if (index < 0) {
		return NULL;
	}
	data_tx_e = getq(&STADb[index].tx_free_q);
	if (data_tx_e) {
		buffer = (uint8 *)(data_tx_e->tx_buffer);
	}
    return buffer;
 }
 /**
  * Application uses this function to free a tx a buffer associated to a peer station 
  * 
  * @param 
  * @param buffer
  * @return 
  */
void 	peersta_TxFreeBuffer(wcb_t *buffer)
{
	int i;
	for(i = 0; i < NUM_TX_ELEMENTS; i++)
	{
		if(peerstatx_e[i].tx_buffer == (uint8 *)buffer)
			break;
	}
    putq(&STADb[peerstatx_e[i].sta_index].tx_free_q,&peerstatx_e[i]);
}

/**
 * Application uses this function to tx a buffer to the peer station
 * 
 * @param MacAddress
 * @param buffer
 * @return 
 */
Status_e 	peersta_SendTxBuffer(uint8 *buffer)
{
	int i;
	for(i = 0; i < NUM_TX_ELEMENTS; i++)
	{
		if(peerstatx_e[i].tx_buffer == (uint8 *)buffer)
			break;
	}
    putq(&STADb[peerstatx_e[i].sta_index].tx_q,&peerstatx_e[i]);
	update_peersta_IE();
	return SUCCESS;
}

 /** private functions **/
 
