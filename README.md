
      Version history:
      
      
	ver. 0.61a
		- optimized browser function
		- added dark_green color in browser for text on white line cursor 
	ver. 0.62a
		- optimized DATABASE_PARSER finction
		- DATABASE_PARSER using WFORMDYNAMIC massive only 
	ver. 0.63a
		- optimized LOAD_TRACK function. Now the PCM[] massive is not used.
		- PCM[] converted in uint16_t
		- optimized audio processing for new uint16_t PCM[]. Now the calculation of 1 sample is 5us instead of 7.5us 
	ver. 0.64a
		- fixed joint in circular buffer in audio processing
		- optimized ring buffer size for increased audio processing performance
	ver. 0.65a
		- optimized audio processing (read SDRAM). Now the calculation of 1 sample is 4us 
	 	- added SD_LED blink
		- added encoder blink when loading track
	ver. 0.66a
		- added touch screen control
	ver. 0.67a
		- added basic function needle search
		- fixed function BSP_LCD_DisplayStringAt. 
		-	added TRANSPARENT_MODE for text. 
		- changed method of determination the font with dynamic width (Font15P) and other fonts with static width.  
		- optimized touch controller driver
		- create audioparcer for SEEK function
	ver. 0.68a
		- added QSPI flash support
		- added animation startup logo
	ver. 0.69a
		- added REKORDBOX logo on startup
		- added tag list 
		- added internal function for RedrawWaveforms process 
	ver. 0.70a
		- added INFO mode in BROWSER and TAG LIST menu
		- added internal functions for optimize code
	ver. 0.71a
		- improved the functionality of the TAG LIST menu
	ver. 0.72a
		- added symbol red check mark for TAG LIST
		- improved the functionality of the TAG LIST (added TAG TRACK/REMOVE button)
		- bugs fixes internal functions browser and tag list
		- bugs fixes NAVIGATOR function
		- added loading track from TAG LIST
	ver. 0.73a
		- added UTILITY window
		- added long press MENU button for entering to UTILITY
		- fixed check WAV header
		-	optimized process add/delete tracks in TAG LIST
	ver. 0.74a
		- added SPI transfer for CDJ-1000mk3 panel
	ver. 0.75a
		- added reading cues and memory pionts from ANLZXXXX.DAT
		- fixed bug SPI DMA (see note*) 
	ver. 0.76a
		- added PLAY, TEMPO, TEMPO RESET, JOG MODE buttons
		- added pitch slider 4 ranges
		- added calculation tempo and bpm after pitch change
	ver. 0.77a
		- added PLAY blink led, CUE led
		- improved jog vinyl mode (inertial process)
		- added TRACK SEARCH buttons
		- improved ShowTempo function
		- added TIME MODE button, REMAIN MODE
		- added CDJ JOG MODE work
		- optimized pitch bend coeficients aka CDJ-1000mk3
		- optimized VINYL MODE precision jog step
	ver. 0.78a
		- improved static scroll UI
		- added BEATGRID massive
		- shift up 1px static information, static waveform 
		- beatgrid support
		- improved Phase Meter
	ver. 0.79a
		- improved BPM calculating after pitch change
		- added BPMGRID for tracks with variable BPM 
	ver. 0.80a
		- added Font13D for phase bars
		- added slip mode marker on jog display
		- added master player ICON
		- added slip mode red button and slip mode jog illumination
		- fixed bug spin jog at maximum speed (variable overflow)
		- added first SLIP MODE functions with audio processing
		- added animation icon and gradient for phase bars
		- improved jog pitch bend in reverse mode
		- improved jog in slip mode
	ver. 0.81a
		- added CUE blink
		- improved time mode button code
		- added VINYL RELEASE/START and TOUCH/BREAKE mode 
		-	optimized SPI-DMA transfer process
		-	optimized potenciometer's curve for VINYL RELEASE/START and TOUCH/BREAKE mode
	ver. 0.82a
		- added track nubmer and status (playing or played) in INFO mode
		- added filling buffer step sequencer for optimize time gaps
		- fixed filling buffer step sequencer algoritm
	ver. 0.83a
		- improved the work of the function of static and dynamic waveforms
		- added "remain/foward time style" for progress bar
	ver. 0.84b
		- added blink progress bar when the remaining time is less than 30sec
		- fixed DrawMinuteMarkers function
		- improved performance static and dynamic waveforms (added ForceDrawVLine function)
		- added checking device UID 
		- fixed fatal error when deleting a track from an empty tag list
		- the first addition of a function CUE audio
	ver. 0.85b
		- added CUE button process
		- improved CUE audio process
		- optimized pitch bend coefficients
		- added loading of Hot Cues and Memory Cues attributes
	ver. 0.86b
		- added MEMORY CUE calling
		- bugs fixes calling CUEs when jog in CDJ mode
		- added CUE, MEMORY CUE and HOT CUE triangles on dynamic waveform
	ver. 0.90b
		- database parser DeviceSQL updated based on document: "Rekordbox Export Structure Analysis" James Elliott Deep Symmetry, LLC
		- change colorystic dynamic waveform (lower white point)
		- change long touch timer for MENU button
		- added bpm to INFO menu
		- added colored rating to INFO menu
		- added KEY to INFO menu
		- added duration to INFO menu
		- maximum tracks in database - 512 (not enough memory)
		- maximum playlists in database - 20 (not enough memory)
		- supported only latin encoding in track names and tags
		- added encoder signal filter
		- show KEY on waveform fisplay
		- added pages in the browser with animation: playlists, tracks, SD card information
		- browser animation bug fixed
		- resized cue marker on dynamic waveform
		- change logic INFO button
		- fixed database parser
		- fixed text line overflow error
		- added REALTIME CUE (set, when track playing) 			
		- added AUTO CUE to the first bit of the bitgrid
		- added full UID chip in HEX in utility			
		- fixed TAG LIST exiting borders when deleting tracks
	ver. 0.97b
		- improved browser menu
		- add flash disk name and date (at the root of the browser)
		- fixed floating pitch tempo values
		-	improved slip mode on CUE
		- fixed the work of jog with a hot CUE
		-	improved mechanical imitation of jog
		-	changed color and style gradient bar
		-	changed color dynamic waveform
		- optimized dynamic waveform work
		- added QUANTIZE for CUE and LOOP
		- exclude noise at the end of the track
		- added CRC control for SPI Rx package
		- fixed start phase detection BEATGRID
		-	added LOOP MODE (beta)
		- optimized time display function to improve performance	
	ver. 1.03
		-	changed function control for SPI Rx package
		- in UTILITY it is now possible to change parameters and save to internal memory
		- added load lock
		- added AUTO CUE LEVEL with MEMORY, FIRST BEAT MODE and analog tresholds (-36dB, -42dB and other)
		-	added TIME MODE DEFAULT 
		-	added TEMPO RANGE DEFAULT 
		-	SLIP REVERSE MODE for REVERSE SWITCH
		-	added ability to select RGB or BLUE waveforms
		-	added LCD brightness
		-	added jog indicator when the track ends 
		-	added jog brightness
		-	added BPM color
		-	added audio output level
		- for use JOG PWM output pin CN4-7 for it to work. see schematic STM32F746 Discovery
		- for LCD PWM to work, remove R85 and install 
			it between U10-7 and GND. Remove R81 and R66. Connect together the U10-7 and RMII_CRS_DV, 
			at the point where R66 was.) see schematic STM32F746 Discovery
	ver. 1.05
		-	added dynamic waveform color map from original dump cdj-2000nxs
	ver. 1.07
		-	fixed rekordbox parser from parser version 0.39 SLDZ project
	ver. 1.11 
		- SDRAM initialization has been removed, which speeds up loading
		- the code is split into separate files
		- increased delay before starting SPI DMA to start transactions between STM and Pioneer panel mpu
		- modify HAL_SAI_Transmit_IT and SAI_FillFifo and HAL_SAI_IRQHandler function
	ver. 1.13 
		- changed interpolators coefficients (optimal 32x, 4-point, 3rd-order)
	ver. 1.15 
		- optimized audio prc (removed unnecessary variable translation operation before sending to SAI)
		- added variable audio TRIM depending on playback speed
		- added an algorithm for smooth changes in the TRIM variable to prevent clicks in the sound when there is a sudden change
	ver. 1.17
		- the dynamic waveform is now 480px wide and has HOT CUEs lettering



Source code 1.03:

https:drive.google.com/file/d/1VFx4JItAnkkie4v-_Njo-SxVj8lTepl5/view?usp=sharing



https:drive.google.com/open?id=18gmD5JTporbCdqWj66FL6tRKBdp1W8vY

