
   ;```````````````````````````````````````````````````````````````
   ;  eKush - autogaz2699.a26

   dim _Ch0_Sound = q
   dim _Ch0_Duration = r
   dim _Ch0_Counter = s

   dim _Bit0_Reset_Restrainer = y

   const noscore = 1

__Start_Restart

   AUDV0 = 0 : AUDV1 = 0

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


  player0x=77:player0y=28


   _Bit0_Reset_Restrainer{0} = 1


__Main_Loop


   if _Ch0_Sound then goto __Skip_Fire


   if joy0fire then _Ch0_Sound = 1 : _Ch0_Duration = 1 : _Ch0_Counter = 0 : player0x=0 : player0y=0 : player1x=77  : player1y=28

__Skip_Fire


   if !_Ch0_Sound then goto __Skip_Ch_0

   _Ch0_Duration = _Ch0_Duration - 1

   if _Ch0_Duration then goto __Skip_Ch_0

   if _Ch0_Sound <> 1 then goto __Skip_Ch0_Sound_001

   temp4 = _SD_FireB[_Ch0_Counter]

   if temp4 = 255 then player1x=0 : player1y=0 : player0x=77 : player0y=28
   if temp4 = 255 then goto __Clear_Ch_0

   _Ch0_Counter = _Ch0_Counter + 1
   temp5 = _SD_FireB[_Ch0_Counter] : _Ch0_Counter = _Ch0_Counter + 1
   temp6 = _SD_FireB[_Ch0_Counter] : _Ch0_Counter = _Ch0_Counter + 1


   AUDV0 = temp4
   AUDC0 = temp5
   AUDF0 = temp6


   _Ch0_Duration = _SD_FireB[_Ch0_Counter] : _Ch0_Counter = _Ch0_Counter + 1


   goto __Skip_Ch_0

__Skip_Ch0_Sound_001


   goto __Skip_Ch_0

__Clear_Ch_0
   
   _Ch0_Sound = 0 : AUDV0 = 0



__Skip_Ch_0


   drawscreen


   if !switchreset then _Bit0_Reset_Restrainer{0} = 0 : goto __Main_Loop


   if _Bit0_Reset_Restrainer{0} then goto __Main_Loop

   goto __Start_Restart

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