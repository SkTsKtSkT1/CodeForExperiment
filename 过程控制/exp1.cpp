#include <stdio.h>
#include <windows.h>
#include "driver.h" 
#include <time.h>
#include <conio.h>

/*
void main() 

{ 
   long DriverHandle; 
   ULONG num1=0,num2=0;
   int count;
   float quality;
   float b;
   USHORT over=0;
   PT_CounterEventStart ptCounterEventStart;
   PT_CounterEventRead ptCounterEventRead;


   DRV_DeviceOpen(0,&DriverHandle); 

   ptCounterEventStart.counter=0;  //0通道计数器初始化
   DRV_CounterEventStart(DriverHandle,&ptCounterEventStart);
 
   for(int i=0;i<1000;i++)
   {  ptCounterEventRead.counter=0;  //0通道计数器
   ptCounterEventRead.overflow=&over;  //计数溢出标志
   ptCounterEventRead.count=&num1; //存储当前计数值
   DRV_CounterEventRead (DriverHandle, (LPT_CounterEventRead)&ptCounterEventRead);
   Sleep(1000);
   ptCounterEventRead.counter=0;
   ptCounterEventRead.overflow=&over;
   ptCounterEventRead.count=&num2;
   DRV_CounterEventRead (DriverHandle, (LPT_CounterEventRead)&ptCounterEventRead);
   count=num2-num1;
   quality=count*((15.0-0.5)/(1200.0-13.0))+(15.0-1200.0*((15.0-0.5)/(1200.0-13.0)));
   printf("count_num=%d,quality=%fl/min\n",count,quality);
   }
   DRV_CounterReset(DriverHandle,0); 
 
   DRV_DeviceClose( &DriverHandle ); 
}    
*/