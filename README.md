# CDJ-1000mk3_new_life_project
CDJ-1000mk3 mod, which allows SD-card playback
This firmware for F7 chip, 32F746G DISCOVERY board.
Added support for some Rekordbox functions, display of static and dynamic waveform, audio interpolation process. 

History of firmware changes

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
//		ver. 0.68a
//		- added QSPI flash support
//		- added animation startup logo
//		ver. 0.69a
//		- added REKORDBOX logo on startup
//		- added tag list 
//		- added internal function for RedrawWaveforms process 
//		ver. 0.70a
//		- added INFO mode in BROWSER and TAG LIST menu
//		- added internal functions for optimize code
//		ver. 0.71a
//		- improved the functionality of the TAG LIST menu
//		ver. 0.72a
//		- added symbol red check mark for TAG LIST
//		- improved the functionality of the TAG LIST (added TAG TRACK/REMOVE button)
//		- bugs fixes internal functions browser and tag list
//		- bugs fixes NAVIGATOR function
//		- added loading track from TAG LIST
//		ver. 0.73a
//		- added UTILITY window
//		- added long press MENU button for entering to UTILITY
//		- fixed check WAV header
//		-	optimized process add/delete tracks in TAG LIST
//		ver. 0.74a
//		- added SPI transfer for CDJ-1000mk3 panel
//		ver. 0.75a
//		- added reading cues and memory pionts from ANLZXXXX.DAT
//		- fixed bug SPI DMA (see note*) 
//		ver. 0.76a
//		- added PLAY, TEMPO, TEMPO RESET, JOG MODE buttons
//		- added pitch slider 4 ranges
//		- added calculation tempo and bpm after pitch change
//		ver. 0.77a
//		- added PLAY blink led, CUE led
//		- improved jog vinyl mode (inertial process)
//		- added TRACK SEARCH buttons
//		- improved ShowTempo function
//		- added TIME MODE button, REMAIN MODE
//		- added CDJ JOG MODE work
//		- optimized pitch bend coeficients aka CDJ-1000mk3
//		- optimized VINYL MODE precision jog step
//		ver. 0.78a
//		- improved static scroll UI
//		- added BEATGRID massive
//		- shift up 1px static information, static waveform 
//		- beatgrid support
//		- improved Phase Meter
//		ver. 0.79a
//		- improved BPM calculating after pitch change
//		- added BPMGRID for tracks with variable BPM 
//		ver. 0.80a
//		- added Font13D for phase bars
//		- added slip mode marker on jog display
//		- added master player ICON
//		- added slip mode red button and slip mode jog illumination
//		- fixed bug spin jog at maximum speed (variable overflow)
//		- added first SLIP MODE functions with audio processing
//		- added animation icon and gradient for phase bars
//		- improved jog pitch bend in reverse mode
//		- improved jog in slip mode
//		ver. 0.81a
//		- added CUE blink
//		- improved time mode button code
//		- added VINYL RELEASE/START and TOUCH/BREAKE mode 
//		-	optimized SPI-DMA transfer process
//		-	optimized potenciometer's curve for VINYL RELEASE/START and TOUCH/BREAKE mode
//		ver. 0.82a
//		- added track nubmer and status (playing or played) in INFO mode
//		- added filling buffer step sequencer for optimize time gaps
//		- fixed filling buffer step sequencer algoritm
//		ver. 0.83a
//		- improved the work of the function of static and dynamic waveforms
//		- added "remain/foward time style" for progress bar
//		ver. 0.84b
//		- added blink progress bar when the remaining time is less than 30sec
//		- fixed DrawMinuteMarkers function
//		- improved performance static and dynamic waveforms (added ForceDrawVLine function)
//		- added checking device UID 
//		- fixed fatal error when deleting a track from an empty tag list
//		- the first addition of a function CUE audio
//		ver. 0.85b
//		- added CUE button process
//		- improved CUE audio process
