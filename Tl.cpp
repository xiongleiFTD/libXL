
/******获取网络时间********
需导入#include <afxsock.h>

*************************/
void getNetworkTime()
{
	//时间同步
	CSocket sockClient;
	TIME_ZONE_INFORMATION tzinfo;
	ZeroMemory(&tzinfo,sizeof(tzinfo));
	DWORD dwStandardDaylight;
	long sminute,shour;
	double bias;
	vector <string> vi;
	string strServer;
	BOOL ret;
	//将时间服务器放在一个VECTOR中。
	strServer = "time.nist.gov";
	vi.push_back(strServer); 
	strServer = "time-a.nist.gov";
	vi.push_back(strServer); 
	strServer = "time-b.nist.gov";
	vi.push_back(strServer); 
	strServer = "time-nw.nist.gov";
	vi.push_back(strServer); 
	strServer = "nist1.nyc.certifiedtime.com";
	vi.push_back(strServer); 
	strServer = "time-b.nist.gov";
	vi.push_back(strServer); 

	//初始化CSocket
	AfxSocketInit();
	sockClient.Create();   //创建socket
	//UpdateData(FALSE); 

	dwStandardDaylight = GetTimeZoneInformation(&tzinfo); //获取时区与UTC的时间差 应该返回-8
	bias = tzinfo.Bias;
	if (dwStandardDaylight == TIME_ZONE_ID_INVALID) //函数执行失败
	{
		printf("函数执行失败\n");
		return; 
	}
	if (dwStandardDaylight == TIME_ZONE_ID_STANDARD) //标准时间有效
		bias += tzinfo.StandardBias;

	if (dwStandardDaylight == TIME_ZONE_ID_DAYLIGHT) //夏令时间
		bias += tzinfo.DaylightBias;

	shour = bias/60;
	sminute = fmod(bias,60);
	bool bo =false;
	//循环判断服务器是否连接成功
	for (int n=0; n < vi.size(); ++ n)
	{
		ret = sockClient.Connect(vi[n].c_str(),13);
		if (1 == ret)
		{
			bo=true;	
			break;   
		}

	}
	if (bo)			printf("链接成功\n");
	else 	printf("链接失败\n");
	unsigned char nTime[40];   //临时接收数据要求足够的大
	memset(nTime, 0, sizeof(nTime));

	sockClient.Receive(nTime, sizeof(nTime)); //接收服务器发送来得的数据
	sockClient.Close();    //关闭socket

	CString strTime;
	strTime.Format("%s",nTime);

	int first=strTime.Find("-");
	int second=strTime.Find("-",first+1);

	int tfirst = strTime.Find(":");
	int tsecond = strTime.Find(":",tfirst+1);

	//printf("年 =%d\n",atoi(strTime.Mid(first-2,2)));
	int hyear=2000+atoi(strTime.Mid(first-2,2));

	//printf("月 =%d\n",atoi(strTime.Mid(first+1,2)));
	int hmonth=atoi(strTime.Mid(first+1,2));

	//printf("日 =%d\n",atoi(strTime.Mid(second+1,2)));
	int hday=atoi(strTime.Mid(second+1,2));

	//printf("时 =%d\n",atoi(strTime.Mid(tfirst-2,2))-shour);
	int hhour=atoi(strTime.Mid(tfirst-2,2))-shour;

	//printf("分 =%d\n",atoi(strTime.Mid(tfirst+1,2))-sminute);
	int hminute=atoi(strTime.Mid(tfirst+1,2))-sminute;

	//printf("秒 =%d\n",atoi(strTime.Mid(tsecond+1,2)));
	int hsecond=atoi(strTime.Mid(tsecond+1,2));
	CTime temptime(hyear,hmonth,hday,hhour,hminute,hsecond);
	//CString szTime = temptime.Format("%Y-%m-%d %H:%M:%S"); 
	//printf(szTime+"\n");
	//设置系统时间
	//SYSTEMTIME systm;
	//systm.wYear = temptime.GetYear();
	//systm.wMonth = temptime.GetMonth();
	//systm.wDay = temptime.GetDay();
	//systm.wHour = temptime.GetHour();
	//systm.wMinute = temptime.GetMinute();
	//systm.wSecond = temptime.GetSecond();
	//systm.wMilliseconds = 0;

	//if(0 != ::SetLocalTime(&systm))
	//{
	//	//UpdateData(FALSE);
	//	AfxMessageBox("本地时间设置成功！！！");
	//}
	//else
	//{
	//	//UpdateData(FALSE);
	//	AfxMessageBox("本地时间设置失败！！！");
	//}


}
/****************************************
tm_year 从1900年计算，所以要加1900， 
月tm_mon，从0计算，所以要加1

time( &nowtime );  获取时间
localtime ( &nowtime ); 转为当地时间 
*****************************************/
//获取系统时间
void getSysTime()
{
	time_t _time = time(NULL);
	struct tm  _now = *localtime(&_time);

	//year = _now.tm_year + 1900;
	//month = _now.tm_mon + 1;
	//day = _now.tm_mday;
	//hour= _now.tm_hour;
	//min = _now.tm_min;
	//sec = _now.tm_sec;


	//1.使用CTime类 

	CTime tm=CTime::GetCurrentTime(); 

	CString str=tm.Format("现在时间是:%Y年%m月%d日 %X"); 



	//2: 得到系统时间日期(使用GetLocalTime) 

	//SYSTEMTIME st; 

	//CString strDate,strTime; 

	//GetLocalTime(&st); 

	//strDate.Format("%4d-%2d-%2d",st.wYear,st.wMonth,st.wDay); 

	//strTime.Format("%2d:%2d:%2d",st.wHour,st.wMinute,st.wSecond); 

	////3.使用GetTickCount//获取程序运行时间 

	//long t1=GetTickCount();//程序段开始前取得系统运行时间(ms) 

	////。。。//程序段 

	//long t2=GetTickCount();//程序段结束后取得系统运行时间(ms) 

	//long t = t2-t1; //前后之差即 程序运行时间 (ms) 


}
/*************************************
getWangKaInfo函数功能获得网卡信息
//需导
#include"iphlpapi.h"
#pragma comment(lib,"Iphlpapi.lib")
************************************/
void CGameClientDlg::getWangKaInfo()
{

	PIP_ADAPTER_INFO pAdapterInfo;

	PIP_ADAPTER_INFO pAdapter=NULL;
	DWORD dwRetVal=0;
	pAdapterInfo=(IP_ADAPTER_INFO*)malloc(sizeof(IP_ADAPTER_INFO));
	ULONG ulOutBufLen=sizeof(IP_ADAPTER_INFO);
	if(GetAdaptersInfo(pAdapterInfo,&ulOutBufLen)!=ERROR_SUCCESS)
	{	
		GlobalFree(pAdapterInfo);
		pAdapterInfo=(IP_ADAPTER_INFO*)malloc(ulOutBufLen);
	}

	if((dwRetVal=GetAdaptersInfo(pAdapterInfo,&ulOutBufLen))==NO_ERROR)
	{	
		pAdapter=pAdapterInfo;
		while(pAdapter)
		{
			printf("Adapter Name: \t%s\n", pAdapter->AdapterName);
			printf("Adapter Desc: \t%s\n", pAdapter->Description);
			printf("MAC Addr: \t%02x-%02x-%02x-%02x-%02x-%02x\n",pAdapter->Address[0], pAdapter->Address[1], pAdapter->Address[2], 
				pAdapter->Address[3], pAdapter->Address[4], pAdapter->Address[5]);
			printf("IP Address: \t%s\n", pAdapter->IpAddressList.IpAddress.String);
			printf("IP Mask: \t%s\n", pAdapter->IpAddressList.IpMask.String);
			printf("Gateway: \t%s\n", pAdapter->GatewayList.IpAddress.String); 
			pAdapter=pAdapter->Next;
		}
	}
	else
	{
		printf("CalltoGetAdaptersInfofailed.\n");
	}
	return ;
}
/****************************************
FindProcess函数功能获得系统内所有在运行的进程
可以根据自己需要关闭指定进程
需导头文件
#include "windows.h"　　
#include "tlhelp32.h"
****************************************/
void FindProcess()
{
	PROCESSENTRY32 pe32;
	//在使用这个结构前，先设置它的大小
	pe32.dwSize = sizeof(pe32);
	//给系统内所有的进程拍个快照
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		printf("CreateToolhelp32Snapshot 调用失败.\n");
	}
	//遍历进程快照，轮流显示每个进程的信息
	BOOL bMore = ::Process32First(hProcessSnap,&pe32);
	while (bMore)
	{
		//printf("进程名称：%s\n",pe32.szExeFile);
		//printf("进程ID：%u\n\n",pe32.th32ProcessID);
		//关闭指定进程，不需要的可以注释
		if (stricmp(pe32.szExeFile,"sqlcmd.exe") == 0)
		{
			//printf("找到指定进程\n");
			HANDLE hProcess=OpenProcess(PROCESS_TERMINATE, FALSE, pe32.th32ProcessID);
			if(hProcess)
			{
				//printf("---------句柄获取成功\n");
				if(TerminateProcess(hProcess,0) == false)
				{
					AfxMessageBox("进程关闭失败");
				}

			}
		}
		bMore = ::Process32Next(hProcessSnap,&pe32);	
	}
	//不要忘记清除掉snapshot对象	　　
	//关闭快照不能调用CloseHandle
	::CloseHandle(hProcessSnap);

}