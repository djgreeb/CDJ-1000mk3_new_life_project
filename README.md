# CDJ-1000mk3_new_life_project
CDJ-1000mk3 mod, which allows SD-card playback
This firmware for F7 chip, 32F746G DISCOVERY board.
Added support for some Rekordbox functions, display of static and dynamic waveform, audio interpolation process. 


//	ver. 0.90b

//		- database parser DeviceSQL updated based on document: "Rekordbox Export Structure Analysis" James Elliott Deep Symmetry, LLC

//		- change colorystic dynamic waveform (lower white point)

//		- change long touch timer for MENU button

//		- added bpm to INFO menu

//		- added colored rating to INFO menu

//		- added KEY to INFO menu

//		- added duration to INFO menu

//		- maximum tracks in database - 512 (not enough memory)

//		- maximum playlists in database - 20 (not enough memory)

//		- supported only latin encoding in track names and tags

//		- added encoder signal filter

//		- show KEY on waveform fisplay

//		- added pages in the browser with animation: playlists, tracks, SD card information

//		- browser animation bug fixed

//		- resized cue marker on dynamic waveform

//		- change logic INFO button

//		- fixed database parser

//		- fixed text line overflow error

//		- added REALTIME CUE (set, when track playing) 

//		- added AUTO CUE to the first bit of the bitgrid

//		- added full UID chip in HEX in utility		

//		- fixed TAG LIST exiting borders when deleting tracks

//	ver. 0.97b

//		- improved browser menu

//		- add flash disk name and date (at the root of the browser)

//		- fixed floating pitch tempo values

//		- improved slip mode on CUE

//		- fixed the work of jog with a hot CUE

//		- improved mechanical imitation of jog

//		- changed color and style gradient bar

//		- changed color dynamic waveform

//		- optimized dynamic waveform work

//		- added QUANTIZE for CUE and LOOP

//		- exclude noise at the end of the track

//		- added CRC control for SPI Rx package

//		- fixed start phase detection BEATGRID

//		- added LOOP MODE (beta)

//		- optimized time display function to improve performance	

//	ver. 1.03

//		- changed function control for SPI Rx package

//		- in UTILITY it is now possible to change parameters and save to internal memory

//		- added load lock

//		- added AUTO CUE LEVEL with MEMORY, FIRST BEAT MODE and analog tresholds (-36dB, -42dB and other)

//		- added TIME MODE DEFAULT 

//		- added TEMPO RANGE DEFAULT 

//		- SLIP REVERSE MODE for REVERSE SWITCH

//		- added ability to select RGB or BLUE waveforms

//		- added LCD brightness

//		- added jog indicator when the track ends 

//		- added jog brightness

//		- added BPM color

//		- added audio output level

//		- for use JOG PWM output pin CN4-7 for it to work. see schematic STM32F746 Discovery

//		- for LCD PWM to work, remove R85 and install

//		  it between U10-7 and GND. Remove R81 and R66. Connect together the U10-7 and RMII_CRS_DV, 

//		  at the point where R66 was.) see schematic STM32F746 Discovery


SOURCE CODE LINK:
ver. 1.03
https://drive.google.com/file/d/1VFx4JItAnkkie4v-_Njo-SxVj8lTepl5/view?usp=sharing


https://drive.google.com/open?id=18gmD5JTporbCdqWj66FL6tRKBdp1W8vY

Keil uVision5
