/*
 * Copyright Notice:
 * Copyright Treck Incorporated  1997 - 2004
 * Copyright Elmic Systems Japan 1997 - 2004
 * No portions or the complete contents of this file may be copied or
 * distributed in any form (including but not limited to printed or electronic
 * forms) without the expressed written consent of Treck Incorporated OR
 * Elmic Systems.  Copyright laws and International Treaties protect the
 * contents of this file.  Unauthorized use is prohibited.
 * All rights reserved.
 *
 * Description: Macro Definitions
 * Filename: trmacro.h
 * Author: Odile & Paul
 * Date Created: 9/24/97
 * $Source: include/trmacro.h $
 *
 * Modification History
 * $Revision: 4.1 $
 * $Date: 2004/11/07 21:05:14PST $
 * $Author: xwang $
 * $ProjectName: /home/mks/proj/tcpip.pj $
 * $ProjectRevision: 4.1.2.1 $
 */

#ifndef _TRMACRO_H_

#define _TRMACRO_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef TM_INTERRUPT
#define TM_INTERRUPT interrupt
#endif /* ! TM_INTERRUPT */

#ifndef TM_GLOBAL_QLF
#define TM_GLOBAL_QLF
#endif /* ! TM_GLOBAL_QLF */

#ifndef TM_GLOBAL_NEAR
#define TM_GLOBAL_NEAR
#endif /* ! TM_GLOBAL_NEAR */

#ifndef TM_CONST_QLF
#define TM_CONST_QLF
#endif /* ! TM_CONST_QLF */


/* undefine IPv6 feature macros, if we aren't using IPv6 */






#ifdef TM_6_IP_REASSEMBLY
#undef TM_6_IP_REASSEMBLY
#endif /* TM_6_IP_REASSEMBLY */







#ifndef TM_HTTPD_DEF_MAX_SERVERS
/* Default max number of servers allowed */
#define TM_HTTPD_DEF_MAX_SERVERS    (tt16Bit)10
#endif /* TM_HTTPD_DEF_MAX_SERVERS */

/* SSI needs CGI to send dynamic data*/

/* Disable all HTTPD features if HTTPD is disabled*/
#undef TM_USE_HTTPD_SSI
#undef TM_USE_HTTPD_CGI


/* check that we are either IPv4, IPv6 or dual IP layer */

/* Ensure the user picks only one custom checksum option */
#if (defined(TM_USER_OCS) && defined(TM_CUSTOM_OCS))
#error "You may #define only one of TM_USER_OCS and TM_CUSTOM_OCS!"
#endif





#if (defined(TM_SINGLE_INTERFACE_HOME) && !(defined(TM_SINGLE_INTERFACE)))
#define TM_SINGLE_INTERFACE
#endif /* TM_SINGLE_INTERFACE_HOME && !TM_SINGLE_INTERFACE */

#ifdef TM_DEMO_TIME_LIMIT
#define TM_DEMO_LIMIT_SEC       TM_UL(3600)
#endif /* TM_DEMO_TIME_LIMIT */

#ifndef TM_SHEAP_NUM_PAGE
/*
 * Make sure the simple heap macros are correctly defined.
 */
#define TM_SHEAP_NUM_PAGE 1
#endif /* !TM_SHEAP_NUM_PAGE */

#if (TM_SHEAP_NUM_PAGE < 1)
#error "TM_SHEAP_NUM_PAGE must be greater or equal than 1."
#endif /* !(TM_SHEAP_NUM_PAGE < 1) */


#ifdef TM_USE_TEST_POINTS
#ifndef TM_USE_NETSTAT
/* some of the test point code requires netstat */
#define TM_USE_NETSTAT
#endif /* ! TM_USE_NETSTAT */
#endif /* TM_USE_TEST_POINTS */

#ifdef TM_USE_NETSTAT
#ifndef TM_SNMP_CACHE
/* TM_SNMP_CACHE is needed for netstat tools */
#define TM_SNMP_CACHE
#endif /* !TM_SNMP_CACHE */
#endif /* TM_USE_NETSTAT */

#ifdef TM_PC_LINT
/*
 * PC-LINT error messages suppression 
 */

#define TM_LINT /* Using a Lint program */

/* External variable or function not referenced */
#define LINT_UNREF_EXTRN_BEGIN(x)       /*lint -esym(714,x) */
#define LINT_UNREF_EXTRN_END(x)         /*lint +esym(714,x) */

/* Automatic variable not initialized */
#define LINT_UNINIT_AUTO_BEGIN(x)       /*lint -esym(644,x) -esym(771,x) */
#define LINT_UNINIT_AUTO_END(x)         /*lint +esym(644,x) +esym(771,x) */

/* Automatic variable not initialized */
#define LINT_UNINIT_SYM_BEGIN(x)        /*lint -esym(530,x) */
#define LINT_UNINIT_SYM_END(x)          /*lint +esym(530,x) */

/* Automatic variable not accessed */
#define LINT_UNACCESS_SYM_BEGIN(x)      /*lint -esym(550,x) */
#define LINT_UNACCESS_SYM_END(x)        /*lint +esym(550,x) */

/* Formal parameter not referenced */
#define LINT_UNREF_PARAM_BEGIN(x)       /*lint -esym(715,x) */
#define LINT_UNREF_PARAM_END(x)         /*lint +esym(715,x) */

/* Boolean expression always evaluate to true/false */
#define LINT_BOOLEAN_BEGIN              /*lint -e774 */
#define LINT_BOOLEAN_END                /*lint +e774 */

/* Possible use of null pointer */
#define LINT_NULL_PTR_BEGIN(x)          /*lint -esym(613,x) -esym(794,x) */
#define LINT_NULL_PTR_END(x)            /*lint +esym(613,x) +esym(794,x) */

/* Member of struct or union not reference. */
#define LINT_UNREF_MEMBER_BEGIN         /*lint ++flb */
#define LINT_UNREF_MEMBER_END           /*lint --flb */

/* Enable pointer cast. */
#define LINT_PTR_CAST_BEGIN             /*lint +fpc */
#define LINT_PTR_CAST_END               /*lint -fpc */

/* Possible access of out-of-bounds pointer */
#define LINT_OUT_OF_BOUNDS_PTR_BEGIN    /*lint -e661 */
#define LINT_OUT_OF_BOUNDS_PTR_END      /*lint +e661 */

/* Constant evaluate to zero, overflow and excessive shift value */
#define LINT_ZERO_OVFL_EXS_CONST        /*lint -e(778) -e(648) -e(572) */

/* Header file not used */
#define LINT_UNUSED_HEADER              /*lint -e766 */

/* Macro not referenced */
#define LINT_UNREF_MACRO(x)             /*lint -esym(750,x) */

/* Function not referenced */
#define LINT_UNREF_FUNCTION(x)          /*lint -esym(528,x) */

/* Function not referenced */
#define LINT_UNREF_SYMBOL(x)            /*lint -esym(528,x) -esym(529,x) */

/* Excessive shift value and constant expression evaluate to 0 */
#define LINT_EXSH_ZERO_BEGIN            /*lint -save -e572 -e778 */

/* Symbol not accessed and not initialized */
#define LINT_ACCE_INIT_BEGIN            /*lint -save -e550 -e530 */

/* Restore */
#define LINT_RESTORE                    /*lint -restore */

/* Constant evaluate to zero */
#define LINT_ZERO_MACRO(x)              /*lint -emacro(778,x) */

/* Automatic variable not accessed within macro */
#define LINT_UNACCESS_MACRO(x)          /*lint -emacro(550,x) */

/* Expected unsigned type */
#define LINT_EXPT_U_MACRO(x)            /*lint -emacro(502,x) */

/* Shift left of signed quanlity */
#define LINT_SHIFT_L_MACRO(x)           /*lint -emacro(701,x) */

/* Shift right of signed quanlity */
#define LINT_SHIFT_R_MACRO(x)           /*lint -emacro(702,x) */

/* Loss of precision */
#define LINT_LOSS_PREC_MACRO(x)         /*lint -emacro(713,x) -emacro(734,x) */

/* Possible use of null pointer */
#define LINT_NULL_PTR_MACRO(x)          /*lint -emacro(613,x) */

/* Unusual pointer cast */
#define LINT_PTR_CAST_MACRO(x)          /*lint -emacro(740,x) */

#endif /* TM_PC_LINT */

#ifndef TM_LINT
/*
 * Not using a LINT program
 */

#define LINT_UNREF_EXTRN_BEGIN(x)
#define LINT_UNREF_EXTRN_END(x)
#define LINT_UNINIT_AUTO_BEGIN(x)
#define LINT_UNINIT_AUTO_END(x)
#define LINT_UNINIT_SYM_BEGIN(x)
#define LINT_UNINIT_SYM_END(x)
#define LINT_UNACCESS_SYM_BEGIN(x)
#define LINT_UNACCESS_SYM_END(x)
#define LINT_UNREF_PARAM_BEGIN(x)
#define LINT_UNREF_PARAM_END(x)
#define LINT_BOOLEAN_BEGIN
#define LINT_BOOLEAN_END
#define LINT_NULL_PTR_BEGIN(x)
#define LINT_NULL_PTR_END(x)
#define LINT_UNREF_MEMBER_BEGIN
#define LINT_UNREF_MEMBER_END
#define LINT_PTR_CAST_BEGIN
#define LINT_PTR_CAST_END
#define LINT_OUT_OF_BOUNDS_PTR_BEGIN
#define LINT_OUT_OF_BOUNDS_PTR_END
#define LINT_ZERO_OVFL_EXS_CONST
#define LINT_UNUSED_HEADER
#define LINT_UNREF_MACRO(x)
#define LINT_UNREF_FUNCTION(x)
#define LINT_EXSH_ZERO_BEGIN
#define LINT_ACCE_INIT_BEGIN
#define LINT_RESTORE
#define LINT_ZERO_MACRO(x)
#define LINT_UNACCESS_MACRO(x)
#define LINT_EXPT_U_MACRO(x)
#define LINT_SHIFT_L_MACRO(x)
#define LINT_SHIFT_R_MACRO(x)
#define LINT_LOSS_PREC_MACRO(x)
#define LINT_NULL_PTR_MACRO(x)
#define LINT_PTR_CAST_MACRO(x)

#endif /* TM_LINT */

#ifndef TM_DISABLE_ANSI_LINE_FILE
#define TM_USE_ANSI_LINE_FILE
#endif /* ! TM_DISABLE_ANSI_LINE_FILE */


#ifdef TM_SNMP_VERSION
/*
 * For SNMP agents, TM_SNMP_VERSION is automatically defined when trmibtls
 * is run.
 */

#ifndef TM_SNMP_MIB
#define TM_SNMP_MIB
#endif /* TM_SNMP_MIB */

#endif /* TM_SNMP_VERSION */

#ifdef TM_SNMP_CACHE
/* Invalid cache index, when routing/ARP/socket entry is not in the cache */
#define TM_SNMPC_NO_INDEX    (tt8Bit)255
/*
 * Number of caches:
 * 4 caches (one for the ARP entries, one for the routing entries UDP socket,
 * one for TCP vectors, one for UDP sockets.)
 */
#define TM_SNMPD_CACHES_PER_INET  4
#define TM_SNMPD_CACHES           TM_SNMPD_CACHES_PER_INET
/* Index of SNMP ARP entries cache in tvSnmpdCaches[] */
#define TM_SNMPC_ARP_INDEX        0
/* Index of SNMP route entries cache in tvSnmpdCaches[] */
#define TM_SNMPC_ROUTE_INDEX      1
/* Index of SNMP TCP vectors cache in tvSnmpdCaches[] */
#define TM_SNMPC_TCP_INDEX        2
/* Index of SNMP UDP sockets cache in tvSnmpdCaches[] */
#define TM_SNMPC_UDP_INDEX        3


#endif /* TM_SNMP_CACHE */

#ifdef TM_TRECK_LIB
/*
 * To build Treck libraries
 */
/* For library environment, assume everything is needed */

#define TM_CRITICAL_NEEDED
#define TM_LOCK_NEEDED
#define TM_PEND_POST_NEEDED
#define TM_RECV_PEND_ISR_POST_NEEDED
#define TM_SENT_PEND_ISR_POST_NEEDED
#define TM_XMIT_PEND_POST_NEEDED

#else /* ! TM_TRECK_LIB */
/*
 * To build in source environment
 */
/* Macros generated by trsystem.h switches, in source environment */

#ifdef TM_TRECK_TASK
/* Use defaults. Nothing special needed */
#endif /* TM_TRECK_TASK */

#ifdef TM_TRECK_NO_KERNEL
/* Only critical sections needed */
#define TM_CRITICAL_NEEDED
#endif /* TM_TRECK_NO_KERNEL */

#ifdef TM_TRECK_PREEMPTIVE_KERNEL
/* Critical sections, locks, Pend/post needed */
#define TM_CRITICAL_NEEDED
#define TM_LOCK_NEEDED
#define TM_PEND_POST_NEEDED
#endif /* TM_TRECK_PREEMPTIVE_KERNEL  */

#ifdef TM_TRECK_NONPREEMPTIVE_KERNEL
/* Critical sections, Pend/post needed */
#define TM_CRITICAL_NEEDED
#define TM_PEND_POST_NEEDED
#endif /*TM_TRECK_NONPREEMPTIVE_KERNEL  */

#ifdef TM_PEND_POST_NEEDED
#ifdef TM_TASK_RECV
/* Check if RECV ISR POST event is needed */
#define TM_RECV_PEND_ISR_POST_NEEDED
#endif /* TM_TASK_RECV */
#endif /* TM_PEND_POST_NEEDED */

#ifdef TM_PEND_POST_NEEDED
#ifdef TM_TASK_SEND
/* Check if SEND ISR POST event is needed */
#define TM_SENT_PEND_ISR_POST_NEEDED
#endif /* TM_TASK_SEND */
#endif /* TM_PEND_POST_NEEDED */

#ifdef TM_PEND_POST_NEEDED
#ifdef TM_TASK_XMIT
/* Check if XMIT task POST event is needed */
#define TM_XMIT_PEND_POST_NEEDED
#endif /* TM_TASK_XMIT */
#endif /* TM_PEND_POST_NEEDED */

#endif /* ! TM_TRECK_LIB */

#ifdef TM_MULTIPLE_CONTEXT
/*
 * Multiple context IP stack. If defined, context dependent global
 * variables, are dereferenced from a dynamically allocated structure.
 */
#if defined(TM_KERNEL_WIN32_X86) || defined(TM_KERNEL_LINUX_APP)
#define tm_context(x)   ((ttContextPtr)tfKernelGetContext())->x
#else /* TM_KERNEL_WIN32_X86 */
#define tm_context(x)   tvCurrentContext->x
#endif /* TM_KERNEL_WIN32_X86 */
#else /* !TM_MULTIPLE_CONTEXT */
#define tm_context(x)   tvCurrentContextStruct.x
#endif /* !TM_MULTIPLE_CONTEXT */

#define tm_global(x)    tvGlobals.x

#ifndef TM_CRITICAL_NEEDED
#define tm_kernel_set_critical
#define tm_kernel_release_critical
#endif /* TM_CRITICAL_NEEDED */

#ifdef TM_USE_SHEAP
#define tm_kernel_single_malloc(size)     tfSheapMalloc(size)
#define tm_kernel_free(memPtr)            tfSheapFree(memPtr)
#define tm_sheap_create()                 tfSheapCreate()
#else /* !TM_USE_SHEAP */
#define tm_kernel_single_malloc(size)     tfKernelMalloc(size)
#define tm_kernel_free(memPtr)            tfKernelFree(memPtr)
#define tm_sheap_create()
#endif /* TM_USE_SHEAP */

#ifdef TM_RECV_PEND_ISR_POST_NEEDED
#define tm_kernel_create_recv_event(x)    tfKernelCreateEvent(x)
#define tm_kernel_isr_post_recv_event(x)  tfKernelIsrPostEvent(x)
#define tm_kernel_post_recv_event(x)      tfKernelTaskPostEvent(x)
#define TM_PEND_EVENT_NEEDED
#else /* ! TM_RECV_PEND_ISR_POST_NEEDED */
#define tm_kernel_create_recv_event(x)
#define tm_kernel_isr_post_recv_event(x)
#define tm_kernel_post_recv_event(x)
#endif /* TM_RECV_PEND_ISR_POST_NEEDED */

#ifdef TM_SENT_PEND_ISR_POST_NEEDED
#define tm_kernel_create_sent_event(x)    tfKernelCreateEvent(x)
#define tm_kernel_isr_post_sent_event(x)  tfKernelIsrPostEvent(x)
#define tm_kernel_post_sent_event(x)      tfKernelTaskPostEvent(x)
#define TM_PEND_EVENT_NEEDED
#else /* ! TM_SENT_PEND_ISR_POST_NEEDED */
#define tm_kernel_create_sent_event(x)
#define tm_kernel_isr_post_sent_event(x)
#define tm_kernel_post_sent_event(x)
#endif /* TM_SENT_PEND_ISR_POST_NEEDED */

#ifdef TM_XMIT_PEND_POST_NEEDED
#define tm_kernel_create_xmit_event(x)    tfKernelCreateEvent(x)
#define tm_kernel_post_xmit_event(x)      tfKernelTaskPostEvent(x)
#define TM_PEND_EVENT_NEEDED
#else /* !TM_XMIT_PEND_POST_NEEDED */
#define tm_kernel_create_xmit_event(x)
#define tm_kernel_post_xmit_event(x)
#endif /* TM_XMIT_PEND_POST_NEEDED */

#ifdef TM_PEND_EVENT_NEEDED
#define tm_kernel_pend_event(x)           tfKernelPendEvent(x)
#else /* !TM_PEND_EVENT_NEEDED */
#define tm_kernel_pend_event(x)
#endif /* TM_PEND_EVENT_NEEDED */

#ifdef TM_PEND_POST_NEEDED
#define TM_DEF_PEND_POST_STATUS TM_8BIT_YES
#else /* !TM_PEND_POST_NEEDED */
#define TM_DEF_PEND_POST_STATUS TM_8BIT_ZERO
#endif /* TM_PEND_POST_NEEDED */

#ifdef TM_LOCK_NEEDED
#define TM_DEF_LOCK_STATUS TM_8BIT_YES
#else /* !TM_LOCK_NEEDED */
#define TM_DEF_LOCK_STATUS TM_8BIT_ZERO
#endif /* TM_LOCK_NEEDED */

/* Macro to control definition of TM_EXTERN for global variables.
 * TM_GLOBAL only defined in trinit.c to get the global variables
 * declared as non extern global. For all other modules TM_EXTERN is
 * defined as extern, and global variables are only referenced.
 */
#ifdef TM_GLOBAL
#define TM_EXTERN /* global variables are declared */
#else /* !TM_GLOBAL */
#define TM_EXTERN extern /* global variables are referenced */
#endif /* TM_GLOBAL */

/* Far Macro's */
#ifndef TM_MACRO_FAR
#define TM_MACRO_FAR far
#endif /* TM_FAR */

#ifdef  TM_INTEL_SMALL_REAL_MODEL
#ifndef TM_FAR
#define TM_FAR TM_MACRO_FAR
#endif /* TM_FAR */
#endif /* TM_INTEL_SMALL_REAL_MODEL */

#ifdef  TM_INTEL_MEDIUM_REAL_MODEL
#ifndef TM_FAR
#define TM_FAR TM_MACRO_FAR
#endif /* TM_FAR */
#endif /* TM_INTEL_MEDIUM_REAL_MODEL */

#ifdef  TM_INTEL_COMPACT_REAL_MODEL
#ifndef TM_FAR
#define TM_FAR TM_MACRO_FAR
#endif /* TM_FAR */
#endif /* TM_INTEL_COMPACT_REAL_MODEL */

#ifdef  TM_INTEL_LARGE_REAL_MODEL
#ifndef TM_FAR
#define TM_FAR TM_MACRO_FAR
#endif /* TM_FAR */
#endif /* TM_INTEL_LARGE_REAL_MODEL */

#ifndef TM_FAR
#define TM_FAR
#endif /* TM_FAR */

#ifndef TM_CODE_FAR
#define TM_CODE_FAR
#endif /* TM_CODE_FAR */

#ifndef TM_GLOBAL_NEAR
/* Global pointers */
/* If user did not define TM_GLOBAL_NEAR in trsystem.h */
#define TM_GLOBAL_NEAR
#endif /* TM_GLOBAL_NEAR */

#ifdef TM_USE_CLIB
#define tm_bcopy(a, b, c)       (void)memcpy( (ttVoidPtr)(b), \
                                              (ttConstVoidPtr)(a), \
                                              (unsigned int)(c) )
#define tm_bzero(a, b)          (void)memset( (ttVoidPtr)(a), 0, \
                                              (unsigned int)(b) )
#define tm_memset(a, b, c)      memset( (ttVoidPtr)(a), (int)(b), \
                                        (unsigned int)(c))
#define tm_memcmp(a, b, c)      memcmp( (ttConstVoidPtr)(a), \
                                        (ttConstVoidPtr)(b), \
                                        (unsigned int)(c) )
#define tm_memcpy(a, b, c)      memcpy( (ttVoidPtr)(a), \
                                          (ttConstVoidPtr)(b), \
                                          (unsigned int)(c))
#define tm_memchr(a, b, c)      memchr( (ttConstVoidPtr)(a), \
                                        (int)(b), \
                                        (unsigned int)(c) )
#define tm_strcmp(a, b)         strcmp( (ttConstCharPtr)(a), \
                                        (ttConstCharPtr)(b))
#define tm_strncmp(a, b, c)     strncmp( (ttConstCharPtr)(a), \
                                         (ttConstCharPtr)(b), \
                                         (unsigned int)(c) )
#define tm_strcpy(a, b)         (void)strcpy( (ttCharPtr)(a), \
                                              (ttConstCharPtr)(b))
#define tm_strcat(a, b)         (void)strcat( (ttCharPtr)(a), \
                                              (ttConstCharPtr)(b))
#define tm_strlen(a)            strlen((ttConstCharPtr)(a))
#define tm_strncpy(a, b, c)     strncpy( (ttCharPtr)(a), \
                                         (ttConstCharPtr)(b), \
                                         (unsigned int)(c))
#define tm_strcspn(a, b)        strcspn( (ttConstCharPtr)(a), \
                                         (ttConstCharPtr)(b))
#define tm_strchr(a, b)         strchr((ttConstCharPtr)(a), (int)(b))
#define tm_strrchr(a, b)        strrchr((ttConstCharPtr)(a), (int)(b))
#define tm_strstr(a, b)         strstr( (ttConstCharPtr)(a), \
                                        (ttConstCharPtr)(b))
#define tm_sprintf              sprintf
#define tm_vsprintf(a, b, c)    vsprintf( (ttCharPtr)(a), \
                                          (ttConstCharPtr)(b), \
                                          (c))
#define tm_sscanf               sscanf
#define tm_toupper(asciiCh)     toupper(asciiCh)
#define tm_isspace(c)           isspace(c)
#define tm_isupper(c)           isupper(c)
#define tm_islower(c)           islower(c)
#define tm_isdigit(c)           isdigit(c)
#define tm_isxdigit(c)          isxdigit(c)
#define tm_isalpha(c)           isalpha(c)
#define tm_isprint(c)           isprint(c)
#define tm_qsort(a, b, c, d)    qsort((ttVoidPtr)a, (unsigned int)(b), \
                                      (unsigned int)(c), (ttCmpFuncPtr)(d))
#define tm_strtol(a, b, c)      strtol(a, b, c)
#define tm_strtoul(a, b, c)     strtoul(a, b, c)
#else /* !TM_USE_CLIB */
#ifndef tm_bcopy
#define tm_bcopy(a, b, c)       tfBcopy( (ttConstVoidPtr)(a), \
                                         (ttVoidPtr)(b), \
                                         (unsigned int)(c) )
#endif /* ! tm_bcopy */
#ifndef tm_memset
#define tm_memset(a, b, c)      tfMemSet( (ttVoidPtr)(a), \
                                          (int)(b), \
                                          (unsigned int)(c) )
#endif /* ! tm_memset */
#ifndef tm_bzero
#define tm_bzero(a, b)          tfBzero((ttVoidPtr)(a), (unsigned int)(b))
#endif /* ! tm_bzero */
#ifndef tm_memcmp
#define tm_memcmp(a, b, c)      tfMemCmp( (ttConstVoidPtr)(a), \
                                          (ttConstVoidPtr)(b), \
                                          (unsigned int)(c))
#endif /* ! tm_memcmp */
#ifndef tm_memcpy
#define tm_memcpy(a, b, c)      tfMemCpy( (ttVoidPtr)(a), \
                                          (ttConstVoidPtr)(b), \
                                          (unsigned int)(c))
#endif /* ! tm_memcpy */
#ifndef tm_memchr
#define tm_memchr(a, b, c)      tfMemChr( (ttConstVoidPtr)(a), \
                                          (int)(b), \
                                          (unsigned int)(c) )
#endif /* ! tm_memchr */
#ifndef tm_strcmp
#define tm_strcmp(a, b)         tfStrCmp( (ttConstCharPtr)(a), \
                                          (ttConstCharPtr)(b))
#endif /* ! tm_strcmp */
#ifndef tm_strncmp
#define tm_strncmp(a, b, c)     tfStrNCmp((ttConstCharPtr)(a), \
                                          (ttConstCharPtr)(b), \
                                          (unsigned int)(c))
#endif /* ! tm_strncmp */
#ifndef tm_strcpy
#define tm_strcpy(a, b)         tfStrCpy((ttCharPtr)(a), (ttConstCharPtr)(b))
#endif /* ! tm_strcpy */
#ifndef tm_strcat
#define tm_strcat(a, b)         tfStrCat((ttCharPtr)(a), (ttConstCharPtr)(b))
#endif /* ! tm_strcat */
#ifndef tm_strlen
#define tm_strlen(a)            tfStrLen((ttConstCharPtr)(a))
#endif /* ! tm_strlen */
#ifndef tm_strncpy
#define tm_strncpy(a, b, c)     (void)tfStrNCpy( (ttCharPtr)(a), \
                                           (ttConstCharPtr)(b), \
                                           (unsigned int)(c) )
#endif /* ! tm_strncpy */
#ifndef tm_strrchr
#define tm_strrchr(a, b)        tfStrRChr((ttConstCharPtr)(a), (int)(b))
#endif /* ! tm_strrchr */
#ifndef tm_strchr
#define tm_strchr(a, b)         tfStrChr( (ttConstCharPtr)(a), (int)(b))
#endif /* ! tm_strchr */
#ifndef tm_strcspn
#define tm_strcspn(a, b)        tfStrCSpn((ttConstCharPtr)(a), \
                                          (ttConstCharPtr)(b))
#endif /* ! tm_strcspn */
#ifndef tm_strstr
#define tm_strstr(a, b)         tfStrStr( (ttConstCharPtr)(a), \
                                          (ttConstCharPtr)(b))
#endif /* ! tm_strstr */
#ifndef tm_sprintf
#define tm_sprintf              tfSPrintF
#endif /* ! tm_sprintf               */
#ifndef tm_vsprintf
#define tm_vsprintf(a, b, c)    tfVSPrintF( (ttCharPtr)(a),        \
                                            (ttConstCharPtr)(b),   \
                                            (c) )
#endif /* ! tm_vsprintf */
#ifndef tm_sscanf
#define tm_sscanf               tfSScanF
#endif /* ! tm_sscanf */
#ifndef tm_isspace
#define tm_isspace(c)           (c == ' ')
#endif /* ! tm_isspace */
#ifndef tm_isupper
#define tm_isupper(c)           ((c >= 'A') && (c <= 'Z'))
#endif /* ! tm_isupper */
#ifndef tm_islower
#define tm_islower(c)           ((c >= 'a') && (c <= 'z'))
#endif /* ! tm_islower */
#ifndef tm_isdigit
#define tm_isdigit(c)           ((c >= '0') && (c <= '9'))
#endif /* ! tm_isdigit */
#ifndef tm_isxdigit
#define tm_isxdigit(c)          (      (tm_isdigit(c) != 0)        \
                                    || ((c >= 'a') && (c <= 'f'))  \
                                    || ((c >= 'A') && (c <= 'F')) )
#endif /* ! tm_isxdigit */
#ifndef tm_isalpha
#define tm_isalpha(c)           (    (tm_isupper(c) != 0) \
                                  || (tm_islower(c) != 0) )
#endif /* ! tm_isalpha */
#ifndef tm_isprint
#define tm_isprint(c)           ((c >= 0x20) && (c <= 0x7E))
#endif /* ! tm_isprint */
#ifndef tm_toupper
#define tm_toupper(asciiCh)     (char)((asciiCh) + 'A' - 'a')
#endif /* ! tm_toupper */
#ifndef tm_qsort
#define tm_qsort(a, b, c, d)    tfQSort((ttVoidPtr)a, (unsigned int)(b), \
                                        (unsigned int)(c), (ttCmpFuncPtr)(d))
#endif /* ! tm_qsort */
#ifndef tm_strtol
#define tm_strtol(a, b, c)      tfStrToL(a, b, c)
#endif /* ! tm_strtol */
#ifndef tm_strtoul
#define tm_strtoul(a, b, c)     tfStrToUl(a, b, c)
#endif /* ! tm_strtoul */

#endif /* TM_USE_CLIB */

#ifndef tm_isascii
#define tm_isascii(c)           ((unsigned)(c) <= 0x7F)
#endif /* ! tm_isascii */

#define tm_bcopy_to_packed(src, dst, len) tfBcopyToPacked(                   \
                                                (const void TM_FAR *) (src), \
                                                (void TM_FAR *) (dst),       \
                                                (unsigned int) len,          \
                                                (unsigned int) 0 )

#define tm_bcopy_to_packed_offset(src, dst, len, offset) tfBcopyToPacked(    \
                                                (const void TM_FAR *) (src), \
                                                (void TM_FAR *) (dst),       \
                                                (unsigned int) len,          \
                                                (unsigned int) offset )


#define tm_bcopy_to_unpacked(src, dst, len) tfBcopyToUnpacked(                \
                                                 (const void TM_FAR *) (src), \
                                                 (void TM_FAR *) (dst),       \
                                                 (unsigned int) (len))


#define tm_zero_struct(x)       tm_bzero(&(x), sizeof(x))

#define tm_64Bit_incr(LL)       (LL).low++, (LL).high += ((LL).low == 0)
#define tm_64Bit_augm(LL, L)    (LL).low += (tt32Bit) (L);    \
                                if ((LL).low < (tt32Bit) (L)) \
                                {                             \
                                    (LL).high++;              \
                                }
#define tm_get_64bit_low(a)     a.low
#define tm_get_64bit_high(a)    a.high

#ifndef TM_SHEAP_SIZE
/* Default value if TM_SHEAP_SIZE is not defined */
/*
 * Can't define an array size of zero. It will not be used
 * if it is not called
 */
#define TM_SHEAP_SIZE sizeof(tt32Bit)
#endif /* TM_SHEAP_SIZE */

#ifndef TM_DISABLE_DYNAMIC_MEMORY
/*
 * User did not disallow use of dynamic memory. 
 */
#define TM_USE_DYNAMIC_MEMORY
#endif /* TM_DISABLE_DYNAMIC_MEMORY */

#ifdef TM_USE_DYNAMIC_MEMORY
#define tm_kernel_malloc    tfBufferDoubleMalloc
#else /* !TM_USE_DYNAMIC_MEMORY */
#define tm_kernel_malloc    tm_kernel_single_malloc
#endif /* !TM_USE_DYNAMIC_MEMORY */

/* definition of tm_assert() macro for debugging purposes */
#ifdef TM_ERROR_CHECKING
#ifdef TM_USE_ANSI_LINE_FILE
#define tm_assert(funcName, cond) \
    if (!(cond)) \
    { \
        (void)tm_sprintf((char *) tm_global(tvAssertTempBuf), \
                  "assertion error line %d, file(%s)\n", \
                  __LINE__, __FILE__); \
        tfKernelError( #funcName , (char *) tm_global(tvAssertTempBuf)); \
        tm_thread_stop; \
    }
#else   /* ! TM_USE_ANSI_LINE_FILE */
#define tm_assert(funcName, cond) \
    if (!(cond)) \
    { \
        tfKernelError( #funcName , #cond " was false.\n"); \
        tm_thread_stop; \
    }
#endif  /* ! TM_USE_ANSI_LINE_FILE */
#else   /* ! TM_ERROR_CHECKING */
#define tm_assert(funcName, cond)
#endif  /* ! TM_ERROR_CHECKING */


#if (       defined(TM_ERROR_CHECKING)      \
         && defined(TM_USE_ANSI_LINE_FILE)  \
         && (defined(TM_KERNEL_VISUAL_X86) || defined(TM_KERNEL_WIN32_X86) \
             || defined(TM_MEMORY_LOGGING)))
#define tm_get_raw_buffer(size) \
        (ttVoidPtr)tfGetRawBuffer(size, __FILE__, __LINE__);

#if (defined(TM_KERNEL_VISUAL_X86) || defined(TM_KERNEL_WIN32_X86))
#define tm_free_raw_buffer(memPtr)                                 \
{                                                                  \
        tm_call_lock_wait(&tm_global(tvMemoryLeakLock));           \
        tfFreeRawBuffer((ttRawBufferPtr)(ttVoidPtr)(memPtr));      \
        fprintf(tm_global(tvMemoryLeakDetector), "%s, line, %d\n", \
                      __FILE__, __LINE__);                         \
        fflush(tm_global(tvMemoryLeakDetector));                   \
        tm_call_unlock(&tm_global(tvMemoryLeakLock));              \
}
#else /* TM_MEMORY_LOGGING, non-Win32 */
/* NOTE: when using TM_MEMORY_LOGGING, avoid doing any operation
   that requires Treck memory allocation when walking the log buffer. For
   example, have your log walk function call the RTOS printf() function to
   output log messages, since that does not use any Treck memory. */
#define tm_free_raw_buffer(memPtr)                                 \
{                                                                  \
        tm_call_lock_wait(&tm_global(tvMemoryLeakLock));           \
        tfFreeRawBuffer((ttRawBufferPtr)(ttVoidPtr)(memPtr));      \
        tm_memory_log3("%s%s, line, %d\n",                         \
        (char *) tm_global(tvAssertTempBuf), __FILE__, __LINE__);  \
        tm_call_unlock(&tm_global(tvMemoryLeakLock));              \
}
#endif /* TM_MEMORY_LOGGING, non-Win32 */

#else /* TM_ERROR_CHECKING, TM_KERNEL_VISUAL_X86, TM_USE_ANSI_LINE_FILE */
#define tm_get_raw_buffer(size)         (ttVoidPtr)tfGetRawBuffer(size)
#define tm_free_raw_buffer(memPtr) \
        tfFreeRawBuffer((ttRawBufferPtr)(ttVoidPtr)(memPtr));
#endif /* TM_ERROR_CHECKING, TM_KERNEL_VISUAL_X86, TM_USE_ANSI_LINE_FILE */

#ifdef tm_alloc_packet_data_buffer
#ifndef TM_USER_PACKET_DATA_ALLOC
#define TM_USER_PACKET_DATA_ALLOC
#endif /* ! TM_USER_PACKET_DATA_ALLOC */
#ifndef tm_free_packet_data_buffer
/*
 * tm_free_packet_data_buffer must be defined if tm_alloc_packet_data_buffer
 * is defined.
 */
#error "You must define define tm_free_packet_data_buffer!"
#endif /* !tm_free_packet_data_buffer */
#endif /* tm_alloc_packet_data_buffer */

#ifdef TM_USE_DYNAMIC_MEMORY
/* allocate a packet data buffer, called by tfGetSharedBuffer */
#define tm_packet_alloc(size)           tm_recycle_alloc(size, TM_RECY_PACKET)
#define tm_recycle_alloc(size, index)   tfRecycleAlloc(index)
#define tm_recycle_free(rcylPtr, index) tfRecycleFree(rcylPtr, index)
#else /* !TM_USE_DYNAMIC_MEMORY */
#define tm_packet_alloc(size)           tm_kernel_malloc(size)
#define tm_recycle_alloc(size, index)   tm_calloc(size)
#define tm_recycle_free(rcylPtr, index) tm_kernel_free(rcylPtr)
#define tm_calloc(size)                 tfBufferCalloc(size)
#endif /* TM_USE_DYNAMIC_MEMORY */


#ifndef TM_COUNT_SEM_BLOCK_COUNT
/* Default number of allocated semaphores in a block */
#define TM_COUNT_SEM_BLOCK_COUNT        16
#endif /* TM_COUNT_SEM_BLOCK_COUNT */

#ifndef TM_SEL_PEND_BLOCK_COUNT
/* Default number of allocated select pend entries in a block */
#define TM_SEL_PEND_BLOCK_COUNT          4
#endif /* TM_SEL_PEND_BLOCK_COUNT */

#ifndef TM_DISABLE_IP_SRC_ERROR_CHECKING
/* RFC 1122 says that this should be turned on */
#define TM_IP_SRC_ERROR_CHECKING
#endif /* TM_DISABLE_IP_SRC_ERROR_CHECKING */

#ifndef TM_IP_FRAG_Q_SIZE
/* Default number of IP datagrams waiting for reassembly */
#define TM_IP_FRAG_Q_SIZE               (tt8Bit)5
#endif /* TM_IP_FRAG_Q_SIZE */

#ifndef TM_IP_FRAG_ENTRY_MAX_SIZE
/*
 * Default maximum IP datagram data size (without IP header) in the IP
 * reassembly queue.
 */
#define TM_IP_FRAG_ENTRY_MAX_SIZE       TM_UL(8200)
#endif /* TM_IP_FRAG_ENTRY_MAX_SIZE */

#ifndef TM_IP_DEF_FORWARD
/* Default is no forwarding IP packets */
#define TM_IP_DEF_FORWARD           0
#endif /* TM_IP_DEF_FORWARD */

#ifndef TM_ETHER_HW_ALIGN
/*
 * TM_ETHER_HW_ALIGN is used by tfGetEthernetBuffer()
 * TM_ETHER_HW_ALIGN specifies what type of boundary (4 byte, 16 byte, etc)
 * a receive ethernet buffer is aligned on.  This is useful for devices that
 * require a DMA'ed recieve buffer to be aligned on a certain boundary.
 *
 * In tfGetEthernetBuffer(), TM_ETHER_IP_ALIGN is added to the Ethernet
 * buffer so that the TCP/IP header is aligned on a 4-byte boundary.
 * This makes the Ethernet buffer not aligned on a TM_ETHER_HW_ALIGN
 * (4-byte, 16-byte, etc.) boundary anymore.
 *
 * Device drivers that require that the Ethernet buffer be aligned on
 * TM_ETHER_HW_ALIGN, then have to subract TM_ETHER_IP_ALIGN to the
 * pointer returned by tfGetEthernetBuffer().
 */
#define TM_ETHER_HW_ALIGN           4
#endif /* TM_ETHER_HW_ALIGN */

#ifndef TM_ETHER_HW_TX_ALIGN
/*
 * TM_ETHER_HW_TX_ALIGN is used to make sure that allocated buffers for
 * transmission end up being aligned on a certain boundary.
 * TM_ETHER_HW_TX_ALIGN specifies what type of boundary (4 byte, 16 byte, etc)
 * an ethernet buffer to be transmitted need to be aligned on.
 * This is useful for devices that require a transmit buffer to be aligned
 * on a certain boundary.
 */
#define TM_ETHER_HW_TX_ALIGN        2
#endif /* TM_ETHER_HW_TX_ALIGN */

#ifndef TM_DEV_DEF_RECV_CONT_HDR_LENGTH
/*
 * Scattered recv from the device driver:
 * That amount of bytes received from the device driver will be made contiguous
 * in tfRecvScatInterface(), if not already so. Data will be aligned so that
 * the ARP header, or IP header will be aligned on a 32-bit boundary.
 *
 * Non-scattered recv from the device driver:
 * If TM_USE_DRV_SCAT_RECV && TM_NEED_ETHER_32BIT_ALIGNMENT are defined,
 * and tfUseInterfaceScatRecv is not called, amount of Ethernet data copied into
 * a separate buffer in tfRecvInterface, so that the ARP header, or IP header
 * is aligned on a 32-bit boundary.
 */
#define TM_DEV_DEF_RECV_CONT_HDR_LENGTH     \
                                    (TM_ETHER_HEADER_SIZE + TM_ARP_HEADER_SIZE)
#endif /* TM_DEV_DEF_RECV_CONT_HDR_LENGTH */

#ifndef TM_DEV_DEF_RECV_CONT_BUF_LENGTH
/*
 * Non-scattered recv from the device driver:
 * If TM_USE_DRV_SCAT_RECV && TM_NEED_ETHER_32BIT_ALIGNMENT are defined,
 * and tfUseInterfaceScatRecv is not called, threshhold below which the
 * entire Ethernet buffer is copied into a new buffer in tfRecvInterface()
 * so that the ARP header, or IP header is aligned on a 32-bit boundary.
 *
 * Note that TM_DEV_DEF_REVC_CONT_BUF_LENGTH should be bigger than
 * TM_DEV_DEF_RECV_CONT_HDR_LENGTH
 */
#define TM_DEV_DEF_RECV_CONT_BUF_LENGTH  \
                                    (2 * TM_DEV_DEF_RECV_CONT_HDR_LENGTH)
#endif /* TM_DEV_DEF_RECV_CONT_BUF_LENGTH */

#ifndef TM_RIP_DEF
/* 0 (disabled), or 1 (enabled). Default is RIP disabled */
#define TM_RIP_DEF                  TM_8BIT_ZERO
#endif /* TM_RIP_DEF */

#define TM_RIP_NONE                 (tt8Bit)0
#define TM_RIP_1                    (tt8Bit)1 /* enable RIP 1 */
#define TM_RIP_2                    (tt8Bit)2 /* enable RIP 2 */
#define TM_RIP_2_BROADCAST          (tt8Bit)4 /* Send RIP 2 broadcasts */
#define TM_RIP_INF_METRIC                  16 /* RIP infinity metric */

#ifndef TM_NOTIFY_SEND_LOW_WATER
#define TM_NOTIFY_SEND_LOW_WATER    2048
#endif /* TM_NOTIFY_SEND_LOW_WATER */

#ifndef TM_RIP_DEF_SEND_MODE
/* TM_RIP_NONE, TM_RIP_1, TM_RIP_2, TM_RIP_2_BROADCAST */
#define TM_RIP_DEF_SEND_MODE        TM_RIP_2_BROADCAST
#endif /* TM_RIP_MODE */

#ifndef TM_RIP_DEF_RECV_MODE
/* TM_RIP_NONE, TM_RIP_1, TM_RIP_2, or TM_RIP_1|TM_RIP_2 */
#define TM_RIP_DEF_RECV_MODE        (TM_RIP_1|TM_RIP_2)
#endif /* TM_RIP_MODE */

#ifndef TM_IP_DEF_DBCAST_FORWARD
/* Default is forwarding of directed broadcast of IP packets */
#define TM_IP_DEF_DBCAST_FORWARD    1
#endif /* TM_IP_DEF_DBCAST_FORWARD */

#ifndef TM_IP_DEF_DBCAST_FORWARD
/* Default is no forwarding of multicasts packets */
#define TM_IP_DEF_MCAST_FORWARD    0
#endif /* TM_IP_DEF_DBCAST_FORWARD */

#ifndef TM_IP_DEF_FRAGMENT
/*
 * Default is fragmentation allowed, since the variable initialized with
 * this value is only used if TM_IP_FRAGMENT is defined in trsystem.h.
 */
#define TM_IP_DEF_FRAGMENT          1
#endif /* TM_IP_DEF_FRAGMENT */

#ifndef TM_IP_DEF_FRAG_RES
/*
 * Timer resolution for fragmentation reassembly in milliseconds.
 * Default is 1 second.
 */
#define TM_IP_DEF_FRAG_RES          TM_UL(1000)
#endif /* TM_IP_DEF_FRAG_RES */

#ifndef TM_IP_DEF_FRAG_TTL
/*
 * Fragment reassembly timeout:
 * time to live for fragments waiting in the reassembly queue in seconds.
 * Default is 64 seconds.
 */
#define TM_IP_DEF_FRAG_TTL         64
#endif /* TM_IP_DEF_FRAG_RES */

#ifndef TM_IP_DEF_TTL
/* Default IP datagram time to live */
#define TM_IP_DEF_TTL              64
#endif /* TM_IP_DEF_TTL */

#ifndef TM_IP_DEF_MULTICAST_TTL
/* Default IP Multicast datagrams time to live */
#define TM_IP_DEF_MULTICAST_TTL    1
#endif /* TM_IP_DEF_TTL */

#ifndef TM_IGMP_ROUTER_V1_PRESENT
/* Default IGMP version 1 router present timeout (400 seconds) */
#define TM_IGMP_ROUTER_V1_TIMEOUT  400
#endif /* TM_IGMP_ROUTER_V1_PRESENT */

#ifndef TM_IGMP_UNSOL_REPORT_INTV
/* Default unsolicited report interval in seconds (10 seconds) . */
#define TM_IGMP_UNSOL_REPORT_INTV  10
#endif /* TM_IGMP_UNSOL_REPORT_INTV */

#ifndef TM_DEF_ENET_MCAST_ENTRIES
/*
 * Default maximum number of Ethernet multicast entries that can be
 * allocated at a time.
 */
#define TM_DEF_ENET_MCAST_ENTRIES         (TM_BUF_Q1_SIZE/8)
#endif /* TM_DEF_ENET_MCAST_ENTRIES */

#ifndef TM_DEF_ENET_MCAST_ALLOCATIONS
/*
 * Default number of times that TM_DEF_ENET_MCAST_ENTRIES Ethernet multicast
 * entries can be allocated.
 */
#define TM_DEF_ENET_MCAST_ALLOCATIONS     2
#endif /* TM_DEF_ENET_MCAST_ALLOCATIONS */

#ifndef TM_IP_DEF_TOS
/* Default tos */
#define TM_IP_DEF_TOS              0
#endif /* TM_IP_DEF_TOS */

#ifndef TM_IP_ROUTER_ID
#define TM_IP_ROUTER_ID            (tt4IpAddress)TM_UL(0x0)
#endif /* TM_IP_ROUTER_ID */

#ifndef TM_RT_TIMEOUT
/* Default timout value of RIP, REDIRECT, entries */
#define TM_RT_TIMEOUT              TM_UL(180000) /* 3 minutes in milliseconds */
#endif /* TM_RT_TIMEOUT */

/*
 * User can remove all Path MTU discovery code by defining
 * TM_DISABLE_PMTU_DISC in trsystem.h.
 */
#if (!defined(TM_DISABLE_PMTU_DISC) && defined(TM_USE_IPV4))
#define TM_PMTU_DISC
#endif /* !TM_DISABLE_PMTU_DISC && TM_USE_IPV4 */

#ifndef TM_RT_DECREASED_MTU_TIMEOUT
/*
 * Default timout value of path MTU discovery entries when the path MTU
 * estimate is decreased
 */
#define TM_RT_DECREASED_MTU_TIMEOUT    TM_UL(600000) /* 10 mins (millisecs) */
#endif /* TM_RT_DECREASED_MTU_TIMEOUT */

#ifndef TM_RT_LARGER_MTU_TIMEOUT
/*
 * Default timout value of path MTU discovery entries when a larger path
 * MTU estimate is attempted
 */
#define TM_RT_LARGER_MTU_TIMEOUT   TM_UL(120000) /* 2 minutes in milliseconds */
#endif /* TM_RT_LARGER_MTU_TIMEOUT */

#ifndef TM_RT_MAX_ENTRIES
/* Default maximum number of dynamic routing entries */
#define TM_RT_MAX_ENTRIES              10
#endif /* TM_RT_MAX_ENTRIES */

#ifndef TM_ICMP_DEF_ADDRMAGENT
/* Default is no address mask agent */
#define TM_ICMP_DEF_ADDRMAGENT      0
#endif /* TM_ICMP_DEF_ADDRMAGENT */

#ifndef TM_UDP_DEF_CHECKSUM
/* Default is UDP checksum on */
#define TM_UDP_DEF_CHECKSUM         1
#endif /* TM_UDP_DEF_CHECKSUM */

#ifndef TM_UDP_DEF_INCOMING_CHECKSUM
/* UDP Incoming checksum check turned on by default */
#define TM_UDP_DEF_INCOMING_CHECKSUM         1
#endif /* TM_UDP_DEF_INCOMING_CHECKSUM */

#ifndef TM_TCP_DEF_INCOMING_CHECKSUM
/* TCP Incoming checksum check turned on by default */
#define TM_TCP_DEF_INCOMING_CHECKSUM         1
#endif /* TM_TCP_DEF_INCOMING_CHECKSUM */

#ifndef TM_10M_ETHER_LINESPEED
#define TM_10M_ETHER_LINESPEED   TM_L(10000000)
#endif /* TM_10M_ETHER_LINESPEED */

#ifndef TM_100M_ETHER_LINESPEED
#define TM_100M_ETHER_LINESPEED TM_L(100000000)
#endif /* TM_100M_ETHER_LINESPEED */

#ifndef TM_PAK_LL_MAX_HDR_LEN
/*
 * Maximum link layer header size (rounded up to a multiple of 4). Note that
 * this is a packed byte count. Currently, the Ethernet link-layer frame
 * header is stored unpacked.
 */
#ifdef TM_USE_VCHAN
#define TM_PAK_LL_MAX_HDR_LEN              20
#else /* !TM_USE_VCHAN */
#define TM_PAK_LL_MAX_HDR_LEN              16
#endif /* !TM_USE_VCHAN */
#endif /* TM_PAK_LL_MAX_HDR_LEN */

#ifndef TM_MAX_LINK_LAYER_NAME
#define TM_MAX_LINK_LAYER_NAME 14 /* make it a multiple of 2 */
#endif /* TM_MAX_LINK_LAYER_NAME */

#ifndef TM_MAX_IPS_PER_IF
#ifdef TM_SINGLE_INTERFACE_HOME
#define TM_MAX_IPS_PER_IF       1 /* no multi homing */
#else /* !TM_SINGLE_INTERFACE_HOME */
#define TM_MAX_IPS_PER_IF       4 /* make it a multiple of 4 */
#endif /* TM_SINGLE_INTERFACE_HOME */
#endif /* TM_MAX_IPS_PER_IF */

#ifdef TM_SINGLE_INTERFACE_HOME
/* maximum number of interfaces that can be added per context */
#define TM_MAX_NUM_IFS 1
#else /* ! TM_SINGLE_INTERFACE_HOME */
#ifndef TM_MAX_NUM_IFS
#define TM_MAX_NUM_IFS 255
#endif /* ! TM_MAX_NUM_IFS */
#endif /* ! TM_SINGLE_INTERFACE_HOME */

#ifndef TM_MAX_USER_DHCP_ENTRIES
#define TM_MAX_USER_DHCP_ENTRIES   0
#endif /* TM_MAX_USER_DHCP_ENTRIES */

#ifndef TM_MAX_PHYS_ADDR
#define TM_MAX_PHYS_ADDR        6
#endif /* TM_MAX_PHYS_ADDR */

#ifndef TM_MAX_DEVICE_NAME
#define TM_MAX_DEVICE_NAME     14 /* make it a multiple of 2 */
#endif /* TM_MAX_DEVICE_NAME */

#ifndef TM_SOC_INDEX_MAX
#define TM_SOC_INDEX_MAX       64
#endif /*TM_SOC_INDEX_MAX*/

#ifndef TM_TCPVECT_MAX_COUNT
#define TM_TCPVECT_MAX_COUNT   TM_SOC_INDEX_MAX
#endif /*TM_TCPVECT_MAX_COUNT*/
    
#ifndef TM_SOC_RECV_Q_BYTES
/* Default recv queue size in bytes */
#define TM_SOC_RECV_Q_BYTES  8192
#endif /* TM_SOC_RECV_Q_BYTES */

#ifndef TM_SOC_SEND_Q_BYTES
/* Default send queue size in bytes */
#define TM_SOC_SEND_Q_BYTES  8192
#endif /* TM_SOC_SEND_Q_BYTES */

#ifndef TM_SOC_RECV_Q_DGRAMS
/* Default recv queue size in datagrams (non-TCP) */
#define TM_SOC_RECV_Q_DGRAMS  8
#endif /* TM_SOC_RECV_Q_DGRAMS */

#ifndef TM_SOC_SEND_Q_DGRAMS
/* Default send queue size in datagrams (non-TCP) */
#define TM_SOC_SEND_Q_DGRAMS  8
#endif /* TM_SOC_SEND_Q_DGRAMS */

#ifndef TM_SOC_SEND_OOB_Q_BYTES
/*
 * Default out of band data queue size (beyond the default send queue size in
 * bytes).
 */
#define TM_SOC_SEND_OOB_Q_BYTES 512
#endif /* TM_SOC_SEND_OOB_DATA */

#ifndef TM_SOC_SEND_APPEND_THRESHOLD
/*
 * Threshold in bytes below which we try and append to previous send buffer
 * for TCP
 */
#define TM_SOC_SEND_APPEND_THRESHOLD 128
#endif /* TM_SOC_SEND_APPEND_THRESHOLD */

#ifndef TM_SOC_RECV_APPEND_FRACTION
/*
 * Fraction use of the buffer below which we try and append to previous
 * recv buffer for TCP
 */
#define TM_SOC_RECV_APPEND_FRACTION 4 /* 25 % */
#endif /* TM_SOC_APPEND_FRACTION */

#ifndef TM_SOC_RECV_COPY_FRACTION
/*
 * Fraction use of the buffer below which we try and copy to a smaller
 * recv buffer for UDP
 */
#define TM_SOC_RECV_COPY_FRACTION   4 /* 25% */
#endif /* TM_SOC_COPY_FRACTION */

#ifndef TM_SOC_LINGER_TIME
/* Default linger time in seconds when closing a TCP socket */
#define TM_SOC_LINGER_TIME           60      /* 60 seconds */
#endif

#ifndef TM_SOC_DEF_OPTIONS
/* By default, no socket options are enabled */
#define TM_SOC_DEF_OPTIONS 0
#endif /* TM_SOC_DEF_OPTIONS */

/*
 * ARP configuration macros
 */
#ifndef TM_ARP_MAX_ENTRIES
/* Maximum number of ARP entries */
#define TM_ARP_MAX_ENTRIES           64
#endif /* TM_ARP_MAX_ENTRIES */

#ifndef TM_ARP_TIMEOUT
/* Arp entry time to live, in milliseconds (about 10 minutes) */
#define TM_ARP_TIMEOUT          TM_UL(600000)
#endif /* TM_ARP_TIMEOUT */

#ifndef TM_ARP_RESOLUTION
/* Arp Timer resolution, in milliseconds (about 1 minute) */
#define TM_ARP_RESOLUTION       TM_UL(60000)
#endif /* TM_ARP_TIMEOUT */

#ifndef TM_ARP_DEF_SMART
/* ARP Cache Smart */
/*
 * ARP smart cache option, that allows us to store all ARP mappings even
 * the ones we did not request, is on by default.
 */
#define TM_ARP_DEF_SMART        TM_8BIT_YES
#endif /* TM_ARP_DEF_SMART */

/* time to live for permanent entry */
#define TM_ARP_INFINITE_TIMEOUT TM_UL(0x7FFFFFFF)

#define TM_ARP_REQUEST_TRIES    5 /* Lets try five times */
#define TM_ARP_QUIET_TIME       TM_UL(20000)
#define TM_ARP_RETRY_TIME       TM_UL(1000)

/*
 * TFTP macros
 */
#ifndef TM_DISABLE_TFTP
/* Use TFTP */
#endif /* TM_DISABLE_TFTP */

#ifndef TM_DISABLE_TFTPD
/* Use TFTPD */
#endif /* TM_DISABLE_TFTPD */

/*
 * TCP configuration macros
 */
#ifndef TM_DISABLE_TCP
/*
 * User can remove the TCP code by defining TM_DISABLE_TCP in trsystem.h
 */
#define TM_USE_TCP

#else /* TM_DISABLE_TCP */

/* If TCP is disabled, then so are FTP, FTPD, HTTPD, SSL, and TELNETD */
#ifndef TM_DISABLE_FTP
#define TM_DISABLE_FTP
#endif /* !TM_DISABLE_FTP */

#ifndef TM_DISABLE_FTPD
#define TM_DISABLE_FTPD
#endif /* !TM_DISABLE_FTPD */




#ifdef TM_USE_SSL
#undef TM_USE_SSL
#endif /* TM_USE_SSL */



#ifndef TM_DISABLE_TELNETD
#define TM_DISABLE_TELNETD
#endif /* !TM_DISABLE_TELNETD */

#endif /* TM_DISABLE_TCP */

#ifndef TM_DISABLE_FTP
/*
 * User can remove the FTP code by defining TM_DISABLE_FTP in trsystem.h
 */
#endif /* !TM_DISABLE_FTP */

#ifndef TM_DISABLE_FTPD
/*
 * User can remove the FTPD code by defining TM_DISABLE_FTPD in trsystem.h
 */
#endif /* !TM_DISABLE_FTPD */

#ifndef TM_DISABLE_TELNETD
/*
 * User can remove the TELNETD code by defining TM_DISABLE_TELNETD in
 * trsystem.h.
 */
#endif /* !TM_DISABLE_TELNETD */

#ifndef TM_DISABLE_TCP_SACK
#ifndef TM_USE_TCP_PACKET
/*
 * User can remove the TCP Selective Acknowlegment Options (RFC2018) code by
 * defining TM_DISABLE_TCP_SACK in trsystem.h.
 * Note that if the user wants to use the TM_TCP_PACKET option, then
 * SACK is also disabled.
 */
#define TM_TCP_SACK
#endif /* TM_USE_TCP_PACKET */
#endif /* TM_DISABLE_TCP_SACK */

#ifndef TM_DISABLE_TCP_RFC2414
/*
 * User can remove the TCP Initial Send Window Increase as described in
 * RFC2414 , by defining TM_DISABLE_TCP_RFC2414 in trsystem.h.
 */
#define TM_TCP_RFC2414
#endif /* TM_DISABLE_TCP_RFC2414 */

#ifndef TM_DISABLE_TCP_RFC2581
/*
 * User can remove the TCP congestion control update to RFC2001 as described in
 * RFC2581 , by defining TM_DISABLE_TCP_RFC2581 in trsystem.h.
 */
#endif /* TM_DISABLE_TCP_RFC2581 */

#ifndef TM_DISABLE_TCP_RFC3042
/*
 * User can remove the Limited Transmit Algorithm that enhances TCP's Loss
 * Recovery as described in RFC3042 , by defining TM_DISABLE_TCP_RFC3042 in
 * trsystem.h.
 */
#endif /* TM_DISABLE_TCP_RFC3042 */

#ifndef TM_DISABLE_TCP_ACK_PUSH
/*
 * By default, the TCP/IP stack sends an immediate ACK, when receiving a
 * PUSH segment. To disable that behaviour, define TM_DISABLE_TCP_ACK_PUSH
 * in trsystem.h. Note that if the peer is running Windows 2000, do not
 * disable that behaviour.
 */
#define TM_TCP_ACK_PUSH
#endif /* TM_DISABLE_TCP_ACK_PUSH */

#ifndef TM_TCP_DEF_MSS
/* Today, every router should be able to handle 1500 bytes IP MTU */
#define TM_TCP_DEF_MSS          1460 /* 1500 - size of IP and TCP headers */
#endif /* TM_TCP_DEF_MSS */

#ifndef TM_TCP_DEF_OPTIONS
/*
 * Can set TCP_NODELAY, TCP_NOPUSH, TCP_NOOPT, TM_TCP_SEL_ACK
 * TM_TCP_WND_SCALE, TM_TCP_TS, TM_TCP_SLOW_START
 */
#define TM_TCP_DEF_OPTIONS   (TM_TCP_SLOW_START | TCP_STDURG  \
                              | TM_TCP_SEL_ACK                  \
                              | TM_TCP_TS                       \
                              | TM_TCP_WND_SCALE)
#endif /* TM_TCP_DEF_OPT */

#ifndef TM_TCP_RESOLUTION
/*
 * Resolution in milli-seconds of the global TCP timer used to increase
 * ISS
 */
#define TM_TCP_RESOLUTION           TM_UL(500) /* in milli-seconds */
#endif

#ifndef TM_TCP_DELAY_ACK_TIME
/* Delay ACK timer */
#define TM_TCP_DELAY_ACK_TIME       TM_UL(200) /* Default 200 milli-seconds */
#endif

#ifndef TM_TCP_ISS_INCR
/* Default value is 1 for every 4 us or 250 for every millisecond */
#define TM_TCP_ISS_INCR             TM_UL(250) /* ISS increase by millisecond */
#endif /* TM_TCP_ISS_INCR */

#ifndef TM_TCP_CONN_TIME
/* Connection establishment time in milliseconds */
#define TM_TCP_CONN_TIME            TM_UL(75000) /* 75 secs (in milliseconds) */
#endif /* TM_TCP_CONN_TIME */

#ifndef TM_TCP_DEF_RTO_TIME
/* Default time out value in milleseconds */
#define TM_TCP_DEF_RTO_TIME         TM_UL(3000) /* 3 secs (in milliseconds) */
#endif /* TM_TCP_DEF_RTT_TIME */

#ifndef TM_TCP_MIN_RTO_TIME
/* Minimum time out value in milliseconds */
#define TM_TCP_MIN_RTO_TIME         TM_UL(100)      /* 100 ms */
#endif /* TM_TCP_MIN_RTO_TIME */


#ifndef TM_TCP_MAX_RTO_TIME
/* Maximum time out value in milliseconds */
/* TM_TCP_MAX_RTO_TIME == 2MSL == 60 seconds in millisecs */
#define TM_TCP_MAX_RTO_TIME         TM_UL(60000)
#endif /* TM_TCP_MAX_RTO_TIME */

#ifndef TM_TCP_MAX_REXMIT_CNT
/* Maximum number of retransmissions */
#define TM_TCP_MAX_REXMIT_CNT       12 /* Maximum number of retries */
#endif /* TM_TCP_MAX_REXMIT_CNT */

#ifndef TM_TCP_MIN_PROBEWINDOW_TIME
/* Zero window probe min interval time in milliseconds */
#define TM_TCP_MIN_PROBEWINDOW_TIME 500 /* 500 milliseconds */
#endif /* TM_TCP_MIN_PROBEWINDOW_TIME */

#ifndef TM_TCP_MAX_PROBEWINDOW_TIME
/* Zero window probe max interval time in milliseconds */
#define TM_TCP_MAX_PROBEWINDOW_TIME TM_UL(60000) /* 60 seconds in millisecs */
#endif /* TM_TCP_MAX_PROBEWINDOW_TIME */

#ifndef TM_TCP_FINWAIT2_TIME
/* FIN WAIT 2 time in seconds */
#define TM_TCP_FINWAIT2_TIME        600 /* 10 minutes (in seconds) */
#endif /* TM_TCP_FINWAIT2_TIME */

#ifndef TM_TCP_2MSL_TIME
/*
 * TIME wait time in milliseconds
 * 2 * Maximum segment life time (2 * 30 seconds) (TIME WAIT timer) in
 * milliseconds
 */
#define TM_TCP_2MSL_TIME            60 /* 1 minute (in seconds) */
#endif /* TM_TCP_2MSL_TIME */

#ifndef TM_TCP_KEEPALIVE_TIME
/* Keep alive idle time (before starting sending probes) in seconds */
#define TM_TCP_KEEPALIVE_TIME       7200 /* 2 hours (in seconds) */
#endif /* TM_TCP_KEEPALIVE_TIME */

#ifndef TM_TCP_IDLE_INTV
/* Idle interval. Used for keep alive probe interval, and idle time updates */
#define TM_TCP_IDLE_INTV            75  /* 75 seconds */
#endif /* TM_TCP_IDLE_INTV */

#ifndef TM_TCP_KEEPALIVE_PROBECNT
/* Keep alive probe count */
#define TM_TCP_KEEPALIVE_PROBECNT      8
#endif /* TM_TCP_KEEPALIVE_PROBECNT */

#ifndef TM_TCP_SWS_FS
/* Sender silly window syndrome fraction (recommended value 2) */
/* 7/1 98 changed to 4 */
#define TM_TCP_SWS_FS                  4 /* we are using 4 */
#endif /* TM_TCP_SWS_FS */

#ifndef TM_TCP_SWS_FR
/* Receiver silly window syndrome fraction (recommended value 2) */
/* ANVL 14.20: changed from 4 to 2 as recommended by the RFC */
#define TM_TCP_SWS_FR                  2
#endif /* TM_TCP_SWS_FR */

#ifndef TM_RAW_IP_PORT
/* Use for raw socket, to ensure only one raw socket per protocol */
#define TM_RAW_IP_PORT                 0x7FFF
#endif /* TM_RAW_IP_PORT */

/*
 * DNS resolver macros
 */
#ifndef TM_DNS_MAX_SERVERS
/* Maximum number of DNS servers in server list */
#define TM_DNS_MAX_SERVERS 2
#endif /* TM_DNS_MAX_SERVERS */

#ifndef TM_DNS_MAX_TTL
/* Maximum TTL value that we will use */
#define TM_DNS_MAX_TTL TM_UL(4294967)
#endif /* TM_DNS_MAX_TTL */

#ifndef TM_DEF_NAT_MAX_ENTRIES
/* Default maximum number of NAT entries */
#define TM_DEF_NAT_MAX_ENTRIES    64
#endif /* TM_DEF_NAT_MAX_ENTRIES */

/*
 * Macros to define in line assembly keywords for different compilers
 */

#ifdef _MSC_VER
#if     _MSC_VER >= 800
#define ASM1    __asm
#define ASM2    __asm
#else
#define ASM1    _asm
#define ASM2    _asm
#endif /* >= 800 */
#define LEFTSQ   {
#define RIGHTSQ  }
#define SEMICOL
#endif /* _MSC_VER */


#ifdef __BORLANDC__
#define ASM1    __asm
#define ASM2
#define LEFTSQ
#define RIGHTSQ
#define SEMICOL ;
#endif /* __BORLANDC__ */

#ifdef TM_X86_ASM_CRITICAL
/* Microsoft Visual C++/Borland compiler inline critical sections */

#ifndef tm_kernel_set_critical
#define tm_kernel_set_critical          ASM1 LEFTSQ ASM2 cli RIGHTSQ
#endif /* tm_kernel_set_critical */

#ifndef tm_kernel_release_critical
#define tm_kernel_release_critical      ASM1 LEFTSQ ASM2 sti RIGHTSQ
#endif /* tm_kernel_release_critical */

#endif /* TM_X86_ASM_CRITICAL */


#ifdef TM_SDS68K_ASM_CRITICAL

#ifndef tm_kernel_set_critical
#define tm_kernel_set_critical  asm(" ori #0x700,SR")
#endif /* tm_kernel_set_critical */

#ifndef tm_kernel_release_critical
#define tm_kernel_release_critical asm(" andi.w #~0x700,SR")
#endif /* tm_kernel_release_critical */

#endif /* TM_SDS68K_ASM_CRITICAL */


#ifdef TM_MRI_ASM_CRITICAL

#ifndef tm_kernel_set_critical
#define tm_kernel_set_critical  asm(" ori #$0700,SR")
#endif /* tm_kernel_set_critical */

#ifndef tm_kernel_release_critical
#define tm_kernel_release_critical asm(" andi.w #$f8ff,SR")
#endif /* tm_kernel_release_critical */

#endif /* TM_MRI_ASM_CRITICAL */


#ifdef TM_DDIPPC_ASM_CRITICAL

#ifndef tm_kernel_set_critical
asm void tm_asm_set_critical (void)
{
    mfmsr   r12            # get the MSR (Machine Status Register)
    xor     r11,r11,r11    # Clear R11
    ori     r11,r11,0x8000 # set the EE Bit (External Interrupt Enable) in r11
    andc    r12,r12,r11    # and r11 with r12 (Clearing the EE Bit)
    mtmsr   r12            # Save the MSR
}
#define tm_kernel_set_critical  tm_asm_set_critical()
#endif /* tm_kernel_set_critical */

#ifndef tm_kernel_release_critical
asm void tm_asm_release_critical (void)
{
    mfmsr   r12              # get the MSR (Machine Status Register)
    ori     r12,r12,0x8000   # set the EE Bit in r12
    mtmsr   r12              # save the MSR
}
#define tm_kernel_release_critical  tm_asm_release_critical()
#endif /* tm_kernel_release_critical */

#endif /* TM_DDIPPC_ASM_CRITICAL */

#ifdef TM_GHSPPC_ASM_CRITICAL

#ifndef tm_kernel_set_critical
asm void tm_ppc_disable_ints(void)
{
%
        mfmsr   r12
        xor     r11,r11,r11
        ori     r11,r11,0x8000
        andc    r12,r12,r11
        mtmsr   r12
}
#define tm_kernel_set_critical tm_ppc_disable_ints()
#endif /* tm_kernel_set_critical */

#ifndef tm_kernel_release_critical
asm void tm_ppc_enable_ints(void)
{
%
        mfmsr   r12
        ori     r12,r12,0x8000
        mtmsr   r12
}
#define tm_kernel_release_critical tm_ppc_enable_ints()
#endif /* tm_kernel_release_critical */

#endif /* TM_GHSPPC_ASM_CRITICAL */

#ifdef TM_GNUPPC_ASM_CRITICAL

#define tm_kernel_set_critical \
 __asm__ __volatile__ \
  ("mfmsr r12\n xor r11,r11,r11\n ori r11,r11,0x8000\n andc r12,r12,r11\n mtmsr r12\n" \
   : : : "r11", "r12")

#define tm_kernel_release_critical \
 __asm__ __volatile__ \
  ("mfmsr r12\n ori r12,r12,0x8000\n mtmsr r12\n" \
   : : : "r12")

#endif /* TM_GNUPPC_ASM_CRITICAL */

#ifdef TM_GHSARM7_ASM_CRITICAL

#ifndef tm_kernel_set_critical
asm void tm_arm7_disable_ints(void)
{
%
    MRS r0,CPSR
    ORR r1,r0,0x80
    MSR CPSR_cxsf,r1
}
#define tm_kernel_set_critical tm_arm7_disable_ints()
#endif /* tm_kernel_set_critical */

#ifndef tm_kernel_release_critical
asm void tm_arm7_enable_ints(void)
{
%
    MRS r0,CPSR
    AND r1,r0,0x3F
    MSR CPSR_cxsf,r1
}
#define tm_kernel_release_critical tm_arm7_enable_ints()
#endif /* tm_kernel_release_critical */

#endif /* TM_GHSARM7_ASM_CRITICAL */

#ifdef TM_GNUMIPS4K_ASM_CRITICAL

#ifndef tm_kernel_set_critical
#define tm_kernel_set_critical \
 __asm__ volatile        \
  ("mfc0 $2,$12\n nop\n li $8,-2\n and $2,$2,$8\n mtc0 $2,$12")
#endif /* tm_kernel_set_critical */

#ifndef tm_kernel_release_critical
#define tm_kernel_release_critical \
 __asm__ volatile        \
  ("mfc0 $2,$12\n nop\n ori $2,$2,0x1\n mtc0 $2,$12")
#endif /* tm_kernel_release_critical */

#endif /* TM_GNUMIPS4K_ASM_CRITICAL */

/* critical functions */
#ifndef tm_kernel_set_critical
#define TM_KERNEL_FUNC_SET_CRITICAL
#define tm_kernel_set_critical  tfKernelSetCritical()
#endif /* tm_kernel_set_critical */

#ifndef tm_kernel_release_critical
#define TM_KERNEL_FUNC_RELEASE_CRITICAL
#define tm_kernel_release_critical tfKernelReleaseCritical()
#endif /* tm_kernel_release_critical */

/*
 * Constant node to host and host to node byte order conversion.
 * To be used to initialize constants. Evaluated at compile time.
 */

#ifndef tm_const_ntohl
#ifdef TM_LITTLE_ENDIAN
#define tm_const_ntohl(x)  LINT_ZERO_OVFL_EXS_CONST \
     (tt32Bit)((tt32Bit)(((x)>>24)&TM_UL(0xff))\
         |(tt32Bit)(((x)>>8)&TM_UL(0xff00))\
         |(tt32Bit)(((x & 0xffffff)<<8)&TM_UL(0xff0000))\
         |(tt32Bit)(((x & 0xff)<<24)&TM_UL(0xff000000)))
#else /* TM_LITTLE_ENDIAN */
#define tm_const_ntohl(x) (tt32Bit)(x)
#endif /* TM_LITTLE_ENDIAN */
#endif /* tm_const_ntohl */

#ifndef tm_const_ntohs
#ifdef TM_LITTLE_ENDIAN
#define tm_const_ntohs(x) LINT_EXSH_ZERO_BEGIN \
        (tt16Bit)((tt16Bit)(((x)>>8)&0xffU) \
              |(tt16Bit)(((x)<<8)&0xff00U)) \
LINT_RESTORE

#else /* TM_LITTLE_ENDIAN */
#define tm_const_ntohs(x) (tt16Bit)(x)
#endif /* TM_LITTLE_ENDIAN */
#endif /* tm_const_ntohs */

#ifndef tm_const_htonl
#define tm_const_htonl(x) tm_const_ntohl(x)
#endif /* tm_const_htonl */

#ifndef tm_const_htons
#define tm_const_htons(x) tm_const_ntohs(x)
#endif /* tm_const_htons */

/*
 * Non Constant node to host and host to node byte order conversion.
 * 'C' version and in-line assembly versions.
 */

#ifndef tm_ntohl

#ifdef TM_C_SWAP
#define tm_ntohl(from,to) to=ntohl(from)
#define tm_htonl(from,to) to=htonl(from)
#endif /* TM_C_SWAP */

#ifdef TM_X386_ASM_SWAP
/* Microsoft Visual C++/Borland compiler */
#define tm_ntohl(from,to) \
{ \
    tt32Bit temp; \
    temp = from; \
    ASM1 { \
        ASM2 mov eax,temp SEMICOL \
        ASM2 ror ax,8 SEMICOL \
        ASM2 ror eax,16 SEMICOL \
        ASM2 ror ax,8 SEMICOL \
        ASM2 mov to,eax SEMICOL \
    } \
}

#define tm_htonl(from,to) \
{ \
    tt32Bit temp; \
    ASM1 { \
        ASM2 mov eax,from SEMICOL \
        ASM2 ror ax,8 SEMICOL \
        ASM2 ror eax,16 SEMICOL \
        ASM2 ror ax,8 SEMICOL \
        ASM2 mov temp,eax SEMICOL \
    } \
    to = temp; \
}
#endif /* TM_X386_ASM_SWAP */


#ifdef TM_INTEL_GNU_ASM_SWAP /* Taken from BSD 4.4 i386\endian.h */

#define tm_ntohl(from,to) \
({ register u_long __X = (from); \
   __asm ("rorw $8, %w1\n\trorl $16, %1\n\trorw $8, %w1" \
    : "=r" (__X) \
    : "0" (__X)); \
   to = __X; })

#ifdef TM_KERNEL_LINUX
#define tm_htonl(from,to) \
({ register u_long __X = (from); \
   __asm ("rorw $8, %w1\n\trorl $16, %1\n\trorw $8, %w1" \
    : "=r" (__X) \
    : "0" (__X)); \
   to = __X; })
#endif /* TM_KERNEL_LINUX */

#endif /* TM_INTEL_GNU_ASM_SWAP */

#ifndef tm_ntohl
#define tm_ntohl(from,to) (to=ntohl(from))
#define tm_htonl(from,to) (to=htonl(from))
#endif /* tm_endian_long */

#endif /* tm_ntohl */


#ifndef tm_ntohs



/* Microsoft Visual C++/Borland Compiler */

#ifdef TM_X386_ASM_SWAP
#define TM_X86_ASM_SWAP
#endif /* TM_MSC386_ASM_SWAP */
#ifdef TM_X186_ASM_SWAP
#define TM_X86_ASM_SWAP
#endif /* TM_MSC186_ASM_SWAP */

#ifdef TM_X86_ASM_SWAP
#define tm_ntohs(from, to) LINT_ACCE_INIT_BEGIN \
{ \
register    tt16Bit temp; \
    temp = (tt16Bit)from; \
    ASM1 { \
        ASM2 mov ax,temp SEMICOL \
        ASM2 ror ax,8 SEMICOL \
        ASM2 mov to,ax SEMICOL \
    } \
} LINT_RESTORE

#define tm_htons(from, to) LINT_ACCE_INIT_BEGIN \
{ \
register    tt16Bit temp; \
    ASM1 { \
        ASM2 mov ax,from SEMICOL \
        ASM2 ror ax,8 SEMICOL \
        ASM2 mov temp,ax SEMICOL \
    } \
    to = temp; \
} LINT_RESTORE
#endif /* TM_X86_ASM_SWAP */


#ifdef TM_INTEL_GNU_ASM_SWAP /* Taken from BSD 4.4 i386\endian.h */
#define tm_ntohs(from, to) \
({ register u_short __X = (from); \
   __asm ("rorw $8, %w1" \
    : "=r" (__X) \
    : "0" (__X)); \
   to = __X; })
#define tm_htons(from,to)   tm_ntohs(from, to)
#endif /* TM_INTEL_GNU_ASM_SWAP */

#ifndef tm_ntohs
#define TM_C_SWAP
#endif /* tm_ntohs */

#ifdef TM_C_SWAP
#define tm_ntohs(from,to) to=ntohs(from)
#define tm_htons(from,to) to=htons(from)
#endif /* TM_C_SWAP */

#endif /* tm_ntohs */

/* Here it makes sence to breach the rules of not to use trigraph syntax */
#define tm_max(a,b)   ((a)>(b)?(a):(b))
#define tm_min(a,b)   ((a)<(b)?(a):(b))
#define tm_between_min_and_max(x, min, max)    ((x)>=(min) && (x)<=(max))

#define TM_NET_ENDIAN_LONG    tm_const_htonl(TM_UL(0x01020304))
#define TM_NET_ENDIAN_FIRST   (tt8Bit)0x01


/*
 * Buffer allocation macros
 */
#define TM_ROUND_SIZE               TM_L(3)
#define TM_ROUND_MASK_SIZE          ~(TM_ROUND_SIZE)

#define TM_ROUND_PTR                TM_L(3)
#define TM_ROUND_MASK_PTR           ~(TM_ROUND_PTR)

/*
 * Layers 2-4 header sizes
 */
#define TM_MAX_TCP_HDR_LEN       64 /* Maximum TCP header with options */
#define TM_MAX_UDP_HDR_LEN       8 /* UDP header */

/*  Maximum IP header size (including options) (60) */
#define TM_4_IP_MAX_HDR_LEN      60 /* IPv4 with IP options */
#define TM_4PAK_IP_MAX_HDR_LEN   tm_packed_byte_count(TM_4_IP_MAX_HDR_LEN)

/*  Minimum IP header size (20 bytes or sizeof(ttIpHeader))  */
#define TM_4_IP_MIN_HDR_LEN      20 /* IPv4 without IP options */
#define TM_4PAK_IP_MIN_HDR_LEN   tm_packed_byte_count(TM_4_IP_MIN_HDR_LEN)

/* The minimum number of bytes in an IPv6 header. */
#define TM_6_IP_MIN_HDR_LEN      40
#define TM_6PAK_IP_MIN_HDR_LEN   tm_packed_byte_count(TM_6_IP_MIN_HDR_LEN)

/* length of Mobile IPv6 Home Address destination option */
#define TM_6_HOME_ADDR_OPT_MIN_LEN      24
#define TM_6PAK_HOME_ADDR_OPT_MIN_LEN \
  tm_packed_byte_count(TM_6_HOME_ADDR_OPT_MIN_LEN)
/* length of Mobile IPv6 Type 2 Routing Header */
#define TM_6_TYPE_2_ROUTING_HDR_LEN     24
#define TM_6PAK_TYPE_2_ROUTING_HDR_LEN \
  tm_packed_byte_count(TM_6_TYPE_2_ROUTING_HDR_LEN)

/* AH and ESP header size (IPsec option) */
#define TM_MAX_AH_HDR_LEN               24
#define TM_MAX_ESP_HDR_LEN              24
#define TM_AH_HEADER_SIZE               24
/*need to consider IV seperately*/
#define TM_ESP_HEADER_SIZE              8
/* authentication exists in ESP, add 12 bytes to the end*/
#define TM_ESP_AH_TRAILER_SIZE          12
#define TM_IPSEC_MAX_BLOCK_SIZE         16
#define TM_ESP_MAX_IV_LEN               16

/* Offset in bytes to the ICMPv6 checksum from the beginning of ICMPv6 header*/
#define TM_6_ICMP_BYTE_XSUM_OFFSET  2
/* Offset in bytes to the TCP checksum from the beginning of TCP header */
#define TM_UDP_BYTE_XSUM_OFFSET     6
/* Offset in bytes to the UDP checksum from the beginning of UDP header */
#define TM_TCP_BYTE_XSUM_OFFSET    16
/*
 * Offset in bytes to the IP checksum field from the beginning of the
 * IPv4 header
 */
#define TM_IP_BYTE_XSUM_OFFSET     10
/*
 * Maximum rounded link layer header
 * Note that it is a byte count for DSP, because the Ethernet header is
 * unpacked (unlike any other TCP/IP header). This will ensure that we
 * have enough room allocated for the Ethernet header.
 */
#define TM_LL_MAX_HDR_LEN        tm_byte_count(TM_PAK_LL_MAX_HDR_LEN)

/* no MIPv6 route optimization */
#define TM_6_MIP_MAX_HDR_LEN        0

/* Maximum TCP header with options */
#define TM_PAK_MAX_TCP_HDR          tm_packed_byte_count(TM_MAX_TCP_HDR_LEN)

/* UDP header */
#define TM_PAK_MAX_UDP_HDR          tm_packed_byte_count(TM_MAX_UDP_HDR_LEN)

/*
 * For incoming PPP data from the network (leave room for forwarding, and
 * VJ decompression)
 */
#define TM_MAX_HEADERS_SIZE \
        (TM_MAX_TCP_HDR_LEN+TM_4_IP_MAX_HDR_LEN+TM_LL_MAX_HDR_LEN)

#ifndef TM_DEF_SEND_TRAILER_SIZE
/*
 * Default send trailer size.
 */
#define TM_DEF_SEND_TRAILER_SIZE 0
#endif /* TM_DEF_SEND_TRAILER_SIZE */

/*
 * Raw buffers sizes in raw buffer queues.
 */

#define TM_BUF_QX_SIZE                32 /* 1 - 32 */
#define TM_BUF_Q0_SIZE                64 /* 33 - 64 */
#define TM_BUF_Q1_SIZE               128 /* 65 - 128 */
#define TM_BUF_Q2_SIZE               256 /* 129 - 256 */
#define TM_BUF_Q3_SIZE               512 /* 257 - 512 */
#define TM_BUF_Q4_SIZE              1024 /* 513 - 1024 */
#define TM_BUF_Q5_SIZE              2048 /* 1025 - 2048 */
#define TM_BUF_Q6_SIZE              4096 /* 2049 - 4096 */

/*
 * Recycle list index in tlRecycleArray[].
 */
#define TM_RECY_TIMER                   0
#define TM_RECY_PACKET                  1
#define TM_RECY_RTE                     2
#define TM_RECY_SOCKET                  3
#ifdef TM_USE_TCP
#define TM_RECY_TCPVEC                  4
#endif /* TM_USE_TCP */

/*
 * Shared data flags
 */
/* Shared data point to user application memory */
#define TM_BUF_USER_DATA            (tt16Bit)0x0010
/* Buffer is used for a (non TCP) send */
#define TM_BUF_USER_SEND            (tt16Bit)0x0020
/* Shared data point to user device memory */
#define TM_BUF_USER_DEVICE_DATA     (tt16Bit)0x0040
/* Buffer is in TCP reassembly queue */
#define TM_BUF_TCP_IN_QUEUE         (tt16Bit)0x0080
/* Out of band data in TCP buffer */
#define TM_BUF_TCP_OOB_BYTE         (tt16Bit)0x0100
/* TCP buffer Time stamp on */
#define TM_BUF_TCP_TS_ON            (tt16Bit)0x0200
/* Buffer is a recycled TCP header */
#define TM_BUF_HDR_RECYCLED         (tt16Bit)0x0400
/*
 * Buffer is a TCP header (with socketPtr valid)
 * NOTE: TM_BUF_SOCKETENTRYPTR must be 0x800 (to match
 * TM_TCPF_SOCKETENTRYPTR in trtcp.c)
 */
#define TM_BUF_SOCKETENTRYPTR       (tt16Bit)0x0800
/*
 * Buffer is a TCP header, and can be recycled
 * NOTE: TM_BUF_TCP_HDR_BLOCK must be 0x1000 (to match TM_TCPF_RECYCLE_HDR
 * in trtcp.c)
 */
#define TM_BUF_TCP_HDR_BLOCK        (tt16Bit)0x1000


/*
 * Flag value passed to Free packet routines
 */
#define TM_SOCKET_UNLOCKED  (tt8Bit)0 /* socket is unlocked */
#define TM_SOCKET_LOCKED    (tt8Bit)1 /* socket is locked */

/*
 * Flag value passed to tfSocketCheckValidLock(), indicating  whether
 * a socket need bo be checkedout.
 */
#define TM_SOCKET_CHECKEDOUT  (tt8Bit)0
#define TM_SOCKET_NOCHECKOUT  (tt8Bit)1

/*
 * Sockets Flags
 */
/* socket entry is in the array (accessible to the user) */
#define TM_SOCF_OPEN                 (tt16Bit)0x0001
/*
 * Socket is inserted in a tree (accessible to the layer 3/4 protocols).
 */
#define TM_SOCF_IN_TREE              (tt16Bit)0x0002
#define TM_SOCF_CONNECTED            (tt16Bit)0x0004
#define TM_SOCF_LINGER               (tt16Bit)0x0008
/* User has issued a close */
#define TM_SOCF_CLOSING              (tt16Bit)0x0010
/* Socket is bound */
#define TM_SOCF_BIND                 (tt16Bit)0x0020
/* NOTE: need to match MSG_WAITALL (0x0040) in trsocket.h */
#define TM_SOCF_WAITALL              (tt16Bit)MSG_WAITALL /* 0x0040 */
/* NOTE: need to match MSG_DONTWAIT (0x0080) in trsocket.h */
#define TM_SOCF_NONBLOCKING          (tt16Bit)MSG_DONTWAIT /* 0x0080 */
/* No more data can be put into recv queue */
#define TM_SOCF_NO_MORE_RECV_DATA    (tt16Bit)0x0100
/* No more data can be put into send queue */
#define TM_SOCF_NO_MORE_SEND_DATA    (tt16Bit)0x0200
/* Out-of-band data mark */
#define TM_SOCF_RCVATMARK            (tt16Bit)0x0400
/* Received end of file marker */
#define TM_SOCF_REOF                 (tt16Bit)0x0800
/* Use call back function with extra parameter */
#define TM_SOCF_CB_PARAM_ON          (tt16Bit)0x1000
/* Socket is a listening socket */
#define TM_SOCF_LISTENING            (tt16Bit)0x2000
/* socket is in a reuseaddress list */
#define TM_SOCF_REUSEADDR_LIST       (tt16Bit)0x4000
/* socket is disconnected */
#define TM_SOCF_DISCONNECTED         (tt16Bit)0x8000

#define TM_SOCF_MSG_WAIT_FLAGS       (tt16Bit)((tt16Bit)MSG_WAITALL| \
                                              (tt16Bit)MSG_DONTWAIT)

/*
 * Sockets Flags2
 */
/* Socket source IP address is set */
#define TM_SOCF2_IP_SRCADDR          (tt8Bit)0x01
/* Socket is connecting */
#define TM_SOCF2_CONNECTING          (tt8Bit)0x02
/* Bind was done on port zero with a non zero local IP address */
#define TM_SOCF2_BIND_PORT_ZERO      (tt8Bit)0x04
/* Socket traffic (if TCP) bypasses IPsec. We set this flag so that next
 * time when we want to send TCP data using this socket, we don't need
 * to query the IPsec policy every time.
 */
#define TM_SOCF2_IPSEC_BYPASS        (tt8Bit)0x08
/* Socket traffic (if TCP) will be discarded according to IPsec */
#define TM_SOCF2_IPSEC_DISCARD       (tt8Bit)0x10


/*
 * socket macros
 */
#define TM_SOC_NO_INDEX             (tt16Bit)0x7FFF /* invalid index */

/*
 * Socket RB macros
 */

#ifndef TM_SOC_TREE_BLOCKING_FACTOR
/*
 * Blocking factor used for allocating red-black trees.
 */
#define TM_SOC_TREE_BLOCKING_FACTOR     8
#endif /* TM_SOC_TREE_BLOCKING_FACTOR */

#define TM_SOC_TREE_MIN_BLOCKING_FACTOR 4

/*
 * Used in tfSocketTreeReInsert().
 * Indicate that we can re-insert the socket with random port number in the
 * tree.
 */
#define TM_SOC_TREE_RANDOM_PORT    (tt16Bit)0x0800
/*
 * Tree type flag. Used in tfxSocketLookup(), tfSocketTreeInsert(),
 * tfSocketTreeReInsert(), tfRandPortInsert(), and
 * in socCoFlags field of ttSocketEntry
 */
#define TM_SOC_TREE_CO_FLAG         (tt16Bit)0x1000
#define TM_SOC_TREE_CL_FLAG         (tt16Bit)0x2000
#define TM_SOC_TREE_ALL_FLAG                                               \
                        (tt16Bit)(TM_SOC_TREE_CO_FLAG | TM_SOC_TREE_CL_FLAG)
/*
 * Tree lock flag. Used in tfSocketTreeInsert(), tfSocketTreeDelete(),
 * and tfRandPortInsert.
 */
/* Indicate that the Tree is already locked (no need to lock it) */
#define TM_SOC_TREE_LOCKED_FLAG     (tt16Bit)0x4000
/* Indicate to leave the Tree locked upon return. */
#define TM_SOC_TREE_KEEP_LOCK_FLAG  (tt16Bit)0x8000
/*
 * Exact match flag. Used in tfxSocketLookup() to indicate that we
 * only want to find an exact local IP address match, if the reuse address
 * list is used.
 */
#define TM_SOC_TREE_EXACT_MATCH     (tt16Bit)0x4000
/*
 * Broadcast or Multicast match. Used in tfxSocketLookup().
 * Indicate that the packet's destination is broadcast or multicast.
*/
#define TM_SOC_TREE_M_B_CAST_MATCH  (tt16Bit)0x8000

/* Maximum number of connected-oriented trees */
#define TM_SOC_TREE_MAX_CO_INDEX  1
#define TM_SOC_TREE_INT_NO_INDEX  255
#define TM_SOC_TREE_NO_INDEX      (tt8Bit)TM_SOC_TREE_INT_NO_INDEX
#define TM_SOC_TREE_INDEX_ERROR    -1

/* Low Water Macros */
/* Used by send process */
#define tm_send_low_water(x) (x/TM_L(4))
#define tm_recv_low_water(x) (TM_L(1))  /* Receive is always 1 for now */

/* Receive queue size macro */
#define tm_soc_recv_left(socketPtr)                 \
   (socketPtr->socMaxRecvQueueBytes - socketPtr->socRecvQueueBytes)

/* Receive queue size macro */
#define tm_tcp_soc_recv_left(socketPtr, tcpVectPtr) \
   tm_soc_recv_left(socketPtr)


/*
 * Flag passed to tfSocketReturnAddr
 */
/* Retrieve peer address */
#define TM_ADDR_PEER_FLAG   (tt8Bit)0
/* Retrieve local address */
#define TM_ADDR_LOCAL_FLAG  (tt8Bit)1


/*
 * Select Flags
 */
#define TM_SELECT_READ      0x0001
#define TM_SELECT_WRITE     0x0002
#define TM_SELECT_EXCEPTION 0x0004

/*
 * Locking Macros
 */
#define TM_LOC_NOWAIT     0
#define TM_LOC_WAIT       1

#define TM_LOC_OKAY       0
#define TM_LOC_WOULDBLOCK 1
#define TM_LOC_INCOMPLETE 2


#ifdef TM_LOCK_NEEDED

/* Initialize variable lock entry pointer */
#define tm_lock_entry_init(lockVarPtr, lockEntryPtr) \
    lockVarPtr = lockEntryPtr

/* check if locking is enabled */
#ifdef TM_TRECK_LIB
#define tm_if_lock_enabled if(tvLockEnabled != TM_8BIT_ZERO)
#else /* ! TM_TRECK_LIB */
#define tm_if_lock_enabled
#endif /* ! TM_TRECK_LIB */

/* non inline macros, when not in send/recv/select path */
#if (    defined(TM_ERROR_CHECKING) && defined(TM_DEBUG_LOGGING) \
      && defined(TM_LOCK_LOGGING) )
#define tm_call_lock_wait(lockEntryPtr) \
{ \
    tm_if_lock_enabled \
    { \
    tfLogMsg(tm_context(tvLogCtrlBlkPtr), "  LOCK ", \
         "0x%x %s, file %s, line %d", \
         lockEntryPtr, #lockEntryPtr, __FILE__, __LINE__); \
    (void)tfLock(lockEntryPtr, TM_LOC_WAIT); \
    } \
}
#define tm_call_unlock(lockEntryPtr) \
{ \
    tm_if_lock_enabled \
    { \
    tfLogMsg(tm_context(tvLogCtrlBlkPtr), "UNLOCK ", \
         "0x%x %s, file %s, line %d", \
         lockEntryPtr, #lockEntryPtr, __FILE__, __LINE__); \
    tfUnLock(lockEntryPtr);\
    } \
}
#else /* TM_ERROR_CHECKING && TM_DEBUG_LOGGING && TM_LOCK_LOGGING*/
#define tm_call_lock_wait(lockEntryPtr) \
{ \
    tm_if_lock_enabled \
    { \
    (void)tfLock(lockEntryPtr, TM_LOC_WAIT); \
    } \
}
#define tm_call_unlock(lockEntryPtr) \
{ \
    tm_if_lock_enabled \
    { \
    tfUnLock(lockEntryPtr);\
    } \
}
#endif /* TM_ERROR_CHECKING && TM_DEBUG_LOGGING && TM_LOCK_LOGGING*/

#ifdef TM_TRECK_LIB
#define tm_if_call_lock_nowait(lockEntryPtr)    \
    if ( (tvLockEnabled == TM_8BIT_ZERO) || \
         (tfLock(lockEntryPtr, TM_LOC_NOWAIT) == TM_LOC_OKAY) )
#else /* ! TM_TRECK_LIB */
#define tm_if_call_lock_nowait(lockEntryPtr)    \
    if ( tfLock(lockEntryPtr, TM_LOC_NOWAIT) == TM_LOC_OKAY)
#endif /* ! TM_TRECK_LIB */

#ifdef TM_OPTIMIZE_SIZE

/* non inline macros */
#define tm_lock_wait_no_log(lockEntryPtr) \
{ \
    tm_if_lock_enabled \
    { \
    (void)tfLock(lockEntryPtr, TM_LOC_WAIT); \
    } \
}
#define tm_unlock_no_log(lockEntryPtr) \
{ \
    tm_if_lock_enabled \
    { \
    tfUnLock(lockEntryPtr);\
    } \
}

#else /* !TM_OPTIMIZE_SIZE */

/* Inline lock/unlock macros for most common cases */
#define tm_lock_wait_no_log(lockEntryPtr) \
    tm_if_lock_enabled \
    { \
/* set the critical section */ \
    tm_kernel_set_critical; \
/* check the resource lock count */ \
    if ((lockEntryPtr)->locCount == 0) \
    { \
        (lockEntryPtr)->locCount++; \
        tm_kernel_release_critical; \
    } else \
    { \
        tm_kernel_release_critical; \
        (void)tfLock(lockEntryPtr, TM_LOC_WAIT); \
        } \
    }

#define tm_unlock_no_log(lockEntryPtr) \
    tm_if_lock_enabled \
    { \
    tm_kernel_set_critical; \
/* If there is no semaphore to be freed and nobody else is waiting */ \
    if (((lockEntryPtr)->locCountSemValid == TM_8BIT_ZERO) \
        && ((lockEntryPtr)->locCount == 1) ) \
    { \
        (lockEntryPtr)->locCount = 0; \
        tm_kernel_release_critical; \
    } else \
    { \
        tm_kernel_release_critical; \
        tfUnLock(lockEntryPtr); \
        } \
    }
#endif /* TM_OPTIMIZE_SIZE */

#if (    defined(TM_ERROR_CHECKING) && defined(TM_DEBUG_LOGGING) \
      && defined(TM_LOCK_LOGGING) )
#define tm_lock_wait(lockEntryPtr) \
    tfLogMsg(tm_context(tvLogCtrlBlkPtr), "  LOCK ", \
             "0x%x %s, file %s, line %d", \
             lockEntryPtr, #lockEntryPtr, __FILE__, __LINE__); \
    tm_lock_wait_no_log(lockEntryPtr);
#define tm_unlock(lockEntryPtr) \
    tfLogMsg(tm_context(tvLogCtrlBlkPtr), "UNLOCK ", \
             "0x%x %s, file %s, line %d", \
             lockEntryPtr, #lockEntryPtr, __FILE__, __LINE__); \
    tm_unlock_no_log(lockEntryPtr)
#else /* TM_ERROR_CHECKING && TM_DEBUG_LOGGING && TM_LOCK_LOGGING*/
#define tm_lock_wait(lockEntryPtr)  tm_lock_wait_no_log(lockEntryPtr)
#define tm_unlock(lockEntryPtr)     tm_unlock_no_log(lockEntryPtr)
#endif /* TM_ERROR_CHECKING && TM_DEBUG_LOGGING && TM_LOCK_LOGGING*/


#ifdef TM_ERROR_CHECKING
#ifdef TM_USE_ANSI_LINE_FILE
/* check to make sure is locked, however does not check who has the lock */
#define tm_assert_is_locked(funcName, lockEntryPtr) \
    if ((tvLockEnabled != TM_8BIT_ZERO) && ((lockEntryPtr)->locCount == 0)) \
    { \
        (void)tm_sprintf((char TM_FAR *)tm_global(tvAssertTempBuf), \
                  "locCount was 0: line %d, file(%s)\n", \
                  __LINE__, __FILE__); \
        tfKernelError( #funcName , (char TM_FAR *)tm_global(tvAssertTempBuf)); \
        tm_thread_stop; \
    }
#else   /* ! TM_USE_ANSI_LINE_FILE */
/* check to make sure is locked, however does not check who has the lock */
#define tm_assert_is_locked(funcName, lockEntryPtr) \
    if ((tvLockEnabled != TM_8BIT_ZERO) && ((lockEntryPtr)->locCount == 0)) \
    { \
        tfKernelError( #funcName , "locCount was 0"); \
        tm_thread_stop; \
    }
#endif  /* ! TM_USE_ANSI_LINE_FILE */

#define TM_MAX_LOCK_CHECK   5
#define TM_LOCK_CHECK_DELAY 1000

#else   /* ! TM_ERROR_CHECKING */
#define tm_assert_is_locked(funcName, lockEntryPtr)
#endif  /* ! TM_ERROR_CHECKING */

#else   /* ! TM_LOCK_NEEDED */

#define tm_lock_wait_no_log(lockEntryPtr)
#define tm_unlock_no_log(lockEntryPtr)
#define tm_lock_entry_init(lockVarPtr, lockEntryPtr)
#define tm_call_lock_wait(lockEntryPtr)
#define tm_call_unlock(lockEntryPtr)
#define tm_if_call_lock_nowait(lockEntryPtr)
#define tm_lock_wait(lockEntryPtr)
#define tm_unlock(lockEntryPtr)
#define tm_assert_is_locked(funcName, lockEntryPtr)

#endif /* ! TM_LOCK_NEEDED */

/*
 * Post/Pend macros
 */

#ifdef TM_PEND_POST_NEEDED
#define tm_post(pendEntryPtr, lockEntryPtr, errorCode)   \
                        tfPost(pendEntryPtr, lockEntryPtr, errorCode)
#ifdef TM_USE_PEND_TIMEOUT
#define tm_pend(pendEntryPtr, lockEntryPtr)   \
                        tfPend(pendEntryPtr, lockEntryPtr, TM_UL(0))
#define tm_pend_timeout(pendEntryPtr, lockEntryPtr, timeout)   \
                        tfPend(pendEntryPtr, lockEntryPtr, timeout)
#else /* !TM_USE_PEND_TIMEOUT */
#define tm_pend(pendEntryPtr, lockEntryPtr)   \
                        tfPend(pendEntryPtr, lockEntryPtr)
#endif /* !TM_USE_PEND_TIMEOUT */
#else /* !TM_PEND_POST_NEEDED */
#define tm_post(pendEntryPtr, lockEntryPtr, errorCode)
#define tm_pend(pendEntryPtr, lockEntryPtr) TM_EWOULDBLOCK
#ifdef TM_USE_PEND_TIMEOUT
#define tm_pend_timeout(pendEntryPtr, lockEntryPtr, timeout) TM_EWOULDBLOCK
#endif /* TM_USE_PEND_TIMEOUT */
#endif /* TM_PEND_POST_NEEDED */

/*
 * Socket Checkin/checkout
 */
#define tm_socket_checkout(socketPtr) (socketPtr)->socOwnerCount++

#define tm_socket_checkin_unlock(socketPtr) \
{ \
    (socketPtr)->socOwnerCount--; \
    if ((socketPtr)->socOwnerCount==0) \
    { \
        tfSocketFree(socketPtr); \
    } \
    else \
    { \
        tm_unlock(&(socketPtr)->socLockEntry); \
    } \
}

#define tm_socket_checkin_call_unlock(socketPtr) \
{ \
    (socketPtr)->socOwnerCount--; \
    if ((socketPtr)->socOwnerCount==0) \
    { \
        tfSocketFree(socketPtr); \
    } \
    else \
    { \
        tm_call_unlock(&(socketPtr)->socLockEntry); \
    } \
}



/*
 * Red-Black Macros
 */
#define TM_SOC_RB_OKAY        0
#define TM_SOC_RB_ALREADY     1
#define TM_SOC_RB_NOT_FOUND   2
#define TM_SOC_RB_NOT_ACCEPT  3

/*
 * Timer Macros
 */
#define TM_TIM_OKAY       0
#define TM_TIM_NO_TIMER   1
#define TM_TIM_ERROR      -1

#define TM_TIM_AUTO       (tt8Bit)0x01
#define TM_TIM_SUSPENDED  (tt8Bit)0x02
/* Timer flags used internal to trtimer.c:
   #define TM_TIM_USER_OWN   (tt8Bit)0x04
   #define TM_TIM_IN_WORK_Q  (tt8Bit)0x08 */

#define TM_MSECS_PER_SECOND TM_L(1000)

/* there are two different intended usage scenarios for auto-timers:
   1) TCP way: timers are very rarely removed, instead they will be rescheduled
      by calling tfTimerNewTime and suspended (to be later reactivated by a
      call to tfTimerNewTime). This is more efficient when you need to keep
      a timer around for a long time (i.e. for the duration of a TCP stream),
      and you will be frequently stoping/restarting/rescheduling it.

   2) simpler way: when an auto-timer needs to have its timer period changed
      (i.e. be rescheduled), it is removed first, and then a new timer is
      added with the new timer period. Auto-timers are never suspended, only
      removed and re-added. */

#define tm_timer_remove(timerBlockPtr) \
    ((timerBlockPtr)->tmrRemove = TM_8BIT_YES)

#define tm_timer_suspend(timerBlockPtr) \
    ((timerBlockPtr)->tmrSuspend = TM_8BIT_YES)

/* Re-initialize an automatic timer with a new interval of call back */
#define tm_timer_new_time(tmrPtr, time) \
    tfTimerNewTime(tmrPtr, time)

#define tm_timer_removed(timerBlockPtr) \
    ((timerBlockPtr)->tmrRemove != TM_8BIT_ZERO)
/* tm_timer_not_removed is intended to be called from a timer callback, to
   make sure that the timer we are executing hasn't been removed by another
   task */
#define tm_timer_not_removed(timerBlockPtr) \
    ((timerBlockPtr)->tmrRemove == TM_8BIT_ZERO)

/*
 * Always called from a timer call back function.
 * This is to check whether a timer has been reinitialized or suspended
 * betwen the call to the call back function, and obtaining the TCP
 * vector, or DHCP device lock inside the call back function. No need
 * to be critical here since we are protected by the TCP (resp. DHCP)
 * lock and we are inside the timer thread.
 */
#define tm_timer_not_reinit(tmrPtr) \
    ((tmrPtr->tmrCallBackFlag != TM_8BIT_ZERO) \
     && (tmrPtr->tmrSuspend == TM_8BIT_ZERO))

/* Conversion from milliseconds to ticks */
#define tm_ms_to_ticks(ms) \
    ( ((tt32Bit)(ms)+(tt32Bit)(tvTimerTickLength)-1) \
                            /(tt32Bit)(tvTimerTickLength) )

/* Conversion from ticks to milliseconds */
#define tm_ticks_to_ms(ticks) \
    ( (tt32Bit)(ticks)*(tt32Bit)(tvTimerTickLength) )

/*
 * Link layer macros
 */

/* Ethernet link layer */
#define tm_ll_is_enet(devPtr) \
                 (((devPtr)->devLinkLayerProtocol) == TM_LINK_LAYER_ETHERNET)

/* Null link layer (used by loopback) */
#define tm_ll_is_null(devPtr) \
                 (((devPtr)->devLinkLayerProtocol) == TM_LINK_LAYER_NULL)

/* PPP link layer */
#define tm_ll_is_ppp(devPtr) \
                 (((devPtr)->devLinkLayerProtocol) == TM_LINK_LAYER_PPP)

/* SLIP link layer */
#define tm_ll_is_slip(devPtr) \
                 (((devPtr)->devLinkLayerProtocol) == TM_LINK_LAYER_SLIP)

/* Transparent link layer */
#define tm_ll_is_transp(devPtr) \
                 (((devPtr)->devLinkLayerProtocol) == TM_LINK_LAYER_TRANSP)

/* Lan link layer (Ethernet) */
#define tm_4_ll_is_lan(devPtr) \
  tm_16bit_all_bits_set(((devPtr)->devLinkLayerProtocol), TM_LINK_LAN_MASK)

/* Link layer supports broadcast */
#define tm_4_ll_is_broadcast(devPtr) \
  tm_16bit_one_bit_set(((devPtr)->devLinkLayerProtocol), TM_LINK_BROADCAST_MASK)

/* Point to point link layer (PPP, SLIP, TRANSP) */
#define tm_ll_is_pt2pt(devPtr) \
  tm_16bit_one_bit_set(((devPtr)->devLinkLayerProtocol), TM_LINK_PT2PT_MASK)

/* Tunnel link layer */
#define tm_6_ll_is_tunnel(devPtr) \
  tm_16bit_one_bit_set(((devPtr)->devLinkLayerProtocol), TM_LINK_TUNNEL_MASK)


/*
 * Link Layer Flags
 */

/* Minimum ethernet packet size (without CRC). Hardware requirement. */
#define TM_ETHER_MIN_PACKET         60

#define TM_ETHER_IP_MIN_PACKET        (60 - TM_ETHER_HEADER_SIZE)
#define TM_TAGGED_ETHER_IP_MIN_PACKET (60 - TM_TAGGED_ETHER_HEADER_SIZE)

/*
 * When compile with PC-lint, it complain about tm_const_htons. So
 * use macro definition for big/little endian conversion.
 */
#ifdef TM_LITTLE_ENDIAN
/* Ethernet type constant for IP packets in network byte order */
#define TM_ETHER_IP_TYPE            0x0008
/* Ethernet type for ARP packets, constant in network byte order */
#define TM_ETHER_ARP_TYPE           0x0608
#else /* TM_BIG_ENDIAN */
/* Ethernet type constant for IP packets in network byte order */
#define TM_ETHER_IP_TYPE            0x0800
/* Ethernet type for ARP packets, constant in network byte order */
#define TM_ETHER_ARP_TYPE           0x0806
#endif /*TM_LITTLE_ENDIAN */

#define TM_ETHER_ADDRESS_LENGTH     6
/* Link layer hardware address type for ARP and BOOTP (RFC1700)*/
#define TM_ARP_ETHER                1
#define TM_IP_ADDRESS_LENGTH        4
/* Host prefix length == Number of bits in IP address */
#define TM_IP_HOST_PREFIX_LENGTH   32

/* Return values from tfIpBroadMcastToLan() */
#define TM_ETHER_UCAST              0
#define TM_ETHER_BCAST              1
#define TM_ETHER_MULTI              2

/*
 * Note that ethernet addresses are always short int aligned, and are
 * very unlikely to be long int aligned. So we will assume short int
 * alignment, and not assume long int alignment.
 */

/* Check that 2 Ethernet Addresses match */
#define tm_ether_match(eth1, eth2) \
    (    ( ( ((tt16BitPtr)(eth1))[0] ) == ( ((tt16BitPtr)(eth2))[0] ) )   \
      && ( ( ((tt16BitPtr)(eth1))[1] ) == ( ((tt16BitPtr)(eth2))[1] ) )   \
      && ( ( ((tt16BitPtr)(eth1))[2] ) == ( ((tt16BitPtr)(eth2))[2] ) ) )

/* Check that an Ethernet Addresses is broadcast */
#define tm_ether_broadcast(eth) \
    (    ( ( ((tt16BitPtr)(eth))[0] )  ==   (tt16Bit)0xFFFF )             \
      && ( ( ((tt16BitPtr)(eth))[1] )  ==   (tt16Bit)0xFFFF )             \
      && ( ( ((tt16BitPtr)(eth))[2] )  ==   (tt16Bit)0xFFFF ) )

/*
 * Check that an Ethernet Addresses is multicast.
 * Note that we use 2 different definitions for Little and Big Endian
 * instead of using the tm_const_htons() macro, since some
 * compiler cannot handle the long line in the macro expansion.
 */
#ifdef TM_LITTLE_ENDIAN

#define tm_ether_mcast(eth) \
    (    ( ((tt16BitPtr)(eth))[0] == (tt16Bit)0x0001 )                    \
      && ( (eth)[2] == (tt8Bit)0x5E ) )

#else /* !TM_LITTLE_ENDIAN */

#define tm_ether_mcast(eth) \
    (    ( ((tt16BitPtr)(eth))[0] == (tt16Bit)0x0100 )                    \
      && ( (eth)[2] == (tt8Bit)0x5E ) )

#endif /* TM_LITTLE_ENDIAN */

/* Copy Ethernet address from to*/
#define tm_ether_copy(from, to) \
    { \
        ((tt16BitPtr)(to))[0] = ((tt16BitPtr)(from))[0];                  \
        ((tt16BitPtr)(to))[1] = ((tt16BitPtr)(from))[1];                  \
        ((tt16BitPtr)(to))[2] = ((tt16BitPtr)(from))[2];                  \
    }

/*
 * tm_phys_addr_match
 *  Description: check that two physical addresses match for a
 *               given length.
 *  Parameters:
 *       (IN) tt8BitPtr physPtr1:
 *              Pointer to first physical address to check
 *       (IN) tt8BitPtr physPtr2:
 *              Pointer to second physical address to check
 *       (IN) int       len
                Length of physical addresses
 *       (OUT) int      match
 *          Upon return physical addresses match if match == len
 */
#define tm_phys_addr_match(physPtr1, physPtr2, len, match)                  \
{                                                                           \
    for (match = 0; match < len; match++)                                   \
    {                                                                       \
        if (physPtr1[match] != physPtr2[match])                             \
        {                                                                   \
            break;                                                          \
        }                                                                   \
    }                                                                       \
}


/*
 * Device macros
 */
/* Multihoming flag (devIpConfFlag)
 */
#define TM_DEV_IP_CONFIG            0x1     /* IP address configured */
#define TM_DEV_IP_CONF_STARTED      0x2     /* IP address being configured */

/*
 * Device Flag (devFlag):
 * Device opened, Multicast enabled (defined in trsocket.h)
 * IP Forwarding enabled (defined in trsocket.h),
 * IP Directed broadcast (defined in trsocket.h),
 * IP Mcast forwarding enabled (defined in trsocket.h)
 * Scattered write flag (defined in trsocket.h)
 * BOOTP or DHCP configuration (defined in trsocket.h)
 */
#define TM_DEV_XMIT_TASK               0x1 /* Transmit task enabled */
#define TM_DEV_OPENED                  0x2 /* Device opened */
/* Device connection in progress (i.e modem dialer, PPP connection) */
#define TM_DEV_CONNECTING              0x4
/*
 * Device connected (modem connection and PPP connection completed,
 * or BOOTP/DHCP connection completed)
 */
#define TM_DEV_CONNECTED               0x8
/* Transmitting inside the device transmit queue logic */
#define TM_DEV_XMITING                 0x10

#define tm_dev_mcast_enb(devPtr)       tm_16bit_one_bit_set( \
                                              devPtr->devFlag \
                                              ,TM_DEV_MCAST_ENB)
#define tm_dev_dbroad_forward(devPtr)  tm_16bit_all_bits_set( \
                                              devPtr->devFlag \
                                             , TM_DEV_IP_FORW_DBROAD_ENB \
                                               |TM_DEV_IP_FORW_ENB)
#define tm_4_dev_forward(devPtr)       tm_16bit_one_bit_set( \
                                              devPtr->devFlag \
                                             , TM_DEV_IP_FORW_ENB)
#define tm_4_dev_mcast_forward(devPtr) tm_16bit_all_bits_set( \
                                              devPtr->devFlag \
                                              , TM_DEV_IP_FORW_MCAST_ENB \
                                               |TM_DEV_IP_FORW_ENB)
#define tm_dev_is_loopback(devPtr)     (devPtr == tm_context(tvLoopbackDevPtr))
#define tm_dev_no_check(devPtr)        tm_16bit_one_bit_set(  \
                                              devPtr->devFlag  \
                                            , TM_DEV_IP_NO_CHECK)
#define tm_dev_mcast_route(devPtr)     tm_16bit_one_bit_set(  \
                                              devPtr->devFlag  \
                                            , TM_DEV_IP_MCAST_ROUTE)

/*
 * NAT options flag devNatOpt
 */
/* outside NAT interface */
#define TM_NAT_PUBLIC            0x01
/* NAT fields initialized */
#define TM_NAT_INITED            0x02

/* Checking wheter an interface has been configured as a public (outside)
   NAT interface */
#define tm_nat_public(devPtr)                                              \
    tm_16bit_one_bit_set(devPtr->devNatOpt,   TM_NAT_PUBLIC )

/* Flags used in second parameter of (*devPtr->devNatFuncPtr)() */
/* Traffic is outgoing (Need to translate the source IP address) */
#define TM_NAT_SEND         0x1
/* Traffic is incoming (Need to translate the destination IP address) */
#define TM_NAT_INCOMING     0X2

#define TM_NTRTTL_FOREVER TM_UL(0x7FFFFFFF)
#define TM_NTRTTL_DYNAMIC (15*60)       /* 15 minutes for any dynamic traffic*/
                                        /* timeouts below for NAPT & SERVER  */
#define TM_NTRTTL_TCP     (TM_UL(24)*60*60) /* 24 hours for nonterminated TCP*/
#define TM_NTRTTL_TCPFIN  (1*60)        /* 1 minute for FIN'd or RST'd TCP   */
#define TM_NTRTTL_TCPSYN  (1*60)        /* 1 minute for TCP SYN (either dir) */
#define TM_NTRTTL_FTPPORT (5*60)        /* 5 minutes for FTP PORT connection */
#define TM_NTRTTL_UDP     (5*60)        /* 5 minutes for non-DNS UDP         */
#define TM_NTRTTL_UDPDNS  (1*60)        /* 1 minute for DNS UDP              */
#define TM_NTRTTL_ICMPID  (5*60)        /* 5 minutes ICMP id msgs, eg ping   */

#define TM_NTRTYPE_STATIC             1 /* match priv/pub inbound/outbound   */
#define TM_NTRTYPE_DYNAMIC            2 /* match priv/pub and remote         */
#define TM_NTRTYPE_DYNAMIC_OUTBOUND   3 /* spawns new DYNAMIC associations   */
#define TM_NTRTYPE_TCPSESSION         4 /* track priv&pub&remote port&ip     */
#define TM_NTRTYPE_UDPSESSION         5 /* track priv&pub&remote port&ip     */
#define TM_NTRTYPE_ICMPIDSESSION      6 /* track ICMP identifier             */
#define TM_NTRTYPE_NAPT               7 /* spawns new TCP sessions for NAPT  */
#define TM_NTRTYPE_INNERTCPSERVER     8 /* TCP for specific incoming ports   */
#define TM_NTRTYPE_INNERFTPSERVER     9 /* FTP for command port              */
#define TM_NTRTYPE_INNERUDPSERVER    10 /* UDP for specific incoming ports   */

#define TM_NTRFLAG_ACTIVE 0x01     /* DYNO trig already spawned a DYN trig   */
#define TM_NTRFLAG_TCPSEQ 0x02     /* TCP sequence/ack adjustment            */
#define TM_NTRFLAG_TCPFIN 0x08     /* TCP connection detected FIN or RST     */


/* Ring of queued transmit buffers is empty */
#define TM_RING_EMPTY (tt8Bit)0x0

/* Queue is not empty */
#define tm_iface_xmit_queue_not_empty(devXmitPtr)                           \
                          ( devXmitPtr->xmitFlag != TM_RING_EMPTY )

/* User turned on the device XMIT queue option */
#define tm_iface_xmit_queue_option(interfaceHandle)                         \
  ( ((ttDeviceEntryPtr)interfaceHandle)->devXmitEntryPtr != (ttVoidPtr)0 )

/*
 * Get Virtual channel. Called from device driver send function.
 * Can be used instead of tfInterfaceGetVirtualChannel()
 */
#define tm_iface_get_vchan(interfaceHandle) \
    tm_dev_get_vchan((ttDeviceEntryPtr)interfaceHandle)

#ifdef TM_SINGLE_INTERFACE_HOME
#define tm_dev_get_vchan(devEntryPtr) (devEntryPtr)->devVChan
#else /* !TM_SINGLE_INTERFACE_HOME */
#define tm_dev_get_vchan(devEntryPtr)  \
(devEntryPtr)->devVChanArray[(devEntryPtr)->devSendQueueCurPtr->pktMhomeIndex]
#endif /* TM_SINGLE_INTERFACE_HOME */

/*
 * 8 bit flags macros
 */
/* One byte with all bits set */
#define TM_8BIT_ONES                (tt8Bit)0xFF
/* One byte with all bits zero */
#define TM_8BIT_ZERO                (tt8Bit)0
#define TM_8BIT_NO                  (tt8Bit)0
/* One byte with non zero value */
#define TM_8BIT_YES                 (tt8Bit)1
/* one byte with MSB bit set */
#define TM_8BIT_MSB                 (tt8Bit)0x80
/* convert number of bytes to number of bits */
#define tm_bytes_to_bits(bytes)      (tt16Bit)(((tt16Bit)(bytes)) << 3)

#define tm_8bit_one_bit_set(byte, bitmask8)  \
               (((tt8Bit)(byte) & (tt8Bit)(bitmask8)) != TM_8BIT_ZERO)

#define tm_8bit_all_bits_set(byte, bitmask8)  \
               (((tt8Bit)(byte) & (tt8Bit)(bitmask8)) == (tt8Bit)(bitmask8))

#define tm_8bit_bits_not_set(byte, bitmask8)  \
               (((tt8Bit)(byte) & (tt8Bit)(bitmask8)) == TM_8BIT_ZERO)

/*
 * 16 bits flag macros
 */

/* two bytes with all bits set */
#define TM_16BIT_ONES                (tt16Bit)0xFFFF
/* two bytes with all bits zero */
#define TM_16BIT_ZERO                (tt16Bit)0x0
/* two byte with MSB bit set */
#define TM_16BIT_MSB                 (tt16Bit)0x8000

/* convert number of 16-bit shorts to number of bits */
#define tm_shorts16_to_bits(shorts16) (((tt32Bit)(shorts16)) << 4)

#define tm_16bit_one_bit_set(short16, bitmask16)  \
       (((tt16Bit)(short16) & (tt16Bit)(bitmask16)) != TM_16BIT_ZERO)

#define tm_16bit_all_bits_set(short16, bitmask16)  \
       (((tt16Bit)(short16) & (tt16Bit)(bitmask16)) == (tt16Bit)(bitmask16))

#define tm_16bit_bits_not_set(short16, bitmask16)  \
       (((tt16Bit)(short16) & (tt16Bit)(bitmask16)) == TM_16BIT_ZERO)

/*
 * ARP macros
 */
#define TM_ARP_HEADER_SIZE       28
#define TM_ARP_HARDWARE_ETHERNET tm_const_htons(0x0001)
#define TM_ARP_REQUEST           tm_const_htons(0x0001)
#define TM_ARP_REPLY             tm_const_htons(0x0002)


/*
 * IP macros
 */

/* Version (4) and length (5 double words) */
#define TM_IP_NOOPT_VERSLEN     (tt8Bit)0x45
/* IP version 4 */
#define TM_IP_4                 (tt8Bit)4
/* IP version 6 */
#define TM_IP_6                 (tt8Bit)6

/*
 * From IP header version-length field to IP header length in bytes:
 * 4*(lower 4 bits of 8bit verLen)
 */
#define tm_ip_hdr_len(verLen)   ((tt16Bit)(((tt16Bit)((verLen)&0x0F)) << 2))

/* upper 4 bits of 8bit verLen */
#define tm_ip_version(verLen)   (tt8Bit)((verLen&0xF0) >> 4)

/* Convert IP header length in bytes to IP header version-length field */
#define tm_ip_set_vers_Len(hdrLen) (tt8Bit)((tt8Bit)(TM_IP_4<<4) \
                                    |(tt8Bit)(((tt8Bit)(hdrLen))>>2))

#ifndef TM_IP_MIN_MTU
/*
 * RFC 791: IP MTU that every host should be able to forward without
 * further fragmentation is 68 bytes. This is because the maximum IP
 * header size is 60 bytes, and the minimum fragment is 8 bytes.
 */
#define TM_IP_MIN_MTU   68
#endif

/*
 * Flags for ttPacket pktFlags
 */

/* Physical Destination address is unicast */
#define TM_LL_UNICAST_FLAG          (tt16Bit)0x1
/* Physical Destination address is broadcast */
#define TM_LL_BROADCAST_FLAG        (tt16Bit)0x2
/* Physical Destination address is multicast */
#define TM_LL_MULTICAST_FLAG        (tt16Bit)0x4
/* tfIpDestToPacket() called or bypassed */
#define TM_IP_DEST_ROUTE_SET        (tt16Bit)0x8
/* Destination IP address is unicast */
#define TM_IP_UNICAST_FLAG          (tt16Bit)0x10
/* Destination IP address is Broadcast */
#define TM_IP_BROADCAST_FLAG        (tt16Bit)0x20
/* Destination IP address is multicast */
#define TM_IP_MULTICAST_FLAG        (tt16Bit)0x40
/* Datagram to send is being forwarded */
#define TM_IP_FORWARD_FLAG          (tt16Bit)0x80
/* Destination is on local network */
#define TM_IP_LOCAL_FLAG            (tt16Bit)0x100
/* Routing pointer already stored in packet entry */
#define TM_IP_RTE_FLAG              (tt16Bit)0x200
/* Output device/multi home index is already stored in ttPacket */
#define TM_OUTPUT_DEVICE_FLAG       (tt16Bit)0x400
/* Destination Ethernet address is already stored in ttPacket */
#define TM_ETHERNET_ADDRESS_FLAG    (tt16Bit)0x800
/* Datagram is being multicast forwarded */
#define TM_IP_MCAST_FORWARD_FLAG    (tt16Bit)0x1000
/* Mobile IPv6 packet flag. When set in the send-path, this flag indicates
   to bypass route optimization for this specific packet. When set in the
   receive-path, this flag indicates that the packet was received without a
   type 2 Routing Header, i.e. it was not route optimized. This is used by
   both the correspondent node and the mobile node. */
#define TM_6_MIP_BYPASS_RO_FLAG     (tt16Bit)0x2000
/* pktFlags, indicates HAO is presented in the received packet */
#define TM_6_MIP_HAO_FLAG           (tt16Bit)0x4000

/*
 * Values for pktNetworkLayer
 */
/* IPv4 network layer packet */
#define TM_NETWORK_IPV4_LAYER   0
/* IPv6 network layer packet */
#define TM_NETWORK_IPV6_LAYER   1
/* ARP network layer packet */
#define TM_NETWORK_ARP_LAYER    2
/* Link layer packet */
#define TM_NETWORK_LINK_LAYER   3

/*
 * Flags for ttPacket pktUserFlags
 */
/* User owns the zero copy buffer, and is allowed to free it */
#define TM_PKTF_USER_OWNS           (tt16Bit)0x200
/* User owns the user device buffer, and is allowed to free it */
#define TM_PKTF_USER_DEV_OWNS       (tt16Bit)0x400
/*
 * The device driver buffer is in the process of being written
 * (intramachine only)
 */
#define TM_PKTF_USER_DEV_WRITE      (tt16Bit)0x800
/*
 * User has called tfSendCompletePacketInterface on this packet.
 */
#define TM_PKTF_USER_DEV_SEND_CMPLT (tt16Bit)0x1000
/*
 * User is calling tfSendToFrom, and a from IP address is
 * specified in the ttSharedData (dataFromIpAddress).
 */
#define TM_PKTF_USER_SEND_FROM      (tt16Bit)0x2000

/* TCP has started sending data for this packet. Don't append to it */
#define TM_PKTF_USER_DATA_SENT      (tt16Bit)0x4000

/*
 * Flags for ttPacket pktFlags2
 */
/* TCP Remote has SACK'ed all the bytes in this packet */
#define TM_PF2_TCP_SACK_ON          (tt16Bit)0x0001
/* TCP Buffer is being re-transmitted */
#define TM_PF2_TCP_REXMIT           (tt16Bit)0x0002
/* TCP Buffer is being timed (for Vegas RTT computation) */
#define TM_PF2_TCP_TIMED            (tt16Bit)0x0004
/* This packet destination is indirect */
#define TM_PF2_INDIRECT             (tt16Bit)0x0008
/* No unacknowledged data before the dataPacketPtr->pktLinkDataPtr */
#define TM_PF2_NO_UNACK_DATA        (tt16Bit)0x0010

/* For IPsec use. Indicates that this packet is allocated for
 * adding IPsec trailer, and for TCP protocol packet.
 */
#define TM_PF2_TCP_TRAILER          (tt16Bit)0x0100
/* For IPsec use. Indicates that a received packet has been authenticated
   by IPsec. */
#define TM_PF2_IPSEC_RCV_AUTH       (tt16Bit)0x0200
/* For IPsec use. Indicates that the fragment extension header has
 * been processed, and it should not be included in the AH
 * hash caculation
 */
#define TM_PF2_FRAGHEADER_NOT_IN_AH (tt16Bit)0x0400
/* For IPsec use. Indicates that this packet->pktLinkDataLength
 * includes the ICV size which attached at the end
 */
#define TM_PF2_AUTH_ICVDATA_AT_END  (tt16Bit)0x0800

/* User has set the mhome index, i.e. pktMhomeIndex is set. */
#define TM_PF2_MHOMEINDEX_SET       (tt16Bit)0x2000
/* Make sure that source IP address is zero */
#define TM_PF2_ZERO_SRC_IP_ADDRESS  (tt16Bit)0x4000
/* There is valid data following the TCP data in this buffer; be aware if
   a trailer needs to be added. */
#define TM_PF2_TRAILING_DATA        (tt16Bit)0x8000

/* Maximum IP TTL value (255) */
#define TM_IP_MAX_TTL               (tt8Bit)255
/* Minimum value of TTL for an outgoing forwarded packet */
#define TM_IP_MIN_FORW_TTL          (tt8Bit)1
/* TTL value set by a sender to forbid forwarding of the packet */
#define TM_IP_NO_FORW_TTL          (tt8Bit)0

/* Upper Layer Protocol values */
#define TM_IP_ICMP                  (tt8Bit)1
#define TM_IP_IGMP                  (tt8Bit)2
#define TM_IP_IP_ENCAP              (tt8Bit)4
#define TM_IP_TCP                   (tt8Bit)6
#define TM_IP_UDP                   (tt8Bit)17
#define TM_IP_IPV6                  (tt8Bit)41
#define TM_IP_ESP                   (tt8Bit)50
#define TM_IP_AH                    (tt8Bit)51
#define TM_IP_MIN_ENCAP             (tt8Bit)55
#define TM_IP_ICMPV6                (tt8Bit)58
#define TM_IP_OSPF                  (tt8Bit)89

/* Fragment field bits */
/* first 3 bits (network byte order) */
/* reserved - must be off */
#ifdef TM_LITTLE_ENDIAN
#define TM_IP_RESV_FRAG          0x0080
/* All fragment bits */
#define TM_IP_FRAG_BITS          0xFFDF
/* Don't Fragment flag */
#define TM_IP_DONT_FRAG          0x0040
/* More Fragments flag */
#define TM_IP_MORE_FRAG          0x0020
/* More flag and fragment offset bits */
#define TM_IP_MORE_FRAG_OFFSET   0xFF3F
/* last 13 bits (net byte order). Mask for fragment offset in IP datagram */
#define TM_IP_FRAG_OFFSET        0xFF1F

#else /* TM_BIG_ENDIAN */

#define TM_IP_RESV_FRAG          0x8000
/* All fragment bits */
#define TM_IP_FRAG_BITS          0xDFFF
/* Don't Fragment flag */
#define TM_IP_DONT_FRAG          0x4000
/* More Fragments flag */
#define TM_IP_MORE_FRAG          0x2000
/* More flag and fragment offset bits */
#define TM_IP_MORE_FRAG_OFFSET   0x3FFF
/* last 13 bits (net byte order). Mask for fragment offset in IP datagram */
#define TM_IP_FRAG_OFFSET        0x1FFF

#endif /* TM_LITTLE_ENDIAN */

/* last 13 bits (host byte order). Mask for fragment offset in IP datagram */
#define TM_IP_HFRAG_OFFSET        0x1FFF
/*
 * port macros
 */
#define TM_PORT_ZERO             0x0U

/*
 * Note that we use 2 different definitions for Little and Big Endian
 * instead of using the tm_const_htons() macro, since some
 * compiler cannot handle the long lines in the macro expansions
 * of tm_ip_class_net_mask(ipAddr), and of tm_ip_ext_class_net_mask(ipAddr)
 */

#ifdef TM_LITTLE_ENDIAN
/*
 * Net masks
 */
#define TM_IP_CLASS_A_NET_MASK        TM_UL(0x000000FF)
#define TM_IP_CLASS_B_NET_MASK        TM_UL(0x0000FFFF)
#define TM_IP_CLASS_C_NET_MASK        TM_UL(0x00FFFFFF)

/*
 * Host masks
 */

#define TM_IP_CLASS_A_HOST_MASK       TM_UL(0xFFFFFF00)
#define TM_IP_CLASS_B_HOST_MASK       TM_UL(0xFFFF0000)
#define TM_IP_CLASS_C_HOST_MASK       TM_UL(0xFF000000)

/*
 * High bits set (one, two, three, four)
 */
#define TM_IP_ONE_HIGH_BIT            TM_UL(0x00000080)
#define TM_IP_TWO_HIGH_BITS           TM_UL(0x000000C0)
#define TM_IP_THREE_HIGH_BITS         TM_UL(0x000000E0)
#define TM_IP_FOUR_HIGH_BITS          TM_UL(0x000000F0)
/*
 * High bit set (second, third, or fourth)
 */
#define TM_IP_SECOND_HIGH_BIT         TM_UL(0x00000040)
#define TM_IP_THIRD_HIGH_BIT          TM_UL(0x00000020)
#define TM_IP_FOURTH_HIGH_BIT         TM_UL(0x00000010)

/*
 * Loop back net address.
 */
#define TM_IP_LOOP_BACK_NET_ADDR       TM_UL(0x0000007F)
#define TM_IP_LOOP_BACK_ADDR           TM_UL(0x0100007F)

#else /* TM_BIG_ENDIAN */
/*
 * Net masks
 */
#define TM_IP_CLASS_A_NET_MASK        TM_UL(0xFF000000)
#define TM_IP_CLASS_B_NET_MASK        TM_UL(0xFFFF0000)
#define TM_IP_CLASS_C_NET_MASK        TM_UL(0xFFFFFF00)

/*
 * Host masks
 */

#define TM_IP_CLASS_A_HOST_MASK       TM_UL(0x00FFFFFF)
#define TM_IP_CLASS_B_HOST_MASK       TM_UL(0x0000FFFF)
#define TM_IP_CLASS_C_HOST_MASK       TM_UL(0x000000FF)

/*
 * High bits set (one, two, three, four)
 */
#define TM_IP_ONE_HIGH_BIT            TM_UL(0x80000000)
#define TM_IP_TWO_HIGH_BITS           TM_UL(0xC0000000)
#define TM_IP_THREE_HIGH_BITS         TM_UL(0xE0000000)
#define TM_IP_FOUR_HIGH_BITS          TM_UL(0xF0000000)

#define TM_IP_SECOND_HIGH_BIT         TM_UL(0x40000000)
#define TM_IP_THIRD_HIGH_BIT          TM_UL(0x20000000)
#define TM_IP_FOURTH_HIGH_BIT         TM_UL(0x10000000)

/*
 * Loop back net address.
 */
#define TM_IP_LOOP_BACK_NET_ADDR      TM_UL(0x7F000000)
#define TM_IP_LOOP_BACK_ADDR          TM_UL(0x7F000001)

#endif /* TM_BIG_ENDIAN */


/*
 * Special IP addresses
 */
/* Minimum valid netmask */
#define TM_IP_MIN_NETMASK            TM_IP_CLASS_A_NET_MASK
#define TM_IP_ALL_ONES               (tt4IpAddress)TM_UL(0xFFFFFFFF)
/* Limited broadcast (all bits set) */
#define TM_IP_LIMITED_BROADCAST      TM_IP_ALL_ONES
/* Host netmask (all bits set) */
#define TM_IP_HOST_MASK              TM_IP_ALL_ONES
/* Zero IP address */
#define TM_IP_ZERO                   (tt4IpAddress)TM_UL(0x0)

/*
 * IP addresses manipulation macros
 */
/* OR of 2 ip addresses. Yields all bits that are on */
#define tm_ip_or(ipAddr, netMask)         ((ipAddr)|(netMask))

/* XOR of 2 ip addresses. Yields bits that are different */
#define tm_ip_xor(ipAddr1, ipAddr2)       ((ipAddr1)^(ipAddr2))

/* AND of 2 ip addresses. Yields bits that are the same */
#define tm_ip_and(ipAddr1, ipAddr2)       ((ipAddr1)&(ipAddr2))

/*
 * Not of ipAddr. Yields opposite bits
 * (same as tm_ip_xor(ipAddr, TM_IP_ALL_ONES))
 */
#define tm_ip_not(ipAddr)                 (~(ipAddr))

/*
 * Get the network portion of an IP address
 */
#define tm_ip_net(ipAddr, netMask)        tm_ip_and(ipAddr, netMask)


#ifdef TM_LITTLE_ENDIAN
/*
 * compute the number of leading bits that are the same at the beginning
 * of key and ipAddr. (IPV4 addresses).
 * Result will be stored in bits.
 *
 * key value arg, ipAddr value arg, bittest temp var, bits var arg
 */
#define tm_ip_bit_offset(key, ipAddr, bits) \
{ \
   bits = tfRtBitOffset(key, ipAddr, (tt4IpAddressPtr)0); \
}

/*
 * Get the mask prefix length for an IPV4 address.
 * Result will be stored in bits.
 */
#define tm_ip_len_mask(netMask, bits) \
    tm_ip_bit_offset(netMask, TM_IP_ALL_ONES, bits)

#else /* TM_BIG_ENDIAN */

/*
 * compute the number of leading bits that are the same at the beginning
 * of key and ipAddr. (IPV4 addresses)
 * Result will be stored in bits. bittest is a temporary variable.
 *
 * key value arg, ipAddr value arg, bittest temp var, bits var arg
 * 1. keep only the bits that differ
 * 2. assume all bits match
 * 3. Shift right until no more bits are different
 *   .Decrease by the number of bits that differ
 *   .shift rightmost bit right
 */
#define tm_ip_bit_offset(key, ipAddr, bittest, bits) \
{ \
    bittest = tm_ip_xor(key, ipAddr); \
    bits = tm_bytes_to_bits(tm_byte_count(sizeof(tt4IpAddress))); \
    while (tm_ip_not_zero(bittest)) \
    { \
        bits--; \
        bittest >>= 1; \
    } \
}

/*
 * Get the mask prefix length (IPV4 address)
 * Result will be stored in bits. bittest is a temporary variable.
 */
#define tm_ip_len_mask(netMask, bits, bittest) \
    tm_ip_bit_offset(netMask, TM_IP_ALL_ONES, bittest, bits)

#endif /* TM_BIG_ENDIAN */

/*
 * Check whether 2 IP addresses match exactly
 */
#define tm_ip_match(ipAddr1, ipAddr2)     ((ipAddr1) == (ipAddr2))
/* or (tm_ip_xor(ipAddr1, ipAddr2) == TM_IP_ZERO) */

/*
 * Check whether 2 IP addresses do not match
 */
#define tm_ip_not_match(ipAddr1, ipAddr2) ((ipAddr1) != (ipAddr2))
/* or (tm_ip_xor(ipAddr1, ipAddr2) != TM_IP_ZERO) */

/*
 * Check whether an IP address is zero
 */
#define tm_ip_zero(ipAddr)                ((ipAddr) == (TM_IP_ZERO))

/*
 * Check whether an IP address is not zero
 */
#define tm_ip_not_zero(ipAddr)            ((ipAddr) != (TM_IP_ZERO))

/*
 * Check whether two IP addresses are on the same network
 */
#define tm_ip_net_match(ipAddr1, ipAddr2, netMask) \
    tm_ip_zero(tm_ip_net(tm_ip_xor(ipAddr1, ipAddr2), netMask))

/*
 * build the directed broadcast for the interface ipAddr, netMask
 */
#define tm_ip_bld_d_broadcast(ipAddr, netMask) \
    tm_ip_or(tm_ip_net(netMask, ipAddr), tm_ip_not(netMask))

/* Copy an IP address */
#define tm_ip_copy(srcIpAddr, destIpAddr)    (destIpAddr = srcIpAddr)

/* swap the two ip addresses */
#define tm_ip_swap(ipAddrA, ipAddrB)    \
    ipAddrA = (ipAddrA) ^ (ipAddrB);    \
    ipAddrB = (ipAddrA) ^ (ipAddrB);    \
    ipAddrA = (ipAddrA) ^ (ipAddrB);

/*
 * tm_sockaddr_storage_copy(srcPtr, dstPtr)
 *  Description: copy one sockaddr storage to another
 *  Parameters:
 *     srcPtr: struct sockaddr_storage srcPtr
 *     dstPtr: struct sockaddr_storage dstPtr
 */
#define tm_sockaddr_storage_copy(srcPtr, dstPtr) \
{ \
    tm_bcopy((srcPtr), (dstPtr), (sizeof(struct sockaddr_storage))); \
  \
}

/*
 * There is no guarantee that Ip addresss in the IP options will
 * be 32-bit aligned (depending on how the issuer organized the options)
 */
/* Assume processor needs 32-bit alignemnts of pointers */
#define TM_NEED_32BIT_ALIGNMENT

#ifdef TM_NEED_32BIT_ALIGNMENT
#define tm_ip_ptr_u_copy(ipAddrPtr, destIpAddr) tm_bcopy( \
                                                   ipAddrPtr, \
                                                   &destIpAddr, \
                                                   sizeof(tt4IpAddress))
#define tm_ip_u_ptr_copy(srcIpAddr, ipAddrPtr) tm_bcopy( \
                                                   &srcIpAddr, \
                                                   ipAddrPtr, \
                                                   sizeof(tt4IpAddress))
#define tm_ip_ptr_ptr_copy(srcIpAddrPtr, destIpAddrPtr) tm_bcopy( \
                                                   &srcIpAddr, \
                                                   &srcIpAddrPtr, \
                                                   sizeof(tt4IpAddress))
#else /* !TM_NEED_32BIT_ALIGNMENT */
#define tm_ip_ptr_u_copy(ipAddrPtr, destIpAddr) \
                         destIpAddr = *((tt4IpAddressPtr)ipAddrPtr)
#define tm_ip_u_ptr_copy(srcIpAddr, ipAddrPtr) \
                         *((tt4IpAddressPtr)ipAddrPtr) = srcIpAddr
#define tm_ip_ptr_ptr_copy(srcIpAddrPtr, destIpAddrPtr) \
                         *((tt4IpAddressPtr)destIpAddrPtr) = \
                                     *((tt4IpAddressPtr)srcIpAddrPtr)
#endif /* TM_NEED_32BIT_ALIGNMENT */

/*
 * Class of IP address
 */

/* Binary high order bits: 0xxx */
#define tm_ip_is_class_a(ipAddr) \
  tm_ip_zero(tm_ip_and(TM_IP_ONE_HIGH_BIT, ipAddr))
/* Binary high order bits: 10xx */
#define tm_ip_is_class_b(ipAddr) \
  tm_ip_match(tm_ip_and(TM_IP_TWO_HIGH_BITS, ipAddr), TM_IP_ONE_HIGH_BIT)
/* Binary high order bits: 110x */
#define tm_ip_is_class_c(ipAddr) \
  tm_ip_match(tm_ip_and(TM_IP_THREE_HIGH_BITS, ipAddr), TM_IP_TWO_HIGH_BITS)
/* Binary high order bits: 1110 */
#define tm_ip_is_class_d(ipAddr) \
  tm_ip_match(tm_ip_and(TM_IP_FOUR_HIGH_BITS, ipAddr), TM_IP_THREE_HIGH_BITS)
/* Binary high order bits: 1111 */
#define tm_ip_is_class_e(ipAddr) \
  tm_ip_match(tm_ip_and(TM_IP_FOUR_HIGH_BITS, ipAddr), TM_IP_FOUR_HIGH_BITS)
/* Binary high order bits: 111x */
#define tm_ip_is_class_d_e(ipAddr) \
  tm_ip_match(tm_ip_and(TM_IP_THREE_HIGH_BITS, ipAddr), TM_IP_THREE_HIGH_BITS)

/* Binary high order bits: 00000000 */
#define tm_ip_is_net_0(ipAddr) \
  tm_ip_match(tm_ip_and(TM_IP_CLASS_A_NET_MASK, ipAddr), TM_IP_ZERO)

/*
 * Multicast addresses
 */

#define tm_ip_is_multicast(ipAddr)   tm_ip_is_class_d(ipAddr)
/* 224.0.0.255 */
#define TM_IP_MCAST_MAX_LOCAL_HOST   tm_const_htonl(TM_UL(0xE00000FF))
/* 224.0.0.0 */
#define TM_IP_MCAST_UNSPEC_HOST      tm_const_htonl(TM_UL(0xE0000000))
/* 224.0.0.1 */
#define TM_IP_MCAST_ALL_HOSTS        tm_const_htonl(TM_UL(0xE0000001))
/* 224.0.0.2 */
#define TM_IP_MCAST_ALL_RTRS         tm_const_htonl(TM_UL(0xE0000002))
/* 224.0.0.9 */
#define TM_IP_MCAST_RIP_RTR          tm_const_htonl(TM_UL(0xE0000009))
#define TM_IP_MCAST_NON_LOCAL_MASK   ~TM_IP_MCAST_MAX_LOCAL_HOST
#define tm_ip_is_local_mcast_address(x) \
  tm_ip_net_match(x, TM_IP_MCAST_MAX_LOCAL_HOST, TM_IP_CLASS_C_NET_MASK)
                                    /* 224.0.0.1 .. 224.0.0.FF */

/*
 * Loop back addresses: class A address with 127 (0x7F) network address
 */
#define tm_ip_is_loop_back(ipAddr) \
    tm_ip_match(tm_ip_and(TM_IP_CLASS_A_NET_MASK, ipAddr), \
                TM_IP_LOOP_BACK_NET_ADDR )
/* or tm_ip_net_match(tm_const_htonl(TM_UL(0x7F000000)), ipAddr
                                                  , TM_IP_CLASS_A_NET_MASK) */

#define tm_ip_class_net_mask(ipAddr) \
   ( tm_ip_zero(tm_ip_and(ipAddr, TM_IP_ONE_HIGH_BIT)) ? \
     TM_IP_CLASS_A_NET_MASK : \
     tm_ip_zero(tm_ip_and(ipAddr, TM_IP_SECOND_HIGH_BIT)) ? \
     TM_IP_CLASS_B_NET_MASK : \
     tm_ip_zero(tm_ip_and(ipAddr, TM_IP_THIRD_HIGH_BIT)) ? \
     TM_IP_CLASS_C_NET_MASK : TM_IP_HOST_MASK )

#define tm_ip_is_host(ipAddr, netMask) \
     tm_ip_not_zero(tm_ip_and(ipAddr, tm_ip_not(netMask)))

#define tm_ip_is_broadcast(ipAddr, netMask)                 \
     (tm_ip_match(netMask, TM_IP_LIMITED_BROADCAST) ?        \
     tm_ip_match(ipAddr, TM_IP_LIMITED_BROADCAST) :         \
     tm_ip_match(tm_ip_or(ipAddr, netMask), TM_IP_LIMITED_BROADCAST))

/*
 * Used by ICMP to check that an address mask reply is valid.
 * Invalid netmask is either all 1's, or if not zero, first 8 bits not set.
 */
#define tm_icmp_valid_netmask(netMask) \
    ( tm_ip_not_match(netMask, TM_IP_ALL_ONES) && \
      (tm_ip_zero(netMask) \
       || tm_ip_match(tm_ip_and(netMask, TM_IP_MIN_NETMASK), \
                                                TM_IP_MIN_NETMASK)) )

/* First 8 bits are not set */
#define tm_icmp_8bits_not_set(netMask) \
       tm_ip_not_match(tm_ip_and(netMask, TM_IP_MIN_NETMASK), \
                                                TM_IP_MIN_NETMASK)

/* Mask is not zero, and first 8 bits are not set */
#define tm_icmp_not_zero_8bits_not_set(netMask) \
      ( tm_ip_not_zero(netMask) \
       && tm_icmp_8bits_not_set(netMask) )

#define tm_icmp_invalid_netmask(netMask) \
    ( tm_ip_match(netMask, TM_IP_ALL_ONES) || \
      tm_icmp_not_zero_8bits_not_set(netMask) )

/*
 * Check whether a subnet mask given by the user
 * is invalid. Invalid set netmasks have their first 8 bits not set when
 * they are non 0.  Should we allow netmask of 0? Disallowed for now.
 */
#define tm_ip_netmask_invalid(netMask) \
        (tm_icmp_8bits_not_set(netMask))
/* This version allows netmask of 0:
        ( tm_icmp_not_zero_8bits_not_set(netMask) \
          || ((is_lan) && tm_ip_match(myNetMask, TM_IP_ALL_ONES))
        )
*/

/*
 * Returns true if the specified IP address is a global IPv4 address -
 * i.e. not in the following sets of
 * addresses: 10.0.0.0/8, 172.16.0.0/12, 192.168.0.0/16, 169.254.0.0/15
 */
#define tm_4_ip_is_global(ipAddr)                                \
(    !tm_ip_net_match((ipAddr), TM_IP_LOCAL_NET_A, 8)            \
  && !tm_ip_net_match((ipAddr), TM_IP_LOCAL_NET_B, 12)           \
  && !tm_ip_net_match((ipAddr), TM_IP_LOCAL_NET_C, 16)           \
  && !tm_ip_net_match((ipAddr), TM_IP_LOCAL_NET_MS, 16) )

/* IPv4 Class A local network - 10.0.0.0/8 */
#define TM_IP_LOCAL_NET_A        tm_const_htonl(TM_UL(0x0a000000))

/* IPv4 Class B local network - 172.16.0.0/12 */
#define TM_IP_LOCAL_NET_B        tm_const_htonl(TM_UL(0xac100000))

/* IPv4 Class C local network - 192.168.0.0/16 */
#define TM_IP_LOCAL_NET_C        tm_const_htonl(TM_UL(0xc0a80000))

/* IPv4 local network, Microsoft autoconfiguration - 169.254.0.0/16 */
#define TM_IP_LOCAL_NET_MS       tm_const_htonl(TM_UL(0xa9fe0000))


#ifdef TM_SINGLE_INTERFACE_HOME
/* device Ip address */
#define tm_ip_dev_addr(devPtr, x) \
        (devPtr)->devIpAddr
/* device Ip mask */
#define tm_ip_dev_mask(devPtr, x)  \
        (devPtr)->devNetMask
/* device virtual channel */
#define tm_ip_dev_vchan(devPtr, x)  \
        (devPtr)->devVChan
/* device directed broadcast Ip address */
#define tm_ip_dev_dbroad_addr(devPtr, x) \
        (devPtr)->devDBroadIpAddr
#define tm_ip_dev_conf_flag(devPtr, x) \
        (devPtr)->devIpConfFlag
#define tm_4_ip_dev_prefixLen(devPtr, x) \
        (devPtr)->dev4PrefixLen
#else /* ! TM_SINGLE_INTERFACE_HOME */
/* device Ip address */
#define tm_ip_dev_addr(devPtr, mhomeIndex) \
        (devPtr)->devIpAddrArray[mhomeIndex]
/* device Ip mask */
#define tm_ip_dev_mask(devPtr, mhomeIndex) \
        (devPtr)->devNetMaskArray[mhomeIndex]
/* device virtual channel */
#define tm_ip_dev_vchan(devPtr, mhomeIndex)  \
        (devPtr)->devVChanArray[mhomeIndex]
/* device directed broadcast Ip address */
#define tm_ip_dev_dbroad_addr(devPtr, mhomeIndex) \
        (devPtr)->devDBroadIpAddrArray[mhomeIndex]
/* device Ip address */
#define tm_ip_dev_conf_flag(devPtr, mhomeIndex) \
        (devPtr)->devIpConfFlagArray[mhomeIndex]
#define tm_4_ip_dev_prefixLen(devPtr, mhomeIndex) \
        (devPtr)->dev4PrefixLenArray[mhomeIndex]
#endif /* ! TM_SINGLE_INTERFACE_HOME */

/* From routing entry to device Ip address */
#ifdef TM_SINGLE_INTERFACE_HOME
#define tm_ip_rte_dev_addr(rtePtr) \
        tm_ip_dev_addr((rtePtr)->rteDevPtr, 0)
#else /* TM_SINGLE_INTERFACE_HOME */
#define tm_ip_rte_dev_addr(rtePtr) \
        tm_ip_dev_addr((rtePtr)->rteDevPtr, (rtePtr)->rteMhomeIndex)
#endif /* TM_SINGLE_INTERFACE_HOME */

/*
 * Router macros
 */

/* routing entry flags */
#define TM_RTE_PMTU_DISC    (tt16Bit)0x0001 /* Path MTU discovery entry */
#define TM_RTE_INDIRECT     (tt16Bit)0x0002 /* Indirect route (gateway) */
/* Path Mtu Discovery not allowed on routing entry */
#define TM_RTE_NO_PMTU_DISC (tt16Bit)0x0004
/* Arp entry (enet address field is used) */
#define TM_RTE_LINK_LAYER   (tt16Bit)0x0008
/* Route is local (interface IP address) */
#define TM_RTE_LOCAL        (tt16Bit)0x0010
/* Route is static (added by the user) */
#define TM_RTE_STATIC       (tt16Bit)0x0020
/* route created by RIP */
#define TM_RTE_RIP          (tt16Bit)0x0040
/* route created by ICMP redirect */
#define TM_RTE_REDIRECT     (tt16Bit)0x0080
/* route created by Router Discovery */
#define TM_RTE_RDISC        (tt16Bit)0x0100
/* local route can be cloned (for ARP/permanent entries) */
#define TM_RTE_CLONE        (tt16Bit)0x0200
/*
 * cloned entry (ARP entry/multicast entry/ broadcast entry/ loop back entry)
 */
#define TM_RTE_CLONED       (tt16Bit)0x0400
/* cloned entry is an ARP entry */
#define TM_RTE_ARP          (tt16Bit)0x0800
/* output through this route rejected */
#define TM_RTE_REJECT       (tt16Bit)0x1000
/* route created by SNMP manager */
#define TM_RTE_SNMP         (tt16Bit)0x2000
/* Entry is mapping to multicast ethernet address */
#define TM_RTE_MCAST        (tt16Bit)0x4000
/* Entry is mapping to broadcast ethernet address */
#define TM_RTE_BCAST        (tt16Bit)0x8000
/* create bits */
#define TM_RTE_CREATE_MASK  ((tt16Bit)(  TM_RTE_STATIC | TM_RTE_REDIRECT \
                                        | TM_RTE_PMTU_DISC \
                                        | TM_RTE_RIP | TM_RTE_RDISC \
                                        | TM_RTE_LOCAL | TM_RTE_ARP \
                                        | TM_RTE_SNMP ))
#define TM_RTE_ROUTE_DYNAMIC \
       ( (tt16Bit)(   TM_RTE_REDIRECT | TM_RTE_RIP \
                    | TM_RTE_RDISC    | TM_RTE_SNMP ) )

#define TM_RTE_DYNAMIC \
       ( (tt16Bit)(   TM_RTE_REDIRECT | TM_RTE_RIP | TM_RTE_PMTU_DISC \
                    | TM_RTE_RDISC    | TM_RTE_SNMP ) )

/*
 * Additional routing entry flags (set in rteFlags2)
 */
#define TM_RTE2_UP         (tt16Bit)0x0001 /* route is up */
/* Leaf is a host entry (netmask all 1's) */
#define TM_RTE2_HOST       (tt16Bit)0x0004
#define TM_RTE2_INVALID    (tt16Bit)0x0008 /* route is invalid */

#ifndef TM_RT_TIMER_RESOLUTION
/* timer resolution for aging RIP, REDIRECT, and router discovery entries */
#define TM_RT_TIMER_RESOLUTION TM_UL(30000)  /* 30 seconds in milliseconds */
#endif /* TM_RT_TIMER_RESOLUTION */

/* Max IP MTU allowed */
#define TM_PMTU_MAX              (tt16Bit)65535U

/*
 * Arp cache interface commands, used for arpmCommand in ttArpMapping
 * Need to be a flag value.
 */
/* The SNMP Agent code expects the command names to be the following: */
/* Delete ARP entry by IP address */
#define TM_ARP_DEL_BY_IP      (tt8Bit)0x1
/* IPv4 Delete ARP entry by Phys address */
#define TM_ARP_DEL_BY_PHYS    (tt8Bit)0x2
/* Get ARP entry by Ip address */
#define TM_ARP_GET_BY_IP      (tt8Bit)0x4
/* IPv4 Get ARP entry by Physical address */
#define TM_ARP_GET_BY_PHYS    (tt8Bit)0x8
/* Change ARP entry ttl */
#define TM_ARP_SET_TTL        (tt8Bit)0x10
/* Change ARP entry physical address */
#define TM_ARP_SET_PHYS       (tt8Bit)0x20
/* check wether an ARP entry exists */
#define TM_ARP_CHK_BY_IP      (tt8Bit)0x40
/* All flags set */
#define TM_ARP_CMD_SET                                                      \
 (tt8Bit)(   TM_ARP_DEL_BY_IP | TM_ARP_DEL_BY_PHYS | TM_ARP_GET_BY_IP       \
           | TM_ARP_GET_BY_PHYS | TM_ARP_SET_TTL | TM_ARP_SET_PHYS          \
           | TM_ARP_CHK_BY_IP )
/* Last valid command */
#define TM_ARP_LAST_COMMAND   TM_ARP_CHK_BY_IP
/* NOTE: high-order bit (value 0x80) is reserved, used for TM_6_ARP_CMD_FLAG */

/* IPv4-specific command codes */
#define TM_4_ARP_DEL_BY_IP    TM_ARP_DEL_BY_IP
#define TM_4_ARP_DEL_BY_PHYS  TM_ARP_DEL_BY_PHYS
#define TM_4_ARP_GET_BY_IP    TM_ARP_GET_BY_IP
#define TM_4_ARP_GET_BY_PHYS  TM_ARP_GET_BY_PHYS
#define TM_4_ARP_SET_TTL      TM_ARP_SET_TTL
#define TM_4_ARP_SET_PHYS     TM_ARP_SET_PHYS
#define TM_4_ARP_CHK_BY_IP    TM_ARP_CHK_BY_IP


/*
 * ICMP macros
 */
/* Minimum size of an ICMP header */
#define TM_4PAK_ICMP_MIN_LEN        tm_packed_byte_count(8)
#define TM_4PAK_ICMP_ERROR_LEN(iphdrlen) \
    (TM_4PAK_ICMP_MIN_LEN + iphdrlen + tm_packed_byte_count(8))

/* Maximum ICMP error message length: 8(ICMP header)+TM_MAX_IP_HEADER(60)+8 */
#define TM_ICMP_MAX_ERROR_MSG    76

#define TM_ICMP_ERROR            -1 /* ICMP error */
/* ICMP code/type */
#define TM_ICMP_TYPE_PARM_PROB   (tt8Bit)12 /* parameter problem */
/* default code for parameter problem */
#define TM_ICMP_CODE_PARM        (tt8Bit)0
#define TM_ICMP_CODE_PARM_OPTION (tt8Bit)1 /* option is missing */

#define TM_ICMP_TYPE_UNREACH     (tt8Bit)3 /* destination unreachable */
#define TM_ICMP_CODE_NET         (tt8Bit)0 /* net unreachable*/
#define TM_ICMP_CODE_HOST        (tt8Bit)1 /* host unreachable*/
#define TM_ICMP_CODE_PROTO       (tt8Bit)2 /* protocol unreachable */
#define TM_ICMP_CODE_PORT        (tt8Bit)3 /* port unreachable */
#define TM_ICMP_CODE_FRAG        (tt8Bit)4 /* fragment needed and DF set */
#define TM_ICMP_CODE_SRC         (tt8Bit)5 /* source route failed */
/* ICMP destination unreachable code network unknown */
#define TM_ICMP_CODE_NET_UNKWN   (tt8Bit)6
/* ICMP destination unreachable code host unknown */
#define TM_ICMP_CODE_HOST_UNKWN  (tt8Bit)7
/* ICMP destination unreachable code source host isolated */
#define TM_ICMP_CODE_SHOST_ISOL  (tt8Bit)8
/* ICMP destination unreachable code network access prohibited */
#define TM_ICMP_CODE_NET_PROHB   (tt8Bit)9
/* ICMP destination unreachable code host access prohibited */
#define TM_ICMP_CODE_HOST_PROHB  (tt8Bit)10
/* ICMP destination unreachable code network unreachable for tos */
#define TM_ICMP_CODE_NET_TOS     (tt8Bit)11
/* ICMP destination unreachable code host unreachable for tos */
#define TM_ICMP_CODE_HOST_TOS    (tt8Bit)12


/* time exceeded for IP datagram */
#define TM_ICMP_TYPE_TIME_XCEED  (tt8Bit)11
#define TM_ICMP_CODE_XCEED_TRANS (tt8Bit)0 /* time exceeded in transit */
#define TM_ICMP_CODE_XCEED_REASS (tt8Bit)1 /* time exceeded in reassembly */

#define TM_ICMP_TYPE_REDIRECT       (tt8Bit)5 /* ICMP redirect */

/* ICMP source quench type message (4) */
#define TM_ICMP_TYPE_SRC_QNCH       (tt8Bit)4

/* ICMP echo request type message (8) */
#define TM_ICMP_TYPE_ECHO_REQ       (tt8Bit)8

/* ICMP echo reply type message  */
#define TM_ICMP_TYPE_ECHO_RPL       (tt8Bit)0

/* ICMP information request type message */
#define TM_ICMP_TYPE_INFO_REQ       (tt8Bit)15
/* ICMP information reply type message */
#define TM_ICMP_TYPE_INFO_RPL       (tt8Bit)16

/* ICMP timestamp request (13) */
#define TM_ICMP_TYPE_TIME_REQ       (tt8Bit)13
/* ICMP timestamp reply (13) */
#define TM_ICMP_TYPE_TIME_RPL       (tt8Bit)14

/* ICMP address mask request */
#define TM_ICMP_TYPE_MASK_REQ       (tt8Bit)17

/* ICMP address mask reply */
#define TM_ICMP_TYPE_MASK_RPL       (tt8Bit)18

/* ICMP router discovery advertisement */
#define TM_ICMP_TYPE_RDISC_ADV      (tt8Bit)9
/* ICMP router discovery solicitations */
#define TM_ICMP_TYPE_RDISC_SOL      (tt8Bit)10

#define TM_ICMP_TYPE_MS_SYNC_REQ    (tt8Bit)0xA5
#define TM_ICMP_TYPE_MS_SYNC_RPL    (tt8Bit)0xA6

#define TM_MS_SYNC_1                tm_const_htons(0xFEAE)
#define TM_MS_SYNC_2                tm_const_htons(0xCAD7)
#define TM_MS_SYNC_3                tm_const_htons(0xA8CD)
#define TM_MS_SYNC_SIZE             (ttPktLen)6

/* ICMP Identification field for non socket ICMP calls */
#define TM_ICMP_KERNEL_IDENT        0xFFFFU

/* security related ICMP message, per RFC 2521 */
#define TM_ICMP_TYPE_SECURITY_FAILURES   (tt8Bit)40
/* authentication failed */
#define TM_ICMP_CODE_AUTH_FAIL           (tt8Bit)1
/* Decompression Failed */
#define TM_ICMP_CODE_DECOMP_FAIL         (tt8Bit)2
/* Decryption Failed */
#define TM_ICMP_CODE_DECRYPT_FAIL        (tt8Bit)3
/* Need Authentication */
#define TM_ICMP_CODE_NEED_AUTHENTICATION (tt8Bit)4
/* Need Authorization */
#define TM_ICMP_CODE_NEED_AUTHORIZATION  (tt8Bit)5

/*
 * IGMP protocol
 */


/* Link layer mcast function command: Add or Delete */
#define TM_LNK_ADD_MCAST             0
#define TM_LNK_DEL_MCAST             1

#ifndef TM_ETHER_MCAST_MAX_ENTRIES
/* Maximum number of Ethenet Mcast entries that can be allocated */
#define TM_ETHER_MCAST_MAX_ENTRIES  (TM_BUF_Q1_SIZE/8)
#endif /* TM_ETHER_MCAST_MAX_ENTRIES */

/*
 * BOOTP, DHCP macros
 */

/*
 * Boot entry type (either automatic configuration), or user request.
 */
#define TM_BT_CONF (tt8Bit)0
#define TM_BT_USER (tt8Bit)1

/*
 * DHCP parameters set by the user
 */
#define TM_BT_USER_SET_CLIENTID  (tt8Bit)0x1
#define TM_BT_USER_SET_IPADDRESS (tt8Bit)0x2
#define TM_BT_SUPPRESS_CLIENT_ID (tt8Bit)0x4

#ifndef TM_BOOT_RETRIES
/* Number of retries + 1 for BOOTP request / DHCP messages */
#define TM_BOOT_RETRIES               6
#endif /* TM_BOOT_RETRIES */

#ifndef TM_BOOT_INIT_TIMEOUT
/* Default initial timeout value for DHCP/BOOTP retries. (in seconds) */
#define TM_BOOT_INIT_TIMEOUT          4
#endif /* TM_BOOT_INIT_TIMEOUT */

/* BOOTP or DHCP request stored in bootOp */
#define TM_BOOTREQUEST                1
/* BOOTP or DHCP reply stored in bootOp */
#define TM_BOOTREPLY                  2
/* BOOTP or DHCP client hardware address array size */
#define TM_BOOTHADDR_SIZE             16

/* DHCP options field minimum size (used in ttBootHeader structure) */
#define TM_4PAK_DHCP_OPTIONS_LEN      tm_packed_byte_count(312)

/* BOOTP vendor field minimum size */
#define TM_BOOTVEND_SIZE              64
/* DHCP options minimum size */
#define TM_DHCP_OPTIONS_SIZE          312

/*
 * BOOTP/DHCP tag in Vend/Options area
 */
/* padding fields */
#define TM_BOOT_PAD_TAG         (tt8Bit)0
/* end of usable data */
#define TM_BOOT_END_TAG         (tt8Bit)255
/* subnet mask tag */
#define TM_SUBNETMASK_TAG       (tt8Bit)1
/* router tag */
#define TM_ROUTER_TAG           (tt8Bit)3
/* List of domain name servers (use for DNS)*/
#define TM_DOMAINSERVER_TAG     (tt8Bit)6
/* Boot File size in multiple of 512 bytes */
#define TM_BOOTFILESIZE_TAG     (tt8Bit)13
/* domain name use for DNS */
#define TM_DOMAINNAME_TAG       (tt8Bit)15
/* Enable/disable IP forwarding at the IP level (tvIpForward) */
#define TM_IPFORWARD_TAG        (tt8Bit)19
/* Net bios Name Server (NBNS) option */
#define TM_NBNS_TAG             (tt8Bit)44
/* Fully Qualified Domain Name */
#define TM_FQDN_TAG             (tt8Bit)81

/*
 * Maximum BOOTP/DHCP packet we are ready to receive when fragmentation in
 * on
 */
#define TM_MAX_BOOT_PACKET          4096
/*
 * BOOTP header size (only used to allocate BOOTP buffer)
 * (236 bytes + 64 bytes == 300 bytes)
 */
#define TM_BOOTP_HEADER_SIZE        300
/*
 * BOOTP/DHCP minimum header size (used to verify length sent by servers)
 * (236 + 8 == 244 bytes)
 */
#define TM_BOOTP_MIN_HEADER_SIZE    244
/*
 * DHCP header size (only used to allocate DHCP buffer)
 * (236 bytes + 312 bytes == 548 bytes)
 */
#define TM_DHCP_HEADER_SIZE         548

/* Tags in options field */
/* Tag code size */
#define TM_BOOT_TAGCODE_SIZE          1
/* Tag code + Tag len size */
#define TM_BOOT_TAGCODELEN_SIZE       2
/* Offset of Tag value field (after Tag Code + Tag Len fields) */
#define TM_BOOT_TAG_OFFSET            TM_BOOT_TAGCODELEN_SIZE

#ifdef TM_LITTLE_ENDIAN

/* BOOTP client UDP port in network byte order */
#define TM_BOOTPC_PORT                0x4400    /* 68 */
/* BOOTP server or relay agent UDP port in network byte order */
#define TM_BOOTPS_PORT                0x4300    /* 67 */
/*
 * RFC 2132:
 * When used with BOOTP, the first four octets of the vendor information
 * field have been assigned to the "magic cookie" (as suggested in RFC
 * 951). This field identifies the mode in which the succeeding data is
 * to be interpreted.  The value of the magic cookie is the 4 octet
 * dotted decimal 99.130.83.99 (or hexadecimal number 63.82.53.63) in
 * network byte order.
 * (RFC2131 also states that the same magic cookie is used for DHCP)
 */
#define TM_BOOT_MAGIC                  TM_UL(0x63538263)

#else /* TM_BIG_ENDIAN */

/* BOOTP client UDP port in network byte order */
#define TM_BOOTPC_PORT                0x0044    /* 68 */
/* BOOTP server or relay agent UDP port in network byte order */
#define TM_BOOTPS_PORT                0x0043    /* 67 */
/*
 * RFC 2132:
 * When used with BOOTP, the first four octets of the vendor information
 * field have been assigned to the "magic cookie" (as suggested in RFC
 * 951). This field identifies the mode in which the succeeding data is
 * to be interpreted.  The value of the magic cookie is the 4 octet
 * dotted decimal 99.130.83.99 (or hexadecimal number 63.82.53.63) in
 * network byte order.
 * (RFC2131 also states that the same magic cookie is used for DHCP)
 */
#define TM_BOOT_MAGIC                 TM_UL(0x63825363)

#endif /* TM_LITTLE_ENDIAN */

/*
 * DHCP Client states. Used as index in the DHCP state transition matrix.
 * Those values should not be changed, as ordering of these values is used
 * in the code.
 */
#define TM_DHCPS_INIT        0
#define TM_DHCPS_SELECTING   1
#define TM_DHCPS_REQUESTING  2
#define TM_DHCPS_INITREBOOT  3
#define TM_DHCPS_REBOOTING   4
#define TM_DHCPS_BOUND       5
#define TM_DHCPS_RENEWING    6
#define TM_DHCPS_REBINDING   7

/*
 * Flags for btLeaseFlags
 */
#define TM_BTF_LEASE_TIME   (tt8Bit)0x01
#define TM_BTF_T1           (tt8Bit)0x02
#define TM_BTF_T2           (tt8Bit)0x04
#define TM_BTF_LEASE        (tt8Bit)0x08


/*
 * PPP macros
 */
/* Default MRU value. [RFC2460].R5:30 */
#define TM_PPP_DEF_MRU                  1500
/* Default open failure timeout, in seconds */
#define TM_PPP_DEF_OPEN_FAIL_TIMEOUT    15

#ifndef TM_PPP_DEF_SEND_SIZE
/*
 * Default number of bytes copied to PPP buffer, before it is flushed
 * to the driver
 */
#define TM_PPP_DEF_SEND_SIZE            1
#endif /* TM_PPP_DEF_SEND_SIZE */

#ifndef TM_PPP_DEF_BUF_SIZE
/* Default maximum pre-allocated PPP buffer size */
#define TM_PPP_DEF_BUF_SIZE             20
#endif /* TM_PPP_DEF_BUF_SIZE */

#define TM_CHAP_MAX_VALUE_SIZE             48 /* msv1=8, msv2=16 */
#define TM_MSCHAP_RESPONSE_SIZE            24
#define TM_MSCHAP_RESPONSE_PACKET_SIZE     49 /*24+24+1*/
#define TM_MSCHAP_CHANGEPASSWORD_SIZE    1118
#define TM_MSCHAP_V2_CHALLENGE_SIZE        16
#define TM_MSCHAP_V1_CHALLENGE_SIZE         8

/*
 * Maximum size of a LCP or IPCP configuration request.  Used for checking
 * the validity of a Conf-Ack.  (must be divisible by 4)
 */
#define TM_PPP_MAX_CONF_REQ 28

/*
 * SLIP macros
 */
#define TM_SLIP_MTU 1006 /* per RFC1055 */

#ifndef TM_SLIP_DEF_SEND_SIZE
/*
 * Default number of bytes copied to SLIP buffer, before it is flused
 * to the driver
 */
#define TM_SLIP_DEF_SEND_SIZE    1
#endif /* TM_SLIP_DEF_SEND_SIZE */

#ifndef TM_SLIP_DEF_BUF_SIZE
/* Default maximum pre-allocated SLIP buffer size */
#define TM_SLIP_DEF_BUF_SIZE    80
#endif /* TM_SLIP_DEF_BUF_SIZE */

/*
 * Common macros between UDP and TCP
 */

#define TM_4PAK_PSEUDO_HDR_LEN         tm_packed_byte_count(12)

/*
 * UDP macros
 */

/* Sizeof UDP header. (8 bytes or sizeof(ttUdpHeader) */
#define TM_PAK_UDP_HDR_LEN             tm_packed_byte_count(8)

/*
 * TCP macros
 */

/*
 * state of the TCP connection
 */
#define TM_TCPS_CLOSED       0
#define TM_TCPS_LISTEN       1
#define TM_TCPS_SYN_SENT     2
#define TM_TCPS_SYN_RECEIVED 3
#define TM_TCPS_ESTABLISHED  4
#define TM_TCPS_CLOSE_WAIT   5
#define TM_TCPS_FIN_WAIT_1   6
#define TM_TCPS_CLOSING      7
#define TM_TCPS_LAST_ACK     8
#define TM_TCPS_FIN_WAIT_2   9
#define TM_TCPS_TIME_WAIT    10
#define TM_TCPS_INVALID      20 /* invalid state */

/*
 * State Function events
 */
/* Active open (i.e connect) */
#define TM_TCPE_ACTIVE_OPEN            (tt8Bit)0
/* Passive open (i.e listen) */
#define TM_TCPE_PASSIVE_OPEN           (tt8Bit)1

/*
 * Maximum Timer index (in the timer array of a state vector)
 */
#define TM_TCPTM_LINGER                         7

/*
 * Tcp macros common betwen trtcp.c and trppp.c
 */

/*
 * Control bits in TCP header.
 */
#define TM_TCP_URG   ((tt8Bit)0x0020) /* Urgent pointer field significant */
#define TM_TCP_ACK   ((tt8Bit)0x0010) /* Acknowledgment field significant */
#define TM_TCP_PSH   ((tt8Bit)0x0008) /* PUSH Function */
#define TM_TCP_RST   ((tt8Bit)0x0004) /* Reset the connection */
#define TM_TCP_SYN   ((tt8Bit)0x0002) /* Synchronize sequence numbers */
#define TM_TCP_FIN   ((tt8Bit)0x0001) /* No more data from sender */

/* Sizeof TCP header without options. (20 bytes or sizeof(ttTcpHeader) */
#define TM_PAK_TCP_HDR_LEN             tm_packed_byte_count(20)

/*
 * From TCP header lower 4 bits of data offset in 32-bit words, to
 * TCP header length in bytes. (4 times data offset(lower 4 bits of 8-bit
 * tcpDataOffset), so shift right 2 (right 4 - 2 left))
 */
#define tm_tcp_hdrLen(dataOffset)   ((tt16Bit)(((tt8Bit)((dataOffset)>>2)) \
                                                                & 0x3C))
#ifdef TM_DEV_SEND_OFFLOAD
#define  tm_tcp_send_seg_offload(offloadFlags)                         \
    tm_16bit_one_bit_set( offloadFlags,                                \
                            TM_DEVO_TCP_SEGMENT_OFFLOAD_V4             \
                          | TM_DEVO_TCP_SEGMENT_OFFLOAD_V6 )
#endif /* TM_DEV_SEND_OFFLOAD */

/*
 * TCP macro common between trtcp.c and trsockex.c
 */
#define TM_TCPF2_PROBE_OOB      (tt16Bit)0x0040 /* has to be 0x40 */
/*
 * TCP macro common between trtcp.c and SSL
 */
#define TM_TCPF2_SEND_DATA      (tt16Bit)0x2000 /* has to be 0x2000 */

/*
 * TCP macro common between trtcp.c, trsockex.c, trsend.c, trrecv.c, etc.
 */
#define TM_TCPF_PACKET         (tt16Bit)TM_TCP_PACKET     /* 0x0400 */

/*
 * TCP macros common between trtcp.c, and trtcpopt.c
 */
/* Retransmit forever, even connection requests */
#define TM_TCPF_RT_FOREVER      (tt16Bit)0x0020 /* has to be 0x20 */
/*
 * This flag is set by default, and allow path MTU discovery.
 * If user wants to disable path MTU discovery, the user should
 * use the TCP_MAXSEG setsockopt()
 */
#define TM_TCPF_PMTU_DISCOVERY  (tt16Bit)0x8000 /* has bo be 0x8000 */
/* Minimum mss that can be defined */
#define TM_TCP_MIN_MSS          (TM_MAX_TCP_HDR_LEN + 4)
#ifdef TM_TCP_ANVL
/* For ANVL */
#define TM_TCP_MIN_MSS_TS_OPT   (16) /* 12 + 4 */
#define TM_TCP_MIN_MSS_NO_OPT   (4)  /* 0 + 4 */
/* End of ANVL additions */
#endif /* TM_TCP_ANVL */
/* Time Stamp length with NOOP fillers. */
#define TM_TCPO_TS_NOOP_LENGTH  (tt16Bit)12
/* User has set a listening recv window */
#define TM_TCPF3_LISTEN_RECV_WINDOW  (tt16Bit)1

/*
 * IP Header Compression macros
 */

/* IP header compression opcodes */
#define TM_IPHC_INIT          0
#define TM_IPHC_CLOSE         1
#define TM_IPHC_COMPRESS      2
#define TM_IPHC_DECOMPRESS    3

/*
 * IP header compression packet types
 * The actual values of these macros is arbitrary, as they are only used
 * internally (e.g. they are not included in an actual network header).
 * However, the values chosen correspond to the protocol numbers used by
 * PPP.  This is so PPP can pass the protocol value to IPHC directly from
 * the packet.
 * */
#define TM_IPHC_UDP_COMPRESSED  tm_const_htons(0x0065)
#define TM_IPHC_TCP_COMPRESSED  tm_const_htons(0x0063)
#define TM_IPHC_FULL_HEADER     tm_const_htons(0x0061)
#define TM_IPHC_CONTEXT_STATE   tm_const_htons(0x2065)
#define TM_IPHC_IP              tm_const_htons(0x0021)
#define TM_IPHC_IPV6            tm_const_htons(0x0057)
#define TM_IPHC_OTHER           0
#define TM_IPHC_ERROR           -1


/*
 * Mobile IPv4 macros
 */
#define tm_mobile_ip4_enb(devPtr)  \
    ((devPtr)->devMobileIp4VectPtr != (ttMobileVectPtr) 0)

#define tm_mobile_ip4_not_enb(devPtr)  \
    ((devPtr)->devMobileIp4VectPtr == (ttMobileVectPtr) 0)

/*
 * Status flags
 */

/* Bound to a foreign agent */
#define TM_MIP_ST_BOUND_FA       0x0001

/* Waiting for agent advertisement (for agent discover) */
#define TM_MIP_ST_ADV_WAIT       0x0002

/* Waiting for DHCP response */
#define TM_MIP_ST_DHCP_WAIT      0x0004

/* Using a co-located care-of address */
#define TM_MIP_ST_COLOCATE_ADDR  0x0008

/* Waiting for the user to set the home address */
#define TM_MIP_ST_HOME_ADDR      0x0010

/* Waiting for the user to set the home netmask */
#define TM_MIP_ST_HOME_NETMASK   0x0020

/* The device was opened on the home network. */
#define TM_MIP_ST_OPEN_HOME      0x0040

/* Waiting for an agent advertisement (for agent rediscover) */
#define TM_MIP_ST_RE_ADV_WAIT    0x0080

#define TM_MIP_ST_REG_FA         0x0100

#define TM_MIP_ST_FA_ROUTE       0x0200

#define TM_MIP_ST_FA_AUTH        0x0400

#define TM_MIP_ST_DHCP_MHOME     0x0800

#define TM_MIP_ST_OPEN           0x1000

#define TM_MIP_ST_AT_HOME        0x2000

/*
 * IPv4 tunneling macros
 */
/* Maximum number of times an IP packet may be decapsulated */
#define TM_IP_MAX_TUNNEL_COUNT 2

/*
 * Owner count macros
 */
/* Decrease owner count in critical section and save it to a temp variable */
#define tm_decr_owner_count(savOwnerCount, ownerCount) \
{ \
     tm_kernel_set_critical; \
     savOwnerCount = --(ownerCount); \
     tm_kernel_release_critical; \
}

/* Increase owner count in critical section */
#define tm_incr_owner_count(ownerCount) \
{ \
    tm_kernel_set_critical; \
    (ownerCount)++; \
    tm_kernel_release_critical; \
}

/*
 * Packet header length and pointer movements.
 */
/* Make the packet point to the ULP and update lengths,
 * when the current Layer header is hdrLen
 */
#define tm_pkt_ulp_hdr(packetPtr, hdrLen)                      \
{                                                              \
    packetPtr->pktChainDataLength -= tm_byte_count(hdrLen);    \
    packetPtr->pktLinkDataLength  -= tm_byte_count(hdrLen);    \
    packetPtr->pktLinkDataPtr     += hdrLen;                   \
}

/* Make the packet point to the LLP (Lower Layer Protocol) and update
 * lengths, when the lower layer header length is llpHdrLen.
 */
#define tm_pkt_llp_hdr(packetPtr, llpHdrLen)                   \
{                                                              \
    packetPtr->pktChainDataLength += tm_byte_count(llpHdrLen); \
    packetPtr->pktLinkDataLength  += tm_byte_count(llpHdrLen); \
    packetPtr->pktLinkDataPtr     -= llpHdrLen;                \
}

#if (defined(TM_USE_DRV_SCAT_RECV) || defined(TM_USE_IPV6))
/*
 * Check whether data at packetPtr->pktLinkDataPtr is contiguous for the
 * next headerLength bytes.
 */
#define tm_pkt_hdr_is_not_cont(packetPtr, headerLength)                     \
    (    (packetPtr->pktLinkExtraCount > 0)                                 \
      && (packetPtr->pktLinkDataLength < (ttPktLen)(headerLength)) )

#ifndef TM_RECV_SCAT_MIN_INCR_BUF
/*
 * Minimum increment when allocating a new shared data to make the
 * header contiguous in a scattered recv buffer.
 */
#define TM_RECV_SCAT_MIN_INCR_BUF    128
#endif /* TM_RECV_SCAT_MIN_INCR_BUF */
#endif /* TM_USE_DRV_SCAT_RECV or TM_USE_IPV6 */

/* Bound variable x between min and max */
#define tm_bound(x, min, max) \
{\
    if ((x) < (min)) \
    { \
        x = (min); \
    } \
    else \
    { \
        if ((x) > (max)) \
        { \
            x = (max); \
        } \
    } \
}

/* Odd byte data length */
#define tm_is_odd(item)       ( (((int)(item)) & 1) != 0 )


/* Non 4-byte aligned quantity */
#define tm_is_not_round(item) ( (((int)(item)) & TM_ROUND_SIZE) != 0 )

/*
 * Packet checksum macros.
 */

/*  Fold 32-bit sum to 16 bits */
#define tm_fold_sum(sum) \
    (sum = (sum & TM_UL(0xFFFF)) + ((sum>>16)&TM_UL(0xFFFF)))

/* Fold 32-bit sum to 16 bits and add possible carry from above addition */
#define tm_add_carries_sum(sum) \
{ \
    tm_fold_sum(sum); \
    if (sum > TM_UL(0xFFFF)) \
    { \
        sum -= TM_UL(0xFFFF); \
    } \
}

/*
 * Fold, and add carries so that we can safely byte swap the sum,
 * byte swap the sum
 */
#define tm_byte_swap_sum(sum) \
{ \
    tm_add_carries_sum(sum); \
    sum = sum << 8; \
}


/* Zero */
#define TM_32BIT_ZERO               TM_UL(0)
#define TM_S32BIT_ZERO              TM_L(0)

/*
 * Global magic number to indicate that tfGlobalVarsInit()
 * (via tfInitTreckOptions) has been called prior to calling tfStartTreck()
 */
#define TM_GLOBAL_MAGIC_NUMBER  TM_UL(0xf0a55a0f)

/* Null pointers */

#ifdef TM_IGMP
#define TM_SOC_IGMP_INFO_NULL_PTR    (ttSockIgmpInfoPtr)0
#endif /* TM_IGMP */
#define TM_RTE_NULL_PTR              (ttRteEntryPtr)0
#define TM_RTC_NULL_PTR              (ttRtCacheEntryPtr)0
#define TM_DEV_NULL_PTR              (ttDeviceEntryPtr)0
#define TM_LINK_NULL_PTR             (ttLinkLayerEntryPtr)0
#define TM_TMR_NULL_PTR              (ttTimerPtr)0
#define TM_LOC_NULL_PTR              (ttCountSemPtr)0
#define TM_PND_NULL_PTR              (ttCountSemPtr)0
#define TM_LOCK_ENTRY_NULL_PTR       (ttLockEntryPtr)0
#define TM_RCYL_NULL_PTR             (ttRcylPtr)0
#define TM_PACKET_NULL_PTR           (ttPacketPtr)0
#define TM_8BIT_NULL_PTR             (tt8BitPtr)0
#define TM_16BIT_NULL_PTR            (tt16BitPtr)0
#define TM_32BIT_NULL_PTR            (tt32BitPtr)0
#define TM_4_IPADDR_NULL_PTR         (tt4IpAddressPtr)0
#define TM_SHARED_DATA_NULL_PTR      (ttSharedDataPtr)0
#define TM_SOCKET_ENTRY_NULL_PTR     (ttSocketEntryPtr)0
#define TM_SOCKET_ENTRY_NULL_PTRPTR  (ttSocketEntryPtrPtr)0
#define TM_PEND_ENTRY_NULL_PTR       (ttPendEntryPtr)0
#define TM_VOID_NULL_PTR             (ttVoidPtr)0
#define TM_VOID_NULL_PTRPTR          (ttVoidPtrPtr)0
#define TM_TCPVECT_NULL_PTR          (ttTcpVectPtr)0
#define TM_RES_ENTRY_NULL_PTR        (ttResEntryPtr)0
#define TM_LL_NOTIFY_FUNC_NULL_PTR   (ttLnkNotifyFuncPtr)0
#define TM_LL_RESOLVE_FUNC_NULL_PTR  (ttLnkResolveFuncPtr)0
#define TM_LL_POST_FUNC_NULL_PTR     (ttLnkPostFuncPtr)0
#define TM_LL_MCAST_FUNC_NULL_PTR    (ttLnkMcastFuncPtr)0
#define TM_LL_HDR_COMP_FUNC_NULL_PTR (ttLnkHdrCompressFuncPtr) 0
#define TM_LOG_CTRL_BLK_NULL_PTR     (ttLogCtrlBlkPtr) 0
#define TM_LOG_MSG_HDR_NULL_PTR      (ttLogMsgHdrPtr) 0
#define TM_IP_ADDR_ENTRY_NULL_PTR    (ttIpAddrEntryPtr) 0
#define TM_ANC_HDR_NULL_PTR          (ttAncHdrPtr)0
#define TM_ANC_DATA_NULL_PTR         (ttAncDataPtr)0
#define TM_6_BINDING_NULL_PTR        (tt6BindingEntryPtr)0
#define TM_LOCK_NULL_PTR             (ttLockEntryPtr)0

/* support for debug, error and trace logging to circular buffer */
#ifndef TM_DEF_MAX_LOG_MSG_LEN
#define TM_DEF_MAX_LOG_MSG_LEN  256
#endif /* ! TM_DEF_MAX_LOG_MSG_LEN */
#ifndef TM_DEF_NUM_LOG_MSGS
#define TM_DEF_NUM_LOG_MSGS     25
#endif /* ! TM_DEF_NUM_LOG_MSGS */
#define TM_LOG_MSG_HDR_PAD_LEN  tm_align(TM_DEF_MAX_LOG_MSG_LEN)

#ifdef TM_MEMORY_LOGGING
#define TM_MEMORY_MSG_PREFIX "M "
#if ((defined(TM_KERNEL_WIN32_X86) || defined(TM_KERNEL_VISUAL_X86)) \
      && (!defined(TM_TRECK_DEMO)))
#define tm_memory_log0(formatString) \
    (void) tfKernelPrint(TM_PRINT_TYPE_INFO,formatString);
#define tm_memory_log1(formatString, arg1) \
    (void) tfKernelPrint(TM_PRINT_TYPE_INFO,formatString, arg1);
#define tm_memory_log2(formatString, arg1, arg2) \
    (void) tfKernelPrint(TM_PRINT_TYPE_INFO, \
        formatString, arg1, arg2);
#define tm_memory_log3(formatString, arg1, arg2, arg3) \
    (void) tfKernelPrint(TM_PRINT_TYPE_INFO, \
        formatString, arg1, arg2, arg3);
#define tm_memory_log4(formatString, arg1, arg2, arg3, arg4) \
    (void) tfKernelPrint(TM_PRINT_TYPE_INFO, \
        formatString, arg1, arg2, arg3, arg4);
#define tm_memory_log5(formatString, arg1, arg2, arg3, arg4, arg5) \
    (void) tfKernelPrint(TM_PRINT_TYPE_INFO, \
        formatString, arg1, arg2, arg3, arg4, arg5);
#define tm_memory_log6(formatString, arg1, arg2, arg3, arg4, arg5, arg6) \
    (void) tfKernelPrint(TM_PRINT_TYPE_INFO, \
        formatString, arg1, arg2, arg3, arg4, arg5, arg6);
#define tm_memory_log7(formatString, arg1, arg2, arg3, arg4, arg5, arg6, arg7) \
    (void) tfKernelPrint(TM_PRINT_TYPE_INFO, \
        formatString, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
#define tm_memory_log8(formatString, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8) \
    (void) tfKernelPrint(TM_PRINT_TYPE_INFO, \
        formatString, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
#else /* !TM_TRECK_DEMO &&(TM_KERNEL_WIN32_X86 | TM_KERNEL_VISUAL_X86) */
#define tm_memory_log0(formatString) \
    (void) tfLogMsg(tm_context(tvLogCtrlBlkPtr), TM_MEMORY_MSG_PREFIX, \
        formatString);
#define tm_memory_log1(formatString, arg1) \
    (void) tfLogMsg(tm_context(tvLogCtrlBlkPtr), TM_MEMORY_MSG_PREFIX, \
        formatString, arg1);
#define tm_memory_log2(formatString, arg1, arg2) \
    (void) tfLogMsg(tm_context(tvLogCtrlBlkPtr), TM_MEMORY_MSG_PREFIX, \
        formatString, arg1, arg2);
#define tm_memory_log3(formatString, arg1, arg2, arg3) \
    (void) tfLogMsg(tm_context(tvLogCtrlBlkPtr), TM_MEMORY_MSG_PREFIX, \
        formatString, arg1, arg2, arg3);
#define tm_memory_log4(formatString, arg1, arg2, arg3, arg4) \
    (void) tfLogMsg(tm_context(tvLogCtrlBlkPtr), TM_MEMORY_MSG_PREFIX, \
        formatString, arg1, arg2, arg3, arg4);
#define tm_memory_log5(formatString, arg1, arg2, arg3, arg4, arg5) \
    (void) tfLogMsg(tm_context(tvLogCtrlBlkPtr), TM_MEMORY_MSG_PREFIX, \
        formatString, arg1, arg2, arg3, arg4, arg5);
#define tm_memory_log6(formatString, arg1, arg2, arg3, arg4, arg5, arg6) \
    (void) tfLogMsg(tm_context(tvLogCtrlBlkPtr), TM_MEMORY_MSG_PREFIX, \
        formatString, arg1, arg2, arg3, arg4, arg5, arg6);
#define tm_memory_log7(formatString, arg1, arg2, arg3, arg4, arg5, arg6, arg7) \
    (void) tfLogMsg(tm_context(tvLogCtrlBlkPtr), TM_MEMORY_MSG_PREFIX, \
        formatString, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
#define tm_memory_log8(formatString, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8) \
    (void) tfLogMsg(tm_context(tvLogCtrlBlkPtr), TM_MEMORY_MSG_PREFIX, \
        formatString, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
#endif /* !TM_TRECK_DEMO &&(TM_KERNEL_WIN32_X86 | TM_KERNEL_VISUAL_X86) */
#else  /* !TM_MEMORY_LOGGING */
#define tm_memory_log0(formatString)
#define tm_memory_log1(formatString, arg1)
#define tm_memory_log2(formatString, arg1, arg2)
#define tm_memory_log3(formatString, arg1, arg2, arg3)
#define tm_memory_log4(formatString, arg1, arg2, arg3, arg4)
#define tm_memory_log5(formatString, arg1, arg2, arg3, arg4, arg5)
#define tm_memory_log6(formatString, arg1, arg2, arg3, arg4, arg5, arg6)
#define tm_memory_log7(formatString, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define tm_memory_log8(formatString, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)
#endif /* !TM_MEMORY_LOGGING */

#ifdef TM_DEBUG_LOGGING
#define TM_DEBUG_MSG_PREFIX "D "
#if ((defined(TM_KERNEL_WIN32_X86) || defined(TM_KERNEL_VISUAL_X86)) \
      && (!defined(TM_TRECK_DEMO)))
#define tm_debug_log0(formatString) \
    (void) tfKernelPrint(TM_PRINT_TYPE_INFO,formatString);
#define tm_debug_log1(formatString, arg1) \
    (void) tfKernelPrint(TM_PRINT_TYPE_INFO,formatString, arg1);
#define tm_debug_log2(formatString, arg1, arg2) \
    (void) tfKernelPrint(TM_PRINT_TYPE_INFO, \
        formatString, arg1, arg2);
#define tm_debug_log3(formatString, arg1, arg2, arg3) \
    (void) tfKernelPrint(TM_PRINT_TYPE_INFO, \
        formatString, arg1, arg2, arg3);
#define tm_debug_log4(formatString, arg1, arg2, arg3, arg4) \
    (void) tfKernelPrint(TM_PRINT_TYPE_INFO, \
        formatString, arg1, arg2, arg3, arg4);
#define tm_debug_log5(formatString, arg1, arg2, arg3, arg4, arg5) \
    (void) tfKernelPrint(TM_PRINT_TYPE_INFO, \
        formatString, arg1, arg2, arg3, arg4, arg5);
#define tm_debug_log6(formatString, arg1, arg2, arg3, arg4, arg5, arg6) \
    (void) tfKernelPrint(TM_PRINT_TYPE_INFO, \
        formatString, arg1, arg2, arg3, arg4, arg5, arg6);
#define tm_debug_log7(formatString, arg1, arg2, arg3, arg4, arg5, arg6, arg7) \
    (void) tfKernelPrint(TM_PRINT_TYPE_INFO, \
        formatString, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
#define tm_debug_log8(formatString, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8) \
    (void) tfKernelPrint(TM_PRINT_TYPE_INFO, \
        formatString, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
#else /* !TM_TRECK_DEMO &&(TM_KERNEL_WIN32_X86 | TM_KERNEL_VISUAL_X86) */
#define tm_debug_log0(formatString) \
    (void) tfLogMsg(tm_context(tvLogCtrlBlkPtr), TM_DEBUG_MSG_PREFIX, \
        formatString);
#define tm_debug_log1(formatString, arg1) \
    (void) tfLogMsg(tm_context(tvLogCtrlBlkPtr), TM_DEBUG_MSG_PREFIX, \
        formatString, arg1);
#define tm_debug_log2(formatString, arg1, arg2) \
    (void) tfLogMsg(tm_context(tvLogCtrlBlkPtr), TM_DEBUG_MSG_PREFIX, \
        formatString, arg1, arg2);
#define tm_debug_log3(formatString, arg1, arg2, arg3) \
    (void) tfLogMsg(tm_context(tvLogCtrlBlkPtr), TM_DEBUG_MSG_PREFIX, \
        formatString, arg1, arg2, arg3);
#define tm_debug_log4(formatString, arg1, arg2, arg3, arg4) \
    (void) tfLogMsg(tm_context(tvLogCtrlBlkPtr), TM_DEBUG_MSG_PREFIX, \
        formatString, arg1, arg2, arg3, arg4);
#define tm_debug_log5(formatString, arg1, arg2, arg3, arg4, arg5) \
    (void) tfLogMsg(tm_context(tvLogCtrlBlkPtr), TM_DEBUG_MSG_PREFIX, \
        formatString, arg1, arg2, arg3, arg4, arg5);
#define tm_debug_log6(formatString, arg1, arg2, arg3, arg4, arg5, arg6) \
    (void) tfLogMsg(tm_context(tvLogCtrlBlkPtr), TM_DEBUG_MSG_PREFIX, \
        formatString, arg1, arg2, arg3, arg4, arg5, arg6);
#define tm_debug_log7(formatString, arg1, arg2, arg3, arg4, arg5, arg6, arg7) \
    (void) tfLogMsg(tm_context(tvLogCtrlBlkPtr), TM_DEBUG_MSG_PREFIX, \
        formatString, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
#define tm_debug_log8(formatString, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8) \
    (void) tfLogMsg(tm_context(tvLogCtrlBlkPtr), TM_DEBUG_MSG_PREFIX, \
        formatString, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
#endif /* !TM_TRECK_DEMO &&(TM_KERNEL_WIN32_X86 | TM_KERNEL_VISUAL_X86) */
#else  /* !TM_DEBUG_LOGGING */
#define tm_debug_log0(formatString)
#define tm_debug_log1(formatString, arg1)
#define tm_debug_log2(formatString, arg1, arg2)
#define tm_debug_log3(formatString, arg1, arg2, arg3)
#define tm_debug_log4(formatString, arg1, arg2, arg3, arg4)
#define tm_debug_log5(formatString, arg1, arg2, arg3, arg4, arg5)
#define tm_debug_log6(formatString, arg1, arg2, arg3, arg4, arg5, arg6)
#define tm_debug_log7(formatString, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define tm_debug_log8(formatString, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)
#endif /* !TM_DEBUG_LOGGING */

#ifdef TM_ERROR_LOGGING
#define TM_ERROR_MSG_PREFIX "E "
#if ((defined(TM_KERNEL_WIN32_X86) || defined(TM_KERNEL_VISUAL_X86)) \
      && (!defined(TM_TRECK_DEMO)))
#define tm_error_log0(formatString) \
    (void) tfKernelPrint(TM_PRINT_TYPE_FAIL, formatString);
#define tm_error_log1(formatString, arg1) \
    (void) tfKernelPrint(TM_PRINT_TYPE_FAIL, formatString, arg1);
#define tm_error_log2(formatString, arg1, arg2) \
    (void) tfKernelPrint(TM_PRINT_TYPE_FAIL, formatString, arg1, arg2);
#define tm_error_log3(formatString, arg1, arg2, arg3) \
    (void) tfKernelPrint(TM_PRINT_TYPE_FAIL, TM_ERROR_MSG_PREFIX, \
        formatString, arg1, arg2, arg3);
#define tm_error_log4(formatString, arg1, arg2, arg3, arg4) \
    (void) tfKernelPrint(TM_PRINT_TYPE_FAIL, TM_ERROR_MSG_PREFIX, \
        formatString, arg1, arg2, arg3, arg4);
#define tm_error_log5(formatString, arg1, arg2, arg3, arg4, arg5) \
    (void) tfKernelPrint(TM_PRINT_TYPE_FAIL, TM_ERROR_MSG_PREFIX, \
        formatString, arg1, arg2, arg3, arg4, arg5);
#define tm_error_log6(formatString, arg1, arg2, arg3, arg4, arg5, arg6) \
    (void) tfKernelPrint(TM_PRINT_TYPE_FAIL, TM_ERROR_MSG_PREFIX, \
        formatString, arg1, arg2, arg3, arg4, arg5, arg6);
#define tm_error_log7(formatString, arg1, arg2, arg3, arg4, arg5, arg6, arg7) \
    (void) tfKernelPrint(TM_PRINT_TYPE_FAIL, TM_ERROR_MSG_PREFIX, \
        formatString, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
#define tm_error_log8(formatString, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8) \
    (void) tfKernelPrint(TM_PRINT_TYPE_FAIL, TM_ERROR_MSG_PREFIX, \
        formatString, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
#else /* !TM_TRECK_DEMO &&(TM_KERNEL_WIN32_X86 | TM_KERNEL_VISUAL_X86) */
#define tm_error_log0(formatString) \
    (void) tfLogMsg(tm_context(tvLogCtrlBlkPtr), TM_ERROR_MSG_PREFIX, \
        formatString);
#define tm_error_log1(formatString, arg1) \
    (void) tfLogMsg(tm_context(tvLogCtrlBlkPtr), TM_ERROR_MSG_PREFIX, \
        formatString, arg1);
#define tm_error_log2(formatString, arg1, arg2) \
    (void) tfLogMsg(tm_context(tvLogCtrlBlkPtr), TM_ERROR_MSG_PREFIX, \
        formatString, arg1, arg2);
#define tm_error_log3(formatString, arg1, arg2, arg3) \
    (void) tfLogMsg(tm_context(tvLogCtrlBlkPtr), TM_ERROR_MSG_PREFIX, \
        formatString, arg1, arg2, arg3);
#define tm_error_log4(formatString, arg1, arg2, arg3, arg4) \
    (void) tfLogMsg(tm_context(tvLogCtrlBlkPtr), TM_ERROR_MSG_PREFIX, \
        formatString, arg1, arg2, arg3, arg4);
#define tm_error_log5(formatString, arg1, arg2, arg3, arg4, arg5) \
    (void) tfLogMsg(tm_context(tvLogCtrlBlkPtr), TM_ERROR_MSG_PREFIX, \
        formatString, arg1, arg2, arg3, arg4, arg5);
#define tm_error_log6(formatString, arg1, arg2, arg3, arg4, arg5, arg6) \
    (void) tfLogMsg(tm_context(tvLogCtrlBlkPtr), TM_ERROR_MSG_PREFIX, \
        formatString, arg1, arg2, arg3, arg4, arg5, arg6);
#define tm_error_log7(formatString, arg1, arg2, arg3, arg4, arg5, arg6, arg7) \
    (void) tfLogMsg(tm_context(tvLogCtrlBlkPtr), TM_ERROR_MSG_PREFIX, \
        formatString, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
#define tm_error_log8(formatString, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8) \
    (void) tfLogMsg(tm_context(tvLogCtrlBlkPtr), TM_ERROR_MSG_PREFIX, \
        formatString, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
#endif /* !TM_TRECK_DEMO &&(TM_KERNEL_WIN32_X86 | TM_KERNEL_VISUAL_X86) */
#else  /* !TM_ERROR_LOGGING */
#define tm_error_log0(formatString)
#define tm_error_log1(formatString, arg1)
#define tm_error_log2(formatString, arg1, arg2)
#define tm_error_log3(formatString, arg1, arg2, arg3)
#define tm_error_log4(formatString, arg1, arg2, arg3, arg4)
#define tm_error_log5(formatString, arg1, arg2, arg3, arg4, arg5)
#define tm_error_log6(formatString, arg1, arg2, arg3, arg4, arg5, arg6)
#define tm_error_log7(formatString, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define tm_error_log8(formatString, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)
#endif /* !TM_ERROR_LOGGING */

#ifdef TM_TRACE_LOGGING
#define TM_TRACE_MSG_PREFIX "T "
#define tm_trace_func_entry(functionName) \
    (void) tfLogMsg(tm_context(tvLogCtrlBlkPtr), TM_TRACE_MSG_PREFIX, \
        "Enter " #functionName );

#define tm_trace_func_exit(functionName) \
    tfLogMsg(tm_context(tvLogCtrlBlkPtr), TM_TRACE_MSG_PREFIX, \
        "Leave routine " #functionName ", line %d", __LINE__);

#define tm_trace_func_exit_rc(functionName, returnCode) \
    (void) tfLogMsg(tm_context(tvLogCtrlBlkPtr), TM_TRACE_MSG_PREFIX, \
        "Leave " #functionName ", line %d, rc(%d)", __LINE__, \
        returnCode);

#else  /* !TM_TRACE_LOGGING */
#define tm_trace_func_entry(functionName)
#define tm_trace_func_exit(functionName)
#define tm_trace_func_exit_rc(functionName, returnCode)
#endif /* !TM_TRACE_LOGGING */

/* Get device driver specific pointer */
#define tm_device_get_pointer(interfaceHandle) \
            (((ttDeviceEntryPtr)interfaceHandle)->devDriverPtr)

/* Set device driver specific pointer */
#define tm_device_set_pointer(interfaceHandle, pointer) \
            ((ttDeviceEntryPtr)interfaceHandle)->devDriverPtr = pointer

/* Get device network id */
#define tm_device_get_network_id(interfaceHandle) \
            (((ttDeviceEntryPtr)interfaceHandle)->devNetworkId)

/* Set device network id */
#define tm_device_set_network_id(interfaceHandle, id) \
            ((ttDeviceEntryPtr)interfaceHandle)->devNetworkId = id

#ifndef TM_MAX_NUMBER_DEVICES
/* Default maximum number of devices that can be installed */
#define TM_MAX_NUMBER_DEVICES       4
#endif /* TM_MAX_NUMBER_DEVICES */

/* tm_4_netmask
   Description: IPv4 macro used to calculate the IPv4 netmask from a
        specified prefix length.
   Parameters:
        (IN) int prefixLen:
            the prefix length in bits
        (OUT) tt32Bit netmask:
            the IPv4 netmask to set
*/
#define tm_4_netmask(prefixLen, netmask) \
{ \
    tm_assert(tm_4_netmask, ((prefixLen >= 0) && (prefixLen <= 32))); \
    netmask = tvRt32ContiguousBitsPtr[prefixLen]; \
}

/* tm_4_ip_addr
 * Description: Returns a 32-bit IPv4 address from a ttIpAddress type.  In IPv6
 *     mode, returns the 32-bit IPv4 address from an IPv4 mapped IPv6 address;
 *     in IPv4 mode just returns the whole address.
 * Parameters:
 *     (IN) ttIpAddress ipAddr:
 *         the ttIpAddress type to return the IPv4 address from.
 */
#define tm_4_ip_addr(ipAddr) ipAddr

/* tm_4_cmp_ip_addr, tm_6_cmp_ip_addr
 * Description:
 *      Compares two IPv4(IPv6) addresses.
 *      Returns a negative, null or positive number depending if ipAddr1 is
 *      lower than, equal to or greater than ipAddr2.
 * Parameters:
 *      (IN) ttIpAddress ipAddr1: first Ip Address to compare.
 *      (IN) ttIpAddress ipAddr2: second Ip Address to compare.
 */
#define tm_4_cmp_ip_addr(ipAddr1, ipAddr2)   tm_memcmp(&(ipAddr1),            \
                                                       &(ipAddr2),            \
                                                       sizeof(tt32Bit))



#ifndef TM_IP_DEF_MAX_HDR_LEN
/* The maximum expected header overhead (in bytes), including the base packet
   header, IPsec and extension headers, but not including the link-layer
   header. */
/* IPv4-only w/out IPsec: base IPv4 header */
#define TM_IP_DEF_MAX_HDR_LEN   TM_4_IP_MIN_HDR_LEN
#endif /* ! TM_IP_DEF_MAX_HDR_LEN */

#define TM_PAK_IP_DEF_MAX_HDR_LEN tm_packed_byte_count(TM_IP_DEF_MAX_HDR_LEN)

/*
 * Following macro used for user application send buffers
 * (Note ping application sends ICMP header as data, TCP header allocated
 *  somewhere else)
 * Note that since we are not sending any IP options, we use 20 (instead
 * of 60) for IPv4 IP header size. We use 40 for IPv6 header size.
 * Maximum UDP header send size: 8( UDP)+20(IP)+16(LL) == 44, for IPv4
 * Maximum UDP header send size: 8( UDP)+40(IP)+16(LL) == 64, for IPv6
 */
#define TM_MAX_SEND_HEADERS_SIZE \
        (TM_MAX_UDP_HDR_LEN+TM_IP_DEF_MAX_HDR_LEN+TM_LL_MAX_HDR_LEN)

/*
 * Following macro used for zero copy send buffers
 * Note that since we are not sending any IP options, we use 20 (instead
 * of 60) for IPv4 IP header size. We use 40 for IPv6 header size.
 * Maximum TCP header send size: 64(TCP)+20(IP)+16(LL) == 100
 * Maximum TCP header send size: 64(TCP)+40(IP)+16(LL) == 120
 */
#define TM_MAX_SEND_TCP_HEADERS_SIZE \
        (TM_MAX_TCP_HDR_LEN+TM_IP_DEF_MAX_HDR_LEN+TM_LL_MAX_HDR_LEN)

#ifndef TM_IP_DEF_MAX_TRAILER_LEN
/*
 * The maximum expected trailer overhead (in bytes), including
 * IPsec trailers.
 */
/* w/out IPsec: no trailer */
#define TM_IP_DEF_MAX_TRAILER_LEN       0
#endif /* ! TM_IP_DEF_MAX_TRAILER_LEN */

#define TM_PAK_IP_DEF_MAX_TRAILER_LEN \
    tm_packed_byte_count(TM_IP_DEF_MAX_TRAILER_LEN)

/* Flag for tfTunnelConfig: Don't add route when configuring tunnel. */
#define TM_TUNNEL_NO_ROUTE_FLAG         0x1

/*
 * Device flags 2 (devFlag2)
 */

/* When acquiring an IPv4 address via DHCP/BOOTP, do not automatically
   configure an IPv4-compatible IPv6 address. */
#define TM_6_DEVF2_NO_DHCP_CONF         0x0001


/*
 * Crypto related macros. Don't put this into trsecapi.h, because
 * trppp.c won't include trsecapi.h
 */
#define TM_MD4_HASH_SIZE                    16
#define TM_MD5_HASH_SIZE                    16
#define TM_SHA1_HASH_SIZE                   20
#define TM_MAX_HASH_SIZE                    20
#define TM_SHA256_HASH_SIZE                 32
#define TM_RIPEMD_HASH_SIZE                 20 
#define TM_HMAC_BLOCK_SIZE                  64
#define TM_CRYPTO_HMAC_INNER_PAD            0x36
#define TM_CRYPTO_HMAC_OUTER_PAD            0x5c




#ifndef TM_USE_PPP
#undef TM_PPP_LQM
#else /* TM_USE_PPP is defined */
#define TM_USE_MD5
#endif /* TM_USE_PPP */

#if ((!defined(TM_USE_PPP)) && (!defined(TM_USE_IPSEC)) && \
     (!defined(TM_USE_HTTPD)) && (!defined(TM_USE_SSL_SERVER)) \
     && (!defined(TM_USE_SSL_CLIENT)))
#undef TM_USE_MD5
#endif

#undef TM_USE_SHA1

#undef TM_USE_DES
#undef TM_USE_PKI


/* PPP - MSCHAP stuff */
#undef TM_USE_MD4
#undef TM_USE_ARCFOUR



#ifdef TM_IKE_PHASE1_NULL_ENCRYPT
#ifndef TM_IKE_NULL_ENCRYPT_SUPPORT
#define TM_IKE_NULL_ENCRYPT_SUPPORT
#endif /* TM_IKE_NULL_ENCRYPT_SUPPORT */
#endif /* TM_IKE_PHASE1_NULL_ENCRYPT */

#ifdef TM_USE_IPSEC_TASK
#ifdef TM_PEND_POST_NEEDED
#define TM_PEND_EVENT_NEEDED
#endif /*TM_PEND_POST_NEEDED*/
#endif /*TM_USE_IPSEC_TASK*/

/* undefine IPsec feature macros, if we aren't using IPsec */
#undef TM_USE_IKE /* Cannot have IKE without IPSEC */
#undef TM_IPSEC_INCOMING_ICMP_BYPASS
#undef TM_6_IPSEC_ICMPV6_NDMLD_BYPASS
#undef TM_IPSEC_INCOMING_ICMP_NOSRCCHK

#undef TM_USE_BLOWFISH
#undef TM_USE_RC5
#undef TM_USE_CAST128
#undef TM_USE_AES
#undef TM_USE_TWOFISH
#undef TM_USE_3DES

/* #else originally we put here that ifdef TM_USE_IPSEC, we must
 * define software copy of TM_USE_MD5 and TM_USE_SHA1. This is not
 * true, because if we always use hardware crypto engine, we dont
 * need any of them
 */

#if(!defined(TM_USE_IKE) && !defined(TM_SSL_USE_EPHEMERAL_DH))
#undef TM_PUBKEY_USE_DIFFIEHELLMAN
#endif /* TM_USE_IKE */

#undef TM_PUBKEY_USE_DSA
#undef TM_PUBKEY_USE_RSA

#if (defined(TM_PUBKEY_USE_DSA) || defined(TM_PUBKEY_USE_RSA))
/*RSA and DSA must needs SHA1 */
#define TM_USE_SHA1
#endif /* RSA | DSA */

#ifdef TM_AUTO_TEST
/* Don't reject, we are testing ...*/
#undef TM_SSL_REJECT_EXPORTABLE_CIPHER
#endif /* TM_AUTO_TEST*/



/* print types for printing a string in the right location with good color */
#define TM_PRINT_TYPE_TEST    1
#define TM_PRINT_TYPE_PASS    2
#define TM_PRINT_TYPE_FAIL    3
#define TM_PRINT_TYPE_INFO    4
#define TM_PRINT_TYPE_WARN    5
#define TM_PRINT_TYPE_STAT    6

/* these two macros can be used to get a cmsghdr out of ttAncHdr,
 * not the public API msghdr
 */

/*
 * returns a pointer to the first msghdr in the ancillary data
 * buffer associated with the passed ancHdr.
 */
#define TM_ANC_FIRSTHDR(ancHdr) \
        ( (ancHdr)->ancDataLen >= sizeof(struct ttAncHdr) ? \
          (struct cmsghdr *)(ancHdr)->ancData : \
          (struct cmsghdr *)NULL )

/* given pointer to struct cmsghdr, return pointer to next cmsghdr */
#define TM_ANC_NXTHDR(ancHdr, cmsg) \
        ( ((cmsg) == NULL) ? TM_ANC_FIRSTHDR(ancHdr) : \
          (((u_char *)(cmsg) + CMSG_ALIGN((cmsg)->cmsg_len) \
                             + CMSG_ALIGN(sizeof(struct cmsghdr)) > \
            (u_char *)((ancHdr)->ancData) + (ancHdr)->ancDataLen) ? \
           (struct cmsghdr *)NULL : \
           (struct cmsghdr *)((   u_char *)(cmsg) \
                                + CMSG_ALIGN((cmsg)->cmsg_len))) )



/* length of a line in the base63 encoded buffer */
#define TM_BASE64_LINE_LEN  76

/* Time string format for HTTP Date header */
#define TM_TIME_FORMAT_HTTP     0

/* Time string format for SMTP Date header */
#define TM_TIME_FORMAT_SMTP     1



#ifndef TM_FS_FILE_BLOCK_SIZE
/* The default file system read block size */
#define TM_FS_FILE_BLOCK_SIZE    512
#endif  /* TM_FS_FILE_BLOCK_SIZE */

#ifdef __cplusplus
}
#endif

#endif /* _TRMACRO_H_ */

/* Processed by Treck Shipping Tool v2.37 
 * 2004/11/17 12:28:53      For: Marvell
 * File Name: trmacro.h     File Revision: 4.1 
 * Project Name: tcpip.pj   Project Release: 4.1.2.1
 * Input checksum: 21019    Output checksum: 16365
 */
