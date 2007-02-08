/*! \file mdns.h 
 *  \brief mDNS API and definitions */

#ifndef MDNS_H
#define MDNS_H

#include "system.h"

/* settings */

#define MDNS_BUFFER_LEN		1000	/**< length of mDNS TX and RX buffers */
#define MDNS_MAX_NAME_LEN	255		/**< max octets allowed in a domain name */
#define MDNS_MAX_LABEL_LEN	63 		/**< max octets allowed in a label */
#define MDNS_MAX_QUESTIONS	32 		/**< max Questions allowed in a message */
#define MDNS_MAX_ANSWERS	10 		/**< max Answers allowed in a message */

/* class */

#define C_IN	0x0001 /**< DNS and mDNS Internet class */
#define C_FLUSH	0x8001 /**< mDNS Flush class (Internet with MSB set to 1) */

/* types (that are used in mDNS) */

#define T_A		1	/**< host address */
#define T_NS	2	/**< authoritative name server */
#define T_CNAME	5 	/**< canonical name for an alias */
#define T_PTR	12	/**<  domain name pointer */
#define T_TXT	16	/**< text strings */
#define T_SRV	33	/**< service record */
#define T_ANY	255	/**< any records (wildcard) */

/*! \struct mdns_header
 *  \brief mDNS fixed-size message header */
PACKED_STRUCT_BEGIN struct mdns_header {
	UINT16 id; /**< transaction ID */
	PACKED_STRUCT_BEGIN union {
		PACKED_STRUCT_BEGIN struct { UINT16
			rcode:4,	/**< response code */
			cd:1,		/**< checking disabled  RFC-2535*/
			ad:1,		/**< authentic data RFC-2535 */
			z:1,		/**< zero */
			ra:1,		/**< recursion available */
			rd:1,		/**< recursion desired */
			tc:1,		/**< truncated */
			aa:1,		/**< authoritative answer */
			opcode:4,	/**< should be 0 for mDNS messages */
			qr:1;		/**< query/response */
		} fields ;
		UINT16 num;
	} flags;
	UINT16 qdcount; /**< number of entries in questions section */
	UINT16 ancount; /**< number of resource records in answer section */
	UINT16 nscount;
	UINT16 arcount;
} ;

/* Resource Record (RR) representations */
PACKED_STRUCT_BEGIN struct rr_a { UINT32 ip; };
PACKED_STRUCT_BEGIN struct rr_cname { char *name; };
PACKED_STRUCT_BEGIN struct rr_txt { char *data; };
PACKED_STRUCT_BEGIN struct rr_ns { char *name; };
PACKED_STRUCT_BEGIN struct rr_srv { UINT16 priority; UINT16 weight; 
									UINT16 port; char *target; };
PACKED_STRUCT_BEGIN struct rr_ptr { char *name; };

/*! \union rr_p
 *  \brief Resource Record (RR) pointer */
PACKED_STRUCT_BEGIN union rr_p {
	struct rr_a *a;
	struct rr_cname *cname;
	struct rr_txt *txt;
	struct rr_ns *ns;
	struct rr_srv *srv;
	struct rr_ptr *ptr;
} ;

/*! \struct mdns_question
 *  \brief mDNS question (query) representation */
PACKED_STRUCT_BEGIN struct mdns_question {
	char *qname; /**< question domain name */
	UINT16 qtype; /**< question type */
	UINT16 qclass; /**< question class */
} ;

/*! \struct mdns_resource
 *  \brief mDNS resource record (RR) format */
PACKED_STRUCT_BEGIN struct mdns_resource {
	char *name; /**< resource domain name */
	UINT16 type; /**< resource type for the RDATA field */
	UINT16 class; /**< resource class for the RDATA field */
	UINT32 ttl; /**< how long the record may be cached */
	UINT16 rdlength; /**< length of RDATA field */
	void *rdata; /**< resource data (RDATA) */
} ;

/*! \struct mdns_message
 *  \brief mDNS message representation */
PACKED_STRUCT_BEGIN struct mdns_message {
	struct mdns_header *header;
	char *cur; /**< message data pointer */
	/* pointers to (and representations of) questions and answers */
	struct mdns_question questions[MDNS_MAX_QUESTIONS];
	struct mdns_resource answers[MDNS_MAX_ANSWERS];
	UINT16 num_questions, num_answers; /* for convenience */
} ;

/*! \struct mdns_rr
 *  \brief mDNS resource record wrapper */
PACKED_STRUCT_BEGIN struct mdns_rr {
	void(*transfer)( struct mdns_message *m, union rr_p r ); /**< move the 
										resource record data into a message */
	UINT16(*length)( union rr_p r ); /**< calculate the length of the resource 
										  record data */
	union rr_p data; /**< the resource record data */
} ;

/* helper macros */

/*! \def MDNS_IS_QUERY(msg)
 *  \brief is the message a query? */
#define MDNS_IS_QUERY(msg)		(msg.header->flags.fields.qr == 0)
/*! \def MDNS_IS_RESPONSE(msg)
 *  \brief is the message a query response? */
#define MDNS_IS_RESPONSE(msg)	(msg.header->flags.fields.qr == 1)

/** Mark and set up a message for transmit.
 * 
 * This function should be called as a first step in transmitting a message. It
 * initializes the message data structure and sets up the message header 
 * appropriately for an outgoing message.  The message's 'cur' pointer will 
 * point to the end of the header after this function returns.  The next step
 * is to mark the message as a query or response and then to add questions 
 * and/or answers as needed. Finally, call mdns_send_message to transmit.
 *
 * \param[in] m The message to set up for transmit.
 * \param[in] b The buffer to use for this message.
 * */
void mdns_transmit_init( struct mdns_message *m, char *b );

/** Mark a message as an mDNS query response.
 *
 * This function should be called after mdns_transmit_init has set up the
 * message.  It marks bits in the message header appropriately for a 'response'
 * type message.
 *
 * \param[in] m The message to mark as a response.
 * */
void mdns_mark_response( struct mdns_message *m );

/** Mark a message as an mDNS query (question).
 *
 * This function should be called after mdns_transmit_init has set up the
 * message.  It marks bits in the message header appropriately for a 'query'
 * type message.
 *
 * \param[in] m The message to mark as a query (question).
 * */
void mdns_mark_question( struct mdns_message *m );

/** Send an mDNS message.
 *
 * This function should be called only after the message is marked as a
 * transmit message, marked as a query or response, and questions and/or
 * answers are added to it.  This sets up the message header and 'cur' pointer
 * correctly for mdns_send_message.
 *
 * \param[in] m The message to send.
 * \param[in] sock The multicast socket to send on.
 * \retval int 1 on success, 0 on failure
 * */
int mdns_send_message( struct mdns_message *m, int sock );

/** Add a question to an outgoing message.
 *
 * This function may be called after the outgoing message has been set up with
 * mdns_transmit_init and mdns_mark_question.  It adds the question to the 
 * message buffer and updates the message data structure accordingly, moving
 * the 'cur' pointer to the end of the buffer.
 *
 * \param[in] m The message to add a question to.
 * \param[in] qname The question domain name.
 * \param[in] qtype The question Type.
 * \param[in] qclass The question Class.
 * */
void mdns_add_question( struct mdns_message *m, char* qname,
    UINT16 qtype, UINT16 qclass );

/** Add a resource (answer) to an outgoing message.
 *
 * This function may be called after the outgoing message has been set up with
 * mdns_transmit_init and mdns_mark_response.  It adds the response resource to
 * the message buffer and updates the message data structure accordingly, 
 * moving the 'cur' pointer to the end of the buffer.
 *
 * \param[in] m The message to add a resource (answer) to.
 * \param[in] name The resource domain name.
 * \param[in] type The resource Type.
 * \param[in] class The resource Class.
 * \param[in] ttl The resource Time To Live.
 * \param[in] rr The resource record to add as RDATA.
 * */
void mdns_add_answer( struct mdns_message *m, char *name, UINT16 type,
    UINT16 class, UINT32 ttl, struct mdns_rr *rr );

/** Parse an incomming message buffer.
 *
 * This function should be used on receive buffers.  It parses the incomming
 * message buffer, setting up an mdns_message data structure to represent the
 * message.  This function attempts to detect invalid messages and will return
 * an error in such cases.
 *
 * \param[in] m The message to use on the buffer.
 * \param[in] b The buffer to parse.
 * \retval int 1 on success, 0 on failure.
 * */
int mdns_parse_message( struct mdns_message *m, char *b );

void rr_transfer_a( struct mdns_message *m, union rr_p r );
void rr_transfer_cname( struct mdns_message *m, union rr_p r );
void rr_transfer_txt( struct mdns_message *m, union rr_p r );
void rr_transfer_ns( struct mdns_message *m, union rr_p r );
void rr_transfer_srv( struct mdns_message *m, union rr_p r );
void rr_transfer_ptr( struct mdns_message *m, union rr_p r );

UINT16 rr_length_a( union rr_p r );
UINT16 rr_length_cname( union rr_p r );
UINT16 rr_length_txt( union rr_p r );
UINT16 rr_length_ns( union rr_p r );
UINT16 rr_length_srv( union rr_p r );
UINT16 rr_length_ptr( union rr_p r );

#endif
