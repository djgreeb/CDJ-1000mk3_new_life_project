#ifndef __FT5336_H
#define __FT5336_H
//------------------------------------------
#include "stm32f7xx_hal.h"
#include <string.h>
#include <stdlib.h>
//------------------------------------------
#define TS_SWAP_NONE ((uint8_t) 0x01)
#define TS_SWAP_X ((uint8_t) 0x02)
#define TS_SWAP_Y ((uint8_t) 0x04)
#define TS_SWAP_XY ((uint8_t) 0x08)
//------------------------------------------
#define TS_I2C_ADDRESS ((uint16_t)0x70)
#define FT5336_ID_VALUE ((uint8_t)0x51)
#define FT5336_TD_STAT_REG ((uint8_t)0x02)
#define FT5336_GMODE_REG ((uint8_t)0xA4)
#define FT5336_CHIP_ID_REG ((uint8_t)0xA8)
#define FT5336_G_MODE_INTERRUPT_POLLING ((uint8_t)0x00)
#define FT5336_G_MODE_INTERRUPT_MASK ((uint8_t)0x03)
#define FT5336_TD_STAT_MASK ((uint8_t)0x0F)
#define FT5336_G_MODE_INTERRUPT_SHIFT ((uint8_t)0x00)
#define FT5336_GEST_ID_REG ((uint8_t)0x01)
//------------------------------------------
/* Possible values of FT5336_GEST_ID_REG */
#define FT5336_GEST_ID_NO_GESTURE ((uint8_t)0x00)
#define FT5336_GEST_ID_MOVE_UP ((uint8_t)0x10)
#define FT5336_GEST_ID_MOVE_RIGHT ((uint8_t)0x14)
#define FT5336_GEST_ID_MOVE_DOWN ((uint8_t)0x18)
#define FT5336_GEST_ID_MOVE_LEFT ((uint8_t)0x1C)
#define FT5336_GEST_ID_SINGLE_CLICK ((uint8_t)0x20)
#define FT5336_GEST_ID_DOUBLE_CLICK ((uint8_t)0x22)
#define FT5336_GEST_ID_ROTATE_CLOCKWISE ((uint8_t)0x28)
#define FT5336_GEST_ID_ROTATE_C_CLOCKWISE ((uint8_t)0x29)
#define FT5336_GEST_ID_ZOOM_IN ((uint8_t)0x40)
#define FT5336_GEST_ID_ZOOM_OUT ((uint8_t)0x49)
#define FT5336_I2C_NOT_INITIALIZED ((uint8_t)0x00)
#define FT5336_I2C_INITIALIZED ((uint8_t)0x01)
#define FT5336_MAX_DETECTABLE_TOUCH ((uint8_t)0x05)
//------------------------------------------
#define FT5336_TOUCH_POS_MSB_MASK ((uint8_t)0x0F)
#define FT5336_TOUCH_POS_MSB_SHIFT ((uint8_t)0x00)
#define FT5336_TOUCH_POS_LSB_MASK ((uint8_t)0xFF)
#define FT5336_TOUCH_POS_LSB_SHIFT ((uint8_t)0x00)
//------------------------------------------
#define FT5336_P1_XH_REG ((uint8_t)0x03)
#define FT5336_P1_XL_REG ((uint8_t)0x04)
#define FT5336_P1_YH_REG ((uint8_t)0x05)
#define FT5336_P1_YL_REG ((uint8_t)0x06)
#define FT5336_P2_XH_REG ((uint8_t)0x09)
#define FT5336_P2_XL_REG ((uint8_t)0x0A)
#define FT5336_P2_YH_REG ((uint8_t)0x0B)
#define FT5336_P2_YL_REG ((uint8_t)0x0C)
#define FT5336_P3_XH_REG ((uint8_t)0x0F)
#define FT5336_P3_XL_REG ((uint8_t)0x10)
#define FT5336_P3_YH_REG ((uint8_t)0x11)
#define FT5336_P3_YL_REG ((uint8_t)0x12)
#define FT5336_P4_XH_REG ((uint8_t)0x15)
#define FT5336_P4_XL_REG ((uint8_t)0x16)
#define FT5336_P4_YH_REG ((uint8_t)0x17)
#define FT5336_P4_YL_REG ((uint8_t)0x18)
#define FT5336_P5_XH_REG ((uint8_t)0x1B)
#define FT5336_P5_XL_REG ((uint8_t)0x1C)
#define FT5336_P5_YH_REG ((uint8_t)0x1D)
#define FT5336_P5_YL_REG ((uint8_t)0x1E)
#define FT5336_P6_XH_REG ((uint8_t)0x21)
#define FT5336_P6_XL_REG ((uint8_t)0x22)
#define FT5336_P6_YH_REG ((uint8_t)0x23)
#define FT5336_P6_YL_REG ((uint8_t)0x24)
#define FT5336_P7_XH_REG ((uint8_t)0x27)
#define FT5336_P7_XL_REG ((uint8_t)0x28)
#define FT5336_P7_YH_REG ((uint8_t)0x29)
#define FT5336_P7_YL_REG ((uint8_t)0x2A)
#define FT5336_P8_XH_REG ((uint8_t)0x2D)
#define FT5336_P8_XL_REG ((uint8_t)0x2E)
#define FT5336_P8_YH_REG ((uint8_t)0x2F)
#define FT5336_P8_YL_REG ((uint8_t)0x30)
#define FT5336_P9_XH_REG ((uint8_t)0x33)
#define FT5336_P9_XL_REG ((uint8_t)0x34)
#define FT5336_P9_YH_REG ((uint8_t)0x35)
#define FT5336_P9_YL_REG ((uint8_t)0x36)
#define FT5336_P10_XH_REG ((uint8_t)0x39)
#define FT5336_P10_XL_REG ((uint8_t)0x3A)
#define FT5336_P10_YH_REG ((uint8_t)0x3B)
#define FT5336_P10_YL_REG ((uint8_t)0x3C)
//------------------------------------------
#define FT5336_P1_WEIGHT_REG ((uint8_t)0x07)
#define FT5336_P1_MISC_REG ((uint8_t)0x08)
#define FT5336_P2_WEIGHT_REG ((uint8_t)0x0D)
#define FT5336_P2_MISC_REG ((uint8_t)0x0E)
#define FT5336_P3_WEIGHT_REG ((uint8_t)0x13)
#define FT5336_P3_MISC_REG ((uint8_t)0x14)
#define FT5336_P4_WEIGHT_REG ((uint8_t)0x19)
#define FT5336_P4_MISC_REG ((uint8_t)0x1A)
#define FT5336_P5_WEIGHT_REG ((uint8_t)0x1F)
#define FT5336_P5_MISC_REG ((uint8_t)0x20)
#define FT5336_P6_WEIGHT_REG ((uint8_t)0x25)
#define FT5336_P6_MISC_REG ((uint8_t)0x26)
#define FT5336_P7_WEIGHT_REG ((uint8_t)0x2B)
#define FT5336_P7_MISC_REG ((uint8_t)0x2C)
#define FT5336_P8_WEIGHT_REG ((uint8_t)0x31)
#define FT5336_P8_MISC_REG ((uint8_t)0x32)
#define FT5336_P9_WEIGHT_REG ((uint8_t)0x37)
#define FT5336_P9_MISC_REG ((uint8_t)0x38)
#define FT5336_P10_WEIGHT_REG ((uint8_t)0x3D)
#define FT5336_P10_MISC_REG ((uint8_t)0x3E)
//------------------------------------------
#define FT5336_TOUCH_EVT_FLAG_PRESS_DOWN ((uint8_t)0x00)
#define FT5336_TOUCH_EVT_FLAG_LIFT_UP ((uint8_t)0x01)
#define FT5336_TOUCH_EVT_FLAG_CONTACT ((uint8_t)0x02)
#define FT5336_TOUCH_EVT_FLAG_NO_EVENT ((uint8_t)0x03)
//------------------------------------------
#define FT5336_TOUCH_EVT_FLAG_SHIFT ((uint8_t)0x06)
#define FT5336_TOUCH_EVT_FLAG_MASK ((uint8_t)(3 << FT5336_TOUCH_EVT_FLAG_SHIFT))
#define FT5336_TOUCH_WEIGHT_MASK ((uint8_t)0xFF)
#define FT5336_TOUCH_WEIGHT_SHIFT ((uint8_t)0x00)
#define FT5336_TOUCH_AREA_MASK ((uint8_t)(0x04 << 4))
#define FT5336_TOUCH_AREA_SHIFT ((uint8_t)0x04)
//------------------------------------------
#define TS_MAX_NB_TOUCH ((uint32_t) FT5336_MAX_DETECTABLE_TOUCH)
//------------------------------------------
 typedef struct
{
  uint8_t i2cInitialized;
  uint8_t currActiveTouchNb;
  uint8_t currActiveTouchIdx;
} ft5336_handle_TypeDef;
//------------------------------------------
typedef struct
{
 uint8_t touchDetected; /*!< Total number of active touches detected at last scan */
 uint16_t touchX[TS_MAX_NB_TOUCH]; /*!< Touch X[0], X[1] coordinates on 12 bits */
 uint16_t touchY[TS_MAX_NB_TOUCH]; /*!< Touch Y[0], Y[1] coordinates on 12 bits */
 uint8_t touchWeight[TS_MAX_NB_TOUCH]; /*!< Touch_Weight[0], Touch_Weight[1] : weight property of touches */
 uint8_t touchEventId[TS_MAX_NB_TOUCH]; /*!< Touch_EventId[0], Touch_EventId[1] : take value of type @ref TS_TouchEventTypeDef */
 uint8_t touchArea[TS_MAX_NB_TOUCH]; /*!< Touch_Area[0], Touch_Area[1] : touch area of each touch */
 uint32_t gestureId; /*!< type of gesture detected : take value of type @ref TS_GestureIdTypeDef */
} TS_StateTypeDef;
//------------------------------------------
typedef enum
{
 TS_OK = 0x00, /*!< Touch Ok */
 TS_ERROR = 0x01, /*!< Touch Error */
 TS_TIMEOUT = 0x02, /*!< Touch Timeout */
 TS_DEVICE_NOT_FOUND = 0x03 /*!< Touchscreen device not found */
}TS_StatusTypeDef;
//------------------------------------------
typedef enum
{
 TOUCH_EVENT_NO_EVT = 0x00, /*!< Touch Event : undetermined */
 TOUCH_EVENT_PRESS_DOWN = 0x01, /*!< Touch Event Press Down */
 TOUCH_EVENT_LIFT_UP = 0x02, /*!< Touch Event Lift Up */
 TOUCH_EVENT_CONTACT = 0x03, /*!< Touch Event Contact */
 TOUCH_EVENT_NB_MAX = 0x04 /*!< max number of touch events kind */
} TS_TouchEventTypeDef;
//------------------------------------------
typedef enum
{
 GEST_ID_NO_GESTURE = 0x00, /*!< Gesture not defined / recognized */
 GEST_ID_MOVE_UP = 0x01, /*!< Gesture Move Up */
 GEST_ID_MOVE_RIGHT = 0x02, /*!< Gesture Move Right */
 GEST_ID_MOVE_DOWN = 0x03, /*!< Gesture Move Down */
 GEST_ID_MOVE_LEFT = 0x04, /*!< Gesture Move Left */
 GEST_ID_ZOOM_IN = 0x05, /*!< Gesture Zoom In */
 GEST_ID_ZOOM_OUT = 0x06, /*!< Gesture Zoom Out */
 GEST_ID_NB_MAX = 0x07 /*!< max number of gesture id */
} TS_GestureIdTypeDef;
//------------------------------------------
void Touch_Ini(void);
uint8_t TS_GetState(TS_StateTypeDef *TS_State);
//------------------------------------------
#endif /* __FT5336_H */
