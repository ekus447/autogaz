!-BEFORE

10 PRINT ""
20 PRINT ""
30 PRINT "                 {reverse on}{cyan}      "
40 PRINT "                {reverse on} {reverse off}      {reverse on} "
50 PRINT "               {reverse on} {reverse off}  {reverse on} {reverse off} {reverse on} {reverse off}   {reverse on} "
60 PRINT "               {reverse on} {reverse off}        {reverse on} "
70 PRINT "               {reverse on} {reverse off}        {reverse on} "
80 PRINT "               {reverse on} {reverse off}  {reverse on}    {reverse off}  {reverse on} "
90 PRINT "                {reverse on} {reverse off}      {reverse on} "
100 PRINT "                 {reverse on}      "
110 PRINT "                   {reverse on}  "
120 PRINT "            {reverse on}   {reverse off}    {reverse on}  "
130 PRINT "               {reverse on}        "
140 PRINT "                   {reverse on}  {reverse off}  {reverse on}   "
150 PRINT "                   {reverse on}  "
160 PRINT "                   {reverse on}  "
170 PRINT "                   {reverse on}  "
180 PRINT "                {reverse on}     "
190 PRINT "               {reverse on} {reverse off}    {reverse on} "
200 PRINT "              {reverse on} {reverse off}     {reverse on}  "
210 PRINT "              {reverse on} {reverse off}       {reverse on} "
220 PRINT "              {reverse on} {reverse off}        {reverse on} "
230 PRINT "              {reverse on} {reverse off}        {reverse on} "
240 PRINT ""
250 PRINT ""

!-KEY LISTENER

260 keyListener = peek(56321)
270 IF keyListener=239 THEN goto 290
280 goto 260

!-AFTER

290 PRINT ""
300 PRINT ""
310 PRINT "                 {reverse on}{cyan}      "
320 PRINT "                {reverse on} {reverse off}      {reverse on} "
330 PRINT "               {reverse on} {reverse off}  {reverse on} {reverse off} {reverse on} {reverse off}   {reverse on} "
340 PRINT "               {reverse on} {reverse off}        {reverse on} "
350 PRINT "               {reverse on} {reverse off} {reverse on} {reverse off}    {reverse on} {reverse off} {reverse on} "
360 PRINT "               {reverse on} {reverse off}  {reverse on}    {reverse off}  {reverse on} "
370 PRINT "                {reverse on} {reverse off}      {reverse on} "
380 PRINT "                 {reverse on}      "
390 PRINT "                   {reverse on}  "
400 PRINT "            {reverse on}   {reverse off}    {reverse on}  "
410 PRINT "               {reverse on}        "
420 PRINT "                   {reverse on}  {reverse off}  {reverse on}   "
430 PRINT "                   {reverse on}  "
440 PRINT "                   {reverse on}  "
450 PRINT "                   {reverse on}  "
460 PRINT "                {reverse on}     "
470 PRINT "               {reverse on} {reverse off}    {reverse on} "
480 PRINT "              {reverse on} {reverse off}     {reverse on}  "
490 PRINT "              {reverse on} {reverse off}       {reverse on} "
500 PRINT "              {reverse on} {reverse off}        {reverse on} "
510 PRINT "              {reverse on} {reverse off}        {reverse on} "
520 PRINT ""
530 PRINT ""

!-MUSIC

!-AYARLAR
540 FORL=54272TO54296:POKEL,0:NEXT
550 V=54296:W=54276:A=54277:S=54278:H=54273:L=54272
560 POKEV,15:POKEA,190:POKES,89
!-A
570 POKEH,57:POKEL,172
580 POKEW,33:FORT=1TO150:NEXT
!-B
590 POKEH,64:POKEL,188
600 POKEW,33:FORT=1TO150:NEXT
!-C
610 POKEH,68:POKEL,149
620 POKEW,33:FORT=1TO150:NEXT
!-E
630 POKEH,43:POKEL,52
640 POKEW,33:FORT=1TO100:NEXT
!-BOS
650 POKEH,0:POKEL,0
660 POKEW,33:FORT=1TO200:NEXT
!-E
670 POKEH,43:POKEL,52
680 POKEW,33:FORT=1TO800:NEXT
!-A
690 POKEH,57:POKEL,172
700 POKEW,33:FORT=1TO200:NEXT
!-BOS
710 POKEH,0:POKEL,0
720 POKEW,33:FORT=1TO200:NEXT
!-
730 POKEH,57:POKEL,172
740 POKEW,33:FORT=1TO800:next
!-BOS
750 POKEH,0:POKEL,0
760 POKEW,33:FORT=1TO150:NEXT
!-A
770 POKEH,57:POKEL,172
780 POKEW,33:FORT=1TO400:NEXT
!-E
790 POKEH,43:POKEL,52
800 POKEW,33:FORT=1TO400:NEXT
!-D
810 POKEH,38:POKEL,126
820 POKEW,33:FORT=1TO400:NEXT
!-E
830 POKEH,43:POKEL,52
840 POKEW,33:FORT=1TO400:NEXT
!-G
850 POKEH,51:POKEL,97
860 POKEW,33:FORT=1TO450:NEXT
!-E
870 POKEH,43:POKEL,52
880 POKEW,33:FORT=1TO1200:NEXT
!-STOP
890 POKEW,32

!-LOOP
900 goto 10