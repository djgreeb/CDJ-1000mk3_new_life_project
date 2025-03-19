
      
      
      
      //	ver. 1.05
      //		-	added dynamic waveform color map from original dump cdj-2000nxs
      //	ver. 1.07
      //		-	fixed rekordbox parser from parser version 0.39 SLDZ project
      //	ver. 1.11 
      //		- SDRAM initialization has been removed, which speeds up loading
      //		- the code is split into separate files
      //		- increased delay before starting SPI DMA to start transactions between STM and Pioneer panel mpu
      //		- modify HAL_SAI_Transmit_IT and SAI_FillFifo and HAL_SAI_IRQHandler function
      //	ver. 1.13 
      //		- changed interpolators coefficients (optimal 32x, 4-point, 3rd-order)
      //	ver. 1.15 
      //		- optimized audio prc (removed unnecessary variable translation operation before sending to SAI)
      //		- added variable audio TRIM depending on playback speed
      //		- added an algorithm for smooth changes in the TRIM variable to prevent clicks in the sound when there is a sudden change
      //	ver. 1.17
      //		- the dynamic waveform is now 480px wide and has HOT CUEs lettering



https://drive.google.com/file/d/1VFx4JItAnkkie4v-_Njo-SxVj8lTepl5/view?usp=sharing

https://drive.google.com/open?id=18gmD5JTporbCdqWj66FL6tRKBdp1W8vY

