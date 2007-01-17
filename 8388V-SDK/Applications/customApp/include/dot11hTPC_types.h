#ifndef _DOT11H_TPC_TYPES_H_
#define _DOT11H_TPC_TYPES_H_

typedef struct LocalPowerConstraint_t {
    Boolean valid;
    uint8 channel;
    uint8 dBm;
} LocalPowerConstraint_t;

#endif
