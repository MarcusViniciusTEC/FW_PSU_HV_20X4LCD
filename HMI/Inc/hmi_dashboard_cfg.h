#ifndef __HMI_CONST_CURRENT_CFG_H
#define __HMI_CONST_CURRENT_CFG_H

#include "hmi_dashboard_types.h"

#define MIN_DIGIT 0
#define MAX_DIGIT 9

#define cursor_set_edit_default             \
{                                           \
    {/*voltage*/                            \
        {                                   \
            .col = 4,                       \
            .row = 0,                       \
            .value = 0                      \
        }                                   \
        ,                                   \
        {                                   \
            .col = 5,                       \
            .row = 0,                       \
            .value = 0                      \
        }                                   \
    }                                       \
    ,                                       \
    {/*current*/                            \
        {                                   \
            .col = 16,                      \
            .row = 0,                       \
            .value = 0                      \
        }                                   \
        ,                                   \
        {                                   \
            .col = 17,                      \
            .row = 0,                       \
            .value = 0                      \
        }                                   \
    }                                       \
}

#define cursor_set_edit_voltage_current \
{                                       \
    {                                   \
        .col = 5,                       \
        .row = 0                        \
    }                                   \
    ,                                   \
    {                                   \
        .col = 6,                       \
        .row = 0                        \
    }                                   \
}

#endif