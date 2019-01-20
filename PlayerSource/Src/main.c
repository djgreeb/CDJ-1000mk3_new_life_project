/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f7xx_hal.h"
#include "dma.h"
#include "dma2d.h"
#include "fatfs.h"
#include "i2c.h"
#include "ltdc.h"
#include "quadspi.h"
#include "sai.h"
#include "sdmmc.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"
#include "fmc.h"

/* USER CODE BEGIN Includes */

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		This firmware for F7 chip, 32F746G DISCOVERY board.
//		Added support for some Rekordbox functions, display of static and dynamic waveform, audio interpolation process. 
//
//		Added support for control buttons with led backlight.
//		IMPORTANT!!! 
//		After regenerating the code with Cube, make these corrections:
//		File sai.c
//		 - hsai_BlockA2.FrameInit.ActiveFrameLength = 32;
//		File dma2d.c, comment 2 lines:
//		 - //  hdma2d.LayerCfg[1].AlphaInverted = DMA2D_REGULAR_ALPHA;
//		 - //  hdma2d.LayerCfg[1].RedBlueSwap = DMA2D_RB_REGULAR;
//		File arm_math.h, add line:
//		 - #define ARM_MATH_CM7
//		init this functions after LCD_Layers initialization:	
//			-	MX_DMA_Init();
//  		-	MX_SPI1_Init();
//			-	HAL_SPI_TransmitReceive_DMA(&hspi1, Tbuffer, Rbuffer, 27);
//
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	ver. 0.61a
//		- optimized browser function
//		- added dark_green color in browser for text on white line cursor 
//	ver. 0.62a
//		- optimized DATABASE_PARSER finction
//		- DATABASE_PARSER using WFORMDYNAMIC massive only 
//	ver. 0.63a
//		- optimized LOAD_TRACK function. Now the PCM[] massive is not used.
//		- PCM[] converted in uint16_t
//		- optimized audio processing for new uint16_t PCM[]. Now the calculation of 1 sample is 5us instead of 7.5us 
//	ver. 0.64a
//		- fixed joint in circular buffer in audio processing
//		- optimized ring buffer size for increased audio processing performance
//	ver. 0.65a
//		- optimized audio processing (read SDRAM). Now the calculation of 1 sample is 4us 
//	 	- added SD_LED blink
//		- added encoder blink when loading track
//	ver. 0.66a
//		- added touch screen control
//	ver. 0.67a
//		- added basic function needle search
//		- fixed function BSP_LCD_DisplayStringAt. 
//		-	added TRANSPARENT_MODE for text. 
//		- changed method of determination the font with dynamic width (Font15P) and other fonts with static width.  
//		- optimized touch controller driver
//		- create audioparcer for SEEK function
//	ver. 0.68a
//		- added QSPI flash support
//		- added animation startup logo
//	ver. 0.69a
//		- added REKORDBOX logo on startup
//		- added tag list 
//		- added internal function for RedrawWaveforms process 
//	ver. 0.70a
//		- added INFO mode in BROWSER and TAG LIST menu
//		- added internal functions for optimize code
//	ver. 0.71a
//		- improved the functionality of the TAG LIST menu
//	ver. 0.72a
//		- added symbol red check mark for TAG LIST
//		- improved the functionality of the TAG LIST (added TAG TRACK/REMOVE button)
//		- bugs fixes internal functions browser and tag list
//		- bugs fixes NAVIGATOR function
//		- added loading track from TAG LIST
//	ver. 0.73a
//		- added UTILITY window
//		- added long press MENU button for entering to UTILITY
//		- fixed check WAV header
//		-	optimized process add/delete tracks in TAG LIST
//	ver. 0.74a
//		- added SPI transfer for CDJ-1000mk3 panel
//	ver. 0.75a
//		- added reading cues and memory pionts from ANLZXXXX.DAT
//		- fixed bug SPI DMA (see note*) 
//	ver. 0.76a
//		- added PLAY, TEMPO, TEMPO RESET, JOG MODE buttons
//		- added pitch slider 4 ranges
//		- added calculation tempo and bpm after pitch change
//	ver. 0.77a
//		- added PLAY blink led, CUE led
//		- improved jog vinyl mode (inertial process)
//		- added TRACK SEARCH buttons
//		- improved ShowTempo function
//		- added TIME MODE button, REMAIN MODE
//		- added CDJ JOG MODE work
//		- optimized pitch bend coeficients aka CDJ-1000mk3
//		- optimized VINYL MODE precision jog step
//	ver. 0.78a
//		- improved static scroll UI
//		- added BEATGRID massive
//		- shift up 1px static information, static waveform 
//		- beatgrid support
//		- improved Phase Meter
//	ver. 0.79a
//		- improved BPM calculating after pitch change
//		- added BPMGRID for tracks with variable BPM 
//	ver. 0.80a
//		- added Font13D for phase bars
//		- added slip mode marker on jog display
//		- added master player ICON
//		- added slip mode red button and slip mode jog illumination
//		- fixed bug spin jog at maximum speed (variable overflow)
//		- added first SLIP MODE functions with audio processing
//		- added animation icon and gradient for phase bars
//		- improved jog pitch bend in reverse mode
//		- improved jog in slip mode
//	ver. 0.81a
//		- added CUE blink
//		- improved time mode button code
//		- added VINYL RELEASE/START and TOUCH/BREAKE mode 
//		-	optimized SPI-DMA transfer process
//		-	optimized potenciometer's curve for VINYL RELEASE/START and TOUCH/BREAKE mode
//	ver. 0.82a
//		- added track nubmer and status (playing or played) in INFO mode
//		- added filling buffer step sequencer for optimize time gaps
//		- fixed filling buffer step sequencer algoritm
//	ver. 0.83a
//		- improved the work of the function of static and dynamic waveforms
//		- added "remain/foward time style" for progress bar
//	ver. 0.84b
//		- added blink progress bar when the remaining time is less than 30sec
//		- fixed DrawMinuteMarkers function
//		- improved performance static and dynamic waveforms (added ForceDrawVLine function)
//		- added checking device UID 
//		- fixed fatal error when deleting a track from an empty tag list
//		- the first addition of a function CUE audio
//	ver. 0.85b
//		- added CUE button process
//		- improved CUE audio process
//		- optimized pitch bend coefficients
//		- added loading of Hot Cues and Memory Cues attributes
//	ver. 0.86b
//		- added MEMORY CUE calling
//		- bugs fixes calling CUEs when jog in CDJ mode
//		- added CUE, MEMORY CUE and HOT CUE triangles on dynamic waveform  
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////		

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "stm32746g_discovery.h"
#include "stm32746g_discovery_lcd.h"
#include "ff.h"
#include "wm8994.h"


/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
#include <math.h>
#include "arm_math.h"
#include "skins.h"
#include "logo.h"
#include "ft5336.h"
#include "stm32746g_discovery_qspi.h"
extern DMA_HandleTypeDef hdma_spi1_tx;
extern DMA_HandleTypeDef hdma_spi1_rx;

///////////////////////////////////////////FatFS///////////////////
unsigned int nbytes;
FRESULT res;
FATFS fat;
FIL file;
FILINFO finfo;
FATFS FAT;
DIR directory;

/* Dynamic Waveform variables ---------------------------------------------------------*/
uint32_t all_long = 0;						//all long of Track in 0.5*frames   150 on 1 sec
uint8_t DynamicWaveformZOOM = 4;	//zoom parametr 1-2-4-8-16
char WFORMDYNAMIC[135000]; 		//__attribute__ ((section(".sdram")));; for force waveform
uint32_t BEATGRID[4096];			// beatgrid (0, 3, 7... )
uint16_t BPMGRID[4096];				// bpmgrit BPM*100
uint32_t PreviousPositionDW = 0;
uint16_t PreviousSecondsPos = 9;
uint8_t VisibleLayer = 0;
#define BROWSER 			0
#define WAVEFORM 			1
#define BROWSER_INFO 	2
#define TAG_LIST			3
#define TAG_LIST_INFO 4
#define UTILITY				5
uint8_t forcibly_redraw = 0; 
uint8_t RED_VERTICAL_LINE = 0;
uint16_t PreviousPhase = 0;
uint16_t bars = 0;
const uint32_t COLOR_MAP[8] = 
{
0xFF00009F,
0xFF2F2FAF,
0xFF4F4FBF,
0xFF6F6FCF,
0xFF8F8FDF,
0xFFBFBFEF,
0xFFDFDFFF,
0xFFFFFFFF,	
};
//const uint32_t COLOR_MAP[8] = 
//{
//0xFFFF0000,
//0xFFFF2F00,
//0xFFAF8F00,
//0xFF6FFF00,
//0xFF00FF6F,
//0xFF008FAF,
//0xFF002FFF,
//0xFF0000FF,	
//};

/* Static Waveform variables ---------------------------------------------------------*/
uint16_t previous_position_bar = 0;
char WFORMSTATIC[400];
uint8_t dSHOW = BROWSER;
uint8_t MemoryCuePyramid_ENABLE = 0;
#define DRAW_NEW_STATIC_WAVEFORM		400
#define CLEAR_WAVEFORM_ARRAY				401
#define MS_NOT_LOADED								402
#define REDRAW_IN_NREMAIN_MODE			403
#define REDRAW_IN_REMAIN_MODE				404
#define MS_ERROR										410
uint8_t DRAWN_IN_REMAIN = 0;
uint8_t need_DSW = 0;						//flag for redraw progressbar remain or nrmain mode when track long 30s
#define PBAR_COLOR_1				((uint32_t)0xFFC4C5C6)
#define PBAR_COLOR_2				((uint32_t)0xFF64656D)
#define PBAR_COLOR_3				((uint32_t)0xFF44454A)
const uint32_t WS_COLOR_MAP[2] = 
{
0xFF6F6FFF,
0xFFBFBFFF,
};
//const uint32_t WS_COLOR_MAP[2] = 
//{
//LCD_COLOR_MAGENTA,	
//LCD_COLOR_CYAN,
//};

/* HOT CUES and MEMORY variables ---------------------------------------------------------*/
#define NONE_MARK				0
#define MEMORY_MARK			1
#define HOT_CUE_A_MARK	2
#define HOT_CUE_B_MARK	3
#define HOT_CUE_C_MARK	4
uint32_t HCUE_adr[2][3] = {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};  																		//[0] => HCUE in 0.5*frames [1] => loop end in 0.5*frames   150 on 1 sec;  
uint32_t MEMORY_adr[2][10] = {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
															0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}; 	//[0] => MEMORY CUE in 0.5*frames [1] =>  MEMORY loop end in 0.5*frames   150 on 1 sec;
uint8_t	HCUE_type[3] = {0};	 			//b0 (0=cue / 1=loop); b1 (0=inactive / 1=active); 													
uint8_t	MEMORY_type[10] = {0};		//b0 (0=cue / 1=loop); b1 (0=inactive / 1=active); 

uint8_t number_of_hot_cue_points = 0;
uint8_t number_of_memory_cue_points = 0;

/* Audio processing variables ---------------------------------------------------------*/
uint32_t play_adr = 0;						//Playing adress in samples (44100 per second)
uint32_t slip_play_adr = 0;				//Playing adress for SLIP MODE in samples (44100 per second)
uint32_t sdram_adr = 0;
uint8_t SAMPLE[4];
uint16_t PCM[206][8192][2]					__attribute__ ((section(".sdram")));;
uint16_t start_adr_valid_data = 0;							//filling adress in memory
uint16_t end_adr_valid_data = 0;								//filling adress in memory ()
uint8_t filling_step = 0;
uint8_t reload = 1;
uint8_t output_level = 90;
uint8_t change_speed = 0;							//flag for RELEASE/START or TOUCH/BREAKE 
#define NO_CHANGE	0
#define NEED_UP		1
#define NEED_DOWN	2
uint32_t CUE_ADR = 0;					//REAL CUE adr in frames 150
uint8_t offset_adress = 0;			//address offset for calling CUE audio data (for work)
uint8_t mem_offset_adress = 0;	//address offset for calling CUE audio data (for memory)

uint16_t acceleration_UP = 0xFFFF;				//acceleration_UP for vinyl RELEASE/START
uint16_t acceleration_DOWN = 0xFFFF;			//acceleration_DOWN for vinyl TOUCH/BREAKE
uint16_t LOG_TABLE[8] = {1200, 128, 72, 40, 18, 15, 9, 3};		

uint16_t PCM_2[2];
int16_t LR[2][4];
float c0, c1, c2, c3, r0, r1, r2, r3;
int32_t even1, even2, odd1, odd2;
static float COEF[8] = {			//////optimal 2x
0.45868970870461956,
0.04131401926395584,
0.48068024766578432,
0.17577925564495955,
-0.246185007019907091, 
0.24614027139700284,
-0.36030925263849456,
0.10174985775982505
};
uint32_t i = 0;
uint8_t reverse = 0;
uint16_t pitch = 10000;	// 10000 = 100% step 0,01%			
uint32_t position = 0;
uint32_t slip_position = 0;
uint16_t pitch_for_slip = 10000;	// 10000 = 100% step 0,01%	
uint8_t step_position = 0;
float SAMPLE_BUFFER;
float T;


/* Rekordbox processing variables ---------------------------------------------------------*/
uint8_t playlist[768][55] 		__attribute__ ((section(".sdram")));;		//54byte - status byte
uint32_t parcser_adress[768] 	__attribute__ ((section(".sdram")));;
uint16_t TOTAL_TRACKS = 0;
uint16_t track_number = 0;
char path_export[]="0:/PIONEER/rekordbox/export.pdb";

/* Display and static information variables ---------------------------------------------------------*/
#define LCD_FRAME_BUFFER    SDRAM_DEVICE_ADDR
#define LCD_FRAME_BUFFER_2	((uint32_t)0xC0780800)
char FIRMWARE_VERSION[] = "angus";		
uint8_t lcd_status = LCD_OK;
uint8_t Buf[20]={0};
uint16_t originalBPM = 0xFFFF;						//this original BPM*100 of track (pitch = 0.00%) 
uint8_t REMAIN_ENABLE = 0;
uint32_t BLUE_BAR[4][14] = {
    0xffe5e5fb, 0xffe5e5fb, 0xffe5e5fb, 0xffe5e5fb, 0xffe2e2fb, 0xffdcdcfa, 0xffd5d5f8, 0xffc9c9f6, 0xffbbbbf4, 0xffaaaaf2, 0xff9696ef, 0xff8181ec, 0xff6c6ce8, 0xff5757e5, 
    0xffe2e2fb, 0xffe1e1fb, 0xffe0e0fb, 0xffddddf9, 0xffd9d9f9, 0xffd1d1f8, 0xffc8c8f7, 0xffbcbcf5, 0xffadadf2, 0xff9c9cf0, 0xff8888ee, 0xff7474ea, 0xff6060e6, 0xff4d4de4, 
    0xffc0c0f5, 0xffbfbff5, 0xffbcbcf5, 0xffb7b7f4, 0xffafaff3, 0xffa7a7f2, 0xff9b9bf0, 0xff8e8eee, 0xff8080ec, 0xff7171e9, 0xff6060e7, 0xff5050e4, 0xff4040e2, 0xff3232e1, 
    0xff7878ea, 0xff7878ea, 0xff7575ea, 0xff7070e9, 0xff6a6ae8, 0xff6363e7, 0xff5959e6, 0xff5050e5, 0xff4646e3, 0xff3a3ae2, 0xff3030df, 0xff2626de, 0xff1d1ddc, 0xff1414db
};
uint32_t RED_BAR[4][14] = {
    0xfffae3df, 0xfffae3df, 0xfffae3de, 0xfffae2dd, 0xfff9dfdb, 0xfff8dbd7, 0xfff6d5ce, 0xfff6ccc3, 0xfff3bfb5, 0xfff0b0a5, 0xffed9f92, 0xffea8e7d, 0xffe77d68, 0xffe46a55, 
    0xfff9dfdb, 0xfff9dfdb, 0xfff9deda, 0xfff8dcd7, 0xfff8d8d3, 0xfff6d2cc, 0xfff5cac2, 0xfff4c0b6, 0xfff1b3a8, 0xffefa497, 0xffec9484, 0xffe88371, 0xffe5725d, 0xffe3624b, 
    0xfff4c2ba, 0xfff4c2b9, 0xfff3c0b6, 0xfff3bbb1, 0xfff1b6aa, 0xfff0ada2, 0xffefa597, 0xffed998a, 0xffea8d7c, 0xffe87f6d, 0xffe5725d, 0xffe3644d, 0xffe0573e, 0xffde4b30, 
    0xffe98775, 0xffe98674, 0xffe88472, 0xffe8806c, 0xffe77b66, 0xffe67460, 0xffe46d57, 0xffe2654e, 0xffe15c44, 0xffe05339, 0xffdd492e, 0xffdc4125, 0xffda391b, 0xffd93113
};


/* Display Browser variables ---------------------------------------------------------*/
uint8_t BCurrentCursorPosition = 0;			//0...7 position
uint16_t BCurrentTrackPosition = 1;			//1....TOTAL_TRACKS-7
uint8_t ScrollLong = 142;								//5...142
uint8_t ScrollPosition = 0;							//0...142-ScrollLong
#define LCD_COLOR_DGREEN           	((uint32_t)0xFF00CD00)
#define LCD_COLOR_LIGHT_1           ((uint32_t)0xFFDFDFDF)				//color for line-cursor browser
#define LCD_COLOR_LIGHT_2           ((uint32_t)0xFFEFEFEF)				//color for line-cursor browser
#define LCD_COLOR_LIGHT_3						((uint32_t)0xFFCFCFCF)				//color for secondary line-cursor UTILITY
#define LCD_COLOR_LIGHT_4						((uint32_t)0xFFAFAFAF)				//color for secondary line-cursor UTILITY
#define LCD_COLOR_LIGHT_5						((uint32_t)0xFFBFBFBF)				//color for secondary line-cursor UTILITY
#define LCD_COLOR_DARK_1           	((uint32_t)0xFF606060)
#define LCD_COLOR_DARK_2           	((uint32_t)0xFF404040)
#define BROWSER_DOWN		0
#define BROWSER_UP			1
#define TAGLIST_DOWN		2
#define TAGLIST_UP			3
#define UTILITY_DOWN		4
#define UTILITY_UP			5

/* Display TAG LIST variables ---------------------------------------------------------*/
uint8_t TOTAL_TRACKS_IN_TAG_LIST = 0;					//0....100
uint16_t TAG_LIST_BASE[101] = {0};				//base tag list [track_number]
uint8_t TCurrentCursorPosition = 0;			//0...7 position
uint8_t TCurrentTrackPosition = 1;			//1....TOTAL_TRACKS_IN_TAG_LIST-7
uint8_t s = 0;													//counter for write-delete-add tag track

/* Display INFO variables ---------------------------------------------------------*/
uint8_t BROWSER_INFO_enable = 0;
uint8_t TAG_LIST_INFO_enable = 0;
#define LCD_COLOR_PAPER           	((uint32_t)0xFFFFFCE3)
#define LCD_COLOR_SHADOW           	((uint32_t)0xFFD4D4D4)

/* Display UTILITY variables ---------------------------------------------------------*/
uint8_t TOTAL_U_POSITIONS = 11;			
char UTILITY_BASE[11][16] = {"PLAY MODE       ",
														 "EJECT/LOAD LOCK ",
														 "AUTO CUE LEVEL  ",
														 "SLIP FLASHING   ",
														 "MIDI CHANNEL    ",
														 "AUTO STANDBY    ",
														 "LCD BRIGHTNESS  ",
														 "SCREEN SAVER    ",
														 "OUTPUT LEVEL    ",
														 "DEVICE UID      ",
														 "VERSION No.     "	};				//UTILITY list
uint32_t DEVICE_UID = 0;
uint8_t UCurrentCursorPosition = 0;			//0...7 position
uint8_t CurrentUPosition = 1;			//1....TOTAL_UTILITY_POSITIONS-7
uint8_t	countUTILITY = 0;
														 
/* Buttons variables ---------------------------------------------------------*/
uint8_t need_up = 0;
uint8_t need_down = 0;
uint8_t LOAD_pressed = 0;
uint8_t KEY_BROWSE_pressed = 0;
uint8_t KEY_SD_pressed = 0;
uint8_t KEY_TAG_LIST_pressed = 0;
uint8_t KEY_MENU_pressed = 0;
uint8_t KEY_TAG_TRACK_pressed = 0;
uint8_t	KEY_USB_pressed = 0;
uint8_t KEY_MIDI_pressed = 0;														 
uint8_t KEY_INFO_pressed = 0;
uint8_t LED_SD_timer = 0;
uint8_t timer_time = 0;
uint8_t ENCODER_LED_BLINK = 8;

/* Touch screen variables ---------------------------------------------------------*/
static TS_StateTypeDef TS_State;
static uint8_t tscnt[2]={0};
#define TS_NEEDLE_X_MIN		40
#define TS_NEEDLE_X_MAX		439
#define TS_NEEDLE_Y_MIN		235
#define TS_NEEDLE_Y_MAX		271
uint8_t needle_enable = 0;
uint16_t previous_needle_position = 0;

/* SPI transfer variables ---------------------------------------------------------*/
uint8_t Tbuffer[27] = {
	168,  119,  119,  0,  119,  119,  0,  0,  0,  1,  176,  0,  0,  0,  0,  0,  0,  0xC,  0,  0x20,  0,  0,  0,  88,  0,  0,  0};
uint8_t load_animation_enable = 0;
uint8_t Rbuffer[27]= {0};
uint16_t zi = 0;
uint8_t a = 0;
uint8_t dma_cnt = 0;
uint32_t DD;
uint16_t potenciometer_tempo;
uint32_t pitch_center;
uint16_t previous_potenciometer_tempo = 0;
uint8_t tempo_need_update = 0;
uint8_t tempo_range = 1;					//10% default
uint8_t tempo_range_need_update = 0;
uint8_t time_mode_need_update = 0;
uint8_t track_need_load = 0;
uint8_t play_enable = 0;
uint8_t inertial_rotation = 0;							//inertial rotation for jog
uint8_t keep_to_play = 0;
uint8_t PLAY_BUTTON_pressed = 0;
uint8_t CUE_BUTTON_pressed = 0;
uint8_t JOG_MODE_BUTTON_pressed = 0;
uint8_t TEMPO_RESET_BUTTON_pressed = 0;
uint8_t TEMPO_RANGE_BUTTON_pressed = 0;
uint8_t TRACK_NEXT_BUTTON_pressed = 0;
uint8_t TRACK_PREVIOUS_BUTTON_pressed = 0;
uint8_t CALL_NEXT_BUTTON_pressed = 0;
uint8_t CALL_PREVIOUS_BUTTON_pressed = 0;
uint8_t SEARCH_FF_BUTTON_pressed = 0;
uint8_t SEARCH_REW_BUTTON_pressed = 0;
uint8_t TIME_MODE_BUTTON_pressed = 0;
uint8_t SLIP_MODE_BUTTON_pressed = 0;
uint8_t REVERSE_SWITCH_pressed = 0;
uint8_t TIM_PLAY_LED = 0;
uint8_t TIM_CUE_LED = 0;
uint8_t RED_CIRCLE_CUE_ADR = 1;
uint8_t TMPSLP = 0;

uint8_t CUE_OPERATION = 0;
#define CUE_NEED_SET 1
#define CUE_NEED_CALL 2
#define MEMORY_NEED_NEXT_SET 3
#define MEMORY_NEED_PREVIOUS_SET 4
#define MEMORY_NEED_SET_PART2 5

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
void DrawMinuteMarkers(void);
void DrawZOOMGRID(void);
void DrawREKORDBOX(void);
void DrawLOGO(void);
void DrawStaticWFM(uint16_t Tpos);
void DrawTempoRange(uint8_t Range);
void DrawMemoryCuePyramid(uint8_t draw);
void ShowBPM(uint16_t bpm);
void ShowACUE(uint8_t acue);
void ShowREMAIN(uint8_t remain);
void DrawStaticInformation(void);
void ShowNEEDLE(uint8_t needle);
void ShowTempo(uint16_t tempo);
void ShowTrackNumber(uint8_t track);
void RedrawWaveforms(uint32_t position);
void DrawCueMarker(uint16_t p);														//Draw orange triangle CUE marker
void DrawMemoryMarker(uint16_t p, uint8_t type, uint32_t color);							//Draw red triangle memory marker
void DAC_Init_and_Start(uint8_t volume);
uint16_t DATABASE_PARSER(void);
uint16_t LOAD_TRACK(uint16_t TRACK_NUMBER);
void PREPARE_LOAD_TRACK(uint16_t TRACK_NUMBER);
void ShowPhaseMeter(uint16_t phase);											//Show Phase meter 0 - none 1...4 phase
void SwitchInformationLayer(uint8_t LAY);									//Switch Dynamic Waveform/Browser
void ReDrawScroll(uint16_t total_elements, uint16_t current_element_pos);
void NAVIGATOR(uint8_t UPDOWN);														//Navigate browser, TAG LIST and UTILITY	
void int_B_DRAW_ALL_LINES(void);													//internal function for Browser
void int_B_DRAW_ONE_LINE(uint8_t UPDOWN);									//internal function for Browser
void int_BI_DRAW_ALL_LINES(void);													//internal function for Browser + INFO
void int_BI_DRAW_ONE_LINE(uint8_t UPDOWN);								//internal function for Browser + INFO
void int_T_DRAW_ALL_LINES(void);													//internal function for Browser
void int_T_DRAW_ONE_LINE(uint8_t UPDOWN);									//internal function for Browser
void int_TI_DRAW_ALL_LINES(void);													//internal function for Browser + INFO
void int_TI_DRAW_ONE_LINE(uint8_t UPDOWN);								//internal function for Browser + INFO
void int_U_DRAW_ALL_LINES(void);													//internal function for Browser
void int_U_DRAW_ONE_LINE(uint8_t UPDOWN);									//internal function for Browser
void intDrawTriangle(uint8_t CurrentCursorPosition);			//draw triangle for browser with INFO
void intDrawLayer0_INFO(uint8_t CurrentCursorPosition);		//draw layer 0 for INFO BROWSER and TAGLIST
void intDrawLayer0_NOINFO(uint8_t CurrentCursorPosition);	//draw layer 0 for without INFO BROWSER and TAGLIST
void intDRAW_WAVEFORM_FRAME(uint32_t position);						//internal function for redraw waveform
void TOUCH_SCREEN_HANDLER(void);													//touch screen handler
void SEEK_AUDIOFRAME(uint32_t seek_adr);									//seek adress in samples (44100 per second)
void UTILITY_PARAMETER(uint8_t num_parameter);						//write to Buf[] name state parameter for utility
void CheckTXCRC(void);
void SET_CUE(uint32_t nf_adr);															//nf_adr = new frame adress in 1/150s
void CALL_CUE(void);
void SET_MEMORY_CUE_1(uint32_t nf_adr);
void SET_MEMORY_CUE_2(void);

void SAI2_IRQHandler(void)												////////////////////////////////AUDIO PROCESSING   44K1Hz//////////////////////////////
	{
	//HAL_GPIO_WritePin(GPIOB, LED_TAG_LIST_Pin, GPIO_PIN_SET);
	HAL_SAI_IRQHandler(&hsai_BlockA2);
	HAL_SAI_Transmit_IT(&hsai_BlockA2, SAMPLE, 2);
	
	if(Tbuffer[19]&0x8 && ((slip_play_adr+((slip_position+pitch_for_slip)/10000))<294*all_long))					//SLIP MODE ENABLE
		{
		slip_position+= pitch_for_slip;
		slip_play_adr+=slip_position/10000;	
		slip_position = slip_position%10000;	
		}
		
	position+= pitch;
	
	if(position>9999)	
		{
		step_position = position/10000;	
		if(reverse==0 && ((play_adr+step_position+3)<=(294*all_long)))					
			{
			play_adr+= step_position;	
			if(step_position==1)
				{
				LR[0][0] = LR[0][1];
				LR[1][0] = LR[1][1];
				LR[0][1] = LR[0][2];
				LR[1][1] = LR[1][2];
				LR[0][2] = LR[0][3];
				LR[1][2] = LR[1][3];					
				}
			else
				{
				sdram_adr = play_adr&0xFFFFF;						
				LR[0][0] = PCM[(sdram_adr>>13)+offset_adress][sdram_adr&0x1FFF][0];							
				LR[1][0] = PCM[(sdram_adr>>13)+offset_adress][sdram_adr&0x1FFF][1];
				sdram_adr = (play_adr+1)&0xFFFFF;
				LR[0][1] = PCM[(sdram_adr>>13)+offset_adress][sdram_adr&0x1FFF][0];								
				LR[1][1] = PCM[(sdram_adr>>13)+offset_adress][sdram_adr&0x1FFF][1];		
				sdram_adr = (play_adr+2)&0xFFFFF;
				LR[0][2] = PCM[(sdram_adr>>13)+offset_adress][sdram_adr&0x1FFF][0];									
				LR[1][2] = PCM[(sdram_adr>>13)+offset_adress][sdram_adr&0x1FFF][1];
				}
			sdram_adr = (play_adr+3)&0xFFFFF;	
			LR[0][3] = PCM[(sdram_adr>>13)+offset_adress][sdram_adr&0x1FFF][0];							
			LR[1][3] = PCM[(sdram_adr>>13)+offset_adress][sdram_adr&0x1FFF][1];		
			}
		else if(reverse==1 && play_adr>=step_position)
			{
			play_adr-= step_position;
			if(step_position==1)
				{
				LR[0][0] = LR[0][1];
				LR[1][0] = LR[1][1];
				LR[0][1] = LR[0][2];
				LR[1][1] = LR[1][2];
				LR[0][2] = LR[0][3];
				LR[1][2] = LR[1][3];
				sdram_adr = (play_adr)&0xFFFFF;	
				LR[0][3] = PCM[(sdram_adr>>13)+offset_adress][sdram_adr&0x1FFF][0];							
				LR[1][3] = PCM[(sdram_adr>>13)+offset_adress][sdram_adr&0x1FFF][1];		
				}
			else
				{
				sdram_adr = play_adr&0xFFFFF;						
				LR[0][3] = PCM[(sdram_adr>>13)+offset_adress][sdram_adr&0x1FFF][0];							
				LR[1][3] = PCM[(sdram_adr>>13)+offset_adress][sdram_adr&0x1FFF][1];
				sdram_adr = (play_adr+1)&0xFFFFF;
				LR[0][2] = PCM[(sdram_adr>>13)+offset_adress][sdram_adr&0x1FFF][0];								
				LR[1][2] = PCM[(sdram_adr>>13)+offset_adress][sdram_adr&0x1FFF][1];		
				sdram_adr = (play_adr+2)&0xFFFFF;
				LR[0][1] = PCM[(sdram_adr>>13)+offset_adress][sdram_adr&0x1FFF][0];									
				LR[1][1] = PCM[(sdram_adr>>13)+offset_adress][sdram_adr&0x1FFF][1];
				sdram_adr = (play_adr+3)&0xFFFFF;	
				LR[0][0] = PCM[(sdram_adr>>13)+offset_adress][sdram_adr&0x1FFF][0];							
				LR[1][0] = PCM[(sdram_adr>>13)+offset_adress][sdram_adr&0x1FFF][1];			
				}	
			}
		position = position%10000;	
		}	

	T = position;
	T = T/10000;
	T = T - 1/2.0F;
	
	even1 = LR[0][2];
	even1 = even1 + LR[0][1];
	odd1 = LR[0][2];
	odd1 = odd1 - LR[0][1];
	even2 = LR[0][3];
	even2 = even2 + LR[0][0]; 
	odd2 = LR[0][3];
	odd2 = odd2 - LR[0][0];
	c0 = (float)even1*COEF[0];
	r0 = (float)even2*COEF[1];
	c0 = c0 + r0;
	c1 = (float)odd1*COEF[2];
	r1 = (float)odd2*COEF[3];
	c1 = c1 + r1;
	c2 = (float)even1*COEF[4]; 
	r2 = (float)even2*COEF[5];
	c2 = c2 + r2;
	c3 = (float)odd1*COEF[6];
	r3 = (float)odd2*COEF[7];
	c3 = c3 + r3;

	SAMPLE_BUFFER = c0+T*(c1+T*(c2+T*c3));
	SAMPLE_BUFFER = SAMPLE_BUFFER*0.90F;
	PCM_2[0] = (int)SAMPLE_BUFFER;

	even1 = LR[1][2];
	even1 = even1 + LR[1][1];
	odd1 = LR[1][2];
	odd1 = odd1 - LR[1][1];
	even2 = LR[1][3];
	even2 = even2 + LR[1][0]; 
	odd2 = LR[1][3];
	odd2 = odd2 - LR[1][0];
	c0 = (float)even1*COEF[0];
	r0 = (float)even2*COEF[1];
	c0 = c0 + r0;
	c1 = (float)odd1*COEF[2];
	r1 = (float)odd2*COEF[3];
	c1 = c1 + r1;
	c2 = (float)even1*COEF[4]; 
	r2 = (float)even2*COEF[5];
	c2 = c2 + r2;
	c3 = (float)odd1*COEF[6];
	r3 = (float)odd2*COEF[7];
	c3 = c3 + r3;

	SAMPLE_BUFFER = c0+T*(c1+T*(c2+T*c3));
	SAMPLE_BUFFER = SAMPLE_BUFFER*0.90F;
	PCM_2[1] = (int)SAMPLE_BUFFER;
	
	SAMPLE[3] = PCM_2[0]/256;
	SAMPLE[2] = PCM_2[0]%256;
	SAMPLE[1] = PCM_2[1]/256;
	SAMPLE[0] = PCM_2[1]%256;
	//HAL_GPIO_WritePin(GPIOB, LED_TAG_LIST_Pin, GPIO_PIN_RESET);
	}

/* USER CODE END 0 */

int main(void)
{
	//__HAL_RCC_USB_OTG_HS_ULPI_CLK_DISABLE();
	//__HAL_RCC_USB_OTG_HS_ULPI_CLK_SLEEP_DISABLE();

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* Enable I-Cache-------------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache-------------------------------------------------------------*/
  SCB_EnableDCache();

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */


	/* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA2D_Init();
  MX_FMC_Init();
  MX_LTDC_Init();
  MX_SDMMC1_SD_Init();
  MX_FATFS_Init();
  MX_I2C3_Init();
  MX_SAI2_Init();
  MX_TIM1_Init();
  MX_QUADSPI_Init();

  /* USER CODE BEGIN 2 */
	BSP_QSPI_Init();
  BSP_QSPI_MemoryMappedMode();
	WRITE_REG(QUADSPI->LPTR, 0xFFF);
	
	CheckTXCRC();
	
	Touch_Ini();
	
	BSP_SD_Init();
		
	lcd_status = BSP_LCD_Init();
	BSP_LCD_LayerDefaultInit(0, LCD_FRAME_BUFFER);
	BSP_LCD_LayerDefaultInit(1, LCD_FRAME_BUFFER_2);
	
	BSP_LCD_SelectLayer(1);
	BSP_LCD_Clear(0x00000000);				//верхний слой залит прозрачным цветом
	BSP_LCD_SetTransparency(1, 0);		//слой выставлен полностью прозрачным
	
	BSP_LCD_SelectLayer(0);
	BSP_LCD_SetTransparency(0, 255);
	BSP_LCD_Clear(LCD_COLOR_BLACK);
	BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
	
	MX_DMA_Init();
  MX_SPI1_Init();
	HAL_SPI_TransmitReceive_DMA(&hspi1, Tbuffer, Rbuffer, 27);
	
	HAL_Delay(500);
	BSP_LCD_DisplayOn();
	DrawLOGO();

	UTILITY_BASE[0][15] = 0;
	UTILITY_BASE[1][15] = 0;
	UTILITY_BASE[2][15] = 0;
	UTILITY_BASE[3][15] = 0;
	UTILITY_BASE[4][15] = 0;
	UTILITY_BASE[5][15] = 0;
	UTILITY_BASE[6][15] = 0;
	UTILITY_BASE[7][15] = 0;
	UTILITY_BASE[8][15] = 0;
	UTILITY_BASE[9][15] = 0;
	UTILITY_BASE[10][15] = 0;

	DEVICE_UID = *(unsigned long*)(0x1FF0F420);							//Read device ID
	DEVICE_UID = DEVICE_UID%1000000000;
	
	BSP_LCD_SetFont(&Font8);

	res = f_mount(&FAT, "0", 1);
		if (res == FR_OK)
		{
		BSP_LCD_SetTextColor(LCD_COLOR_GREEN);	
		sprintf((char*)Buf, "%s", "> Disc mounted OK");
		BSP_LCD_DisplayStringAt(10,1,Buf, LEFT_MODE);	
		}
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_RED);	
		sprintf((char*)Buf, "%s", "> Disc not mounted!");
		BSP_LCD_DisplayStringAt(10,1,Buf, LEFT_MODE);
		while(1)
			{}		
		}

	TOTAL_TRACKS = DATABASE_PARSER();
	
	if(TOTAL_TRACKS==0)
		{
		BSP_LCD_SetTextColor(LCD_COLOR_RED);	
		sprintf((char*)Buf, "%s", "Rekordbox database not found!");
		BSP_LCD_DisplayStringAt(10,37,Buf, LEFT_MODE);
		while(1)
			{}
		}	
	
	HAL_TIM_Base_Start_IT(&htim1);

	DAC_Init_and_Start(output_level);											/////Start AUDIO PROCESSING
	
	HAL_GPIO_WritePin(GPIOB, LED_TAG_LIST_Pin|LED_MENU_Pin, GPIO_PIN_RESET);				//power off unnecessary leds
	HAL_GPIO_WritePin(GPIOI, LED_INFO_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOF, LED_MIDI_Pin|LED_USB_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, LED_BROWSE_Pin, GPIO_PIN_RESET);	
	//////////////////////////////////////////////////////////////////////////////////
	
	BSP_LCD_Clear(LCD_COLOR_BLACK);	
	DrawStaticInformation();	
	DrawTempoRange(tempo_range);			
	ShowTempo(10000);
	ShowBPM(originalBPM);
	ShowTrackNumber(track_number);
	ShowREMAIN(REMAIN_ENABLE);	
	ShowACUE(1);
	SwitchInformationLayer(WAVEFORM);		//Show browser
	RedrawWaveforms(0);
			
	DrawCueMarker(0);	
	DrawStaticWFM(CLEAR_WAVEFORM_ARRAY);
	DrawStaticWFM(MS_NOT_LOADED);	
	
	DrawREKORDBOX();

	
	uint8_t JJ;			//variable for internal calc
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {	
	RedrawWaveforms(play_adr/294);	
		
	if(end_adr_valid_data<128)
		{
		f_read(&file, PCM[end_adr_valid_data][0], 32768, &nbytes);
		//DrawCueMarker(1+((end_adr_valid_data*11145)/all_long));	
		end_adr_valid_data++;	
		}
	else if((end_adr_valid_data<((play_adr>>13)+42)) && (filling_step==0 || filling_step==6))									//filling the buffer forward
		{
		if(filling_step==6)
			{
			f_lseek(&file, ((32768*end_adr_valid_data)+44));	
			filling_step = 0;	
			}
		f_read(&file, PCM[end_adr_valid_data&0x7F][0], 32768, &nbytes);
		//DrawCueMarker(1+((end_adr_valid_data*11145)/all_long));
		end_adr_valid_data++;
		if((end_adr_valid_data-start_adr_valid_data)>128)
			{
			start_adr_valid_data = end_adr_valid_data-128;	
			}
		}
	else if(((end_adr_valid_data>((play_adr>>13)+86) || ((end_adr_valid_data-start_adr_valid_data)<124)) && start_adr_valid_data>3) || (filling_step!=0 && filling_step!=6))					//filling the buffer back
		{
		if(filling_step==0 || filling_step==6)
			{
			if((end_adr_valid_data-start_adr_valid_data)>127)	
				{
				end_adr_valid_data = start_adr_valid_data+124;	
				}	
			start_adr_valid_data-= 4;	
			f_lseek(&file, ((32768*(start_adr_valid_data))+44));
			filling_step = 1;	
			}
		else if(filling_step==1)
			{
			f_read(&file, PCM[start_adr_valid_data&0x7F][0], 32768, &nbytes);
			filling_step = 2;	
			}
		else if(filling_step==2)
			{
			f_read(&file, PCM[(start_adr_valid_data+1)&0x7F][0], 32768, &nbytes);
			filling_step = 3;	
			}
		else if(filling_step==3)
			{
			f_read(&file, PCM[(start_adr_valid_data+2)&0x7F][0], 32768, &nbytes);
			filling_step = 4;	
			}
		else if(filling_step==4)
			{
			f_read(&file, PCM[(start_adr_valid_data+3)&0x7F][0], 32768, &nbytes);
			filling_step = 5;	
			}
		else if(filling_step==5)
			{
			//DrawCueMarker(1+((start_adr_valid_data*11145)/all_long));	
			filling_step = 6;		
			}
		}			
		
		
	TOUCH_SCREEN_HANDLER();	
				
	if(track_need_load!=0 && TOTAL_TRACKS!=0)
		{
		if(track_need_load==1)				//load next track
			{
			if(track_number==TOTAL_TRACKS)
				{
				track_number = 1;	
				}
			else
				{
				track_number++;	
				}
			PREPARE_LOAD_TRACK(track_number);		
			}
		else if(track_need_load==2) 	//load previous track
			{
			if(track_number==1 || track_number==0)
				{
				track_number = TOTAL_TRACKS;	
				}
			else
				{
				track_number--;	
				}	
			PREPARE_LOAD_TRACK(track_number);		
			}
		track_need_load = 0;	
		}
		
	if(tempo_range_need_update)
		{	
		DrawTempoRange(tempo_range);
		tempo_need_update = 1;	
		tempo_range_need_update = 0;	
		}
		
	if(tempo_need_update>0)
		{
		if(tempo_need_update==1)
			{
			ShowTempo(potenciometer_tempo);	
			}	
		if(originalBPM!=0xFFFF)
			{
			ShowBPM(((originalBPM+5)*potenciometer_tempo)/100000);	
			}
		tempo_need_update = 0;
		}

		
	if(time_mode_need_update)			
		{
		forcibly_redraw = 1;	
		ShowREMAIN(REMAIN_ENABLE);
		if(REMAIN_ENABLE)
			{
			DrawStaticWFM(REDRAW_IN_REMAIN_MODE);
			}
		else
			{
			DrawStaticWFM(REDRAW_IN_NREMAIN_MODE);	
			}
		time_mode_need_update = 0;		
		}
	else if(need_DSW>0)
		{
		if(need_DSW==1)
			{
			DrawStaticWFM(REDRAW_IN_NREMAIN_MODE);	
			}
		else
			{
			DrawStaticWFM(REDRAW_IN_REMAIN_MODE);	
			}
		need_DSW = 0;	
		}
		
	if(CUE_OPERATION==CUE_NEED_SET)
		{
		SET_CUE(play_adr/294);
		CUE_OPERATION = 0;	
		}
	else if(CUE_OPERATION==CUE_NEED_CALL)
		{
		CALL_CUE();
		CUE_OPERATION = 0;			
		}
	else if(CUE_OPERATION==MEMORY_NEED_NEXT_SET)
		{
		if(number_of_memory_cue_points>0)
			{
			JJ=0;
			while(MEMORY_adr[0][JJ]<=(play_adr/294) && (JJ<number_of_memory_cue_points-1))
				{
				JJ++;	
				}
			if((play_adr/294)<MEMORY_adr[0][JJ])
				{
				SET_MEMORY_CUE_1(MEMORY_adr[0][JJ]);
				CUE_OPERATION = MEMORY_NEED_SET_PART2;	
				}
			else
				{
				CUE_OPERATION = 0;	
				}
			}
		else
			{
			CUE_OPERATION = 0;	
			}
		}	
	else if(CUE_OPERATION==MEMORY_NEED_PREVIOUS_SET)
		{
		if(number_of_memory_cue_points>0)
			{
			JJ = number_of_memory_cue_points-1;
			while(MEMORY_adr[0][JJ]>=(play_adr/294) && (JJ>0))
				{
				JJ--;	
				}
			if((play_adr/294)>MEMORY_adr[0][JJ])
				{
				SET_MEMORY_CUE_1(MEMORY_adr[0][JJ]);
				CUE_OPERATION = MEMORY_NEED_SET_PART2;	
				}
			else
				{
				CUE_OPERATION = 0;	
				}
			}
		else
			{
			CUE_OPERATION = 0;	
			}			
		}	
	else if(CUE_OPERATION==MEMORY_NEED_SET_PART2 && ((end_adr_valid_data-start_adr_valid_data)>64))	
		{
		SET_MEMORY_CUE_2();
		offset_adress = 0;		
		CUE_OPERATION = 0;		
		}
			
		
	if(HAL_GPIO_ReadPin(GPIOG, KEY_BROWSE_Pin)==0 & KEY_BROWSE_pressed==0)					//BROWSER-WAVEFORM BUTTON
		{
		if(dSHOW != BROWSER && dSHOW != BROWSER_INFO)
			{
			if(BROWSER_INFO_enable)
				{
				SwitchInformationLayer(BROWSER_INFO);
				}
			else
				{
				SwitchInformationLayer(BROWSER);	
				}
			}
		else
			{
			SwitchInformationLayer(WAVEFORM);	
			}
		KEY_BROWSE_pressed = 1;	
		}
	else if(HAL_GPIO_ReadPin(GPIOG, KEY_BROWSE_Pin)==1 & KEY_BROWSE_pressed==1)
		{
		KEY_BROWSE_pressed = 0;		
		}
	
		
	if(HAL_GPIO_ReadPin(KEY_INFO_GPIO_Port, KEY_INFO_Pin)==0 & KEY_INFO_pressed==0)					//INFO BUTTON
		{
		if(dSHOW==TAG_LIST)
			{
			TAG_LIST_INFO_enable = 1;	
			SwitchInformationLayer(TAG_LIST_INFO);
			}	
		else if(dSHOW==TAG_LIST_INFO)
			{
			TAG_LIST_INFO_enable = 0;	
			SwitchInformationLayer(TAG_LIST);
			}
		else if(dSHOW==BROWSER)
			{
			BROWSER_INFO_enable = 1;	
			SwitchInformationLayer(BROWSER_INFO);
			}		
		else if(dSHOW==BROWSER_INFO)
			{
			BROWSER_INFO_enable = 0;		
			SwitchInformationLayer(BROWSER);
			}		
		KEY_INFO_pressed = 1;	
		}
	else if(HAL_GPIO_ReadPin(KEY_INFO_GPIO_Port, KEY_INFO_Pin)==1 & KEY_INFO_pressed==1)
		{
		KEY_INFO_pressed = 0;		
		}	
		
		
	if(HAL_GPIO_ReadPin(GPIOF, KEY_SD_Pin)==0 & KEY_SD_pressed==0)					//SD BUTTON
		{
		if(dSHOW==WAVEFORM || dSHOW==TAG_LIST)
			{
			if(BROWSER_INFO_enable)
				{
				SwitchInformationLayer(BROWSER_INFO);
				}
			else
				{
				SwitchInformationLayer(BROWSER);	
				}
			}	
		KEY_SD_pressed = 1;	
		}
	else if(HAL_GPIO_ReadPin(GPIOF, KEY_SD_Pin)==1 & KEY_SD_pressed==1)	
		{
		KEY_SD_pressed = 0;	
		}
	else if(HAL_GPIO_ReadPin(GPIOF, KEY_MIDI_Pin)==0 & KEY_MIDI_pressed==0)					//MIDI BUTTON
		{	
		/////////////////////////////
		CUE_OPERATION = MEMORY_NEED_NEXT_SET;	
			
		HAL_GPIO_WritePin(GPIOF, LED_MIDI_Pin, GPIO_PIN_SET);			
		KEY_MIDI_pressed = 1;		
		}
	else if(HAL_GPIO_ReadPin(GPIOF, KEY_MIDI_Pin)==1 & KEY_MIDI_pressed==1)	
		{
		HAL_GPIO_WritePin(GPIOF, LED_MIDI_Pin, GPIO_PIN_RESET);				
		KEY_MIDI_pressed = 0;	
		}			
	else if(HAL_GPIO_ReadPin(GPIOF, KEY_USB_Pin)==0 & KEY_USB_pressed==0)					//USB BUTTON
		{
		//CALL_CUE();	
		////////////////////////
			
		HAL_GPIO_WritePin(GPIOF, LED_USB_Pin, GPIO_PIN_SET);			
		KEY_USB_pressed = 1;		
		}
	else if(HAL_GPIO_ReadPin(GPIOF, KEY_USB_Pin)==1 & KEY_USB_pressed==1)	
		{
		//offset_adress = 0;																///	   temporary operation
		HAL_GPIO_WritePin(GPIOF, LED_USB_Pin, GPIO_PIN_RESET);				
		KEY_USB_pressed = 0;	
		}	
		
		
		
		
		
		
	if(HAL_GPIO_ReadPin(GPIOG, KEY_TAG_TRACK_Pin)==0 & KEY_TAG_TRACK_pressed==0)					//TAG TRACK/REMOVE BUTTON
		{
		if(dSHOW==BROWSER || dSHOW==BROWSER_INFO)
			{
			if((playlist[BCurrentCursorPosition + BCurrentTrackPosition-1][54]&0x2)==0)					//////////////when track is not in tag list
				{
				if(TOTAL_TRACKS_IN_TAG_LIST<100)
					{
					playlist[BCurrentCursorPosition + BCurrentTrackPosition-1][54]|=0x02;		//////////////////write add taglist mark
					TAG_LIST_BASE[TOTAL_TRACKS_IN_TAG_LIST] = BCurrentCursorPosition + BCurrentTrackPosition;
					TOTAL_TRACKS_IN_TAG_LIST++;
					}
				}
			else																													///////////////////////Delete Track from TAG LIST
				{
				playlist[BCurrentCursorPosition + BCurrentTrackPosition-1][54]&=0xFD;		//write delete taglist mark	
					s = 0;
				while(TAG_LIST_BASE[s]!=(BCurrentCursorPosition + BCurrentTrackPosition) && s<TOTAL_TRACKS_IN_TAG_LIST)
					{
					s++;		
					}
				TOTAL_TRACKS_IN_TAG_LIST--;	
				while(s<TOTAL_TRACKS_IN_TAG_LIST)
					{
					TAG_LIST_BASE[s] = TAG_LIST_BASE[s+1];	
					s++;	
					}
				}
			SwitchInformationLayer(dSHOW);
			}
		else if((dSHOW==TAG_LIST || dSHOW==TAG_LIST_INFO) & TOTAL_TRACKS_IN_TAG_LIST>0)
			{
			playlist[TAG_LIST_BASE[TCurrentCursorPosition + TCurrentTrackPosition-1]-1][54]&=0xFD;		//delete taglist mark	
			s = TCurrentCursorPosition + TCurrentTrackPosition-1;
			TOTAL_TRACKS_IN_TAG_LIST--;	
			while(s<TOTAL_TRACKS_IN_TAG_LIST)
				{
				TAG_LIST_BASE[s] = TAG_LIST_BASE[s+1];	
				s++;	
				}
			SwitchInformationLayer(dSHOW);			
			}
		KEY_TAG_TRACK_pressed = 1;	
		}
	else if(HAL_GPIO_ReadPin(GPIOG, KEY_TAG_TRACK_Pin)==1 & KEY_TAG_TRACK_pressed==1)	
		{
		KEY_TAG_TRACK_pressed = 0;	
		}
			
		
	if(HAL_GPIO_ReadPin(GPIOC, KEY_TAG_LIST_Pin)==0 & KEY_TAG_LIST_pressed==0)					//TAG LIST BUTTON
		{
		if(dSHOW != TAG_LIST && dSHOW != TAG_LIST_INFO)
			{
			if(TAG_LIST_INFO_enable)
				{
				SwitchInformationLayer(TAG_LIST_INFO);
				}
			else
				{
				SwitchInformationLayer(TAG_LIST);	
				}	
			}
		else if(dSHOW==TAG_LIST || dSHOW==TAG_LIST_INFO)
			{
			SwitchInformationLayer(WAVEFORM);
			}
		KEY_TAG_LIST_pressed = 1;	
		}
	else if(HAL_GPIO_ReadPin(GPIOC, KEY_TAG_LIST_Pin)==1 & KEY_TAG_LIST_pressed==1)	
		{
		KEY_TAG_LIST_pressed = 0;	
		}		
	
		
	if(HAL_GPIO_ReadPin(KEY_MENU_GPIO_Port, KEY_MENU_Pin)==0 & KEY_MENU_pressed==0)					//MENU BUTTON
		{
		if(dSHOW != UTILITY)
			{
			if(countUTILITY==10)
				{
				SwitchInformationLayer(UTILITY);
				KEY_MENU_pressed = 1;	
				}	
			}	
			else
			{	
			SwitchInformationLayer(WAVEFORM);
			KEY_MENU_pressed = 1;		
			}	
		}
	else if(HAL_GPIO_ReadPin(KEY_MENU_GPIO_Port, KEY_MENU_Pin)==1 & KEY_MENU_pressed==1)	
		{	
		KEY_MENU_pressed = 0;	
		}	
		
		
		
		
	if(need_up)	
		{
		if(dSHOW==BROWSER || dSHOW==BROWSER_INFO)
			{
			NAVIGATOR(BROWSER_UP);	
			}
		else if(dSHOW==WAVEFORM)
			{
			if(DynamicWaveformZOOM>1)
				{
				DynamicWaveformZOOM = DynamicWaveformZOOM/2;
				if(track_number!=0)
					{
					forcibly_redraw = 1;
					}			
				}				
			}
		else if(dSHOW==TAG_LIST || dSHOW==TAG_LIST_INFO)
			{
			NAVIGATOR(TAGLIST_UP);	
			}
		else if(dSHOW==UTILITY)
			{
			NAVIGATOR(UTILITY_UP);	
			}
		need_up = 0;	
		}
	else if(need_down)
		{	
		if(dSHOW==BROWSER || dSHOW==BROWSER_INFO)
			{
			NAVIGATOR(BROWSER_DOWN);	
			}
		else if(dSHOW==WAVEFORM)
			{				
			if(DynamicWaveformZOOM<16)
				{
				DynamicWaveformZOOM = DynamicWaveformZOOM*2;
				if(track_number!=0)
					{	
					forcibly_redraw = 1;
					}
				}
			}
		else if(dSHOW==TAG_LIST || dSHOW==TAG_LIST_INFO)
			{
			NAVIGATOR(TAGLIST_DOWN);	
			}
		else if(dSHOW==UTILITY)
			{
			NAVIGATOR(UTILITY_DOWN);	
			}
		need_down = 0;	
		}
	
	if(HAL_GPIO_ReadPin(GPIOI, ENC_BUTTON_Pin)==0 && 
		(dSHOW==BROWSER || dSHOW==BROWSER_INFO || ((dSHOW==TAG_LIST || dSHOW==TAG_LIST_INFO) && TOTAL_TRACKS_IN_TAG_LIST>0)) && LOAD_pressed==0) //LOAD TRACK
		{
		ENCODER_LED_BLINK = 0;
		load_animation_enable = 1;	
		if(dSHOW==BROWSER || dSHOW==BROWSER_INFO)
			{
			track_number = BCurrentCursorPosition + BCurrentTrackPosition;
			}
		else if(dSHOW==TAG_LIST || dSHOW==TAG_LIST_INFO)
			{
			track_number = TAG_LIST_BASE[TCurrentCursorPosition + TCurrentTrackPosition-1];
			}
		PREPARE_LOAD_TRACK(track_number);	
		LOAD_pressed = 1;	
		}	
	else if(HAL_GPIO_ReadPin(GPIOI, ENC_BUTTON_Pin)==1 && (dSHOW==BROWSER || dSHOW==BROWSER_INFO || ((dSHOW==TAG_LIST || dSHOW==TAG_LIST_INFO) && TOTAL_TRACKS_IN_TAG_LIST>0)) && LOAD_pressed==1)
		{
		LOAD_pressed = 0;		
		}
		
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 432;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 15;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Activate the Over-Drive mode 
    */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC|RCC_PERIPHCLK_SAI2
                              |RCC_PERIPHCLK_I2C3|RCC_PERIPHCLK_SDMMC1
                              |RCC_PERIPHCLK_CLK48;
  PeriphClkInitStruct.PLLI2S.PLLI2SN = 135;
  PeriphClkInitStruct.PLLI2S.PLLI2SP = RCC_PLLP_DIV2;
  PeriphClkInitStruct.PLLI2S.PLLI2SR = 2;
  PeriphClkInitStruct.PLLI2S.PLLI2SQ = 3;
  PeriphClkInitStruct.PLLSAI.PLLSAIN = 120;
  PeriphClkInitStruct.PLLSAI.PLLSAIR = 2;
  PeriphClkInitStruct.PLLSAI.PLLSAIQ = 2;
  PeriphClkInitStruct.PLLSAI.PLLSAIP = RCC_PLLSAIP_DIV2;
  PeriphClkInitStruct.PLLI2SDivQ = 4;
  PeriphClkInitStruct.PLLSAIDivQ = 1;
  PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_2;
  PeriphClkInitStruct.Sai2ClockSelection = RCC_SAI2CLKSOURCE_PLLI2S;
  PeriphClkInitStruct.I2c3ClockSelection = RCC_I2C3CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLL;
  PeriphClkInitStruct.Sdmmc1ClockSelection = RCC_SDMMC1CLKSOURCE_CLK48;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

void DrawStaticInformation(void)
	{
	BSP_LCD_SetFont(&FontBMP);
	BSP_LCD_SetTextColor(LCD_COLOR_GRAY);		
	sprintf((char*)Buf, "%s", "67");						//PLAYER
	BSP_LCD_DisplayStringAt(4,186,Buf, TRANSPARENT_MODE);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	sprintf((char*)Buf, "%s", "/0");						//TEMPO
	BSP_LCD_DisplayStringAt(325,185,Buf, TRANSPARENT_MODE);		
	sprintf((char*)Buf, "%s", "23");						//TRACK
	BSP_LCD_DisplayStringAt(47,183,Buf, TRANSPARENT_MODE);
	
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_SetFont(&Font23D);
	sprintf((char*)Buf, "%s", "m");			//M:
	BSP_LCD_DisplayStringAt(176,194,Buf, TRANSPARENT_MODE);	
	sprintf((char*)Buf, "%s", "s");			//S
	BSP_LCD_DisplayStringAt(230,194,Buf, TRANSPARENT_MODE);
	sprintf((char*)Buf, "%s", ".");			//dot
	BSP_LCD_DisplayStringAt(284,194,Buf, TRANSPARENT_MODE);
	sprintf((char*)Buf, "%s", "f");			//F
	BSP_LCD_DisplayStringAt(309,194,Buf, TRANSPARENT_MODE);
		
	BSP_LCD_SetFont(&Font18D);	
	sprintf((char*)Buf, "%s", "%");			//PITCH procent symbol
	BSP_LCD_DisplayStringAt(400,199,Buf, TRANSPARENT_MODE);

	BSP_LCD_SetTextColor(LCD_COLOR_GRAY);
	sprintf((char*)Buf, "%s", "#");			//PLAYER NUBMER 1
	BSP_LCD_DisplayStringAt(15,201,Buf, TRANSPARENT_MODE);
	
	BSP_LCD_DrawLine(3, 221, 36, 221);					//PLAYER shield
	BSP_LCD_DrawLine(1, 184, 1, 219);
	BSP_LCD_DrawPixel(2, 183, 0xFF808080);
	BSP_LCD_DrawPixel(2, 220, 0xFF808080);
	BSP_LCD_DrawLine(3, 182, 36, 182);
	BSP_LCD_DrawLine(38, 184, 38, 219);
	BSP_LCD_DrawPixel(37, 183, 0xFF808080);
	BSP_LCD_DrawPixel(37, 220, 0xFF808080);	
	}

///////////////////////////	
//show A.CUE on display
//	
void ShowACUE(uint8_t acue)
	{	
	BSP_LCD_SetFont(&FontBMP);	
	if(acue==1)
		{
		BSP_LCD_SetTextColor(LCD_COLOR_RED);	
		}
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		}
	sprintf((char*)Buf, "%s", "%&");						//A.CUE
	BSP_LCD_DisplayStringAt(93,206,Buf, TRANSPARENT_MODE);
	}
	
///////////////////////////	
//show REMAIN on display
//
void ShowREMAIN(uint8_t remain)
	{
	BSP_LCD_SetFont(&FontBMP);
	if(remain==1)
		{
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);	
		}
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		}
	sprintf((char*)Buf, "%s", "-.");						//REMAIN	
	BSP_LCD_DisplayStringAt(93,196,Buf, TRANSPARENT_MODE);
	}
	
	

//show bpm and bpm shield
//
// input bpm*10
//	
void ShowBPM(uint16_t bpm)
	{		
	if(bpm != 0xFFFF)
		{
		bpm = bpm%10000; 	
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		BSP_LCD_SetFont(&Font20D);
		sprintf((char*)Buf, "%3lu", bpm/10);			//BPM count
		BSP_LCD_DisplayStringAt(418,187,Buf, LEFT_MODE);
		BSP_LCD_DrawPixel(466, 205, 0xFFFFFFFF);	//
		BSP_LCD_DrawPixel(466, 206, 0xFFFFFFFF);	//	DOT
		BSP_LCD_DrawPixel(465, 205, 0xFFFFFFFF);	//
		BSP_LCD_DrawPixel(465, 206, 0xFFFFFFFF);	//
			
		BSP_LCD_SetFont(&Font11D);
		sprintf((char*)Buf, "%01lu", bpm%10);			//BPM count
		BSP_LCD_DisplayStringAt(469,196,Buf, LEFT_MODE);
		}
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		BSP_LCD_FillRect(418, 187, 58, 20);	
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);		
		}

	BSP_LCD_SetFont(&FontBMP);
	sprintf((char*)Buf, "%s", "1");						//BPM
	BSP_LCD_DisplayStringAt(454,212,Buf, TRANSPARENT_MODE);
		
	BSP_LCD_DrawLine(416, 219, 476, 219);					//BPM shield
	BSP_LCD_DrawLine(478, 217, 478, 184);
	BSP_LCD_DrawPixel(477, 218, 0xFFFFFFFF);
	BSP_LCD_DrawPixel(477, 183, 0xFFFFFFFF);
	BSP_LCD_DrawLine(416, 182, 476, 182);
	BSP_LCD_DrawLine(414, 217, 414, 184);
	BSP_LCD_DrawPixel(415, 218, 0xFFFFFFFF);
	BSP_LCD_DrawPixel(415, 183, 0xFFFFFFFF);		
	}

//////////////////////////////////////////////////	
//Draw time position bar 0...399
//
//	DRAW_NEW_STATIC_WAVEFORM		400
//	CLEAR_WAVEFORM_ARRAY				401
//	MS_NOT_LOADED								402
//	REDRAW_IN_NREMAIN_MODE	
//	REDRAW_IN_REMAIN_MODE		
//	MS_ERROR										410
//
void DrawStaticWFM(uint16_t Tpos)
	{
	uint16_t i;
	if(Tpos<400)
		{	
		if(previous_position_bar != Tpos)			
			{
			ForceDrawVLine(previous_position_bar+40, 230, 36, LCD_COLOR_BLACK);
			ForceDrawVLine(previous_position_bar+41, 230, 36, LCD_COLOR_BLACK);					
			ForceDrawVLine(previous_position_bar+40, 256-(WFORMSTATIC[previous_position_bar]&0x1F), (WFORMSTATIC[previous_position_bar]&0x1F)+1, WS_COLOR_MAP[WFORMSTATIC[previous_position_bar]>>7]);	
			ForceDrawVLine(previous_position_bar+41, 256-(WFORMSTATIC[previous_position_bar+1]&0x1F), (WFORMSTATIC[previous_position_bar+1]&0x1F)+1, WS_COLOR_MAP[WFORMSTATIC[previous_position_bar+1]>>7]);

			if(previous_position_bar>Tpos)						//___<<||___     moving
				{				
				i = previous_position_bar - Tpos;
				if(REMAIN_ENABLE)
					{	
					while(i>0)
						{
						if((Tpos+i)%2==0 && (Tpos+i<398))
							{
							ForceDrawVLine(Tpos+i+42, 259, 4, LCD_COLOR_WHITE);	
							}
						else
							{
							ForceDrawVLine(Tpos+i+42, 259, 4, PBAR_COLOR_1);	
							}
							i--;	
						}
					}
				else
					{
					BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
					BSP_LCD_FillRect(Tpos+41, 260, (previous_position_bar-Tpos+2), 2);			
					while(i>0)
						{
						if(((Tpos+i)%2==0) && (Tpos+i<398))
							{
							BSP_LCD_DrawPixel(Tpos+i+42, 259, PBAR_COLOR_2);	
							BSP_LCD_DrawPixel(Tpos+i+42, 262, PBAR_COLOR_2);		
							}
						else if(((Tpos+i)%2==1) && (Tpos+i<398))
							{
							BSP_LCD_DrawPixel(Tpos+i+42, 259, PBAR_COLOR_3);	
							BSP_LCD_DrawPixel(Tpos+i+42, 262, PBAR_COLOR_3);		
							}
						i--;	
						}
					ForceDrawVLine(439, 259, 4, PBAR_COLOR_2);						//gray scroll vertical line in end					
					}
				}
			else if(previous_position_bar<Tpos)																			//___||>>___     moving
				{
				i = Tpos - previous_position_bar+1;	
				if(REMAIN_ENABLE)																		
					{
					BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
					BSP_LCD_FillRect(previous_position_bar+41, 260, (Tpos - previous_position_bar+2), 2);						
					ForceDrawVLine(40, 259, 4, PBAR_COLOR_2);						//gray scroll vertical line in start	
					while(i>0)
						{
						if((previous_position_bar+i)>1)	
							{
							if((previous_position_bar+i)%2==0)
								{
								BSP_LCD_DrawPixel(38+previous_position_bar+i, 259, PBAR_COLOR_2);	
								BSP_LCD_DrawPixel(38+previous_position_bar+i, 262, PBAR_COLOR_2);	
								}
							else
								{
								BSP_LCD_DrawPixel(38+previous_position_bar+i, 259, PBAR_COLOR_3);	
								BSP_LCD_DrawPixel(38+previous_position_bar+i, 262, PBAR_COLOR_3);		
								}
							}
						i--;					
						}
					}
				else				
					{
					while(i>0)
						{
						if((previous_position_bar+i)>1)	
							{
							if((previous_position_bar+i)%2==0)
								{
								ForceDrawVLine(38+previous_position_bar+i, 259, 4, LCD_COLOR_WHITE);		
								}
							else
								{
								ForceDrawVLine(38+previous_position_bar+i, 259, 4, PBAR_COLOR_1);		
								}
							}
						i--;					
						}
					}
				}			
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
			BSP_LCD_FillRect(Tpos+39, 259, 4, 4);	
			
			if(RED_VERTICAL_LINE)						//detecting touch on sensor or touch on jog 
				{
				ForceDrawVLine(Tpos+40, 230, 36, LCD_COLOR_RED);		//red vertical scroll
				ForceDrawVLine(Tpos+41, 230, 36, LCD_COLOR_RED);		
				}
			else
				{
				ForceDrawVLine(Tpos+40, 230, 36, LCD_COLOR_WHITE);		//white vertical scroll
				ForceDrawVLine(Tpos+41, 230, 36, LCD_COLOR_WHITE);		
				}
			previous_position_bar = Tpos;	
			}
		else if(forcibly_redraw==1)
			{
			if(RED_VERTICAL_LINE)						//detecting touch on sensor or touch on jog 
				{
				ForceDrawVLine(Tpos+40, 230, 36, LCD_COLOR_RED);		//red vertical scroll
				ForceDrawVLine(Tpos+41, 230, 36, LCD_COLOR_RED);			
				}
			else
				{
				ForceDrawVLine(Tpos+40, 230, 36, LCD_COLOR_WHITE);		//white vertical scroll
				ForceDrawVLine(Tpos+41, 230, 36, LCD_COLOR_WHITE);		
				}
			}
		return;	
		}
	else if(Tpos==REDRAW_IN_NREMAIN_MODE)
		{
		if(previous_position_bar<397)
			{
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
			BSP_LCD_FillRect(previous_position_bar+42, 260, 397-previous_position_bar, 2);
			}				
		for(i=0;i<400;i++)						
			{			
			if(i<previous_position_bar-1)
				{
				if(i%2==0)							//drawing ||||||||||||||||||
					{
					ForceDrawVLine(i+40, 259, 4, LCD_COLOR_WHITE);		
					}
				else
					{
					ForceDrawVLine(i+40, 259, 4, PBAR_COLOR_1);		
					}
				}	
			else if(i>previous_position_bar+2)
				{
				if(i%2==0)						//drawing :::::::::::::::
					{
					BSP_LCD_DrawPixel(40+i, 259, PBAR_COLOR_2);	
					BSP_LCD_DrawPixel(40+i, 262, PBAR_COLOR_2);	
					}
				else
					{
					BSP_LCD_DrawPixel(40+i, 259, PBAR_COLOR_3);	
					BSP_LCD_DrawPixel(40+i, 262, PBAR_COLOR_3);		
					}	
				}
			}
		if(previous_position_bar<397)
			{
			ForceDrawVLine(439, 259, 4, PBAR_COLOR_2);						//gray scroll vertical line in end	
			}	
		DRAWN_IN_REMAIN = 0;	
		return;	
		}
	else if(Tpos==REDRAW_IN_REMAIN_MODE)
		{
		if(previous_position_bar>1)
			{
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
			BSP_LCD_FillRect(41, 260, previous_position_bar-1, 2);
			}
		for(i=0;i<400;i++)						
			{	
			if(i<previous_position_bar-1)
				{
				if(i%2==0)						//drawing :::::::::::::::
					{
					BSP_LCD_DrawPixel(40+i, 259, PBAR_COLOR_2);	
					BSP_LCD_DrawPixel(40+i, 262, PBAR_COLOR_2);	
					}
				else
					{
					BSP_LCD_DrawPixel(40+i, 259, PBAR_COLOR_3);	
					BSP_LCD_DrawPixel(40+i, 262, PBAR_COLOR_3);		
					}	
				}	
			else if(i>previous_position_bar+2)
				{
				if(i%2==0)							//drawing ||||||||||||||||||
					{
					ForceDrawVLine(i+40, 259, 4, LCD_COLOR_WHITE);		
					}
				else
					{
					ForceDrawVLine(i+40, 259, 4, PBAR_COLOR_1);		
					}
				}	
			}
		if(previous_position_bar>1)
			{
			ForceDrawVLine(40, 259, 4, PBAR_COLOR_2);						//gray scroll vertical line in start				
			}		
		DRAWN_IN_REMAIN = 1;	
		return;	
		}	
	else if(Tpos==DRAW_NEW_STATIC_WAVEFORM)						//Draw new static waveform		
		{
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		BSP_LCD_FillRect(40, 230, 400, 40);				
		for(i=0;i<400;i++)						
			{			
			ForceDrawVLine(i+40, 256-(WFORMSTATIC[i]&0x1F), (WFORMSTATIC[i]&0x1F)+1, WS_COLOR_MAP[WFORMSTATIC[i]>>7]);		
			if(REMAIN_ENABLE)
				{
				if(i%2==0)
					{
					ForceDrawVLine(i+40, 259, 4, LCD_COLOR_WHITE);		
					}
				else
					{
					ForceDrawVLine(i+40, 259, 4, PBAR_COLOR_1);		
					}
				}
			else
				{
				if(i%2==0)
					{
					BSP_LCD_DrawPixel(40+i, 259, PBAR_COLOR_2);	
					BSP_LCD_DrawPixel(40+i, 262, PBAR_COLOR_2);	
					}
				else
					{
					BSP_LCD_DrawPixel(40+i, 259, PBAR_COLOR_3);	
					BSP_LCD_DrawPixel(40+i, 262, PBAR_COLOR_3);		
					}
				}
			}
		if(REMAIN_ENABLE==0)	
			{
			ForceDrawVLine(40, 259, 4, PBAR_COLOR_2);						//gray scroll vertical line in start	
			ForceDrawVLine(439, 259, 4, PBAR_COLOR_2);					//gray scroll vertical line in end	
			}
		}
	else if(Tpos==CLEAR_WAVEFORM_ARRAY)										//clear waveform array
		{
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		BSP_LCD_FillRect(40, 220, 400, 51);			
		}
	else if(Tpos==MS_NOT_LOADED)
		{
		BSP_LCD_SetFont(&Font15P);	
		BSP_LCD_SetTextColor(LCD_COLOR_RED);
		sprintf((char*)Buf, "%s", "NOT LOADED");	
		BSP_LCD_DisplayStringAt(193,239,Buf, LEFT_MODE);
		}
	else if(Tpos>MS_ERROR)													//Error message
		{
		BSP_LCD_SetFont(&Font15P);	
		BSP_LCD_SetTextColor(LCD_COLOR_RED);
		sprintf((char*)Buf, "%s", "ERROR");	
		BSP_LCD_DisplayStringAt(193,239,Buf, LEFT_MODE);
		sprintf((char*)Buf, "%2lu", (Tpos-MS_ERROR));			//ERROR NUMBER
		BSP_LCD_DisplayStringAt(252,239,Buf, LEFT_MODE);			
		}
	}


//function drawing range display
// 0 - 6%			0
// 1 - 10%		1
// 2 - 16%		2
// 3 - WIDE		3
void DrawTempoRange(uint8_t Range)
	{	
	BSP_LCD_SetFont(&FontBMP);
	if(Range==3)
		{
		BSP_LCD_SetTextColor(0xFFFF0000);
		}
	else
		{
		BSP_LCD_SetTextColor(0x30FF0000);	
		}
	sprintf((char*)Buf, "%s", "8");						//WIDE
	BSP_LCD_DisplayStringAt(445,227,Buf, TRANSPARENT_MODE);
	sprintf((char*)Buf, "%s", "9");						//WIDE
	BSP_LCD_DisplayStringAt(455,227,Buf, TRANSPARENT_MODE);	
		
	if(Range==2)
		{
		BSP_LCD_SetTextColor(0xFFFFFFFF);
		}
	else
		{
		BSP_LCD_SetTextColor(0x30FFFFFF);	
		}
	sprintf((char*)Buf, "%s", ")");						//+-16%
	BSP_LCD_DisplayStringAt(445,238,Buf, TRANSPARENT_MODE);
	sprintf((char*)Buf, "%s", "*");						//+-16%
	BSP_LCD_DisplayStringAt(455,238,Buf, TRANSPARENT_MODE);
		
	if(Range==1)
		{
		BSP_LCD_SetTextColor(0xFFFF0000);
		}
	else
		{
		BSP_LCD_SetTextColor(0x30FF0000);	
		}	
	sprintf((char*)Buf, "%s", "'");						//+-10%
	BSP_LCD_DisplayStringAt(445,249,Buf, TRANSPARENT_MODE);
	sprintf((char*)Buf, "%s", "(");						//+-10%
	BSP_LCD_DisplayStringAt(455,249,Buf, TRANSPARENT_MODE);	
	
	if(Range==0)
		{
		BSP_LCD_SetTextColor(0xFF00FF00);
		}
	else
		{
		BSP_LCD_SetTextColor(0x3000FF00);	
		}
	sprintf((char*)Buf, "%s", "+");						//+-6%
	BSP_LCD_DisplayStringAt(445,260,Buf, TRANSPARENT_MODE);
	sprintf((char*)Buf, "%s", ",");						//+-6%	
	BSP_LCD_DisplayStringAt(455,260,Buf, TRANSPARENT_MODE);	
	}



//function drawing Memory and Cue Pyramids
// 0 - nothing
// 1 - only Cue Pyramids
// 2 - only Memory Pyramids
// 3 - Memory and Cue Pyramids	
void DrawMemoryCuePyramid(uint8_t draw)
	{
	BSP_LCD_SetFont(&FontBMP);	
	if(draw==0 || draw==2)
		{
		BSP_LCD_SetTextColor(0xFF000000);	
		}
	else
		{
		BSP_LCD_SetTextColor(0xFFFF0000);		
		}
	sprintf((char*)Buf, "%s", " ");		//pyramid CUE		
	BSP_LCD_DisplayStringAt(1,256,Buf, TRANSPARENT_MODE);
	sprintf((char*)Buf, "%s", "!");		//pyramid CUE		
	BSP_LCD_DisplayStringAt(13,256,Buf, TRANSPARENT_MODE);
	
	if(draw==0 || draw==2)
		{
		BSP_LCD_SetTextColor(0xFF000000);	
		}
	else
		{
		BSP_LCD_SetTextColor(0xFF8080FF);		
		}
	sprintf((char*)Buf, "%s", " ");		//pyramid CUE		
	BSP_LCD_DisplayStringAt(1,156,Buf, TRANSPARENT_MODE);
	sprintf((char*)Buf, "%s", "!");		//pyramid CUE		
	BSP_LCD_DisplayStringAt(13,156,Buf, TRANSPARENT_MODE);
		
	if(draw==0 || draw==1)
		{
		BSP_LCD_SetTextColor(0xFF000000);	
		}
	else
		{
		BSP_LCD_SetTextColor(0xFFFF0000);		
		}
	sprintf((char*)Buf, "%s", "#");						//pyramid memory
	BSP_LCD_DisplayStringAt(1,230,Buf, TRANSPARENT_MODE);
	sprintf((char*)Buf, "%s", "$");						//pyramid memory
	BSP_LCD_DisplayStringAt(13,230,Buf, TRANSPARENT_MODE);
				
	if(draw==0 || draw==1)
		{
		BSP_LCD_SetTextColor(0xFF000000);	
		}
	else
		{
		BSP_LCD_SetTextColor(0xFF8080FF);		
		}
	sprintf((char*)Buf, "%s", "#");						//pyramid memory	
	BSP_LCD_DisplayStringAt(1, 83,Buf, TRANSPARENT_MODE);
	sprintf((char*)Buf, "%s", "$");						//pyramid memory	
	BSP_LCD_DisplayStringAt(13,83,Buf, TRANSPARENT_MODE);	
	}
	
void DrawMinuteMarkers(void)
	{
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);	
	uint16_t a = 3600000/all_long;
	uint8_t npoint = all_long/9000;
	while(npoint>0)
		{
		if(npoint*a<400)
			{
			BSP_LCD_DrawLine(40+npoint*a, 269, 40+npoint*a, 266);
			}	
		npoint--;		
		}
	}
	
	
	
//draw ZOOMGRID picture	on display
void DrawZOOMGRID(void)
	{
	uint8_t clr;
	uint32_t color;	
	uint16_t n = 0;
	while(n<5148)						//draw picture
		{
		color = 0x0000FF00;	
		clr = (LAYER[n*2+1]>>2)<<3;
		color += clr;	
		color = color<<8;
		clr = (LAYER[n*2+1]<<6)+((LAYER[n*2]>>5)<<3);
		color += clr;
		color = color<<8;		
		clr = LAYER[n*2]<<3;
		color += clr;
		BSP_LCD_DrawPixel(441+n%36, (174-n/36), color);
		n++;		
		}
	return;	
	}

//	draw logo Pioneer DJ on display
void DrawLOGO(void)
	{		
	uint8_t frame = 0;
	uint8_t VL = 0;	
	uint16_t n = 0;
	while(frame<26)							//change frame
		{	
		n = 0;	
		if(VL==0)
			{
			BSP_LCD_SelectLayer(1);	
			while(n<57600)						//draw picture
				{
				BSP_LCD_DrawPixel(n%480, (215-n/480), (0xFF000000+ANIMATION[frame][n][1]+256*ANIMATION[frame][n][2]+65536*ANIMATION[frame][n][0]));
				n++;		
				}	
			BSP_LCD_SetTransparency(1, 255);		//верхний слой виден
			BSP_LCD_SelectLayer(0);
			VL = 1;
			}
		else
			{
			BSP_LCD_SelectLayer(0);
			while(n<57600)						//draw picture
				{
				BSP_LCD_DrawPixel(n%480, (215-n/480), (0xFF000000+ANIMATION[frame][n][1]+256*ANIMATION[frame][n][2]+65536*ANIMATION[frame][n][0]));
				n++;		
				}		
			BSP_LCD_SetTransparency(1, 0);		//верхний не слой виден
			VL = 0;	
			}
		frame++;
		HAL_Delay(15);	
		}
	BSP_LCD_SetTransparency(1, 0);		//верхний не слой виден	
	BSP_LCD_SelectLayer(0);	
		
	HAL_Delay(330);	
	BSP_LCD_SetFont(&Font15P);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	sprintf((char*)Buf, "%s", "ver.");
	BSP_LCD_DisplayStringAt(384,257,Buf, LEFT_MODE);	
	BSP_LCD_DisplayStringAt(420,257,FIRMWARE_VERSION, LEFT_MODE);	
			
	BSP_LCD_SetTextColor(LCD_COLOR_GRAY);	
	BSP_LCD_DrawLine(101, 190, 379, 190);	
	BSP_LCD_DrawLine(101, 196, 379, 196);
	BSP_LCD_DrawLine(100, 191, 100, 195);
	BSP_LCD_DrawLine(380, 191, 380, 195);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);		
	n = 0;	
	while(n<277)
		{
		BSP_LCD_DrawLine(102+n, 192, 102+n, 194);
		n++;	
		HAL_Delay(11);	
		}	
	HAL_Delay(600);		
	return;	
	}	

///////////////////////////////////	
//
//	Draw REKORDBOX logo on Display
//	
void DrawREKORDBOX(void)
	{		
	uint16_t n = 0;
	
	BSP_LCD_SelectLayer(1);	
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_DrawLine(240,86,240,171);
	BSP_LCD_DrawLine(241,86,241,171);			
	while(n<11264)						//draw picture
		{
		BSP_LCD_DrawPixel((112+(n&0xFF)), (146-(n>>8)), (0xFF000000+REKBX[n][1]+256*REKBX[n][2]+65536*REKBX[n][0]));
		n++;		
		}
	BSP_LCD_SetTransparency(1, 255);		//верхний слой виден
	BSP_LCD_SelectLayer(0);		
	return;	
	}	
	
	
	
//Show NEEDLE on display	
void ShowNEEDLE(uint8_t needle)
	{
	if(needle)
		{	
		BSP_LCD_SetTextColor(LCD_COLOR_RED);
		}
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
		}
	BSP_LCD_SetFont(&FontBMP);
	sprintf((char*)Buf, "%s", "45");						//NEEDLE
	BSP_LCD_DisplayStringAt(142,185,Buf, TRANSPARENT_MODE);	
	}

//Show tempo func
//
//
void ShowTempo(uint16_t tempo)
	{
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_SetFont(&Font23D);

	if(tempo==10000)	
		{
		sprintf((char*)Buf, "%s", " ");			//NONE	
		}	
	else if(tempo>10000)
		{
		sprintf((char*)Buf, "%s", "+");			//PLUS	
		}
	else
		{
		sprintf((char*)Buf, "%s", "-");			//MINUS
		}

	if(tempo>10000)	
		{
		tempo = tempo - 10000;	
		}
	else
		{
		tempo = 10000-tempo;	
		}
		
	BSP_LCD_DisplayStringAt(323,194,Buf, LEFT_MODE);
	sprintf((char*)Buf, "%s", ".");			//dot for pitch
	
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_FillRect(364, 199, 19, 18);	
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);

	if(tempo_range==3)							//wide range
		{
		BSP_LCD_DisplayStringAt(379,194,Buf, TRANSPARENT_MODE);
		BSP_LCD_SetFont(&Font18D);
		sprintf((char*)Buf, "%3lu", tempo/100);			//PITCH
		BSP_LCD_DisplayStringAt(333, 199, Buf, LEFT_MODE);
		sprintf((char*)Buf, "%01lu", (tempo%100)/10);			//PITCH
		BSP_LCD_DisplayStringAt(384, 199, Buf, LEFT_MODE);	
		}
	else
		{
		BSP_LCD_DisplayStringAt(364,194,Buf, TRANSPARENT_MODE);
		BSP_LCD_SetFont(&Font18D);
		sprintf((char*)Buf, "%2lu", tempo/100);			//PITCH
		BSP_LCD_DisplayStringAt(333, 199, Buf, LEFT_MODE);	
		sprintf((char*)Buf, "%02lu", tempo%100);			//PITCH
		BSP_LCD_DisplayStringAt(369, 199, Buf, LEFT_MODE);		
		}
	}

//SHOW TRACK NUMBER
void ShowTrackNumber(uint8_t track)
	{
	track = track%100;	
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_SetFont(&Font26D);
	sprintf((char*)Buf, "%01lu", track/10);			//TRACK NUMBER x10
	BSP_LCD_DisplayStringAt(48,192,Buf, LEFT_MODE);
	sprintf((char*)Buf, "%01lu", track%10);			//TRACK NUMBER	x1
	BSP_LCD_DisplayStringAt(66,192,Buf, LEFT_MODE);	
	}

//Function redraw bar on static waveform and redraw dynamic waveform
//position = 1/150 sec
void RedrawWaveforms(uint32_t position)
	{
	if(position>all_long)
		{
		return;	
		}
	uint32_t clock_pos;	

	if(REMAIN_ENABLE)
		{
		clock_pos = all_long - position;	
		}	
	else
		{
		clock_pos	= position;
		}
		
	uint16_t t = clock_pos>>1;
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);	
	BSP_LCD_SetFont(&Font23D);
		
	if((PreviousSecondsPos != (t/75)%60) || forcibly_redraw==1)			//Speed adaptive alg/
		{
		PreviousSecondsPos = (t/75)%60;	
		sprintf((char *)Buf , "%02lu", t/4500);				//Min
		BSP_LCD_DisplayStringAt(139,194,Buf, LEFT_MODE);	
		sprintf((char *)Buf , "%02lu", PreviousSecondsPos);		//Sec
		BSP_LCD_DisplayStringAt(198,194,Buf, LEFT_MODE);
		}	
	sprintf((char *)Buf , "%02lu", t%75);					//F
	BSP_LCD_DisplayStringAt(247,194,Buf, LEFT_MODE);			
		
	BSP_LCD_SetFont(&Font18D);		
	if(clock_pos%2==1)
		{
		sprintf((char*)Buf, "%s", "5");	
		}
	else
		{
		sprintf((char*)Buf, "%s", "0");	
		}
	BSP_LCD_DisplayStringAt(291,199,Buf, LEFT_MODE);		
			
	if(needle_enable || (Tbuffer[23]&0x20))						//detecting touch on sensor or touch on jog 
		{
		if(RED_VERTICAL_LINE==0)
			{
			RED_VERTICAL_LINE = 1;	
			forcibly_redraw = 1;	
			}
		}
	else
		{
		if(RED_VERTICAL_LINE)
			{
			RED_VERTICAL_LINE = 0;	
			forcibly_redraw = 1;	
			}
		}

	DrawStaticWFM(position*399/all_long);	
		
	if(dSHOW==WAVEFORM)
		{
		position = position/DynamicWaveformZOOM;					//zoom correction	
		if(position != PreviousPositionDW || forcibly_redraw==1)
			{
			PreviousPositionDW = position;
			if(VisibleLayer==0)
				{
				BSP_LCD_SelectLayer(1);	
				intDRAW_WAVEFORM_FRAME(position);	
				BSP_LCD_SetTransparency(1, 255);		//верхний слой виден
				BSP_LCD_SelectLayer(0);
				VisibleLayer = 1;
				}
			else
				{
				BSP_LCD_SelectLayer(0);
				intDRAW_WAVEFORM_FRAME(position);	
				BSP_LCD_SetTransparency(1, 0);		//верхний не слой виден
				VisibleLayer = 0;	
				}
			ShowPhaseMeter(bars);
			if(originalBPM != BPMGRID[bars])										//вынести проверку за пределы (dSHOW==WAVEFORM)!!!
				{
				originalBPM = BPMGRID[bars];
				tempo_need_update = 2;		
				}	
			}
		}	
	forcibly_redraw = 0;
	}
//////////////////////////////////////////////	
//	
//	internal function for redraw waveform	
//	
void intDRAW_WAVEFORM_FRAME(uint32_t position)
	{
	uint32_t i, adr;
	uint16_t u, x; 	
	uint8_t	j;
	x = 0;
	u = 0;	
	if(position>=200)
		{
		while((BEATGRID[u]-(BEATGRID[u]%DynamicWaveformZOOM))<(DynamicWaveformZOOM*(position-200)))
			{
			u++;	
			}	
		}	
		
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_FillRect(40, 83, 400, 84);
	BSP_LCD_FillRect(37, 76, 409, 7);
	BSP_LCD_FillRect(37, 167, 409, 7);	
		
	for(i=0;i<400;i++)
		{	
		adr = DynamicWaveformZOOM*(i+position-200);	
		
		if(adr<=all_long)
			{
			if(number_of_memory_cue_points>0)					//Draw MEMORY CUE triangle
				{
				for(j=0;j<number_of_memory_cue_points;j++)
					{
					if((MEMORY_adr[0][j]-(MEMORY_adr[0][j]%DynamicWaveformZOOM))==adr)
						{
						BSP_LCD_SetTextColor(LCD_COLOR_RED);
						FillTriangle(i+37, i+43, i+40, 79, 79, 82);	
						}	
					}
				}	
			if(number_of_hot_cue_points>0)				//Draw HCUE triangle
				{
				BSP_LCD_SetFont(&Font8);	
				Buf[1] = 0;		
				for(j=0;j<3;j++)
					{
					if(HCUE_adr[0][j]!=0xFFFF)							
						{
						if((HCUE_adr[0][j]-(HCUE_adr[0][j]%DynamicWaveformZOOM))==adr)
							{
							if(HCUE_type[j]&0x1)
								{
								BSP_LCD_SetTextColor(0xFFF08138);	
								}
							else
								{
								BSP_LCD_SetTextColor(LCD_COLOR_GREEN);	
								}
							if(i<397)
								{
								Buf[0] = 35+j;
								BSP_LCD_DisplayStringAt(i+45, 76, Buf, TRANSPARENT_MODE);	
								BSP_LCD_DisplayStringAt(i+45, 167, Buf, TRANSPARENT_MODE);	
								}
							FillTriangle(i+37, i+43, i+40, 79, 79, 82);
							FillTriangle(i+37, i+43, i+40, 170, 170, 167);
							}
						}
					}
				}
			if((CUE_ADR-(CUE_ADR%DynamicWaveformZOOM))==adr) //Draw CUE triangle
				{
				BSP_LCD_SetTextColor(0xFFF08138);
				FillTriangle(i+37, i+43, i+40, 170, 170, 167);	
				}
				
			if((BEATGRID[u+x]-(BEATGRID[u+x]%DynamicWaveformZOOM))==adr)	
				{
				if(((u+x)%4)==0)					//red grid
					{
					ForceDrawVLine(i+40, 83, 6, LCD_COLOR_RED);
					ForceDrawVLine(i+40, 161, 6, LCD_COLOR_RED);
					}
				else if(DynamicWaveformZOOM<8)			//white grid
					{	
					ForceDrawVLine(i+40, 84, 5, LCD_COLOR_WHITE);
					ForceDrawVLine(i+40, 161, 5, LCD_COLOR_WHITE);		
					}
				x++;	
				}		
			}

		if(i==200)																		///you can optimize 1 raz draw red line
			{
			bars = u+x;		
			}
		else if(i==201)
			{
			}
		else
			{
			if(adr<=all_long)
				{
				if(DynamicWaveformZOOM==1)
					{
					ForceDrawVLine(i+40, 124-(WFORMDYNAMIC[adr]&0x1F), 2+2*(WFORMDYNAMIC[adr]&0x1F), COLOR_MAP[WFORMDYNAMIC[adr]>>5]);		//124-125px center		
					}
				else 	
					{
					uint8_t amplitude = (WFORMDYNAMIC[adr]&0x1F);
					uint8_t color = (WFORMDYNAMIC[adr]>>5);
					for(j=0;j<(DynamicWaveformZOOM-1);j++)
						{
						if((WFORMDYNAMIC[adr+j+1]&0x1F)>amplitude)
							{
							amplitude	= (WFORMDYNAMIC[adr+j+1]&0x1F);
							if(amplitude>17)
								{
								color = (WFORMDYNAMIC[adr+j+1]>>5);
								}
							}
						}
					ForceDrawVLine(i+40, 124-amplitude, 2+2*amplitude, COLOR_MAP[color]);		//124-125px center
					}
				}					
			}			
		}
	if(RED_VERTICAL_LINE)							//detecting touch on sensor or touch on jog 
		{
		ForceDrawVLine(240, 76, 98, LCD_COLOR_RED);			//red scroll
		ForceDrawVLine(241, 76, 98, LCD_COLOR_RED);		
		}
	else
		{
		ForceDrawVLine(240, 76, 98, LCD_COLOR_WHITE);	
		ForceDrawVLine(241, 76, 98, LCD_COLOR_WHITE);		
		}			
	}
	

//Draw CUE orange triangle on time bar	
// p=0 - disable triangle	
void DrawCueMarker(uint16_t p)
	{
	if(p<401)
		{
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		BSP_LCD_FillRect(37, 266, 406, 4);
		DrawMinuteMarkers();
		if(p!=0)
			{
			BSP_LCD_SetTextColor(0xFFF08138);
			FillTriangle(p+36, p+42, p+39, 269, 269, 266);
			}	
		}	
	}

////////////////////////////////////////////////	
//Draw MEMORY red triangle on time bar	
// p=0 - disable all triangle	
//#define NONE_MARK				0
//#define MEMORY_MARK			1
//#define HOT_CUE_A_MARK	2
//#define HOT_CUE_B_MARK	3
//#define HOT_CUE_C_MARK	4
void DrawMemoryMarker(uint16_t p, uint8_t type, uint32_t color)
	{
	if(type==NONE_MARK)
		{
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		BSP_LCD_FillRect(37, 223, 406, 7);
		return;
		}
	else if(p<401)
		{
		if(p!=0)
			{
			BSP_LCD_SetTextColor(color);
			if(type>1)
				{
				BSP_LCD_SetFont(&Font8);	
				Buf[1] = 0;	
				Buf[0] = 33+type;
				BSP_LCD_DisplayStringAt(p+44, 223, Buf, TRANSPARENT_MODE);		
				FillTriangle(p+36, p+42, p+39, 226, 226, 229);
				//FillTriangle(p+36, p+42, p+39, 269, 269, 266);				//перенести выполнение этой функции за пределы функции LOAD TRACK. Выполнять ее после DrawMinuteMarker!
				}
			else
				{	
				FillTriangle(p+36, p+42, p+39, 226, 226, 229);
				}	
			}	
		else
			{
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
			BSP_LCD_FillRect(37, 226, 406, 4);
			}
		return;
		}	
	}	
	
//init DAC on board discovery 
//and start SAI transaction
void DAC_Init_and_Start(uint8_t volume)
	{
	uint8_t _tmp[2];
	uint32_t deviceid = 0x00;
	AUDIO_DrvTypeDef          *audio_drv;
	HAL_SAI_Transmit(&hsai_BlockA2, _tmp, 2, 200);
  deviceid = wm8994_drv.ReadID(AUDIO_I2C_ADDRESS);
  if((deviceid) == WM8994_ID)
		{  
		wm8994_Reset(AUDIO_I2C_ADDRESS);
    audio_drv = &wm8994_drv; 
    audio_drv->Init(AUDIO_I2C_ADDRESS, OUTPUT_DEVICE_BOTH, volume, AUDIO_FREQUENCY_44K);
		wm8994_SetMute(AUDIO_I2C_ADDRESS, AUDIO_MUTE_OFF);	
		}
	HAL_SAI_Transmit_IT(&hsai_BlockA2, _tmp, 2);
	return;	
	}
	
	
//Rekordbox database parser
//Functions:
//Open and read file export.pdb
//find all tracks
//write trackname in massive
//write adress position datatrack in file export.pdb
//return number of tracks or 0 if error enable 	
uint16_t DATABASE_PARSER(void)
	{	
	res = f_open(&file, path_export, FA_READ);
	if (res != FR_OK)
		{
		return 0;	
		}		
	uint32_t FILSIZE = f_size(&file);
	char str[4] = ".DAT";
	uint32_t cursor = 0;
	uint16_t inputs = 0;
	uint16_t page = 0;
		
	while(page<(FILSIZE>>12))	
		{
		res = f_read(&file, WFORMDYNAMIC, 4096, &nbytes);
		if (res != FR_OK)
			{
			return 0;	
			}
		cursor = 0;	
		while(cursor<4096)
			{
			if(WFORMDYNAMIC[cursor]==str[1])
				{
				if(WFORMDYNAMIC[cursor-1]==str[0] & 
					 WFORMDYNAMIC[cursor+1]==str[2] & 
					 WFORMDYNAMIC[cursor+2]==str[3])				//".DAT" Finded!
					{
					parcser_adress[inputs] = (4096*page)+cursor;										//save position ".[D]AT"	
					cursor = cursor+16;	
					for(i=0;(i<54 & WFORMDYNAMIC[cursor+i] != 3);i++)						//copy track name	
						{		
						playlist[inputs][i] = WFORMDYNAMIC[cursor+i];
						}
					
					while(i<54)
						{
						playlist[inputs][i] = 32;
						i++;		
						}
						
					playlist[inputs][53] = 0;
					playlist[inputs][54] = 0;	
						
					i = 0;	
					inputs++;
					cursor += 40;	
					}
				}
			cursor++;	
			}
		page++;	
		}	
	
	f_close(&file);					//Close file Export.pdb

	return inputs;	
	}

////////////////////////////////////////////////////////////////////////
//	open export.pdb file, extract path for ANLZXXXX.DAT file
//	open ANLZXXXX.DAT file, extract path for audio file
//	extract static waveform data, bpm	
//	open ANLZXXXX.EXT file, extract dynamic waveform data, all_long data
//	open audio file
//
//	output: error code
////////////////////////////////////////////////////////////////////////	
uint16_t LOAD_TRACK(uint16_t TRACK_NUMBER)
		{
		if(TRACK_NUMBER==0)
			{
			return 1;				//invalid track number
			}
		uint16_t ERROR = 0;
		res = f_open(&file, path_export, FA_READ);
		if (res != FR_OK)
			{
			return 2;	//cannot open database
			}
		f_lseek(&file, (parcser_adress[TRACK_NUMBER-1]-42));			//find 0.DAT in file
		char path_ANLZ[45];
		res = f_read(&file, path_ANLZ, sizeof(path_ANLZ), &nbytes);
		if (res != FR_OK)
			{
			return 3;		//data not read
			}					
		f_close(&file);					//Close file Export.pdb
		path_ANLZ[0] = 48;
		path_ANLZ[1] = 58;	
		res = f_open(&file, path_ANLZ, FA_READ);
		if (res != FR_OK)
			{
			return 4;	//cannot open ANLZXXXX.DAT file
			}
		uint32_t FILSIZE = f_size(&file);
		if(FILSIZE>135000)
			{
			return 4;	
			}
		res = f_read(&file, WFORMDYNAMIC, FILSIZE, &nbytes);
		if (res != FR_OK)
			{
			return 5;	//cannot read ANLZXXXX.DAT file		
			}
		f_close(&file);					//Close file ANLZXXXX.DAT		
		uint32_t fsz;	
		fsz = WFORMDYNAMIC[8];			
		fsz = fsz<<8;	
		fsz+=WFORMDYNAMIC[9];	
		fsz = fsz<<8;
		fsz+=WFORMDYNAMIC[10];	
		fsz = fsz<<8;
		fsz+=WFORMDYNAMIC[11];
		if(fsz != FILSIZE)
			{
			return 6;   //file ANLZXXXX.DAT is damadge!		
			}
		fsz = WFORMDYNAMIC[4];			
		fsz = fsz<<8;	
		fsz+=WFORMDYNAMIC[5];	
		fsz = fsz<<8;
		fsz+=WFORMDYNAMIC[6];	
		fsz = fsz<<8;
		fsz+=WFORMDYNAMIC[7];
		uint32_t StPosHead = fsz;		
		if(WFORMDYNAMIC[StPosHead] != 80 |
			 WFORMDYNAMIC[StPosHead+1] != 80 |
			 WFORMDYNAMIC[StPosHead+2] != 84 | 
			 WFORMDYNAMIC[StPosHead+3] != 72)		//Check PPHT position in file
			{
			return 6;   //file ANLZXXXX.DAT is damadge!		
			}	
		fsz = WFORMDYNAMIC[StPosHead+4];			
		fsz = fsz<<8;	
		fsz+=WFORMDYNAMIC[StPosHead+5];	
		fsz = fsz<<8;
		fsz+=WFORMDYNAMIC[StPosHead+6];	
		fsz = fsz<<8;
		fsz+=WFORMDYNAMIC[StPosHead+7];								//head size
		uint32_t SPP = fsz+StPosHead+1;	
		fsz = WFORMDYNAMIC[StPosHead+12];			
		fsz = fsz<<8;	
		fsz+=WFORMDYNAMIC[StPosHead+13];	
		fsz = fsz<<8;
		fsz+=WFORMDYNAMIC[StPosHead+14];	
		fsz = fsz<<8;
		fsz+=WFORMDYNAMIC[StPosHead+15];							//Path Size
		char path_AUDIOTRACK[(fsz/2)+2];			//Create a Path for audiotrack
		uint16_t E=0;
		while(E<fsz+4)
			{
			path_AUDIOTRACK[(E/2)+2] = WFORMDYNAMIC[SPP+E];			//Fill path
			E=E+2;	
			}
		path_AUDIOTRACK[0] = 48;
		path_AUDIOTRACK[1] = 58;	
		fsz = WFORMDYNAMIC[StPosHead+8];			
		fsz = fsz<<8;	
		fsz+=WFORMDYNAMIC[StPosHead+9];	
		fsz = fsz<<8;
		fsz+=WFORMDYNAMIC[StPosHead+10];
		fsz = fsz<<8;
		fsz+=WFORMDYNAMIC[StPosHead+11];	
		StPosHead = StPosHead+fsz;	//PVBR position
		if(WFORMDYNAMIC[StPosHead] != 80 |
			 WFORMDYNAMIC[StPosHead+1] != 86 |
			 WFORMDYNAMIC[StPosHead+2] != 66 | 
			 WFORMDYNAMIC[StPosHead+3] != 82)	//Check PVBR position in file
			{
			return 6;   //file ANLZXXXX.DAT is damadge!		
			}	
		fsz = WFORMDYNAMIC[StPosHead+8];			
		fsz = fsz<<8;	
		fsz+=WFORMDYNAMIC[StPosHead+9];	
		fsz = fsz<<8;
		fsz+=WFORMDYNAMIC[StPosHead+10];	
		fsz = fsz<<8;
		fsz+=WFORMDYNAMIC[StPosHead+11];		
		StPosHead = StPosHead+fsz;	//PQTZ position
		if(WFORMDYNAMIC[StPosHead] != 80 |
			 WFORMDYNAMIC[StPosHead+1] != 81 |
			 WFORMDYNAMIC[StPosHead+2] != 84 | 
			 WFORMDYNAMIC[StPosHead+3] != 90)	//Check PQTZ position in file
			{
			return 6;   //file ANLZXXXX.DAT is damadge!		
			}	
		fsz = WFORMDYNAMIC[StPosHead+4];				
		fsz = fsz<<8;	
		fsz+=WFORMDYNAMIC[StPosHead+5];		
		fsz = fsz<<8;
		fsz+=WFORMDYNAMIC[StPosHead+6];		
		fsz = fsz<<8;
		fsz+=WFORMDYNAMIC[StPosHead+7];				//Head size
		uint32_t number_of_entries;
		number_of_entries = WFORMDYNAMIC[StPosHead+20];				
		number_of_entries = number_of_entries<<8;	
		number_of_entries+=WFORMDYNAMIC[StPosHead+21];		
		number_of_entries = number_of_entries<<8;
		number_of_entries+=WFORMDYNAMIC[StPosHead+22];		
		number_of_entries = number_of_entries<<8;
		number_of_entries+=WFORMDYNAMIC[StPosHead+23];				//calculate number_of_entries
		if(number_of_entries>4096)
			{
			number_of_entries = 4096;	
			}
		SPP = StPosHead + fsz + 2;						//start first BPM data.
		E = 0;	
		while(E<number_of_entries)
			{
			BPMGRID[E] = WFORMDYNAMIC[SPP+(E*8)];	
			BPMGRID[E] = BPMGRID[E]<<8;
			BPMGRID[E]+= WFORMDYNAMIC[SPP+1+(E*8)];		
			BEATGRID[E] = WFORMDYNAMIC[SPP+2+(E*8)];	
			BEATGRID[E] = BEATGRID[E]<<8;
			BEATGRID[E]+= WFORMDYNAMIC[SPP+3+(E*8)];	
			BEATGRID[E] = BEATGRID[E]<<8;
			BEATGRID[E]+= WFORMDYNAMIC[SPP+4+(E*8)];	
			BEATGRID[E] = BEATGRID[E]<<8;
			BEATGRID[E]+= WFORMDYNAMIC[SPP+5+(E*8)];	
			BEATGRID[E] = (BEATGRID[E]*3)/20; 	//translate ms to 1/150s frames
			E++;	
			}
		if(E==4096)
			{
			BEATGRID[4095] = 0xFFFF;	
			}
		else
			{
			BEATGRID[E] = 0xFFFF;	
			BPMGRID[E] = BPMGRID[E-1];	
			}
		originalBPM = BPMGRID[0];			//SEND ORIGINAL BPM		

		fsz = WFORMDYNAMIC[StPosHead+8];			
		fsz = fsz<<8;	
		fsz+=WFORMDYNAMIC[StPosHead+9];
		fsz = fsz<<8;
		fsz+=WFORMDYNAMIC[StPosHead+10];	
		fsz = fsz<<8;
		fsz+=WFORMDYNAMIC[StPosHead+11];	
		StPosHead = StPosHead+fsz;	//PWAV position	
		if(WFORMDYNAMIC[StPosHead] != 80 |
			 WFORMDYNAMIC[StPosHead+1] != 87 |
			 WFORMDYNAMIC[StPosHead+2] != 65 | 
			 WFORMDYNAMIC[StPosHead+3] != 86)		//Check PWAV position in file
			{
			return 6;   //file ANLZXXXX.DAT is damadge!		
			}
		fsz = WFORMDYNAMIC[StPosHead+4];			
		fsz = fsz<<8;	
		fsz+=WFORMDYNAMIC[StPosHead+5];
		fsz = fsz<<8;
		fsz+=WFORMDYNAMIC[StPosHead+6];	
		fsz = fsz<<8;
		fsz+=WFORMDYNAMIC[StPosHead+7];				//PWAV Header size
		fsz+=StPosHead;		
		E = 0;
		while(E<400)
			{
			WFORMSTATIC[E] = WFORMDYNAMIC[fsz+E];			//Fill Static Waveform
			E++;	
			}		
		fsz = WFORMDYNAMIC[StPosHead+8];			
		fsz = fsz<<8;	
		fsz+=WFORMDYNAMIC[StPosHead+9];
		fsz = fsz<<8;
		fsz+=WFORMDYNAMIC[StPosHead+10];	
		fsz = fsz<<8;
		fsz+=WFORMDYNAMIC[StPosHead+11];				//PWAV TAG size
		StPosHead+=fsz;													//PWV2 start adress
		if(WFORMDYNAMIC[StPosHead] != 80 |
			 WFORMDYNAMIC[StPosHead+1] != 87 |
			 WFORMDYNAMIC[StPosHead+2] != 86 | 
			 WFORMDYNAMIC[StPosHead+3] != 50)		//Check PWV2 position in file
			{
			return 6;   //file ANLZXXXX.DAT is damadge!		
			}
		fsz = WFORMDYNAMIC[StPosHead+8];			
		fsz = fsz<<8;	
		fsz+=WFORMDYNAMIC[StPosHead+9];
		fsz = fsz<<8;
		fsz+=WFORMDYNAMIC[StPosHead+10];	
		fsz = fsz<<8;
		fsz+=WFORMDYNAMIC[StPosHead+11];				//PWV2 Tag size
		StPosHead = StPosHead+fsz;	//PCOB position				
		if(WFORMDYNAMIC[StPosHead] != 80 |
			 WFORMDYNAMIC[StPosHead+1] != 67 |
			 WFORMDYNAMIC[StPosHead+2] != 79 | 
			 WFORMDYNAMIC[StPosHead+3] != 66)		//Check PCOB position in file
			{
			return 6;   //file ANLZXXXX.DAT is damadge!		
			}		
		DrawMemoryMarker(0, NONE_MARK, LCD_COLOR_BLACK);
		MemoryCuePyramid_ENABLE = 1;	
		uint32_t PCOB2_adr;
			
		//////////////////prepare HOT CUES	
		for(E=0;E<3;E++)									//Clear нcue massive
			{
			HCUE_type[E] = 0;	 							//b0 (0=cue / 1=loop); b1 (0=inactive / 1=active); 		
			HCUE_adr[0][E] = 0xFFFF;		
			HCUE_adr[1][E] = 0xFFFF;		
			}
		
		if(WFORMDYNAMIC[StPosHead+15]==1)    //check type PCOB - for HOT CUE points
			{
			number_of_hot_cue_points = WFORMDYNAMIC[StPosHead+19]&0xF;				//number of hotcue points		
			
			if(number_of_hot_cue_points>0)
				{
				MemoryCuePyramid_ENABLE = 3;	
				}
			fsz = WFORMDYNAMIC[StPosHead+8];			
			fsz = fsz<<8;	
			fsz+=WFORMDYNAMIC[StPosHead+9];
			fsz = fsz<<8;
			fsz+=WFORMDYNAMIC[StPosHead+10];	
			fsz = fsz<<8;
			fsz+=WFORMDYNAMIC[StPosHead+11];				//PCOB TAG size
			PCOB2_adr = fsz+StPosHead;							//start adress PCOB2
			fsz = WFORMDYNAMIC[StPosHead+4];			
			fsz = fsz<<8;	
			fsz+=WFORMDYNAMIC[StPosHead+5];
			fsz = fsz<<8;
			fsz+=WFORMDYNAMIC[StPosHead+6];	
			fsz = fsz<<8;
			fsz+=WFORMDYNAMIC[StPosHead+7];				//PCOB head size
			StPosHead+= fsz;		

			uint8_t HCUE_NAME = 0;
		
			E = 0;	
			while(E<number_of_hot_cue_points)	
				{	
				fsz = WFORMDYNAMIC[StPosHead+8];			
				fsz = fsz<<8;	
				fsz+=WFORMDYNAMIC[StPosHead+9];
				fsz = fsz<<8;
				fsz+=WFORMDYNAMIC[StPosHead+10];	
				fsz = fsz<<8;
				fsz+=WFORMDYNAMIC[StPosHead+11];				//PCPT TAG size
				SPP = StPosHead+fsz;										//Next PCPT adress

				HCUE_NAME = WFORMDYNAMIC[StPosHead+15];
				if(WFORMDYNAMIC[StPosHead+19]!=0)				//when hcue active
					{
					if((HCUE_NAME<4) && (HCUE_NAME>0))
						{
						HCUE_type[HCUE_NAME-1] = 2;			//write 
						}
					}
					
				fsz = WFORMDYNAMIC[StPosHead+4];			
				fsz = fsz<<8;	
				fsz+=WFORMDYNAMIC[StPosHead+5];
				fsz = fsz<<8;
				fsz+=WFORMDYNAMIC[StPosHead+6];	
				fsz = fsz<<8;
				fsz+=WFORMDYNAMIC[StPosHead+7];				//PCPT Head size
				StPosHead+= fsz;		
		
				if((HCUE_NAME<4) && (HCUE_NAME>0))
					{
					HCUE_NAME--;
					HCUE_adr[0][HCUE_NAME] = WFORMDYNAMIC[StPosHead+4];
					HCUE_adr[0][HCUE_NAME] = HCUE_adr[0][HCUE_NAME]<<8;
					HCUE_adr[0][HCUE_NAME]+= WFORMDYNAMIC[StPosHead+5];
					HCUE_adr[0][HCUE_NAME] = HCUE_adr[0][HCUE_NAME]<<8;
					HCUE_adr[0][HCUE_NAME]+= WFORMDYNAMIC[StPosHead+6];
					HCUE_adr[0][HCUE_NAME] = HCUE_adr[0][HCUE_NAME]<<8;	
					HCUE_adr[0][HCUE_NAME]+= WFORMDYNAMIC[StPosHead+7];
					if(WFORMDYNAMIC[StPosHead]==2)						//when hot cue type=loop
						{
						HCUE_type[HCUE_NAME] |= 0x1;		
						HCUE_adr[1][HCUE_NAME] = WFORMDYNAMIC[StPosHead+8];
						HCUE_adr[1][HCUE_NAME] = HCUE_adr[1][HCUE_NAME]<<8;
						HCUE_adr[1][HCUE_NAME]+= WFORMDYNAMIC[StPosHead+9];
						HCUE_adr[1][HCUE_NAME] = HCUE_adr[1][HCUE_NAME]<<8;
						HCUE_adr[1][HCUE_NAME]+= WFORMDYNAMIC[StPosHead+10];
						HCUE_adr[1][HCUE_NAME] = HCUE_adr[1][HCUE_NAME]<<8;	
						HCUE_adr[1][HCUE_NAME]+= WFORMDYNAMIC[StPosHead+11];
						HCUE_adr[1][HCUE_NAME] = (HCUE_adr[1][HCUE_NAME]*3)/20; 	//translate ms to 1/150s frames
						}
					}
				StPosHead = SPP;	
				E++;	
				}
			}
			
		//////////////////prepare MEMORY CUES
		for(E=0;E<10;E++)									//Clear memory cue massive
			{
			MEMORY_type[E] = 0;						//b0 (0=cue / 1=loop); b1 (0=inactive / 1=active); 		
			MEMORY_adr[0][E] = 0xFFFF;
			MEMORY_adr[1][E] = 0xFFFF;	
			}

		StPosHead = PCOB2_adr;	
		if(WFORMDYNAMIC[StPosHead+15]==0)    //check type PCOB - for MEMORY CUE points
			{
			number_of_memory_cue_points = WFORMDYNAMIC[StPosHead+19]&0xF;				//number of MEMORY points		
			if(number_of_memory_cue_points>0)
				{
				MemoryCuePyramid_ENABLE = 3;	
				}
			fsz = WFORMDYNAMIC[StPosHead+4];			
			fsz = fsz<<8;	
			fsz+=WFORMDYNAMIC[StPosHead+5];
			fsz = fsz<<8;
			fsz+=WFORMDYNAMIC[StPosHead+6];	
			fsz = fsz<<8;
			fsz+=WFORMDYNAMIC[StPosHead+7];				//PCOB head size
			StPosHead = StPosHead+fsz;		

			E = 0;	
			while(E<number_of_memory_cue_points)	
				{	
				fsz = WFORMDYNAMIC[StPosHead+8];			
				fsz = fsz<<8;	
				fsz+=WFORMDYNAMIC[StPosHead+9];
				fsz = fsz<<8;
				fsz+=WFORMDYNAMIC[StPosHead+10];	
				fsz = fsz<<8;
				fsz+=WFORMDYNAMIC[StPosHead+11];				//PCPT TAG size
				SPP = StPosHead+fsz;										//Next PCPT adress

				if(WFORMDYNAMIC[StPosHead+19]!=0)				//when hcue active
					{
					MEMORY_type[E] = 2;			//write 
					}	
					
				fsz = WFORMDYNAMIC[StPosHead+4];			
				fsz = fsz<<8;	
				fsz+=WFORMDYNAMIC[StPosHead+5];
				fsz = fsz<<8;
				fsz+=WFORMDYNAMIC[StPosHead+6];	
				fsz = fsz<<8;
				fsz+=WFORMDYNAMIC[StPosHead+7];				//PCPT Head size
				StPosHead = StPosHead+fsz;		
		
				MEMORY_adr[0][E] = WFORMDYNAMIC[StPosHead+4];
				MEMORY_adr[0][E] = MEMORY_adr[0][E]<<8;
				MEMORY_adr[0][E]+= WFORMDYNAMIC[StPosHead+5];
				MEMORY_adr[0][E] = MEMORY_adr[0][E]<<8;
				MEMORY_adr[0][E]+= WFORMDYNAMIC[StPosHead+6];
				MEMORY_adr[0][E] = MEMORY_adr[0][E]<<8;	
				MEMORY_adr[0][E]+= WFORMDYNAMIC[StPosHead+7];	
				if(WFORMDYNAMIC[StPosHead]==2)						//when hot cue type=loop
					{
					MEMORY_type[E] |= 0x1;		
					MEMORY_adr[1][E] = WFORMDYNAMIC[StPosHead+8];
					MEMORY_adr[1][E] = MEMORY_adr[1][E]<<8;
					MEMORY_adr[1][E]+= WFORMDYNAMIC[StPosHead+9];
					MEMORY_adr[1][E] = MEMORY_adr[1][E]<<8;
					MEMORY_adr[1][E]+= WFORMDYNAMIC[StPosHead+10];
					MEMORY_adr[1][E] = MEMORY_adr[1][E]<<8;	
					MEMORY_adr[1][E]+= WFORMDYNAMIC[StPosHead+11];	
					MEMORY_adr[1][E] = (MEMORY_adr[1][E]*3)/20; 	//translate ms to 1/150s frames
					}
				StPosHead = SPP;	
				E++;	
				}
			}
			
		path_ANLZ[42] = 69;								//	
		path_ANLZ[43] = 88;								//	Replace *.DAT to *.EXT
		path_ANLZ[44] = 84;								//	for open EXT file with dynamic waveform	

		res = f_open(&file, path_ANLZ, FA_READ);	
		if (res != FR_OK)
			{
			ERROR = 7;	//cannot open ANLZXXXX.EXT file
			}
		else
			{
			FILSIZE = f_size(&file);
			fsz = FILSIZE;
			if(fsz>135000)
				{
				fsz = 135000;	
				}
			res = f_read(&file, WFORMDYNAMIC, fsz, &nbytes);
			if(res != FR_OK)
				{
				ERROR = 8;	//ANLZXXXX.EXT file is damadge
				f_close(&file);					//Close file ANLZXXXX.EXT							
				}
			else
				{
				f_close(&file);					//Close file ANLZXXXX.EXT					
				fsz = WFORMDYNAMIC[8];			
				fsz = fsz<<8;	
				fsz+=WFORMDYNAMIC[9];	
				fsz = fsz<<8;
				fsz+=WFORMDYNAMIC[10];	
				fsz = fsz<<8;
				fsz+=WFORMDYNAMIC[11];	//file size		
				if(fsz != FILSIZE)
					{
					ERROR = 9;	//ANLZXXXX.EXT file is damadge
					}
				else		
					{	
					fsz = WFORMDYNAMIC[4];			
					fsz = fsz<<8;	
					fsz+=WFORMDYNAMIC[5];	
					fsz = fsz<<8;
					fsz+=WFORMDYNAMIC[6];	
					fsz = fsz<<8;
					fsz+=WFORMDYNAMIC[7];	//Header size
					StPosHead = fsz;	
					if(WFORMDYNAMIC[StPosHead] != 80 | 
						 WFORMDYNAMIC[StPosHead+1] != 80 | 
					   WFORMDYNAMIC[StPosHead+2] != 84 | 
					   WFORMDYNAMIC[StPosHead+3] != 72)		//Check PPTH position in file
						{
						ERROR = 10;	//ANLZXXXX.EXT file is damadge				
						}
					else
						{		
						fsz = WFORMDYNAMIC[StPosHead+8];			
						fsz = fsz<<8;	
						fsz+=WFORMDYNAMIC[StPosHead+9];	
						fsz = fsz<<8;
						fsz+=WFORMDYNAMIC[StPosHead+10];	
						fsz = fsz<<8;
						fsz+=WFORMDYNAMIC[StPosHead+11];	//Tag size
						StPosHead += fsz;		
						if(WFORMDYNAMIC[StPosHead] != 80 | 
							 WFORMDYNAMIC[StPosHead+1] != 87 | 
						   WFORMDYNAMIC[StPosHead+2] != 86 | 
						   WFORMDYNAMIC[StPosHead+3] != 51)		//Check PWV3 position in file
							{
							return 11;	//ANLZXXXX.EXT file is damadge			
							}
						else
							{	
							fsz = WFORMDYNAMIC[StPosHead+4];			
							fsz = fsz<<8;	
							fsz+=WFORMDYNAMIC[StPosHead+5];	
							fsz = fsz<<8;
							fsz+=WFORMDYNAMIC[StPosHead+6];	
							fsz = fsz<<8;
							fsz+=WFORMDYNAMIC[StPosHead+7];	//Header size
							SPP = StPosHead + fsz; //Start position waveform	 
									
							fsz = WFORMDYNAMIC[StPosHead+16];			
							fsz = fsz<<8;	
							fsz+=WFORMDYNAMIC[StPosHead+17];	
							fsz = fsz<<8;
							fsz+=WFORMDYNAMIC[StPosHead+18];	
							fsz = fsz<<8;
							fsz+=WFORMDYNAMIC[StPosHead+19];		//waveform data size
							all_long = fsz;
							if(fsz>135000-SPP)
								{
								fsz = 135000-SPP;	
								}

							uint32_t j;							//Data shift
							for(j=0;j<fsz;j++)
								{
								WFORMDYNAMIC[j] = WFORMDYNAMIC[j+SPP];	
								}
								
							for(E=0;E<number_of_memory_cue_points;E++)											//Draw CUES on Display
								{
								if(MEMORY_adr[0][E] != 0xFFFF)
									{
									DrawMemoryMarker(1+(60*MEMORY_adr[0][E]/all_long), MEMORY_MARK, LCD_COLOR_RED);	
									MEMORY_adr[0][E] = (MEMORY_adr[0][E]*3)/20;				//translate ms to 1/150s frames
									}	
								}	

							////////////////////////////////////sorting algoritm	
							uint8_t MINM, ii;
												
							for(E=0;E<number_of_memory_cue_points-1;E++)	
								{
								MINM = E;
								for(ii=(E+1);ii<number_of_memory_cue_points;ii++)
									{
									if(MEMORY_adr[0][ii]<MEMORY_adr[0][MINM])
										{
										j = MEMORY_adr[0][MINM];
										MEMORY_adr[0][MINM] = MEMORY_adr[0][ii];
										MEMORY_adr[0][ii] = j;
										j = MEMORY_adr[1][MINM];
										MEMORY_adr[1][MINM] = MEMORY_adr[1][ii];
										MEMORY_adr[1][ii] = j;												
										j = MEMORY_type[MINM];
										MEMORY_type[MINM] = MEMORY_type[ii];
										MEMORY_type[ii] = j;	
										}
									}
								}

							for(E=0;E<3;E++)											//Draw MEMORY on Display
								{
								if(HCUE_adr[0][E] != 0xFFFF)
									{
									if(HCUE_type[E]&0x1)
										{
										DrawMemoryMarker(1+(60*HCUE_adr[0][E]/all_long), 2+E, 0xFFF08138);				//Draw ORANGE MARKER
										}
									else
										{
										DrawMemoryMarker(1+(60*HCUE_adr[0][E]/all_long), 2+E, LCD_COLOR_GREEN);				//Draw GREEN MARKER	
										}
									HCUE_adr[0][E] = (HCUE_adr[0][E]*3)/20;				//translate ms to 1/150s frames										
									if(E==0)							//HCUE_A
										{
										if(HCUE_type[E]&0x1)
											{
											Tbuffer[18] |= 0x1;	
											Tbuffer[17] &= 0x7F;	
											}
										else
											{
											Tbuffer[17] |= 0x80;
											Tbuffer[18] &= 0x7E;	
											}
										}
									else if(E==1)					//HCUE_B
										{
										if(HCUE_type[E]&0x1)
											{
											Tbuffer[18] |= 0x8;	
											Tbuffer[18] &= 0x7B;	
											}
										else
											{	
											Tbuffer[18] |= 0x4;
											Tbuffer[18] &= 0x77;	
											}
										}	
									else if(E==2)					//HCUE_C
										{
										if(HCUE_type[E]&0x1)
											{
											Tbuffer[18] |= 0x40;		
											Tbuffer[18] &= 0x5F;	
											}
										else
											{	
											Tbuffer[18] |= 0x20;
											Tbuffer[18] &= 0x3F;	
											}
										}	
									}
								else				//turn off leds
									{
									if(E==0)							//HCUE_A
										{
										Tbuffer[17] &= 0x7F;
										Tbuffer[18] &= 0x7E;	
										}
									else if(E==1)					//HCUE_B
										{
										Tbuffer[18] &= 0x73;		 
										}	
									else if(E==2)					//HCUE_C
										{	
										Tbuffer[18] &= 0x1F;		
										}
									}									
								}
							}
						}
					}			
				}	
			}

		res = f_open(&file, path_AUDIOTRACK, FA_READ);				//Open audio file
		if (res != FR_OK)
			{
			return 13;	//cannot open AUDIOTRACK
			}
		res = f_read(&file, PCM, 512, &nbytes);
		if(res != FR_OK)
			{
			ERROR = 14;	//cannot read AUDIOTRACK
 			}
		if(PCM[0][5][0] != 1 || PCM[0][5][1] != 2 || PCM[0][6][0] != 44100 || PCM[0][8][1] != 16)	//Check audio format
			{
			ERROR = 15;	//unsupported audio format	
			}
		f_lseek(&file, 44);
		if(ERROR==0)
			{
			playlist[TRACK_NUMBER-1][54] |= 0x01;				//write history mem
			}
		return ERROR;	
		}

		
///////////////////////////////////////////		
//preparing for loading next track
//
void PREPARE_LOAD_TRACK(uint16_t TRACK_NUMBER)
	{
	uint16_t ERR = 0;	
	f_close(&file);	
	pitch = 0;	
	play_enable = 0;
	if(Tbuffer[19]&0x8)					//OFF_SLIP_MODE
		{
		Tbuffer[19] &= 0xF7;	
		}		
	play_adr = 0;	
	slip_play_adr = 0;	
	ERR = LOAD_TRACK(TRACK_NUMBER);		
	if(ERR==0)
		{		
		tempo_need_update = 1;
		ShowTrackNumber(TRACK_NUMBER);
		DrawMemoryCuePyramid(MemoryCuePyramid_ENABLE);			
		RedrawWaveforms(0);	
		SwitchInformationLayer(WAVEFORM);
		DrawStaticWFM(DRAW_NEW_STATIC_WAVEFORM);				//Draw New waveform
		DrawMinuteMarkers();	
		end_adr_valid_data = 0;
		start_adr_valid_data = 0;
		RED_CIRCLE_CUE_ADR = 1;
		CUE_ADR = 0;		
		}
	else
		{
		all_long = 0;		
		originalBPM = 0xFFFF;	
		ShowBPM(originalBPM);
		ShowTrackNumber(0);
		MemoryCuePyramid_ENABLE = 0;	
		DrawMemoryCuePyramid(MemoryCuePyramid_ENABLE);
		SwitchInformationLayer(WAVEFORM);	
		DrawStaticWFM(CLEAR_WAVEFORM_ARRAY);	
		DrawStaticWFM(MS_ERROR+ERR);	
		RED_CIRCLE_CUE_ADR = 0;	
		}		
	}	


//////////////////////////////////////	
//	Show Phase meter 0 - none 1...4 phase	
//		
void ShowPhaseMeter(uint16_t phase)
	{
	uint8_t j;
			
	if(PreviousPhase==phase)
		{
		return;	
		}
	if(phase==0xFFFF)
		{
		BSP_LCD_SetFont(&Font13D);	
		BSP_LCD_SetTextColor(0xFFF08138);
		BSP_LCD_DrawRect(178, 36, 29, 9);		
		BSP_LCD_FillRect(207, 39, 3, 3);	
		BSP_LCD_DrawRect(210, 36, 29, 9);
		BSP_LCD_FillRect(239, 39, 3, 3);
		BSP_LCD_DrawRect(242, 36, 29, 9);
		BSP_LCD_FillRect(271, 39, 3, 3);	
		BSP_LCD_DrawRect(274, 36, 29, 9);
		sprintf((char*)Buf, "%s", ".");	
		BSP_LCD_DisplayStringAt(327, 33, Buf, LEFT_MODE);	
		sprintf((char*)Buf, "%s", "-");	
		BSP_LCD_DisplayStringAt(310, 33, Buf, LEFT_MODE);
		sprintf((char*)Buf, "%s", "-");	
		BSP_LCD_DisplayStringAt(321, 33, Buf, LEFT_MODE);		
		sprintf((char*)Buf, "%s", "-");				
		BSP_LCD_DisplayStringAt(336, 33, Buf, LEFT_MODE);		
		BSP_LCD_SetFont(&FontBMP);
		sprintf((char*)Buf, "%s", ":");						//Bars
		BSP_LCD_DisplayStringAt(348, 40, Buf, TRANSPARENT_MODE);
		sprintf((char*)Buf, "%s", ";<=");						//MASTER PLAYER
		BSP_LCD_DisplayStringAt(117, 29, Buf, TRANSPARENT_MODE);
		sprintf((char*)Buf, "%s", ">?@");						//MASTER PLAYER
		BSP_LCD_DisplayStringAt(117, 40, Buf, TRANSPARENT_MODE);
		BSP_LCD_SetTextColor(0x55F08138);						//GRADIENT
		BSP_LCD_DrawHLine(179, 40, 27);	
		BSP_LCD_DrawHLine(211, 40, 27);	
		BSP_LCD_DrawHLine(243, 40, 27);	
		BSP_LCD_DrawHLine(275, 40, 27);	
		BSP_LCD_SetTextColor(0x69F08138);
		BSP_LCD_DrawHLine(179, 41, 27);	
		BSP_LCD_DrawHLine(211, 41, 27);	
		BSP_LCD_DrawHLine(243, 41, 27);	
		BSP_LCD_DrawHLine(275, 41, 27);	
		BSP_LCD_SetTextColor(0x82F08138);
		BSP_LCD_DrawHLine(179, 42, 27);	
		BSP_LCD_DrawHLine(211, 42, 27);	
		BSP_LCD_DrawHLine(243, 42, 27);	
		BSP_LCD_DrawHLine(275, 42, 27);	
		BSP_LCD_SetTextColor(0x9BF08138);
		BSP_LCD_DrawHLine(179, 43, 27);	
		BSP_LCD_DrawHLine(211, 43, 27);	
		BSP_LCD_DrawHLine(243, 43, 27);	
		BSP_LCD_DrawHLine(275, 43, 27);	
		BSP_LCD_SetFont(&Font13D);	
		BSP_LCD_SetTextColor(LCD_COLOR_LIGHTBLUE);
		BSP_LCD_DrawRect(178, 50, 29, 9);
		BSP_LCD_FillRect(207, 53, 3, 3);	
		BSP_LCD_DrawRect(210, 50, 29, 9);
		BSP_LCD_FillRect(239, 53, 3, 3);
		BSP_LCD_DrawRect(242, 50, 29, 9);
		BSP_LCD_FillRect(271, 53, 3, 3);	
		BSP_LCD_DrawRect(274, 50, 29,	 9);	
		sprintf((char*)Buf, "%s", ".");	
		BSP_LCD_DisplayStringAt(327,49, Buf, LEFT_MODE);	
		sprintf((char*)Buf, "%s", "-");	
		BSP_LCD_DisplayStringAt(310, 49, Buf, LEFT_MODE);
		sprintf((char*)Buf, "%s", "-");	
		BSP_LCD_DisplayStringAt(321, 49, Buf, LEFT_MODE);		
		sprintf((char*)Buf, "%s", "-");				
		BSP_LCD_DisplayStringAt(336, 49, Buf, LEFT_MODE);			
		BSP_LCD_SetFont(&FontBMP);
		sprintf((char*)Buf, "%s", ":");						//Bars
		BSP_LCD_DisplayStringAt(348, 56, Buf, TRANSPARENT_MODE);
		BSP_LCD_SetTextColor(0x558080FF);					//GRADIENT
		BSP_LCD_DrawHLine(179, 54, 27);	
		BSP_LCD_DrawHLine(211, 54, 27);	
		BSP_LCD_DrawHLine(243, 54, 27);	
		BSP_LCD_DrawHLine(275, 54, 27);	
		BSP_LCD_SetTextColor(0x698080FF);
		BSP_LCD_DrawHLine(179, 53, 27);	
		BSP_LCD_DrawHLine(211, 53, 27);	
		BSP_LCD_DrawHLine(243, 53, 27);	
		BSP_LCD_DrawHLine(275, 53, 27);	
		BSP_LCD_SetTextColor(0x828080FF);
		BSP_LCD_DrawHLine(179, 52, 27);	
		BSP_LCD_DrawHLine(211, 52, 27);	
		BSP_LCD_DrawHLine(243, 52, 27);	
		BSP_LCD_DrawHLine(275, 52, 27);	
		BSP_LCD_SetTextColor(0x9B8080FF);
		BSP_LCD_DrawHLine(179, 51, 27);	
		BSP_LCD_DrawHLine(211, 51, 27);	
		BSP_LCD_DrawHLine(243, 51, 27);	
		BSP_LCD_DrawHLine(275, 51, 27);	
		}
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
		BSP_LCD_FillRect((179+32*((PreviousPhase+3)%4)), 55, 27, 3);
		BSP_LCD_SetTextColor(0x558080FF);
		BSP_LCD_DrawHLine((179+32*((PreviousPhase+3)%4)), 54, 27);		
		BSP_LCD_SetTextColor(0x698080FF);
		BSP_LCD_DrawHLine((179+32*((PreviousPhase+3)%4)), 53, 27);	
		BSP_LCD_SetTextColor(0x828080FF);
		BSP_LCD_DrawHLine((179+32*((PreviousPhase+3)%4)), 52, 27);	
		BSP_LCD_SetTextColor(0x9B8080FF);
		BSP_LCD_DrawHLine((179+32*((PreviousPhase+3)%4)), 51, 27);	
		BSP_LCD_SetTextColor(LCD_COLOR_LIGHTBLUE);	
		BSP_LCD_DrawRect(178+32*((PreviousPhase+3)%4), 50, 29, 9);	
			
		if((phase+3)%4==0)															//RED PHASE
			{
			for(j=0; j<56; j++)	
				{
				BSP_LCD_DrawPixel((192+j%14), 54-(j/14), RED_BAR[j/14][j%14]);	
				BSP_LCD_DrawPixel((192+j%14), 54+(j/14), RED_BAR[j/14][j%14]);		
				BSP_LCD_DrawPixel((192-j%14), 54-(j/14), RED_BAR[j/14][j%14]);	
				BSP_LCD_DrawPixel((192-j%14), 54+(j/14), RED_BAR[j/14][j%14]);		
				}	
			}
		else																						//BLUE PHASE
			{	
			for(j=0; j<56; j++)	
				{
				BSP_LCD_DrawPixel((192+j%14)+32*((phase+3)%4), 54-(j/14), BLUE_BAR[j/14][j%14]);	
				BSP_LCD_DrawPixel((192+j%14)+32*((phase+3)%4), 54+(j/14), BLUE_BAR[j/14][j%14]);		
				BSP_LCD_DrawPixel((192-j%14)+32*((phase+3)%4), 54-(j/14), BLUE_BAR[j/14][j%14]);	
				BSP_LCD_DrawPixel((192-j%14)+32*((phase+3)%4), 54+(j/14), BLUE_BAR[j/14][j%14]);		
				}	
			}	
		BSP_LCD_SetTextColor(0xFFA6C8FF);	
		BSP_LCD_DrawRect(178+32*((phase+3)%4), 50, 29, 9);		

		BSP_LCD_SetFont(&Font13D);
		BSP_LCD_SetTextColor(LCD_COLOR_LIGHTBLUE);
		sprintf((char*)Buf, "%s", ".");	
		BSP_LCD_DisplayStringAt(327, 49, Buf, LEFT_MODE);	
		sprintf((char *)Buf , "%01lu", ((phase+3)/40)%10);	
		BSP_LCD_DisplayStringAt(310, 49, Buf, LEFT_MODE);
		sprintf((char *)Buf , "%01lu", ((phase+3)>>2)%10);	
		BSP_LCD_DisplayStringAt(321, 49, Buf, LEFT_MODE);		
		sprintf((char *)Buf , "%01lu", ((phase+3)%4)+1);					
		BSP_LCD_DisplayStringAt(336, 49, Buf, LEFT_MODE);	
		}		
	PreviousPhase = phase;
	return;	
	}

//Switch Dynamic Waveform/Browser
//
//	
void SwitchInformationLayer(uint8_t LAY)
	{
	uint8_t update_all_page = 0;
	if(dSHOW != LAY)
		{
		dSHOW = LAY;
		update_all_page = 1;	
		}	
	uint16_t E;	
	if(LAY==WAVEFORM)		//X => WAVEFORM
		{
		HAL_GPIO_WritePin(GPIOB, LED_BROWSE_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, LED_TAG_LIST_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED_INFO_GPIO_Port, LED_INFO_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, LED_MENU_Pin, GPIO_PIN_RESET);		
		BSP_LCD_SelectLayer(1);
		BSP_LCD_SetTransparency(1, 0);	
		BSP_LCD_Clear(0x00000000);
		BSP_LCD_SelectLayer(0);
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
		BSP_LCD_FillRect(0, 0, 480, 182);	
		for(E=0;E<480;E++)
			{
			ForceDrawVLine(E, 0, 18, 0xFF0000F0-E/2);	
			}		
		BSP_LCD_SetFont(&Font15P);
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		if(track_number>0)
			{	
			sprintf((char*)Buf, "%s", "~");
			BSP_LCD_DisplayStringAt(5,2,Buf, TRANSPARENT_MODE);		
			BSP_LCD_DisplayStringAt(21,2, playlist[track_number-1], TRANSPARENT_MODE);
			}
		DrawMemoryCuePyramid(MemoryCuePyramid_ENABLE);
		DrawZOOMGRID();
		PreviousPhase = 0;										//forcibly draw phasemeter static parts
		ShowPhaseMeter(0xFFFF);		
		if(track_number!=0)
			{	
			forcibly_redraw = 1;	
			}
		else
			{
			DrawREKORDBOX();	
			}
		return;
		}
	else if(LAY==BROWSER)													//X => BROWSER
		{
		if(update_all_page==1)
			{
			HAL_GPIO_WritePin(GPIOB, LED_BROWSE_Pin, GPIO_PIN_SET);	
			HAL_GPIO_WritePin(GPIOB, LED_TAG_LIST_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_INFO_GPIO_Port, LED_INFO_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, LED_MENU_Pin, GPIO_PIN_RESET);		
			BSP_LCD_SetTransparency(1, 0);	
			BSP_LCD_SelectLayer(1);
			BSP_LCD_Clear(0x00000000);	
			BSP_LCD_SelectLayer(0);
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
			BSP_LCD_FillRect(0, 0, 480, 182);		
			for(E=0;E<480;E++)
				{
				ForceDrawVLine(E, 0, 18, 0xFF0000F0-E/2);	
				}

			intDrawLayer0_NOINFO(BCurrentCursorPosition);	
			
			BSP_LCD_SelectLayer(1);	
			BSP_LCD_SetTransparency(1, 255);		
			BSP_LCD_SetFont(&Font15P);
			BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
			sprintf((char*)Buf, "%s", "[TRACK]");						
			BSP_LCD_DisplayStringAt(5,2,Buf, TRANSPARENT_MODE);
			sprintf((char*)Buf, "%s", "Total Track");						
			BSP_LCD_DisplayStringAt(350,2,Buf, TRANSPARENT_MODE);
			sprintf((char *)Buf , "%01lu", TOTAL_TRACKS);					
			BSP_LCD_DisplayStringAt(450,2,Buf, TRANSPARENT_MODE);		
			}
		else
			{
			BSP_LCD_SelectLayer(1);		
			BSP_LCD_SetTextColor(0x00000000);	
			BSP_LCD_FillRect(50, 20+(19*BCurrentCursorPosition), 15, 19);		
			}
		int_B_DRAW_ALL_LINES();	
		return;
		}
	else if(LAY==TAG_LIST)													//X => TAG LIST
		{
		HAL_GPIO_WritePin(GPIOB, LED_TAG_LIST_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, LED_BROWSE_Pin, GPIO_PIN_RESET);	
		HAL_GPIO_WritePin(LED_INFO_GPIO_Port, LED_INFO_Pin, GPIO_PIN_RESET);	
		HAL_GPIO_WritePin(GPIOB, LED_MENU_Pin, GPIO_PIN_RESET);		
		BSP_LCD_SetTransparency(1, 0);	
		BSP_LCD_SelectLayer(1);
		BSP_LCD_Clear(0x00000000);	
		BSP_LCD_SelectLayer(0);
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
		BSP_LCD_FillRect(0, 0, 480, 182);	
			
		BSP_LCD_SetTextColor(0xFF173367);
		BSP_LCD_FillRect(0, 0, 480, 18);		
			
		intDrawLayer0_NOINFO(TCurrentCursorPosition);
			
		BSP_LCD_SelectLayer(1);	
		BSP_LCD_SetTransparency(1, 255);		
		BSP_LCD_SetFont(&Font15P);
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		sprintf((char*)Buf, "%s", "TAG LIST");			
		BSP_LCD_DisplayStringAt(5,2,Buf, TRANSPARENT_MODE);
		sprintf((char*)Buf, "%s", "Total Track");						
		BSP_LCD_DisplayStringAt(350,2,Buf, TRANSPARENT_MODE);
		sprintf((char *)Buf , "%01lu", TOTAL_TRACKS_IN_TAG_LIST);					
		BSP_LCD_DisplayStringAt(450,2,Buf, TRANSPARENT_MODE);	
		int_T_DRAW_ALL_LINES();	
		return;	
		}
	else if(LAY==BROWSER_INFO)													//X => BROWSER_INFO
		{
		if(update_all_page==1)
			{	
			HAL_GPIO_WritePin(GPIOB, LED_BROWSE_Pin, GPIO_PIN_SET);	
			HAL_GPIO_WritePin(GPIOB, LED_TAG_LIST_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_INFO_GPIO_Port, LED_INFO_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB, LED_MENU_Pin, GPIO_PIN_RESET);				
			BSP_LCD_SetTransparency(1, 0);	
			BSP_LCD_SelectLayer(1);
			BSP_LCD_Clear(0x00000000);	
			BSP_LCD_SelectLayer(0);
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
			BSP_LCD_FillRect(0, 0, 480, 182);		
			for(E=0;E<480;E++)
				{
				ForceDrawVLine(E, 0, 18, 0xFF0000F0-E/2);	
				}
		
			intDrawLayer0_INFO(BCurrentCursorPosition);
							
			BSP_LCD_SelectLayer(1);	
			BSP_LCD_SetTransparency(1, 255);		
			BSP_LCD_SetFont(&Font15P);
			BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
			sprintf((char*)Buf, "%s", "[TRACK]");						
			BSP_LCD_DisplayStringAt(5,2,Buf, TRANSPARENT_MODE);
			sprintf((char*)Buf, "%s", "Total Track");						
			BSP_LCD_DisplayStringAt(350,2,Buf, TRANSPARENT_MODE);
			sprintf((char *)Buf , "%01lu", TOTAL_TRACKS);					
			BSP_LCD_DisplayStringAt(450,2,Buf, TRANSPARENT_MODE);
			}
		else
			{
			BSP_LCD_SelectLayer(1);		
			BSP_LCD_SetTextColor(0x00000000);	
			BSP_LCD_FillRect(20, 20+(19*BCurrentCursorPosition), 15, 19);		
			}
		int_BI_DRAW_ALL_LINES();				
		return;
		}
	else if(LAY==TAG_LIST_INFO)													//X => TAG LIST INFO
		{
		HAL_GPIO_WritePin(GPIOB, LED_TAG_LIST_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, LED_BROWSE_Pin, GPIO_PIN_RESET);	
		HAL_GPIO_WritePin(LED_INFO_GPIO_Port, LED_INFO_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, LED_MENU_Pin, GPIO_PIN_RESET);				
		BSP_LCD_SetTransparency(1, 0);	
		BSP_LCD_SelectLayer(1);
		BSP_LCD_Clear(0x00000000);	
		BSP_LCD_SelectLayer(0);
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
		BSP_LCD_FillRect(0, 0, 480, 182);	
			
		BSP_LCD_SetTextColor(0xFF173367);
		BSP_LCD_FillRect(0, 0, 480, 18);		
		
		intDrawLayer0_INFO(TCurrentCursorPosition);

		BSP_LCD_SelectLayer(1);	
		BSP_LCD_SetTransparency(1, 255);		
		BSP_LCD_SetFont(&Font15P);
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		sprintf((char*)Buf, "%s", "TAG LIST");			
		BSP_LCD_DisplayStringAt(5,2,Buf, TRANSPARENT_MODE);
		sprintf((char*)Buf, "%s", "Total Track");						
		BSP_LCD_DisplayStringAt(350,2,Buf, TRANSPARENT_MODE);
		sprintf((char *)Buf , "%01lu", TOTAL_TRACKS_IN_TAG_LIST);					
		BSP_LCD_DisplayStringAt(450,2,Buf, TRANSPARENT_MODE);	
		int_TI_DRAW_ALL_LINES();		
		return;	
		}
	else if(LAY==UTILITY)															//X => UTILITY
		{
		HAL_GPIO_WritePin(GPIOB, LED_TAG_LIST_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, LED_BROWSE_Pin, GPIO_PIN_RESET);	
		HAL_GPIO_WritePin(LED_INFO_GPIO_Port, LED_INFO_Pin, GPIO_PIN_RESET);		
		HAL_GPIO_WritePin(GPIOB, LED_MENU_Pin, GPIO_PIN_SET);	
		BSP_LCD_SetTransparency(1, 0);	
		BSP_LCD_SelectLayer(1);
		BSP_LCD_Clear(0x00000000);	
		BSP_LCD_SelectLayer(0);
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
		BSP_LCD_FillRect(0, 0, 480, 182);	
		BSP_LCD_SetTextColor(LCD_COLOR_DARK_2);
		BSP_LCD_FillRect(0, 0, 480, 18);		
		BSP_LCD_SetTextColor(LCD_COLOR_DARK_1);
		BSP_LCD_DrawRect(0, 18, 14, 152);	
			
		BSP_LCD_SetTextColor(0xFF0F0F0);	
		BSP_LCD_FillRect(14, 18, 466, 152);	
			
		BSP_LCD_SetTextColor(LCD_COLOR_DARK_1);
		BSP_LCD_DrawLine(14,18,479,18);
		BSP_LCD_DrawLine(14,37,479,37);
		BSP_LCD_DrawLine(14,56,479,56);
		BSP_LCD_DrawLine(14,75,479,75);
		BSP_LCD_DrawLine(14,94,479,94);
		BSP_LCD_DrawLine(14,113,479,113);
		BSP_LCD_DrawLine(14,132,479,132);
		BSP_LCD_DrawLine(14,151,479,151);
		BSP_LCD_DrawLine(0,170,479,170);
		BSP_LCD_DrawLine(479,18,479,170);
		BSP_LCD_DrawLine(270,18,270,170);				//vertical line

		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);		
		BSP_LCD_FillRect(14, (18+(19*UCurrentCursorPosition)), 256, 9);			////Draw selected cursor	
		BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_2);
		BSP_LCD_FillRect(14, (27+(19*UCurrentCursorPosition)), 256, 5);
		BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_1);
		BSP_LCD_FillRect(14, (32+(19*UCurrentCursorPosition)), 256, 5);
		BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_3);		
		BSP_LCD_FillRect(271, (18+(19*UCurrentCursorPosition)), 208, 9);			////Draw selected cursor	
		BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_5);
		BSP_LCD_FillRect(271, (27+(19*UCurrentCursorPosition)), 208, 5);
		BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_4);
		BSP_LCD_FillRect(271, (32+(19*UCurrentCursorPosition)), 208, 5);
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		FillTriangle(20, 20, 27, 20+(19*UCurrentCursorPosition), 34+(19*UCurrentCursorPosition), 27 +(19*UCurrentCursorPosition));	
		
		BSP_LCD_SelectLayer(1);	
		BSP_LCD_SetTransparency(1, 255);		
		BSP_LCD_SetFont(&Font15P);
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		sprintf((char*)Buf, "%s", "UTILITY");			
		BSP_LCD_DisplayStringAt(5,2,Buf, TRANSPARENT_MODE);

		int_U_DRAW_ALL_LINES();	
		return;	
		}
	}

//Redraw scroll on new position	
void ReDrawScroll(uint16_t total_elements, uint16_t current_element_pos)
	{
	ForceDrawVLine(5, ScrollPosition+24, ScrollLong-2, LCD_COLOR_BLACK);							//
	ForceDrawVLine(6, ScrollPosition+23, ScrollLong, LCD_COLOR_BLACK);								//	SCROLL
	ForceDrawVLine(7, ScrollPosition+23, ScrollLong, LCD_COLOR_BLACK);								//
	ForceDrawVLine(8, ScrollPosition+24, ScrollLong-2, LCD_COLOR_BLACK);							//
	
		///Calculate scroll	
	if(total_elements<9)
		{
		ScrollLong = 142;
		ScrollPosition = 0;	
		}
	else
		{
		ScrollLong = 1136/total_elements;
		if(ScrollLong>142)
			{
			ScrollLong = 142;	
			}
		else if(ScrollLong<5)
			{
			ScrollLong = 5;	
			}	
		ScrollPosition = ((current_element_pos-1)*(142-ScrollLong))/(total_elements-8);	
		}
	ForceDrawVLine(5, ScrollPosition+24, ScrollLong-2, LCD_COLOR_WHITE);							//
	ForceDrawVLine(6, ScrollPosition+23, ScrollLong, 	LCD_COLOR_WHITE);								//	SCROLL
	ForceDrawVLine(7, ScrollPosition+23, ScrollLong, 	LCD_COLOR_WHITE);								//
	ForceDrawVLine(8, ScrollPosition+24, ScrollLong-2, LCD_COLOR_WHITE);							//		
	}

//Navigate function for browser, taglis and utility
//
//	BROWSER_DOWN		0 position track-- (cursor up)
//	BROWSER_UP			1 position track++ (cursor down)
//	TAGLIST_DOWN		2	position track-- (cursor up)
//	TAGLIST_UP			3	position track++ (cursor down)
//	
void NAVIGATOR(uint8_t UPDOWN)
	{	
	if(dSHOW==BROWSER || dSHOW==BROWSER_INFO)		///////////////////////Browser mode selected
		{
		if(UPDOWN==BROWSER_UP)
			{
			if(BCurrentCursorPosition==7)												//All lines update++
				{
				if(BCurrentTrackPosition == TOTAL_TRACKS-7)
					{
					return;	
					}
				else
					{
					BSP_LCD_SelectLayer(1);	
					BSP_LCD_SetTransparency(1, 255);
					BSP_LCD_SetTextColor(0x00000000);
					BSP_LCD_FillRect(14, 18, 466, 152);		
					BSP_LCD_SetFont(&Font15P);
					BCurrentTrackPosition++;
					if(dSHOW==BROWSER)
						{
						int_B_DRAW_ALL_LINES();
						}
					else
						{
						int_BI_DRAW_ALL_LINES();	
						}
					return;
					}
				}
			else																								//One line update++
				{
				if(TOTAL_TRACKS>8 || BCurrentCursorPosition<(TOTAL_TRACKS-1))
					{
					if(dSHOW==BROWSER)
						{	
						int_B_DRAW_ONE_LINE(UPDOWN);
						}
					else
						{
						int_BI_DRAW_ONE_LINE(UPDOWN);	
						}
					}
				return;
				}
			}
		else if(UPDOWN==BROWSER_DOWN)
			{
			if(BCurrentCursorPosition==0)											//All lines update--
				{
				if(BCurrentTrackPosition == 1)
					{
					return;	
					}
				else
					{
					BSP_LCD_SelectLayer(1);	
					BSP_LCD_SetTransparency(1, 255);
					BSP_LCD_SetTextColor(0x00000000);
					BSP_LCD_FillRect(14, 18, 466, 152);	
					BSP_LCD_SetFont(&Font15P);
					BCurrentTrackPosition--;
					if(dSHOW==BROWSER)
						{
						int_B_DRAW_ALL_LINES();
						}
					else
						{
						int_BI_DRAW_ALL_LINES();	
						}	
					return;						
					}	
				}
			else														//One line update--
				{
				if(dSHOW==BROWSER)
					{	
					int_B_DRAW_ONE_LINE(UPDOWN);
					}
				else
					{
					int_BI_DRAW_ONE_LINE(UPDOWN);	
					}	
				return;			
				}	
			}	
		return;
		}
	else if(dSHOW==TAG_LIST || dSHOW==TAG_LIST_INFO)	///////////////////TAG LIST mode selected
		{
		if(UPDOWN==TAGLIST_UP)
			{
			if(TCurrentCursorPosition==7)												//All lines update++
				{
				if(TCurrentTrackPosition == TOTAL_TRACKS_IN_TAG_LIST-7)
					{
					return;	
					}
				else
					{
					BSP_LCD_SelectLayer(1);	
					BSP_LCD_SetTransparency(1, 255);
					BSP_LCD_SetTextColor(0x00000000);
					BSP_LCD_FillRect(14, 18, 466, 152);		
					BSP_LCD_SetFont(&Font15P);
					TCurrentTrackPosition++;
					if(dSHOW==TAG_LIST)
						{
						int_T_DRAW_ALL_LINES();
						}
					else
						{
						int_TI_DRAW_ALL_LINES();	
						}
					return;
					}
				}
			else																								//One line update++
				{
				if(TOTAL_TRACKS_IN_TAG_LIST>8 || TCurrentCursorPosition<(TOTAL_TRACKS_IN_TAG_LIST-1))
					{
					if(dSHOW==TAG_LIST)
						{	
						int_T_DRAW_ONE_LINE(UPDOWN);
						}
					else
						{
						int_TI_DRAW_ONE_LINE(UPDOWN);	
						}
					}
				return;
				}
			}
		else if(UPDOWN==TAGLIST_DOWN)
			{
			if(TCurrentCursorPosition==0)											//All lines update--
				{
				if(TCurrentTrackPosition == 1)
					{
					return;	
					}
				else
					{
					BSP_LCD_SelectLayer(1);	
					BSP_LCD_SetTransparency(1, 255);
					BSP_LCD_SetTextColor(0x00000000);
					BSP_LCD_FillRect(14, 18, 466, 152);	
					BSP_LCD_SetFont(&Font15P);
					TCurrentTrackPosition--;
					if(dSHOW==TAG_LIST)
						{
						int_T_DRAW_ALL_LINES();
						}
					else
						{
						int_TI_DRAW_ALL_LINES();	
						}	
					return;						
					}	
				}
			else														//One line update--
				{
				if(dSHOW==TAG_LIST)
					{	
					int_T_DRAW_ONE_LINE(UPDOWN);
					}
				else
					{
					int_TI_DRAW_ONE_LINE(UPDOWN);	
					}	
				return;			
				}	
			}	
		return;	
		}
	else if(dSHOW==UTILITY)		///////////////////////UTILITY mode selected
		{
		if(UPDOWN==UTILITY_UP)
			{
			if(UCurrentCursorPosition==7)												//All lines update++
				{
				if(CurrentUPosition == TOTAL_U_POSITIONS-7)
					{
					return;	
					}
				else
					{
					BSP_LCD_SelectLayer(1);	
					BSP_LCD_SetTransparency(1, 255);
					BSP_LCD_SetTextColor(0x00000000);
					BSP_LCD_FillRect(14, 18, 466, 152);		
					BSP_LCD_SetFont(&Font15P);
					CurrentUPosition++;
					int_U_DRAW_ALL_LINES();
					return;
					}
				}
			else if(TOTAL_U_POSITIONS>8 || UCurrentCursorPosition<(TOTAL_U_POSITIONS-1))								//One line update++
				{
				int_U_DRAW_ONE_LINE(UPDOWN);
				return;
				}
			}
		else if(UPDOWN==UTILITY_DOWN)
			{
			if(UCurrentCursorPosition==0)											//All lines update--
				{
				if(CurrentUPosition == 1)
					{
					return;	
					}
				else
					{
					BSP_LCD_SelectLayer(1);	
					BSP_LCD_SetTransparency(1, 255);
					BSP_LCD_SetTextColor(0x00000000);
					BSP_LCD_FillRect(14, 18, 466, 152);	
					BSP_LCD_SetFont(&Font15P);
					CurrentUPosition--;
					int_U_DRAW_ALL_LINES();
					return;						
					}	
				}
			else														//One line update--
				{
				int_U_DRAW_ONE_LINE(UPDOWN);
				return;			
				}	
			}	
		return;
		}
	return;
	}
	
	
////////////////////////////////////////////////////
//draw triangle for browser with INFO
//
void intDrawTriangle(uint8_t CurrentCursorPosition)
	{
	BSP_LCD_SetTextColor(LCD_COLOR_PAPER);
	BSP_LCD_DrawLine(270, 18, 270, 169)	;	
	BSP_LCD_SetTextColor(LCD_COLOR_SHADOW);
	BSP_LCD_DrawLine(271, 19, 271, 168);	
		
	for(uint16_t j = 0; j<304; j++)
		{
		BSP_LCD_DrawPixel(256+(j&0xF), ((36+(19*CurrentCursorPosition))-(j>>4)), (0xFF000000+strelka[j][0]+256*strelka[j][1]+65536*strelka[j][2]));
		}	
	BSP_LCD_DrawPixel(271, 18, LCD_COLOR_PAPER);	
	BSP_LCD_DrawPixel(271, 19, LCD_COLOR_SHADOW);	
	BSP_LCD_DrawPixel(271, 169, LCD_COLOR_PAPER);	
	BSP_LCD_DrawPixel(271, 168, LCD_COLOR_SHADOW);		
	return;
	}
	
/////////////////////////////////////////////////	
//
//draw layer 0 for INFO BROWSER and TAGLIST
//
void intDrawLayer0_INFO(uint8_t CurrentCursorPosition)
	{
	uint8_t E;	
	BSP_LCD_SetTextColor(LCD_COLOR_DARK_1);
	BSP_LCD_DrawRect(0, 18, 14, 152);	
	for(E=0;E<4;E++)															/////Draw gray lines
		{
		if(CurrentCursorPosition%2==1 | (CurrentCursorPosition/2) != E)
			{		
			BSP_LCD_FillRect(14, 18+(38*E), 256, 19);
			}			
		}
	BSP_LCD_SetTextColor(LCD_COLOR_DARK_2);
	for(E=0;E<4;E++)
		{
		if(CurrentCursorPosition%2==0 | (CurrentCursorPosition/2) != E)
			{				
			BSP_LCD_FillRect(14, 37+(38*E), 256, 19);
			}			
		}
		
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);		
	BSP_LCD_FillRect(14, (18+(19*CurrentCursorPosition)), 242, 9);			////Draw selected cursor
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_2);
	BSP_LCD_FillRect(14, (27+(19*CurrentCursorPosition)), 242, 5);
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_1);
	BSP_LCD_FillRect(14, (32+(19*CurrentCursorPosition)), 242, 5);
			
	BSP_LCD_SetTextColor(LCD_COLOR_PAPER);					//Draw paper rectangle
	BSP_LCD_FillRect(270, 18, 210, 152);	
	BSP_LCD_SetTextColor(LCD_COLOR_SHADOW);					//Shadow
	BSP_LCD_DrawRect(271, 19, 208, 150);
	intDrawTriangle(CurrentCursorPosition);					//Draw triangle	
	for(uint16_t j = 0; j<7921; j++)					//Label
		{
		BSP_LCD_DrawPixel(384+(j%89), 120-j/89, (0xFF000000+disc[j][0]+256*disc[j][1]+65536*disc[j][2]));
		}		
	uint8_t j, k;
	for(j=0;j<7;j++)							////Dots
		{
		for(k=0;k<47;k++)
			{
			BSP_LCD_DrawPixel(282+2*k, 36+19*j, LCD_COLOR_DARK_1);
			}
		}
	for(k=0;k<192;k++)					////ARTIST
		{
		if((iARTIST[k/16]>>(k%16))%2)
			{
			BSP_LCD_DrawPixel(300-k%16, 40+k/16, LCD_COLOR_DARK_1);
			}	
		}	
	for(k=0;k<192;k++)					////TIME
		{
		if((iTIME[k/16]>>(k%16))%2)
			{
			BSP_LCD_DrawPixel(300-k%16, 59+k/16, LCD_COLOR_DARK_1);
			}	
		}
	for(k=0;k<192;k++)					////BPM
		{
		if((iBPM[k/16]>>(k%16))%2)
			{
			BSP_LCD_DrawPixel(299-k%16, 78+k/16, LCD_COLOR_DARK_1);
			}	
		}
	for(k=0;k<224;k++)					////TONE
		{
		if((iTONE[k/16]>>(k%16))%2)
			{
			BSP_LCD_DrawPixel(299-k%16, 97+k/16, LCD_COLOR_DARK_1);
			}	
		}
	for(k=0;k<208;k++)					////DISC
		{
		if((iDISC[k/16]>>(k%16))%2)
			{
			BSP_LCD_DrawPixel(299-k%16, 135+k/16, LCD_COLOR_DARK_1);
			}	
		}
	for(k=0;k<224;k++)					////COMENTS
		{
		if((iCOMMENTS[k/16]>>(k%16))%2)
			{
			BSP_LCD_DrawPixel(299-k%16, 153+k/16, LCD_COLOR_DARK_1);
			}	
		}	
	for(j=0;j<5;j++)							////STARS
		{	
		for(k=0;k<208;k++)
			{
			if((iSTAR[k/16]>>(k%16))%2)
				{
				BSP_LCD_DrawPixel((j*14)+299-k%16, 116+k/16, LCD_COLOR_DARK_2);
				}	
			}
		}
	BSP_LCD_SetFont(&Font15P);
	BSP_LCD_SetTextColor(LCD_COLOR_DARK_2);
	sprintf((char*)Buf, "%s", "House");			
	BSP_LCD_DisplayStringAt(301, 135, Buf, TRANSPARENT_MODE);
	return;	
	}


	
//////////////////////////////////////////////////	
//	
//	draw layer 0 for without INFO BROWSER and TAGLIS	
void intDrawLayer0_NOINFO(uint8_t CurrentCursorPosition)
	{
	uint8_t E;
		
	BSP_LCD_SetTextColor(LCD_COLOR_DARK_1);
	BSP_LCD_DrawRect(0, 18, 14, 152);	
	for(E=0;E<4;E++)							/////Draw gray lines
		{
		if(CurrentCursorPosition%2==1 | (CurrentCursorPosition/2) != E)
			{		
			BSP_LCD_FillRect(14, 18+(38*E), 466, 19);
			}			
		}
	BSP_LCD_SetTextColor(LCD_COLOR_DARK_2);
	for(E=0;E<4;E++)
		{
		if(CurrentCursorPosition%2==0 | (CurrentCursorPosition/2) != E)
			{				
			BSP_LCD_FillRect(14, 37+(38*E), 466, 19);
			}			
		}

	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);		
	BSP_LCD_FillRect(14, (18+(19*CurrentCursorPosition)), 466, 9);			////Draw selected cursor	
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_2);
	BSP_LCD_FillRect(14, (27+(19*CurrentCursorPosition)), 466, 5);
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_1);
	BSP_LCD_FillRect(14, (32+(19*CurrentCursorPosition)), 466, 5);
	return;	
	}
	
	
/////////////////////////////////	
//
//internal function for Browser
void int_B_DRAW_ALL_LINES(void)
	{
	uint16_t E;
	for(E=0;E<8 && TOTAL_TRACKS>(E+BCurrentTrackPosition-1);E++)
		{
		if(E==BCurrentCursorPosition && (E+BCurrentTrackPosition == track_number | (playlist[E+BCurrentTrackPosition-1][54]%2)==1))
			{
			BSP_LCD_SetTextColor(LCD_COLOR_DGREEN);	
			}
		else if(E+BCurrentTrackPosition == track_number | (playlist[E+BCurrentTrackPosition-1][54]%2)==1)	
			{
			BSP_LCD_SetTextColor(LCD_COLOR_GREEN);	
			}
		else if(E==BCurrentCursorPosition)
			{
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
			}
		else
			{
			BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
			}
		sprintf((char *)Buf , "%03lu", E+BCurrentTrackPosition);					
		BSP_LCD_DisplayStringAt(20,20+(19*E),Buf, TRANSPARENT_MODE);
		BSP_LCD_DisplayStringAt(69,20+(19*E), playlist[E+BCurrentTrackPosition-1], TRANSPARENT_MODE);	
		if(E+BCurrentTrackPosition == track_number)
			{
			sprintf((char*)Buf, "%s", ">");	
			}
		else if((playlist[E+BCurrentTrackPosition-1][54]&0x2)==0)
			{
			sprintf((char*)Buf, "%s", "~");
			}	
		else
			{
			BSP_LCD_SetTextColor(LCD_COLOR_RED);
			sprintf((char*)Buf, "%s", "<");	
			}			
		BSP_LCD_DisplayStringAt(50,20+(19*E),Buf, TRANSPARENT_MODE);	
		}	
	BSP_LCD_SelectLayer(0);
	ReDrawScroll(TOTAL_TRACKS, BCurrentTrackPosition);	
	}
/////////////////////////////////	
//
//internal function for Browser
void int_B_DRAW_ONE_LINE(uint8_t UPDOWN)							
	{
	if(BCurrentCursorPosition%2==0)
		{
		BSP_LCD_SetTextColor(LCD_COLOR_DARK_1);
		}
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_DARK_2);
		}
	BSP_LCD_FillRect(14, 18+(19*BCurrentCursorPosition), 466, 19);
	BSP_LCD_SelectLayer(1);	
	BSP_LCD_SetTransparency(1, 255);	
	BSP_LCD_SetFont(&Font15P);
	if(BCurrentTrackPosition+BCurrentCursorPosition == track_number | (playlist[BCurrentTrackPosition+BCurrentCursorPosition-1][54]%2)==1)
		{
		BSP_LCD_SetTextColor(LCD_COLOR_GREEN);	
		}	
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		}		
	sprintf((char *)Buf , "%03lu", BCurrentTrackPosition+BCurrentCursorPosition);					
	BSP_LCD_DisplayStringAt(20,20+(19*BCurrentCursorPosition),Buf, TRANSPARENT_MODE);
	BSP_LCD_DisplayStringAt(69,20+(19*BCurrentCursorPosition), playlist[BCurrentCursorPosition+BCurrentTrackPosition-1], TRANSPARENT_MODE);	
	if(BCurrentTrackPosition+BCurrentCursorPosition == track_number)
		{
		sprintf((char*)Buf, "%s", ">");	
		}	
	else if((playlist[BCurrentTrackPosition+BCurrentCursorPosition-1][54]&0x2)==0)
		{
		sprintf((char*)Buf, "%s", "~");
		}		
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_RED);
		sprintf((char*)Buf, "%s", "<");		
		}
	BSP_LCD_DisplayStringAt(50,20+(19*BCurrentCursorPosition),Buf, TRANSPARENT_MODE);	
	if(UPDOWN==1)
		{
		BCurrentCursorPosition++;
		}
	else
		{
		BCurrentCursorPosition--;	
		}
	if(BCurrentTrackPosition+BCurrentCursorPosition == track_number | (playlist[BCurrentTrackPosition+BCurrentCursorPosition-1][54]%2)==1)
		{
		BSP_LCD_SetTextColor(LCD_COLOR_DGREEN);	
		}	
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		}		
	sprintf((char *)Buf , "%03lu", BCurrentTrackPosition+BCurrentCursorPosition);					
	BSP_LCD_DisplayStringAt(20,20+(19*BCurrentCursorPosition),Buf, TRANSPARENT_MODE);	
	BSP_LCD_DisplayStringAt(69,20+(19*BCurrentCursorPosition), playlist[BCurrentCursorPosition+BCurrentTrackPosition-1], TRANSPARENT_MODE);	
	if(BCurrentTrackPosition+BCurrentCursorPosition == track_number)
		{
		sprintf((char*)Buf, "%s", ">");	
		}	
	else if((playlist[BCurrentTrackPosition+BCurrentCursorPosition-1][54]&0x2)==0)
		{
		sprintf((char*)Buf, "%s", "~");
		}		
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_RED);
		sprintf((char*)Buf, "%s", "<");		
		}
	BSP_LCD_DisplayStringAt(50,20+(19*BCurrentCursorPosition),Buf, TRANSPARENT_MODE);	
	BSP_LCD_SelectLayer(0);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);		
	BSP_LCD_FillRect(14, (18+(19*BCurrentCursorPosition)), 466, 9);			//////////////////////////////	
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_2);
	BSP_LCD_FillRect(14, (27+(19*BCurrentCursorPosition)), 466, 5);
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_1);
	BSP_LCD_FillRect(14, (32+(19*BCurrentCursorPosition)), 466, 5);
	return;			
	}	
	

/////////////////////////////////	
//
//internal function for Browser + INFO
void int_BI_DRAW_ALL_LINES(void)
	{
	uint16_t E;
	for(E=0;E<8 && TOTAL_TRACKS>(E+BCurrentTrackPosition-1);E++)
		{
		if(E==BCurrentCursorPosition && (E+BCurrentTrackPosition == track_number | (playlist[E+BCurrentTrackPosition-1][54]%2)==1))
			{
			BSP_LCD_SetTextColor(LCD_COLOR_DGREEN);	
			}
		else if(E+BCurrentTrackPosition == track_number | (playlist[E+BCurrentTrackPosition-1][54]%2)==1)	
			{
			BSP_LCD_SetTextColor(LCD_COLOR_GREEN);	
			}
		else if(E==BCurrentCursorPosition)
			{
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
			}
		else
			{
			BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
			}
		BSP_LCD_DisplayStringAt(39,20+(19*E), playlist[E+BCurrentTrackPosition-1], INFO_MODE);		
		if(E+BCurrentTrackPosition == track_number)
			{
			sprintf((char*)Buf, "%s", ">");	
			}	
		else if((playlist[E+BCurrentTrackPosition-1][54]&0x2)==0)
			{
			sprintf((char*)Buf, "%s", "~");
			}		
		else
			{
			BSP_LCD_SetTextColor(LCD_COLOR_RED);
			sprintf((char*)Buf, "%s", "<");		
			}				
		BSP_LCD_DisplayStringAt(20,20+(19*E),Buf, INFO_MODE);		
		}	
		BSP_LCD_SetTextColor(LCD_COLOR_PAPER);								////Draw track number and status (playing or played) in INFO mode
		BSP_LCD_FillRect(301, 40, 26, 13);
		BSP_LCD_FillRect(301, 59, 54, 15);	
		BSP_LCD_SetTextColor(LCD_COLOR_DARK_2);
		sprintf((char *)Buf , "%03lu", BCurrentCursorPosition+BCurrentTrackPosition);		
		BSP_LCD_DisplayStringAt(301, 40,Buf, TRANSPARENT_MODE);	
		if(BCurrentTrackPosition+BCurrentCursorPosition == track_number)
			{
			sprintf((char*)Buf, "%s", "Playing");	
			BSP_LCD_DisplayStringAt(301, 59, Buf, TRANSPARENT_MODE);	
			}
		else if((playlist[BCurrentTrackPosition+BCurrentCursorPosition-1][54]%2)==1)
			{
			sprintf((char*)Buf, "%s", "Played");	
			BSP_LCD_DisplayStringAt(301, 59, Buf, TRANSPARENT_MODE);		
			}	
	BSP_LCD_SelectLayer(0);
	ReDrawScroll(TOTAL_TRACKS, BCurrentTrackPosition);	
	}
/////////////////////////////////	
//
//internal function for Browser + INFO
void int_BI_DRAW_ONE_LINE(uint8_t UPDOWN)							
	{
	if(BCurrentCursorPosition%2==0)
		{
		BSP_LCD_SetTextColor(LCD_COLOR_DARK_1);
		}
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_DARK_2);
		}
	BSP_LCD_FillRect(14, 18+(19*BCurrentCursorPosition), 256, 19);
	BSP_LCD_SelectLayer(1);	
	BSP_LCD_SetTransparency(1, 255);	
	BSP_LCD_SetFont(&Font15P);
	if(BCurrentTrackPosition+BCurrentCursorPosition == track_number | (playlist[BCurrentTrackPosition+BCurrentCursorPosition-1][54]%2)==1)
		{
		BSP_LCD_SetTextColor(LCD_COLOR_GREEN);	
		}	
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		}		
	BSP_LCD_DisplayStringAt(39,20+(19*BCurrentCursorPosition), playlist[BCurrentCursorPosition+BCurrentTrackPosition-1], INFO_MODE);	
	if(BCurrentTrackPosition+BCurrentCursorPosition == track_number)
		{
		sprintf((char*)Buf, "%s", ">");	
		}	
	else if((playlist[BCurrentTrackPosition+BCurrentCursorPosition-1][54]&0x2)==0)
		{
		sprintf((char*)Buf, "%s", "~");
		}		
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_RED);
		sprintf((char*)Buf, "%s", "<");		
		}
	BSP_LCD_DisplayStringAt(20,20+(19*BCurrentCursorPosition),Buf, INFO_MODE);		
	if(UPDOWN==1)
		{
		BCurrentCursorPosition++;
		}
	else
		{
		BCurrentCursorPosition--;	
		}
	if(BCurrentTrackPosition+BCurrentCursorPosition == track_number | (playlist[BCurrentTrackPosition+BCurrentCursorPosition-1][54]%2)==1)
		{			
		BSP_LCD_SetTextColor(LCD_COLOR_DGREEN);	
		}	
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		}	
	BSP_LCD_DisplayStringAt(39,20+(19*BCurrentCursorPosition), playlist[BCurrentCursorPosition+BCurrentTrackPosition-1], INFO_MODE);
	if(BCurrentTrackPosition+BCurrentCursorPosition == track_number)
		{
		sprintf((char*)Buf, "%s", ">");	
		}	
	else if((playlist[BCurrentTrackPosition+BCurrentCursorPosition-1][54]&0x2)==0)
		{
		sprintf((char*)Buf, "%s", "~");
		}		
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_RED);
		sprintf((char*)Buf, "%s", "<");		
		}
	BSP_LCD_DisplayStringAt(20,20+(19*BCurrentCursorPosition),Buf, INFO_MODE);	
	BSP_LCD_SetTextColor(LCD_COLOR_PAPER);								////Draw track number and status (playing or played) in INFO mode
	BSP_LCD_FillRect(301, 40, 26, 13);
	BSP_LCD_FillRect(301, 59, 54, 15);	
	BSP_LCD_SetTextColor(LCD_COLOR_DARK_2);
	sprintf((char *)Buf , "%03lu", BCurrentCursorPosition+BCurrentTrackPosition);		
	BSP_LCD_DisplayStringAt(301, 40,Buf, TRANSPARENT_MODE);	
	if(BCurrentTrackPosition+BCurrentCursorPosition == track_number)
		{
		sprintf((char*)Buf, "%s", "Playing");	
		BSP_LCD_DisplayStringAt(301, 59, Buf, TRANSPARENT_MODE);	
		}
	else if((playlist[BCurrentTrackPosition+BCurrentCursorPosition-1][54]%2)==1)
		{
		sprintf((char*)Buf, "%s", "Played");	
		BSP_LCD_DisplayStringAt(301, 59, Buf, TRANSPARENT_MODE);		
		}
	BSP_LCD_SelectLayer(0);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);		
	BSP_LCD_FillRect(14, (18+(19*BCurrentCursorPosition)), 242, 9);			//////////////////////////////	
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_2);
	BSP_LCD_FillRect(14, (27+(19*BCurrentCursorPosition)), 242, 5);
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_1);
	BSP_LCD_FillRect(14, (32+(19*BCurrentCursorPosition)), 242, 5);
	intDrawTriangle(BCurrentCursorPosition);			
	return;			
	}		
	
/////////////////////////////////	
//
//internal function for TAG LIST
void int_T_DRAW_ALL_LINES(void)
	{
	uint16_t E;
	for(E=0;E<8 && TOTAL_TRACKS_IN_TAG_LIST>(E+TCurrentTrackPosition-1);E++)
		{
		if(E==TCurrentCursorPosition && (TAG_LIST_BASE[E+TCurrentTrackPosition-1] == track_number | (playlist[TAG_LIST_BASE[E+TCurrentTrackPosition-1]-1][54]%2)==1))
			{
			BSP_LCD_SetTextColor(LCD_COLOR_DGREEN);	
			}
		else if(TAG_LIST_BASE[E+TCurrentTrackPosition-1] == track_number | (playlist[TAG_LIST_BASE[E+TCurrentTrackPosition-1]-1][54]%2)==1)	
			{
			BSP_LCD_SetTextColor(LCD_COLOR_GREEN);	
			}
		else if(E==TCurrentCursorPosition)
			{
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
			}
		else
			{
			BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
			}
//		sprintf((char *)Buf , "%03lu", E+TCurrentTrackPosition);					
//		BSP_LCD_DisplayStringAt(20,20+(19*E),Buf, TRANSPARENT_MODE);
		if(TAG_LIST_BASE[E+TCurrentTrackPosition-1] == track_number)
			{
			sprintf((char*)Buf, "%s", ">");	
			}
		else
			{
			sprintf((char*)Buf, "%s", "~");
			}					
		BSP_LCD_DisplayStringAt(20,20+(19*E),Buf, TRANSPARENT_MODE);	
		BSP_LCD_DisplayStringAt(39,20+(19*E), playlist[TAG_LIST_BASE[E+TCurrentTrackPosition-1]-1], TRANSPARENT_MODE);	
		}	
	BSP_LCD_SelectLayer(0);
	ReDrawScroll(TOTAL_TRACKS_IN_TAG_LIST, TCurrentTrackPosition);	
	}
/////////////////////////////////	
//
//internal function for TAG LIST
void int_T_DRAW_ONE_LINE(uint8_t UPDOWN)							
	{
	if(TCurrentCursorPosition%2==0)
		{
		BSP_LCD_SetTextColor(LCD_COLOR_DARK_1);
		}
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_DARK_2);
		}
	BSP_LCD_FillRect(14, 18+(19*TCurrentCursorPosition), 466, 19);
	BSP_LCD_SelectLayer(1);	
	BSP_LCD_SetTransparency(1, 255);	
	BSP_LCD_SetFont(&Font15P);
	if(TAG_LIST_BASE[TCurrentTrackPosition+TCurrentCursorPosition-1] == track_number | (playlist[TAG_LIST_BASE[TCurrentTrackPosition+TCurrentCursorPosition-1]-1][54]%2)==1)
		{
		BSP_LCD_SetTextColor(LCD_COLOR_GREEN);	
		}	
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		}		
//	sprintf((char *)Buf , "%03lu", TCurrentTrackPosition+TCurrentCursorPosition);					
//	BSP_LCD_DisplayStringAt(20,20+(19*TCurrentCursorPosition),Buf, TRANSPARENT_MODE);
	if(TAG_LIST_BASE[TCurrentTrackPosition+TCurrentCursorPosition-1] == track_number)
		{
		sprintf((char*)Buf, "%s", ">");	
		}	
	else
		{
		sprintf((char*)Buf, "%s", "~");
		}		
	BSP_LCD_DisplayStringAt(20,20+(19*TCurrentCursorPosition),Buf, TRANSPARENT_MODE);	
	BSP_LCD_DisplayStringAt(39,20+(19*TCurrentCursorPosition), playlist[TAG_LIST_BASE[TCurrentCursorPosition+TCurrentTrackPosition-1]-1], TRANSPARENT_MODE);		
	if(UPDOWN==TAGLIST_UP)
		{
		TCurrentCursorPosition++;
		}
	else
		{
		TCurrentCursorPosition--;	
		}
	if(TAG_LIST_BASE[TCurrentTrackPosition+TCurrentCursorPosition-1] == track_number | (playlist[TAG_LIST_BASE[TCurrentTrackPosition+TCurrentCursorPosition-1]-1][54]%2)==1)
		{
		BSP_LCD_SetTextColor(LCD_COLOR_DGREEN);	
		}	
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		}		
//	sprintf((char *)Buf , "%03lu", TCurrentTrackPosition+TCurrentCursorPosition);					
//	BSP_LCD_DisplayStringAt(20,20+(19*TCurrentCursorPosition),Buf, TRANSPARENT_MODE);	
	if(TAG_LIST_BASE[TCurrentTrackPosition+TCurrentCursorPosition-1] == track_number)
		{
		sprintf((char*)Buf, "%s", ">");	
		}	
	else
		{
		sprintf((char*)Buf, "%s", "~");
		}		
	BSP_LCD_DisplayStringAt(20,20+(19*TCurrentCursorPosition),Buf, TRANSPARENT_MODE);	
	BSP_LCD_DisplayStringAt(39,20+(19*TCurrentCursorPosition), playlist[TAG_LIST_BASE[TCurrentCursorPosition+TCurrentTrackPosition-1]-1], TRANSPARENT_MODE);
	BSP_LCD_SelectLayer(0);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);		
	BSP_LCD_FillRect(14, (18+(19*TCurrentCursorPosition)), 466, 9);			//////////////////////////////	
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_2);
	BSP_LCD_FillRect(14, (27+(19*TCurrentCursorPosition)), 466, 5);
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_1);
	BSP_LCD_FillRect(14, (32+(19*TCurrentCursorPosition)), 466, 5);
	return;			
	}	
	

/////////////////////////////////	
//
//internal function for TAG LIST + INFO
void int_TI_DRAW_ALL_LINES(void)
	{
	uint16_t E;
	for(E=0;E<8 && TOTAL_TRACKS_IN_TAG_LIST>(E+TCurrentTrackPosition-1);E++)
		{
		if(E==TCurrentCursorPosition && (TAG_LIST_BASE[E+TCurrentTrackPosition-1] == track_number | (playlist[TAG_LIST_BASE[E+TCurrentTrackPosition-1]-1][54]%2)==1))
			{
			BSP_LCD_SetTextColor(LCD_COLOR_DGREEN);	
			}
		else if(TAG_LIST_BASE[E+TCurrentTrackPosition-1] == track_number | (playlist[TAG_LIST_BASE[E+TCurrentTrackPosition-1]-1][54]%2)==1)	
			{
			BSP_LCD_SetTextColor(LCD_COLOR_GREEN);	
			}
		else if(E==TCurrentCursorPosition)
			{
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
			}
		else
			{
			BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
			}
			
		if(TAG_LIST_BASE[E+TCurrentTrackPosition-1] == track_number)
			{
			sprintf((char*)Buf, "%s", ">");	
			}
		else
			{
			sprintf((char*)Buf, "%s", "~");
			}					
		BSP_LCD_DisplayStringAt(20,20+(19*E),Buf, INFO_MODE);	
		BSP_LCD_DisplayStringAt(39,20+(19*E), playlist[TAG_LIST_BASE[E+TCurrentTrackPosition-1]-1], INFO_MODE);	
		}	
		BSP_LCD_SetTextColor(LCD_COLOR_PAPER);								////Draw track number and status (playing or played) in INFO mode
		BSP_LCD_FillRect(301, 40, 26, 13);
		BSP_LCD_FillRect(301, 59, 54, 15);	
		BSP_LCD_SetTextColor(LCD_COLOR_DARK_2);
		sprintf((char *)Buf , "%03lu", TAG_LIST_BASE[TCurrentTrackPosition+TCurrentCursorPosition-1]);		
		BSP_LCD_DisplayStringAt(301, 40,Buf, TRANSPARENT_MODE);	
		if(TAG_LIST_BASE[TCurrentTrackPosition+TCurrentCursorPosition-1] == track_number)
			{
			sprintf((char*)Buf, "%s", "Playing");	
			BSP_LCD_DisplayStringAt(301, 59, Buf, TRANSPARENT_MODE);	
			}
		else if((playlist[TAG_LIST_BASE[TCurrentTrackPosition+TCurrentCursorPosition-1]-1][54]%2)==1)
			{
			sprintf((char*)Buf, "%s", "Played");	
			BSP_LCD_DisplayStringAt(301, 59, Buf, TRANSPARENT_MODE);		
			}		
	BSP_LCD_SelectLayer(0);
	ReDrawScroll(TOTAL_TRACKS_IN_TAG_LIST, TCurrentTrackPosition);	
	}
/////////////////////////////////	
//
//internal function for TAG LIST + INFO
void int_TI_DRAW_ONE_LINE(uint8_t UPDOWN)							
	{
	if(TCurrentCursorPosition%2==0)
		{
		BSP_LCD_SetTextColor(LCD_COLOR_DARK_1);
		}
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_DARK_2);
		}
	BSP_LCD_FillRect(14, 18+(19*TCurrentCursorPosition), 256, 19);
	BSP_LCD_SelectLayer(1);	
	BSP_LCD_SetTransparency(1, 255);	
	BSP_LCD_SetFont(&Font15P);
	if(TAG_LIST_BASE[TCurrentTrackPosition+TCurrentCursorPosition-1] == track_number | (playlist[TAG_LIST_BASE[TCurrentTrackPosition+TCurrentCursorPosition-1]-1][54]%2)==1)
		{
		BSP_LCD_SetTextColor(LCD_COLOR_GREEN);	
		}	
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		}		
	if(TAG_LIST_BASE[TCurrentTrackPosition+TCurrentCursorPosition-1] == track_number)
		{
		sprintf((char*)Buf, "%s", ">");	
		}	
	else
		{
		sprintf((char*)Buf, "%s", "~");
		}		
	BSP_LCD_DisplayStringAt(20,20+(19*TCurrentCursorPosition),Buf, INFO_MODE);	
	BSP_LCD_DisplayStringAt(39,20+(19*TCurrentCursorPosition), playlist[TAG_LIST_BASE[TCurrentCursorPosition+TCurrentTrackPosition-1]-1], INFO_MODE);		
	if(UPDOWN==TAGLIST_UP)
		{
		TCurrentCursorPosition++;
		}
	else
		{
		TCurrentCursorPosition--;	
		}
	if(TAG_LIST_BASE[TCurrentTrackPosition+TCurrentCursorPosition-1] == track_number | (playlist[TAG_LIST_BASE[TCurrentTrackPosition+TCurrentCursorPosition-1]-1][54]%2)==1)
		{
		BSP_LCD_SetTextColor(LCD_COLOR_DGREEN);	
		}	
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		}		
	if(TAG_LIST_BASE[TCurrentTrackPosition+TCurrentCursorPosition-1] == track_number)
		{
		sprintf((char*)Buf, "%s", ">");	
		}	
	else
		{
		sprintf((char*)Buf, "%s", "~");
		}		
	BSP_LCD_DisplayStringAt(20,20+(19*TCurrentCursorPosition),Buf, INFO_MODE);	
	BSP_LCD_DisplayStringAt(39,20+(19*TCurrentCursorPosition), playlist[TAG_LIST_BASE[TCurrentCursorPosition+TCurrentTrackPosition-1]-1], INFO_MODE);
	BSP_LCD_SetTextColor(LCD_COLOR_PAPER);								////Draw track number and status (playing or played) in INFO mode
	BSP_LCD_FillRect(301, 40, 26, 13);
	BSP_LCD_FillRect(301, 59, 54, 15);	
	BSP_LCD_SetTextColor(LCD_COLOR_DARK_2);
	sprintf((char *)Buf , "%03lu", TAG_LIST_BASE[TCurrentTrackPosition+TCurrentCursorPosition-1]);		
	BSP_LCD_DisplayStringAt(301, 40,Buf, TRANSPARENT_MODE);	
	if(TAG_LIST_BASE[TCurrentTrackPosition+TCurrentCursorPosition-1] == track_number)
		{
		sprintf((char*)Buf, "%s", "Playing");	
		BSP_LCD_DisplayStringAt(301, 59, Buf, TRANSPARENT_MODE);	
		}
	else if((playlist[TAG_LIST_BASE[TCurrentTrackPosition+TCurrentCursorPosition-1]-1][54]%2)==1)
		{
		sprintf((char*)Buf, "%s", "Played");	
		BSP_LCD_DisplayStringAt(301, 59, Buf, TRANSPARENT_MODE);		
		}	
	BSP_LCD_SelectLayer(0);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);		
	BSP_LCD_FillRect(14, (18+(19*TCurrentCursorPosition)), 242, 9);			//////////////////////////////	
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_2);
	BSP_LCD_FillRect(14, (27+(19*TCurrentCursorPosition)), 242, 5);
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_1);
	BSP_LCD_FillRect(14, (32+(19*TCurrentCursorPosition)), 242, 5);
	intDrawTriangle(TCurrentCursorPosition);	
	return;			
	}		
	


/////////////////////////////////	
//
//internal function for UTILITY
void int_U_DRAW_ALL_LINES(void)
	{
	uint16_t E;
	for(E=0;E<8 && TOTAL_U_POSITIONS>(E+CurrentUPosition-1);E++)
		{
		if(E==UCurrentCursorPosition)
			{
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
			}
		else
			{
			BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
			}
		BSP_LCD_DisplayStringAt(35, 21+(19*E), UTILITY_BASE[E+CurrentUPosition-1], TRANSPARENT_MODE);
		UTILITY_PARAMETER(E+CurrentUPosition-1);	
		BSP_LCD_DisplayStringAt(292, 21+(19*E),Buf, TRANSPARENT_MODE);			
		}	
	BSP_LCD_SelectLayer(0);
	ReDrawScroll(TOTAL_U_POSITIONS, CurrentUPosition);	
	}
/////////////////////////////////	
//
//internal function for UTILITY
void int_U_DRAW_ONE_LINE(uint8_t UPDOWN)							
	{
	BSP_LCD_SetTextColor(0xFF0F0F0);				
	BSP_LCD_FillRect(14, 18+(19*UCurrentCursorPosition), 256, 19);
	BSP_LCD_FillRect(271, 18+(19*UCurrentCursorPosition), 208, 19);	
	BSP_LCD_SetTextColor(LCD_COLOR_DARK_1);
	BSP_LCD_DrawLine(14,18+(19*UCurrentCursorPosition),479,18+(19*UCurrentCursorPosition));
	BSP_LCD_SelectLayer(1);	
	BSP_LCD_SetTransparency(1, 255);	
	BSP_LCD_SetFont(&Font15P);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_DisplayStringAt(35, 21+(19*UCurrentCursorPosition), UTILITY_BASE[UCurrentCursorPosition+CurrentUPosition-1], TRANSPARENT_MODE);	
	UTILITY_PARAMETER(UCurrentCursorPosition+CurrentUPosition-1);	
	BSP_LCD_DisplayStringAt(292, 21+(19*UCurrentCursorPosition),Buf, TRANSPARENT_MODE);		

		
	if(UPDOWN==UTILITY_UP)
		{
		UCurrentCursorPosition++;
		}
	else
		{
		UCurrentCursorPosition--;	
		}
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_DisplayStringAt(35,21+(19*UCurrentCursorPosition), UTILITY_BASE[UCurrentCursorPosition+CurrentUPosition-1], TRANSPARENT_MODE);
	UTILITY_PARAMETER(UCurrentCursorPosition+CurrentUPosition-1);	
	BSP_LCD_DisplayStringAt(292, 21+(19*UCurrentCursorPosition),Buf, TRANSPARENT_MODE);				
	BSP_LCD_SelectLayer(0);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);		
	BSP_LCD_FillRect(14, (18+(19*UCurrentCursorPosition)), 256, 9);			////Draw selected cursor	
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_2);
	BSP_LCD_FillRect(14, (27+(19*UCurrentCursorPosition)), 256, 5);
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_1);
	BSP_LCD_FillRect(14, (32+(19*UCurrentCursorPosition)), 256, 5);
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_3);		
	BSP_LCD_FillRect(271, (18+(19*UCurrentCursorPosition)), 208, 9);			////Draw selected cursor	
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_5);
	BSP_LCD_FillRect(271, (27+(19*UCurrentCursorPosition)), 208, 5);
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_4);
	BSP_LCD_FillRect(271, (32+(19*UCurrentCursorPosition)), 208, 5);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	FillTriangle(20, 20, 27, 20+(19*UCurrentCursorPosition), 34+(19*UCurrentCursorPosition), 27 +(19*UCurrentCursorPosition));		
	return;			
	}	
	


//////////////////////////////////////////
//
//
//
//
void UTILITY_PARAMETER(uint8_t num_parameter)
	{
	switch (num_parameter)
		{
		case 0:
			{
			sprintf((char*)Buf, "%s", "SINGLE");	
			break;	
			}
		case 1:
			{
			sprintf((char*)Buf, "%s", "UNLOCK");	
			break;	
			}	
		case 2:
			{
			sprintf((char*)Buf, "%s", "-60dB");	
			break;	
			}		
		case 3:
			{
			sprintf((char*)Buf, "%s", "OFF");	
			break;	
			}
		case 4:
			{
			sprintf((char*)Buf, "%s", "1");	
			break;	
			}	
		case 5:
			{
			sprintf((char*)Buf, "%s", "OFF");	
			break;	
			}		
		case 6:
			{
			sprintf((char*)Buf, "%s", "5");	
			break;	
			}	
		case 7:
			{
			sprintf((char*)Buf, "%s", "OFF");	
			break;	
			}	
		case 8:
			{
			sprintf((char *)Buf , "%01lu", output_level);	
			break;	
			}		
		case 9:
			{
			sprintf((char *)Buf , "%09lu", DEVICE_UID);	
			break;	
			}		
		case 10:
			{
			sprintf((char*)Buf, "%s", "Ver. ");
			Buf[5] = FIRMWARE_VERSION[0];
			Buf[6] = FIRMWARE_VERSION[1];
			Buf[7] = FIRMWARE_VERSION[2];
			Buf[8] = FIRMWARE_VERSION[3];	
			Buf[9] = FIRMWARE_VERSION[4];					
			Buf[10] = 0;					
			break;	
			}		
		default:
			break;	
		}	
	return;	
	}

	
	
///////////////////
//Encoder interrupt
//
//		
void EXTI15_10_IRQHandler(void)
	{
	if(HAL_GPIO_ReadPin(GPIOA, ENC_A_Pin))
		{	
		need_up = 1;	
		}
	else
		{
		need_down = 1;	
		}
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
	}

///////////////////
//Timer 1000 Hz
//
//		
void TIM1_UP_TIM10_IRQHandler(void)
{
	if(change_speed==NEED_UP)
		{
		if(pitch<potenciometer_tempo-acceleration_UP)
			{
			pitch+=acceleration_UP;	
			}
		else
			{
			change_speed = NO_CHANGE;
			pitch = potenciometer_tempo;	
			}	
		}
	else if(change_speed==NEED_DOWN)
		{
		if(pitch>acceleration_DOWN)
			{
			pitch-=acceleration_DOWN;	
			}
		else
			{
			change_speed = NO_CHANGE;
			pitch = 0;	
			}	
		}
		
	if(timer_time<124)
		{
		timer_time++;	
		}
	else
		{
		timer_time = 0;
		if(ENCODER_LED_BLINK<8)
			{
			load_animation_enable = 1;	
			if(ENCODER_LED_BLINK%2==0)
				{
				HAL_GPIO_WritePin(GPIOB, LED_ENCODER_Pin, GPIO_PIN_RESET);	
				}
			else
				{
				HAL_GPIO_WritePin(GPIOB, LED_ENCODER_Pin, GPIO_PIN_SET);
				}
			ENCODER_LED_BLINK++;	
			}
		else
			{
			load_animation_enable = 0;	
			}
		if(LED_SD_timer<7)
			{
			LED_SD_timer++;
			if(LED_SD_timer==4)
				{
				TIM_PLAY_LED = 1;
				TIM_CUE_LED = 0;		
				HAL_GPIO_WritePin(LED_SD_GPIO_Port, LED_SD_Pin, GPIO_PIN_SET);	
				}
			else if(LED_SD_timer==2 || LED_SD_timer==6)
				{
				TIM_CUE_LED = 1;					
				}
			else if((LED_SD_timer==1 || LED_SD_timer==5) && track_number!=0)
				{
				if((all_long-(play_adr/294))<4500)
					{
					if(DRAWN_IN_REMAIN==1)
						{
						need_DSW = 1;	
						}
					else
						{
						need_DSW = 2;	
						}	
					}
				else	
					{
					if((DRAWN_IN_REMAIN==0) && (REMAIN_ENABLE==1))
						{
						need_DSW = 2;	
						}
					else if((DRAWN_IN_REMAIN==1) && (REMAIN_ENABLE==0))
						{
						need_DSW = 1;	
						}
					}		
				}	
			else if((LED_SD_timer==3 || LED_SD_timer==7) && (track_number!=0) && (all_long-(play_adr/294))<2250)
				{
				if(DRAWN_IN_REMAIN==1)
					{
					need_DSW = 1;	
					}
				else
					{
					need_DSW = 2;	
					}		
				}		
			}
		else
			{
			if(track_number==0)
				{
				HAL_GPIO_WritePin(LED_SD_GPIO_Port, LED_SD_Pin, GPIO_PIN_SET);
				TIM_PLAY_LED = 0;	
				}
			else
				{
				TIM_PLAY_LED = 0;	
				TIM_CUE_LED = 0;	
				HAL_GPIO_WritePin(LED_SD_GPIO_Port, LED_SD_Pin, GPIO_PIN_RESET);	
				}
			LED_SD_timer = 0;	
			}
		if(HAL_GPIO_ReadPin(KEY_MENU_GPIO_Port, KEY_MENU_Pin)==0  &&	dSHOW != UTILITY && countUTILITY<10)		///counter for long press MENU button
			{
			countUTILITY++;	
			}
		else
			{
			countUTILITY = 0;	
			}
		}
	
  HAL_TIM_IRQHandler(&htim1);
}
////////////////////////////////
//TOUCH_SCREEN_HANDLER
//#define TS_NEEDLE_X_MIN		40
//#define TS_NEEDLE_X_MAX		439
//#define TS_NEEDLE_Y_MIN		235
//#define TS_NEEDLE_Y_MAX		271
void TOUCH_SCREEN_HANDLER(void)
	{
	uint16_t x=0, y=0;
	TS_GetState(&TS_State);
		
	if(TS_State.touchDetected)
		{
		x = TS_State.touchX[0];
		y = TS_State.touchY[0];

		if(tscnt[0]==1)						//Holding touch			
			{
			if(needle_enable)
				{
				if(y>=TS_NEEDLE_Y_MIN && y<=TS_NEEDLE_Y_MAX && x>=TS_NEEDLE_X_MIN && x<=TS_NEEDLE_X_MAX)
					{
					if(previous_needle_position != x)
						{
						previous_needle_position = x;
						SEEK_AUDIOFRAME(294*(((x-40)*all_long)/399));
						}	
					}
				else
					{
					ShowNEEDLE(0);	
					needle_enable = 0;
					forcibly_redraw = 1;	
					}
				}
			}
		else											//First touch
			{
			if(y>=TS_NEEDLE_Y_MIN && y<=TS_NEEDLE_Y_MAX && x>=(previous_position_bar+27) && x<=(previous_position_bar+53))	
				{
				ShowNEEDLE(1);	
				needle_enable = 1;
				forcibly_redraw = 1;	
				}
			tscnt[0] = 1;	
			}

		if (TS_State.touchDetected == 2)
			{
			x = TS_State.touchX[1];
			y = TS_State.touchY[1];
			if(tscnt[1]==1)						//Holding touch
				{

				}
			else											//First touch
				{
				tscnt[0] = 1;	
				}
			}
		}
	else
		{		
		tscnt[0]=0;
		tscnt[1]=0;
		if(needle_enable)
			{
			ShowNEEDLE(0);			
			needle_enable = 0;
			forcibly_redraw = 1;	
			}	
		}
	}
	
/////////////////////////////////////////////////////
//SPI transfer DMA handler 
//	
void DMA2_Stream5_IRQHandler(void)
{	
	HAL_DMA_IRQHandler(&hdma_spi1_tx);
	
	uint32_t ptch;
	uint8_t acc_t;
		
	if((Rbuffer[14]&0x1) && PLAY_BUTTON_pressed==0)										///////////PLAY button
		{
		if(CUE_BUTTON_pressed==0)
			{
			if(play_enable)
				{
				play_enable = 0;		
				change_speed = NEED_DOWN;
				}
			else
				{
				if(Tbuffer[19]&0x8)					//SLIP MODE ENABLE
					{	
					play_adr = slip_play_adr;	
					change_speed = NO_CHANGE;
					}
				else if(CUE_ADR!=(play_adr/294))			//when playback starts from any adress
					{
					change_speed = NEED_UP;
					}
				else																//when playback starts from CUE adress
					{
					change_speed = NO_CHANGE;	
					}
				play_enable = 1;	
				}
			}
		else
			{
			keep_to_play = 1;	
			}
		PLAY_BUTTON_pressed = 1;	
		}
	else if((Rbuffer[14]&0x1)==0 && PLAY_BUTTON_pressed==1)
		{
		PLAY_BUTTON_pressed = 0;	
		}
	else if((Rbuffer[14]&0x2) && CUE_BUTTON_pressed==0)										///////////CUE button
		{
		if(play_enable & ((Rbuffer[12]&0x20)==0))								//return to CUE, when track playing
			{
			pitch = 0;	
			play_enable = 0;	
			CUE_OPERATION = CUE_NEED_CALL;			
			}	
		else if((play_enable==0) && (CUE_ADR!=(play_adr/294)))			//Set new CUE, when track stopped		
			{
			CUE_OPERATION = CUE_NEED_SET;		
			}
		else if((play_enable==0)&&(CUE_ADR==(play_adr/294)))				//return to CUE adress, when track stopped
			{
			play_adr = 294*CUE_ADR;		
			if(Tbuffer[19]&0x8)					//SLIP MODE ENABLE
				{	
				slip_play_adr = play_adr;	
				}	
			play_enable = 1;	
			}
		else if(play_enable & ((Rbuffer[12]&0x20)!=0) && (Tbuffer[19]&0x20))				//Set new CUE, when track played and press jog and JOG in Vinyl MODE
			{
			CUE_OPERATION = CUE_NEED_SET;	
			play_enable = 0;		
			}
		CUE_BUTTON_pressed = 1;	
		}
	else if((Rbuffer[14]&0x2)==0 && CUE_BUTTON_pressed==1)
		{
		if(keep_to_play==0)		//button play not pressed	
			{
			play_enable = 0;	
			play_adr = 294*CUE_ADR;		
			if(Tbuffer[19]&0x8)					//SLIP MODE ENABLE
				{	
				slip_play_adr = play_adr;	
				}
			if((Tbuffer[19]&0x20)==0)   //CDJ mode
				{
				pitch = 0;	
				}
			}
		else
			{
			keep_to_play = 0;	
			}
		offset_adress = 0;																///	   temporary operation		
		CUE_BUTTON_pressed = 0;	
		}
		
		
		
		
	if((Rbuffer[12]&0x2)==0 && REVERSE_SWITCH_pressed==0)					///////////reverse switch position
		{
		Tbuffer[17] |= 0x20;					//enable red led reverse
		REVERSE_SWITCH_pressed = 1;	
		}
	else if((Rbuffer[12]&0x2)!=0 && REVERSE_SWITCH_pressed==1)
		{
		if(Tbuffer[19]&0x8)					//SLIP MODE ENABLE
			{	
			play_adr = slip_play_adr;	
			}		
		Tbuffer[17] &= 0xDF;					//disable red led reverse
		REVERSE_SWITCH_pressed = 0;		
		}	
		
		///////////////////////////////////////////////////JOG MECHANICAL PROCESS///////////////////////////////////////////////
		
	if(inertial_rotation)
		{
		if(((Tbuffer[17]&0x20)==0 && (((Rbuffer[12]&0xC0)==0) || ((Rbuffer[12]&0x40) && pitch<potenciometer_tempo))) || 
			(Tbuffer[17]&0x20 && (((Rbuffer[12]&0xC0)==0x40) || ((Rbuffer[12]&0x40)==0 && pitch<potenciometer_tempo))))		//if rotation foward and stopped
			{
			inertial_rotation = 0;	
			}
		}	

	if(((Rbuffer[12]&0x20)!=0 || play_enable==0 || inertial_rotation==1) && (Tbuffer[19]&0x20))				/////////////touch enable!
		{
		if(Rbuffer[12]&0x80)					//rotation detect
			{
			ptch = (256*Rbuffer[10]+Rbuffer[11]);
			if(ptch<86)
				{
				ptch = 86;	
				}
			ptch = 5574324/ptch;
			pitch = ptch;
			pitch_for_slip = potenciometer_tempo; 	
			inertial_rotation = 1;		
			}
		else if(change_speed==NO_CHANGE)
			{
			pitch = 0;	
			}
		
		if(change_speed==NO_CHANGE)
			{	
			if(Rbuffer[12]&0x40)				//foward/reverse rotation
				{
				reverse = 0;	
				}
			else
				{
				reverse = 1;	
				}		
			}
		else
			{
			if(Tbuffer[17]&0x20)					//reverse diode enable
				{
				reverse = 1;
				}
			else
				{
				reverse = 0;	
				}	
			}
		Tbuffer[23] |= 0x20;				//touch enable circle on display		
		}
	else if((Rbuffer[12]&0xA0)==0)				//touch disable and rotation disable
		{
		pitch_for_slip = potenciometer_tempo;	
		if(play_enable)
			{				
			if(change_speed==NO_CHANGE)
				{
				pitch = potenciometer_tempo;	
				}	
			if(Tbuffer[17]&0x20)					//reverse diode enable
				{
				reverse = 1;
				}
			else
				{
				reverse = 0;	
				}
			if(Tbuffer[23]&0x20)					//jog UNPRESSED
				{
				if(Tbuffer[19]&0x8)					//SLIP MODE ENABLE
					{	
					play_adr = slip_play_adr;	
					}	
				Tbuffer[23] &= 0xDF;				//disable touch circle on display
				}
			}
		}
	else if	((Rbuffer[12]&0x80) && play_enable)	//rotation detected			(pitch bend)	
		{
			
			
			
		//ptch = (418728/(256*Rbuffer[10]+Rbuffer[11]));	
		
		ptch = (256*Rbuffer[10]+Rbuffer[11]);
		if(ptch>139)
			{
			ptch = ptch-139;	
			}
		else
			{
			ptch = 1;	
			}
		ptch = 150000/ptch;
			
			
		if(ptch>4225)
			{
			ptch = 4225;	
			}	
		if((Rbuffer[12]&0x40 && (Tbuffer[17]&0x20)==0) || ((Rbuffer[12]&0x40)==0 && Tbuffer[17]&0x20))		//foward rotation and reverse off OR reverse rotation and reverse on (pitch bend)			
			{
			ptch+= potenciometer_tempo;
			if(ptch>20000)
				{
				ptch = 20000;	
				}
			pitch = ptch;	
			}
		else if(((Rbuffer[12]&0x40)==0 && (Tbuffer[17]&0x20)==0) || (Rbuffer[12]&0x40 && Tbuffer[17]&0x20))	 //reverse rotation and reverse off OR foward rotation and reverse on(pitch bend)	
			{
			if(ptch<potenciometer_tempo)
				{
				pitch = potenciometer_tempo - ptch;
				}
			else
				{
				pitch = 0;	
				}
			}
		if(Tbuffer[17]&0x20)					//reverse diode enable
			{
			reverse = 1;
			}
		else
			{
			reverse = 0;	
			}	
		if(Tbuffer[23]&0x20)					//jog UNPRESSED
			{
			if(Tbuffer[19]&0x8)					//SLIP MODE ENABLE
				{	
				play_adr = slip_play_adr;	
				}	
			Tbuffer[23] &= 0xDF;				//disable touch circle on display	
			}	
		else
			{
			pitch_for_slip = pitch;		
			}
		}
		
	if(dma_cnt<5)									////////////////////////////////////////cicle divider////////////////////////////
		{
		dma_cnt++;	
		pitch_center+= (Rbuffer[7]+256*Rbuffer[6]);
		DD+= (Rbuffer[5]+256*Rbuffer[4]);			
		if(dma_cnt==1)
			{
			acc_t = Rbuffer[2]>>1;
			if(acc_t<4)
				{
				acceleration_DOWN = LOG_TABLE[0];	
				}
			else if(acc_t<12)
				{
				acceleration_DOWN = ((LOG_TABLE[1]*(acc_t-3)+LOG_TABLE[0]*(11-acc_t))>>3);	
				}
			else if(acc_t<28)
				{
				acceleration_DOWN = ((LOG_TABLE[2]*(acc_t-11)+LOG_TABLE[1]*(27-acc_t))>>4);	
				}
			else if(acc_t<44)
				{
				acceleration_DOWN = ((LOG_TABLE[3]*(acc_t-27)+LOG_TABLE[2]*(43-acc_t))>>4);	
				}
			else if(acc_t<76)
				{
				acceleration_DOWN = ((LOG_TABLE[4]*(acc_t-43)+LOG_TABLE[3]*(75-acc_t))>>5);	
				}
			else if(acc_t<92)
				{
				acceleration_DOWN = ((LOG_TABLE[5]*(acc_t-75)+LOG_TABLE[4]*(91-acc_t))>>4);	
				}
			else if(acc_t<108)
				{
				acceleration_DOWN = ((LOG_TABLE[6]*(acc_t-91)+LOG_TABLE[5]*(107-acc_t))>>4);	
				}	
			else if(acc_t<124)
				{
				acceleration_DOWN = ((LOG_TABLE[7]*(acc_t-107)+LOG_TABLE[6]*(123-acc_t))>>4);	
				}			
			else
				{
				acceleration_DOWN = 2;	
				}
			}
		else if(dma_cnt==2)
			{
			if((Rbuffer[16]&0x4) && TRACK_NEXT_BUTTON_pressed==0) 							///////////TRACK NEXT Button
				{
				track_need_load = 1;
				TRACK_NEXT_BUTTON_pressed = 1;	
				}
			else if((Rbuffer[16]&0x4)==0 && TRACK_NEXT_BUTTON_pressed==1)	
				{
				TRACK_NEXT_BUTTON_pressed = 0;	
				}	
			else if((Rbuffer[16]&0x2) && TRACK_PREVIOUS_BUTTON_pressed==0) 			///////////TRACK PREVIOUS Button
				{
				track_need_load = 2;
				TRACK_PREVIOUS_BUTTON_pressed = 1;	
				}
			else if((Rbuffer[16]&0x2)==0 && TRACK_PREVIOUS_BUTTON_pressed==1)	
				{
				TRACK_PREVIOUS_BUTTON_pressed = 0;	
				}	
			else if((Rbuffer[18]&0x20) && TIME_MODE_BUTTON_pressed==0) 					///////////TIME MODE Button
				{	
				TIME_MODE_BUTTON_pressed = 1;	
				}
			else if((Rbuffer[18]&0x20)==0 && TIME_MODE_BUTTON_pressed==1)	
				{
				if(REMAIN_ENABLE)
					{
					REMAIN_ENABLE = 0;	
					}
				else
					{
					REMAIN_ENABLE = 1;	
					}
				time_mode_need_update = 1;		
				TIME_MODE_BUTTON_pressed = 0;	
				}			
			}
		else if(dma_cnt==3)
			{
			acc_t = Rbuffer[3]>>1;
			if(acc_t<4)
					{
					acceleration_UP = LOG_TABLE[0];	
					}
				else if(acc_t<12)
					{
					acceleration_UP = ((LOG_TABLE[1]*(acc_t-3)+LOG_TABLE[0]*(11-acc_t))>>3);	
					}
				else if(acc_t<28)
					{
					acceleration_UP = ((LOG_TABLE[2]*(acc_t-11)+LOG_TABLE[1]*(27-acc_t))>>4);	
					}
				else if(acc_t<44)
					{
					acceleration_UP = ((LOG_TABLE[3]*(acc_t-27)+LOG_TABLE[2]*(43-acc_t))>>4);	
					}
				else if(acc_t<76)
					{
					acceleration_UP = ((LOG_TABLE[4]*(acc_t-43)+LOG_TABLE[3]*(75-acc_t))>>5);	
					}
				else if(acc_t<92)
					{
					acceleration_UP = ((LOG_TABLE[5]*(acc_t-75)+LOG_TABLE[4]*(91-acc_t))>>4);	
					}
				else if(acc_t<108)
					{
					acceleration_UP = ((LOG_TABLE[6]*(acc_t-91)+LOG_TABLE[5]*(107-acc_t))>>4);	
					}	
				else if(acc_t<124)
					{
					acceleration_UP = ((LOG_TABLE[7]*(acc_t-107)+LOG_TABLE[6]*(123-acc_t))>>4);	
					}			
				else
					{
					acceleration_UP = 2;	
					}
			}	
		else if(dma_cnt==4)
			{
			if((Rbuffer[16]&0x10) && SEARCH_FF_BUTTON_pressed==0) 							///////////SEARCH FF>> Button
				{
				if(play_enable & play_adr<(all_long+100000))	
					{
					//SEEK_AUDIOFRAME(play_adr+100000);	
					}
				else if(play_enable==0 & play_adr/294<(all_long+1))
					{	
					play_adr+=294;
					}
					SEARCH_FF_BUTTON_pressed = 1;	
				}
			else if((Rbuffer[16]&0x10)==0 && SEARCH_FF_BUTTON_pressed==1)	
				{
				SEARCH_FF_BUTTON_pressed = 0;	
				}	
				
			if((Rbuffer[16]&0x8) && SEARCH_REW_BUTTON_pressed==0) 							///////////SEARCH <<REW Button
				{
				if(play_enable & play_adr>100000)	
					{
					//SEEK_AUDIOFRAME(play_adr-100000);
					}
				else if(play_enable==0 & play_adr>294)
					{	
					play_adr-=294;
					}
				SEARCH_REW_BUTTON_pressed = 1;	
				}
			else if((Rbuffer[16]&0x8)==0 && SEARCH_REW_BUTTON_pressed==1)	
				{
				SEARCH_REW_BUTTON_pressed = 0;	
				}
			}		
		else if(dma_cnt==5)
			{
			if((Rbuffer[17]&0x4) && JOG_MODE_BUTTON_pressed==0) 							///////////Jog Mode button
				{
				if(Tbuffer[19]&0x20)			//VINYL => CDJ
					{
					Tbuffer[19] &= 0xDF;	
					Tbuffer[19] |= 0x40;
					Tbuffer[23] &= 0xE7;
					}	
				else												//CDJ => VINYL
					{
					Tbuffer[19] &= 0xBF;	
					Tbuffer[19] |= 0x20;
					Tbuffer[23] |= 0x18;	
					}
				JOG_MODE_BUTTON_pressed = 1;	
				}
			else if((Rbuffer[17]&0x4)==0 && JOG_MODE_BUTTON_pressed==1)	
				{
				JOG_MODE_BUTTON_pressed = 0;	
				}
				
			if((Rbuffer[17]&0x20) && TEMPO_RESET_BUTTON_pressed==0) 							///////////TEMPO RESET Button
				{
				if(Tbuffer[19]&0x10)				//ON_RESET => OFF_RESET
					{
					Tbuffer[19] &= 0xEF;
					}	
				else												//OFF_RESET => ON_RESET
					{
					Tbuffer[19] |= 0x10;
					}
				TEMPO_RESET_BUTTON_pressed = 1;	
				}
			else if((Rbuffer[17]&0x20)==0 && TEMPO_RESET_BUTTON_pressed==1)	
				{
				TEMPO_RESET_BUTTON_pressed = 0;	
				}	
			else if((Rbuffer[17]&0x8) && TEMPO_RANGE_BUTTON_pressed==0) 							///////////TEMPO RANGE Button
				{
				if(tempo_range<3)
					{
					tempo_range++;
					}
				else
					{
					tempo_range = 0;	
					}
				tempo_range_need_update = 1;	
				TEMPO_RANGE_BUTTON_pressed = 1;	
				}
			else if((Rbuffer[17]&0x8)==0 && TEMPO_RANGE_BUTTON_pressed==1)	
				{
				TEMPO_RANGE_BUTTON_pressed = 0;	
				}	
			else if((Rbuffer[18]&0x1) && CALL_NEXT_BUTTON_pressed==0) 							///////////CALL NEXT Button	>
				{
				CUE_OPERATION = MEMORY_NEED_NEXT_SET;
				CALL_NEXT_BUTTON_pressed = 1;	
				}
			else if((Rbuffer[18]&0x1)==0 && CALL_NEXT_BUTTON_pressed==1)	
				{
				CALL_NEXT_BUTTON_pressed = 0;	
				}
			else if((Rbuffer[18]&0x2) && CALL_PREVIOUS_BUTTON_pressed==0) 							///////////CALL PREVIOUS Button <
				{
				CUE_OPERATION = MEMORY_NEED_PREVIOUS_SET;
				CALL_PREVIOUS_BUTTON_pressed = 1;	
				}
			else if((Rbuffer[18]&0x2)==0 && CALL_PREVIOUS_BUTTON_pressed==1)	
				{
				CALL_PREVIOUS_BUTTON_pressed = 0;	
				}
				
			if((Rbuffer[17]&0x10) && SLIP_MODE_BUTTON_pressed==0) 							///////////SLIP MODE Button
				{
				if(Tbuffer[19]&0x8)					//ON_SLIP_MODE => OFF_SLIP_MODE
					{
					Tbuffer[19] &= 0xF7;	
					}	
				else												//OFF_SLIP_MODE => ON_SLIP_MODE
					{	
					Tbuffer[19] |= 0x8;	
					slip_play_adr = play_adr; 	
					}
				SLIP_MODE_BUTTON_pressed = 1;	
				}
			else if((Rbuffer[17]&0x10)==0 && SLIP_MODE_BUTTON_pressed==1)	
				{
				SLIP_MODE_BUTTON_pressed = 0;	
				}		
			}			
		}
	else
		{
		dma_cnt = 0;
		if(Tbuffer[19]&0x10)				//TEMPRO RESET ON
			{
			potenciometer_tempo = 10000;	
			}
		else				//////////////////////////////////////////TEMPO CALCULATION
			{		
			pitch_center = pitch_center/5;
			DD = DD/5;
			if(((pitch_center+128)>DD) && ((pitch_center-128)<DD))	
				{
				potenciometer_tempo = 10000;	
				}
			else if((pitch_center+128)<=DD)					/////pitch>0%
				{
				if(tempo_range==0)										//	6%
					{
					potenciometer_tempo = 2*((DD - pitch_center - 128)/108);
					if(potenciometer_tempo>600)
						{
						potenciometer_tempo = 600;	
						}	
					}	
				else if(tempo_range==1)										//	10%	
					{
					potenciometer_tempo = 5*((DD - pitch_center - 128)/162);
					if(potenciometer_tempo>1000)
						{
						potenciometer_tempo = 1000;	
						}
					}	
				else if(tempo_range==2)										//	16%	
					{
					potenciometer_tempo = 5*((DD - pitch_center - 128)/101);
					if(potenciometer_tempo>1600)
						{
						potenciometer_tempo = 1600;	
						}
					}	
				else																		//	WIDE	
					{
					potenciometer_tempo = 50*((DD - pitch_center - 128)/162);
					if(potenciometer_tempo>10000)
						{
						potenciometer_tempo = 10000;	
						}
					}	
				potenciometer_tempo = 10000 + potenciometer_tempo;	
				}
			else if((pitch_center-128)>=DD)				/////pitch<0%
				{
				if(tempo_range==0)										//	6%	
					{	
					potenciometer_tempo = 2*((pitch_center - 128 - DD)/108);
					if(potenciometer_tempo>600)
						{
						potenciometer_tempo = 600;	
						}
					}	
				else if(tempo_range==1)										//	10%	
					{	
					potenciometer_tempo = 5*((pitch_center - 128 - DD)/162);
					if(potenciometer_tempo>1000)
						{
						potenciometer_tempo = 1000;	
						}
					}
				else if(tempo_range==2)										//	16%	
					{	
					potenciometer_tempo = 5*((pitch_center - 128 - DD)/101);
					if(potenciometer_tempo>1600)
						{
						potenciometer_tempo = 1600;	
						}
					}	
				else																			//	WIDE
					{	
					potenciometer_tempo = 50*((pitch_center - 128 - DD)/162);
					if(potenciometer_tempo>10000)
						{
						potenciometer_tempo = 10000;	
						}
					}
				potenciometer_tempo = 10000 - potenciometer_tempo;	
				}				
			}
		if(previous_potenciometer_tempo != potenciometer_tempo)
			{
			previous_potenciometer_tempo = potenciometer_tempo;	
			tempo_need_update = 1;
			}
		pitch_center = 0;
		DD = 0;	
		}
	
	if(track_number==0)
		{
		Tbuffer[17] &= 0xFC;			//PLAY & CUE leds off
		Tbuffer[23] &= 0xDF;				//disable touch circle on display	
		}
	else
		{
		Tbuffer[17] |= 0x2;				//CUE led on
		if(play_enable)
			{
			Tbuffer[17] |= 0x3;				//PLAY and CUE led on	
			}
		else											//Play led blink
			{	
			if(TIM_PLAY_LED)	
				{
				Tbuffer[17] |= 0x1;	
				}
			else
				{
				Tbuffer[17] &= 0xFE;	
				}
			if(TIM_CUE_LED)	
				{
				Tbuffer[17] |= 0x2;	
				}
			else if(CUE_ADR!=play_adr/294)  
				{
				Tbuffer[17] &= 0xFD;	
				}
			}
		}
				
	if(load_animation_enable)
		{
		Tbuffer[25] = 137;		
		}
	else if(track_number==0)
		{
		Tbuffer[25] = 0;
		Tbuffer[21] = 0;	
		}
	else
		{
		if(Tbuffer[19]&0x8)					//SLIP MODE ENABLE
			{
			if(TMPSLP)
				{
				Tbuffer[21] = RED_CIRCLE_CUE_ADR;		
				TMPSLP = 0;	
				}
			else
				{
				zi = ((slip_play_adr/588)%135)+1;	
				Tbuffer[21] = (1000*zi/1589)+1;	
				TMPSLP = 1;	
				}
			}		
		else
			{
			Tbuffer[21] = RED_CIRCLE_CUE_ADR;		
			}
		zi = ((play_adr/588)%135)+1;		
		Tbuffer[25] = zi;
		}
	CheckTXCRC();
}	
	
	
	
/////////////////////////////////////////////	
//seek adress in samples (44100 per second)
//
void SEEK_AUDIOFRAME(uint32_t seek_adr)
	{
	if(seek_adr>(294*all_long))
		{
		return;	
		}
	seek_adr &= 0xFFFFE000;	

	if(FR_OK==f_lseek(&file, ((seek_adr<<2)+44)))
		{
		end_adr_valid_data = (seek_adr>>13);
		start_adr_valid_data = end_adr_valid_data; 	
		play_adr = seek_adr;	
		if(Tbuffer[19]&0x8)					//SLIP MODE ENABLE
			{	
			slip_play_adr = play_adr;	
			}			
		}
	return;	
	}

	
	
/////////////////////////////////////////////	
//SET CUE,create adress CUE_ADR
//create offset data
//copy audiodata from main audio buffer to cue_mem buffer
//
//	nf_adr - new frame adress in 1/150s
void SET_CUE(uint32_t nf_adr)
	{
	uint16_t c_adr;	
	uint32_t copy_cnt = 0;				//internal counter
	uint32_t AIS = 0;							//adress in samples 44k for CUE	
		
	CUE_ADR = nf_adr;	
	AIS = (294*CUE_ADR)&0xFFFFE000;							//rounding up to 8192
	AIS-=81920;
	mem_offset_adress = (AIS&0xFFFFF)>>13;
		
	for(copy_cnt=0;copy_cnt<327680;copy_cnt++)
		{
		PCM[(copy_cnt>>14)+128][(copy_cnt>>1)&0x1FFF][copy_cnt&0x01] = PCM[((copy_cnt>>14)+mem_offset_adress)&0x7F][(copy_cnt>>1)&0x1FFF][copy_cnt&0x01];	
		}
	DrawCueMarker(1+(400*CUE_ADR/all_long));	
	c_adr = ((nf_adr/2)%135)+1;	
	RED_CIRCLE_CUE_ADR = (1000*c_adr/1589)+1;		
	return;	
	}

/////////////////////////////////////////////	
//SET CUE to MEMORY adress, create adress CUE_ADR
//create offset data
//copy audiodata from main audio buffer to cue_mem buffer
//
//	nf_adr - new frame adress in 1/150s	
void SET_MEMORY_CUE_1(uint32_t nf_adr)
	{
	uint16_t c_adr;	
	uint32_t AIS = 0;							//adress in samples 44k for CUE	
	pitch = 0;	
	play_enable = 0;
	if(Tbuffer[19]&0x8)					//OFF_SLIP_MODE
		{
		Tbuffer[19] &= 0xF7;	
		}	
		
	CUE_ADR = nf_adr;	
	AIS = (294*CUE_ADR)&0xFFFFE000;							//rounding up to 8192
	if(FR_OK==f_lseek(&file, ((AIS<<2)+44)))
		{
		end_adr_valid_data = (AIS>>13);
		start_adr_valid_data = end_adr_valid_data; 	
		play_adr = 294*CUE_ADR;		
		}

	AIS-=81920;
	mem_offset_adress = (AIS&0xFFFFF)>>13;	
	DrawCueMarker(1+(400*CUE_ADR/all_long));	
	c_adr = ((nf_adr/2)%135)+1;	
	RED_CIRCLE_CUE_ADR = (1000*c_adr/1589)+1;		
	return;		
	}
	
/////////////////////////////////////////////	
//SET CUE to MEMORY adress second part operation
//Fill CUE buffer
//
//
void SET_MEMORY_CUE_2(void)
	{
	uint32_t copy_cnt = 0;				//internal counter
	for(copy_cnt=0;copy_cnt<327680;copy_cnt++)
		{
		PCM[(copy_cnt>>14)+128][(copy_cnt>>1)&0x1FFF][copy_cnt&0x01] = PCM[((copy_cnt>>14)+mem_offset_adress)&0x7F][(copy_cnt>>1)&0x1FFF][copy_cnt&0x01];	
		}	
	offset_adress = 128-mem_offset_adress;	
	return;	
	}
	
	
/////////////////////////////////////////////	
//CAL CUE,seek audio frame
//
//
//	
void CALL_CUE(void)
	{
	uint32_t seek_adr = 294*CUE_ADR;
	seek_adr &= 0xFFFFE000;	
	if(FR_OK==f_lseek(&file, ((seek_adr<<2)+44)))
		{
		end_adr_valid_data = (seek_adr>>13);
		start_adr_valid_data = end_adr_valid_data; 	
		play_adr = 294*CUE_ADR;		
		if(Tbuffer[19]&0x8)					//SLIP MODE ENABLE
			{	
			slip_play_adr = play_adr;	
			}			
		}
	offset_adress = 128-mem_offset_adress;	
	}
	
	
//////////////////////////////////////
//Function Checksum for TX package	
//
void CheckTXCRC()
	{
	uint8_t sdata = 141;
	uint8_t bt = 17;
	while(bt<26)
		{
		sdata+=Tbuffer[bt];	
		bt++;	
		}
	Tbuffer[26] = sdata;
	return;	
	}


		
	
//////////////////////////////////////////////////FatFs Functions//////////////////////
WCHAR ff_convert (WCHAR wch, UINT dir) 
{ 
          if (wch < 0x80) { 
                    /* ASCII Char */ 
                    return wch; 
          }  

          /* I don't support unicode it is too big! */ 
          return 0; 
}  

WCHAR ff_wtoupper (WCHAR wch) 
{ 
          if (wch < 0x80) {      
                    /* ASCII Char */ 
                    if (wch >= 'a' && wch <= 'z') { 
                              wch &= ~0x20; 
                     } 
                      return wch; 
          }  

          /* I don't support unicode it is too big! */ 
          return 0; 
} 
//////////////////////////////////////////////////////////////////////////////////////

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
