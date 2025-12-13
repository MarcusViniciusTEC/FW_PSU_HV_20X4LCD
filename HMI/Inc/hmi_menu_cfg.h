#ifndef _HMI_MENU_CFG_H_
#define _HMI_MENU_CFG_H_


#endif

#define HMI_MENU_NUMBER_OF_ITEMS 4
#define HMI_CURSOR ">"


#define hmi_menu_vector_item_default                    \
{                                                       \
    {                                                   \
        .id = HMI_MENU_ID_DASHBOARD,                    \
        .string = "1.DASHBOARD"                         \
    }                                                   \
    ,                                                   \
    {                                                   \
        .id = HMI_MENU_ID_CURRENT_RAMP,                 \
        .string ="2.CURRENT RAMP"                       \
    }                                                   \
    ,                                                   \
    {                                                   \
        .id = HMI_MENU_ID_VOLTAGE_RAMP,                 \
        .string ="3.VOLTAGE RAMP"                       \
    }                                                   \
    ,                                                   \
    {                                                   \
        .id = HMI_MENU_ID_SETTINGS,                     \
        .string ="4.SETTINGS"                           \   
    }                                                   \
}