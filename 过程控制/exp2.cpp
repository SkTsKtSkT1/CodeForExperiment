#include <stdio.h>
#include <windows.h>
#include "driver.h"  
#include "conio.h"
#include <time.h> 

void main()
{
	int i=0;
	float advalue;  //input 
    float height;  //height

	float davalue; //output

	float target=200.0/80;
    float error[3]={0}; //
    float TI=2,TD=0,Kc=2,Ts=0.5; //设定 Ts s
    float KI=((Kc*Ts)/TI),KD=((Kc*TD)/Ts);

    DWORD ErrCde;
    ULONG DevNum=0;
    long DriverHandle;
    float delta_u;
    float u=0; //初始输出控制量为0
    char keyboard;
	
	FILE *fp=fopen("H:\PID4.txt","w");
	if(fp==NULL)
	{printf("file open error!");}
    PT_AIConfig ptAIConfig;//配置输入
    PT_AIVoltageIn ptAIVoltageIn;

    PT_AOConfig ptAOConfig;//配置输出
    PT_AOVoltageOut ptAOVoltageOut;

    ErrCde = DRV_DeviceOpen(DevNum,&DriverHandle); //打开设备,然后进行试验.
    if(ErrCde==SUCCESS) //设备打开成功
    {
        printf("设备打开成功!\n");
    }
 //input 
	ptAIConfig.DasChan=0;
    ptAIConfig.DasGain=4;//-10~+10 V
    DRV_AIConfig(DriverHandle,(LPT_AIConfig)&ptAIConfig);  //AI0配置
//output
	ptAOConfig.chan=0;
    ptAOConfig.RefSrc=0;
    ptAOConfig.MaxValue=10;
    ptAOConfig.MinValue=0;
    DRV_AOConfig(DriverHandle,(LPT_AOConfig)&ptAOConfig); //AO0配置
   
//AI0
	ptAIVoltageIn.chan=0;
    ptAIVoltageIn.gain=4;// 输入范围,同输入配置
    ptAIVoltageIn.TrigMode=0;//内部触发
    ptAIVoltageIn.voltage=(FLOAT far *)&advalue; 
//DRV_AIVoltageIn(DriverHandle,(LPT_AIVoltageIn)&ptAIVoltageIn); 测量调用函数。
	ptAOVoltageOut.chan=0;
    ptAOVoltageOut.OutputValue=davalue;
    //DRV_AOVoltageOut(DriverHandle,(LPT_AOVoltageOut)&ptAOVoltageOut); 输出调用函数

	while(1)
	{
		DRV_AIVoltageIn(DriverHandle,(LPT_AIVoltageIn)&ptAIVoltageIn);  //测量当前液位电压
		error[2]=error[1];
		error[1]=error[0];
		error[0]=target-advalue;
		
		
		delta_u=Kc*(error[0]-error[1])+KI*error[0]+KD*(error[0]-2*error[1]+error[2]); 
		u=u+delta_u;//计算输出的控制量
		if(u>10)
		{u=10;}
		if(u<3)
		{u=3;}
		printf("height:%f,error:%f,u:%f,time:%fs\n",advalue*80,error[0]*80,u,i*0.5); //height error
		ptAOVoltageOut.OutputValue=u;
        DRV_AOVoltageOut(DriverHandle,(LPT_AOVoltageOut)&ptAOVoltageOut);  //输出控制量
		//fprintf(fp,"voltage:%f,height:%f,time:%f\n",u,advalue*80,i*0.5);
		fprintf(fp,"%f,%f,%f\n",u,advalue*80,i*0.5);
		if(kbhit())
        {
            keyboard=getch();
            if(keyboard=='q'){
                ptAOVoltageOut.OutputValue = 0; //输出设定
                DRV_AOVoltageOut(DriverHandle,(LPT_AOVoltageOut)&ptAOVoltageOut); //输出电压清零	
		        break;
            }
        }
        //fopen();
       Sleep(1000*Ts);
	i++;
	}




}