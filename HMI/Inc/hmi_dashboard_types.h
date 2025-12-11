#ifndef __HMI_CONST_CURRENT_TYPES_H
#define __HMI_CONST_CURRENT_TYPES_H

#include "hmi_dashboard_cfg.h"
#include "stdint.h"

typedef enum
{
    INDEX_FIRST_DIGIT = 0U,
    INDEX_SECOND_DIGIT,
    INDEX_THIRD_DIGIT,
    INDEX_FOURTH_DIGIT,
    NUMBER_OF_INDEX_DIGITS
}hmi_dash_dig_pos_t;

typedef enum
{
    FIELD_VOLTAGE = 0U,
    FIELD_CURRENT,
    NUMBER_OF_FIELDS,
}hmi_dash_fields_edits_t;


typedef struct 
{
    uint8_t index;
    hmi_dash_fields_edits_t field;
}hmi_dash_ctrl_t;

typedef struct 
{   
    uint8_t value;
    uint8_t index;
}hmi_dash_edit_t;





typedef struct 
{
    uint8_t col;
    uint8_t row;
    uint8_t value;
}hmi_dash_set_cursor_t;






#endif