
   ;```````````````````````````````````````````````````````````````
   ;  Channel 0 sound variables.
   ;
   dim _Ch0_Sound = q
   dim _Ch0_Duration = r
   dim _Ch0_Counter = s

   ;```````````````````````````````````````````````````````````````
   ;  Keeps the reset switch from repeating when pressed.
   ;
   dim _Bit0_Reset_Restrainer = y



   ;***************************************************************
   ;
   ;  Disables the score.
   ;
   const noscore = 1





   ;***************************************************************
   ;***************************************************************
   ;
   ;  PROGRAM START/RESTART
   ;
   ;
__Start_Restart


   ;***************************************************************
   ;
   ;  Mutes volume of both sound channels.
   ;
   AUDV0 = 0 : AUDV1 = 0


   ;***************************************************************
   ;
   ;  Clears all normal variables (fastest way).
   ;
   a = 0 : b = 0 : c = 0 : d = 0 : e = 0 : f = 0 : g = 0 : h = 0 : i = 0
   j = 0 : k = 0 : l = 0 : m = 0 : n = 0 : o = 0 : p = 0 : q = 0 : r = 0
   s = 0 : t = 0 : u = 0 : v = 0 : w = 0 : x = 0 : y = 0 : z = 0

 playfield:
 ................................
 ............XXXXXXXX............
 ...........XX......XX...........
 ...........XX......XX...........
 ............XXXXXXXX............
 ...............XX...............
 ............XXXXXXXX............
 ..........XX...XX...XX..........
 ..............XXXX..............
 ............XXX..XXX............
 ...........XX......XX...........
end



 player0:
 %00000000
 %01111110
 %00000000
 %00000000
 %00100100
 %00100100
 %00100100
 %00000000
end


 player1:
 %00000000
 %00111100
 %01000010
 %00000000
 %00100100
 %00100100
 %00100100
 %00000000
end


 COLUBK = $0E


   ;***************************************************************
   ;
   ;   timelines


  player0x=77:player0y=28




   ;***************************************************************
   ;
   ;  Restrains the reset switch for the main loop.
   ;
   ;  This bit fixes it so the reset switch becomes inactive if
   ;  it hasn't been released after being pressed once.
   ;
   _Bit0_Reset_Restrainer{0} = 1





   ;***************************************************************
   ;***************************************************************
   ;
   ;  MAIN LOOP (MAKES THE PROGRAM GO)
   ;
   ;
__Main_Loop


   ;***************************************************************
   ;
   ;  Fire button check.
   ;
   ;```````````````````````````````````````````````````````````````
   ;  Skips this section if a channel 0 sound effect is on.
   ;
   if _Ch0_Sound then goto __Skip_Fire

   ;```````````````````````````````````````````````````````````````
   ;  Turns on channel 0 sound effect 1 if fire button is pressed.
   ;
   if joy0fire then _Ch0_Sound = 1 : _Ch0_Duration = 1 : _Ch0_Counter = 0 : player0x=0 : player0y=0 : player1x=77  : player1y=28

__Skip_Fire



   ;***************************************************************
   ;
   ;  Channel 0 sound effect check.
   ;
   ;```````````````````````````````````````````````````````````````
   ;  Skips all channel 0 sounds if sounds are off.
   ;
   if !_Ch0_Sound then goto __Skip_Ch_0

   ;```````````````````````````````````````````````````````````````
   ;  Decreases channel 0 duration counter.
   ;
   _Ch0_Duration = _Ch0_Duration - 1

   ;```````````````````````````````````````````````````````````````
   ;  Skips all channel 0 sounds if duration counter is greater
   ;  than zero.
   ;
   if _Ch0_Duration then goto __Skip_Ch_0



   ;***************************************************************
   ;
   ;  Channel 0 sound effect 001.
   ;
   ;  Fire button.
   ;
   ;```````````````````````````````````````````````````````````````
   ;  Skips this section if sound 001 isn't on.
   ;
   if _Ch0_Sound <> 1 then goto __Skip_Ch0_Sound_001

   ;```````````````````````````````````````````````````````````````
   ;  Retrieves first part of channel 0 data.
   ;
   temp4 = _SD_FireB[_Ch0_Counter]

   ;```````````````````````````````````````````````````````````````
   ;  Checks for end of data.
   ;
   if temp4 = 255 then player1x=0 : player1y=0 : player0x=77 : player0y=28
   if temp4 = 255 then goto __Clear_Ch_0

   ;```````````````````````````````````````````````````````````````
   ;  Retrieves more channel 0 data.
   ;
   _Ch0_Counter = _Ch0_Counter + 1
   temp5 = _SD_FireB[_Ch0_Counter] : _Ch0_Counter = _Ch0_Counter + 1
   temp6 = _SD_FireB[_Ch0_Counter] : _Ch0_Counter = _Ch0_Counter + 1

   ;```````````````````````````````````````````````````````````````
   ;  Plays channel 0.
   ;
   AUDV0 = temp4
   AUDC0 = temp5
   AUDF0 = temp6

   ;```````````````````````````````````````````````````````````````
   ;  Sets duration.
   ;
   _Ch0_Duration = _SD_FireB[_Ch0_Counter] : _Ch0_Counter = _Ch0_Counter + 1

   ;```````````````````````````````````````````````````````````````
   ;  Jumps to end of channel 0 area.
   ;
   goto __Skip_Ch_0

__Skip_Ch0_Sound_001



   ;,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
   ;```````````````````````````````````````````````````````````````
   ;
   ;  Other channel 0 sound effects go here.
   ;
   ;,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
   ;```````````````````````````````````````````````````````````````



   ;***************************************************************
   ;
   ;  Jumps to end of channel 0 area. (This catches any mistakes.)
   ;
   goto __Skip_Ch_0



   ;***************************************************************
   ;
   ;  Clears channel 0.
   ;
__Clear_Ch_0
   
   _Ch0_Sound = 0 : AUDV0 = 0



   ;***************************************************************
   ;
   ;  End of channel 0 area.
   ;
__Skip_Ch_0



   ;***************************************************************
   ;
   ;  Displays the screen.
   ;
   drawscreen



   ;***************************************************************
   ;
   ;  Reset switch check and end of main loop.
   ;
   ;  Any Atari 2600 program should restart when the reset  
   ;  switch is pressed. It is part of the usual standards
   ;  and procedures.
   ;
   ;```````````````````````````````````````````````````````````````
   ;  Turns off reset restrainer bit and jumps to beginning of
   ;  main loop if the reset switch is not pressed.
   ;
   if !switchreset then _Bit0_Reset_Restrainer{0} = 0 : goto __Main_Loop

   ;```````````````````````````````````````````````````````````````
   ;  Jumps to beginning of main loop if the reset switch hasn't
   ;  been released after being pressed.
   ;
   if _Bit0_Reset_Restrainer{0} then goto __Main_Loop

   ;```````````````````````````````````````````````````````````````
   ;  Restarts the program.
   ;
   goto __Start_Restart





   ;,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
   ;```````````````````````````````````````````````````````````````
   ;
   ;  END OF MAIN LOOP
   ;
   ;,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
   ;```````````````````````````````````````````````````````````````





   ;***************************************************************
   ;***************************************************************
   ;
   ;
   ;  Sound effect data starts here.
   ;
   ;
   ;***************************************************************
   ;***************************************************************
   ;
   ;  Sound data for fire button sound effect.
   ;
   data _SD_FireB
  8,12,11
  3
  0,0,0
  1
  2,12,11
  1
  0,0,0
  2

  8,4,31
  3
  0,0,0
  1
  2,4,31
  1  
  0,0,0
  2

  8,4,29
  3
  0,0,0
  1
  2,4,29
  1
  0,0,0
  2

  8,4,23
  15
  0,0,0
  1
  2,4,23
  1
  0,0,0
  8

  8,4,23
  38
  0,0,0
  1
  2,4,23
  1
  0,0,0
  8

  8,12,11
  15
  0,0,0
  1
  2,12,11
  1
  0,0,0
  8

  8,12,11
  50
  0,0,0
  1
  2,12,11
  1
  0,0,0
  8

  8,12,11
  20
  0,0,0
  1
  2,12,11
  1
  0,0,0
  8

  8,4,23
  20
  0,0,0
  1
  2,4,23
 1
  0,0,0
  8

  8,4,26
  20
  0,0,0
  1
  2,4,26
  1
  0,0,0
  8

  8,4,23
  20
  0,0,0
  1
  2,4,23
  1
  0,0,0
  8

  8,4,19
  20
  0,0,0
  1
  2,4,19
  1
  0,0,0
  8

  8,4,23
  66
  0,0,0
  1
  2,4,23
  1
  0,0,0
  8

  255

end