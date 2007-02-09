/*! \file mdns.c
 *  \brief low-level mDNS functions */

#include "system.h"
#include "mdns.h"

UINT16 mdns_name_length( char *name );

/** get a host-order UINT16 from a message and advance the message pointer */
UINT16 mdns_read_n16( struct mdns_message *m )
{
	PACKED UINT16 *p = (UINT16*)m->cur;
    m->cur += sizeof(UINT16);
    return ntohs(*p);
}

/** get a host-order UINT32 from a message and advance the message pointer */
UINT32 mdns_read_n32( struct mdns_message *m )
{
    PACKED UINT32 *p = (UINT32*)m->cur;
    m->cur += sizeof(UINT32);
    return ntohl((*p));
}

/** write a UINT16 in network order to a message, advance message pointer */
void mdns_write_n16( struct mdns_message *m, UINT16 n )
{
    PACKED UINT16 *p = ((UINT16*)m->cur);
	*p = htons(n);
    m->cur += sizeof(UINT16);
}

/** write a UINT32 in network order to a message, advance message pointer */
void mdns_write_n32( struct mdns_message *m, UINT32 n )
{
    PACKED UINT32* p = ((UINT32*)m->cur);
	*p = htonl(n);
    m->cur += sizeof(UINT32);
}

/** copy a domain name to a message, advance message pointer */
void mdns_write_name( struct mdns_message *m, char *name )
{
    UINT16 len = mdns_name_length( name );
    memcpy( m->cur, name, len );
    m->cur += len;
}

void mdns_mark_response( struct mdns_message *m )
{
    m->header->flags.fields.qr = 1; /* response */
    m->header->flags.fields.aa = 1; /* authoritative */
    m->header->flags.fields.rcode = 0;
    m->header->flags.num = htons(m->header->flags.num);
}

void mdns_mark_question( struct mdns_message *m )
{
    m->header->flags.fields.qr = 0; /* query */
}

/** traverse a domain name in a message and adjust the message pointer past it
 *
 *  \note a name may be one of:
 *  - a series of labels terminated by a NULL byte
 *  - a series of labels terminated by a pointer
 *  - a pointer */
int mdns_traverse_name( struct mdns_message *m )
{
    while( *(m->cur) != 0x00 ) {
        if( ( *(m->cur) & 0xC0 ) ) { /* pointer */
            m->cur++;
            break;
        }
        else if( *(m->cur) <= MDNS_MAX_LABEL_LEN ) /* valid label */
            m->cur += *(m->cur)+1; /* move to next label */
        else /* invalid label */
            return 0;
    }
    m->cur++; /* move past terminating byte */
    return 1;
}

/** calculate the length (including termiation) of a domain name */
UINT16 mdns_name_length( char *name )
{
    char *start = name;

    while( *name != 0 ) {
        if( *name & 0xC0 ) { /* pointer */
            name++;
            break;
        }
        name += *name+1;
    }

    return (UINT32)name - (UINT32)start + 1;
}

/** traverse a received message, set up data representations for its contents 
 *
 *  \note This function will check that the message appears to be valid, as
 *  defined in the mDNS specification for an incomming message.  It will return
 *  failure if the message does not appear to be valid.
 *
 *  \param[in] m Message structure that will represent this message. 
 *  \param[in] b Buffer containing the message data 
 *
 *  \retval int 1 on success, 0 on failure */
int mdns_parse_message( struct mdns_message *m, char *b )
{
    UINT16 i, t;

    m->header = (struct mdns_header *)b;
    m->num_questions = ntohs(m->header->qdcount);
    m->num_answers = ntohs(m->header->ancount);

    m->header->flags.num = ntohs(m->header->flags.num);

    if( m->header->flags.fields.opcode != 0 ) {
        /* DB_PRINT( "dropping message with opcode != 0\n" ); */
        return 0;
    }

    #if 0
    if( m->header->flags.fields.rcode != 0 ) {
        DB_PRINT( "dropping message with rcode != 0\n" ); 
        return 0;
    }
    #endif
    m->cur = (char *)m->header + sizeof(struct mdns_header);

    for( i = 0; i < m->num_questions && i < MDNS_MAX_QUESTIONS; i++ ) {
        /* get qname */
        m->questions[i].qname = m->cur;
        if( !mdns_traverse_name( m ) ) { /* move past qname */
           /*DB_PRINT( "dropping message: invalid label in question %d\n",i);*/
            return 0;
        }
        /* get qtype */
        t = mdns_read_n16( m );
        if( t <= T_ANY )
            m->questions[i].qtype = t;
        else {
            /*DB_PRINT( "dropping message: invalid type %u\n", t );*/
            return 0;
        }
        /* get qclass */
        t = mdns_read_n16( m );
        if( ( t & ~0x8000 ) == 1 )
            m->questions[i].qclass = t;
        else {
            /*DB_PRINT( "dropping message: invalid class %u\n", t );*/
            return 0;
        }
    }

    for( i = 0; i < m->num_answers && i < MDNS_MAX_ANSWERS; i++ ) {
        m->answers[i].name = m->cur;
        if( !mdns_traverse_name( m ) ) {
            /*DB_PRINT( "dropping message: invalid label in answer %d\n", i);*/
            return 0;
        }
        m->answers[i].type = mdns_read_n16( m );
        m->answers[i].class = mdns_read_n16( m );
        m->answers[i].ttl = mdns_read_n32( m );
        m->answers[i].rdlength = mdns_read_n16( m );
        m->answers[i].rdata = m->cur;
        m->cur += m->answers[i].rdlength;
    }
    return 1;
}

/* transmit message creation */

void rr_transfer_a( struct mdns_message *m, union rr_p r )
{
    mdns_write_n32( m, r.a->ip );
}

void rr_transfer_cname( struct mdns_message *m, union rr_p r )
{
    mdns_write_name( m, r.cname->name );
}

void rr_transfer_txt( struct mdns_message *m, union rr_p r )
{
    mdns_write_name( m, r.txt->data );
}

void rr_transfer_ns( struct mdns_message *m, union rr_p r )
{
    mdns_write_name( m, r.ns->name );
}

void rr_transfer_srv( struct mdns_message *m, union rr_p r )
{
    mdns_write_n16( m, r.srv->priority );
    mdns_write_n16( m, r.srv->weight );
    mdns_write_n16( m, r.srv->port );
    mdns_write_name( m, r.srv->target );
}

void rr_transfer_ptr( struct mdns_message *m, union rr_p r )
{
    mdns_write_name( m, r.ptr->name );
}

UINT16 rr_length_a( union rr_p r )
{
    return sizeof(UINT32);
}

UINT16 rr_length_cname( union rr_p r )
{
    return mdns_name_length( r.cname->name );
}

UINT16 rr_length_txt( union rr_p r )
{
    return mdns_name_length( r.txt->data );
}

UINT16 rr_length_ns( union rr_p r )
{
    return mdns_name_length( r.ns->name );
}

UINT16 rr_length_ptr( union rr_p r )
{
    return mdns_name_length( r.ptr->name );
}

UINT16 rr_length_srv( union rr_p r )
{
    return 3*sizeof(UINT16)+mdns_name_length( r.srv->target );
}

void mdns_transmit_init( struct mdns_message *m, char *b )
{
    m->header = (struct mdns_header *)b;
    m->cur = b + sizeof(struct mdns_header);
    memset( (void *)m->header, 0x00, sizeof(struct mdns_header) );
}

void mdns_add_question( struct mdns_message *m, char* qname,
    UINT16 qtype, UINT16 qclass )
{
    UINT16 len = mdns_name_length( qname );

    memcpy( m->cur, qname, len );
    m->cur += len;
    mdns_write_n16( m, qtype );
    mdns_write_n16( m, qclass );
    m->header->qdcount += htons(1);
}

void mdns_add_answer( struct mdns_message *m, char *name, UINT16 type,
    UINT16 class, UINT32 ttl, struct mdns_rr *rr )
{
    UINT16 d_len = mdns_name_length( name );

    memcpy( m->cur, name, d_len );
    m->cur += d_len;
    mdns_write_n16( m, type );
    mdns_write_n16( m, class );
    mdns_write_n32( m, ttl );
    mdns_write_n16( m, rr->length( rr->data ) );
    rr->transfer( m, rr->data );

    m->header->ancount += htons(1);
}

/** send an outgoing mDNS message 
 *  XXX this current assumes that sendto() will send the whole message in one
 *  shot, which is probably not a valid assumption... */
int mdns_send_message( struct mdns_message *m, int sock )
{
    struct sockaddr_in to;
    int size, len;

    /* get message length */
    size = (unsigned int)m->cur - (unsigned int)m->header;

    to.sin_family = AF_INET;
    to.sin_port = htons(5353);
    to.sin_addr.s_addr = inet_addr("224.0.0.251"); /* FIXME use macro */

    len = sendto( sock, (char *)m->header, size, 0, (struct sockaddr *)&to,
        sizeof(struct sockaddr_in) );

    if( len < size ) {
        return 0;
    }
    return 1;
}

