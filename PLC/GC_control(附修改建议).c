/*
 * @Author: SkTsKtSkT1
 * @Date: 2020-08-22 00:03:53
 * @LastEditors: SkTsKtSkT1
 * @LastEditTime: 2020-08-22 19:24:02
 * @Description: 
 */


/**************修改建议*******************

老师检查时这个程序要能够用键盘自动退出执行，并且
输出端口的电压要清零，使比例阀开度为0，实现如下：
while(1)中加入一个kbhit()的键盘输入检测（别忘加
头文件）#include "conio.h"
参考如下：
if(kbhit()){
	‘接受键值的变量’ = getch();
	if(接受键值的变量 == '键盘字母') {
		ptAOVoltageOut.OutputValue = 0; //输出设定
        DRV_AOVoltageOut(DriverHandle,(LPT_AOVoltageOut)&ptAOVoltageOut); //输出电压清零	
		break;
	}
}
*****************************************/




#include <stdio.h>
#include <windows.h>
#include "driver.h"    //实验室电脑带有驱动的文件,具体函数API查阅老师下发的USB-4711A文档.

void main()
{
    float advalue0,advalue1;
    float davalue;
    float target;
    float error[3]={0};
    float TI=0,TD=0,Kc=1,Ts=0.1; //自己设定
    float KI=((KC*Ts)/TI),KD=((KC*TD)/Ts);
    DWORD ErrCde;
    ULONG DevNum=0;
    long DriverHandle;
    float delta_u;
    float u=0; //初始输出控制量为0
    PT_AIConfig ptAIConfig0,ptAIConfig1;//配置输入
    PT_AIVoltageIn ptAIVoltageIn0,ptAIVoltageIn1;
    PT_AOConfig ptAOConfig;//配置输出
    PT_AOVoltageOut ptAOVoltageOut;
    
    /*************
     * 初始化设备信息
     * 设备使用前必须先开启,使用结束后应当进行关闭.
     **************/
    ErrCde = DRV_DeviceOpen(DevNum,&DriverHandle); //打开设备,然后进行试验.
    if(ErrCde==SUCCESS) //设备打开成功
    {
        printf("设备打开成功!\n");
    }
     /**
     * 配置模拟口输入模拟口输出,液位传感器和温度传感器输出均为模拟量,并使用
     * 模拟量输出来进行比例阀的控制.接下来是接口分配
     * AI0 液位传感器
     * AI1 温度传感器
     * AO0 比例阀控制信号
     **/
  
    //配置输入口
    ptAIConfig0.DasChan=0;
    ptAIConfig0.DasGain=4;//-10~+10 V
    DRV_AIConfig(DriverHandle,(LPT_AIConfig)&ptAIConfig0);  //AI0配置

    ptAIConfig1.DasChan=1;
    ptAIConfig1.DasGain=4;//-10~+10 V
    DRV_AIConfig(DriverHandle,(LPT_AIConfig)&ptAIConfig1);  //AI1配置

    //配置输出口
    ptAOConfig.chan=0;
    ptAOConfig.RefSrc=0;
    ptAOConfig.MaxValue=10;
    ptAOConfig,MinValue=0;
    DRV_AOConfig(DriverHandle,(LPT_AOConfig)&ptAOConfig);
//这里应该加入循环的延时,TS为一个采样周期
    //读取AI1,AI0的输入电压
    ptAIVoltageIn0.chan=0;
    ptAIVoltageIn0.gain=4;// 输入范围,同输入配置
    ptAIVoltageIn0.TrigMode=0;//内部触发
    ptAIVoltageIn0.voltage=(FLOAT far *)&advalue0; //float far win32 
    DRV_AIVoltageIn(DriverHandle,(LPT_AIVoltageIn)&ptAIVoltageIn0); 

    ptAIVoltageIn1.chan=1;
    ptAIVoltageIn1.gain=4;// 输入范围,同输入配置
    ptAIVoltageIn1.TrigMode=0;//内部触发
    ptAIVoltageIn1.voltage=(FLOAT far *)&advalue1; //float far win32 
    DRV_AIVoltageIn(DriverHandle,(LPT_AIVoltageIn)&ptAIVoltageIn1);  //测量函数

    ptAOVoltageOut.chan=0;
    ptAOVoltageOut.OutputValue=davalue;
    DRV_AOVoltageOut(DriverHandle,(LPT_AOVoltageOut)&ptAOVoltageOut); 

    
    printf("输入水箱液位的设定值:");
    scanf("%d",&target);
    /**
     * PID算法计算控制量  0为当前时刻,1为上一时刻;
     **/
    while(1) //可以加入printf,显示目标水位,当前水位(换算),以及控制量的值
    {
        DRV_AIVoltageIn(DriverHandle,(LPT_AIVoltageIn)&ptAIVoltageIn0);  //测量当前液位
        error[2]=error[1];
        error[1]=error[0];
        error[0]=target-advalue0;
        delta_u=Kc*(error[0]-error[1])+KI*error[0]+KD*(error[0]-2*error[1]+error[2]); //增量式PID算法
        u=u+delta_u;//计算输出的控制量
        ptAOVoltageOut.OutputValue=u;
        DRV_AOVoltageOut(DriverHandle,(LPT_AOVoltageOut)&ptAOVoltageOut);  //输出控制量
        //疑惑怎么加入采样时间emm
        delay(Ts);
    }

    ErrCde = DRV_DeviceClose(DevNum,&DriverHandle);
    if(ErrCde==SUCCESS) //设备关闭成功
    {
        printf("设备关闭成功!\n"); //记得清零控制量
    }
}

