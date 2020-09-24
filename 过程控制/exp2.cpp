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
    float TI=2,TD=0,Kc=2,Ts=0.5; //�趨 Ts s
    float KI=((Kc*Ts)/TI),KD=((Kc*TD)/Ts);

    DWORD ErrCde;
    ULONG DevNum=0;
    long DriverHandle;
    float delta_u;
    float u=0; //��ʼ���������Ϊ0
    char keyboard;
	
	FILE *fp=fopen("H:\PID4.txt","w");
	if(fp==NULL)
	{printf("file open error!");}
    PT_AIConfig ptAIConfig;//��������
    PT_AIVoltageIn ptAIVoltageIn;

    PT_AOConfig ptAOConfig;//�������
    PT_AOVoltageOut ptAOVoltageOut;

    ErrCde = DRV_DeviceOpen(DevNum,&DriverHandle); //���豸,Ȼ���������.
    if(ErrCde==SUCCESS) //�豸�򿪳ɹ�
    {
        printf("�豸�򿪳ɹ�!\n");
    }
 //input 
	ptAIConfig.DasChan=0;
    ptAIConfig.DasGain=4;//-10~+10 V
    DRV_AIConfig(DriverHandle,(LPT_AIConfig)&ptAIConfig);  //AI0����
//output
	ptAOConfig.chan=0;
    ptAOConfig.RefSrc=0;
    ptAOConfig.MaxValue=10;
    ptAOConfig.MinValue=0;
    DRV_AOConfig(DriverHandle,(LPT_AOConfig)&ptAOConfig); //AO0����
   
//AI0
	ptAIVoltageIn.chan=0;
    ptAIVoltageIn.gain=4;// ���뷶Χ,ͬ��������
    ptAIVoltageIn.TrigMode=0;//�ڲ�����
    ptAIVoltageIn.voltage=(FLOAT far *)&advalue; 
//DRV_AIVoltageIn(DriverHandle,(LPT_AIVoltageIn)&ptAIVoltageIn); �������ú�����
	ptAOVoltageOut.chan=0;
    ptAOVoltageOut.OutputValue=davalue;
    //DRV_AOVoltageOut(DriverHandle,(LPT_AOVoltageOut)&ptAOVoltageOut); ������ú���

	while(1)
	{
		DRV_AIVoltageIn(DriverHandle,(LPT_AIVoltageIn)&ptAIVoltageIn);  //������ǰҺλ��ѹ
		error[2]=error[1];
		error[1]=error[0];
		error[0]=target-advalue;
		
		
		delta_u=Kc*(error[0]-error[1])+KI*error[0]+KD*(error[0]-2*error[1]+error[2]); 
		u=u+delta_u;//��������Ŀ�����
		if(u>10)
		{u=10;}
		if(u<3)
		{u=3;}
		printf("height:%f,error:%f,u:%f,time:%fs\n",advalue*80,error[0]*80,u,i*0.5); //height error
		ptAOVoltageOut.OutputValue=u;
        DRV_AOVoltageOut(DriverHandle,(LPT_AOVoltageOut)&ptAOVoltageOut);  //���������
		//fprintf(fp,"voltage:%f,height:%f,time:%f\n",u,advalue*80,i*0.5);
		fprintf(fp,"%f,%f,%f\n",u,advalue*80,i*0.5);
		if(kbhit())
        {
            keyboard=getch();
            if(keyboard=='q'){
                ptAOVoltageOut.OutputValue = 0; //����趨
                DRV_AOVoltageOut(DriverHandle,(LPT_AOVoltageOut)&ptAOVoltageOut); //�����ѹ����	
		        break;
            }
        }
        //fopen();
       Sleep(1000*Ts);
	i++;
	}




}