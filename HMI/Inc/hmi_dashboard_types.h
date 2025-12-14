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
    DISPLAY_NOT_UPDTATING_EVENT= 0U,
    DISPLAY_UPDATING_EVENT,
    DISPLAY_UPDATING_DATA
}hmi_dashboard_update_display_stataus_t;

typedef enum
{
    FIELD_VOLTAGE = 0U,
    FIELD_CURRENT,
    NUMBER_OF_FIELDS,
}hmi_dash_fields_edits_t;

typedef enum
{
    CURSOR_STATE_IDLE = 0U,
    CURSOR_STATE_SHOW_NUMBER,
    CURSOR_STATE_WAIT_HIDE_DELAY,
    CURSOR_STATE_HIDE_NUMBER,
    CURSOR_STATE_WAIT_SHOW_DELAY
}hmi_cursor_blnk_state_t;

typedef enum
{
    CURSOR_BLINK_ON = 0U,
    CURSOR_BLINK_OFF
}hmi_cursor_state_t;

typedef enum
{
    OUT_STATUS_OFF = 0U,
    OUT_STATUS_RUN  
}hmi_dash_out_stauts_t;

typedef struct 
{   
    hmi_cursor_blnk_state_t blnk_state;
    hmi_cursor_state_t state;
    uint32_t last_time_show_cursor;
    uint32_t last_time_hide_cursor;
    uint32_t last_time_blnk_cursor
}hmi_dash_cursor;


typedef struct 
{
    uint8_t index;
    hmi_dash_cursor cursor;
    hmi_dash_out_stauts_t out_status;
    hmi_dash_fields_edits_t field;
    hmi_dashboard_update_display_stataus_t display_update;
    uint16_t target[NUMBER_OF_FIELDS];
}hmi_dash_ctrl_t;

typedef struct 
{   
    uint8_t first_digit;
    uint8_t index;
}hmi_dash_edit_t;


typedef struct 
{
    uint8_t col;
    uint8_t row;
    uint8_t value;
}hmi_dash_set_cursor_t;






#endif