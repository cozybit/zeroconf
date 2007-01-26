#ifdef MDNS_HOST_TEST
#include "host.h"
#else
#include "target.h"
#endif
#include "mdns.h"

UINT16 mdns_read_n16( struct mdns_message *m )
{
	UINT16 n = ntohs(*((UINT16*)m->cur));
	m->cur += sizeof(UINT16);
	return n;
}

UINT32 mdns_read_n32( struct mdns_message *m )
{
	UINT32 n = ntohl(*((UINT32*)m->cur));
	m->cur += sizeof(UINT32);
	return n;
}

void mdns_write_n16( struct mdns_message *m, UINT16 n )
{
	*((UINT16*)m->cur) = htons(n);
	m->cur += sizeof(UINT16);
}

void mdns_write_n32( struct mdns_message *m, UINT32 n )
{
	*((UINT32*)m->cur) = htonl(n);
	m->cur += sizeof(UINT32);
}

void mdns_mark_response( struct mdns_message *m )
{
	m->header->flags.fields.qr = 1; /* response */
	m->header->flags.fields.aa = 1; /* authoritative */
	m->header->flags.fields.rcode = 0;
}

void mdns_mark_question( struct mdns_message *m )
{
	m->header->flags.fields.qr = 0; /* query */
}

/* a name may be one of:
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

int mdns_parse_message( struct mdns_message *m, char *b )
{
	UINT16 i, t;

	m->header = (struct mdns_header *)b;
	m->num_questions = ntohs(m->header->qdcount);
	m->num_answers = ntohs(m->header->ancount);

	m->header->flags.num = ntohs(m->header->flags.num);

	if( m->header->flags.fields.opcode != 0 ) {
		DB_PRINT( "dropping message with opcode != 0\n" ); 
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
			DB_PRINT( "dropping message: invalid label in question %d\n",i);
			return 0;
		}
		/* get qtype */
		t = mdns_read_n16( m );
		if( t <= T_ANY )
			m->questions[i].qtype = t;
		else {
			DB_PRINT( "dropping message: invalid type %u\n", t );
			return 0;
		}
		/* get qclass */
		t = mdns_read_n16( m );
		if( ( t & ~0x8000 ) == 1 )
			m->questions[i].qclass = t;
		else {
			DB_PRINT( "dropping message: invalid class %u\n", t );
			return 0;
		}
	}

	for( i = 0; i < m->num_answers && i < MDNS_MAX_ANSWERS; i++ ) {
		m->answers[i].name = m->cur;
		if( !mdns_traverse_name( m ) ) {
			DB_PRINT( "dropping message: invalid label in answer %d\n", i);
			return 0;
		}
		m->answers[i].type = mdns_read_n16( m );
		m->answers[i].class = mdns_read_n16( m );
		m->answers[i].ttl = mdns_read_n32( m );
		m->answers[i].rdlength = mdns_read_n16( m );
		m->answers[i].rdata = (void *)m->cur;
		m->cur += m->answers[i].rdlength;
	}
	return 1;	
}

/* transmit message creation */

void mdns_transmit_init( struct mdns_message *m, char *b )
{
	m->header = (struct mdns_header *)b;
	m->cur = b + sizeof(struct mdns_header);
	memset( m->header, 0x00, sizeof(struct mdns_header) );
}

void mdns_add_question( struct mdns_message *m, const char* qname, 
	UINT16 qtype, UINT16 qclass )
{
	strcpy( m->cur, qname ); /* copy name, including terminating null label*/
	m->cur += strlen( qname ) + 1;
	mdns_write_n16( m, qtype );
	mdns_write_n16( m, qclass );
	m->header->qdcount += htons(1);
}

void mdns_add_answer( struct mdns_message *m, const char *name, UINT16 type,
	UINT16 class, UINT32 ttl, UINT16 length, void *data )
{
	strcpy( m->cur, name );
	m->cur += strlen( name ) + 1;
	mdns_write_n16( m, type );
	mdns_write_n16( m, class );
	mdns_write_n32( m, ttl );
	mdns_write_n16( m, length );
	memcpy( m->cur, data, length );
	m->cur += length;
	m->header->ancount += htons(1);
}
