#include "debug.h"

void debug_print_txt( char *txt, UINT16 len )
{
	UINT16 i;
	for( i = 0; i < len; i++ )
		putchar( txt[i] );
}

/* print a RFC-1035 format domain name */
void debug_print_name( char *name )
{
	char *s = name;

	while( *s ) {
		if( *s & 0xC0 ) { /* pointer */
			DB_PRINT( "[PTR=0x%02X]", (UINT16)(((*s & ~(0xC0))<<8) | *(s+1)) );
			return;
		}
		else { /* label */
			putchar( '.' );
			debug_print_txt( s+1, *s ); /* print label text */
			s += *s;
		}
		s++;
	}
}

/* print question (query) data */
void debug_print_question( struct mdns_question *q )
{
	DB_PRINT( "--------------------------------------------------------\n"
			  "question: \"" );
	debug_print_name( q->qname );
	DB_PRINT( "\"\n(type %u, class %u)\n", q->qtype, q->qclass);
}

/* print resource (answer) and associated RR */
void debug_print_resource( struct mdns_resource *r )
{
	struct rr_srv *srv;

	DB_PRINT( "--------------------------------------------------------\n"
			"resource \"" );
	debug_print_name( r->name );
	DB_PRINT( "\" (type %u, class %u%s)\n\tttl=%u, rdlength=%u\n",
			r->type,
			r->class & 0x8000 ? r->class & ~(0x8000) : r->class,
			r->class & 0x8000 ? " FLUSH" : "", r->ttl, r->rdlength );
	switch( r->type ) {
		case T_A:
		DB_PRINT( "\tA type, IP=0x%X\n", *((UINT32*)r->rdata) );
		break;
		case T_NS:
		DB_PRINT( "\tNS type, name=\"" );
		debug_print_name( (char *)r->rdata );
		DB_PRINT( "\"\n" );
		break;
		case T_CNAME:
		DB_PRINT( "\tCNAME type, name=\"" );
		debug_print_name( (char *)r->rdata );
		DB_PRINT( "\"\n" );
		break;
		case T_SRV:
		DB_PRINT( "\tSRV type, " );
		srv = (struct rr_srv*)r->rdata;
		printf( "priority: %u, weight: %u, port: %u, target: \"",
			ntohs(srv->priority), ntohs(srv->weight), ntohs(srv->port) );
		debug_print_name( (char *)(r->rdata+3*sizeof(UINT16)) );
		DB_PRINT( "\"\n" );
		break;
		case T_PTR:
		DB_PRINT( "\tPTR type, name=\"" );
		debug_print_name( (char *)r->rdata );
		DB_PRINT( "\"\n" );
		break;
		case T_TXT:
		DB_PRINT( "\tTXT type, data=\"" );
		debug_print_txt( (char *)r->rdata, r->rdlength );
		DB_PRINT( "\"\n" );
		break;
		default:
		DB_PRINT( "\tunknown RR type\n" );
		break;
	}
}

void debug_print_header( struct mdns_header *h )
{
	DB_PRINT( "--------------------------------------------------------\n"
			  "header:\nID=%u, QR=%u, AA=%d OPCODE=%u\n"
			  "QDCOUNT=%u, ANCOUNT=%u, NSCOUNT=%u, ARCOUNT=%u\n",
			ntohs(h->id), h->flags.fields.qr,
			h->flags.fields.aa, h->flags.fields.opcode,
			ntohs(h->qdcount), ntohs(h->ancount),
			ntohs(h->nscount), ntohs(h->arcount) );
}

/* print information about a message: header, questions, answers */
void debug_print_message( struct mdns_message *m )
{
	int i;

	DB_PRINT( "########################################################\n"
			  "printing message:\n" );

	debug_print_header( m->header );

	for( i = 0; i < m->num_questions; i++ ) 
		debug_print_question( &m->questions[i] );

	for( i = 0; i < m->num_answers; i++ )
		debug_print_resource( &m->answers[i] );
}
