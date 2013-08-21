
/******��ȡ����ʱ��********
�赼��#include <afxsock.h>

*************************/
void getNetworkTime()
{
	//ʱ��ͬ��
	CSocket sockClient;
	TIME_ZONE_INFORMATION tzinfo;
	ZeroMemory(&tzinfo,sizeof(tzinfo));
	DWORD dwStandardDaylight;
	long sminute,shour;
	double bias;
	vector <string> vi;
	string strServer;
	BOOL ret;
	//��ʱ�����������һ��VECTOR�С�
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

	//��ʼ��CSocket
	AfxSocketInit();
	sockClient.Create();   //����socket
	//UpdateData(FALSE); 

	dwStandardDaylight = GetTimeZoneInformation(&tzinfo); //��ȡʱ����UTC��ʱ��� Ӧ�÷���-8
	bias = tzinfo.Bias;
	if (dwStandardDaylight == TIME_ZONE_ID_INVALID) //����ִ��ʧ��
	{
		printf("����ִ��ʧ��\n");
		return; 
	}
	if (dwStandardDaylight == TIME_ZONE_ID_STANDARD) //��׼ʱ����Ч
		bias += tzinfo.StandardBias;

	if (dwStandardDaylight == TIME_ZONE_ID_DAYLIGHT) //����ʱ��
		bias += tzinfo.DaylightBias;

	shour = bias/60;
	sminute = fmod(bias,60);
	bool bo =false;
	//ѭ���жϷ������Ƿ����ӳɹ�
	for (int n=0; n < vi.size(); ++ n)
	{
		ret = sockClient.Connect(vi[n].c_str(),13);
		if (1 == ret)
		{
			bo=true;	
			break;   
		}

	}
	if (bo)			printf("���ӳɹ�\n");
	else 	printf("����ʧ��\n");
	unsigned char nTime[40];   //��ʱ��������Ҫ���㹻�Ĵ�
	memset(nTime, 0, sizeof(nTime));

	sockClient.Receive(nTime, sizeof(nTime)); //���շ������������õ�����
	sockClient.Close();    //�ر�socket

	CString strTime;
	strTime.Format("%s",nTime);

	int first=strTime.Find("-");
	int second=strTime.Find("-",first+1);

	int tfirst = strTime.Find(":");
	int tsecond = strTime.Find(":",tfirst+1);

	//printf("�� =%d\n",atoi(strTime.Mid(first-2,2)));
	int hyear=2000+atoi(strTime.Mid(first-2,2));

	//printf("�� =%d\n",atoi(strTime.Mid(first+1,2)));
	int hmonth=atoi(strTime.Mid(first+1,2));

	//printf("�� =%d\n",atoi(strTime.Mid(second+1,2)));
	int hday=atoi(strTime.Mid(second+1,2));

	//printf("ʱ =%d\n",atoi(strTime.Mid(tfirst-2,2))-shour);
	int hhour=atoi(strTime.Mid(tfirst-2,2))-shour;

	//printf("�� =%d\n",atoi(strTime.Mid(tfirst+1,2))-sminute);
	int hminute=atoi(strTime.Mid(tfirst+1,2))-sminute;

	//printf("�� =%d\n",atoi(strTime.Mid(tsecond+1,2)));
	int hsecond=atoi(strTime.Mid(tsecond+1,2));
	CTime temptime(hyear,hmonth,hday,hhour,hminute,hsecond);
	//CString szTime = temptime.Format("%Y-%m-%d %H:%M:%S"); 
	//printf(szTime+"\n");
	//����ϵͳʱ��
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
	//	AfxMessageBox("����ʱ�����óɹ�������");
	//}
	//else
	//{
	//	//UpdateData(FALSE);
	//	AfxMessageBox("����ʱ������ʧ�ܣ�����");
	//}


}
/****************************************
tm_year ��1900����㣬����Ҫ��1900�� 
��tm_mon����0���㣬����Ҫ��1

time( &nowtime );  ��ȡʱ��
localtime ( &nowtime ); תΪ����ʱ�� 
*****************************************/
//��ȡϵͳʱ��
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


	//1.ʹ��CTime�� 

	CTime tm=CTime::GetCurrentTime(); 

	CString str=tm.Format("����ʱ����:%Y��%m��%d�� %X"); 



	//2: �õ�ϵͳʱ������(ʹ��GetLocalTime) 

	//SYSTEMTIME st; 

	//CString strDate,strTime; 

	//GetLocalTime(&st); 

	//strDate.Format("%4d-%2d-%2d",st.wYear,st.wMonth,st.wDay); 

	//strTime.Format("%2d:%2d:%2d",st.wHour,st.wMinute,st.wSecond); 

	////3.ʹ��GetTickCount//��ȡ��������ʱ�� 

	//long t1=GetTickCount();//����ο�ʼǰȡ��ϵͳ����ʱ��(ms) 

	////������//����� 

	//long t2=GetTickCount();//����ν�����ȡ��ϵͳ����ʱ��(ms) 

	//long t = t2-t1; //ǰ��֮� ��������ʱ�� (ms) 


}
/*************************************
getWangKaInfo�������ܻ��������Ϣ
//�赼
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
FindProcess�������ܻ��ϵͳ�����������еĽ���
���Ը����Լ���Ҫ�ر�ָ������
�赼ͷ�ļ�
#include "windows.h"����
#include "tlhelp32.h"
****************************************/
void FindProcess()
{
	PROCESSENTRY32 pe32;
	//��ʹ������ṹǰ�����������Ĵ�С
	pe32.dwSize = sizeof(pe32);
	//��ϵͳ�����еĽ����ĸ�����
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		printf("CreateToolhelp32Snapshot ����ʧ��.\n");
	}
	//�������̿��գ�������ʾÿ�����̵���Ϣ
	BOOL bMore = ::Process32First(hProcessSnap,&pe32);
	while (bMore)
	{
		//printf("�������ƣ�%s\n",pe32.szExeFile);
		//printf("����ID��%u\n\n",pe32.th32ProcessID);
		//�ر�ָ�����̣�����Ҫ�Ŀ���ע��
		if (stricmp(pe32.szExeFile,"sqlcmd.exe") == 0)
		{
			//printf("�ҵ�ָ������\n");
			HANDLE hProcess=OpenProcess(PROCESS_TERMINATE, FALSE, pe32.th32ProcessID);
			if(hProcess)
			{
				//printf("---------�����ȡ�ɹ�\n");
				if(TerminateProcess(hProcess,0) == false)
				{
					AfxMessageBox("���̹ر�ʧ��");
				}

			}
		}
		bMore = ::Process32Next(hProcessSnap,&pe32);	
	}
	//��Ҫ���������snapshot����	����
	//�رտ��ղ��ܵ���CloseHandle
	::CloseHandle(hProcessSnap);

}