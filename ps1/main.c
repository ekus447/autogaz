#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>
#include <libetc.h>
#include <libgs.h>

#include "data/imgs/before.h"
#include "data/imgs/after.h"
#include "data/imgs/polat.h"

#include "data/sounds/agm.h"
#include "audio.h"

#define PADCROSS	PADRdown
#define PADCIRCLE	PADRright

#define OT_LENGTH 1                              // ordering table length
#define PACKETMAX (300)                          // the max num of objects

GsOT      WorldOrderingTable[2];                 // ordering table handlers
GsOT_TAG  OrderingTable[2][1<<OT_LENGTH];        // ordering tables
PACKET    GPUOutputPacket[2][PACKETMAX];         // GPU Packet work area

GsSPRITE image1[2];

unsigned long __ramsize =   0x00200000;          // force 2 megabytes of RAM
unsigned long __stacksize = 0x00004000;          // force 16 kilobytes of stack

extern unsigned char before[];
extern unsigned char after[]; 
extern unsigned char polat[]; 

int currentbuffer; // double buffer holder

void initGraphics();
void clearVRAM();
void initImage();
void display();

int state = 0;
int timerVal = 0;

int main()
{
	int padStatus;

    ResetCallback();

    PadInit(0);
    initGraphics();

    audioInit();
    audioTransferVagToSPU(&agm, agm_size, SPU_0CH);

    while(1)
    {
    	padStatus = PadRead(0);

    	if (state == 0) {
    		displayImage(before);
    	} else if (state == 1) {
    		displayImage(after);
    		audioPlay(SPU_0CH);
    		timerVal = 0;
    		state = 2;
    	} else if (state == 2) {
    		displayImage(after);
    		Timer();
    	} else if (state == 3) {
    		displayImage(polat);
    	}

    	if (padStatus & PADCROSS) {
    		if (state == 0)
    			state = 1;
    	} else if (padStatus & PADCIRCLE) {
    		if (state == 3)
    			state = 0;
    	}

      display();
    }

    ResetGraph(3); // set the video mode back
    PadStopCom(); // stop controller communications

    return 0;
}

void initGraphics()
{
   int SCREEN_WIDTH, SCREEN_HEIGHT;

   // puts display mask into the status specified by the mask (0 not displayed, 1 displayed)
   SetDispMask(1);
   
   // resets the graphic system (0 full reset, 1 cancels the current drawing and flushes the command buffer, 3 initialises the drawing engine while preserving the current display environment)
   ResetGraph(0);

   // clear all VRAM contents
   clearVRAM();

   // automatic video mode control
   if (*(char *)0xbfc7ff52=='E')  // SCEE string address
   {
     // PAL MODE
     SCREEN_WIDTH = 320;
     SCREEN_HEIGHT = 256;
     SetVideoMode(1);
   }
   else
   {
     // NTSC MODE
     SCREEN_WIDTH = 320;
     SCREEN_HEIGHT = 240;
     SetVideoMode(0);
   }

   // set the graphics mode resolutions
   GsInitGraph(SCREEN_WIDTH, SCREEN_HEIGHT, GsNONINTER|GsOFSGPU, 1, 0);

   // set the top left coordinates of the two buffers in video memory
   GsDefDispBuff(0, 0, 0, SCREEN_HEIGHT);

   // initialise the ordering tables
   WorldOrderingTable[0].length = OT_LENGTH;
   WorldOrderingTable[1].length = OT_LENGTH;
   WorldOrderingTable[0].org = OrderingTable[0];
   WorldOrderingTable[1].org = OrderingTable[1];
   
   GsClearOt(0,0,&WorldOrderingTable[0]);
   GsClearOt(0,0,&WorldOrderingTable[1]);

}

void clearVRAM()
{
	RECT rectTL;
	
	setRECT(&rectTL, 0, 0, 1024, 512);
	ClearImage2(&rectTL, 0, 0, 0);
	
    DrawSync(0); // ensure that the VRAM is clear before exiting
    
    return;
}

void displayImage(unsigned char igm[])
{
   RECT            rect;                                    // RECT structure
   GsIMAGE         tim_data;                                // holds tim graphic info
   	
   // put data from tim file into rect         
   GsGetTimInfo ((u_long *)(igm+4),&tim_data);

   // load the image into the frame buffer
   rect.x = tim_data.px;                                    // tim start X coord
   rect.y = tim_data.py;                                    // tim start Y coord
   rect.w = tim_data.pw;                                    // data width
   rect.h = tim_data.ph;                                    // data height
   // load the tim data into the frame buffer
   LoadImage(&rect, tim_data.pixel);       

   // load the CLUT into the frame buffer
   rect.x = tim_data.cx;                                    // x pos in frame buffer
   rect.y = tim_data.cy;                                    // y pos in frame buffer
   rect.w = tim_data.cw;                                    // width of CLUT
   rect.h = tim_data.ch;                                    // height of CLUT
   // load data into frame buffer (DMA from DRAM to VRAM)
   LoadImage(&rect, tim_data.clut);                


   // initialise sprite
   image1[0].attribute=0x2000000;                           // 16 bit CLUT, all options off (0x1 = 8-bit, 0x2 = 16-bit)
   image1[0].x = 0;                                         // draw at x coord 0
   image1[0].y = 0;                                         // draw at y coord 0
   image1[0].w = 256;                                       // width of sprite
   image1[0].h = tim_data.ph;                               // height of sprite
   // texture page | texture mode (0 4-bit, 1 8-bit, 2 16-bit), semi-transparency rate, texture x, texture y in the framebuffer
   image1[0].tpage=GetTPage(1, 2, 320, 0);
   image1[0].r = 128;                                       // RGB Data
   image1[0].g = 128;
   image1[0].b = 128;
   image1[0].u=0;                                           // position within timfile for sprite
   image1[0].v=0;                                           
   image1[0].cx = tim_data.cx;                              // CLUT location x coord
   image1[0].cy = tim_data.cy;                              // CLUT location y coord
   image1[0].r=image1[0].g=image1[0].b=128;                 // normal luminosity
   image1[0].mx = 0;                                        // rotation x coord
   image1[0].my = 0;                                        // rotation y coord
   image1[0].scalex = ONE;                                  // scale x coord (ONE = 100%)
   image1[0].scaley = ONE;                                  // scale y coord (ONE = 100%)
   image1[0].rotate = 0;                                    // degrees to rotate   


   // initialise sprite
   image1[1].attribute=0x2000000;                           // 16 bit CLUT, all options off (0x1 = 8-bit, 0x2 = 16-bit)
   image1[1].x = 256;                                       // draw at x coord 0
   image1[1].y = 0;                                         // draw at y coord 0
   image1[1].w = 64;                                        // width of sprite
   image1[1].h = tim_data.ph;                               // height of sprite
   // texture page | texture mode (0 4-bit, 1 8-bit, 2 16-bit), semi-transparency rate, texture x, texture y in the framebuffer
   image1[1].tpage=GetTPage(1, 2, 576, 0);
   image1[1].r = 128;                                       // RGB Data
   image1[1].g = 128;
   image1[1].b = 128;
   image1[1].u=0;                                           // position within timfile for sprite
   image1[1].v=0;                                           
   image1[1].cx = tim_data.cx;                              // CLUT Location x coord
   image1[1].cy = tim_data.cy;                              // CLUT Location y coord
   image1[1].r=image1[1].g=image1[1].b=128;					// normal luminosity
   image1[1].mx = 0;                                        // rotation x coord
   image1[1].my = 0;                                        // rotation y coord
   image1[1].scalex = ONE;                                  // scale x coord (ONE = 100%)
   image1[1].scaley = ONE;                                  // scale y coord (ONE = 100%)
   image1[1].rotate = 0;                                    // degrees to rotate   

   // wait for all drawing to finish
   DrawSync(0);
}

void display()
{
   // refresh the font

   // get the current buffer
   currentbuffer=GsGetActiveBuff();

   // setup the packet workbase
   GsSetWorkBase((PACKET*)GPUOutputPacket[currentbuffer]);

   // clear the ordering table
   GsClearOt(0,0,&WorldOrderingTable[currentbuffer]);

   // insert sprites into the ordering table

   /* draw before
   */
   GsSortSprite(&image1[0], &WorldOrderingTable[currentbuffer], 0);
   GsSortSprite(&image1[1], &WorldOrderingTable[currentbuffer], 0);

   // wait for all drawing to finish
   DrawSync(0);

   // wait for v_blank interrupt
   VSync(0);

   // flip double buffers
   GsSwapDispBuff();

   // clear the ordering table with a background color
   GsSortClear(0,0,0,&WorldOrderingTable[currentbuffer]);

   // Draw the ordering table for the currentbuffer
   GsDrawOt(&WorldOrderingTable[currentbuffer]);
}

void Timer() {
	timerVal++;
	if (timerVal == 476) state = 3;
}