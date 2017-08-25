// Application_Faro_TrakerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Application_Faro_Traker.h"
#include "Application_Faro_TrakerDlg.h"
#include "tracker.h"
#include "math.h"
#include "time.h"
#include "windows.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstdio>
#include <cctype>
// ********************************************************
// Les librairies
// ********************************************************
#pragma comment(lib,"ws2_32.lib")
#define MY_WM_MESSAGE1  (WM_APP + 1000)

// ********************************************************
// Définition des variables
// ********************************************************
WSADATA initialisation_win32; // Variable permettant de récupérer la structure d'information sur l'initialisation
int erreur; // Variable permettant de récupérer la valeur de retour des fonctions utilisées
int tempo; // Variable temporaire de type int
int nombre_de_caractere; // Indique le nombre de caractères qui a été reçu ou envoyé
char buffer[500]; // Tampon contenant les données reçues ou envoyées
SOCKET id_de_la_socket; // Identifiant de la socket
SOCKET id_de_la_nouvelle_socket; // Identifiant de la nouvelle socket
SOCKADDR_IN information_sur_la_source; // Déclaration de la structure des informations lié à l'écoute

using namespace std;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

Tracker* gpTracker;
FILE *hFile;
////////////////
int imad1;
int imad2;
int imad3;
int imad4;
int imad5;
int imad6;
int imad12;
int imad22;
int imad32;
int imad42;
int imad52;
int imad62;
////////////////
int imaad1;
int imaad2;
int imaad3;
int imaad4;
int imaad5;
int imaad6;
int imaad12;
int imaad22;
int imaad32;
int imaad42;
int imaad52;
int imaad62;
int take_meas;
int START_SAND_RECEV;
int STOP_DISPLAY;
///////////////////
CString ContinTrigg;
CString times;

// Class Point/////////////////////////////////////////////////////////////////

class Point
{
public:
	double X;
	double Y;
	double Z;
	CString SMR;
	CString Measurement_Mode;
	CString Measurement_Mechanism;
	CString filtter;
	CString Star_tig;
	CString Cont1_trig;
	CString Cont2_trig;
	CString Cont3_trig;
	CString Time;	
	int samperobs;

	Point();
};

Point::Point()
{
	 X=0;
	 Y=0;
	 Z=0;
	 SMR="";
	 Measurement_Mode="";
	 Measurement_Mechanism="";
	 filtter="";
	 Star_tig="";
	 Cont1_trig="";
	 Cont2_trig="";
	 Cont3_trig="";
	 Time="";	
	 samperobs=-1;
}
class Points
{
public:
	double XP;
	double YP;
	double ZP;
	Points();
};

Points::Points()
{
	 XP=0;
	 YP=0;
	 ZP=0;
}




CString PT_NAME = "";
CString SMR_TYPE = "";
double PARAM3 = 0;
double PARAM4 = 0;
double PARAM5 = 0;


double X3 = 0;
double Y3 = 0;
double Z3 = 0;

double XM = 0;
double YM = 0;
double ZM = 0;
bool isBacksight;
bool isRelative;

double RADIUS;
double DISTAN;

CString PARAM_HOME = "";
CString PARAM_SMR = "";
double DIAM_SMR = 0;

int CONNECT_TRACKER = 0;
short int M_PORT;

HANDLE CURENT_APP;



vector <Point> traject;
vector <Points> Collec_Stop;
vector<CString> Inparams;
CString X1;
CString Y1;
CString Z1;
SYSTEMTIME Strtime;
int toto;

int flag_5532[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
int flag_5533[6] = {0,0,0,0,0,0};
int flag_5534[6] = {0,0,0,0,0,0};
int flag_5535[2] = {0,0};
int flag_5536 = 0;



////////////////////////////////////////////////////////////////////////////////

CApplication_Faro_TrakerDlg::CApplication_Faro_TrakerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CApplication_Faro_TrakerDlg::IDD, pParent)
	, m_Ip1(_T("128"))
	, m_Ip2(_T("128"))
	, m_Ip3(_T("128"))
	, m_Ip4(_T("100"))
	, m_Measu(_T(""))
	, m_az(_T(""))
	, m_ze(_T(""))
	, m_di(_T(""))
	, m_x(_T(""))
	, m_y(_T(""))
	, m_z(_T(""))
	, m_InterTrig(_T(""))
	, m_DitTrig(_T(""))
	, m_For_Back_measurement(0)
	, m_ContinueTrig(0)
	, m_Filter(0)
	, m_Smr(0)
	, m_Stop_Collec(0)
	, m_Measure_Mode(0)
	, m_Display_Period(_T(""))
	, m_Norm_Dyn_Meas(0)
	, m_Inter_Tol(_T(""))
	, m_check_stop_collec(0)
	, m_Dist_of_Measu(_T(""))
	, m_Period_Of_Measu(_T(""))
	, m_Ip5(_T(""))
	, m_Ip6(_T(""))
	, m_Ip7(_T(""))
	, m_Ip8(_T(""))
    , m_Port(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CApplication_Faro_TrakerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_IP1, m_Ip1);
    DDX_Text(pDX, IDC_EDIT_IP2, m_Ip2);
    DDX_Text(pDX, IDC_EDIT_IP3, m_Ip3);
    DDX_Text(pDX, IDC_EDIT_IP4, m_Ip4);
    DDX_Text(pDX, IDC_EDIT_MEASU, m_Measu);
    DDX_Text(pDX, IDC_EDIT_X, m_x);
    DDX_Text(pDX, IDC_EDIT_Y, m_y);
    DDX_Text(pDX, IDC_EDIT_Z, m_z);
    DDX_Text(pDX, IDC_EDIT_INTTRIG, m_InterTrig);
    DDX_Text(pDX, IDC_EDIT_DISTTRIG, m_DitTrig);
    DDX_Control(pDX, IDC_LIST_MEASURE, m_listeofmeasur);
    DDX_Control(pDX, IDC_BUTTON_CONNECT, m_Button_Connect);
    DDX_Control(pDX, IDC_BUTTON_DISCONNECT, m_Button_Disconnect);
    DDX_Control(pDX, IDC_BUTTON_TAKMEAS, m_Takmeas);
    DDX_Control(pDX, IDC_BUTTON_STATRTMEAS, m_Button_Startmeas);
    DDX_Control(pDX, IDC_BUTTON_STOPMEAS, m_Button_Stopmeas);
    DDX_Control(pDX, IDC_BUTTON_EXPORT, m_Export);
    DDX_Radio(pDX, IDC_RADIO_FRGNDMEAS, m_For_Back_measurement);
    DDX_Radio(pDX, IDC_RADIO_AVERAGEFILTER, m_Filter);
    DDX_Radio(pDX, IDC_RADIO_0_5, m_Smr);
    DDX_Radio(pDX, IDC_NORM_DYN_MEAS, m_Norm_Dyn_Meas);
    DDX_Radio(pDX, IDC_RADIO_NULLCONTTRIGGER, m_ContinueTrig);
    DDX_Text(pDX, IDC_EDIT_DISPL_PERIOD, m_Display_Period);
    DDX_Text(pDX, IDC_EDIT_TOL_INTERV, m_Inter_Tol);
    DDX_Check(pDX, IDC_CHECK_STOP_COLLEC, m_check_stop_collec);
    DDX_Text(pDX, IDC_EDIT_DIST_OF_MEASU, m_Dist_of_Measu);
    DDX_Text(pDX, IDC_EDIT_PERIOD_OF_MEASU, m_Period_Of_Measu);
    DDX_Text(pDX, IDC_EDIT_IP5, m_Ip5);
    DDX_Text(pDX, IDC_EDIT_IP6, m_Ip6);
    DDX_Text(pDX, IDC_EDIT_IP7, m_Ip7);
    DDX_Text(pDX, IDC_EDIT_IP8, m_Ip8);
    DDX_Control(pDX, IDC_LIST_V7000_Connexion, m_List_V7000_Connexion);
    DDX_Text(pDX, IDC_EDIT_PORT, m_Port);
}

BEGIN_MESSAGE_MAP(CApplication_Faro_TrakerDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(IDC_EDIT_IP1, &CApplication_Faro_TrakerDlg::OnEnChangeEditIp1)
	ON_EN_CHANGE(IDC_EDIT_IP2, &CApplication_Faro_TrakerDlg::OnEnChangeEditIp2)
	ON_EN_CHANGE(IDC_EDIT_IP3, &CApplication_Faro_TrakerDlg::OnEnChangeEditIp3)
	ON_EN_CHANGE(IDC_EDIT_IP4, &CApplication_Faro_TrakerDlg::OnEnChangeEditIp4)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CApplication_Faro_TrakerDlg::OnBnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_STATRTMEAS, &CApplication_Faro_TrakerDlg::OnBnClickedButtonStatrtmeas)
	ON_BN_CLICKED(IDC_BUTTON_STOPMEAS, &CApplication_Faro_TrakerDlg::OnBnClickedButtonStopmeas)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, &CApplication_Faro_TrakerDlg::OnBnClickedButtonExport)
	ON_BN_CLICKED(IDC_BUTTON_TAKMEAS, &CApplication_Faro_TrakerDlg::OnBnClickedButtonTakmeas)
	ON_BN_CLICKED(IDC_RADIO_INTERVALTRIGGER, &CApplication_Faro_TrakerDlg::OnBnClickedRadioIntervaltrigger)
	ON_BN_CLICKED(IDC_BUTTON_DISCONNECT, &CApplication_Faro_TrakerDlg::OnBnClickedButtonDisconnect)
	ON_EN_CHANGE(IDC_EDIT_IP5, &CApplication_Faro_TrakerDlg::OnEnChangeEditIp5)
	ON_EN_CHANGE(IDC_EDIT_IP6, &CApplication_Faro_TrakerDlg::OnEnChangeEditIp6)
	ON_EN_CHANGE(IDC_EDIT_IP7, &CApplication_Faro_TrakerDlg::OnEnChangeEditIp7)
	ON_EN_CHANGE(IDC_EDIT_IP8, &CApplication_Faro_TrakerDlg::OnEnChangeEditIp8)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT_V7000, &CApplication_Faro_TrakerDlg::OnBnClickedButtonConnectV7000)
	ON_MESSAGE(MY_WM_MESSAGE1, OnMyMessage1)
    ON_BN_CLICKED(IDC_RADIO_NULLCONTTRIGGER, &CApplication_Faro_TrakerDlg::OnBnClickedRadioNullconttrigger)
    ON_BN_CLICKED(IDC_RADIO_DISTANCE_TRIGGER, &CApplication_Faro_TrakerDlg::OnBnClickedRadioDistanceTrigger)
    ON_BN_CLICKED(IDC_BUTTON_CLEAR_LIST, &CApplication_Faro_TrakerDlg::OnBnClickedButtonClearList)
    ON_BN_CLICKED(IDC_BUTTON_DISCONNECT_V7000, &CApplication_Faro_TrakerDlg::OnBnClickedButtonDisconnectV7000)
END_MESSAGE_MAP()

// CApplication_Faro_TrakerDlg message handlers

DWORD WINAPI MyThreadConnectSimul( LPVOID pParam )
{
  CApplication_Faro_TrakerDlg* pConnectThreadSimul = (CApplication_Faro_TrakerDlg*)pParam;	
  CString ip = pConnectThreadSimul->m_Ip1+"."+pConnectThreadSimul->m_Ip2+"."+pConnectThreadSimul->m_Ip3+"."+pConnectThreadSimul->m_Ip4;
////////////////////////////////////////////////////////////////////
  char *Ip = (LPTSTR) (LPCTSTR)ip;
////////////////////////////////////////////////////////////////////
  try
  {
	  gpTracker = new Tracker("TrackerKeystoneSim");
	  gpTracker->setBlocking(true);
	  if (gpTracker->connected() == false)
	  {
		  try
		  {
			  gpTracker->connect(Ip,"user","");
			  gpTracker->startApplicationFrame("SimulatorAPI","");
		  }
		  catch(TrackerException* e )
		  {
			  char* msg;
		      msg = e->getText();
		      delete e;
			  strcpy_s(buffer,msg); // Copie la chaine de caractère dans buffer
              nombre_de_caractere=send(id_de_la_nouvelle_socket,buffer,strlen(buffer),0);
              pConnectThreadSimul->SendERROR("5530","1101");
		  }
	  }
      else
      {
           pConnectThreadSimul->SendERROR("5530","SIMULATOR_IS_ALREADY_CONNECTED");
      }
	  if (gpTracker->initialized()==false)
	  {
		  gpTracker->initialize(true);
	  }
	  strcpy_s(buffer,"#5530[Simulator;Connected]"); // Copie la chaine de caractère dans buffer
      nombre_de_caractere=send(id_de_la_nouvelle_socket,buffer,strlen(buffer),0);
      pConnectThreadSimul->GetDlgItem(IDC_RADIO_AVERAGEFILTER)->EnableWindow(TRUE);
	  pConnectThreadSimul->GetDlgItem(IDC_RADIO_STDDEVFILTER)->EnableWindow(TRUE);
	  pConnectThreadSimul->GetDlgItem(IDC_RADIO_NULLCONTTRIGGER)->EnableWindow(TRUE);
      pConnectThreadSimul->GetDlgItem(IDC_RADIO_INTERVALTRIGGER)->EnableWindow(TRUE);
      pConnectThreadSimul->GetDlgItem(IDC_RADIO_DISTANCE_TRIGGER)->EnableWindow(TRUE);
	  pConnectThreadSimul->GetDlgItem(IDC_EDIT_INTTRIG)->EnableWindow(TRUE);
	  pConnectThreadSimul->GetDlgItem(IDC_EDIT_DISTTRIG)->EnableWindow(TRUE);
      pConnectThreadSimul->GetDlgItem(IDC_EDIT_DISPL_PERIOD)->EnableWindow(TRUE);
      pConnectThreadSimul->GetDlgItem(IDC_EDIT_MEASU)->EnableWindow(TRUE);
      pConnectThreadSimul->GetDlgItem(IDC_BUTTON_STATRTMEAS)->EnableWindow(TRUE);
      pConnectThreadSimul->GetDlgItem(IDC_BUTTON_TAKMEAS)->EnableWindow(TRUE);
      pConnectThreadSimul ->GetDlgItem(IDC_RADIO_0_5)->EnableWindow(TRUE);
	  pConnectThreadSimul ->GetDlgItem(IDC_RADIO_0_875)->EnableWindow(TRUE);
	  pConnectThreadSimul ->GetDlgItem(IDC_RADIO_1_5)->EnableWindow(TRUE);
      pConnectThreadSimul ->GetDlgItem(IDC_BUTTON_STATRTMEAS)->EnableWindow(TRUE);
      CONNECT_TRACKER = 1;
  }
  catch(TrackerException* e )
  {
	  char* msg;
	  msg = e->getText();
	  delete e;
	  strcpy_s(buffer,msg); // Copie la chaine de caractère dans buffer
      nombre_de_caractere=send(id_de_la_nouvelle_socket,buffer,strlen(buffer),0);
      pConnectThreadSimul->SendERROR("5530","1100");
  }
  ExitThread(0); 
  return 0;
}

DWORD WINAPI MyThreadConnectTracker( LPVOID pParam )
{
  CApplication_Faro_TrakerDlg* pConnectThreadTracker = (CApplication_Faro_TrakerDlg*)pParam;	
  CString ip = pConnectThreadTracker->m_Ip1+"."+pConnectThreadTracker->m_Ip2+"."+pConnectThreadTracker->m_Ip3+"."+pConnectThreadTracker->m_Ip4;
////////////////////////////////////////////////////////////////////
  char *Ip = (LPTSTR) (LPCTSTR)ip;
////////////////////////////////////////////////////////////////////
  try
  {
	  gpTracker = new Tracker("TrackerKeystone");
	  gpTracker->setBlocking(true);
	  if (gpTracker->connected() == false)
	  {
		  try
		  {
              gpTracker->connect(Ip,"user","");
		  }
		  catch(TrackerException* e )
		  {
			  char* msg;
		      msg = e->getText();
		      delete e;
			  strcpy_s(buffer,msg); // Copie la chaine de caractère dans buffer
              nombre_de_caractere=send(id_de_la_nouvelle_socket,buffer,strlen(buffer),0);
			  pConnectThreadTracker->SendERROR("5530","1101");
		  }
	  }
      else
      {
          pConnectThreadTracker->SendERROR("5530","TRACKER_IS_ALREADY_CONNECTED");
      }
	  if (gpTracker->initialized()==false)
	  {
		  gpTracker->initialize(true);
          gpTracker->home(true);
	  }
	  strcpy_s(buffer,"#5530[Tracker;Connected]"); // Copie la chaine de caractère dans buffer
      nombre_de_caractere=send(id_de_la_nouvelle_socket,buffer,strlen(buffer),0);	
      pConnectThreadTracker->GetDlgItem(IDC_RADIO_AVERAGEFILTER)->EnableWindow(TRUE);
	  pConnectThreadTracker->GetDlgItem(IDC_RADIO_STDDEVFILTER)->EnableWindow(TRUE);
	  pConnectThreadTracker->GetDlgItem(IDC_RADIO_NULLCONTTRIGGER)->EnableWindow(TRUE);
      pConnectThreadTracker->GetDlgItem(IDC_RADIO_INTERVALTRIGGER)->EnableWindow(TRUE);
      pConnectThreadTracker->GetDlgItem(IDC_RADIO_DISTANCE_TRIGGER)->EnableWindow(TRUE);
	  pConnectThreadTracker->GetDlgItem(IDC_EDIT_INTTRIG)->EnableWindow(TRUE);
	  pConnectThreadTracker->GetDlgItem(IDC_EDIT_DISTTRIG)->EnableWindow(TRUE);
      pConnectThreadTracker->GetDlgItem(IDC_EDIT_MEASU)->EnableWindow(TRUE);
      pConnectThreadTracker->GetDlgItem(IDC_EDIT_DISPL_PERIOD)->EnableWindow(TRUE);
      pConnectThreadTracker->GetDlgItem(IDC_BUTTON_STATRTMEAS)->EnableWindow(TRUE);
      pConnectThreadTracker->GetDlgItem(IDC_BUTTON_TAKMEAS)->EnableWindow(TRUE);
      pConnectThreadTracker ->GetDlgItem(IDC_RADIO_0_5)->EnableWindow(TRUE);
	  pConnectThreadTracker ->GetDlgItem(IDC_RADIO_0_875)->EnableWindow(TRUE);
	  pConnectThreadTracker ->GetDlgItem(IDC_RADIO_1_5)->EnableWindow(TRUE);
      pConnectThreadTracker ->GetDlgItem(IDC_BUTTON_STATRTMEAS)->EnableWindow(TRUE);
      CONNECT_TRACKER = 1;
  }
  catch(TrackerException* e )
  {
	  char* msg;
	  msg = e->getText();
	  delete e;
	  strcpy_s(buffer,msg); // Copie la chaine de caractère dans buffer
      nombre_de_caractere=send(id_de_la_nouvelle_socket,buffer,strlen(buffer),0);
      pConnectThreadTracker->SendERROR("5530","1100");
  }
  ExitThread(0); 
  return 0;
}

DWORD WINAPI MyThreadDisconnectTracker( LPVOID pParam )
{
    CApplication_Faro_TrakerDlg* pDisconnectThreadTracker = (CApplication_Faro_TrakerDlg*)pParam;
    try
    {
        if (gpTracker->connected()== true)
        {
            gpTracker->setBlocking(false);
            gpTracker->disconnect();
            CONNECT_TRACKER = 0;
            strcpy_s(buffer,"#5530[Tracker;Disconnected]"); // Copie la chaine de caractère dans buffer
            nombre_de_caractere=send(id_de_la_nouvelle_socket,buffer,strlen(buffer),0);    
        }
        else
        {
            pDisconnectThreadTracker->SendERROR("5530","TRACKER_IS_ALREADY_DISCONNECTED");
        }
    }
	catch(TrackerException* e)
	{
        char* msg = "";
        msg = e->getText();
        if(msg != "")
        {
            pDisconnectThreadTracker->SendERROR("5530","1100");
        }
        delete e;
	}
    ExitThread(0); 
    return 0;
}
DWORD WINAPI  MyThreadConnectSND_RCV (LPVOID pParam)
{
	CApplication_Faro_TrakerDlg* pConnectThreadSND_RCV = (CApplication_Faro_TrakerDlg*)pParam;
	pConnectThreadSND_RCV->Send_Recv_Tram();
	return 0;
}
DWORD WINAPI MyThreadDetect_Target( LPVOID pParam )
{
    CApplication_Faro_TrakerDlg* pDetect_Target = (CApplication_Faro_TrakerDlg*)pParam;
    SMXObjectArray* pAppsArray = NULL;
    pAppsArray = gpTracker->startedApplications();
    if( pAppsArray)
	{
		int numApps = pAppsArray->getNumElements();
        while(true)
        {
            if (numApps != 0 && gpTracker->targetPresent()== false)
            {
                //SuspendThread()
            }
        }
	}
return 0;
}
DWORD WINAPI MyThreadConnect_server( LPVOID pParam )
{
    CApplication_Faro_TrakerDlg* pConnect_server = (CApplication_Faro_TrakerDlg*)pParam;
    erreur=99; // Initiation de erreur pour être sur que l'on va rentrer dans la boucle
    while(erreur!=0)// Boucle tant qu'une demande de session (SYN) tcp n'a pas été reçu
	{
		erreur = listen(id_de_la_socket,1);
	}  
	//pConnect_server->m_List_V7000_Connexion.InsertString(0,"listen        : OK");
	//pConnect_server->m_List_V7000_Connexion.UpdateWindow();

	// ********************************************************
	// Acceptation de la demande d'ouverture de session
	// ********************************************************
	//pConnect_server->m_List_V7000_Connexion.InsertString(0,"Pending receipt of request to open a TCP session (SYN)");
	//pConnect_server->m_List_V7000_Connexion.UpdateWindow();
	tempo = sizeof(information_sur_la_source); // Passe par une variable afin d'utiliser un pointeur
	id_de_la_nouvelle_socket = accept(id_de_la_socket,(struct sockaddr*)&information_sur_la_source,&tempo);
	if(id_de_la_nouvelle_socket==INVALID_SOCKET)
	{
		CString WSAGetLastError5;
		WSAGetLastError5.Format("%d", WSAGetLastError());
		//pConnect_server->m_List_V7000_Connexion.InsertString(0,"#9999[5531;2106]");
		//pConnect_server->m_List_V7000_Connexion.UpdateWindow();
	}
	else
	{
		//pConnect_server->m_List_V7000_Connexion.InsertString(0,"accept        : OK");
        //HANDLE m_ConnectThreadSND_RCV;
	    //AfxBeginThread(MyThreadConnectSND_RCV, (LPVOID)GetSafeHwnd());
	    //m_ConnectThreadSND_RCV = CreateThread(NULL,0,MyThreadConnectSND_RCV, (LPVOID)this, 0 ,0);
        START_SAND_RECEV = 1;
	}
    return 0;
}
UINT __cdecl TestMethodThreadCall( LPVOID pParam )
{
	HWND hDlg = (HWND )pParam;
	::SendMessage( hDlg, MY_WM_MESSAGE1, (WPARAM)0, (LPARAM)0);          
	Sleep(5000);
    return 0;
}
int SamPOb;
int Size_Collec_Stop;
double InterTrig;
double DitTrig;
double Distance_Point;
CString smr;
/////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////Normal Dynamic Measurement/////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
//Foreground Measurement//Average Filter//NullContinueTrigger//Non Stop collec///////////////
DWORD WINAPI MyCritariaFrgnd_Measure_Aver_NullCont_Non_Stop( LPVOID pParam )
{
	CApplication_Faro_TrakerDlg* pFrgnd_Measure_Aver_NullCont_Non_Stop = (CApplication_Faro_TrakerDlg*)pParam;
	/////////////////////////////////////////////////////////////////////////////////////
	MeasurePointData* Frgnd_Measure_Aver_NullCont_Non_Stop = NULL;
	AverageFilter filter;
	NullStartTrigger startTrigger;
	/////////////////////////////////////////////////////////////////////////////////////
	NullContinueTrigger continueTrigger;
	/////////////////////////////////////////////////////////////////////////////////////
	MeasureCfg  cfg( pFrgnd_Measure_Aver_NullCont_Non_Stop->SamPOb , &filter , &startTrigger , &continueTrigger);
	gpTracker->startMeasurePoint( &cfg );
	traject.clear();
	 while (true)
	 {
		 Point* Ptpoint;
	     Ptpoint = new Point();
		 try
		 {
			 Frgnd_Measure_Aver_NullCont_Non_Stop = gpTracker->readMeasurePointData();		
	         ASSERT(Frgnd_Measure_Aver_NullCont_Non_Stop);
	         LPARAM lParam = (LPARAM)Frgnd_Measure_Aver_NullCont_Non_Stop;
			 /////////////////////////////////////////////////////////////////////////////
			 pFrgnd_Measure_Aver_NullCont_Non_Stop->az     =     (Frgnd_Measure_Aver_NullCont_Non_Stop->azimuth());
			 pFrgnd_Measure_Aver_NullCont_Non_Stop->ze     =     (Frgnd_Measure_Aver_NullCont_Non_Stop->zenith());    
			 pFrgnd_Measure_Aver_NullCont_Non_Stop->di     =     (Frgnd_Measure_Aver_NullCont_Non_Stop->distance())*1000;
			 /////////////////////////////////////////////////////////////////////////////
			 pFrgnd_Measure_Aver_NullCont_Non_Stop->x      =      pFrgnd_Measure_Aver_NullCont_Non_Stop->di*sin(pFrgnd_Measure_Aver_NullCont_Non_Stop->ze)*cos(pFrgnd_Measure_Aver_NullCont_Non_Stop->az);
			 pFrgnd_Measure_Aver_NullCont_Non_Stop->y      =      pFrgnd_Measure_Aver_NullCont_Non_Stop->di*sin(pFrgnd_Measure_Aver_NullCont_Non_Stop->ze)*sin(pFrgnd_Measure_Aver_NullCont_Non_Stop->az);
			 pFrgnd_Measure_Aver_NullCont_Non_Stop->z      =      pFrgnd_Measure_Aver_NullCont_Non_Stop->di*cos(pFrgnd_Measure_Aver_NullCont_Non_Stop->ze);
			 /////////////////////////////////////////////////////////////////////////////
			 Ptpoint->X = pFrgnd_Measure_Aver_NullCont_Non_Stop->x;
			 Ptpoint->Y = pFrgnd_Measure_Aver_NullCont_Non_Stop->y;
			 Ptpoint->Z = pFrgnd_Measure_Aver_NullCont_Non_Stop->z;
			 /////////////////////////////////////////////////////////////////////////////
			 traject.push_back(*Ptpoint);
			 /////////////////////////////////////////////////////////////////////////////
			 pFrgnd_Measure_Aver_NullCont_Non_Stop->m_x.Format("%g", pFrgnd_Measure_Aver_NullCont_Non_Stop->x);
			 pFrgnd_Measure_Aver_NullCont_Non_Stop->m_y.Format("%g", pFrgnd_Measure_Aver_NullCont_Non_Stop->y);
			 pFrgnd_Measure_Aver_NullCont_Non_Stop->m_z.Format("%g", pFrgnd_Measure_Aver_NullCont_Non_Stop->z);

		 }
		 catch(TrackerException* e )
		 {
			 char* msg;
			 msg = e->getText();
			 AfxMessageBox(msg);
			 delete e;
		 }
		 if (imaad2 == -1)
		 {
			 gpTracker->stopMeasurePoint();
			 break;		 
		 }
	 }
	 ExitThread(0); 
	 return 0;
}

//Foreground Measurement//StdDevFilter//NullContinueTrigger//Non Stop collec/////////////////
DWORD WINAPI MyCritariaFrgnd_Measure_StdD_NullCont_Non_Stop( LPVOID pParam )
{
	CApplication_Faro_TrakerDlg* pFrgnd_Measure_StdD_NullCont_Non_Stop = (CApplication_Faro_TrakerDlg*)pParam;
	/////////////////////////////////////////////////////////////////////////////////////

	MeasurePointData* Frgnd_Measure_StdD_NullCont_Non_Stop = NULL;
	StdDevFilter filter;
	NullStartTrigger startTrigger;
	/////////////////////////////////////////////////////////////////////////////////////
	NullContinueTrigger continueTrigger;
	/////////////////////////////////////////////////////////////////////////////////////
	MeasureCfg  cfg( pFrgnd_Measure_StdD_NullCont_Non_Stop->SamPOb , &filter , &startTrigger , &continueTrigger);
	gpTracker->startMeasurePoint( &cfg );
	traject.clear();
	 while (true)
	 {
		 Point* Ptpoint;
	     Ptpoint = new Point();
		 try
		 {
			 Frgnd_Measure_StdD_NullCont_Non_Stop = gpTracker->readMeasurePointData();		
	         ASSERT(Frgnd_Measure_StdD_NullCont_Non_Stop);
	         LPARAM lParam = (LPARAM)Frgnd_Measure_StdD_NullCont_Non_Stop;
			 /////////////////////////////////////////////////////////////////////////////
			 pFrgnd_Measure_StdD_NullCont_Non_Stop->az     =     (Frgnd_Measure_StdD_NullCont_Non_Stop->azimuth());
			 pFrgnd_Measure_StdD_NullCont_Non_Stop->ze     =     (Frgnd_Measure_StdD_NullCont_Non_Stop->zenith());    
			 pFrgnd_Measure_StdD_NullCont_Non_Stop->di     =     (Frgnd_Measure_StdD_NullCont_Non_Stop->distance())*1000;
			 /////////////////////////////////////////////////////////////////////////////
			 pFrgnd_Measure_StdD_NullCont_Non_Stop->x      =      pFrgnd_Measure_StdD_NullCont_Non_Stop->di*sin(pFrgnd_Measure_StdD_NullCont_Non_Stop->ze)*cos(pFrgnd_Measure_StdD_NullCont_Non_Stop->az);
			 pFrgnd_Measure_StdD_NullCont_Non_Stop->y      =      pFrgnd_Measure_StdD_NullCont_Non_Stop->di*sin(pFrgnd_Measure_StdD_NullCont_Non_Stop->ze)*sin(pFrgnd_Measure_StdD_NullCont_Non_Stop->az);
			 pFrgnd_Measure_StdD_NullCont_Non_Stop->z      =      pFrgnd_Measure_StdD_NullCont_Non_Stop->di*cos(pFrgnd_Measure_StdD_NullCont_Non_Stop->ze);
			 /////////////////////////////////////////////////////////////////////////////
			 Ptpoint->X = pFrgnd_Measure_StdD_NullCont_Non_Stop->x;
			 Ptpoint->Y = pFrgnd_Measure_StdD_NullCont_Non_Stop->y;
			 Ptpoint->Z = pFrgnd_Measure_StdD_NullCont_Non_Stop->z;
			 /////////////////////////////////////////////////////////////////////////////
			 traject.push_back(*Ptpoint);
			 /////////////////////////////////////////////////////////////////////////////
			 pFrgnd_Measure_StdD_NullCont_Non_Stop->m_x.Format("%g", pFrgnd_Measure_StdD_NullCont_Non_Stop->x);
			 pFrgnd_Measure_StdD_NullCont_Non_Stop->m_y.Format("%g", pFrgnd_Measure_StdD_NullCont_Non_Stop->y);
			 pFrgnd_Measure_StdD_NullCont_Non_Stop->m_z.Format("%g", pFrgnd_Measure_StdD_NullCont_Non_Stop->z);
		     
			
		 }
		 catch(TrackerException* e )
		 {
			 char* msg;
			 msg = e->getText();
			 AfxMessageBox(msg);
			 delete e;
		 }
		 if (imaad2 == -1)
		 {
			 gpTracker->stopMeasurePoint();
			 break;
		 }
	 }
	 ExitThread(0); 
	 return 0;
}

//Foreground Measurement//Average Filter//IntervalTrigger//Non Stop collec///////////////////
DWORD WINAPI MyCritariaFrgnd_Measure_Aver_Inter_Non_Stop( LPVOID pParam )
{
	CApplication_Faro_TrakerDlg* pFrgnd_Measure_Aver_Inter_Non_Stop = (CApplication_Faro_TrakerDlg*)pParam;
	/////////////////////////////////////////////////////////////////////////////////////
	MeasurePointData* Frgnd_Measure_Aver_Inter_Non_Stop = NULL;
	AverageFilter filter;
	NullStartTrigger startTrigger;
	/////////////////////////////////////////////////////////////////////////////////////
	IntervalTrigger continueTrigger(pFrgnd_Measure_Aver_Inter_Non_Stop->InterTrig);
	/////////////////////////////////////////////////////////////////////////////////////
	MeasureCfg  cfg( pFrgnd_Measure_Aver_Inter_Non_Stop->SamPOb , &filter , &startTrigger , &continueTrigger);
	gpTracker->startMeasurePoint( &cfg );
	traject.clear();
	 while (true)
	 {
		 Point* Ptpoint;
	     Ptpoint = new Point();
		 try
		 {
			 Frgnd_Measure_Aver_Inter_Non_Stop = gpTracker->readMeasurePointData();		
	         ASSERT(Frgnd_Measure_Aver_Inter_Non_Stop);
	         LPARAM lParam = (LPARAM)Frgnd_Measure_Aver_Inter_Non_Stop;
			 /////////////////////////////////////////////////////////////////////////////
			 pFrgnd_Measure_Aver_Inter_Non_Stop->az     =     (Frgnd_Measure_Aver_Inter_Non_Stop->azimuth());
			 pFrgnd_Measure_Aver_Inter_Non_Stop->ze     =     (Frgnd_Measure_Aver_Inter_Non_Stop->zenith());    
			 pFrgnd_Measure_Aver_Inter_Non_Stop->di     =     (Frgnd_Measure_Aver_Inter_Non_Stop->distance())*1000;
			 /////////////////////////////////////////////////////////////////////////////
			 pFrgnd_Measure_Aver_Inter_Non_Stop->x      =      pFrgnd_Measure_Aver_Inter_Non_Stop->di*sin(pFrgnd_Measure_Aver_Inter_Non_Stop->ze)*cos(pFrgnd_Measure_Aver_Inter_Non_Stop->az);
			 pFrgnd_Measure_Aver_Inter_Non_Stop->y      =      pFrgnd_Measure_Aver_Inter_Non_Stop->di*sin(pFrgnd_Measure_Aver_Inter_Non_Stop->ze)*sin(pFrgnd_Measure_Aver_Inter_Non_Stop->az);
			 pFrgnd_Measure_Aver_Inter_Non_Stop->z      =      pFrgnd_Measure_Aver_Inter_Non_Stop->di*cos(pFrgnd_Measure_Aver_Inter_Non_Stop->ze);
			 /////////////////////////////////////////////////////////////////////////////
			 Ptpoint->X = pFrgnd_Measure_Aver_Inter_Non_Stop->x;
			 Ptpoint->Y = pFrgnd_Measure_Aver_Inter_Non_Stop->y;
			 Ptpoint->Z = pFrgnd_Measure_Aver_Inter_Non_Stop->z;
			 /////////////////////////////////////////////////////////////////////////////
			 traject.push_back(*Ptpoint);
			 /////////////////////////////////////////////////////////////////////////////
			 pFrgnd_Measure_Aver_Inter_Non_Stop->m_x.Format("%g", pFrgnd_Measure_Aver_Inter_Non_Stop->x);
			 pFrgnd_Measure_Aver_Inter_Non_Stop->m_y.Format("%g", pFrgnd_Measure_Aver_Inter_Non_Stop->y);
			 pFrgnd_Measure_Aver_Inter_Non_Stop->m_z.Format("%g", pFrgnd_Measure_Aver_Inter_Non_Stop->z);
			 /////////////////////////////////////////////////////////////////////////////
		     
			 CString Measureb = "x="+pFrgnd_Measure_Aver_Inter_Non_Stop->m_x+"      y="+pFrgnd_Measure_Aver_Inter_Non_Stop->m_y+"      z="+pFrgnd_Measure_Aver_Inter_Non_Stop->m_z;

			 pFrgnd_Measure_Aver_Inter_Non_Stop->m_listeofmeasur.InsertString(0,Measureb);
			 pFrgnd_Measure_Aver_Inter_Non_Stop->m_listeofmeasur.UpdateWindow();

		 }
		 catch(TrackerException* e )
		 {
			 char* msg;
			 msg = e->getText();
			 AfxMessageBox(msg);
			 delete e;
		 }
		 if (imaad2 == -1)
		 {
			 gpTracker->stopMeasurePoint();
			 break;
		 }
	 }
	 ExitThread(0); 
	 return 0;
}

//Foreground Measurement//StdDevFilter//IntervalTrigger//Non Stop collec/////////////////////
DWORD WINAPI MyCritariaFrgnd_Measure_StdD_Inter_Non_Stop( LPVOID pParam )
{
	CApplication_Faro_TrakerDlg* pFrgnd_Measure_StdD_Inter_Non_Stop = (CApplication_Faro_TrakerDlg*)pParam;
	/////////////////////////////////////////////////////////////////////////////////////

	MeasurePointData* Frgnd_Measure_StdD_Inter_Non_Stop = NULL;
	StdDevFilter filter;
	NullStartTrigger startTrigger;
	/////////////////////////////////////////////////////////////////////////////////////
	IntervalTrigger continueTrigger(pFrgnd_Measure_StdD_Inter_Non_Stop->InterTrig);
	/////////////////////////////////////////////////////////////////////////////////////
	MeasureCfg  cfg( pFrgnd_Measure_StdD_Inter_Non_Stop->SamPOb , &filter , &startTrigger , &continueTrigger);
	gpTracker->startMeasurePoint( &cfg );
	traject.clear();

	 while (true)
	 {
		 Point* Ptpoint;
	     Ptpoint = new Point();
		 try
		 {
			 Frgnd_Measure_StdD_Inter_Non_Stop = gpTracker->readMeasurePointData();		
	         ASSERT(Frgnd_Measure_StdD_Inter_Non_Stop);
	         LPARAM lParam = (LPARAM)Frgnd_Measure_StdD_Inter_Non_Stop;

			 /////////////////////////////////////////////////////////////////////////////

			 pFrgnd_Measure_StdD_Inter_Non_Stop->az     =     (Frgnd_Measure_StdD_Inter_Non_Stop->azimuth());
			 pFrgnd_Measure_StdD_Inter_Non_Stop->ze     =     (Frgnd_Measure_StdD_Inter_Non_Stop->zenith());    
			 pFrgnd_Measure_StdD_Inter_Non_Stop->di     =     (Frgnd_Measure_StdD_Inter_Non_Stop->distance())*1000;
			

			 pFrgnd_Measure_StdD_Inter_Non_Stop->x      =      pFrgnd_Measure_StdD_Inter_Non_Stop->di*sin(pFrgnd_Measure_StdD_Inter_Non_Stop->ze)*cos(pFrgnd_Measure_StdD_Inter_Non_Stop->az);
			 pFrgnd_Measure_StdD_Inter_Non_Stop->y      =      pFrgnd_Measure_StdD_Inter_Non_Stop->di*sin(pFrgnd_Measure_StdD_Inter_Non_Stop->ze)*sin(pFrgnd_Measure_StdD_Inter_Non_Stop->az);
			 pFrgnd_Measure_StdD_Inter_Non_Stop->z      =      pFrgnd_Measure_StdD_Inter_Non_Stop->di*cos(pFrgnd_Measure_StdD_Inter_Non_Stop->ze);

			 /////////////////////////////////////////////////////////////////////////////

			 Ptpoint->X = pFrgnd_Measure_StdD_Inter_Non_Stop->x;
			 Ptpoint->Y = pFrgnd_Measure_StdD_Inter_Non_Stop->y;
			 Ptpoint->Z = pFrgnd_Measure_StdD_Inter_Non_Stop->z;

			 traject.push_back(*Ptpoint);

			 /////////////////////////////////////////////////////////////////////////////

			 pFrgnd_Measure_StdD_Inter_Non_Stop->m_x.Format("%g", pFrgnd_Measure_StdD_Inter_Non_Stop->x);
			 pFrgnd_Measure_StdD_Inter_Non_Stop->m_y.Format("%g", pFrgnd_Measure_StdD_Inter_Non_Stop->y);
			 pFrgnd_Measure_StdD_Inter_Non_Stop->m_z.Format("%g", pFrgnd_Measure_StdD_Inter_Non_Stop->z);

		     
			 CString Measureb = "x="+pFrgnd_Measure_StdD_Inter_Non_Stop->m_x+"      y="+pFrgnd_Measure_StdD_Inter_Non_Stop->m_y+"      z="+pFrgnd_Measure_StdD_Inter_Non_Stop->m_z;

			 pFrgnd_Measure_StdD_Inter_Non_Stop->m_listeofmeasur.InsertString(0,Measureb);
			 pFrgnd_Measure_StdD_Inter_Non_Stop->m_listeofmeasur.UpdateWindow();

		 }
		 catch(TrackerException* e )
		 {
			 char* msg;
			 msg = e->getText();
			 AfxMessageBox(msg);
			 delete e;
		 }
		 if (imaad2 == -1)
		 {
			 gpTracker->stopMeasurePoint();
			 break;
		 }
	 }
	 ExitThread(0); 
	 return 0;
}

//Foreground Measurement//Average Filter//DistanceTrigger//Non Stop collec///////////////////
DWORD WINAPI MyCritariaFrgnd_Measure_Aver_Dist_Non_Stop( LPVOID pParam )
{
	CApplication_Faro_TrakerDlg* pFrgnd_Measure_Aver_Dist_Non_Stop = (CApplication_Faro_TrakerDlg*)pParam;
	/////////////////////////////////////////////////////////////////////////////////////

	MeasurePointData* Frgnd_Measure_Aver_Dist_Non_Stop = NULL;
	AverageFilter filter;
	NullStartTrigger startTrigger;
	/////////////////////////////////////////////////////////////////////////////////////
	DistanceTrigger continueTrigger(pFrgnd_Measure_Aver_Dist_Non_Stop->DitTrig);
	/////////////////////////////////////////////////////////////////////////////////////
	MeasureCfg  cfg( pFrgnd_Measure_Aver_Dist_Non_Stop->SamPOb , &filter , &startTrigger , &continueTrigger);
	gpTracker->startMeasurePoint( &cfg );
	traject.clear();

	 while (true)
	 {
		 Point* Ptpoint;
	     Ptpoint = new Point();
		 try
		 {
			 Frgnd_Measure_Aver_Dist_Non_Stop = gpTracker->readMeasurePointData();		
	         ASSERT(Frgnd_Measure_Aver_Dist_Non_Stop);
	         LPARAM lParam = (LPARAM)Frgnd_Measure_Aver_Dist_Non_Stop;

			 /////////////////////////////////////////////////////////////////////////////

			 pFrgnd_Measure_Aver_Dist_Non_Stop->az     =     (Frgnd_Measure_Aver_Dist_Non_Stop->azimuth());
			 pFrgnd_Measure_Aver_Dist_Non_Stop->ze     =     (Frgnd_Measure_Aver_Dist_Non_Stop->zenith());    
			 pFrgnd_Measure_Aver_Dist_Non_Stop->di     =     (Frgnd_Measure_Aver_Dist_Non_Stop->distance())*1000;
			

			 pFrgnd_Measure_Aver_Dist_Non_Stop->x      =      pFrgnd_Measure_Aver_Dist_Non_Stop->di*sin(pFrgnd_Measure_Aver_Dist_Non_Stop->ze)*cos(pFrgnd_Measure_Aver_Dist_Non_Stop->az);
			 pFrgnd_Measure_Aver_Dist_Non_Stop->y      =      pFrgnd_Measure_Aver_Dist_Non_Stop->di*sin(pFrgnd_Measure_Aver_Dist_Non_Stop->ze)*sin(pFrgnd_Measure_Aver_Dist_Non_Stop->az);
			 pFrgnd_Measure_Aver_Dist_Non_Stop->z      =      pFrgnd_Measure_Aver_Dist_Non_Stop->di*cos(pFrgnd_Measure_Aver_Dist_Non_Stop->ze);

			 /////////////////////////////////////////////////////////////////////////////

			 Ptpoint->X = pFrgnd_Measure_Aver_Dist_Non_Stop->x;
			 Ptpoint->Y = pFrgnd_Measure_Aver_Dist_Non_Stop->y;
			 Ptpoint->Z = pFrgnd_Measure_Aver_Dist_Non_Stop->z;

			 traject.push_back(*Ptpoint);

			 /////////////////////////////////////////////////////////////////////////////

			 pFrgnd_Measure_Aver_Dist_Non_Stop->m_x.Format("%g", pFrgnd_Measure_Aver_Dist_Non_Stop->x);
			 pFrgnd_Measure_Aver_Dist_Non_Stop->m_y.Format("%g", pFrgnd_Measure_Aver_Dist_Non_Stop->y);
			 pFrgnd_Measure_Aver_Dist_Non_Stop->m_z.Format("%g", pFrgnd_Measure_Aver_Dist_Non_Stop->z);

			
		     
			 CString Measureb = "x="+pFrgnd_Measure_Aver_Dist_Non_Stop->m_x+"      y="+pFrgnd_Measure_Aver_Dist_Non_Stop->m_y+"      z="+pFrgnd_Measure_Aver_Dist_Non_Stop->m_z;

			 pFrgnd_Measure_Aver_Dist_Non_Stop->m_listeofmeasur.InsertString(0,Measureb);
			 pFrgnd_Measure_Aver_Dist_Non_Stop->m_listeofmeasur.UpdateWindow();

		 }
		 catch(TrackerException* e )
		 {
			 char* msg;
			 msg = e->getText();
			 AfxMessageBox(msg);
			 delete e;
		 }
		 if (imaad2 == -1)
		 {
			 gpTracker->stopMeasurePoint();
			 break;
		 }
	 }
	 ExitThread(0); 
	 return 0;
}

//Foreground Measurement//StdDevFilter//DistanceTrigger//Non Stop collec/////////////////////
DWORD WINAPI MyCritariaFrgnd_Measure_StdD_Dist_Non_Stop( LPVOID pParam )
{
	CApplication_Faro_TrakerDlg* pFrgnd_Measure_StdD_Dist_Non_Stop = (CApplication_Faro_TrakerDlg*)pParam;
	/////////////////////////////////////////////////////////////////////////////////////
	MeasurePointData* Frgnd_Measure_StdD_Dist_Non_Stop = NULL;
	StdDevFilter filter;
	NullStartTrigger startTrigger;
	/////////////////////////////////////////////////////////////////////////////////////
	DistanceTrigger continueTrigger(pFrgnd_Measure_StdD_Dist_Non_Stop->DitTrig);
	/////////////////////////////////////////////////////////////////////////////////////
	MeasureCfg  cfg( pFrgnd_Measure_StdD_Dist_Non_Stop->SamPOb , &filter , &startTrigger , &continueTrigger);
	gpTracker->startMeasurePoint( &cfg );
	traject.clear();
	 while (true)
	 {
		 Point* Ptpoint;
	     Ptpoint = new Point();
		 try
		 {
			 Frgnd_Measure_StdD_Dist_Non_Stop = gpTracker->readMeasurePointData();		
	         ASSERT(Frgnd_Measure_StdD_Dist_Non_Stop);
	         LPARAM lParam = (LPARAM)Frgnd_Measure_StdD_Dist_Non_Stop;
			 /////////////////////////////////////////////////////////////////////////////
			 pFrgnd_Measure_StdD_Dist_Non_Stop->az     =     (Frgnd_Measure_StdD_Dist_Non_Stop->azimuth());
			 pFrgnd_Measure_StdD_Dist_Non_Stop->ze     =     (Frgnd_Measure_StdD_Dist_Non_Stop->zenith());    
			 pFrgnd_Measure_StdD_Dist_Non_Stop->di     =     (Frgnd_Measure_StdD_Dist_Non_Stop->distance())*1000;
             /////////////////////////////////////////////////////////////////////////////
			 pFrgnd_Measure_StdD_Dist_Non_Stop->x      =      pFrgnd_Measure_StdD_Dist_Non_Stop->di*sin(pFrgnd_Measure_StdD_Dist_Non_Stop->ze)*cos(pFrgnd_Measure_StdD_Dist_Non_Stop->az);
			 pFrgnd_Measure_StdD_Dist_Non_Stop->y      =      pFrgnd_Measure_StdD_Dist_Non_Stop->di*sin(pFrgnd_Measure_StdD_Dist_Non_Stop->ze)*sin(pFrgnd_Measure_StdD_Dist_Non_Stop->az);
			 pFrgnd_Measure_StdD_Dist_Non_Stop->z      =      pFrgnd_Measure_StdD_Dist_Non_Stop->di*cos(pFrgnd_Measure_StdD_Dist_Non_Stop->ze);
			 /////////////////////////////////////////////////////////////////////////////
			 Ptpoint->X = pFrgnd_Measure_StdD_Dist_Non_Stop->x;
			 Ptpoint->Y = pFrgnd_Measure_StdD_Dist_Non_Stop->y;
			 Ptpoint->Z = pFrgnd_Measure_StdD_Dist_Non_Stop->z;
			 traject.push_back(*Ptpoint);
			 /////////////////////////////////////////////////////////////////////////////
			 pFrgnd_Measure_StdD_Dist_Non_Stop->m_x.Format("%g", pFrgnd_Measure_StdD_Dist_Non_Stop->x);
			 pFrgnd_Measure_StdD_Dist_Non_Stop->m_y.Format("%g", pFrgnd_Measure_StdD_Dist_Non_Stop->y);
			 pFrgnd_Measure_StdD_Dist_Non_Stop->m_z.Format("%g", pFrgnd_Measure_StdD_Dist_Non_Stop->z);

		     
			 CString Measureb = "x="+pFrgnd_Measure_StdD_Dist_Non_Stop->m_x+"      y="+pFrgnd_Measure_StdD_Dist_Non_Stop->m_y+"      z="+pFrgnd_Measure_StdD_Dist_Non_Stop->m_z;

			 pFrgnd_Measure_StdD_Dist_Non_Stop->m_listeofmeasur.InsertString(0,Measureb);
			 pFrgnd_Measure_StdD_Dist_Non_Stop->m_listeofmeasur.UpdateWindow();

		 }
		 catch(TrackerException* e )
		 {
			 char* msg;
			 msg = e->getText();
			 AfxMessageBox(msg);
			 delete e;
		 }
		 if (imaad2 == -1)
		 {
			 gpTracker->stopMeasurePoint();
			 break;
		 }
	 }
	 ExitThread(0); 
	 return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//Foreground Measurement//Average Filter//NullContinueTrigger//Yes Stop collec///////////////
DWORD WINAPI MyCritariaFrgnd_Measure_Aver_NullCont_Yes_Stop( LPVOID pParam )
{
	CApplication_Faro_TrakerDlg* pFrgnd_Measure_Aver_NullCont_Yes_Stop = (CApplication_Faro_TrakerDlg*)pParam;
	/////////////////////////////////////////////////////////////////////////////////////
	MeasurePointData* Frgnd_Measure_Aver_NullCont_Yes_Stop = NULL;
	AverageFilter filter;
	NullStartTrigger startTrigger;
	/////////////////////////////////////////////////////////////////////////////////////
	NullContinueTrigger continueTrigger;
	/////////////////////////////////////////////////////////////////////////////////////
	MeasureCfg  cfg( pFrgnd_Measure_Aver_NullCont_Yes_Stop->SamPOb , &filter , &startTrigger , &continueTrigger);
	gpTracker->startMeasurePoint( &cfg );
	traject.clear();
	while (true)
	 {
		 Point* Ptpoint;
	     Ptpoint = new Point();
		 try
		 {
			 Frgnd_Measure_Aver_NullCont_Yes_Stop = gpTracker->readMeasurePointData();		
	         ASSERT(Frgnd_Measure_Aver_NullCont_Yes_Stop);
	         LPARAM lParam = (LPARAM)Frgnd_Measure_Aver_NullCont_Yes_Stop;
			 /////////////////////////////////////////////////////////////////////////////
			 pFrgnd_Measure_Aver_NullCont_Yes_Stop->az     =     (Frgnd_Measure_Aver_NullCont_Yes_Stop->azimuth());
			 pFrgnd_Measure_Aver_NullCont_Yes_Stop->ze     =     (Frgnd_Measure_Aver_NullCont_Yes_Stop->zenith());    
			 pFrgnd_Measure_Aver_NullCont_Yes_Stop->di     =     (Frgnd_Measure_Aver_NullCont_Yes_Stop->distance())*1000;
             /////////////////////////////////////////////////////////////////////////////
			 pFrgnd_Measure_Aver_NullCont_Yes_Stop->x      =      pFrgnd_Measure_Aver_NullCont_Yes_Stop->di*sin(pFrgnd_Measure_Aver_NullCont_Yes_Stop->ze)*cos(pFrgnd_Measure_Aver_NullCont_Yes_Stop->az);
			 pFrgnd_Measure_Aver_NullCont_Yes_Stop->y      =      pFrgnd_Measure_Aver_NullCont_Yes_Stop->di*sin(pFrgnd_Measure_Aver_NullCont_Yes_Stop->ze)*sin(pFrgnd_Measure_Aver_NullCont_Yes_Stop->az);
			 pFrgnd_Measure_Aver_NullCont_Yes_Stop->z      =      pFrgnd_Measure_Aver_NullCont_Yes_Stop->di*cos(pFrgnd_Measure_Aver_NullCont_Yes_Stop->ze);
			 /////////////////////////////////////////////////////////////////////////////		
			 Size_Collec_Stop = traject.size();
             /////////////////////////////////////////////////////////////////////////////
             if (Size_Collec_Stop > 0)
			 {
				 Distance_Point = sqrt((traject[Size_Collec_Stop - 1].X - pFrgnd_Measure_Aver_NullCont_Yes_Stop->x)*(traject[Size_Collec_Stop - 1].X - pFrgnd_Measure_Aver_NullCont_Yes_Stop->x)+(traject[Size_Collec_Stop - 1].Y - pFrgnd_Measure_Aver_NullCont_Yes_Stop->y)*(traject[Size_Collec_Stop - 1].Y - pFrgnd_Measure_Aver_NullCont_Yes_Stop->y)+(traject[Size_Collec_Stop - 1].Z - pFrgnd_Measure_Aver_NullCont_Yes_Stop->z)*(traject[Size_Collec_Stop - 1].Z - pFrgnd_Measure_Aver_NullCont_Yes_Stop->z));
				 if (Distance_Point > PARAM4)
				 {
				 	 Ptpoint->X = pFrgnd_Measure_Aver_NullCont_Yes_Stop->x;
					 Ptpoint->Y = pFrgnd_Measure_Aver_NullCont_Yes_Stop->y;
					 Ptpoint->Z = pFrgnd_Measure_Aver_NullCont_Yes_Stop->z;
					 traject.push_back(*Ptpoint);
					 /////////////////////////////////////////////////////////////////////////////
					 pFrgnd_Measure_Aver_NullCont_Yes_Stop->m_x.Format("%g", pFrgnd_Measure_Aver_NullCont_Yes_Stop->x);
					 pFrgnd_Measure_Aver_NullCont_Yes_Stop->m_y.Format("%g", pFrgnd_Measure_Aver_NullCont_Yes_Stop->y);
					 pFrgnd_Measure_Aver_NullCont_Yes_Stop->m_z.Format("%g", pFrgnd_Measure_Aver_NullCont_Yes_Stop->z);			     
					 CString Measureb = "x="+pFrgnd_Measure_Aver_NullCont_Yes_Stop->m_x+"      y="+pFrgnd_Measure_Aver_NullCont_Yes_Stop->m_y+"      z="+pFrgnd_Measure_Aver_NullCont_Yes_Stop->m_z;
					 pFrgnd_Measure_Aver_NullCont_Yes_Stop->m_listeofmeasur.InsertString(0,Measureb);
					 pFrgnd_Measure_Aver_NullCont_Yes_Stop->m_listeofmeasur.UpdateWindow();
				 }
			 }
             else
			 {
			 	 Ptpoint->X = pFrgnd_Measure_Aver_NullCont_Yes_Stop->x;
				 Ptpoint->Y = pFrgnd_Measure_Aver_NullCont_Yes_Stop->y;
				 Ptpoint->Z = pFrgnd_Measure_Aver_NullCont_Yes_Stop->z;
				 traject.push_back(*Ptpoint);
				 /////////////////////////////////////////////////////////////////////////////
				 pFrgnd_Measure_Aver_NullCont_Yes_Stop->m_x.Format("%g", pFrgnd_Measure_Aver_NullCont_Yes_Stop->x);
				 pFrgnd_Measure_Aver_NullCont_Yes_Stop->m_y.Format("%g", pFrgnd_Measure_Aver_NullCont_Yes_Stop->y);
				 pFrgnd_Measure_Aver_NullCont_Yes_Stop->m_z.Format("%g", pFrgnd_Measure_Aver_NullCont_Yes_Stop->z);		     
				 CString Measureb = "x="+pFrgnd_Measure_Aver_NullCont_Yes_Stop->m_x+"      y="+pFrgnd_Measure_Aver_NullCont_Yes_Stop->m_y+"      z="+pFrgnd_Measure_Aver_NullCont_Yes_Stop->m_z;
				 pFrgnd_Measure_Aver_NullCont_Yes_Stop->m_listeofmeasur.InsertString(0,Measureb);
				 pFrgnd_Measure_Aver_NullCont_Yes_Stop->m_listeofmeasur.UpdateWindow();			 
			 }	
		 }
		 catch(TrackerException* e )
		 {
			 char* msg;
			 msg = e->getText();
			 AfxMessageBox(msg);
			 delete e;
		 }
		 if (imaad2 == -1)
		 {
			 gpTracker->stopMeasurePoint();
			 break;
		 }
	 }
	 ExitThread(0); 
	 return 0;
}

//Foreground Measurement//StdDevFilter//NullContinueTrigger//Yes Stop collec/////////////////
DWORD WINAPI MyCritariaFrgnd_Measure_StdD_NullCont_Yes_Stop( LPVOID pParam )
{
	CApplication_Faro_TrakerDlg* pFrgnd_Measure_StdD_NullCont_Yes_Stop = (CApplication_Faro_TrakerDlg*)pParam;
	/////////////////////////////////////////////////////////////////////////////////////
	MeasurePointData* Frgnd_Measure_StdD_NullCont_Yes_Stop = NULL;
	StdDevFilter filter;
	NullStartTrigger startTrigger;
	/////////////////////////////////////////////////////////////////////////////////////
	NullContinueTrigger continueTrigger;
	/////////////////////////////////////////////////////////////////////////////////////
	MeasureCfg  cfg( pFrgnd_Measure_StdD_NullCont_Yes_Stop->SamPOb , &filter , &startTrigger , &continueTrigger);
	gpTracker->startMeasurePoint( &cfg );
	traject.clear();
	 while (true)
	 {
		 Point* Ptpoint;
	     Ptpoint = new Point();
		 try
		 {
			 Frgnd_Measure_StdD_NullCont_Yes_Stop = gpTracker->readMeasurePointData();		
	         ASSERT(Frgnd_Measure_StdD_NullCont_Yes_Stop);
	         LPARAM lParam = (LPARAM)Frgnd_Measure_StdD_NullCont_Yes_Stop;
			 /////////////////////////////////////////////////////////////////////////////
			 pFrgnd_Measure_StdD_NullCont_Yes_Stop->az     =     (Frgnd_Measure_StdD_NullCont_Yes_Stop->azimuth());
			 pFrgnd_Measure_StdD_NullCont_Yes_Stop->ze     =     (Frgnd_Measure_StdD_NullCont_Yes_Stop->zenith());    
			 pFrgnd_Measure_StdD_NullCont_Yes_Stop->di     =     (Frgnd_Measure_StdD_NullCont_Yes_Stop->distance())*1000;
             /////////////////////////////////////////////////////////////////////////////	
			 pFrgnd_Measure_StdD_NullCont_Yes_Stop->x      =      pFrgnd_Measure_StdD_NullCont_Yes_Stop->di*sin(pFrgnd_Measure_StdD_NullCont_Yes_Stop->ze)*cos(pFrgnd_Measure_StdD_NullCont_Yes_Stop->az);
			 pFrgnd_Measure_StdD_NullCont_Yes_Stop->y      =      pFrgnd_Measure_StdD_NullCont_Yes_Stop->di*sin(pFrgnd_Measure_StdD_NullCont_Yes_Stop->ze)*sin(pFrgnd_Measure_StdD_NullCont_Yes_Stop->az);
			 pFrgnd_Measure_StdD_NullCont_Yes_Stop->z      =      pFrgnd_Measure_StdD_NullCont_Yes_Stop->di*cos(pFrgnd_Measure_StdD_NullCont_Yes_Stop->ze);
			 /////////////////////////////////////////////////////////////////////////////
			 Size_Collec_Stop = traject.size();
             if (Size_Collec_Stop > 0)
			 {
				 Distance_Point = sqrt((traject[Size_Collec_Stop - 1].X - pFrgnd_Measure_StdD_NullCont_Yes_Stop->x)*(traject[Size_Collec_Stop - 1].X - pFrgnd_Measure_StdD_NullCont_Yes_Stop->x)+(traject[Size_Collec_Stop - 1].Y - pFrgnd_Measure_StdD_NullCont_Yes_Stop->y)*(traject[Size_Collec_Stop - 1].Y - pFrgnd_Measure_StdD_NullCont_Yes_Stop->y)+(traject[Size_Collec_Stop - 1].Z - pFrgnd_Measure_StdD_NullCont_Yes_Stop->z)*(traject[Size_Collec_Stop - 1].Z - pFrgnd_Measure_StdD_NullCont_Yes_Stop->z));
				 if (Distance_Point > PARAM4)
				 {
				 	 Ptpoint->X = pFrgnd_Measure_StdD_NullCont_Yes_Stop->x;
					 Ptpoint->Y = pFrgnd_Measure_StdD_NullCont_Yes_Stop->y;
					 Ptpoint->Z = pFrgnd_Measure_StdD_NullCont_Yes_Stop->z;	
					 traject.push_back(*Ptpoint);
					 /////////////////////////////////////////////////////////////////////////////
					 pFrgnd_Measure_StdD_NullCont_Yes_Stop->m_x.Format("%g", pFrgnd_Measure_StdD_NullCont_Yes_Stop->x);
					 pFrgnd_Measure_StdD_NullCont_Yes_Stop->m_y.Format("%g", pFrgnd_Measure_StdD_NullCont_Yes_Stop->y);
					 pFrgnd_Measure_StdD_NullCont_Yes_Stop->m_z.Format("%g", pFrgnd_Measure_StdD_NullCont_Yes_Stop->z);					     
					 CString Measureb = "x="+pFrgnd_Measure_StdD_NullCont_Yes_Stop->m_x+"      y="+pFrgnd_Measure_StdD_NullCont_Yes_Stop->m_y+"      z="+pFrgnd_Measure_StdD_NullCont_Yes_Stop->m_z;
					 pFrgnd_Measure_StdD_NullCont_Yes_Stop->m_listeofmeasur.InsertString(0,Measureb);
					 pFrgnd_Measure_StdD_NullCont_Yes_Stop->m_listeofmeasur.UpdateWindow();
				 }
			 }
             else
			 {
			 	 Ptpoint->X = pFrgnd_Measure_StdD_NullCont_Yes_Stop->x;
				 Ptpoint->Y = pFrgnd_Measure_StdD_NullCont_Yes_Stop->y;
				 Ptpoint->Z = pFrgnd_Measure_StdD_NullCont_Yes_Stop->z;
				 traject.push_back(*Ptpoint);
				 /////////////////////////////////////////////////////////////////////////////
				 pFrgnd_Measure_StdD_NullCont_Yes_Stop->m_x.Format("%g", pFrgnd_Measure_StdD_NullCont_Yes_Stop->x);
				 pFrgnd_Measure_StdD_NullCont_Yes_Stop->m_y.Format("%g", pFrgnd_Measure_StdD_NullCont_Yes_Stop->y);
				 pFrgnd_Measure_StdD_NullCont_Yes_Stop->m_z.Format("%g", pFrgnd_Measure_StdD_NullCont_Yes_Stop->z);			     
				 CString Measureb = "x="+pFrgnd_Measure_StdD_NullCont_Yes_Stop->m_x+"      y="+pFrgnd_Measure_StdD_NullCont_Yes_Stop->m_y+"      z="+pFrgnd_Measure_StdD_NullCont_Yes_Stop->m_z;
				 pFrgnd_Measure_StdD_NullCont_Yes_Stop->m_listeofmeasur.InsertString(0,Measureb);
				 pFrgnd_Measure_StdD_NullCont_Yes_Stop->m_listeofmeasur.UpdateWindow();			 
			 }				 
		 }
		 catch(TrackerException* e )
		 {
			 char* msg;
			 msg = e->getText();
			 AfxMessageBox(msg);
			 delete e;
		 }
		 if (imaad2 == -1)
		 {
			 gpTracker->stopMeasurePoint();
			 break;
		 }
	 }
	 ExitThread(0); 
	 return 0;
}

//Foreground Measurement//Average Filter//IntervalTrigger//Yes Stop collec///////////////////
DWORD WINAPI MyCritariaFrgnd_Measure_Aver_Inter_Yes_Stop( LPVOID pParam )
{
	CApplication_Faro_TrakerDlg* pFrgnd_Measure_Aver_Inter_Yes_Stop = (CApplication_Faro_TrakerDlg*)pParam;
	/////////////////////////////////////////////////////////////////////////////////////
	MeasurePointData* Frgnd_Measure_Aver_Inter_Yes_Stop = NULL;
	AverageFilter filter;
	NullStartTrigger startTrigger;
	/////////////////////////////////////////////////////////////////////////////////////
	IntervalTrigger continueTrigger(pFrgnd_Measure_Aver_Inter_Yes_Stop->InterTrig);
	/////////////////////////////////////////////////////////////////////////////////////
	MeasureCfg  cfg( pFrgnd_Measure_Aver_Inter_Yes_Stop->SamPOb , &filter , &startTrigger , &continueTrigger);
	gpTracker->startMeasurePoint( &cfg );
	traject.clear();
	 while (true)
	 {
		 Point* Ptpoint;
	     Ptpoint = new Point();
		 try
		 {
			 Frgnd_Measure_Aver_Inter_Yes_Stop = gpTracker->readMeasurePointData();		
	         ASSERT(Frgnd_Measure_Aver_Inter_Yes_Stop);
	         LPARAM lParam = (LPARAM)Frgnd_Measure_Aver_Inter_Yes_Stop;
			 /////////////////////////////////////////////////////////////////////////////
			 pFrgnd_Measure_Aver_Inter_Yes_Stop->az     =     (Frgnd_Measure_Aver_Inter_Yes_Stop->azimuth());
			 pFrgnd_Measure_Aver_Inter_Yes_Stop->ze     =     (Frgnd_Measure_Aver_Inter_Yes_Stop->zenith());    
			 pFrgnd_Measure_Aver_Inter_Yes_Stop->di     =     (Frgnd_Measure_Aver_Inter_Yes_Stop->distance())*1000;
             /////////////////////////////////////////////////////////////////////////////		
			 pFrgnd_Measure_Aver_Inter_Yes_Stop->x      =      pFrgnd_Measure_Aver_Inter_Yes_Stop->di*sin(pFrgnd_Measure_Aver_Inter_Yes_Stop->ze)*cos(pFrgnd_Measure_Aver_Inter_Yes_Stop->az);
			 pFrgnd_Measure_Aver_Inter_Yes_Stop->y      =      pFrgnd_Measure_Aver_Inter_Yes_Stop->di*sin(pFrgnd_Measure_Aver_Inter_Yes_Stop->ze)*sin(pFrgnd_Measure_Aver_Inter_Yes_Stop->az);
			 pFrgnd_Measure_Aver_Inter_Yes_Stop->z      =      pFrgnd_Measure_Aver_Inter_Yes_Stop->di*cos(pFrgnd_Measure_Aver_Inter_Yes_Stop->ze);
			 /////////////////////////////////////////////////////////////////////////////
			 Size_Collec_Stop = traject.size();
             if (Size_Collec_Stop > 0)
			 {
				 Distance_Point = sqrt((traject[Size_Collec_Stop - 1].X - pFrgnd_Measure_Aver_Inter_Yes_Stop->x)*(traject[Size_Collec_Stop - 1].X - pFrgnd_Measure_Aver_Inter_Yes_Stop->x)+(traject[Size_Collec_Stop - 1].Y - pFrgnd_Measure_Aver_Inter_Yes_Stop->y)*(traject[Size_Collec_Stop - 1].Y - pFrgnd_Measure_Aver_Inter_Yes_Stop->y)+(traject[Size_Collec_Stop - 1].Z - pFrgnd_Measure_Aver_Inter_Yes_Stop->z)*(traject[Size_Collec_Stop - 1].Z - pFrgnd_Measure_Aver_Inter_Yes_Stop->z));
				 if (Distance_Point > PARAM4)
				 {
					 	 Ptpoint->X = pFrgnd_Measure_Aver_Inter_Yes_Stop->x;
						 Ptpoint->Y = pFrgnd_Measure_Aver_Inter_Yes_Stop->y;
						 Ptpoint->Z = pFrgnd_Measure_Aver_Inter_Yes_Stop->z;
						 traject.push_back(*Ptpoint);
						 /////////////////////////////////////////////////////////////////////////////
						 pFrgnd_Measure_Aver_Inter_Yes_Stop->m_x.Format("%g", pFrgnd_Measure_Aver_Inter_Yes_Stop->x);
						 pFrgnd_Measure_Aver_Inter_Yes_Stop->m_y.Format("%g", pFrgnd_Measure_Aver_Inter_Yes_Stop->y);
						 pFrgnd_Measure_Aver_Inter_Yes_Stop->m_z.Format("%g", pFrgnd_Measure_Aver_Inter_Yes_Stop->z);					     
						 CString Measureb = "x="+pFrgnd_Measure_Aver_Inter_Yes_Stop->m_x+"      y="+pFrgnd_Measure_Aver_Inter_Yes_Stop->m_y+"      z="+pFrgnd_Measure_Aver_Inter_Yes_Stop->m_z;
						 pFrgnd_Measure_Aver_Inter_Yes_Stop->m_listeofmeasur.InsertString(0,Measureb);
						 pFrgnd_Measure_Aver_Inter_Yes_Stop->m_listeofmeasur.UpdateWindow();
				 }
			 }
             else
			 {
			 	 Ptpoint->X = pFrgnd_Measure_Aver_Inter_Yes_Stop->x;
				 Ptpoint->Y = pFrgnd_Measure_Aver_Inter_Yes_Stop->y;
				 Ptpoint->Z = pFrgnd_Measure_Aver_Inter_Yes_Stop->z;
				 traject.push_back(*Ptpoint);
				 /////////////////////////////////////////////////////////////////////////////
				 pFrgnd_Measure_Aver_Inter_Yes_Stop->m_x.Format("%g", pFrgnd_Measure_Aver_Inter_Yes_Stop->x);
				 pFrgnd_Measure_Aver_Inter_Yes_Stop->m_y.Format("%g", pFrgnd_Measure_Aver_Inter_Yes_Stop->y);
				 pFrgnd_Measure_Aver_Inter_Yes_Stop->m_z.Format("%g", pFrgnd_Measure_Aver_Inter_Yes_Stop->z);			     
				 CString Measureb = "x="+pFrgnd_Measure_Aver_Inter_Yes_Stop->m_x+"      y="+pFrgnd_Measure_Aver_Inter_Yes_Stop->m_y+"      z="+pFrgnd_Measure_Aver_Inter_Yes_Stop->m_z;
				 pFrgnd_Measure_Aver_Inter_Yes_Stop->m_listeofmeasur.InsertString(0,Measureb);
				 pFrgnd_Measure_Aver_Inter_Yes_Stop->m_listeofmeasur.UpdateWindow();				 
			 }	
		 }
		 catch(TrackerException* e )
		 {
			 char* msg;
			 msg = e->getText();
			 AfxMessageBox(msg);
			 delete e;
		 }
		 if (imaad2 == -1)
		 {
			 gpTracker->stopMeasurePoint();
			 break;
		 }
	 }
	 ExitThread(0); 
	 return 0;
}

//Foreground Measurement//StdDevFilter//IntervalTrigger//Yes Stop collec/////////////////////
DWORD WINAPI MyCritariaFrgnd_Measure_StdD_Inter_Yes_Stop( LPVOID pParam )
{
	CApplication_Faro_TrakerDlg* pFrgnd_Measure_StdD_Inter_Yes_Stop = (CApplication_Faro_TrakerDlg*)pParam;
	/////////////////////////////////////////////////////////////////////////////////////
	MeasurePointData* Frgnd_Measure_StdD_Inter_Yes_Stop = NULL;
	StdDevFilter filter;
	NullStartTrigger startTrigger;
	/////////////////////////////////////////////////////////////////////////////////////
	IntervalTrigger continueTrigger(pFrgnd_Measure_StdD_Inter_Yes_Stop->InterTrig);
	/////////////////////////////////////////////////////////////////////////////////////
	MeasureCfg  cfg( pFrgnd_Measure_StdD_Inter_Yes_Stop->SamPOb , &filter , &startTrigger , &continueTrigger);
	gpTracker->startMeasurePoint( &cfg );
	traject.clear();
	 while (true)
	 {
		 Point* Ptpoint;
	     Ptpoint = new Point();
		 try
		 {
			 Frgnd_Measure_StdD_Inter_Yes_Stop = gpTracker->readMeasurePointData();		
	         ASSERT(Frgnd_Measure_StdD_Inter_Yes_Stop);
	         LPARAM lParam = (LPARAM)Frgnd_Measure_StdD_Inter_Yes_Stop;
			 /////////////////////////////////////////////////////////////////////////////
			 pFrgnd_Measure_StdD_Inter_Yes_Stop->az     =     (Frgnd_Measure_StdD_Inter_Yes_Stop->azimuth());
			 pFrgnd_Measure_StdD_Inter_Yes_Stop->ze     =     (Frgnd_Measure_StdD_Inter_Yes_Stop->zenith());    
			 pFrgnd_Measure_StdD_Inter_Yes_Stop->di     =     (Frgnd_Measure_StdD_Inter_Yes_Stop->distance())*1000;
             /////////////////////////////////////////////////////////////////////////////
			 pFrgnd_Measure_StdD_Inter_Yes_Stop->x      =      pFrgnd_Measure_StdD_Inter_Yes_Stop->di*sin(pFrgnd_Measure_StdD_Inter_Yes_Stop->ze)*cos(pFrgnd_Measure_StdD_Inter_Yes_Stop->az);
			 pFrgnd_Measure_StdD_Inter_Yes_Stop->y      =      pFrgnd_Measure_StdD_Inter_Yes_Stop->di*sin(pFrgnd_Measure_StdD_Inter_Yes_Stop->ze)*sin(pFrgnd_Measure_StdD_Inter_Yes_Stop->az);
			 pFrgnd_Measure_StdD_Inter_Yes_Stop->z      =      pFrgnd_Measure_StdD_Inter_Yes_Stop->di*cos(pFrgnd_Measure_StdD_Inter_Yes_Stop->ze);
			 /////////////////////////////////////////////////////////////////////////////
			 Size_Collec_Stop = traject.size();
             if (Size_Collec_Stop > 0)
			 {
				 Distance_Point = sqrt((traject[Size_Collec_Stop - 1].X - pFrgnd_Measure_StdD_Inter_Yes_Stop->x)*(traject[Size_Collec_Stop - 1].X - pFrgnd_Measure_StdD_Inter_Yes_Stop->x)+(traject[Size_Collec_Stop - 1].Y - pFrgnd_Measure_StdD_Inter_Yes_Stop->y)*(traject[Size_Collec_Stop - 1].Y - pFrgnd_Measure_StdD_Inter_Yes_Stop->y)+(traject[Size_Collec_Stop - 1].Z - pFrgnd_Measure_StdD_Inter_Yes_Stop->z)*(traject[Size_Collec_Stop - 1].Z - pFrgnd_Measure_StdD_Inter_Yes_Stop->z));
				 if (Distance_Point > PARAM4)
				 {
				 	 Ptpoint->X = pFrgnd_Measure_StdD_Inter_Yes_Stop->x;
					 Ptpoint->Y = pFrgnd_Measure_StdD_Inter_Yes_Stop->y;
					 Ptpoint->Z = pFrgnd_Measure_StdD_Inter_Yes_Stop->z;
					 traject.push_back(*Ptpoint);
					 /////////////////////////////////////////////////////////////////////////////
					 pFrgnd_Measure_StdD_Inter_Yes_Stop->m_x.Format("%g", pFrgnd_Measure_StdD_Inter_Yes_Stop->x);
					 pFrgnd_Measure_StdD_Inter_Yes_Stop->m_y.Format("%g", pFrgnd_Measure_StdD_Inter_Yes_Stop->y);
					 pFrgnd_Measure_StdD_Inter_Yes_Stop->m_z.Format("%g", pFrgnd_Measure_StdD_Inter_Yes_Stop->z);					     
					 CString Measureb = "x="+pFrgnd_Measure_StdD_Inter_Yes_Stop->m_x+"      y="+pFrgnd_Measure_StdD_Inter_Yes_Stop->m_y+"      z="+pFrgnd_Measure_StdD_Inter_Yes_Stop->m_z;
					 pFrgnd_Measure_StdD_Inter_Yes_Stop->m_listeofmeasur.InsertString(0,Measureb);
					 pFrgnd_Measure_StdD_Inter_Yes_Stop->m_listeofmeasur.UpdateWindow();
				 }
			 }
             else
			 {
			 	 Ptpoint->X = pFrgnd_Measure_StdD_Inter_Yes_Stop->x;
				 Ptpoint->Y = pFrgnd_Measure_StdD_Inter_Yes_Stop->y;
				 Ptpoint->Z = pFrgnd_Measure_StdD_Inter_Yes_Stop->z;
				 traject.push_back(*Ptpoint);
				 /////////////////////////////////////////////////////////////////////////////
				 pFrgnd_Measure_StdD_Inter_Yes_Stop->m_x.Format("%g", pFrgnd_Measure_StdD_Inter_Yes_Stop->x);
				 pFrgnd_Measure_StdD_Inter_Yes_Stop->m_y.Format("%g", pFrgnd_Measure_StdD_Inter_Yes_Stop->y);
				 pFrgnd_Measure_StdD_Inter_Yes_Stop->m_z.Format("%g", pFrgnd_Measure_StdD_Inter_Yes_Stop->z);			     
				 CString Measureb = "x="+pFrgnd_Measure_StdD_Inter_Yes_Stop->m_x+"      y="+pFrgnd_Measure_StdD_Inter_Yes_Stop->m_y+"      z="+pFrgnd_Measure_StdD_Inter_Yes_Stop->m_z;
				 pFrgnd_Measure_StdD_Inter_Yes_Stop->m_listeofmeasur.InsertString(0,Measureb);
				 pFrgnd_Measure_StdD_Inter_Yes_Stop->m_listeofmeasur.UpdateWindow();				 
			 }	

		 }
		 catch(TrackerException* e )
		 {
			 char* msg;
			 msg = e->getText();
			 AfxMessageBox(msg);
			 delete e;
		 }
		 if (imaad2 == -1)
		 {
			 gpTracker->stopMeasurePoint();
			 break;
		 }
	 }
	 ExitThread(0); 
	 return 0;
}

//Foreground Measurement//Average Filter//DistanceTrigger//Yes Stop collec///////////////////
DWORD WINAPI MyCritariaFrgnd_Measure_Aver_Dist_Yes_Stop( LPVOID pParam )
{
	CApplication_Faro_TrakerDlg* pFrgnd_Measure_Aver_Dist_Yes_Stop = (CApplication_Faro_TrakerDlg*)pParam;
	/////////////////////////////////////////////////////////////////////////////////////
	MeasurePointData* Frgnd_Measure_Aver_Dist_Yes_Stop = NULL;
	AverageFilter filter;
	NullStartTrigger startTrigger;
	/////////////////////////////////////////////////////////////////////////////////////
	DistanceTrigger continueTrigger(pFrgnd_Measure_Aver_Dist_Yes_Stop->DitTrig);
	/////////////////////////////////////////////////////////////////////////////////////
	MeasureCfg  cfg( pFrgnd_Measure_Aver_Dist_Yes_Stop->SamPOb , &filter , &startTrigger , &continueTrigger);
	gpTracker->startMeasurePoint( &cfg );
	traject.clear();
	 while (true)
	 {
		 Point* Ptpoint;
	     Ptpoint = new Point();
		 try
		 {
			 Frgnd_Measure_Aver_Dist_Yes_Stop = gpTracker->readMeasurePointData();		
	         ASSERT(Frgnd_Measure_Aver_Dist_Yes_Stop);
	         LPARAM lParam = (LPARAM)Frgnd_Measure_Aver_Dist_Yes_Stop;
			 /////////////////////////////////////////////////////////////////////////////
			 pFrgnd_Measure_Aver_Dist_Yes_Stop->az     =     (Frgnd_Measure_Aver_Dist_Yes_Stop->azimuth());
			 pFrgnd_Measure_Aver_Dist_Yes_Stop->ze     =     (Frgnd_Measure_Aver_Dist_Yes_Stop->zenith());    
			 pFrgnd_Measure_Aver_Dist_Yes_Stop->di     =     (Frgnd_Measure_Aver_Dist_Yes_Stop->distance())*1000;
             /////////////////////////////////////////////////////////////////////////////
			 pFrgnd_Measure_Aver_Dist_Yes_Stop->x      =      pFrgnd_Measure_Aver_Dist_Yes_Stop->di*sin(pFrgnd_Measure_Aver_Dist_Yes_Stop->ze)*cos(pFrgnd_Measure_Aver_Dist_Yes_Stop->az);
			 pFrgnd_Measure_Aver_Dist_Yes_Stop->y      =      pFrgnd_Measure_Aver_Dist_Yes_Stop->di*sin(pFrgnd_Measure_Aver_Dist_Yes_Stop->ze)*sin(pFrgnd_Measure_Aver_Dist_Yes_Stop->az);
			 pFrgnd_Measure_Aver_Dist_Yes_Stop->z      =      pFrgnd_Measure_Aver_Dist_Yes_Stop->di*cos(pFrgnd_Measure_Aver_Dist_Yes_Stop->ze);
			 /////////////////////////////////////////////////////////////////////////////
			 Size_Collec_Stop = traject.size();
             if (Size_Collec_Stop > 0)
			 {
				 Distance_Point = sqrt((traject[Size_Collec_Stop - 1].X - pFrgnd_Measure_Aver_Dist_Yes_Stop->x)*(traject[Size_Collec_Stop - 1].X - pFrgnd_Measure_Aver_Dist_Yes_Stop->x)+(traject[Size_Collec_Stop - 1].Y - pFrgnd_Measure_Aver_Dist_Yes_Stop->y)*(traject[Size_Collec_Stop - 1].Y - pFrgnd_Measure_Aver_Dist_Yes_Stop->y)+(traject[Size_Collec_Stop - 1].Z - pFrgnd_Measure_Aver_Dist_Yes_Stop->z)*(traject[Size_Collec_Stop - 1].Z - pFrgnd_Measure_Aver_Dist_Yes_Stop->z));
				 if (Distance_Point > PARAM4)
				 {
				 	 Ptpoint->X = pFrgnd_Measure_Aver_Dist_Yes_Stop->x;
					 Ptpoint->Y = pFrgnd_Measure_Aver_Dist_Yes_Stop->y;
					 Ptpoint->Z = pFrgnd_Measure_Aver_Dist_Yes_Stop->z;
					 traject.push_back(*Ptpoint);
					 /////////////////////////////////////////////////////////////////////////////
					 pFrgnd_Measure_Aver_Dist_Yes_Stop->m_x.Format("%g", pFrgnd_Measure_Aver_Dist_Yes_Stop->x);
					 pFrgnd_Measure_Aver_Dist_Yes_Stop->m_y.Format("%g", pFrgnd_Measure_Aver_Dist_Yes_Stop->y);
					 pFrgnd_Measure_Aver_Dist_Yes_Stop->m_z.Format("%g", pFrgnd_Measure_Aver_Dist_Yes_Stop->z);				     
					 CString Measureb = "x="+pFrgnd_Measure_Aver_Dist_Yes_Stop->m_x+"      y="+pFrgnd_Measure_Aver_Dist_Yes_Stop->m_y+"      z="+pFrgnd_Measure_Aver_Dist_Yes_Stop->m_z;
					 pFrgnd_Measure_Aver_Dist_Yes_Stop->m_listeofmeasur.InsertString(0,Measureb);
					 pFrgnd_Measure_Aver_Dist_Yes_Stop->m_listeofmeasur.UpdateWindow();
				 }
			 }
             else
			 {
			 	 Ptpoint->X = pFrgnd_Measure_Aver_Dist_Yes_Stop->x;
				 Ptpoint->Y = pFrgnd_Measure_Aver_Dist_Yes_Stop->y;
				 Ptpoint->Z = pFrgnd_Measure_Aver_Dist_Yes_Stop->z;
				 traject.push_back(*Ptpoint);
				 /////////////////////////////////////////////////////////////////////////////
				 pFrgnd_Measure_Aver_Dist_Yes_Stop->m_x.Format("%g", pFrgnd_Measure_Aver_Dist_Yes_Stop->x);
				 pFrgnd_Measure_Aver_Dist_Yes_Stop->m_y.Format("%g", pFrgnd_Measure_Aver_Dist_Yes_Stop->y);
				 pFrgnd_Measure_Aver_Dist_Yes_Stop->m_z.Format("%g", pFrgnd_Measure_Aver_Dist_Yes_Stop->z);			     
				 CString Measureb = "x="+pFrgnd_Measure_Aver_Dist_Yes_Stop->m_x+"      y="+pFrgnd_Measure_Aver_Dist_Yes_Stop->m_y+"      z="+pFrgnd_Measure_Aver_Dist_Yes_Stop->m_z;
				 pFrgnd_Measure_Aver_Dist_Yes_Stop->m_listeofmeasur.InsertString(0,Measureb);
				 pFrgnd_Measure_Aver_Dist_Yes_Stop->m_listeofmeasur.UpdateWindow();				 
			 }				
		 }
		 catch(TrackerException* e )
		 {
			 char* msg;
			 msg = e->getText();
			 AfxMessageBox(msg);
			 delete e;
		 }
		 if (imaad2 == -1)
		 {
			 gpTracker->stopMeasurePoint();
			 break;
		 }
	 }
	 ExitThread(0); 
	 return 0;
}

//Foreground Measurement//StdDevFilter//DistanceTrigger//Yes Stop collec/////////////////////
DWORD WINAPI MyCritariaFrgnd_Measure_StdD_Dist_Yes_Stop( LPVOID pParam )
{
	CApplication_Faro_TrakerDlg* pFrgnd_Measure_StdD_Dist_Yes_Stop = (CApplication_Faro_TrakerDlg*)pParam;
	/////////////////////////////////////////////////////////////////////////////////////
	MeasurePointData* Frgnd_Measure_StdD_Dist_Yes_Stop = NULL;
	StdDevFilter filter;
	NullStartTrigger startTrigger;
	/////////////////////////////////////////////////////////////////////////////////////
	DistanceTrigger continueTrigger(pFrgnd_Measure_StdD_Dist_Yes_Stop->DitTrig);
	/////////////////////////////////////////////////////////////////////////////////////
	MeasureCfg  cfg( pFrgnd_Measure_StdD_Dist_Yes_Stop->SamPOb , &filter , &startTrigger , &continueTrigger);
	gpTracker->startMeasurePoint( &cfg );
	traject.clear();
	 while (true)
	 {
		 Point* Ptpoint;
	     Ptpoint = new Point();
		 try
		 {
			 Frgnd_Measure_StdD_Dist_Yes_Stop = gpTracker->readMeasurePointData();		
	         ASSERT(Frgnd_Measure_StdD_Dist_Yes_Stop);
	         LPARAM lParam = (LPARAM)Frgnd_Measure_StdD_Dist_Yes_Stop;
			 /////////////////////////////////////////////////////////////////////////////
			 pFrgnd_Measure_StdD_Dist_Yes_Stop->az     =     (Frgnd_Measure_StdD_Dist_Yes_Stop->azimuth());
			 pFrgnd_Measure_StdD_Dist_Yes_Stop->ze     =     (Frgnd_Measure_StdD_Dist_Yes_Stop->zenith());    
			 pFrgnd_Measure_StdD_Dist_Yes_Stop->di     =     (Frgnd_Measure_StdD_Dist_Yes_Stop->distance())*1000;
             /////////////////////////////////////////////////////////////////////////////
			 pFrgnd_Measure_StdD_Dist_Yes_Stop->x      =      pFrgnd_Measure_StdD_Dist_Yes_Stop->di*sin(pFrgnd_Measure_StdD_Dist_Yes_Stop->ze)*cos(pFrgnd_Measure_StdD_Dist_Yes_Stop->az);
			 pFrgnd_Measure_StdD_Dist_Yes_Stop->y      =      pFrgnd_Measure_StdD_Dist_Yes_Stop->di*sin(pFrgnd_Measure_StdD_Dist_Yes_Stop->ze)*sin(pFrgnd_Measure_StdD_Dist_Yes_Stop->az);
			 pFrgnd_Measure_StdD_Dist_Yes_Stop->z      =      pFrgnd_Measure_StdD_Dist_Yes_Stop->di*cos(pFrgnd_Measure_StdD_Dist_Yes_Stop->ze);
			 /////////////////////////////////////////////////////////////////////////////
			 Size_Collec_Stop = traject.size();
             if (Size_Collec_Stop > 0)
			 {
				 Distance_Point = sqrt((traject[Size_Collec_Stop - 1].X - pFrgnd_Measure_StdD_Dist_Yes_Stop->x)*(traject[Size_Collec_Stop - 1].X - pFrgnd_Measure_StdD_Dist_Yes_Stop->x)+(traject[Size_Collec_Stop - 1].Y - pFrgnd_Measure_StdD_Dist_Yes_Stop->y)*(traject[Size_Collec_Stop - 1].Y - pFrgnd_Measure_StdD_Dist_Yes_Stop->y)+(traject[Size_Collec_Stop - 1].Z - pFrgnd_Measure_StdD_Dist_Yes_Stop->z)*(traject[Size_Collec_Stop - 1].Z - pFrgnd_Measure_StdD_Dist_Yes_Stop->z));
				 if (Distance_Point > PARAM4)
				 {
				 	 Ptpoint->X = pFrgnd_Measure_StdD_Dist_Yes_Stop->x;
					 Ptpoint->Y = pFrgnd_Measure_StdD_Dist_Yes_Stop->y;
					 Ptpoint->Z = pFrgnd_Measure_StdD_Dist_Yes_Stop->z;
					 traject.push_back(*Ptpoint);
					 /////////////////////////////////////////////////////////////////////////////
					 pFrgnd_Measure_StdD_Dist_Yes_Stop->m_x.Format("%g", pFrgnd_Measure_StdD_Dist_Yes_Stop->x);
					 pFrgnd_Measure_StdD_Dist_Yes_Stop->m_y.Format("%g", pFrgnd_Measure_StdD_Dist_Yes_Stop->y);
					 pFrgnd_Measure_StdD_Dist_Yes_Stop->m_z.Format("%g", pFrgnd_Measure_StdD_Dist_Yes_Stop->z);				     
					 CString Measureb = "x="+pFrgnd_Measure_StdD_Dist_Yes_Stop->m_x+"      y="+pFrgnd_Measure_StdD_Dist_Yes_Stop->m_y+"      z="+pFrgnd_Measure_StdD_Dist_Yes_Stop->m_z;
					 pFrgnd_Measure_StdD_Dist_Yes_Stop->m_listeofmeasur.InsertString(0,Measureb);
					 pFrgnd_Measure_StdD_Dist_Yes_Stop->m_listeofmeasur.UpdateWindow();
				 }
			 }
             else
			 {
			 	 Ptpoint->X = pFrgnd_Measure_StdD_Dist_Yes_Stop->x;
				 Ptpoint->Y = pFrgnd_Measure_StdD_Dist_Yes_Stop->y;
				 Ptpoint->Z = pFrgnd_Measure_StdD_Dist_Yes_Stop->z;
				 traject.push_back(*Ptpoint);
				 /////////////////////////////////////////////////////////////////////////////
				 pFrgnd_Measure_StdD_Dist_Yes_Stop->m_x.Format("%g", pFrgnd_Measure_StdD_Dist_Yes_Stop->x);
				 pFrgnd_Measure_StdD_Dist_Yes_Stop->m_y.Format("%g", pFrgnd_Measure_StdD_Dist_Yes_Stop->y);
				 pFrgnd_Measure_StdD_Dist_Yes_Stop->m_z.Format("%g", pFrgnd_Measure_StdD_Dist_Yes_Stop->z);		     
				 CString Measureb = "x="+pFrgnd_Measure_StdD_Dist_Yes_Stop->m_x+"      y="+pFrgnd_Measure_StdD_Dist_Yes_Stop->m_y+"      z="+pFrgnd_Measure_StdD_Dist_Yes_Stop->m_z;
				 pFrgnd_Measure_StdD_Dist_Yes_Stop->m_listeofmeasur.InsertString(0,Measureb);
				 pFrgnd_Measure_StdD_Dist_Yes_Stop->m_listeofmeasur.UpdateWindow();			 
			 }			
		 }
		 catch(TrackerException* e )
		 {
			 char* msg;
			 msg = e->getText();
			 AfxMessageBox(msg);
			 delete e;
		 }
		 if (imaad2 == -1)
		 {
			 gpTracker->stopMeasurePoint();
			 break;
		 }
	 }
	 ExitThread(0); 
	 return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////Measurement a long a distance//////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
//Foreground Measurement//Average Filter//NullContinueTrigger//Yes Stop collec///////////////
DWORD WINAPI MyCritariaFrgnd_Measure_Aver_NullCont_Yes_1Stop( LPVOID pParam )
{
	CApplication_Faro_TrakerDlg* pFrgnd_Measure_Aver_NullCont_Yes_1Stop = (CApplication_Faro_TrakerDlg*)pParam;
	/////////////////////////////////////////////////////////////////////////////////////
	MeasurePointData* Frgnd_Measure_Aver_NullCont_Yes_1Stop = NULL;
	AverageFilter filter;
	NullStartTrigger startTrigger;
	/////////////////////////////////////////////////////////////////////////////////////
	NullContinueTrigger continueTrigger;
	/////////////////////////////////////////////////////////////////////////////////////
	MeasureCfg  cfg( pFrgnd_Measure_Aver_NullCont_Yes_1Stop->SamPOb , &filter , &startTrigger , &continueTrigger);
	gpTracker->startMeasurePoint( &cfg );
	traject.clear();
	while (true)
	 {
		 Point* Ptpoint;
	     Ptpoint = new Point();
		 try
		 {
			 Frgnd_Measure_Aver_NullCont_Yes_1Stop = gpTracker->readMeasurePointData();		
	         ASSERT(Frgnd_Measure_Aver_NullCont_Yes_1Stop);
	         LPARAM lParam = (LPARAM)Frgnd_Measure_Aver_NullCont_Yes_1Stop;
			 /////////////////////////////////////////////////////////////////////////////
			 pFrgnd_Measure_Aver_NullCont_Yes_1Stop->az     =     (Frgnd_Measure_Aver_NullCont_Yes_1Stop->azimuth());
			 pFrgnd_Measure_Aver_NullCont_Yes_1Stop->ze     =     (Frgnd_Measure_Aver_NullCont_Yes_1Stop->zenith());    
			 pFrgnd_Measure_Aver_NullCont_Yes_1Stop->di     =     (Frgnd_Measure_Aver_NullCont_Yes_1Stop->distance())*1000;
             /////////////////////////////////////////////////////////////////////////////	
			 pFrgnd_Measure_Aver_NullCont_Yes_1Stop->x      =      pFrgnd_Measure_Aver_NullCont_Yes_1Stop->di*sin(pFrgnd_Measure_Aver_NullCont_Yes_1Stop->ze)*cos(pFrgnd_Measure_Aver_NullCont_Yes_1Stop->az);
			 pFrgnd_Measure_Aver_NullCont_Yes_1Stop->y      =      pFrgnd_Measure_Aver_NullCont_Yes_1Stop->di*sin(pFrgnd_Measure_Aver_NullCont_Yes_1Stop->ze)*sin(pFrgnd_Measure_Aver_NullCont_Yes_1Stop->az);
			 pFrgnd_Measure_Aver_NullCont_Yes_1Stop->z      =      pFrgnd_Measure_Aver_NullCont_Yes_1Stop->di*cos(pFrgnd_Measure_Aver_NullCont_Yes_1Stop->ze);
			 /////////////////////////////////////////////////////////////////////////////		
			 Size_Collec_Stop = traject.size();
             if (Size_Collec_Stop > 0)
			 {
				 Distance_Point = sqrt((traject[0].X - pFrgnd_Measure_Aver_NullCont_Yes_1Stop->x)*(traject[0].X - pFrgnd_Measure_Aver_NullCont_Yes_1Stop->x)+(traject[0].Y - pFrgnd_Measure_Aver_NullCont_Yes_1Stop->y)*(traject[0].Y - pFrgnd_Measure_Aver_NullCont_Yes_1Stop->y)+(traject[0].Z - pFrgnd_Measure_Aver_NullCont_Yes_1Stop->z)*(traject[0].Z - pFrgnd_Measure_Aver_NullCont_Yes_1Stop->z));
				 if (Distance_Point > PARAM4 && Distance_Point < (PARAM3 - PARAM5))
				 {
				 	 Ptpoint->X = pFrgnd_Measure_Aver_NullCont_Yes_1Stop->x;
					 Ptpoint->Y = pFrgnd_Measure_Aver_NullCont_Yes_1Stop->y;
					 Ptpoint->Z = pFrgnd_Measure_Aver_NullCont_Yes_1Stop->z;
					 traject.push_back(*Ptpoint);
					 /////////////////////////////////////////////////////////////////////////////
					 pFrgnd_Measure_Aver_NullCont_Yes_1Stop->m_x.Format("%g", pFrgnd_Measure_Aver_NullCont_Yes_1Stop->x);
					 pFrgnd_Measure_Aver_NullCont_Yes_1Stop->m_y.Format("%g", pFrgnd_Measure_Aver_NullCont_Yes_1Stop->y);
					 pFrgnd_Measure_Aver_NullCont_Yes_1Stop->m_z.Format("%g", pFrgnd_Measure_Aver_NullCont_Yes_1Stop->z);					     
					 CString Measureb = "x="+pFrgnd_Measure_Aver_NullCont_Yes_1Stop->m_x+"      y="+pFrgnd_Measure_Aver_NullCont_Yes_1Stop->m_y+"      z="+pFrgnd_Measure_Aver_NullCont_Yes_1Stop->m_z;
					 pFrgnd_Measure_Aver_NullCont_Yes_1Stop->m_listeofmeasur.InsertString(0,Measureb);
					 pFrgnd_Measure_Aver_NullCont_Yes_1Stop->m_listeofmeasur.UpdateWindow();
				 }
			 }
			 if (Size_Collec_Stop == 0)
			 {
			 	 Ptpoint->X = pFrgnd_Measure_Aver_NullCont_Yes_1Stop->x;
				 Ptpoint->Y = pFrgnd_Measure_Aver_NullCont_Yes_1Stop->y;
				 Ptpoint->Z = pFrgnd_Measure_Aver_NullCont_Yes_1Stop->z;
				 traject.push_back(*Ptpoint);
				 /////////////////////////////////////////////////////////////////////////////
				 pFrgnd_Measure_Aver_NullCont_Yes_1Stop->m_x.Format("%g", pFrgnd_Measure_Aver_NullCont_Yes_1Stop->x);
				 pFrgnd_Measure_Aver_NullCont_Yes_1Stop->m_y.Format("%g", pFrgnd_Measure_Aver_NullCont_Yes_1Stop->y);
				 pFrgnd_Measure_Aver_NullCont_Yes_1Stop->m_z.Format("%g", pFrgnd_Measure_Aver_NullCont_Yes_1Stop->z);			     
				 CString Measureb = "x="+pFrgnd_Measure_Aver_NullCont_Yes_1Stop->m_x+"      y="+pFrgnd_Measure_Aver_NullCont_Yes_1Stop->m_y+"      z="+pFrgnd_Measure_Aver_NullCont_Yes_1Stop->m_z;
				 pFrgnd_Measure_Aver_NullCont_Yes_1Stop->m_listeofmeasur.InsertString(0,Measureb);
				 pFrgnd_Measure_Aver_NullCont_Yes_1Stop->m_listeofmeasur.UpdateWindow();				 
			 }	
		 }
		 catch(TrackerException* e )
		 {
			 char* msg;
			 msg = e->getText();
			 AfxMessageBox(msg);
			 delete e;
		 }
		 if (imaad2 == -1)
		 {
			 gpTracker->stopMeasurePoint();
			 break;
		 }
	 }
	 ExitThread(0); 
	 return 0;
}

//Foreground Measurement//StdDevFilter//NullContinueTrigger//Yes Stop collec/////////////////
DWORD WINAPI MyCritariaFrgnd_Measure_StdD_NullCont_Yes_1Stop( LPVOID pParam )
{
	CApplication_Faro_TrakerDlg* pFrgnd_Measure_StdD_NullCont_Yes_1Stop = (CApplication_Faro_TrakerDlg*)pParam;
	/////////////////////////////////////////////////////////////////////////////////////
	MeasurePointData* Frgnd_Measure_StdD_NullCont_Yes_1Stop = NULL;
	StdDevFilter filter;
	NullStartTrigger startTrigger;
	/////////////////////////////////////////////////////////////////////////////////////
	NullContinueTrigger continueTrigger;
	/////////////////////////////////////////////////////////////////////////////////////
	MeasureCfg  cfg( pFrgnd_Measure_StdD_NullCont_Yes_1Stop->SamPOb , &filter , &startTrigger , &continueTrigger);
	gpTracker->startMeasurePoint( &cfg );
	traject.clear();
	 while (true)
	 {
		 Point* Ptpoint;
	     Ptpoint = new Point();
		 try
		 {
			 Frgnd_Measure_StdD_NullCont_Yes_1Stop = gpTracker->readMeasurePointData();		
	         ASSERT(Frgnd_Measure_StdD_NullCont_Yes_1Stop);
	         LPARAM lParam = (LPARAM)Frgnd_Measure_StdD_NullCont_Yes_1Stop;
			 /////////////////////////////////////////////////////////////////////////////
			 pFrgnd_Measure_StdD_NullCont_Yes_1Stop->az     =     (Frgnd_Measure_StdD_NullCont_Yes_1Stop->azimuth());
			 pFrgnd_Measure_StdD_NullCont_Yes_1Stop->ze     =     (Frgnd_Measure_StdD_NullCont_Yes_1Stop->zenith());    
			 pFrgnd_Measure_StdD_NullCont_Yes_1Stop->di     =     (Frgnd_Measure_StdD_NullCont_Yes_1Stop->distance())*1000;
             /////////////////////////////////////////////////////////////////////////////		
			 pFrgnd_Measure_StdD_NullCont_Yes_1Stop->x      =      pFrgnd_Measure_StdD_NullCont_Yes_1Stop->di*sin(pFrgnd_Measure_StdD_NullCont_Yes_1Stop->ze)*cos(pFrgnd_Measure_StdD_NullCont_Yes_1Stop->az);
			 pFrgnd_Measure_StdD_NullCont_Yes_1Stop->y      =      pFrgnd_Measure_StdD_NullCont_Yes_1Stop->di*sin(pFrgnd_Measure_StdD_NullCont_Yes_1Stop->ze)*sin(pFrgnd_Measure_StdD_NullCont_Yes_1Stop->az);
			 pFrgnd_Measure_StdD_NullCont_Yes_1Stop->z      =      pFrgnd_Measure_StdD_NullCont_Yes_1Stop->di*cos(pFrgnd_Measure_StdD_NullCont_Yes_1Stop->ze);
			 /////////////////////////////////////////////////////////////////////////////
			 Size_Collec_Stop = traject.size();
             if (Size_Collec_Stop > 0)
			 {
				 Distance_Point = sqrt((traject[0].X - pFrgnd_Measure_StdD_NullCont_Yes_1Stop->x)*(traject[0].X - pFrgnd_Measure_StdD_NullCont_Yes_1Stop->x)+(traject[0].Y - pFrgnd_Measure_StdD_NullCont_Yes_1Stop->y)*(traject[0].Y - pFrgnd_Measure_StdD_NullCont_Yes_1Stop->y)+(traject[0].Z - pFrgnd_Measure_StdD_NullCont_Yes_1Stop->z)*(traject[0].Z - pFrgnd_Measure_StdD_NullCont_Yes_1Stop->z));
				 if (Distance_Point > PARAM4 && Distance_Point < (PARAM3 - PARAM5))
				 {
				 	 Ptpoint->X = pFrgnd_Measure_StdD_NullCont_Yes_1Stop->x;
					 Ptpoint->Y = pFrgnd_Measure_StdD_NullCont_Yes_1Stop->y;
					 Ptpoint->Z = pFrgnd_Measure_StdD_NullCont_Yes_1Stop->z;
					 traject.push_back(*Ptpoint);
					 /////////////////////////////////////////////////////////////////////////////
					 pFrgnd_Measure_StdD_NullCont_Yes_1Stop->m_x.Format("%g", pFrgnd_Measure_StdD_NullCont_Yes_1Stop->x);
					 pFrgnd_Measure_StdD_NullCont_Yes_1Stop->m_y.Format("%g", pFrgnd_Measure_StdD_NullCont_Yes_1Stop->y);
					 pFrgnd_Measure_StdD_NullCont_Yes_1Stop->m_z.Format("%g", pFrgnd_Measure_StdD_NullCont_Yes_1Stop->z);				     
					 CString Measureb = "x="+pFrgnd_Measure_StdD_NullCont_Yes_1Stop->m_x+"      y="+pFrgnd_Measure_StdD_NullCont_Yes_1Stop->m_y+"      z="+pFrgnd_Measure_StdD_NullCont_Yes_1Stop->m_z;
					 pFrgnd_Measure_StdD_NullCont_Yes_1Stop->m_listeofmeasur.InsertString(0,Measureb);
					 pFrgnd_Measure_StdD_NullCont_Yes_1Stop->m_listeofmeasur.UpdateWindow();
				 }
			 }
			 if (Size_Collec_Stop == 0)
			 {
			 	 Ptpoint->X = pFrgnd_Measure_StdD_NullCont_Yes_1Stop->x;
				 Ptpoint->Y = pFrgnd_Measure_StdD_NullCont_Yes_1Stop->y;
				 Ptpoint->Z = pFrgnd_Measure_StdD_NullCont_Yes_1Stop->z;
				 traject.push_back(*Ptpoint);
				 /////////////////////////////////////////////////////////////////////////////
				 pFrgnd_Measure_StdD_NullCont_Yes_1Stop->m_x.Format("%g", pFrgnd_Measure_StdD_NullCont_Yes_1Stop->x);
				 pFrgnd_Measure_StdD_NullCont_Yes_1Stop->m_y.Format("%g", pFrgnd_Measure_StdD_NullCont_Yes_1Stop->y);
				 pFrgnd_Measure_StdD_NullCont_Yes_1Stop->m_z.Format("%g", pFrgnd_Measure_StdD_NullCont_Yes_1Stop->z);		     
				 CString Measureb = "x="+pFrgnd_Measure_StdD_NullCont_Yes_1Stop->m_x+"      y="+pFrgnd_Measure_StdD_NullCont_Yes_1Stop->m_y+"      z="+pFrgnd_Measure_StdD_NullCont_Yes_1Stop->m_z;
				 pFrgnd_Measure_StdD_NullCont_Yes_1Stop->m_listeofmeasur.InsertString(0,Measureb);
				 pFrgnd_Measure_StdD_NullCont_Yes_1Stop->m_listeofmeasur.UpdateWindow();				 
			 }
		 }
		 catch(TrackerException* e )
		 {
			 char* msg;
			 msg = e->getText();
			 AfxMessageBox(msg);
			 delete e;
		 }
		 if (imaad2 == -1)
		 {
			 gpTracker->stopMeasurePoint();
			 break;
		 }
	 }
	 ExitThread(0); 
	 return 0;
}

//Foreground Measurement//Average Filter//IntervalTrigger//Yes Stop collec///////////////////
DWORD WINAPI MyCritariaFrgnd_Measure_Aver_Inter_Yes_1Stop( LPVOID pParam )
{
	CApplication_Faro_TrakerDlg* pFrgnd_Measure_Aver_Inter_Yes_1Stop = (CApplication_Faro_TrakerDlg*)pParam;
	/////////////////////////////////////////////////////////////////////////////////////
	MeasurePointData* Frgnd_Measure_Aver_Inter_Yes_1Stop = NULL;
	AverageFilter filter;
	NullStartTrigger startTrigger;
	/////////////////////////////////////////////////////////////////////////////////////
	IntervalTrigger continueTrigger(pFrgnd_Measure_Aver_Inter_Yes_1Stop->InterTrig);
	/////////////////////////////////////////////////////////////////////////////////////
	MeasureCfg  cfg( pFrgnd_Measure_Aver_Inter_Yes_1Stop->SamPOb , &filter , &startTrigger , &continueTrigger);
	gpTracker->startMeasurePoint( &cfg );
	traject.clear();
	 while (true)
	 {
		 Point* Ptpoint;
	     Ptpoint = new Point();
		 try
		 {
			 Frgnd_Measure_Aver_Inter_Yes_1Stop = gpTracker->readMeasurePointData();		
	         ASSERT(Frgnd_Measure_Aver_Inter_Yes_1Stop);
	         LPARAM lParam = (LPARAM)Frgnd_Measure_Aver_Inter_Yes_1Stop;
			 /////////////////////////////////////////////////////////////////////////////
			 pFrgnd_Measure_Aver_Inter_Yes_1Stop->az     =     (Frgnd_Measure_Aver_Inter_Yes_1Stop->azimuth());
			 pFrgnd_Measure_Aver_Inter_Yes_1Stop->ze     =     (Frgnd_Measure_Aver_Inter_Yes_1Stop->zenith());    
			 pFrgnd_Measure_Aver_Inter_Yes_1Stop->di     =     (Frgnd_Measure_Aver_Inter_Yes_1Stop->distance())*1000;
             /////////////////////////////////////////////////////////////////////////////
			 pFrgnd_Measure_Aver_Inter_Yes_1Stop->x      =      pFrgnd_Measure_Aver_Inter_Yes_1Stop->di*sin(pFrgnd_Measure_Aver_Inter_Yes_1Stop->ze)*cos(pFrgnd_Measure_Aver_Inter_Yes_1Stop->az);
			 pFrgnd_Measure_Aver_Inter_Yes_1Stop->y      =      pFrgnd_Measure_Aver_Inter_Yes_1Stop->di*sin(pFrgnd_Measure_Aver_Inter_Yes_1Stop->ze)*sin(pFrgnd_Measure_Aver_Inter_Yes_1Stop->az);
			 pFrgnd_Measure_Aver_Inter_Yes_1Stop->z      =      pFrgnd_Measure_Aver_Inter_Yes_1Stop->di*cos(pFrgnd_Measure_Aver_Inter_Yes_1Stop->ze);
			 /////////////////////////////////////////////////////////////////////////////
			 Size_Collec_Stop = traject.size();
             if (Size_Collec_Stop > 0)
			 {
				 Distance_Point = sqrt((traject[0].X - pFrgnd_Measure_Aver_Inter_Yes_1Stop->x)*(traject[0].X - pFrgnd_Measure_Aver_Inter_Yes_1Stop->x)+(traject[0].Y - pFrgnd_Measure_Aver_Inter_Yes_1Stop->y)*(traject[0].Y - pFrgnd_Measure_Aver_Inter_Yes_1Stop->y)+(traject[0].Z - pFrgnd_Measure_Aver_Inter_Yes_1Stop->z)*(traject[0].Z - pFrgnd_Measure_Aver_Inter_Yes_1Stop->z));
				 if (Distance_Point > PARAM4 && Distance_Point < (PARAM3 - PARAM5))
				 {
				 	 Ptpoint->X = pFrgnd_Measure_Aver_Inter_Yes_1Stop->x;
					 Ptpoint->Y = pFrgnd_Measure_Aver_Inter_Yes_1Stop->y;
					 Ptpoint->Z = pFrgnd_Measure_Aver_Inter_Yes_1Stop->z;
					 traject.push_back(*Ptpoint);
					 /////////////////////////////////////////////////////////////////////////////
					 pFrgnd_Measure_Aver_Inter_Yes_1Stop->m_x.Format("%g", pFrgnd_Measure_Aver_Inter_Yes_1Stop->x);
					 pFrgnd_Measure_Aver_Inter_Yes_1Stop->m_y.Format("%g", pFrgnd_Measure_Aver_Inter_Yes_1Stop->y);
					 pFrgnd_Measure_Aver_Inter_Yes_1Stop->m_z.Format("%g", pFrgnd_Measure_Aver_Inter_Yes_1Stop->z);					     
					 CString Measureb = "x="+pFrgnd_Measure_Aver_Inter_Yes_1Stop->m_x+"      y="+pFrgnd_Measure_Aver_Inter_Yes_1Stop->m_y+"      z="+pFrgnd_Measure_Aver_Inter_Yes_1Stop->m_z;
					 pFrgnd_Measure_Aver_Inter_Yes_1Stop->m_listeofmeasur.InsertString(0,Measureb);
					 pFrgnd_Measure_Aver_Inter_Yes_1Stop->m_listeofmeasur.UpdateWindow();
				 }
                 if (Distance_Point > PARAM3 && traject.size() < 10)
                 {          
                     gpTracker->stopMeasurePoint();
                     pFrgnd_Measure_Aver_Inter_Yes_1Stop->SendERROR("5533","1112");
                     break; 
                 }
			 }
			 if (Size_Collec_Stop == 0)
			 {
			 	 Ptpoint->X = pFrgnd_Measure_Aver_Inter_Yes_1Stop->x;
				 Ptpoint->Y = pFrgnd_Measure_Aver_Inter_Yes_1Stop->y;
				 Ptpoint->Z = pFrgnd_Measure_Aver_Inter_Yes_1Stop->z;
				 traject.push_back(*Ptpoint);
				 /////////////////////////////////////////////////////////////////////////////
				 pFrgnd_Measure_Aver_Inter_Yes_1Stop->m_x.Format("%g", pFrgnd_Measure_Aver_Inter_Yes_1Stop->x);
				 pFrgnd_Measure_Aver_Inter_Yes_1Stop->m_y.Format("%g", pFrgnd_Measure_Aver_Inter_Yes_1Stop->y);
				 pFrgnd_Measure_Aver_Inter_Yes_1Stop->m_z.Format("%g", pFrgnd_Measure_Aver_Inter_Yes_1Stop->z);		     
				 CString Measureb = "x="+pFrgnd_Measure_Aver_Inter_Yes_1Stop->m_x+"      y="+pFrgnd_Measure_Aver_Inter_Yes_1Stop->m_y+"      z="+pFrgnd_Measure_Aver_Inter_Yes_1Stop->m_z;
				 pFrgnd_Measure_Aver_Inter_Yes_1Stop->m_listeofmeasur.InsertString(0,Measureb);
				 pFrgnd_Measure_Aver_Inter_Yes_1Stop->m_listeofmeasur.UpdateWindow();				 
			 }	
		 }
		 catch(TrackerException* e )
		 {
			 char* msg;
			 msg = e->getText();
			 AfxMessageBox(msg);
			 delete e;
		 }
		 if (imaad2 == -1)
		 {
			 gpTracker->stopMeasurePoint();
			 break;
		 }
	 }
	 ExitThread(0); 
	 return 0;
}

//Foreground Measurement//StdDevFilter//IntervalTrigger//Yes Stop collec/////////////////////
DWORD WINAPI MyCritariaFrgnd_Measure_StdD_Inter_Yes_1Stop( LPVOID pParam )
{
	CApplication_Faro_TrakerDlg* pFrgnd_Measure_StdD_Inter_Yes_1Stop = (CApplication_Faro_TrakerDlg*)pParam;
	/////////////////////////////////////////////////////////////////////////////////////
	MeasurePointData* Frgnd_Measure_StdD_Inter_Yes_1Stop = NULL;
	StdDevFilter filter;
	NullStartTrigger startTrigger;
	/////////////////////////////////////////////////////////////////////////////////////
	IntervalTrigger continueTrigger(pFrgnd_Measure_StdD_Inter_Yes_1Stop->InterTrig);
	/////////////////////////////////////////////////////////////////////////////////////
	MeasureCfg  cfg( pFrgnd_Measure_StdD_Inter_Yes_1Stop->SamPOb , &filter , &startTrigger , &continueTrigger);
	gpTracker->startMeasurePoint( &cfg );
	traject.clear();
	 while (true)
	 {
		 Point* Ptpoint;
	     Ptpoint = new Point();
		 try
		 {
			 Frgnd_Measure_StdD_Inter_Yes_1Stop = gpTracker->readMeasurePointData();		
	         ASSERT(Frgnd_Measure_StdD_Inter_Yes_1Stop);
	         LPARAM lParam = (LPARAM)Frgnd_Measure_StdD_Inter_Yes_1Stop;
			 /////////////////////////////////////////////////////////////////////////////
			 pFrgnd_Measure_StdD_Inter_Yes_1Stop->az     =     (Frgnd_Measure_StdD_Inter_Yes_1Stop->azimuth());
			 pFrgnd_Measure_StdD_Inter_Yes_1Stop->ze     =     (Frgnd_Measure_StdD_Inter_Yes_1Stop->zenith());    
			 pFrgnd_Measure_StdD_Inter_Yes_1Stop->di     =     (Frgnd_Measure_StdD_Inter_Yes_1Stop->distance())*1000;
             /////////////////////////////////////////////////////////////////////////////
			 pFrgnd_Measure_StdD_Inter_Yes_1Stop->x      =      pFrgnd_Measure_StdD_Inter_Yes_1Stop->di*sin(pFrgnd_Measure_StdD_Inter_Yes_1Stop->ze)*cos(pFrgnd_Measure_StdD_Inter_Yes_1Stop->az);
			 pFrgnd_Measure_StdD_Inter_Yes_1Stop->y      =      pFrgnd_Measure_StdD_Inter_Yes_1Stop->di*sin(pFrgnd_Measure_StdD_Inter_Yes_1Stop->ze)*sin(pFrgnd_Measure_StdD_Inter_Yes_1Stop->az);
			 pFrgnd_Measure_StdD_Inter_Yes_1Stop->z      =      pFrgnd_Measure_StdD_Inter_Yes_1Stop->di*cos(pFrgnd_Measure_StdD_Inter_Yes_1Stop->ze);
			 /////////////////////////////////////////////////////////////////////////////
			 Size_Collec_Stop = traject.size();
             if (Size_Collec_Stop > 0)
			 {
				 Distance_Point = sqrt((traject[0].X - pFrgnd_Measure_StdD_Inter_Yes_1Stop->x)*(traject[0].X - pFrgnd_Measure_StdD_Inter_Yes_1Stop->x)+(traject[0].Y - pFrgnd_Measure_StdD_Inter_Yes_1Stop->y)*(traject[0].Y - pFrgnd_Measure_StdD_Inter_Yes_1Stop->y)+(traject[0].Z - pFrgnd_Measure_StdD_Inter_Yes_1Stop->z)*(traject[0].Z - pFrgnd_Measure_StdD_Inter_Yes_1Stop->z));
				 if (Distance_Point > PARAM4 && Distance_Point < (PARAM3 - PARAM5))
				 {
				 	 Ptpoint->X = pFrgnd_Measure_StdD_Inter_Yes_1Stop->x;
					 Ptpoint->Y = pFrgnd_Measure_StdD_Inter_Yes_1Stop->y;
					 Ptpoint->Z = pFrgnd_Measure_StdD_Inter_Yes_1Stop->z;
					 traject.push_back(*Ptpoint);
					 /////////////////////////////////////////////////////////////////////////////
					 pFrgnd_Measure_StdD_Inter_Yes_1Stop->m_x.Format("%g", pFrgnd_Measure_StdD_Inter_Yes_1Stop->x);
					 pFrgnd_Measure_StdD_Inter_Yes_1Stop->m_y.Format("%g", pFrgnd_Measure_StdD_Inter_Yes_1Stop->y);
					 pFrgnd_Measure_StdD_Inter_Yes_1Stop->m_z.Format("%g", pFrgnd_Measure_StdD_Inter_Yes_1Stop->z);				     
					 CString Measureb = "x="+pFrgnd_Measure_StdD_Inter_Yes_1Stop->m_x+"      y="+pFrgnd_Measure_StdD_Inter_Yes_1Stop->m_y+"      z="+pFrgnd_Measure_StdD_Inter_Yes_1Stop->m_z;
					 pFrgnd_Measure_StdD_Inter_Yes_1Stop->m_listeofmeasur.InsertString(0,Measureb);
					 pFrgnd_Measure_StdD_Inter_Yes_1Stop->m_listeofmeasur.UpdateWindow();
				 }
                 if (Distance_Point > PARAM3 && traject.size() < 10)
                 {          
                     gpTracker->stopMeasurePoint();
                     pFrgnd_Measure_StdD_Inter_Yes_1Stop->SendERROR("5533","1112");
                     break; 
                 }
			 }
			 if (Size_Collec_Stop == 0)
			 {
			 	 Ptpoint->X = pFrgnd_Measure_StdD_Inter_Yes_1Stop->x;
				 Ptpoint->Y = pFrgnd_Measure_StdD_Inter_Yes_1Stop->y;
				 Ptpoint->Z = pFrgnd_Measure_StdD_Inter_Yes_1Stop->z;
				 traject.push_back(*Ptpoint);
				 /////////////////////////////////////////////////////////////////////////////
				 pFrgnd_Measure_StdD_Inter_Yes_1Stop->m_x.Format("%g", pFrgnd_Measure_StdD_Inter_Yes_1Stop->x);
				 pFrgnd_Measure_StdD_Inter_Yes_1Stop->m_y.Format("%g", pFrgnd_Measure_StdD_Inter_Yes_1Stop->y);
				 pFrgnd_Measure_StdD_Inter_Yes_1Stop->m_z.Format("%g", pFrgnd_Measure_StdD_Inter_Yes_1Stop->z);	     
				 CString Measureb = "x="+pFrgnd_Measure_StdD_Inter_Yes_1Stop->m_x+"      y="+pFrgnd_Measure_StdD_Inter_Yes_1Stop->m_y+"      z="+pFrgnd_Measure_StdD_Inter_Yes_1Stop->m_z;
				 pFrgnd_Measure_StdD_Inter_Yes_1Stop->m_listeofmeasur.InsertString(0,Measureb);
				 pFrgnd_Measure_StdD_Inter_Yes_1Stop->m_listeofmeasur.UpdateWindow();			 
			 }	
		 }
		 catch(TrackerException* e )
		 {
			 char* msg;
			 msg = e->getText();
			 AfxMessageBox(msg);
			 delete e;
		 }
		 if (imaad2 == -1)
		 {
			 gpTracker->stopMeasurePoint();
			 break;
		 }
	 }
	 ExitThread(0); 
	 return 0;
}

//Foreground Measurement//Average Filter//DistanceTrigger//Yes Stop collec///////////////////
DWORD WINAPI MyCritariaFrgnd_Measure_Aver_Dist_Yes_1Stop( LPVOID pParam )
{
	CApplication_Faro_TrakerDlg* pFrgnd_Measure_Aver_Dist_Yes_1Stop = (CApplication_Faro_TrakerDlg*)pParam;
	/////////////////////////////////////////////////////////////////////////////////////

	MeasurePointData* Frgnd_Measure_Aver_Dist_Yes_1Stop = NULL;
	AverageFilter filter;
	NullStartTrigger startTrigger;
	/////////////////////////////////////////////////////////////////////////////////////
	DistanceTrigger continueTrigger(pFrgnd_Measure_Aver_Dist_Yes_1Stop->DitTrig);
	/////////////////////////////////////////////////////////////////////////////////////
	MeasureCfg  cfg( pFrgnd_Measure_Aver_Dist_Yes_1Stop->SamPOb , &filter , &startTrigger , &continueTrigger);
	gpTracker->startMeasurePoint( &cfg );
	traject.clear();

	 while (true)
	 {
		 Point* Ptpoint;
	     Ptpoint = new Point();
		 try
		 {
			 Frgnd_Measure_Aver_Dist_Yes_1Stop = gpTracker->readMeasurePointData();		
	         ASSERT(Frgnd_Measure_Aver_Dist_Yes_1Stop);
	         LPARAM lParam = (LPARAM)Frgnd_Measure_Aver_Dist_Yes_1Stop;
			 /////////////////////////////////////////////////////////////////////////////
			 pFrgnd_Measure_Aver_Dist_Yes_1Stop->az     =     (Frgnd_Measure_Aver_Dist_Yes_1Stop->azimuth());
			 pFrgnd_Measure_Aver_Dist_Yes_1Stop->ze     =     (Frgnd_Measure_Aver_Dist_Yes_1Stop->zenith());    
			 pFrgnd_Measure_Aver_Dist_Yes_1Stop->di     =     (Frgnd_Measure_Aver_Dist_Yes_1Stop->distance())*1000;
             /////////////////////////////////////////////////////////////////////////////
			 pFrgnd_Measure_Aver_Dist_Yes_1Stop->x      =      pFrgnd_Measure_Aver_Dist_Yes_1Stop->di*sin(pFrgnd_Measure_Aver_Dist_Yes_1Stop->ze)*cos(pFrgnd_Measure_Aver_Dist_Yes_1Stop->az);
			 pFrgnd_Measure_Aver_Dist_Yes_1Stop->y      =      pFrgnd_Measure_Aver_Dist_Yes_1Stop->di*sin(pFrgnd_Measure_Aver_Dist_Yes_1Stop->ze)*sin(pFrgnd_Measure_Aver_Dist_Yes_1Stop->az);
			 pFrgnd_Measure_Aver_Dist_Yes_1Stop->z      =      pFrgnd_Measure_Aver_Dist_Yes_1Stop->di*cos(pFrgnd_Measure_Aver_Dist_Yes_1Stop->ze);
			 /////////////////////////////////////////////////////////////////////////////
			 Size_Collec_Stop = traject.size();
             if (Size_Collec_Stop > 0)
			 {
				 Distance_Point = sqrt((traject[0].X - pFrgnd_Measure_Aver_Dist_Yes_1Stop->x)*(traject[0].X - pFrgnd_Measure_Aver_Dist_Yes_1Stop->x)+(traject[0].Y - pFrgnd_Measure_Aver_Dist_Yes_1Stop->y)*(traject[0].Y - pFrgnd_Measure_Aver_Dist_Yes_1Stop->y)+(traject[0].Z - pFrgnd_Measure_Aver_Dist_Yes_1Stop->z)*(traject[0].Z - pFrgnd_Measure_Aver_Dist_Yes_1Stop->z));
				 if (Distance_Point > PARAM4 && Distance_Point < (PARAM3 - PARAM5))
				 {
				 	 Ptpoint->X = pFrgnd_Measure_Aver_Dist_Yes_1Stop->x;
					 Ptpoint->Y = pFrgnd_Measure_Aver_Dist_Yes_1Stop->y;
					 Ptpoint->Z = pFrgnd_Measure_Aver_Dist_Yes_1Stop->z;
					 traject.push_back(*Ptpoint);
					 /////////////////////////////////////////////////////////////////////////////
					 pFrgnd_Measure_Aver_Dist_Yes_1Stop->m_x.Format("%g", pFrgnd_Measure_Aver_Dist_Yes_1Stop->x);
					 pFrgnd_Measure_Aver_Dist_Yes_1Stop->m_y.Format("%g", pFrgnd_Measure_Aver_Dist_Yes_1Stop->y);
					 pFrgnd_Measure_Aver_Dist_Yes_1Stop->m_z.Format("%g", pFrgnd_Measure_Aver_Dist_Yes_1Stop->z);			     
					 CString Measureb = "x="+pFrgnd_Measure_Aver_Dist_Yes_1Stop->m_x+"      y="+pFrgnd_Measure_Aver_Dist_Yes_1Stop->m_y+"      z="+pFrgnd_Measure_Aver_Dist_Yes_1Stop->m_z;
					 pFrgnd_Measure_Aver_Dist_Yes_1Stop->m_listeofmeasur.InsertString(0,Measureb);
					 pFrgnd_Measure_Aver_Dist_Yes_1Stop->m_listeofmeasur.UpdateWindow();
				 }
                 if (Distance_Point > PARAM3 && traject.size() < 10)
                 {          
                     gpTracker->stopMeasurePoint();
                     pFrgnd_Measure_Aver_Dist_Yes_1Stop->SendERROR("5533","1111");
                     break; 
                 }
			 }
			 if (Size_Collec_Stop == 0)
			 {
			 	 Ptpoint->X = pFrgnd_Measure_Aver_Dist_Yes_1Stop->x;
				 Ptpoint->Y = pFrgnd_Measure_Aver_Dist_Yes_1Stop->y;
				 Ptpoint->Z = pFrgnd_Measure_Aver_Dist_Yes_1Stop->z;
				 traject.push_back(*Ptpoint);
				 /////////////////////////////////////////////////////////////////////////////
				 pFrgnd_Measure_Aver_Dist_Yes_1Stop->m_x.Format("%g", pFrgnd_Measure_Aver_Dist_Yes_1Stop->x);
				 pFrgnd_Measure_Aver_Dist_Yes_1Stop->m_y.Format("%g", pFrgnd_Measure_Aver_Dist_Yes_1Stop->y);
				 pFrgnd_Measure_Aver_Dist_Yes_1Stop->m_z.Format("%g", pFrgnd_Measure_Aver_Dist_Yes_1Stop->z);			     
				 CString Measureb = "x="+pFrgnd_Measure_Aver_Dist_Yes_1Stop->m_x+"      y="+pFrgnd_Measure_Aver_Dist_Yes_1Stop->m_y+"      z="+pFrgnd_Measure_Aver_Dist_Yes_1Stop->m_z;
				 pFrgnd_Measure_Aver_Dist_Yes_1Stop->m_listeofmeasur.InsertString(0,Measureb);
				 pFrgnd_Measure_Aver_Dist_Yes_1Stop->m_listeofmeasur.UpdateWindow();			 
			 }	
		 }
		 catch(TrackerException* e )
		 {
			 char* msg;
			 msg = e->getText();
			 AfxMessageBox(msg);
			 delete e;
		 }
		 if (imaad2 == -1)
		 {
			 gpTracker->stopMeasurePoint();
			 break;
		 }
	 }
	 ExitThread(0); 
	 return 0;
}

//Foreground Measurement//StdDevFilter//DistanceTrigger//Yes Stop collec/////////////////////
DWORD WINAPI MyCritariaFrgnd_Measure_StdD_Dist_Yes_1Stop( LPVOID pParam )
{
	CApplication_Faro_TrakerDlg* pFrgnd_Measure_StdD_Dist_Yes_1Stop = (CApplication_Faro_TrakerDlg*)pParam;
	/////////////////////////////////////////////////////////////////////////////////////

	MeasurePointData* Frgnd_Measure_StdD_Dist_Yes_1Stop = NULL;
	StdDevFilter filter;
	NullStartTrigger startTrigger;
	/////////////////////////////////////////////////////////////////////////////////////
	DistanceTrigger continueTrigger(pFrgnd_Measure_StdD_Dist_Yes_1Stop->DitTrig);
	/////////////////////////////////////////////////////////////////////////////////////
	MeasureCfg  cfg( pFrgnd_Measure_StdD_Dist_Yes_1Stop->SamPOb , &filter , &startTrigger , &continueTrigger);
	gpTracker->startMeasurePoint( &cfg );
	traject.clear();

	 while (true)
	 {
		 Point* Ptpoint;
	     Ptpoint = new Point();
		 try
		 {
			 Frgnd_Measure_StdD_Dist_Yes_1Stop = gpTracker->readMeasurePointData();		
	         ASSERT(Frgnd_Measure_StdD_Dist_Yes_1Stop);
	         LPARAM lParam = (LPARAM)Frgnd_Measure_StdD_Dist_Yes_1Stop;
			 /////////////////////////////////////////////////////////////////////////////
			 pFrgnd_Measure_StdD_Dist_Yes_1Stop->az     =     (Frgnd_Measure_StdD_Dist_Yes_1Stop->azimuth());
			 pFrgnd_Measure_StdD_Dist_Yes_1Stop->ze     =     (Frgnd_Measure_StdD_Dist_Yes_1Stop->zenith());    
			 pFrgnd_Measure_StdD_Dist_Yes_1Stop->di     =     (Frgnd_Measure_StdD_Dist_Yes_1Stop->distance())*1000;
             /////////////////////////////////////////////////////////////////////////////
			 pFrgnd_Measure_StdD_Dist_Yes_1Stop->x      =      pFrgnd_Measure_StdD_Dist_Yes_1Stop->di*sin(pFrgnd_Measure_StdD_Dist_Yes_1Stop->ze)*cos(pFrgnd_Measure_StdD_Dist_Yes_1Stop->az);
			 pFrgnd_Measure_StdD_Dist_Yes_1Stop->y      =      pFrgnd_Measure_StdD_Dist_Yes_1Stop->di*sin(pFrgnd_Measure_StdD_Dist_Yes_1Stop->ze)*sin(pFrgnd_Measure_StdD_Dist_Yes_1Stop->az);
			 pFrgnd_Measure_StdD_Dist_Yes_1Stop->z      =      pFrgnd_Measure_StdD_Dist_Yes_1Stop->di*cos(pFrgnd_Measure_StdD_Dist_Yes_1Stop->ze);
			 /////////////////////////////////////////////////////////////////////////////
			 Size_Collec_Stop = traject.size();
             if (Size_Collec_Stop > 0)
			 {
				 Distance_Point = sqrt((traject[0].X - pFrgnd_Measure_StdD_Dist_Yes_1Stop->x)*(traject[0].X - pFrgnd_Measure_StdD_Dist_Yes_1Stop->x)+(traject[0].Y - pFrgnd_Measure_StdD_Dist_Yes_1Stop->y)*(traject[0].Y - pFrgnd_Measure_StdD_Dist_Yes_1Stop->y)+(traject[0].Z - pFrgnd_Measure_StdD_Dist_Yes_1Stop->z)*(traject[0].Z - pFrgnd_Measure_StdD_Dist_Yes_1Stop->z));
				 if (Distance_Point > PARAM4 && Distance_Point < (PARAM3 - PARAM5))
				 {
				 	 Ptpoint->X = pFrgnd_Measure_StdD_Dist_Yes_1Stop->x;
					 Ptpoint->Y = pFrgnd_Measure_StdD_Dist_Yes_1Stop->y;
					 Ptpoint->Z = pFrgnd_Measure_StdD_Dist_Yes_1Stop->z;
					 traject.push_back(*Ptpoint);
					 /////////////////////////////////////////////////////////////////////////////
					 pFrgnd_Measure_StdD_Dist_Yes_1Stop->m_x.Format("%g", pFrgnd_Measure_StdD_Dist_Yes_1Stop->x);
					 pFrgnd_Measure_StdD_Dist_Yes_1Stop->m_y.Format("%g", pFrgnd_Measure_StdD_Dist_Yes_1Stop->y);
					 pFrgnd_Measure_StdD_Dist_Yes_1Stop->m_z.Format("%g", pFrgnd_Measure_StdD_Dist_Yes_1Stop->z);					     
					 CString Measureb = "x="+pFrgnd_Measure_StdD_Dist_Yes_1Stop->m_x+"      y="+pFrgnd_Measure_StdD_Dist_Yes_1Stop->m_y+"      z="+pFrgnd_Measure_StdD_Dist_Yes_1Stop->m_z;
					 pFrgnd_Measure_StdD_Dist_Yes_1Stop->m_listeofmeasur.InsertString(0,Measureb);
					 pFrgnd_Measure_StdD_Dist_Yes_1Stop->m_listeofmeasur.UpdateWindow();
				 }
                 if (Distance_Point > PARAM3 && traject.size() < 10)
                 {          
                     gpTracker->stopMeasurePoint();
                     pFrgnd_Measure_StdD_Dist_Yes_1Stop->SendERROR("5533","1111");
                     break; 
                 }
			 }	
			 if (Size_Collec_Stop == 0)
			 {
			 	 Ptpoint->X = pFrgnd_Measure_StdD_Dist_Yes_1Stop->x;
				 Ptpoint->Y = pFrgnd_Measure_StdD_Dist_Yes_1Stop->y;
				 Ptpoint->Z = pFrgnd_Measure_StdD_Dist_Yes_1Stop->z;
				 traject.push_back(*Ptpoint);
				 /////////////////////////////////////////////////////////////////////////////
				 pFrgnd_Measure_StdD_Dist_Yes_1Stop->m_x.Format("%g", pFrgnd_Measure_StdD_Dist_Yes_1Stop->x);
				 pFrgnd_Measure_StdD_Dist_Yes_1Stop->m_y.Format("%g", pFrgnd_Measure_StdD_Dist_Yes_1Stop->y);
				 pFrgnd_Measure_StdD_Dist_Yes_1Stop->m_z.Format("%g", pFrgnd_Measure_StdD_Dist_Yes_1Stop->z);			     
				 CString Measureb = "x="+pFrgnd_Measure_StdD_Dist_Yes_1Stop->m_x+"      y="+pFrgnd_Measure_StdD_Dist_Yes_1Stop->m_y+"      z="+pFrgnd_Measure_StdD_Dist_Yes_1Stop->m_z;
				 pFrgnd_Measure_StdD_Dist_Yes_1Stop->m_listeofmeasur.InsertString(0,Measureb);
				 pFrgnd_Measure_StdD_Dist_Yes_1Stop->m_listeofmeasur.UpdateWindow();				 
			 }		
		 }
		 catch(TrackerException* e )
		 {
			 char* msg;
			 msg = e->getText();
			 AfxMessageBox(msg);
			 delete e;
		 }
		 if (imaad2 == -1)
		 {
			 gpTracker->stopMeasurePoint();
			 break;
		 }
	 }
	 ExitThread(0); 
	 return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//DWORD WINAPI MyCritariaFrgndMeasureExterConTrig( LPVOID pParam )
//{	
//	Point* Ptpoint;
//	Ptpoint = new Point();
//	CApplication_Faro_TrakerDlg* pFrgndMeasureCritatiaExterConTrig = (CApplication_Faro_TrakerDlg*)pParam;
//	/////////////////////////////////////////////////////////////////////////////////////
//	MeasurePointData* FrgndMeasurement = NULL;
//	AverageFilter filter;
//	NullStartTrigger startTrigger;
//	ExternalContinueTrigger continueTrigger;
//	/////////////////////////////////////////////////////////////////////////////////////
//	MeasureCfg  cfg( pFrgndMeasureCritatiaExterConTrig->SamPOb , &filter , &startTrigger , &continueTrigger);
//	gpTracker->startMeasurePoint( &cfg );
//
//	/////////////////////////////////////////////////////////////////////////////////////
//
//	 try
//	 {
//		 FrgndMeasurement = gpTracker->readMeasurePointData();		
//		 ASSERT(FrgndMeasurement);
//		 LPARAM lParam = (LPARAM)FrgndMeasurement;
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//		 pFrgndMeasureCritatiaExterConTrig->az     =     (FrgndMeasurement->azimuth());
//		 pFrgndMeasureCritatiaExterConTrig->ze     =     (FrgndMeasurement->zenith());    
//		 pFrgndMeasureCritatiaExterConTrig->di     =     (FrgndMeasurement->distance())*1000;
//		
//
//		 pFrgndMeasureCritatiaExterConTrig->x      =      pFrgndMeasureCritatiaExterConTrig->di*sin(pFrgndMeasureCritatiaExterConTrig->ze)*cos(pFrgndMeasureCritatiaExterConTrig->az);
//		 pFrgndMeasureCritatiaExterConTrig->y      =      pFrgndMeasureCritatiaExterConTrig->di*sin(pFrgndMeasureCritatiaExterConTrig->ze)*sin(pFrgndMeasureCritatiaExterConTrig->az);
//		 pFrgndMeasureCritatiaExterConTrig->z      =      pFrgndMeasureCritatiaExterConTrig->di*cos(pFrgndMeasureCritatiaExterConTrig->ze);
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//		 FrgndMeasurement->time();
//		 FrgndMeasurement->status();
//
//		 Ptpoint->X = pFrgndMeasureCritatiaExterConTrig->x;
//		 Ptpoint->Y = pFrgndMeasureCritatiaExterConTrig->y;
//		 Ptpoint->Z = pFrgndMeasureCritatiaExterConTrig->z;
//	 	 Ptpoint->Cont1_trig = "ExternalContinueTrigger";
//		 Ptpoint->Cont2_trig = "";
//		 Ptpoint->Cont3_trig = "";
//		 Ptpoint->filtter = "Average Filter";
//		 Ptpoint->Measurement_Mechanism = "Foreground Measurement";
//		 Ptpoint->Measurement_Mode = "Dynamic Measurement";
//		 Ptpoint->samperobs = pFrgndMeasureCritatiaExterConTrig->SamPOb;
//		 Ptpoint->SMR = pFrgndMeasureCritatiaExterConTrig->smr;
//		 Ptpoint->Star_tig = "Null Start Trigger";
//		  GetSystemTime(&Strtime);
//		 
//		 Ptpoint->Time.Format( _T("%04d%02d%02d%02d%02d%02d%03d")
//			 ,Strtime.wYear
//			 ,Strtime.wMonth
//			 ,Strtime.wDay
//			 ,Strtime.wHour
//			 ,Strtime.wMinute
//			 ,Strtime.wSecond
//			 ,Strtime.wMilliseconds);
//
//		 traject.push_back(*Ptpoint);
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//		 pFrgndMeasureCritatiaExterConTrig->m_x.Format("%g", pFrgndMeasureCritatiaExterConTrig->x);
//		 pFrgndMeasureCritatiaExterConTrig->m_y.Format("%g", pFrgndMeasureCritatiaExterConTrig->y);
//		 pFrgndMeasureCritatiaExterConTrig->m_z.Format("%g", pFrgndMeasureCritatiaExterConTrig->z);
//	     
//		 CString Measureb = "x="+pFrgndMeasureCritatiaExterConTrig->m_x+"      y="+pFrgndMeasureCritatiaExterConTrig->m_y+"      z="+pFrgndMeasureCritatiaExterConTrig->m_z;
//
//		 pFrgndMeasureCritatiaExterConTrig->m_listeofmeasur.InsertString(0,Measureb);
//		 pFrgndMeasureCritatiaExterConTrig->m_listeofmeasur.UpdateWindow();
//	 }
//	 catch(TrackerException* e )
//	 {
//		 char* msg;
//		 msg = e->getText();
//		 AfxMessageBox(msg);
//		 delete e;
//	 }
//	 if (imaad1 == -1)
//	 {
//		 gpTracker->stopMeasurePoint();
//	 }
//ExitThread(0); 
//return 0;
//}
//DWORD WINAPI MyCritariaFrgndMeasureIntertrig( LPVOID pParam )
//{
//	CApplication_Faro_TrakerDlg* pFrgndMeasureCritatiaIntertrig = (CApplication_Faro_TrakerDlg*)pParam;
//	/////////////////////////////////////////////////////////////////////////////////////
//
//	MeasurePointData* FrgndMeasurement = NULL;
//	AverageFilter filter;
//	NullStartTrigger startTrigger;
//	/////////////////////////////////////////////////////////////////////////////////////
//	IntervalTrigger continueTrigger(pFrgndMeasureCritatiaIntertrig->InterTrig);
//	/////////////////////////////////////////////////////////////////////////////////////
//	MeasureCfg  cfg( pFrgndMeasureCritatiaIntertrig->SamPOb , &filter , &startTrigger , &continueTrigger);
//	gpTracker->startMeasurePoint( &cfg );
//
//	 while (true)
//	 {
//		 Point* Ptpoint;
//	     Ptpoint = new Point();
//		 try
//		 {
//			 FrgndMeasurement = gpTracker->readMeasurePointData();		
//	         ASSERT(FrgndMeasurement);
//	         LPARAM lParam = (LPARAM)FrgndMeasurement;
//
//			 /////////////////////////////////////////////////////////////////////////////
//
//			 pFrgndMeasureCritatiaIntertrig->az     =     (FrgndMeasurement->azimuth());
//			 pFrgndMeasureCritatiaIntertrig->ze     =     (FrgndMeasurement->zenith());    
//			 pFrgndMeasureCritatiaIntertrig->di     =     (FrgndMeasurement->distance())*1000;
//			
//
//			 pFrgndMeasureCritatiaIntertrig->x      =      pFrgndMeasureCritatiaIntertrig->di*sin(pFrgndMeasureCritatiaIntertrig->ze)*cos(pFrgndMeasureCritatiaIntertrig->az);
//			 pFrgndMeasureCritatiaIntertrig->y      =      pFrgndMeasureCritatiaIntertrig->di*sin(pFrgndMeasureCritatiaIntertrig->ze)*sin(pFrgndMeasureCritatiaIntertrig->az);
//			 pFrgndMeasureCritatiaIntertrig->z      =      pFrgndMeasureCritatiaIntertrig->di*cos(pFrgndMeasureCritatiaIntertrig->ze);
//
//			 /////////////////////////////////////////////////////////////////////////////
//
//			 FrgndMeasurement->time();
//			 FrgndMeasurement->status();
//
//			 Ptpoint->X = pFrgndMeasureCritatiaIntertrig->x;
//			 Ptpoint->Y = pFrgndMeasureCritatiaIntertrig->y;
//			 Ptpoint->Z = pFrgndMeasureCritatiaIntertrig->z;
//			 Ptpoint->Cont1_trig = "IntervalTrigger";
//			 Ptpoint->Cont2_trig = pFrgndMeasureCritatiaIntertrig->m_InterTrig;
//			 Ptpoint->Cont3_trig ="";
//			 Ptpoint->filtter = "Average Filter";
//			 Ptpoint->Measurement_Mechanism = "Foreground Measurement";
//			 Ptpoint->Measurement_Mode = "Dynamic Measurement";
//			 Ptpoint->samperobs = pFrgndMeasureCritatiaIntertrig->SamPOb;
//			 Ptpoint->SMR = pFrgndMeasureCritatiaIntertrig->smr;
//			 Ptpoint->Star_tig = "Null Start Trigger";
//			  GetSystemTime(&Strtime);
//		 
//			 Ptpoint->Time.Format( _T("%04d%02d%02d%02d%02d%02d%03d")
//				 ,Strtime.wYear
//				 ,Strtime.wMonth
//				 ,Strtime.wDay
//				 ,Strtime.wHour
//				 ,Strtime.wMinute
//				 ,Strtime.wSecond
//				 ,Strtime.wMilliseconds);
//
//			 traject.push_back(*Ptpoint);
//
//			 /////////////////////////////////////////////////////////////////////////////
//
//			 pFrgndMeasureCritatiaIntertrig->m_x.Format("%g", pFrgndMeasureCritatiaIntertrig->x);
//			 pFrgndMeasureCritatiaIntertrig->m_y.Format("%g", pFrgndMeasureCritatiaIntertrig->y);
//			 pFrgndMeasureCritatiaIntertrig->m_z.Format("%g", pFrgndMeasureCritatiaIntertrig->z);
//		     
//			 CString Measureb = "x="+pFrgndMeasureCritatiaIntertrig->m_x+"      y="+pFrgndMeasureCritatiaIntertrig->m_y+"      z="+pFrgndMeasureCritatiaIntertrig->m_z;
//
//			 pFrgndMeasureCritatiaIntertrig->m_listeofmeasur.InsertString(0,Measureb);
//			 pFrgndMeasureCritatiaIntertrig->m_listeofmeasur.UpdateWindow();
//
//		 }
//		 catch(TrackerException* e )
//		 {
//			 char* msg;
//			 msg = e->getText();
//			 AfxMessageBox(msg);
//			 delete e;
//		 }
//		 if (imaad2 == -1)
//		 {
//			 gpTracker->stopMeasurePoint();
//			 break;
//		 }
//	 }
//	 ExitThread(0); 
//	 return 0;
//}
//
//DWORD WINAPI MyCritariaFrgndMeasureDistTrig( LPVOID pParam )
//{	
//	CApplication_Faro_TrakerDlg* pFrgndMeasureCritatiaDistTrig = (CApplication_Faro_TrakerDlg*)pParam;
//	/////////////////////////////////////////////////////////////////////////////////////
//
//	MeasurePointData* FrgndMeasurement = NULL;
//	AverageFilter filter;
//	NullStartTrigger startTrigger;
//	/////////////////////////////////////////////////////////////////////////////////////
//	NullContinueTrigger continueTrigger/*(pFrgndMeasureCritatiaDistTrig->DitTrig)*/;
//	/////////////////////////////////////////////////////////////////////////////////////
//	MeasureCfg  cfg( pFrgndMeasureCritatiaDistTrig->SamPOb , &filter , &startTrigger , &continueTrigger);
//	gpTracker->startMeasurePoint( &cfg );
//
//	 while (true)
//	 {
//		 Point* Ptpoint;
//	     Ptpoint = new Point();
//		 try
//		 {
//			 FrgndMeasurement = gpTracker->readMeasurePointData();		
//	         ASSERT(FrgndMeasurement);
//	         LPARAM lParam = (LPARAM)FrgndMeasurement;
//
//			 /////////////////////////////////////////////////////////////////////////////
//
//			 pFrgndMeasureCritatiaDistTrig->az     =     (FrgndMeasurement->azimuth());
//			 pFrgndMeasureCritatiaDistTrig->ze     =     (FrgndMeasurement->zenith());    
//			 pFrgndMeasureCritatiaDistTrig->di     =     (FrgndMeasurement->distance())*1000;
//			
//
//			 pFrgndMeasureCritatiaDistTrig->x      =      pFrgndMeasureCritatiaDistTrig->di*sin(pFrgndMeasureCritatiaDistTrig->ze)*cos(pFrgndMeasureCritatiaDistTrig->az);
//			 pFrgndMeasureCritatiaDistTrig->y      =      pFrgndMeasureCritatiaDistTrig->di*sin(pFrgndMeasureCritatiaDistTrig->ze)*sin(pFrgndMeasureCritatiaDistTrig->az);
//			 pFrgndMeasureCritatiaDistTrig->z      =      pFrgndMeasureCritatiaDistTrig->di*cos(pFrgndMeasureCritatiaDistTrig->ze);
//
//			 /////////////////////////////////////////////////////////////////////////////
//
//			 FrgndMeasurement->time();
//			 FrgndMeasurement->status();
//
//			 Ptpoint->X = pFrgndMeasureCritatiaDistTrig->x;
//			 Ptpoint->Y = pFrgndMeasureCritatiaDistTrig->y;
//			 Ptpoint->Z = pFrgndMeasureCritatiaDistTrig->z;
//			 Ptpoint->Cont1_trig = "DistanceTrigger";
//			 Ptpoint->Cont2_trig ="";
//			 Ptpoint->Cont3_trig = pFrgndMeasureCritatiaDistTrig->m_DitTrig;
//			 Ptpoint->filtter = "Average Filter";
//			 Ptpoint->Measurement_Mechanism = "Foreground Measurement";
//			 Ptpoint->Measurement_Mode = "Dynamic Measurement";
//			 Ptpoint->samperobs = pFrgndMeasureCritatiaDistTrig->SamPOb;
//			 Ptpoint->SMR = pFrgndMeasureCritatiaDistTrig->smr;
//			 Ptpoint->Star_tig = "Null Start Trigger";
//			  GetSystemTime(&Strtime);
//			 
//			 Ptpoint->Time.Format( _T("%04d%02d%02d%02d%02d%02d%03d")
//				 ,Strtime.wYear
//				 ,Strtime.wMonth
//				 ,Strtime.wDay
//				 ,Strtime.wHour
//				 ,Strtime.wMinute
//				 ,Strtime.wSecond
//				 ,Strtime.wMilliseconds);
//
//			 traject.push_back(*Ptpoint);
//
//			 /////////////////////////////////////////////////////////////////////////////
//
//			 pFrgndMeasureCritatiaDistTrig->m_x.Format("%g", pFrgndMeasureCritatiaDistTrig->x);
//			 pFrgndMeasureCritatiaDistTrig->m_y.Format("%g", pFrgndMeasureCritatiaDistTrig->y);
//			 pFrgndMeasureCritatiaDistTrig->m_z.Format("%g", pFrgndMeasureCritatiaDistTrig->z);
//		     
//			 CString Measureb = "x="+pFrgndMeasureCritatiaDistTrig->m_x+"      y="+pFrgndMeasureCritatiaDistTrig->m_y+"      z="+pFrgndMeasureCritatiaDistTrig->m_z;
//
//			 pFrgndMeasureCritatiaDistTrig->m_listeofmeasur.InsertString(0,Measureb);
//			 pFrgndMeasureCritatiaDistTrig->m_listeofmeasur.UpdateWindow();
//
//		 }
//		 catch(TrackerException* e )
//		 {
//			 char* msg;
//			 msg = e->getText();
//			 AfxMessageBox(msg);
//			 delete e;
//		 }
//		  if (imaad3 == -1)
//		 {
//			 gpTracker->stopMeasurePoint();
//			 break;
//		 }
//	 }
//ExitThread(0); 
//return 0;
//}
//DWORD WINAPI MyCritariaFrgndMeasureExterConTrig1( LPVOID pParam )
//{	
//	Point* Ptpoint;
//	Ptpoint = new Point();
//	CApplication_Faro_TrakerDlg* pFrgndMeasureCritatiaExterConTrig1 = (CApplication_Faro_TrakerDlg*)pParam;
//	/////////////////////////////////////////////////////////////////////////////////////
//	MeasurePointData* FrgndMeasurement = NULL;
//	StdDevFilter filter;
//	NullStartTrigger startTrigger;
//	ExternalContinueTrigger continueTrigger;
//	/////////////////////////////////////////////////////////////////////////////////////
//	MeasureCfg  cfg( pFrgndMeasureCritatiaExterConTrig1->SamPOb , &filter , &startTrigger , &continueTrigger);
//	gpTracker->startMeasurePoint( &cfg );
//
//	/////////////////////////////////////////////////////////////////////////////////////
//
//	 try
//	 {
//		 FrgndMeasurement = gpTracker->readMeasurePointData();		
//		 ASSERT(FrgndMeasurement);
//		 LPARAM lParam = (LPARAM)FrgndMeasurement;
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//		 pFrgndMeasureCritatiaExterConTrig1->az     =     (FrgndMeasurement->azimuth());
//		 pFrgndMeasureCritatiaExterConTrig1->ze     =     (FrgndMeasurement->zenith());    
//		 pFrgndMeasureCritatiaExterConTrig1->di     =     (FrgndMeasurement->distance())*1000;
//		
//
//		 pFrgndMeasureCritatiaExterConTrig1->x      =      pFrgndMeasureCritatiaExterConTrig1->di*sin(pFrgndMeasureCritatiaExterConTrig1->ze)*cos(pFrgndMeasureCritatiaExterConTrig1->az);
//		 pFrgndMeasureCritatiaExterConTrig1->y      =      pFrgndMeasureCritatiaExterConTrig1->di*sin(pFrgndMeasureCritatiaExterConTrig1->ze)*sin(pFrgndMeasureCritatiaExterConTrig1->az);
//		 pFrgndMeasureCritatiaExterConTrig1->z      =      pFrgndMeasureCritatiaExterConTrig1->di*cos(pFrgndMeasureCritatiaExterConTrig1->ze);
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//		 FrgndMeasurement->time();
//		 FrgndMeasurement->status();
//
//		 Ptpoint->X = pFrgndMeasureCritatiaExterConTrig1->x;
//		 Ptpoint->Y = pFrgndMeasureCritatiaExterConTrig1->y;
//		 Ptpoint->Z = pFrgndMeasureCritatiaExterConTrig1->z;
//		 Ptpoint->Cont1_trig = "ExternalContinueTrigger";
//		 Ptpoint->Cont2_trig ="";
//		 Ptpoint->Cont3_trig ="";
//		 Ptpoint->filtter = "StdDevFilter";
//		 Ptpoint->Measurement_Mechanism = "Foreground Measurement";
//		 Ptpoint->Measurement_Mode = "Dynamic Measurement";
//		 Ptpoint->samperobs = pFrgndMeasureCritatiaExterConTrig1->SamPOb;
//		 Ptpoint->SMR = pFrgndMeasureCritatiaExterConTrig1->smr;
//		 Ptpoint->Star_tig = "Null Start Trigger";
//		  GetSystemTime(&Strtime);
//		 
//		 Ptpoint->Time.Format( _T("%04d%02d%02d%02d%02d%02d%03d")
//			 ,Strtime.wYear
//			 ,Strtime.wMonth
//			 ,Strtime.wDay
//			 ,Strtime.wHour
//			 ,Strtime.wMinute
//			 ,Strtime.wSecond
//			 ,Strtime.wMilliseconds);
//
//		 traject.push_back(*Ptpoint);
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//		 pFrgndMeasureCritatiaExterConTrig1->m_x.Format("%g", pFrgndMeasureCritatiaExterConTrig1->x);
//		 pFrgndMeasureCritatiaExterConTrig1->m_y.Format("%g", pFrgndMeasureCritatiaExterConTrig1->y);
//		 pFrgndMeasureCritatiaExterConTrig1->m_z.Format("%g", pFrgndMeasureCritatiaExterConTrig1->z);
//	     
//		 CString Measureb = "x="+pFrgndMeasureCritatiaExterConTrig1->m_x+"      y="+pFrgndMeasureCritatiaExterConTrig1->m_y+"      z="+pFrgndMeasureCritatiaExterConTrig1->m_z;
//
//		 pFrgndMeasureCritatiaExterConTrig1->m_listeofmeasur.InsertString(0,Measureb);
//		 pFrgndMeasureCritatiaExterConTrig1->m_listeofmeasur.UpdateWindow();
//	 }
//	 catch(TrackerException* e )
//	 {
//		 char* msg;
//		 msg = e->getText();
//		 AfxMessageBox(msg);
//		 delete e;
//	 }
//	 if (imaad4 == -1)
//	 {
//		 gpTracker->stopMeasurePoint();
//	 }
//ExitThread(0); 
//return 0;
//}
//DWORD WINAPI MyCritariaFrgndMeasureIntertrig1( LPVOID pParam )
//{	
//	CApplication_Faro_TrakerDlg* pFrgndMeasureCritatiaIntertrig1 = (CApplication_Faro_TrakerDlg*)pParam;
//	/////////////////////////////////////////////////////////////////////////////////////
//
//	MeasurePointData* FrgndMeasurement = NULL;
//	StdDevFilter filter;
//	NullStartTrigger startTrigger;
//	/////////////////////////////////////////////////////////////////////////////////////
//	IntervalTrigger continueTrigger(pFrgndMeasureCritatiaIntertrig1->InterTrig);
//	/////////////////////////////////////////////////////////////////////////////////////
//	MeasureCfg  cfg( pFrgndMeasureCritatiaIntertrig1->SamPOb , &filter , &startTrigger , &continueTrigger);
//	gpTracker->startMeasurePoint( &cfg );
//
//	 while (true)
//	 {
//		 Point* Ptpoint;
//	     Ptpoint = new Point();
//		 try
//		 {
//			 FrgndMeasurement = gpTracker->readMeasurePointData();		
//	         ASSERT(FrgndMeasurement);
//	         LPARAM lParam = (LPARAM)FrgndMeasurement;
//
//			 /////////////////////////////////////////////////////////////////////////////
//
//			 pFrgndMeasureCritatiaIntertrig1->az     =     (FrgndMeasurement->azimuth());
//			 pFrgndMeasureCritatiaIntertrig1->ze     =     (FrgndMeasurement->zenith());    
//			 pFrgndMeasureCritatiaIntertrig1->di     =     (FrgndMeasurement->distance())*1000;
//			
//
//			 pFrgndMeasureCritatiaIntertrig1->x      =      pFrgndMeasureCritatiaIntertrig1->di*sin(pFrgndMeasureCritatiaIntertrig1->ze)*cos(pFrgndMeasureCritatiaIntertrig1->az);
//			 pFrgndMeasureCritatiaIntertrig1->y      =      pFrgndMeasureCritatiaIntertrig1->di*sin(pFrgndMeasureCritatiaIntertrig1->ze)*sin(pFrgndMeasureCritatiaIntertrig1->az);
//			 pFrgndMeasureCritatiaIntertrig1->z      =      pFrgndMeasureCritatiaIntertrig1->di*cos(pFrgndMeasureCritatiaIntertrig1->ze);
//
//			 /////////////////////////////////////////////////////////////////////////////
//
//			 FrgndMeasurement->time();
//			 FrgndMeasurement->status();
//
//			 Ptpoint->X = pFrgndMeasureCritatiaIntertrig1->x;
//			 Ptpoint->Y = pFrgndMeasureCritatiaIntertrig1->y;
//			 Ptpoint->Z = pFrgndMeasureCritatiaIntertrig1->z;
//			 Ptpoint->Cont1_trig = "IntervalTrigger";
//			 Ptpoint->Cont2_trig = pFrgndMeasureCritatiaIntertrig1->m_InterTrig;
//			 Ptpoint->Cont3_trig ="";
//			 Ptpoint->filtter = "StdDevFilter";
//			 Ptpoint->Measurement_Mechanism = "Foreground Measurement";
//			 Ptpoint->Measurement_Mode = "Dynamic Measurement";
//			 Ptpoint->samperobs = pFrgndMeasureCritatiaIntertrig1->SamPOb;
//			 Ptpoint->SMR = pFrgndMeasureCritatiaIntertrig1->smr;
//			 Ptpoint->Star_tig = "Null Start Trigger";
//			  GetSystemTime(&Strtime);
//		 
//			 Ptpoint->Time.Format( _T("%04d%02d%02d%02d%02d%02d%03d")
//				 ,Strtime.wYear
//				 ,Strtime.wMonth
//				 ,Strtime.wDay
//				 ,Strtime.wHour
//				 ,Strtime.wMinute
//				 ,Strtime.wSecond
//				 ,Strtime.wMilliseconds);
//
//			 traject.push_back(*Ptpoint);
//
//			 /////////////////////////////////////////////////////////////////////////////
//
//			 pFrgndMeasureCritatiaIntertrig1->m_x.Format("%g", pFrgndMeasureCritatiaIntertrig1->x);
//			 pFrgndMeasureCritatiaIntertrig1->m_y.Format("%g", pFrgndMeasureCritatiaIntertrig1->y);
//			 pFrgndMeasureCritatiaIntertrig1->m_z.Format("%g", pFrgndMeasureCritatiaIntertrig1->z);
//		     
//			 CString Measureb = "x="+pFrgndMeasureCritatiaIntertrig1->m_x+"      y="+pFrgndMeasureCritatiaIntertrig1->m_y+"      z="+pFrgndMeasureCritatiaIntertrig1->m_z;
//
//			 pFrgndMeasureCritatiaIntertrig1->m_listeofmeasur.InsertString(0,Measureb);
//			 pFrgndMeasureCritatiaIntertrig1->m_listeofmeasur.UpdateWindow();
//
//		 }
//		 catch(TrackerException* e )
//		 {
//			 char* msg;
//			 msg = e->getText();
//			 AfxMessageBox(msg);
//			 delete e;
//		 }
//		 if (imaad5 == -1)
//		 {
//			 gpTracker->stopMeasurePoint();
//			 break;
//		 }
//	 }
//	 ExitThread(0); 
//	 return 0;
//}
//DWORD WINAPI MyCritariaFrgndMeasureDistTrig1( LPVOID pParam )
//{	
//	CApplication_Faro_TrakerDlg* pFrgndMeasureCritatiaDistTrig1 = (CApplication_Faro_TrakerDlg*)pParam;
//	/////////////////////////////////////////////////////////////////////////////////////
//
//	MeasurePointData* FrgndMeasurement = NULL;
//	StdDevFilter filter;
//	NullStartTrigger startTrigger;
//	/////////////////////////////////////////////////////////////////////////////////////
//	DistanceTrigger continueTrigger(pFrgndMeasureCritatiaDistTrig1->DitTrig);
//	/////////////////////////////////////////////////////////////////////////////////////
//	MeasureCfg  cfg( pFrgndMeasureCritatiaDistTrig1->SamPOb , &filter , &startTrigger , &continueTrigger);
//	gpTracker->startMeasurePoint( &cfg );
//
//	 while (true)
//	 {
//		 Point* Ptpoint;
//	     Ptpoint = new Point();
//		 try
//		 {
//			 FrgndMeasurement = gpTracker->readMeasurePointData();		
//	         ASSERT(FrgndMeasurement);
//	         LPARAM lParam = (LPARAM)FrgndMeasurement;
//
//			 /////////////////////////////////////////////////////////////////////////////
//
//			 pFrgndMeasureCritatiaDistTrig1->az     =     (FrgndMeasurement->azimuth());
//			 pFrgndMeasureCritatiaDistTrig1->ze     =     (FrgndMeasurement->zenith());    
//			 pFrgndMeasureCritatiaDistTrig1->di     =     (FrgndMeasurement->distance())*1000;
//			
//
//			 pFrgndMeasureCritatiaDistTrig1->x      =      pFrgndMeasureCritatiaDistTrig1->di*sin(pFrgndMeasureCritatiaDistTrig1->ze)*cos(pFrgndMeasureCritatiaDistTrig1->az);
//			 pFrgndMeasureCritatiaDistTrig1->y      =      pFrgndMeasureCritatiaDistTrig1->di*sin(pFrgndMeasureCritatiaDistTrig1->ze)*sin(pFrgndMeasureCritatiaDistTrig1->az);
//			 pFrgndMeasureCritatiaDistTrig1->z      =      pFrgndMeasureCritatiaDistTrig1->di*cos(pFrgndMeasureCritatiaDistTrig1->ze);
//
//			 /////////////////////////////////////////////////////////////////////////////
//
//			 FrgndMeasurement->time();
//			 FrgndMeasurement->status();
//
//			 Ptpoint->X = pFrgndMeasureCritatiaDistTrig1->x;
//			 Ptpoint->Y = pFrgndMeasureCritatiaDistTrig1->y;
//			 Ptpoint->Z = pFrgndMeasureCritatiaDistTrig1->z;
//			 Ptpoint->Cont1_trig = "DistanceTrigger";
//			 Ptpoint->Cont2_trig ="";
//			 Ptpoint->Cont3_trig = pFrgndMeasureCritatiaDistTrig1->m_DitTrig;
//			 Ptpoint->filtter = "StdDevFilter";
//			 Ptpoint->Measurement_Mechanism = "Foreground Measurement";
//			 Ptpoint->Measurement_Mode = "Dynamic Measurement";
//			 Ptpoint->samperobs = pFrgndMeasureCritatiaDistTrig1->SamPOb;
//			 Ptpoint->SMR = pFrgndMeasureCritatiaDistTrig1->smr;
//			 Ptpoint->Star_tig = "Null Start Trigger";
//			  GetSystemTime(&Strtime);
//		 
//			 Ptpoint->Time.Format( _T("%04d%02d%02d%02d%02d%02d%03d")
//				 ,Strtime.wYear
//				 ,Strtime.wMonth
//				 ,Strtime.wDay
//				 ,Strtime.wHour
//				 ,Strtime.wMinute
//				 ,Strtime.wSecond
//				 ,Strtime.wMilliseconds);
//
//			 traject.push_back(*Ptpoint);
//
//			 /////////////////////////////////////////////////////////////////////////////
//
//			 pFrgndMeasureCritatiaDistTrig1->m_x.Format("%g", pFrgndMeasureCritatiaDistTrig1->x);
//			 pFrgndMeasureCritatiaDistTrig1->m_y.Format("%g", pFrgndMeasureCritatiaDistTrig1->y);
//			 pFrgndMeasureCritatiaDistTrig1->m_z.Format("%g", pFrgndMeasureCritatiaDistTrig1->z);
//		     
//			 CString Measureb = "x="+pFrgndMeasureCritatiaDistTrig1->m_x+"      y="+pFrgndMeasureCritatiaDistTrig1->m_y+"      z="+pFrgndMeasureCritatiaDistTrig1->m_z;
//
//			 pFrgndMeasureCritatiaDistTrig1->m_listeofmeasur.InsertString(0,Measureb);
//			 pFrgndMeasureCritatiaDistTrig1->m_listeofmeasur.UpdateWindow();
//
//		 }
//		 catch(TrackerException* e )
//		 {
//			 char* msg;
//			 msg = e->getText();
//			 AfxMessageBox(msg);
//			 delete e;
//		 }
//		  if (imaad6 == -1)
//		 {
//			 gpTracker->stopMeasurePoint();
//			 break;
//		 }
//	 }
//ExitThread(0); 
//return 0;
//}
//DWORD WINAPI MyCritariaFrgndMeasureExterConTrig2( LPVOID pParam )
//{	
//	Point* Ptpoint;
//	Ptpoint = new Point();
//	CApplication_Faro_TrakerDlg* pFrgndMeasureCritatiaExterConTrig2 = (CApplication_Faro_TrakerDlg*)pParam;
//	/////////////////////////////////////////////////////////////////////////////////////
//	MeasurePointData* FrgndMeasurement = NULL;
//	AverageFilter filter;
//	ExternalStartTrigger startTrigger;
//	ExternalContinueTrigger continueTrigger;
//	/////////////////////////////////////////////////////////////////////////////////////
//	MeasureCfg  cfg( pFrgndMeasureCritatiaExterConTrig2->SamPOb , &filter , &startTrigger , &continueTrigger);
//	gpTracker->startMeasurePoint( &cfg );
//
//	/////////////////////////////////////////////////////////////////////////////////////
//
//	 try
//	 {
//		 FrgndMeasurement = gpTracker->readMeasurePointData();		
//		 ASSERT(FrgndMeasurement);
//		 LPARAM lParam = (LPARAM)FrgndMeasurement;
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//		 pFrgndMeasureCritatiaExterConTrig2->az     =     (FrgndMeasurement->azimuth());
//		 pFrgndMeasureCritatiaExterConTrig2->ze     =     (FrgndMeasurement->zenith());    
//		 pFrgndMeasureCritatiaExterConTrig2->di     =     (FrgndMeasurement->distance())*1000;
//		
//
//		 pFrgndMeasureCritatiaExterConTrig2->x      =      pFrgndMeasureCritatiaExterConTrig2->di*sin(pFrgndMeasureCritatiaExterConTrig2->ze)*cos(pFrgndMeasureCritatiaExterConTrig2->az);
//		 pFrgndMeasureCritatiaExterConTrig2->y      =      pFrgndMeasureCritatiaExterConTrig2->di*sin(pFrgndMeasureCritatiaExterConTrig2->ze)*sin(pFrgndMeasureCritatiaExterConTrig2->az);
//		 pFrgndMeasureCritatiaExterConTrig2->z      =      pFrgndMeasureCritatiaExterConTrig2->di*cos(pFrgndMeasureCritatiaExterConTrig2->ze);
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//		 FrgndMeasurement->time();
//		 FrgndMeasurement->status();
//
//		 Ptpoint->X = pFrgndMeasureCritatiaExterConTrig2->x;
//		 Ptpoint->Y = pFrgndMeasureCritatiaExterConTrig2->y;
//		 Ptpoint->Z = pFrgndMeasureCritatiaExterConTrig2->z;
//		 Ptpoint->Cont1_trig = "External Continue Trigger";
//		 Ptpoint->Cont2_trig ="";
//		 Ptpoint->Cont3_trig ="";
//		 Ptpoint->filtter = "Average Filter";
//		 Ptpoint->Measurement_Mechanism = "Foreground Measurement";
//		 Ptpoint->Measurement_Mode = "Dynamic Measurement";
//		 Ptpoint->samperobs = pFrgndMeasureCritatiaExterConTrig2->SamPOb;
//		 Ptpoint->SMR = pFrgndMeasureCritatiaExterConTrig2->smr;
//		 Ptpoint->Star_tig = "External Start Trigger";
//		  GetSystemTime(&Strtime);
//		 
//		 Ptpoint->Time.Format( _T("%04d%02d%02d%02d%02d%02d%03d")
//			 ,Strtime.wYear
//			 ,Strtime.wMonth
//			 ,Strtime.wDay
//			 ,Strtime.wHour
//			 ,Strtime.wMinute
//			 ,Strtime.wSecond
//			 ,Strtime.wMilliseconds);
//
//		 traject.push_back(*Ptpoint);
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//		 pFrgndMeasureCritatiaExterConTrig2->m_x.Format("%g", pFrgndMeasureCritatiaExterConTrig2->x);
//		 pFrgndMeasureCritatiaExterConTrig2->m_y.Format("%g", pFrgndMeasureCritatiaExterConTrig2->y);
//		 pFrgndMeasureCritatiaExterConTrig2->m_z.Format("%g", pFrgndMeasureCritatiaExterConTrig2->z);
//	     
//		 CString Measureb = "x="+pFrgndMeasureCritatiaExterConTrig2->m_x+"      y="+pFrgndMeasureCritatiaExterConTrig2->m_y+"      z="+pFrgndMeasureCritatiaExterConTrig2->m_z;
//
//		 pFrgndMeasureCritatiaExterConTrig2->m_listeofmeasur.InsertString(0,Measureb);
//		 pFrgndMeasureCritatiaExterConTrig2->m_listeofmeasur.UpdateWindow();
//	 }
//	 catch(TrackerException* e )
//	 {
//		 char* msg;
//		 msg = e->getText();
//		 AfxMessageBox(msg);
//		 delete e;
//	 }
//	 if (imaad12 == -1)
//	 {
//		 gpTracker->stopMeasurePoint();
//	 }
//ExitThread(0); 
//return 0;
//}
//DWORD WINAPI MyCritariaFrgndMeasureIntertrig2( LPVOID pParam )
//{
//	CApplication_Faro_TrakerDlg* pFrgndMeasureCritatiaIntertrig2 = (CApplication_Faro_TrakerDlg*)pParam;
//	/////////////////////////////////////////////////////////////////////////////////////
//
//	MeasurePointData* FrgndMeasurement = NULL;
//	AverageFilter filter;
//	ExternalStartTrigger startTrigger;
//	/////////////////////////////////////////////////////////////////////////////////////
//	IntervalTrigger continueTrigger(pFrgndMeasureCritatiaIntertrig2->InterTrig);
//	/////////////////////////////////////////////////////////////////////////////////////
//	MeasureCfg  cfg( pFrgndMeasureCritatiaIntertrig2->SamPOb , &filter , &startTrigger , &continueTrigger);
//	gpTracker->startMeasurePoint( &cfg );
//
//	 while (true)
//	 {
//		 Point* Ptpoint;
//	     Ptpoint = new Point();
//		 try
//		 {
//			 FrgndMeasurement = gpTracker->readMeasurePointData();		
//	         ASSERT(FrgndMeasurement);
//	         LPARAM lParam = (LPARAM)FrgndMeasurement;
//
//			 /////////////////////////////////////////////////////////////////////////////
//
//			 pFrgndMeasureCritatiaIntertrig2->az     =     (FrgndMeasurement->azimuth());
//			 pFrgndMeasureCritatiaIntertrig2->ze     =     (FrgndMeasurement->zenith());    
//			 pFrgndMeasureCritatiaIntertrig2->di     =     (FrgndMeasurement->distance())*1000;
//			
//
//			 pFrgndMeasureCritatiaIntertrig2->x      =      pFrgndMeasureCritatiaIntertrig2->di*sin(pFrgndMeasureCritatiaIntertrig2->ze)*cos(pFrgndMeasureCritatiaIntertrig2->az);
//			 pFrgndMeasureCritatiaIntertrig2->y      =      pFrgndMeasureCritatiaIntertrig2->di*sin(pFrgndMeasureCritatiaIntertrig2->ze)*sin(pFrgndMeasureCritatiaIntertrig2->az);
//			 pFrgndMeasureCritatiaIntertrig2->z      =      pFrgndMeasureCritatiaIntertrig2->di*cos(pFrgndMeasureCritatiaIntertrig2->ze);
//
//			 /////////////////////////////////////////////////////////////////////////////
//
//			 FrgndMeasurement->time();
//			 FrgndMeasurement->status();
//
//			 Ptpoint->X = pFrgndMeasureCritatiaIntertrig2->x;
//			 Ptpoint->Y = pFrgndMeasureCritatiaIntertrig2->y;
//			 Ptpoint->Z = pFrgndMeasureCritatiaIntertrig2->z;
//			 Ptpoint->Cont1_trig = "IntervalTrigger";
//			 Ptpoint->Cont2_trig = pFrgndMeasureCritatiaIntertrig2->m_InterTrig;
//			 Ptpoint->Cont3_trig ="";
//			 Ptpoint->filtter = "Average Filter";
//			 Ptpoint->Measurement_Mechanism = "Foreground Measurement";
//			 Ptpoint->Measurement_Mode = "Dynamic Measurement";
//			 Ptpoint->samperobs = pFrgndMeasureCritatiaIntertrig2->SamPOb;
//			 Ptpoint->SMR = pFrgndMeasureCritatiaIntertrig2->smr;
//			 Ptpoint->Star_tig = "External Start Trigger";
//			  GetSystemTime(&Strtime);
//			 
//			 Ptpoint->Time.Format( _T("%04d%02d%02d%02d%02d%02d%03d")
//				 ,Strtime.wYear
//				 ,Strtime.wMonth
//				 ,Strtime.wDay
//				 ,Strtime.wHour
//				 ,Strtime.wMinute
//				 ,Strtime.wSecond
//				 ,Strtime.wMilliseconds);
//
//			 traject.push_back(*Ptpoint);
//
//			 /////////////////////////////////////////////////////////////////////////////
//
//			 pFrgndMeasureCritatiaIntertrig2->m_x.Format("%g", pFrgndMeasureCritatiaIntertrig2->x);
//			 pFrgndMeasureCritatiaIntertrig2->m_y.Format("%g", pFrgndMeasureCritatiaIntertrig2->y);
//			 pFrgndMeasureCritatiaIntertrig2->m_z.Format("%g", pFrgndMeasureCritatiaIntertrig2->z);
//		     
//			 CString Measureb = "x="+pFrgndMeasureCritatiaIntertrig2->m_x+"      y="+pFrgndMeasureCritatiaIntertrig2->m_y+"      z="+pFrgndMeasureCritatiaIntertrig2->m_z;
//
//			 pFrgndMeasureCritatiaIntertrig2->m_listeofmeasur.InsertString(0,Measureb);
//			 pFrgndMeasureCritatiaIntertrig2->m_listeofmeasur.UpdateWindow();
//
//		 }
//		 catch(TrackerException* e )
//		 {
//			 char* msg;
//			 msg = e->getText();
//			 AfxMessageBox(msg);
//			 delete e;
//		 }
//		 if (imaad22 == -1)
//		 {
//			 gpTracker->stopMeasurePoint();
//			 break;
//		 }
//	 }
//	 ExitThread(0); 
//	 return 0;
//}
//
//DWORD WINAPI MyCritariaFrgndMeasureDistTrig2( LPVOID pParam )
//{	
//	CApplication_Faro_TrakerDlg* pFrgndMeasureCritatiaDistTrig2 = (CApplication_Faro_TrakerDlg*)pParam;
//	/////////////////////////////////////////////////////////////////////////////////////
//
//	MeasurePointData* FrgndMeasurement = NULL;
//	AverageFilter filter;
//	ExternalStartTrigger startTrigger;
//	/////////////////////////////////////////////////////////////////////////////////////
//	DistanceTrigger continueTrigger(pFrgndMeasureCritatiaDistTrig2->DitTrig);
//	/////////////////////////////////////////////////////////////////////////////////////
//	MeasureCfg  cfg( pFrgndMeasureCritatiaDistTrig2->SamPOb , &filter , &startTrigger , &continueTrigger);
//	gpTracker->startMeasurePoint( &cfg );
//
//	 while (true)
//	 {
//		 Point* Ptpoint;
//	     Ptpoint = new Point();
//		 try
//		 {
//			 FrgndMeasurement = gpTracker->readMeasurePointData();		
//	         ASSERT(FrgndMeasurement);
//	         LPARAM lParam = (LPARAM)FrgndMeasurement;
//
//			 /////////////////////////////////////////////////////////////////////////////
//
//			 pFrgndMeasureCritatiaDistTrig2->az     =     (FrgndMeasurement->azimuth());
//			 pFrgndMeasureCritatiaDistTrig2->ze     =     (FrgndMeasurement->zenith());    
//			 pFrgndMeasureCritatiaDistTrig2->di     =     (FrgndMeasurement->distance())*1000;
//			
//
//			 pFrgndMeasureCritatiaDistTrig2->x      =      pFrgndMeasureCritatiaDistTrig2->di*sin(pFrgndMeasureCritatiaDistTrig2->ze)*cos(pFrgndMeasureCritatiaDistTrig2->az);
//			 pFrgndMeasureCritatiaDistTrig2->y      =      pFrgndMeasureCritatiaDistTrig2->di*sin(pFrgndMeasureCritatiaDistTrig2->ze)*sin(pFrgndMeasureCritatiaDistTrig2->az);
//			 pFrgndMeasureCritatiaDistTrig2->z      =      pFrgndMeasureCritatiaDistTrig2->di*cos(pFrgndMeasureCritatiaDistTrig2->ze);
//
//			 /////////////////////////////////////////////////////////////////////////////
//
//			 FrgndMeasurement->time();
//			 FrgndMeasurement->status();
//
//			 Ptpoint->X = pFrgndMeasureCritatiaDistTrig2->x;
//			 Ptpoint->Y = pFrgndMeasureCritatiaDistTrig2->y;
//			 Ptpoint->Z = pFrgndMeasureCritatiaDistTrig2->z;
//			 Ptpoint->Cont1_trig = "DistanceTrigger";
//			 Ptpoint->Cont2_trig ="";
//			 Ptpoint->Cont3_trig = pFrgndMeasureCritatiaDistTrig2->m_DitTrig;
//			 Ptpoint->filtter = "Average Filter";
//			 Ptpoint->Measurement_Mechanism = "Foreground Measurement";
//			 Ptpoint->Measurement_Mode = "Dynamic Measurement";
//			 Ptpoint->samperobs = pFrgndMeasureCritatiaDistTrig2->SamPOb;
//			 Ptpoint->SMR = pFrgndMeasureCritatiaDistTrig2->smr;
//			 Ptpoint->Star_tig = "External Start Trigger";
//			  GetSystemTime(&Strtime);
//		 
//			 Ptpoint->Time.Format( _T("%04d%02d%02d%02d%02d%02d%03d")
//				 ,Strtime.wYear
//				 ,Strtime.wMonth
//				 ,Strtime.wDay
//				 ,Strtime.wHour
//				 ,Strtime.wMinute
//				 ,Strtime.wSecond
//				 ,Strtime.wMilliseconds);
//
//			 traject.push_back(*Ptpoint);
//
//			 /////////////////////////////////////////////////////////////////////////////
//
//			 pFrgndMeasureCritatiaDistTrig2->m_x.Format("%g", pFrgndMeasureCritatiaDistTrig2->x);
//			 pFrgndMeasureCritatiaDistTrig2->m_y.Format("%g", pFrgndMeasureCritatiaDistTrig2->y);
//			 pFrgndMeasureCritatiaDistTrig2->m_z.Format("%g", pFrgndMeasureCritatiaDistTrig2->z);
//		     
//			 CString Measureb = "x="+pFrgndMeasureCritatiaDistTrig2->m_x+"      y="+pFrgndMeasureCritatiaDistTrig2->m_y+"      z="+pFrgndMeasureCritatiaDistTrig2->m_z;
//
//			 pFrgndMeasureCritatiaDistTrig2->m_listeofmeasur.InsertString(0,Measureb);
//			 pFrgndMeasureCritatiaDistTrig2->m_listeofmeasur.UpdateWindow();
//
//		 }
//		 catch(TrackerException* e )
//		 {
//			 char* msg;
//			 msg = e->getText();
//			 AfxMessageBox(msg);
//			 delete e;
//		 }
//		  if (imaad32 == -1)
//		 {
//			 gpTracker->stopMeasurePoint();
//			 break;
//		 }
//	 }
//ExitThread(0); 
//return 0;
//}
//DWORD WINAPI MyCritariaFrgndMeasureExterConTrig12( LPVOID pParam )
//{	
//	Point* Ptpoint;
//	Ptpoint = new Point();
//	CApplication_Faro_TrakerDlg* pFrgndMeasureCritatiaExterConTrig12 = (CApplication_Faro_TrakerDlg*)pParam;
//	/////////////////////////////////////////////////////////////////////////////////////
//	MeasurePointData* FrgndMeasurement = NULL;
//	StdDevFilter filter;
//	ExternalStartTrigger startTrigger;
//	ExternalContinueTrigger continueTrigger;
//	/////////////////////////////////////////////////////////////////////////////////////
//	MeasureCfg  cfg( pFrgndMeasureCritatiaExterConTrig12->SamPOb , &filter , &startTrigger , &continueTrigger);
//	gpTracker->startMeasurePoint( &cfg );
//
//	/////////////////////////////////////////////////////////////////////////////////////
//
//	 try
//	 {
//		 FrgndMeasurement = gpTracker->readMeasurePointData();		
//		 ASSERT(FrgndMeasurement);
//		 LPARAM lParam = (LPARAM)FrgndMeasurement;
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//		 pFrgndMeasureCritatiaExterConTrig12->az     =     (FrgndMeasurement->azimuth());
//		 pFrgndMeasureCritatiaExterConTrig12->ze     =     (FrgndMeasurement->zenith());    
//		 pFrgndMeasureCritatiaExterConTrig12->di     =     (FrgndMeasurement->distance())*1000;
//		
//
//		 pFrgndMeasureCritatiaExterConTrig12->x      =      pFrgndMeasureCritatiaExterConTrig12->di*sin(pFrgndMeasureCritatiaExterConTrig12->ze)*cos(pFrgndMeasureCritatiaExterConTrig12->az);
//		 pFrgndMeasureCritatiaExterConTrig12->y      =      pFrgndMeasureCritatiaExterConTrig12->di*sin(pFrgndMeasureCritatiaExterConTrig12->ze)*sin(pFrgndMeasureCritatiaExterConTrig12->az);
//		 pFrgndMeasureCritatiaExterConTrig12->z      =      pFrgndMeasureCritatiaExterConTrig12->di*cos(pFrgndMeasureCritatiaExterConTrig12->ze);
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//		 FrgndMeasurement->time();
//		 FrgndMeasurement->status();
//
//		 Ptpoint->X = pFrgndMeasureCritatiaExterConTrig12->x;
//		 Ptpoint->Y = pFrgndMeasureCritatiaExterConTrig12->y;
//		 Ptpoint->Z = pFrgndMeasureCritatiaExterConTrig12->z;
//		 Ptpoint->Cont1_trig = "External Continue Trigger";
//		 Ptpoint->Cont2_trig ="";
//		 Ptpoint->Cont3_trig ="";
//		 Ptpoint->filtter = "StdDevFilter";
//		 Ptpoint->Measurement_Mechanism = "Foreground Measurement";
//		 Ptpoint->Measurement_Mode = "Dynamic Measurement";
//		 Ptpoint->samperobs = pFrgndMeasureCritatiaExterConTrig12->SamPOb;
//		 Ptpoint->SMR = pFrgndMeasureCritatiaExterConTrig12->smr;
//		 Ptpoint->Star_tig = "External Start Trigger";
//		  GetSystemTime(&Strtime);
//		 
//		 Ptpoint->Time.Format( _T("%04d%02d%02d%02d%02d%02d%03d")
//			 ,Strtime.wYear
//			 ,Strtime.wMonth
//			 ,Strtime.wDay
//			 ,Strtime.wHour
//			 ,Strtime.wMinute
//			 ,Strtime.wSecond
//			 ,Strtime.wMilliseconds);
//
//		 traject.push_back(*Ptpoint);
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//		 pFrgndMeasureCritatiaExterConTrig12->m_x.Format("%g", pFrgndMeasureCritatiaExterConTrig12->x);
//		 pFrgndMeasureCritatiaExterConTrig12->m_y.Format("%g", pFrgndMeasureCritatiaExterConTrig12->y);
//		 pFrgndMeasureCritatiaExterConTrig12->m_z.Format("%g", pFrgndMeasureCritatiaExterConTrig12->z);
//	     
//		 CString Measureb = "x="+pFrgndMeasureCritatiaExterConTrig12->m_x+"      y="+pFrgndMeasureCritatiaExterConTrig12->m_y+"      z="+pFrgndMeasureCritatiaExterConTrig12->m_z;
//
//		 pFrgndMeasureCritatiaExterConTrig12->m_listeofmeasur.InsertString(0,Measureb);
//		 pFrgndMeasureCritatiaExterConTrig12->m_listeofmeasur.UpdateWindow();
//	 }
//	 catch(TrackerException* e )
//	 {
//		 char* msg;
//		 msg = e->getText();
//		 AfxMessageBox(msg);
//		 delete e;
//	 }
//	 if (imaad42 == -1)
//	 {
//		 gpTracker->stopMeasurePoint();
//	 }
//ExitThread(0); 
//return 0;
//}
//DWORD WINAPI MyCritariaFrgndMeasureIntertrig12( LPVOID pParam )
//{
//	CApplication_Faro_TrakerDlg* pFrgndMeasureCritatiaIntertrig12 = (CApplication_Faro_TrakerDlg*)pParam;
//	/////////////////////////////////////////////////////////////////////////////////////
//
//	MeasurePointData* FrgndMeasurement = NULL;
//	StdDevFilter filter;
//	ExternalStartTrigger startTrigger;
//	/////////////////////////////////////////////////////////////////////////////////////
//	IntervalTrigger continueTrigger(pFrgndMeasureCritatiaIntertrig12->InterTrig);
//	/////////////////////////////////////////////////////////////////////////////////////
//	MeasureCfg  cfg( pFrgndMeasureCritatiaIntertrig12->SamPOb , &filter , &startTrigger , &continueTrigger);
//	gpTracker->startMeasurePoint( &cfg );
//
//	 while (true)
//	 {
//		 Point* Ptpoint;
//	     Ptpoint = new Point();
//		 try
//		 {
//			 FrgndMeasurement = gpTracker->readMeasurePointData();		
//	         ASSERT(FrgndMeasurement);
//	         LPARAM lParam = (LPARAM)FrgndMeasurement;
//
//			 /////////////////////////////////////////////////////////////////////////////
//
//			 pFrgndMeasureCritatiaIntertrig12->az     =     (FrgndMeasurement->azimuth());
//			 pFrgndMeasureCritatiaIntertrig12->ze     =     (FrgndMeasurement->zenith());    
//			 pFrgndMeasureCritatiaIntertrig12->di     =     (FrgndMeasurement->distance())*1000;
//			
//
//			 pFrgndMeasureCritatiaIntertrig12->x      =      pFrgndMeasureCritatiaIntertrig12->di*sin(pFrgndMeasureCritatiaIntertrig12->ze)*cos(pFrgndMeasureCritatiaIntertrig12->az);
//			 pFrgndMeasureCritatiaIntertrig12->y      =      pFrgndMeasureCritatiaIntertrig12->di*sin(pFrgndMeasureCritatiaIntertrig12->ze)*sin(pFrgndMeasureCritatiaIntertrig12->az);
//			 pFrgndMeasureCritatiaIntertrig12->z      =      pFrgndMeasureCritatiaIntertrig12->di*cos(pFrgndMeasureCritatiaIntertrig12->ze);
//
//			 /////////////////////////////////////////////////////////////////////////////
//
//			 FrgndMeasurement->time();
//			 FrgndMeasurement->status();
//
//			 Ptpoint->X = pFrgndMeasureCritatiaIntertrig12->x;
//			 Ptpoint->Y = pFrgndMeasureCritatiaIntertrig12->y;
//			 Ptpoint->Z = pFrgndMeasureCritatiaIntertrig12->z;
//			 Ptpoint->Cont1_trig = "IntervalTrigger";
//			 Ptpoint->Cont2_trig = pFrgndMeasureCritatiaIntertrig12->m_InterTrig;
//			 Ptpoint->Cont3_trig ="";
//			 Ptpoint->filtter = "StdDevFilter";
//			 Ptpoint->Measurement_Mechanism = "Foreground Measurement";
//			 Ptpoint->Measurement_Mode = "Dynamic Measurement";
//			 Ptpoint->samperobs = pFrgndMeasureCritatiaIntertrig12->SamPOb;
//			 Ptpoint->SMR = pFrgndMeasureCritatiaIntertrig12->smr;
//			 Ptpoint->Star_tig = "External Start Trigger";
//			  GetSystemTime(&Strtime);
//		 
//			 Ptpoint->Time.Format( _T("%04d%02d%02d%02d%02d%02d%03d")
//				 ,Strtime.wYear
//				 ,Strtime.wMonth
//				 ,Strtime.wDay
//				 ,Strtime.wHour
//				 ,Strtime.wMinute
//				 ,Strtime.wSecond
//				 ,Strtime.wMilliseconds);
//
//			 traject.push_back(*Ptpoint);
//
//			 /////////////////////////////////////////////////////////////////////////////
//
//			 pFrgndMeasureCritatiaIntertrig12->m_x.Format("%g", pFrgndMeasureCritatiaIntertrig12->x);
//			 pFrgndMeasureCritatiaIntertrig12->m_y.Format("%g", pFrgndMeasureCritatiaIntertrig12->y);
//			 pFrgndMeasureCritatiaIntertrig12->m_z.Format("%g", pFrgndMeasureCritatiaIntertrig12->z);
//		     
//			 CString Measureb = "x="+pFrgndMeasureCritatiaIntertrig12->m_x+"      y="+pFrgndMeasureCritatiaIntertrig12->m_y+"      z="+pFrgndMeasureCritatiaIntertrig12->m_z;
//
//			 pFrgndMeasureCritatiaIntertrig12->m_listeofmeasur.InsertString(0,Measureb);
//			 pFrgndMeasureCritatiaIntertrig12->m_listeofmeasur.UpdateWindow();
//
//		 }
//		 catch(TrackerException* e )
//		 {
//			 char* msg;
//			 msg = e->getText();
//			 AfxMessageBox(msg);
//			 delete e;
//		 }
//		 if (imaad52 == -1)
//		 {
//			 gpTracker->stopMeasurePoint();
//			 break;
//		 }
//	 }
//	 ExitThread(0); 
//	 return 0;
//}
//DWORD WINAPI MyCritariaFrgndMeasureDistTrig12( LPVOID pParam )
//{	
//	CApplication_Faro_TrakerDlg* pFrgndMeasureCritatiaDistTrig12 = (CApplication_Faro_TrakerDlg*)pParam;
//	/////////////////////////////////////////////////////////////////////////////////////
//
//	MeasurePointData* FrgndMeasurement = NULL;
//	StdDevFilter filter;
//	ExternalStartTrigger startTrigger;
//	/////////////////////////////////////////////////////////////////////////////////////
//	DistanceTrigger continueTrigger(pFrgndMeasureCritatiaDistTrig12->DitTrig);
//	/////////////////////////////////////////////////////////////////////////////////////
//	MeasureCfg  cfg( pFrgndMeasureCritatiaDistTrig12->SamPOb , &filter , &startTrigger , &continueTrigger);
//	gpTracker->startMeasurePoint( &cfg );
//
//	 while (true)
//	 {
//		 Point* Ptpoint;
//	     Ptpoint = new Point();
//		 try
//		 {
//			 FrgndMeasurement = gpTracker->readMeasurePointData();		
//	         ASSERT(FrgndMeasurement);
//	         LPARAM lParam = (LPARAM)FrgndMeasurement;
//
//			 /////////////////////////////////////////////////////////////////////////////
//
//			 pFrgndMeasureCritatiaDistTrig12->az     =     (FrgndMeasurement->azimuth());
//			 pFrgndMeasureCritatiaDistTrig12->ze     =     (FrgndMeasurement->zenith());    
//			 pFrgndMeasureCritatiaDistTrig12->di     =     (FrgndMeasurement->distance())*1000;
//			
//
//			 pFrgndMeasureCritatiaDistTrig12->x      =      pFrgndMeasureCritatiaDistTrig12->di*sin(pFrgndMeasureCritatiaDistTrig12->ze)*cos(pFrgndMeasureCritatiaDistTrig12->az);
//			 pFrgndMeasureCritatiaDistTrig12->y      =      pFrgndMeasureCritatiaDistTrig12->di*sin(pFrgndMeasureCritatiaDistTrig12->ze)*sin(pFrgndMeasureCritatiaDistTrig12->az);
//			 pFrgndMeasureCritatiaDistTrig12->z      =      pFrgndMeasureCritatiaDistTrig12->di*cos(pFrgndMeasureCritatiaDistTrig12->ze);
//
//			 /////////////////////////////////////////////////////////////////////////////
//
//			 FrgndMeasurement->time();
//			 FrgndMeasurement->status();
//
//			 Ptpoint->X = pFrgndMeasureCritatiaDistTrig12->x;
//			 Ptpoint->Y = pFrgndMeasureCritatiaDistTrig12->y;
//			 Ptpoint->Z = pFrgndMeasureCritatiaDistTrig12->z;
//			 Ptpoint->Cont1_trig = "DistanceTrigger";
//			 Ptpoint->Cont2_trig ="";
//			 Ptpoint->Cont3_trig = pFrgndMeasureCritatiaDistTrig12->m_DitTrig;
//			 Ptpoint->filtter = "StdDevFilter";
//			 Ptpoint->Measurement_Mechanism = "Foreground Measurement";
//			 Ptpoint->Measurement_Mode = "Dynamic Measurement";
//			 Ptpoint->samperobs = pFrgndMeasureCritatiaDistTrig12->SamPOb;
//			 Ptpoint->SMR = pFrgndMeasureCritatiaDistTrig12->smr;
//			 Ptpoint->Star_tig = "External Start Trigger";
//			  GetSystemTime(&Strtime);
//		 
//			 Ptpoint->Time.Format( _T("%04d%02d%02d%02d%02d%02d%03d")
//				 ,Strtime.wYear
//				 ,Strtime.wMonth
//				 ,Strtime.wDay
//				 ,Strtime.wHour
//				 ,Strtime.wMinute
//				 ,Strtime.wSecond
//				 ,Strtime.wMilliseconds);
//
//			 traject.push_back(*Ptpoint);
//
//			 /////////////////////////////////////////////////////////////////////////////
//
//			 pFrgndMeasureCritatiaDistTrig12->m_x.Format("%g", pFrgndMeasureCritatiaDistTrig12->x);
//			 pFrgndMeasureCritatiaDistTrig12->m_y.Format("%g", pFrgndMeasureCritatiaDistTrig12->y);
//			 pFrgndMeasureCritatiaDistTrig12->m_z.Format("%g", pFrgndMeasureCritatiaDistTrig12->z);
//		     
//			 CString Measureb = "x="+pFrgndMeasureCritatiaDistTrig12->m_x+"      y="+pFrgndMeasureCritatiaDistTrig12->m_y+"      z="+pFrgndMeasureCritatiaDistTrig12->m_z;
//
//			 pFrgndMeasureCritatiaDistTrig12->m_listeofmeasur.InsertString(0,Measureb);
//			 pFrgndMeasureCritatiaDistTrig12->m_listeofmeasur.UpdateWindow();
//
//		 }
//		 catch(TrackerException* e )
//		 {
//			 char* msg;
//			 msg = e->getText();
//			 AfxMessageBox(msg);
//			 delete e;
//		 }
//		  if (imaad62 == -1)
//		 {
//			 gpTracker->stopMeasurePoint();
//			 break;
//		 }
//	 }
//ExitThread(0); 
//return 0;
//}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
double dif;
/////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////Measurement during a period of time////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
//Foreground Measurement//Average Filter//NullContinueTrigger//Period///////////////
DWORD WINAPI MyCritariaFrgnd_Measure_Aver_NullCont_Period( LPVOID pParam )
{
	CApplication_Faro_TrakerDlg* pFrgnd_Measure_Aver_NullCont_Period = (CApplication_Faro_TrakerDlg*)pParam;
	/////////////////////////////////////////////////////////////////////////////////////
	MeasurePointData* Frgnd_Measure_Aver_NullCont_Period = NULL;
	AverageFilter filter;
	NullStartTrigger startTrigger;
	/////////////////////////////////////////////////////////////////////////////////////
	NullContinueTrigger continueTrigger;
	/////////////////////////////////////////////////////////////////////////////////////
	MeasureCfg  cfg( pFrgnd_Measure_Aver_NullCont_Period->SamPOb , &filter , &startTrigger , &continueTrigger);
	gpTracker->startMeasurePoint( &cfg );
	time_t start,end;
	time (&start);
    Sleep((int)(PARAM4*1000));
	 while (true)
	 {
		 Point* Ptpoint;
	     Ptpoint = new Point();
		 try
		 {
			 Frgnd_Measure_Aver_NullCont_Period = gpTracker->readMeasurePointData();		
	         ASSERT(Frgnd_Measure_Aver_NullCont_Period);
	         LPARAM lParam = (LPARAM)Frgnd_Measure_Aver_NullCont_Period;
			 /////////////////////////////////////////////////////////////////////////////
			 pFrgnd_Measure_Aver_NullCont_Period->az     =     (Frgnd_Measure_Aver_NullCont_Period->azimuth());
			 pFrgnd_Measure_Aver_NullCont_Period->ze     =     (Frgnd_Measure_Aver_NullCont_Period->zenith());    
			 pFrgnd_Measure_Aver_NullCont_Period->di     =     (Frgnd_Measure_Aver_NullCont_Period->distance())*1000;
             /////////////////////////////////////////////////////////////////////////////
			 pFrgnd_Measure_Aver_NullCont_Period->x      =      pFrgnd_Measure_Aver_NullCont_Period->di*sin(pFrgnd_Measure_Aver_NullCont_Period->ze)*cos(pFrgnd_Measure_Aver_NullCont_Period->az);
			 pFrgnd_Measure_Aver_NullCont_Period->y      =      pFrgnd_Measure_Aver_NullCont_Period->di*sin(pFrgnd_Measure_Aver_NullCont_Period->ze)*sin(pFrgnd_Measure_Aver_NullCont_Period->az);
			 pFrgnd_Measure_Aver_NullCont_Period->z      =      pFrgnd_Measure_Aver_NullCont_Period->di*cos(pFrgnd_Measure_Aver_NullCont_Period->ze);
			 /////////////////////////////////////////////////////////////////////////////
			 Frgnd_Measure_Aver_NullCont_Period->time();
			 Frgnd_Measure_Aver_NullCont_Period->status();
             /////////////////////////////////////////////////////////////////////////////
			 time (&end);
			 dif = difftime (end,start);
             /////////////////////////////////////////////////////////////////////////////
			 Ptpoint->X = pFrgnd_Measure_Aver_NullCont_Period->x;
			 Ptpoint->Y = pFrgnd_Measure_Aver_NullCont_Period->y;
			 Ptpoint->Z = pFrgnd_Measure_Aver_NullCont_Period->z;
             /////////////////////////////////////////////////////////////////////////////
			 traject.push_back(*Ptpoint);
			 /////////////////////////////////////////////////////////////////////////////
			 pFrgnd_Measure_Aver_NullCont_Period->m_x.Format("%g", pFrgnd_Measure_Aver_NullCont_Period->x);
			 pFrgnd_Measure_Aver_NullCont_Period->m_y.Format("%g", pFrgnd_Measure_Aver_NullCont_Period->y);
			 pFrgnd_Measure_Aver_NullCont_Period->m_z.Format("%g", pFrgnd_Measure_Aver_NullCont_Period->z);
		     /////////////////////////////////////////////////////////////////////////////
			 CString Measureb = "x="+pFrgnd_Measure_Aver_NullCont_Period->m_x+"      y="+pFrgnd_Measure_Aver_NullCont_Period->m_y+"      z="+pFrgnd_Measure_Aver_NullCont_Period->m_z;
			 pFrgnd_Measure_Aver_NullCont_Period->m_listeofmeasur.InsertString(0,Measureb);
			 pFrgnd_Measure_Aver_NullCont_Period->m_listeofmeasur.UpdateWindow();
		 }
		 catch(TrackerException* e )
		 {
			 char* msg;
			 msg = e->getText();
			 AfxMessageBox(msg);
			 delete e;
		 }
		 if (imaad2 == -1)
		 {
			 gpTracker->stopMeasurePoint();
			 break;
		 }
		 if (dif > (PARAM3 - PARAM5))
		 {
             Sleep((int)(PARAM5*1000));
             pFrgnd_Measure_Aver_NullCont_Period->SendACQUITT("5534","END");
			 gpTracker->stopMeasurePoint();
			 break;
		 }
	 }
	 ExitThread(0); 
	 return 0;
}

//Foreground Measurement//Average Filter//NullContinueTrigger//Period///////////////
DWORD WINAPI MyCritariaFrgnd_Measure_StdD_NullCont_Period( LPVOID pParam )
{
	CApplication_Faro_TrakerDlg* pFrgnd_Measure_StdD_NullCont_Period = (CApplication_Faro_TrakerDlg*)pParam;
	/////////////////////////////////////////////////////////////////////////////////////
	MeasurePointData* Frgnd_Measure_StdD_NullCont_Period = NULL;
	StdDevFilter filter;
	NullStartTrigger startTrigger;
	/////////////////////////////////////////////////////////////////////////////////////
	NullContinueTrigger continueTrigger;
	/////////////////////////////////////////////////////////////////////////////////////
	MeasureCfg  cfg( pFrgnd_Measure_StdD_NullCont_Period->SamPOb , &filter , &startTrigger , &continueTrigger);
	gpTracker->startMeasurePoint( &cfg );
	time_t start,end;
	time (&start);
    Sleep((int)(PARAM4*1000));
	 while (true)
	 {
		 Point* Ptpoint;
	     Ptpoint = new Point();
		 try
		 {
			 Frgnd_Measure_StdD_NullCont_Period = gpTracker->readMeasurePointData();		
	         ASSERT(Frgnd_Measure_StdD_NullCont_Period);
	         LPARAM lParam = (LPARAM)Frgnd_Measure_StdD_NullCont_Period;
			 /////////////////////////////////////////////////////////////////////////////
			 pFrgnd_Measure_StdD_NullCont_Period->az     =     (Frgnd_Measure_StdD_NullCont_Period->azimuth());
			 pFrgnd_Measure_StdD_NullCont_Period->ze     =     (Frgnd_Measure_StdD_NullCont_Period->zenith());    
			 pFrgnd_Measure_StdD_NullCont_Period->di     =     (Frgnd_Measure_StdD_NullCont_Period->distance())*1000;
             /////////////////////////////////////////////////////////////////////////////
			 pFrgnd_Measure_StdD_NullCont_Period->x      =      pFrgnd_Measure_StdD_NullCont_Period->di*sin(pFrgnd_Measure_StdD_NullCont_Period->ze)*cos(pFrgnd_Measure_StdD_NullCont_Period->az);
			 pFrgnd_Measure_StdD_NullCont_Period->y      =      pFrgnd_Measure_StdD_NullCont_Period->di*sin(pFrgnd_Measure_StdD_NullCont_Period->ze)*sin(pFrgnd_Measure_StdD_NullCont_Period->az);
			 pFrgnd_Measure_StdD_NullCont_Period->z      =      pFrgnd_Measure_StdD_NullCont_Period->di*cos(pFrgnd_Measure_StdD_NullCont_Period->ze);
			 /////////////////////////////////////////////////////////////////////////////
			 Frgnd_Measure_StdD_NullCont_Period->time();
			 Frgnd_Measure_StdD_NullCont_Period->status();
             /////////////////////////////////////////////////////////////////////////////
			 time (&end);
			 dif = difftime (end,start);
			 Ptpoint->X = pFrgnd_Measure_StdD_NullCont_Period->x;
			 Ptpoint->Y = pFrgnd_Measure_StdD_NullCont_Period->y;
			 Ptpoint->Z = pFrgnd_Measure_StdD_NullCont_Period->z;
			 traject.push_back(*Ptpoint);
			 /////////////////////////////////////////////////////////////////////////////
			 pFrgnd_Measure_StdD_NullCont_Period->m_x.Format("%g", pFrgnd_Measure_StdD_NullCont_Period->x);
			 pFrgnd_Measure_StdD_NullCont_Period->m_y.Format("%g", pFrgnd_Measure_StdD_NullCont_Period->y);
			 pFrgnd_Measure_StdD_NullCont_Period->m_z.Format("%g", pFrgnd_Measure_StdD_NullCont_Period->z);    
			 CString Measureb = "x="+pFrgnd_Measure_StdD_NullCont_Period->m_x+"      y="+pFrgnd_Measure_StdD_NullCont_Period->m_y+"      z="+pFrgnd_Measure_StdD_NullCont_Period->m_z;
             /////////////////////////////////////////////////////////////////////////////
			 pFrgnd_Measure_StdD_NullCont_Period->m_listeofmeasur.InsertString(0,Measureb);
			 pFrgnd_Measure_StdD_NullCont_Period->m_listeofmeasur.UpdateWindow();
		 }
		 catch(TrackerException* e )
		 {
			 char* msg;
			 msg = e->getText();
			 AfxMessageBox(msg);
			 delete e;
		 }
		 if (imaad2 == -1)
		 {
			 gpTracker->stopMeasurePoint();
			 break;
		 }
		 if (dif > (PARAM3 - PARAM5))
		 {
             Sleep((int)(PARAM5*1000));
             pFrgnd_Measure_StdD_NullCont_Period->SendACQUITT("5534","END");
			 gpTracker->stopMeasurePoint();
			 break;
		 }
	 }
	 ExitThread(0); 
	 return 0;
}

//Foreground Measurement//Average Filter//NullContinueTrigger//Period///////////////
DWORD WINAPI MyCritariaFrgnd_Measure_Aver_Inter_Period( LPVOID pParam )
{
	CApplication_Faro_TrakerDlg* pFrgnd_Measure_Aver_Inter_Period = (CApplication_Faro_TrakerDlg*)pParam;
	/////////////////////////////////////////////////////////////////////////////////////
	MeasurePointData* Frgnd_Measure_Aver_Inter_Period = NULL;
	AverageFilter filter;
	NullStartTrigger startTrigger;
	/////////////////////////////////////////////////////////////////////////////////////
	IntervalTrigger continueTrigger (pFrgnd_Measure_Aver_Inter_Period->InterTrig);
	/////////////////////////////////////////////////////////////////////////////////////
    traject.clear();
    time_t start,end;
	time (&start);
    Sleep((int)(PARAM4*1000));
	MeasureCfg  cfg( pFrgnd_Measure_Aver_Inter_Period->SamPOb , &filter , &startTrigger , &continueTrigger);
	gpTracker->startMeasurePoint( &cfg );
	while (true)
	{
		 Point* Ptpoint;
	     Ptpoint = new Point();
		 try
		 {
			 Frgnd_Measure_Aver_Inter_Period = gpTracker->readMeasurePointData();		
	         ASSERT(Frgnd_Measure_Aver_Inter_Period);
	         LPARAM lParam = (LPARAM)Frgnd_Measure_Aver_Inter_Period;
			 /////////////////////////////////////////////////////////////////////////////
			 pFrgnd_Measure_Aver_Inter_Period->az     =     (Frgnd_Measure_Aver_Inter_Period->azimuth());
			 pFrgnd_Measure_Aver_Inter_Period->ze     =     (Frgnd_Measure_Aver_Inter_Period->zenith());    
			 pFrgnd_Measure_Aver_Inter_Period->di     =     (Frgnd_Measure_Aver_Inter_Period->distance())*1000;
             /////////////////////////////////////////////////////////////////////////////
			 pFrgnd_Measure_Aver_Inter_Period->x      =      pFrgnd_Measure_Aver_Inter_Period->di*sin(pFrgnd_Measure_Aver_Inter_Period->ze)*cos(pFrgnd_Measure_Aver_Inter_Period->az);
			 pFrgnd_Measure_Aver_Inter_Period->y      =      pFrgnd_Measure_Aver_Inter_Period->di*sin(pFrgnd_Measure_Aver_Inter_Period->ze)*sin(pFrgnd_Measure_Aver_Inter_Period->az);
			 pFrgnd_Measure_Aver_Inter_Period->z      =      pFrgnd_Measure_Aver_Inter_Period->di*cos(pFrgnd_Measure_Aver_Inter_Period->ze);
			 /////////////////////////////////////////////////////////////////////////////
			 Frgnd_Measure_Aver_Inter_Period->time();
			 Frgnd_Measure_Aver_Inter_Period->status();
			 time (&end);
			 dif = difftime (end,start);
			 Ptpoint->X = pFrgnd_Measure_Aver_Inter_Period->x;
			 Ptpoint->Y = pFrgnd_Measure_Aver_Inter_Period->y;
			 Ptpoint->Z = pFrgnd_Measure_Aver_Inter_Period->z;
			 traject.push_back(*Ptpoint);
			 /////////////////////////////////////////////////////////////////////////////
			 pFrgnd_Measure_Aver_Inter_Period->m_x.Format("%g", pFrgnd_Measure_Aver_Inter_Period->x);
			 pFrgnd_Measure_Aver_Inter_Period->m_y.Format("%g", pFrgnd_Measure_Aver_Inter_Period->y);
			 pFrgnd_Measure_Aver_Inter_Period->m_z.Format("%g", pFrgnd_Measure_Aver_Inter_Period->z);		     
			 CString Measureb = "x="+pFrgnd_Measure_Aver_Inter_Period->m_x+"      y="+pFrgnd_Measure_Aver_Inter_Period->m_y+"      z="+pFrgnd_Measure_Aver_Inter_Period->m_z;
			 pFrgnd_Measure_Aver_Inter_Period->m_listeofmeasur.InsertString(0,Measureb);
			 pFrgnd_Measure_Aver_Inter_Period->m_listeofmeasur.UpdateWindow();
		 }
		 catch(TrackerException* e )
		 {
			 char* msg;
			 msg = e->getText();
			 AfxMessageBox(msg);
			 delete e;
		 }
		 if (imaad2 == -1)
		 {
			 gpTracker->stopMeasurePoint();
			 break;
		 }
 		 if (dif > (PARAM3 - PARAM5))
		 {
             if(traject.size() < 10)
             {
                 pFrgnd_Measure_Aver_Inter_Period->SendERROR("5534","1110");
                 flag_5534[1] = 0;
                 gpTracker->stopMeasurePoint();
			     break;
             }
             Sleep((int)(PARAM5*1000));
             pFrgnd_Measure_Aver_Inter_Period->SendACQUITT("5534","END");
			 gpTracker->stopMeasurePoint();
			 break;
		 }
	 }
	 ExitThread(0); 
	 return 0;
}

//Foreground Measurement//Average Filter//NullContinueTrigger//Period///////////////
DWORD WINAPI MyCritariaFrgnd_Measure_StdD_Inter_Period( LPVOID pParam )
{
	CApplication_Faro_TrakerDlg* pFrgnd_Measure_StdD_Inter_Period = (CApplication_Faro_TrakerDlg*)pParam;
	/////////////////////////////////////////////////////////////////////////////////////
	MeasurePointData* Frgnd_Measure_StdD_Inter_Period = NULL;
	StdDevFilter filter;
	NullStartTrigger startTrigger;
	/////////////////////////////////////////////////////////////////////////////////////
	IntervalTrigger continueTrigger (pFrgnd_Measure_StdD_Inter_Period->InterTrig);
	/////////////////////////////////////////////////////////////////////////////////////
    time_t start,end;
	time (&start);	
    Sleep((int)(PARAM4*1000));
    /////////////////////////////////////////////////////////////////////////////////////
	MeasureCfg  cfg( pFrgnd_Measure_StdD_Inter_Period->SamPOb , &filter , &startTrigger , &continueTrigger);
	gpTracker->startMeasurePoint( &cfg );
	while (true)
	{
		 Point* Ptpoint;
	     Ptpoint = new Point();
		 try
		 {
			 Frgnd_Measure_StdD_Inter_Period = gpTracker->readMeasurePointData();		
	         ASSERT(Frgnd_Measure_StdD_Inter_Period);
	         LPARAM lParam = (LPARAM)Frgnd_Measure_StdD_Inter_Period;
			 /////////////////////////////////////////////////////////////////////////////
			 pFrgnd_Measure_StdD_Inter_Period->az     =     (Frgnd_Measure_StdD_Inter_Period->azimuth());
			 pFrgnd_Measure_StdD_Inter_Period->ze     =     (Frgnd_Measure_StdD_Inter_Period->zenith());    
			 pFrgnd_Measure_StdD_Inter_Period->di     =     (Frgnd_Measure_StdD_Inter_Period->distance())*1000;
             /////////////////////////////////////////////////////////////////////////////
			 pFrgnd_Measure_StdD_Inter_Period->x      =      pFrgnd_Measure_StdD_Inter_Period->di*sin(pFrgnd_Measure_StdD_Inter_Period->ze)*cos(pFrgnd_Measure_StdD_Inter_Period->az);
			 pFrgnd_Measure_StdD_Inter_Period->y      =      pFrgnd_Measure_StdD_Inter_Period->di*sin(pFrgnd_Measure_StdD_Inter_Period->ze)*sin(pFrgnd_Measure_StdD_Inter_Period->az);
			 pFrgnd_Measure_StdD_Inter_Period->z      =      pFrgnd_Measure_StdD_Inter_Period->di*cos(pFrgnd_Measure_StdD_Inter_Period->ze);
			 /////////////////////////////////////////////////////////////////////////////
			 Frgnd_Measure_StdD_Inter_Period->time();
			 Frgnd_Measure_StdD_Inter_Period->status();
			 time (&end);
			 dif = difftime (end,start);
			 Ptpoint->X = pFrgnd_Measure_StdD_Inter_Period->x;
			 Ptpoint->Y = pFrgnd_Measure_StdD_Inter_Period->y;
			 Ptpoint->Z = pFrgnd_Measure_StdD_Inter_Period->z;
			 traject.push_back(*Ptpoint);
			 /////////////////////////////////////////////////////////////////////////////
			 pFrgnd_Measure_StdD_Inter_Period->m_x.Format("%g", pFrgnd_Measure_StdD_Inter_Period->x);
			 pFrgnd_Measure_StdD_Inter_Period->m_y.Format("%g", pFrgnd_Measure_StdD_Inter_Period->y);
			 pFrgnd_Measure_StdD_Inter_Period->m_z.Format("%g", pFrgnd_Measure_StdD_Inter_Period->z);
		     /////////////////////////////////////////////////////////////////////////////
			 CString Measureb = "x="+pFrgnd_Measure_StdD_Inter_Period->m_x+"      y="+pFrgnd_Measure_StdD_Inter_Period->m_y+"      z="+pFrgnd_Measure_StdD_Inter_Period->m_z;
             /////////////////////////////////////////////////////////////////////////////
			 pFrgnd_Measure_StdD_Inter_Period->m_listeofmeasur.InsertString(0,Measureb);
			 pFrgnd_Measure_StdD_Inter_Period->m_listeofmeasur.UpdateWindow();

		 }
		 catch(TrackerException* e )
		 {
			 char* msg;
			 msg = e->getText();
			 AfxMessageBox(msg);
			 delete e;
		 }
		 if (imaad2 == -1)
		 {
			 gpTracker->stopMeasurePoint();
			 break;
		 }
 		 if (dif > (PARAM3 - PARAM5))
		 {
             if(traject.size() < 10)
             {
                 pFrgnd_Measure_StdD_Inter_Period->SendERROR("5534","1110");
                 flag_5534[4] = 0;
                 gpTracker->stopMeasurePoint();
			     break;
             }
             Sleep((int)(PARAM5*1000));
             pFrgnd_Measure_StdD_Inter_Period->SendACQUITT("5534","END");
			 gpTracker->stopMeasurePoint();
			 break;
		 }
	 }
	 ExitThread(0); 
	 return 0;
}

//Foreground Measurement//Average Filter//NullContinueTrigger//Period///////////////
DWORD WINAPI MyCritariaFrgnd_Measure_Aver_Dist_Period( LPVOID pParam )
{
	CApplication_Faro_TrakerDlg* pFrgnd_Measure_Aver_Dist_Period = (CApplication_Faro_TrakerDlg*)pParam;
	/////////////////////////////////////////////////////////////////////////////////////
	MeasurePointData* Frgnd_Measure_Aver_Dist_Period = NULL;
	AverageFilter filter;
	NullStartTrigger startTrigger;
	/////////////////////////////////////////////////////////////////////////////////////
	DistanceTrigger continueTrigger (pFrgnd_Measure_Aver_Dist_Period->DitTrig);
    /////////////////////////////////////////////////////////////////////////////////////
    traject.clear();
    time_t start,end;
	time (&start);
    Sleep((int)(PARAM4*1000));
	/////////////////////////////////////////////////////////////////////////////////////
	MeasureCfg  cfg( pFrgnd_Measure_Aver_Dist_Period->SamPOb , &filter , &startTrigger , &continueTrigger);
	gpTracker->startMeasurePoint( &cfg );
	 while (true)
	 {
		 Point* Ptpoint;
	     Ptpoint = new Point();
		 try
		 {
			 Frgnd_Measure_Aver_Dist_Period = gpTracker->readMeasurePointData();		
	         ASSERT(Frgnd_Measure_Aver_Dist_Period);
	         LPARAM lParam = (LPARAM)Frgnd_Measure_Aver_Dist_Period;
			 /////////////////////////////////////////////////////////////////////////////
			 pFrgnd_Measure_Aver_Dist_Period->az     =     (Frgnd_Measure_Aver_Dist_Period->azimuth());
			 pFrgnd_Measure_Aver_Dist_Period->ze     =     (Frgnd_Measure_Aver_Dist_Period->zenith());    
			 pFrgnd_Measure_Aver_Dist_Period->di     =     (Frgnd_Measure_Aver_Dist_Period->distance())*1000;
             /////////////////////////////////////////////////////////////////////////////
			 pFrgnd_Measure_Aver_Dist_Period->x      =      pFrgnd_Measure_Aver_Dist_Period->di*sin(pFrgnd_Measure_Aver_Dist_Period->ze)*cos(pFrgnd_Measure_Aver_Dist_Period->az);
			 pFrgnd_Measure_Aver_Dist_Period->y      =      pFrgnd_Measure_Aver_Dist_Period->di*sin(pFrgnd_Measure_Aver_Dist_Period->ze)*sin(pFrgnd_Measure_Aver_Dist_Period->az);
			 pFrgnd_Measure_Aver_Dist_Period->z      =      pFrgnd_Measure_Aver_Dist_Period->di*cos(pFrgnd_Measure_Aver_Dist_Period->ze);
			 /////////////////////////////////////////////////////////////////////////////
			 Frgnd_Measure_Aver_Dist_Period->time();
			 Frgnd_Measure_Aver_Dist_Period->status();
			 time (&end);
			 dif = difftime (end,start);
			 Ptpoint->X = pFrgnd_Measure_Aver_Dist_Period->x;
			 Ptpoint->Y = pFrgnd_Measure_Aver_Dist_Period->y;
			 Ptpoint->Z = pFrgnd_Measure_Aver_Dist_Period->z;
			 traject.push_back(*Ptpoint);
			 /////////////////////////////////////////////////////////////////////////////
			 pFrgnd_Measure_Aver_Dist_Period->m_x.Format("%g", pFrgnd_Measure_Aver_Dist_Period->x);
			 pFrgnd_Measure_Aver_Dist_Period->m_y.Format("%g", pFrgnd_Measure_Aver_Dist_Period->y);
			 pFrgnd_Measure_Aver_Dist_Period->m_z.Format("%g", pFrgnd_Measure_Aver_Dist_Period->z);
             /////////////////////////////////////////////////////////////////////////////		     
			 CString Measureb = "x="+pFrgnd_Measure_Aver_Dist_Period->m_x+"      y="+pFrgnd_Measure_Aver_Dist_Period->m_y+"      z="+pFrgnd_Measure_Aver_Dist_Period->m_z;
			 pFrgnd_Measure_Aver_Dist_Period->m_listeofmeasur.InsertString(0,Measureb);
			 pFrgnd_Measure_Aver_Dist_Period->m_listeofmeasur.UpdateWindow();
		 }
		 catch(TrackerException* e )
		 {
			 char* msg;
			 msg = e->getText();
			 AfxMessageBox(msg);
			 delete e;
		 }
		 if (imaad2 == -1)
		 {
			 gpTracker->stopMeasurePoint();
			 break;
		 }
		 if (dif > (PARAM3 - PARAM5))
		 {
             if(traject.size() < 10)
             {
                 pFrgnd_Measure_Aver_Dist_Period->SendERROR("5534","1110");
                 flag_5534[2] = 0;
                 gpTracker->stopMeasurePoint();
			     break;
             }
             Sleep((int)(PARAM5*1000));
             pFrgnd_Measure_Aver_Dist_Period->SendACQUITT("5534","END");
			 gpTracker->stopMeasurePoint();
			 break;
		 }
	 }
	 ExitThread(0); 
	 return 0;
}

//Foreground Measurement//Average Filter//NullContinueTrigger//Period///////////////
DWORD WINAPI MyCritariaFrgnd_Measure_StdD_Dist_Period( LPVOID pParam )
{
	CApplication_Faro_TrakerDlg* pFrgnd_Measure_StdD_Dist_Period = (CApplication_Faro_TrakerDlg*)pParam;
	/////////////////////////////////////////////////////////////////////////////////////
	MeasurePointData* Frgnd_Measure_StdD_Dist_Period = NULL;
	StdDevFilter filter;
	NullStartTrigger startTrigger;
	/////////////////////////////////////////////////////////////////////////////////////
	DistanceTrigger continueTrigger (pFrgnd_Measure_StdD_Dist_Period->DitTrig);
	/////////////////////////////////////////////////////////////////////////////////////
    traject.clear();
    time_t start,end;
	time (&start);
    Sleep((int)(PARAM4*1000));
    /////////////////////////////////////////////////////////////////////////////////////
	MeasureCfg  cfg( pFrgnd_Measure_StdD_Dist_Period->SamPOb , &filter , &startTrigger , &continueTrigger);
	gpTracker->startMeasurePoint( &cfg );
	
	while (true)
	{
		 Point* Ptpoint;
	     Ptpoint = new Point();
		 try
		 {
			 Frgnd_Measure_StdD_Dist_Period = gpTracker->readMeasurePointData();		
	         ASSERT(Frgnd_Measure_StdD_Dist_Period);
	         LPARAM lParam = (LPARAM)Frgnd_Measure_StdD_Dist_Period;
			 /////////////////////////////////////////////////////////////////////////////
			 pFrgnd_Measure_StdD_Dist_Period->az     =     (Frgnd_Measure_StdD_Dist_Period->azimuth());
			 pFrgnd_Measure_StdD_Dist_Period->ze     =     (Frgnd_Measure_StdD_Dist_Period->zenith());    
			 pFrgnd_Measure_StdD_Dist_Period->di     =     (Frgnd_Measure_StdD_Dist_Period->distance())*1000;
             /////////////////////////////////////////////////////////////////////////////		
			 pFrgnd_Measure_StdD_Dist_Period->x      =      pFrgnd_Measure_StdD_Dist_Period->di*sin(pFrgnd_Measure_StdD_Dist_Period->ze)*cos(pFrgnd_Measure_StdD_Dist_Period->az);
			 pFrgnd_Measure_StdD_Dist_Period->y      =      pFrgnd_Measure_StdD_Dist_Period->di*sin(pFrgnd_Measure_StdD_Dist_Period->ze)*sin(pFrgnd_Measure_StdD_Dist_Period->az);
			 pFrgnd_Measure_StdD_Dist_Period->z      =      pFrgnd_Measure_StdD_Dist_Period->di*cos(pFrgnd_Measure_StdD_Dist_Period->ze);
			 /////////////////////////////////////////////////////////////////////////////
			 Frgnd_Measure_StdD_Dist_Period->time();
			 Frgnd_Measure_StdD_Dist_Period->status();
			 time (&end);
			 dif = difftime (end,start);
			 Ptpoint->X = pFrgnd_Measure_StdD_Dist_Period->x;
			 Ptpoint->Y = pFrgnd_Measure_StdD_Dist_Period->y;
			 Ptpoint->Z = pFrgnd_Measure_StdD_Dist_Period->z;
			 traject.push_back(*Ptpoint);
			 /////////////////////////////////////////////////////////////////////////////
			 pFrgnd_Measure_StdD_Dist_Period->m_x.Format("%g", pFrgnd_Measure_StdD_Dist_Period->x);
			 pFrgnd_Measure_StdD_Dist_Period->m_y.Format("%g", pFrgnd_Measure_StdD_Dist_Period->y);
			 pFrgnd_Measure_StdD_Dist_Period->m_z.Format("%g", pFrgnd_Measure_StdD_Dist_Period->z);	     
			 CString Measureb = "x="+pFrgnd_Measure_StdD_Dist_Period->m_x+"      y="+pFrgnd_Measure_StdD_Dist_Period->m_y+"      z="+pFrgnd_Measure_StdD_Dist_Period->m_z;
			 pFrgnd_Measure_StdD_Dist_Period->m_listeofmeasur.InsertString(0,Measureb);
			 pFrgnd_Measure_StdD_Dist_Period->m_listeofmeasur.UpdateWindow();
		 }
		 catch(TrackerException* e )
		 {
			 char* msg;
			 msg = e->getText();
			 AfxMessageBox(msg);
			 delete e;
		 }
		 if (imaad2 == -1)
		 {
			 gpTracker->stopMeasurePoint();
			 break;
		 }
		 if (dif > (PARAM3 - PARAM5))
		 {
             if(traject.size() < 10)
             {
                 pFrgnd_Measure_StdD_Dist_Period->SendERROR("5534","1110");
                 flag_5534[5] = 0;
                 gpTracker->stopMeasurePoint();
			     break;
             }
             Sleep((int)(PARAM5*1000));
             pFrgnd_Measure_StdD_Dist_Period->SendACQUITT("5534","END");
			 gpTracker->stopMeasurePoint();
			 break;
		 }
	 }
	 ExitThread(0); 
	 return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////Static Measurement/////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////Static Measurement//Average Filter/////////////////////////////////
DWORD WINAPI MyCritariaFrgnd_Measure_Aver_PeriodView( LPVOID pParam )
{
	CApplication_Faro_TrakerDlg* pFrgnd_Measure_Aver_PeriodView = (CApplication_Faro_TrakerDlg*)pParam;
	/////////////////////////////////////////////////////////////////////////////////////
	MeasurePointData* Frgnd_Measure_Aver_PeriodView = NULL;
	AverageFilter filter;
	NullStartTrigger startTrigger;
	/////////////////////////////////////////////////////////////////////////////////////
	NullContinueTrigger continueTrigger;
	/////////////////////////////////////////////////////////////////////////////////////
	MeasureCfg  cfg( pFrgnd_Measure_Aver_PeriodView->SamPOb , &filter , &startTrigger , &continueTrigger);
	gpTracker->startMeasurePoint( &cfg );
	try
	{
		Frgnd_Measure_Aver_PeriodView = gpTracker->readMeasurePointData();		
		ASSERT(Frgnd_Measure_Aver_PeriodView);
		LPARAM lParam = (LPARAM)Frgnd_Measure_Aver_PeriodView;
		/////////////////////////////////////////////////////////////////////////////
		pFrgnd_Measure_Aver_PeriodView->az     =     (Frgnd_Measure_Aver_PeriodView->azimuth());
		pFrgnd_Measure_Aver_PeriodView->ze     =     (Frgnd_Measure_Aver_PeriodView->zenith());    
		pFrgnd_Measure_Aver_PeriodView->di     =     (Frgnd_Measure_Aver_PeriodView->distance())*1000;
		/////////////////////////////////////////////////////////////////////////////
		pFrgnd_Measure_Aver_PeriodView->x      =      pFrgnd_Measure_Aver_PeriodView->di*sin(pFrgnd_Measure_Aver_PeriodView->ze)*cos(pFrgnd_Measure_Aver_PeriodView->az);
		pFrgnd_Measure_Aver_PeriodView->y      =      pFrgnd_Measure_Aver_PeriodView->di*sin(pFrgnd_Measure_Aver_PeriodView->ze)*sin(pFrgnd_Measure_Aver_PeriodView->az);
		pFrgnd_Measure_Aver_PeriodView->z      =      pFrgnd_Measure_Aver_PeriodView->di*cos(pFrgnd_Measure_Aver_PeriodView->ze);
		/////////////////////////////////////////////////////////////////////////////
		Frgnd_Measure_Aver_PeriodView->time();
		Frgnd_Measure_Aver_PeriodView->status();
		/////////////////////////////////////////////////////////////////////////////
		X1.Format("%g", pFrgnd_Measure_Aver_PeriodView->x);
	    Y1.Format("%g", pFrgnd_Measure_Aver_PeriodView->y);
	    Z1.Format("%g", pFrgnd_Measure_Aver_PeriodView->z);
		/////////////////////////////////////////////////////////////////////////////
		memset(buffer,0,sizeof (buffer));
	    strcpy_s(buffer,"#5531["+Inparams[0]+";"+X1+";"+Y1+";"+Z1+"]"); // Copie la chaine de caractère dans buffer
        nombre_de_caractere=send(id_de_la_nouvelle_socket,buffer,strlen(buffer),0);
	}
    catch(TrackerException* e )
	{
	    char* msg;
		msg = e->getText();
		AfxMessageBox(msg);
		delete e;
	}	
	gpTracker->stopMeasurePoint();
	ExitThread(0); 
	return 0;
}

//////////////////////////Static Measurement//StdDevFilter///////////////////////////////////
DWORD WINAPI MyCritariaFrgnd_Measure_StdD_PeriodView( LPVOID pParam )
{
	CApplication_Faro_TrakerDlg* pFrgnd_Measure_StdD_PeriodView = (CApplication_Faro_TrakerDlg*)pParam;
	/////////////////////////////////////////////////////////////////////////////////////
	MeasurePointData* Frgnd_Measure_StdD_PeriodView = NULL;
	AverageFilter filter;
	NullStartTrigger startTrigger;
	/////////////////////////////////////////////////////////////////////////////////////
	NullContinueTrigger continueTrigger;
	/////////////////////////////////////////////////////////////////////////////////////
	MeasureCfg  cfg( pFrgnd_Measure_StdD_PeriodView->SamPOb , &filter , &startTrigger , &continueTrigger);
	gpTracker->startMeasurePoint( &cfg );
	try
	{
		Frgnd_Measure_StdD_PeriodView = gpTracker->readMeasurePointData();		
		ASSERT(Frgnd_Measure_StdD_PeriodView);
		LPARAM lParam = (LPARAM)Frgnd_Measure_StdD_PeriodView;
		/////////////////////////////////////////////////////////////////////////////
		pFrgnd_Measure_StdD_PeriodView->az     =     (Frgnd_Measure_StdD_PeriodView->azimuth());
		pFrgnd_Measure_StdD_PeriodView->ze     =     (Frgnd_Measure_StdD_PeriodView->zenith());    
		pFrgnd_Measure_StdD_PeriodView->di     =     (Frgnd_Measure_StdD_PeriodView->distance())*1000;
		/////////////////////////////////////////////////////////////////////////////
		pFrgnd_Measure_StdD_PeriodView->x      =      pFrgnd_Measure_StdD_PeriodView->di*sin(pFrgnd_Measure_StdD_PeriodView->ze)*cos(pFrgnd_Measure_StdD_PeriodView->az);
		pFrgnd_Measure_StdD_PeriodView->y      =      pFrgnd_Measure_StdD_PeriodView->di*sin(pFrgnd_Measure_StdD_PeriodView->ze)*sin(pFrgnd_Measure_StdD_PeriodView->az);
		pFrgnd_Measure_StdD_PeriodView->z      =      pFrgnd_Measure_StdD_PeriodView->di*cos(pFrgnd_Measure_StdD_PeriodView->ze);
		/////////////////////////////////////////////////////////////////////////////
		Frgnd_Measure_StdD_PeriodView->time();
		Frgnd_Measure_StdD_PeriodView->status();
		/////////////////////////////////////////////////////////////////////////////
		X1.Format("%g", pFrgnd_Measure_StdD_PeriodView->x);
	    Y1.Format("%g", pFrgnd_Measure_StdD_PeriodView->y);
	    Z1.Format("%g", pFrgnd_Measure_StdD_PeriodView->z);
		/////////////////////////////////////////////////////////////////////////////
		memset(buffer,0,sizeof (buffer));
	    strcpy_s(buffer,"#5531["+Inparams[0]+";"+X1+";"+Y1+";"+Z1+"]"); // Copie la chaine de caractère dans buffer
        nombre_de_caractere=send(id_de_la_nouvelle_socket,buffer,strlen(buffer),0);
	}
    catch(TrackerException* e )
	{
	    char* msg;
		msg = e->getText();
		AfxMessageBox(msg);
		delete e;
	}	
	gpTracker->stopMeasurePoint();
	ExitThread(0); 
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////
DWORD WINAPI MyCritariaFrgnd_Measure_Aver_NullCont_Yes_StopROB( LPVOID pParam )
{
    bool AVERTOL = false;
    double Xold = 0;
    double Yold = 0;
    double Zold = 0;

	CApplication_Faro_TrakerDlg* pFrgnd_Measure_Aver_NullCont_Yes_StopROB = (CApplication_Faro_TrakerDlg*)pParam;
	/////////////////////////////////////////////////////////////////////////////////////
	MeasurePointData* Frgnd_Measure_Aver_NullCont_Yes_Stop = NULL;
	AverageFilter filter;
	NullStartTrigger startTrigger;
	/////////////////////////////////////////////////////////////////////////////////////
	NullContinueTrigger continueTrigger;
	/////////////////////////////////////////////////////////////////////////////////////
	MeasureCfg  cfg( pFrgnd_Measure_Aver_NullCont_Yes_StopROB->SamPOb , &filter , &startTrigger , &continueTrigger);
	gpTracker->startMeasurePoint( &cfg );
	traject.clear();
	while (true)
	{
		 Point* Ptpoint;
	     Ptpoint = new Point();
		 try
		 {
			 Frgnd_Measure_Aver_NullCont_Yes_Stop = gpTracker->readMeasurePointData();		
	         ASSERT(Frgnd_Measure_Aver_NullCont_Yes_Stop);
	         LPARAM lParam = (LPARAM)Frgnd_Measure_Aver_NullCont_Yes_Stop;
			 /////////////////////////////////////////////////////////////////////////////
			 pFrgnd_Measure_Aver_NullCont_Yes_StopROB->az     =     (Frgnd_Measure_Aver_NullCont_Yes_Stop->azimuth());
			 pFrgnd_Measure_Aver_NullCont_Yes_StopROB->ze     =     (Frgnd_Measure_Aver_NullCont_Yes_Stop->zenith());    
			 pFrgnd_Measure_Aver_NullCont_Yes_StopROB->di     =     (Frgnd_Measure_Aver_NullCont_Yes_Stop->distance())*1000;
             /////////////////////////////////////////////////////////////////////////////
			 pFrgnd_Measure_Aver_NullCont_Yes_StopROB->x      =      pFrgnd_Measure_Aver_NullCont_Yes_StopROB->di*sin(pFrgnd_Measure_Aver_NullCont_Yes_StopROB->ze)*cos(pFrgnd_Measure_Aver_NullCont_Yes_StopROB->az);
			 pFrgnd_Measure_Aver_NullCont_Yes_StopROB->y      =      pFrgnd_Measure_Aver_NullCont_Yes_StopROB->di*sin(pFrgnd_Measure_Aver_NullCont_Yes_StopROB->ze)*sin(pFrgnd_Measure_Aver_NullCont_Yes_StopROB->az);
			 pFrgnd_Measure_Aver_NullCont_Yes_StopROB->z      =      pFrgnd_Measure_Aver_NullCont_Yes_StopROB->di*cos(pFrgnd_Measure_Aver_NullCont_Yes_StopROB->ze);     
			 /////////////////////////////////////////////////////////////////////////////		
			 Size_Collec_Stop = traject.size();
             /////////////////////////////////////////////////////////////////////////////
             if (Size_Collec_Stop > 0)
			 {
				
				 Distance_Point = sqrt((Xold - pFrgnd_Measure_Aver_NullCont_Yes_StopROB->x)*(Xold - pFrgnd_Measure_Aver_NullCont_Yes_StopROB->x)+(Yold - pFrgnd_Measure_Aver_NullCont_Yes_StopROB->y)*(Yold - pFrgnd_Measure_Aver_NullCont_Yes_StopROB->y)+(Zold - pFrgnd_Measure_Aver_NullCont_Yes_StopROB->z)*(Zold - pFrgnd_Measure_Aver_NullCont_Yes_StopROB->z));
                 Xold=pFrgnd_Measure_Aver_NullCont_Yes_StopROB->x ;
                 Yold=pFrgnd_Measure_Aver_NullCont_Yes_StopROB->y ;
                 Zold=pFrgnd_Measure_Aver_NullCont_Yes_StopROB->z ;

                 if (Distance_Point < PARAM3)
				 {
                     if ( AVERTOL == true)
                     {
                         Ptpoint->X = pFrgnd_Measure_Aver_NullCont_Yes_StopROB->x;
			             Ptpoint->Y = pFrgnd_Measure_Aver_NullCont_Yes_StopROB->y;
			             Ptpoint->Z = pFrgnd_Measure_Aver_NullCont_Yes_StopROB->z;
			             traject.push_back(*Ptpoint);
                         pFrgnd_Measure_Aver_NullCont_Yes_StopROB->m_x.Format("%g", pFrgnd_Measure_Aver_NullCont_Yes_StopROB->x);
				         pFrgnd_Measure_Aver_NullCont_Yes_StopROB->m_y.Format("%g", pFrgnd_Measure_Aver_NullCont_Yes_StopROB->y);
				         pFrgnd_Measure_Aver_NullCont_Yes_StopROB->m_z.Format("%g", pFrgnd_Measure_Aver_NullCont_Yes_StopROB->z);
                     	 memset(buffer,0,sizeof (buffer));
	                     strcpy_s(buffer,"#5535["+Inparams[0]+";"+pFrgnd_Measure_Aver_NullCont_Yes_StopROB->m_x+";"+pFrgnd_Measure_Aver_NullCont_Yes_StopROB->m_y+";"+pFrgnd_Measure_Aver_NullCont_Yes_StopROB->m_z+"]"); // Copie la chaine de caractère dans buffer
                         nombre_de_caractere = send(id_de_la_nouvelle_socket,buffer,strlen(buffer),0);
                         CString Measureb = "x="+pFrgnd_Measure_Aver_NullCont_Yes_StopROB->m_x+"      y="+pFrgnd_Measure_Aver_NullCont_Yes_StopROB->m_y+"      z="+pFrgnd_Measure_Aver_NullCont_Yes_StopROB->m_z;
					     pFrgnd_Measure_Aver_NullCont_Yes_StopROB->m_listeofmeasur.InsertString(0,Measureb);
					     pFrgnd_Measure_Aver_NullCont_Yes_StopROB->m_listeofmeasur.UpdateWindow();
                         AVERTOL = false;
                     }
					 ////////////////////////////////////////////////////////////////////////////		 
				 }
                 else
                 {
                     AVERTOL = true;
                 }
			 }
             else
			 {
			 	 Ptpoint->X = pFrgnd_Measure_Aver_NullCont_Yes_StopROB->x;
				 Ptpoint->Y = pFrgnd_Measure_Aver_NullCont_Yes_StopROB->y;
				 Ptpoint->Z = pFrgnd_Measure_Aver_NullCont_Yes_StopROB->z;
				 traject.push_back(*Ptpoint);
                 Xold= Ptpoint->X;
                 Yold= Ptpoint->Y;
                 Zold= Ptpoint->Z;
				 /////////////////////////////////////////////////////////////////////////////
				 pFrgnd_Measure_Aver_NullCont_Yes_StopROB->m_x.Format("%g", pFrgnd_Measure_Aver_NullCont_Yes_StopROB->x);
				 pFrgnd_Measure_Aver_NullCont_Yes_StopROB->m_y.Format("%g", pFrgnd_Measure_Aver_NullCont_Yes_StopROB->y);
				 pFrgnd_Measure_Aver_NullCont_Yes_StopROB->m_z.Format("%g", pFrgnd_Measure_Aver_NullCont_Yes_StopROB->z);		     
				 CString Measureb = "x="+pFrgnd_Measure_Aver_NullCont_Yes_StopROB->m_x+"      y="+pFrgnd_Measure_Aver_NullCont_Yes_StopROB->m_y+"      z="+pFrgnd_Measure_Aver_NullCont_Yes_StopROB->m_z;
				 pFrgnd_Measure_Aver_NullCont_Yes_StopROB->m_listeofmeasur.InsertString(0,Measureb);
				 pFrgnd_Measure_Aver_NullCont_Yes_StopROB->m_listeofmeasur.UpdateWindow();			 
			 }	
		 }
		 catch(TrackerException* e )
		 {
			 char* msg;
			 msg = e->getText();
			 AfxMessageBox(msg);
			 delete e;
		 }
		 if (imaad2 == -1)
		 {
			 gpTracker->stopMeasurePoint();
			 break;
		 }
	 }
	 ExitThread(0); 
	 return 0;
}

//Foreground Measurement//StdDevFilter//NullContinueTrigger//Yes Stop collec/////////////////
DWORD WINAPI MyCritariaFrgnd_Measure_StdD_NullCont_Yes_StopROB( LPVOID pParam )
{
    bool AVERTOL = false;
    double Xold = 0;
    double Yold = 0;
    double Zold = 0;
	CApplication_Faro_TrakerDlg* pFrgnd_Measure_StdD_NullCont_Yes_StopROB = (CApplication_Faro_TrakerDlg*)pParam;
	/////////////////////////////////////////////////////////////////////////////////////
	MeasurePointData* Frgnd_Measure_StdD_NullCont_Yes_Stop = NULL;
	StdDevFilter filter;
	NullStartTrigger startTrigger;
	/////////////////////////////////////////////////////////////////////////////////////
	NullContinueTrigger continueTrigger;
	/////////////////////////////////////////////////////////////////////////////////////
	MeasureCfg  cfg( pFrgnd_Measure_StdD_NullCont_Yes_StopROB->SamPOb , &filter , &startTrigger , &continueTrigger);
	gpTracker->startMeasurePoint( &cfg );
	traject.clear();
	 while (true)
	 {
		 Point* Ptpoint;
	     Ptpoint = new Point();
		 try
		 {
			 Frgnd_Measure_StdD_NullCont_Yes_Stop = gpTracker->readMeasurePointData();		
	         ASSERT(Frgnd_Measure_StdD_NullCont_Yes_Stop);
	         LPARAM lParam = (LPARAM)Frgnd_Measure_StdD_NullCont_Yes_Stop;
			 /////////////////////////////////////////////////////////////////////////////
			 pFrgnd_Measure_StdD_NullCont_Yes_StopROB->az     =     (Frgnd_Measure_StdD_NullCont_Yes_Stop->azimuth());
			 pFrgnd_Measure_StdD_NullCont_Yes_StopROB->ze     =     (Frgnd_Measure_StdD_NullCont_Yes_Stop->zenith());    
			 pFrgnd_Measure_StdD_NullCont_Yes_StopROB->di     =     (Frgnd_Measure_StdD_NullCont_Yes_Stop->distance())*1000;
             /////////////////////////////////////////////////////////////////////////////	
			 pFrgnd_Measure_StdD_NullCont_Yes_StopROB->x      =      pFrgnd_Measure_StdD_NullCont_Yes_StopROB->di*sin(pFrgnd_Measure_StdD_NullCont_Yes_StopROB->ze)*cos(pFrgnd_Measure_StdD_NullCont_Yes_StopROB->az);
			 pFrgnd_Measure_StdD_NullCont_Yes_StopROB->y      =      pFrgnd_Measure_StdD_NullCont_Yes_StopROB->di*sin(pFrgnd_Measure_StdD_NullCont_Yes_StopROB->ze)*sin(pFrgnd_Measure_StdD_NullCont_Yes_StopROB->az);
			 pFrgnd_Measure_StdD_NullCont_Yes_StopROB->z      =      pFrgnd_Measure_StdD_NullCont_Yes_StopROB->di*cos(pFrgnd_Measure_StdD_NullCont_Yes_StopROB->ze);
			 /////////////////////////////////////////////////////////////////////////////
			 Size_Collec_Stop = traject.size();
             if (Size_Collec_Stop > 0)
			 {
				 Distance_Point = sqrt((traject[Size_Collec_Stop - 1].X - pFrgnd_Measure_StdD_NullCont_Yes_StopROB->x)*(traject[Size_Collec_Stop - 1].X - pFrgnd_Measure_StdD_NullCont_Yes_StopROB->x)+(traject[Size_Collec_Stop - 1].Y - pFrgnd_Measure_StdD_NullCont_Yes_StopROB->y)*(traject[Size_Collec_Stop - 1].Y - pFrgnd_Measure_StdD_NullCont_Yes_StopROB->y)+(traject[Size_Collec_Stop - 1].Z - pFrgnd_Measure_StdD_NullCont_Yes_StopROB->z)*(traject[Size_Collec_Stop - 1].Z - pFrgnd_Measure_StdD_NullCont_Yes_StopROB->z));
                 Xold=pFrgnd_Measure_StdD_NullCont_Yes_StopROB->x ;
                 Yold=pFrgnd_Measure_StdD_NullCont_Yes_StopROB->y ;
                 Zold=pFrgnd_Measure_StdD_NullCont_Yes_StopROB->z ;
				 if (Distance_Point < PARAM3)
				 {
                     if (AVERTOL == true)
                     {
                         Ptpoint->X = pFrgnd_Measure_StdD_NullCont_Yes_StopROB->x;
					     Ptpoint->Y = pFrgnd_Measure_StdD_NullCont_Yes_StopROB->y;
					     Ptpoint->Z = pFrgnd_Measure_StdD_NullCont_Yes_StopROB->z;	
					     traject.push_back(*Ptpoint);
					     /////////////////////////////////////////////////////////////////////////////
					     pFrgnd_Measure_StdD_NullCont_Yes_StopROB->m_x.Format("%g", pFrgnd_Measure_StdD_NullCont_Yes_StopROB->x);
					     pFrgnd_Measure_StdD_NullCont_Yes_StopROB->m_y.Format("%g", pFrgnd_Measure_StdD_NullCont_Yes_StopROB->y);
					     pFrgnd_Measure_StdD_NullCont_Yes_StopROB->m_z.Format("%g", pFrgnd_Measure_StdD_NullCont_Yes_StopROB->z);
                      	 memset(buffer,0,sizeof (buffer));
	                     strcpy_s(buffer,"#5535["+Inparams[0]+";"+pFrgnd_Measure_StdD_NullCont_Yes_StopROB->m_x+";"+pFrgnd_Measure_StdD_NullCont_Yes_StopROB->m_y+";"+pFrgnd_Measure_StdD_NullCont_Yes_StopROB->m_z+"]"); // Copie la chaine de caractère dans buffer
                         nombre_de_caractere = send(id_de_la_nouvelle_socket,buffer,strlen(buffer),0);
					     CString Measureb = "x="+pFrgnd_Measure_StdD_NullCont_Yes_StopROB->m_x+"      y="+pFrgnd_Measure_StdD_NullCont_Yes_StopROB->m_y+"      z="+pFrgnd_Measure_StdD_NullCont_Yes_StopROB->m_z;
					     pFrgnd_Measure_StdD_NullCont_Yes_StopROB->m_listeofmeasur.InsertString(0,Measureb);
					     pFrgnd_Measure_StdD_NullCont_Yes_StopROB->m_listeofmeasur.UpdateWindow();
                         AVERTOL = false;
                     }
                     else
                     {
                         AVERTOL = true;
                     }
				 }
			 }
             else
			 {
			 	 Ptpoint->X = pFrgnd_Measure_StdD_NullCont_Yes_StopROB->x;
				 Ptpoint->Y = pFrgnd_Measure_StdD_NullCont_Yes_StopROB->y;
				 Ptpoint->Z = pFrgnd_Measure_StdD_NullCont_Yes_StopROB->z;
				 traject.push_back(*Ptpoint);
                 Xold= Ptpoint->X;
                 Yold= Ptpoint->Y;
                 Zold= Ptpoint->Z;
				 /////////////////////////////////////////////////////////////////////////////
				 pFrgnd_Measure_StdD_NullCont_Yes_StopROB->m_x.Format("%g", pFrgnd_Measure_StdD_NullCont_Yes_StopROB->x);
				 pFrgnd_Measure_StdD_NullCont_Yes_StopROB->m_y.Format("%g", pFrgnd_Measure_StdD_NullCont_Yes_StopROB->y);
				 pFrgnd_Measure_StdD_NullCont_Yes_StopROB->m_z.Format("%g", pFrgnd_Measure_StdD_NullCont_Yes_StopROB->z);			     
				 CString Measureb = "x="+pFrgnd_Measure_StdD_NullCont_Yes_StopROB->m_x+"      y="+pFrgnd_Measure_StdD_NullCont_Yes_StopROB->m_y+"      z="+pFrgnd_Measure_StdD_NullCont_Yes_StopROB->m_z;
				 pFrgnd_Measure_StdD_NullCont_Yes_StopROB->m_listeofmeasur.InsertString(0,Measureb);
				 pFrgnd_Measure_StdD_NullCont_Yes_StopROB->m_listeofmeasur.UpdateWindow();			 
			 }				 
		 }
		 catch(TrackerException* e )
		 {
			 char* msg;
			 msg = e->getText();
			 AfxMessageBox(msg);
			 delete e;
		 }
		 if (imaad2 == -1)
		 {
			 gpTracker->stopMeasurePoint();
			 break;
		 }
	 }
	 ExitThread(0); 
	 return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////
DWORD WINAPI MyCritariaBknd_Measure_Aver_PeriodView( LPVOID pParam )
{
	CApplication_Faro_TrakerDlg* pBknd_Measure_Aver_PeriodView = (CApplication_Faro_TrakerDlg*)pParam;
	/////////////////////////////////////////////////////////////////////////////////////
	MeasurePointData* Bknd_Measure_Aver_PeriodView = NULL;
	AverageFilter filter;
	NullStartTrigger startTrigger;
	/////////////////////////////////////////////////////////////////////////////////////
    IntervalTrigger continueTrigger(pBknd_Measure_Aver_PeriodView->diplay_period);
	/////////////////////////////////////////////////////////////////////////////////////
	MeasureCfg  cfg( 20 , &filter , &startTrigger , &continueTrigger);
	gpTracker->setBkndMeasureEventRate(1);
   
    gpTracker->setBkndMeasureBlocking(true);
	gpTracker->startBkndMeasurePoint( &cfg );
	 while (true)
	 {		 
		 try
		 {
			 Bknd_Measure_Aver_PeriodView = gpTracker->readBkndMeasurePointData();		
	         ASSERT(Bknd_Measure_Aver_PeriodView);
	         LPARAM lParam = (LPARAM)Bknd_Measure_Aver_PeriodView;
			 /////////////////////////////////////////////////////////////////////////////
			 pBknd_Measure_Aver_PeriodView->az     =     (Bknd_Measure_Aver_PeriodView->azimuth());
			 pBknd_Measure_Aver_PeriodView->ze     =     (Bknd_Measure_Aver_PeriodView->zenith());    
			 pBknd_Measure_Aver_PeriodView->di     =     (Bknd_Measure_Aver_PeriodView->distance())*1000;
			 /////////////////////////////////////////////////////////////////////////////
			 pBknd_Measure_Aver_PeriodView->x      =      pBknd_Measure_Aver_PeriodView->di*sin(pBknd_Measure_Aver_PeriodView->ze)*cos(pBknd_Measure_Aver_PeriodView->az);
			 pBknd_Measure_Aver_PeriodView->y      =      pBknd_Measure_Aver_PeriodView->di*sin(pBknd_Measure_Aver_PeriodView->ze)*sin(pBknd_Measure_Aver_PeriodView->az);
			 pBknd_Measure_Aver_PeriodView->z      =      pBknd_Measure_Aver_PeriodView->di*cos(pBknd_Measure_Aver_PeriodView->ze);
			 /////////////////////////////////////////////////////////////////////////////
			 Bknd_Measure_Aver_PeriodView->time();
			 Bknd_Measure_Aver_PeriodView->status();		
			 /////////////////////////////////////////////////////////////////////////////
			 pBknd_Measure_Aver_PeriodView->m_x.Format("%g", pBknd_Measure_Aver_PeriodView->x);
			 pBknd_Measure_Aver_PeriodView->m_y.Format("%g", pBknd_Measure_Aver_PeriodView->y);
			 pBknd_Measure_Aver_PeriodView->m_z.Format("%g", pBknd_Measure_Aver_PeriodView->z);
             /////////////////////////////////////////////////////////////////////////////
			 
			 pBknd_Measure_Aver_PeriodView->GetDlgItem(IDC_EDIT_X)->SetWindowText(pBknd_Measure_Aver_PeriodView->m_x);
		     pBknd_Measure_Aver_PeriodView->GetDlgItem(IDC_EDIT_Y)->SetWindowText(pBknd_Measure_Aver_PeriodView->m_y);
		     pBknd_Measure_Aver_PeriodView->GetDlgItem(IDC_EDIT_Z)->SetWindowText(pBknd_Measure_Aver_PeriodView->m_z);
			 /////////////////////////////////////////////////////////////////////////////
		     pBknd_Measure_Aver_PeriodView->GetDlgItem(IDC_EDIT_X)->UpdateWindow();
		     pBknd_Measure_Aver_PeriodView->GetDlgItem(IDC_EDIT_Y)->UpdateWindow();
		     pBknd_Measure_Aver_PeriodView->GetDlgItem(IDC_EDIT_Z)->UpdateWindow();
		 }
		 catch(TrackerException* e )
		 {
			 char* msg;
			 msg = e->getText();
			 AfxMessageBox(msg);
			 delete e;
		 }
		 if (STOP_DISPLAY == -1)
		 {
			 gpTracker->stopBkndMeasurePoint();
			 break;
		 }
	 }
	 ExitThread(0); 
	 return 0;
}
DWORD WINAPI MyCritaria_MOVE_TO( LPVOID pParam )
{
    CApplication_Faro_TrakerDlg* P_MOVE_TO = (CApplication_Faro_TrakerDlg*)pParam;
    try
    {
        
        gpTracker->move(XM,YM,ZM,isBacksight,isRelative);

    }
    catch(TrackerException* e)
    {
        char* msg = "";
        msg = e->getText();
        if (msg != "")
        {
            P_MOVE_TO->SendERROR("5536","ERROR_MOTOR_OFF || INVALID_LOCATION");
        }
        delete e;
    }
    ExitThread(0); 
    return 0;

}
DWORD WINAPI MyCritaria_SEARCH_FOR_SMR( LPVOID pParam )
{
    CApplication_Faro_TrakerDlg* p_SEARCH_FOR_SMR = (CApplication_Faro_TrakerDlg*)pParam;
    try
    {
        gpTracker->search(RADIUS,1);   
        p_SEARCH_FOR_SMR->SendACQUITT("5537","TARGET_FOUND");
        
       /* if(DISTAN != 0)
        { */           
            
        //}
       /* else
        {
            gpTracker->search(RADIUS);
            p_SEARCH_FOR_SMR->SendACQUITT("5537","TARGET_FOUND");
        }*/
       
    }
    catch(TrackerException* e)
    {
        char* msg = "";
        msg = e->getText();
        if (msg != "")
        {
            p_SEARCH_FOR_SMR->SendERROR("5537","ERROR_MOTOR_OFF || NO_TARGET_FOUND");
        }
        delete e;
    }
    ExitThread(0); 
    return 0;
}

DWORD WINAPI MyCritaria_HOME ( LPVOID pParam )
{
    CApplication_Faro_TrakerDlg* p_HOME = (CApplication_Faro_TrakerDlg*)pParam;
    SMRTargetType type_target(p_HOME->GET_SMR_DIAMETRE(p_HOME->m_Smr));
    gpTracker->changeTargetType(&type_target);
    try
    {
        
        if (PARAM_HOME == "1")
        {
            gpTracker->home(true);
        }
        else if (PARAM_HOME == "0")
        {
            gpTracker->home(false);
        }
        else
        {
            p_HOME->SendERROR("5538","2205,2");
        }
       
    }
    catch(TrackerException* e)
    {
        char* msg = "";
        msg = e->getText();
        if (msg != "")
        {
            p_HOME->SendERROR("5537","ERROR_MOTOR_OFF || NO_TARGET_FOUND");
        }
        delete e;
    }
    ExitThread(0); 
    return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////
//DWORD WINAPI MyCritariaBkgndMeasureExterConTrig( LPVOID pParam )
//{	
//	Point* Ptpoint;
//	Ptpoint = new Point();
//	CApplication_Faro_TrakerDlg* pBkgndMeasureCritatiaExterConTrig = (CApplication_Faro_TrakerDlg*)pParam;
//	/////////////////////////////////////////////////////////////////////////////////////
//	MeasurePointData* BkgndMeasurement = NULL;
//	AverageFilter filter;
//	NullStartTrigger startTrigger;
//	ExternalContinueTrigger continueTrigger;
//	/////////////////////////////////////////////////////////////////////////////////////
//	MeasureCfg  cfg( pBkgndMeasureCritatiaExterConTrig->SamPOb , &filter , &startTrigger , &continueTrigger);
//	gpTracker->setBkndMeasureEventRate(1);
//    gpTracker->setBlocking(true);
//    gpTracker->setBkndMeasureBlocking(true);
//	gpTracker->startBkndMeasurePoint( &cfg );
//
//	/////////////////////////////////////////////////////////////////////////////////////
//
//	 try
//	 {
//		 BkgndMeasurement = gpTracker->readBkndMeasurePointData();		
//		 ASSERT(BkgndMeasurement);
//		 LPARAM lParam = (LPARAM)BkgndMeasurement;
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//		 pBkgndMeasureCritatiaExterConTrig->az     =     (BkgndMeasurement->azimuth());
//		 pBkgndMeasureCritatiaExterConTrig->ze     =     (BkgndMeasurement->zenith());    
//		 pBkgndMeasureCritatiaExterConTrig->di     =     (BkgndMeasurement->distance())*1000;
//		
//
//		 pBkgndMeasureCritatiaExterConTrig->x      =      pBkgndMeasureCritatiaExterConTrig->di*sin(pBkgndMeasureCritatiaExterConTrig->ze)*cos(pBkgndMeasureCritatiaExterConTrig->az);
//		 pBkgndMeasureCritatiaExterConTrig->y      =      pBkgndMeasureCritatiaExterConTrig->di*sin(pBkgndMeasureCritatiaExterConTrig->ze)*sin(pBkgndMeasureCritatiaExterConTrig->az);
//		 pBkgndMeasureCritatiaExterConTrig->z      =      pBkgndMeasureCritatiaExterConTrig->di*cos(pBkgndMeasureCritatiaExterConTrig->ze);
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//		 BkgndMeasurement->time();
//		 BkgndMeasurement->status();
//
//		 Ptpoint->X = pBkgndMeasureCritatiaExterConTrig->x;
//		 Ptpoint->Y = pBkgndMeasureCritatiaExterConTrig->y;
//		 Ptpoint->Z = pBkgndMeasureCritatiaExterConTrig->z;
//		 Ptpoint->Cont1_trig = "ExternalContinueTrigger";
//		 Ptpoint->Cont2_trig ="";
//		 Ptpoint->Cont3_trig ="";
//		 Ptpoint->filtter = "Average Filter";
//		 Ptpoint->Measurement_Mechanism = "Background Measurement";
//		 Ptpoint->Measurement_Mode = "Dynamic Measurement";
//		 Ptpoint->samperobs = pBkgndMeasureCritatiaExterConTrig->SamPOb;
//		 Ptpoint->SMR = pBkgndMeasureCritatiaExterConTrig->smr;
//		 Ptpoint->Star_tig = "Null Start Trigger";
//		  GetSystemTime(&Strtime);
//		 
//		 Ptpoint->Time.Format( _T("%04d%02d%02d%02d%02d%02d%03d")
//			 ,Strtime.wYear
//			 ,Strtime.wMonth
//			 ,Strtime.wDay
//			 ,Strtime.wHour
//			 ,Strtime.wMinute
//			 ,Strtime.wSecond
//			 ,Strtime.wMilliseconds);
//
//		 traject.push_back(*Ptpoint);
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//		 pBkgndMeasureCritatiaExterConTrig->m_x.Format("%g", pBkgndMeasureCritatiaExterConTrig->x);
//		 pBkgndMeasureCritatiaExterConTrig->m_y.Format("%g", pBkgndMeasureCritatiaExterConTrig->y);
//		 pBkgndMeasureCritatiaExterConTrig->m_z.Format("%g", pBkgndMeasureCritatiaExterConTrig->z);
//	     
//		 CString Measureb = "x="+pBkgndMeasureCritatiaExterConTrig->m_x+"      y="+pBkgndMeasureCritatiaExterConTrig->m_y+"      z="+pBkgndMeasureCritatiaExterConTrig->m_z;
//
//		 pBkgndMeasureCritatiaExterConTrig->m_listeofmeasur.InsertString(0,Measureb);
//		 pBkgndMeasureCritatiaExterConTrig->m_listeofmeasur.UpdateWindow();
//	 }
//	 catch(TrackerException* e )
//	 {
//		 char* msg;
//		 msg = e->getText();
//		 AfxMessageBox(msg);
//		 delete e;
//	 }
//	 if (imad1 == -1)
//	 {
//		 gpTracker->stopBkndMeasurePoint();
//	 }
//ExitThread(0); 
//return 0;
//}
//DWORD WINAPI MyCritariaBkgndMeasureIntertrig( LPVOID pParam )
//{
//	CApplication_Faro_TrakerDlg* pBkgndMeasureCritatiaIntertrig = (CApplication_Faro_TrakerDlg*)pParam;
//	/////////////////////////////////////////////////////////////////////////////////////
//
//	MeasurePointData* BkgndMeasurement = NULL;
//	AverageFilter  filter;
//	NullStartTrigger  startTrigger;
//	/////////////////////////////////////////////////////////////////////////////////////
//	IntervalTrigger continueTrigger(pBkgndMeasureCritatiaIntertrig->InterTrig);
//	/////////////////////////////////////////////////////////////////////////////////////
//	MeasureCfg  cfg( pBkgndMeasureCritatiaIntertrig->SamPOb , &filter , &startTrigger , &continueTrigger);
//	gpTracker->setBkndMeasureEventRate(1);
//    gpTracker->setBlocking(true);
//    gpTracker->setBkndMeasureBlocking(true);
//	gpTracker->startBkndMeasurePoint( &cfg );
//
//	 while (true)
//	 {
//		 Point* Ptpoint;
//	     Ptpoint = new Point();
//		 try
//		 {
//			 BkgndMeasurement = gpTracker->readBkndMeasurePointData();		
//	         ASSERT(BkgndMeasurement);
//	         LPARAM lParam = (LPARAM)BkgndMeasurement;
//
//			 /////////////////////////////////////////////////////////////////////////////
//
//			 pBkgndMeasureCritatiaIntertrig->az     =     (BkgndMeasurement->azimuth());
//			 pBkgndMeasureCritatiaIntertrig->ze     =     (BkgndMeasurement->zenith());    
//			 pBkgndMeasureCritatiaIntertrig->di     =     (BkgndMeasurement->distance())*1000;
//			
//
//			 pBkgndMeasureCritatiaIntertrig->x      =      pBkgndMeasureCritatiaIntertrig->di*sin(pBkgndMeasureCritatiaIntertrig->ze)*cos(pBkgndMeasureCritatiaIntertrig->az);
//			 pBkgndMeasureCritatiaIntertrig->y      =      pBkgndMeasureCritatiaIntertrig->di*sin(pBkgndMeasureCritatiaIntertrig->ze)*sin(pBkgndMeasureCritatiaIntertrig->az);
//			 pBkgndMeasureCritatiaIntertrig->z      =      pBkgndMeasureCritatiaIntertrig->di*cos(pBkgndMeasureCritatiaIntertrig->ze);
//
//			 /////////////////////////////////////////////////////////////////////////////
//
//			 BkgndMeasurement->time();
//			 BkgndMeasurement->status();
//
//			 Ptpoint->X = pBkgndMeasureCritatiaIntertrig->x;
//			 Ptpoint->Y = pBkgndMeasureCritatiaIntertrig->y;
//			 Ptpoint->Z = pBkgndMeasureCritatiaIntertrig->z;
//			 Ptpoint->Cont1_trig = "IntervalTrigger";
//			 Ptpoint->Cont2_trig = pBkgndMeasureCritatiaIntertrig->m_InterTrig;
//			 Ptpoint->Cont3_trig ="";
//			 Ptpoint->filtter = "Average Filter";
//			 Ptpoint->Measurement_Mechanism = "Background Measurement";
//			 Ptpoint->Measurement_Mode = "Dynamic Measurement";
//			 Ptpoint->samperobs = pBkgndMeasureCritatiaIntertrig->SamPOb;
//			 Ptpoint->SMR = pBkgndMeasureCritatiaIntertrig->smr;
//			 Ptpoint->Star_tig = "Null Start Trigger";
//			  GetSystemTime(&Strtime);
//		 
//			 Ptpoint->Time.Format( _T("%04d%02d%02d%02d%02d%02d%03d")
//				 ,Strtime.wYear
//				 ,Strtime.wMonth
//				 ,Strtime.wDay
//				 ,Strtime.wHour
//				 ,Strtime.wMinute
//				 ,Strtime.wSecond
//				 ,Strtime.wMilliseconds);
//
//			 traject.push_back(*Ptpoint);
//
//			 /////////////////////////////////////////////////////////////////////////////
//
//			 pBkgndMeasureCritatiaIntertrig->m_x.Format("%g", pBkgndMeasureCritatiaIntertrig->x);
//			 pBkgndMeasureCritatiaIntertrig->m_y.Format("%g", pBkgndMeasureCritatiaIntertrig->y);
//			 pBkgndMeasureCritatiaIntertrig->m_z.Format("%g", pBkgndMeasureCritatiaIntertrig->z);
//		     
//			 CString Measureb = "x="+pBkgndMeasureCritatiaIntertrig->m_x+"      y="+pBkgndMeasureCritatiaIntertrig->m_y+"      z="+pBkgndMeasureCritatiaIntertrig->m_z;
//
//			 pBkgndMeasureCritatiaIntertrig->m_listeofmeasur.InsertString(0,Measureb);
//			 pBkgndMeasureCritatiaIntertrig->m_listeofmeasur.UpdateWindow();
//
//		 }
//		 catch(TrackerException* e )
//		 {
//			 char* msg;
//			 msg = e->getText();
//			 AfxMessageBox(msg);
//			 delete e;
//		 }
//		 if (imad2 == -1)
//		 {
//			 gpTracker->stopBkndMeasurePoint();
//			 break;
//		 }
//	 }
//	 ExitThread(0); 
//	 return 0;
//}
//
//DWORD WINAPI MyCritariaBkgndMeasureDistTrig( LPVOID pParam )
//{	
//	CApplication_Faro_TrakerDlg* pBkgndMeasureCritatiaDistTrig = (CApplication_Faro_TrakerDlg*)pParam;
//	/////////////////////////////////////////////////////////////////////////////////////
//
//	MeasurePointData* BkgndMeasurement = NULL;
//	AverageFilter filter;
//	NullStartTrigger startTrigger;
//	/////////////////////////////////////////////////////////////////////////////////////
//	DistanceTrigger continueTrigger(pBkgndMeasureCritatiaDistTrig->DitTrig);
//	/////////////////////////////////////////////////////////////////////////////////////
//	MeasureCfg  cfg( pBkgndMeasureCritatiaDistTrig->SamPOb , &filter , &startTrigger , &continueTrigger);
//	gpTracker->setBkndMeasureEventRate(1);
//    gpTracker->setBlocking(true);
//    gpTracker->setBkndMeasureBlocking(true);
//	gpTracker->startBkndMeasurePoint( &cfg );
//
//	 while (true)
//	 {
//		 Point* Ptpoint;
//	     Ptpoint = new Point();
//		 try
//		 {
//			 BkgndMeasurement = gpTracker->readBkndMeasurePointData();		
//	         ASSERT(BkgndMeasurement);
//	         LPARAM lParam = (LPARAM)BkgndMeasurement;
//
//			 /////////////////////////////////////////////////////////////////////////////
//
//			 pBkgndMeasureCritatiaDistTrig->az     =     (BkgndMeasurement->azimuth());
//			 pBkgndMeasureCritatiaDistTrig->ze     =     (BkgndMeasurement->zenith());    
//			 pBkgndMeasureCritatiaDistTrig->di     =     (BkgndMeasurement->distance())*1000;
//			
//
//			 pBkgndMeasureCritatiaDistTrig->x      =      pBkgndMeasureCritatiaDistTrig->di*sin(pBkgndMeasureCritatiaDistTrig->ze)*cos(pBkgndMeasureCritatiaDistTrig->az);
//			 pBkgndMeasureCritatiaDistTrig->y      =      pBkgndMeasureCritatiaDistTrig->di*sin(pBkgndMeasureCritatiaDistTrig->ze)*sin(pBkgndMeasureCritatiaDistTrig->az);
//			 pBkgndMeasureCritatiaDistTrig->z      =      pBkgndMeasureCritatiaDistTrig->di*cos(pBkgndMeasureCritatiaDistTrig->ze);
//
//			 /////////////////////////////////////////////////////////////////////////////
//
//			 BkgndMeasurement->time();
//			 BkgndMeasurement->status();
//
//			 Ptpoint->X = pBkgndMeasureCritatiaDistTrig->x;
//			 Ptpoint->Y = pBkgndMeasureCritatiaDistTrig->y;
//			 Ptpoint->Z = pBkgndMeasureCritatiaDistTrig->z;
//			 Ptpoint->Cont1_trig = "DistanceTrigger";
//			 Ptpoint->Cont2_trig ="";
//			 Ptpoint->Cont3_trig = pBkgndMeasureCritatiaDistTrig->m_DitTrig;
//			 Ptpoint->filtter = "Average Filter";
//			 Ptpoint->Measurement_Mechanism = "Background Measurement";
//			 Ptpoint->Measurement_Mode = "Dynamic Measurement";
//			 Ptpoint->samperobs = pBkgndMeasureCritatiaDistTrig->SamPOb;
//			 Ptpoint->SMR = pBkgndMeasureCritatiaDistTrig->smr;
//			 Ptpoint->Star_tig = "Null Start Trigger";
//			  GetSystemTime(&Strtime);
//		 
//			 Ptpoint->Time.Format( _T("%04d%02d%02d%02d%02d%02d%03d")
//				 ,Strtime.wYear
//				 ,Strtime.wMonth
//				 ,Strtime.wDay
//				 ,Strtime.wHour
//				 ,Strtime.wMinute
//				 ,Strtime.wSecond
//				 ,Strtime.wMilliseconds);
//
//			 traject.push_back(*Ptpoint);
//
//			 /////////////////////////////////////////////////////////////////////////////
//
//			 pBkgndMeasureCritatiaDistTrig->m_x.Format("%g", pBkgndMeasureCritatiaDistTrig->x);
//			 pBkgndMeasureCritatiaDistTrig->m_y.Format("%g", pBkgndMeasureCritatiaDistTrig->y);
//			 pBkgndMeasureCritatiaDistTrig->m_z.Format("%g", pBkgndMeasureCritatiaDistTrig->z);
//		     
//			 CString Measureb = "x="+pBkgndMeasureCritatiaDistTrig->m_x+"      y="+pBkgndMeasureCritatiaDistTrig->m_y+"      z="+pBkgndMeasureCritatiaDistTrig->m_z;
//
//			 pBkgndMeasureCritatiaDistTrig->m_listeofmeasur.InsertString(0,Measureb);
//			 pBkgndMeasureCritatiaDistTrig->m_listeofmeasur.UpdateWindow();
//
//		 }
//		 catch(TrackerException* e )
//		 {
//			 char* msg;
//			 msg = e->getText();
//			 AfxMessageBox(msg);
//			 delete e;
//		 }
//		  if (imad3 == -1)
//		 {
//			 gpTracker->stopBkndMeasurePoint();
//			 break;
//		 }
//	 }
//ExitThread(0); 
//return 0;
//}
//DWORD WINAPI MyCritariaBkgndMeasureExterConTrig1( LPVOID pParam )
//{	
//	Point* Ptpoint;
//	Ptpoint = new Point();
//	CApplication_Faro_TrakerDlg* pBkgndMeasureCritatiaExterConTrig1 = (CApplication_Faro_TrakerDlg*)pParam;
//	/////////////////////////////////////////////////////////////////////////////////////
//	MeasurePointData* BkgndMeasurement = NULL;
//	StdDevFilter filter;
//	NullStartTrigger startTrigger;
//	ExternalContinueTrigger continueTrigger;
//	/////////////////////////////////////////////////////////////////////////////////////
//	MeasureCfg  cfg( pBkgndMeasureCritatiaExterConTrig1->SamPOb , &filter , &startTrigger , &continueTrigger);
//	gpTracker->setBkndMeasureEventRate(1);
//    gpTracker->setBlocking(true);
//    gpTracker->setBkndMeasureBlocking(true);
//	gpTracker->startBkndMeasurePoint( &cfg );
//
//	/////////////////////////////////////////////////////////////////////////////////////
//
//	 try
//	 {
//		 BkgndMeasurement = gpTracker->readBkndMeasurePointData();		
//		 ASSERT(BkgndMeasurement);
//		 LPARAM lParam = (LPARAM)BkgndMeasurement;
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//		 pBkgndMeasureCritatiaExterConTrig1->az     =     (BkgndMeasurement->azimuth());
//		 pBkgndMeasureCritatiaExterConTrig1->ze     =     (BkgndMeasurement->zenith());    
//		 pBkgndMeasureCritatiaExterConTrig1->di     =     (BkgndMeasurement->distance())*1000;
//		
//
//		 pBkgndMeasureCritatiaExterConTrig1->x      =      pBkgndMeasureCritatiaExterConTrig1->di*sin(pBkgndMeasureCritatiaExterConTrig1->ze)*cos(pBkgndMeasureCritatiaExterConTrig1->az);
//		 pBkgndMeasureCritatiaExterConTrig1->y      =      pBkgndMeasureCritatiaExterConTrig1->di*sin(pBkgndMeasureCritatiaExterConTrig1->ze)*sin(pBkgndMeasureCritatiaExterConTrig1->az);
//		 pBkgndMeasureCritatiaExterConTrig1->z      =      pBkgndMeasureCritatiaExterConTrig1->di*cos(pBkgndMeasureCritatiaExterConTrig1->ze);
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//		 BkgndMeasurement->time();
//		 BkgndMeasurement->status();
//
//		 Ptpoint->X = pBkgndMeasureCritatiaExterConTrig1->x;
//		 Ptpoint->Y = pBkgndMeasureCritatiaExterConTrig1->y;
//		 Ptpoint->Z = pBkgndMeasureCritatiaExterConTrig1->z;
//		 Ptpoint->Cont1_trig = "ExternalContinueTrigger";
//		 Ptpoint->Cont2_trig ="";
//		 Ptpoint->Cont3_trig ="";
//		 Ptpoint->filtter = "StdDevFilter";
//		 Ptpoint->Measurement_Mechanism = "Background Measurement";
//		 Ptpoint->Measurement_Mode = "Dynamic Measurement";
//		 Ptpoint->samperobs = pBkgndMeasureCritatiaExterConTrig1->SamPOb;
//		 Ptpoint->SMR = pBkgndMeasureCritatiaExterConTrig1->smr;
//		 Ptpoint->Star_tig = "Null Start Trigger";
//		  GetSystemTime(&Strtime);
//		 
//		 Ptpoint->Time.Format( _T("%04d%02d%02d%02d%02d%02d%03d")
//			 ,Strtime.wYear
//			 ,Strtime.wMonth
//			 ,Strtime.wDay
//			 ,Strtime.wHour
//			 ,Strtime.wMinute
//			 ,Strtime.wSecond
//			 ,Strtime.wMilliseconds);
//
//		 traject.push_back(*Ptpoint);
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//		 pBkgndMeasureCritatiaExterConTrig1->m_x.Format("%g", pBkgndMeasureCritatiaExterConTrig1->x);
//		 pBkgndMeasureCritatiaExterConTrig1->m_y.Format("%g", pBkgndMeasureCritatiaExterConTrig1->y);
//		 pBkgndMeasureCritatiaExterConTrig1->m_z.Format("%g", pBkgndMeasureCritatiaExterConTrig1->z);
//	     
//		 CString Measureb = "x="+pBkgndMeasureCritatiaExterConTrig1->m_x+"      y="+pBkgndMeasureCritatiaExterConTrig1->m_y+"      z="+pBkgndMeasureCritatiaExterConTrig1->m_z;
//
//		 pBkgndMeasureCritatiaExterConTrig1->m_listeofmeasur.InsertString(0,Measureb);
//		 pBkgndMeasureCritatiaExterConTrig1->m_listeofmeasur.UpdateWindow();
//	 }
//	 catch(TrackerException* e )
//	 {
//		 char* msg;
//		 msg = e->getText();
//		 AfxMessageBox(msg);
//		 delete e;
//	 }
//	 if (imad4 == -1)
//	 {
//		 gpTracker->stopBkndMeasurePoint();
//	 }
//ExitThread(0); 
//return 0;
//}
//DWORD WINAPI MyCritariaBkgndMeasureIntertrig1( LPVOID pParam )
//{	
//	CApplication_Faro_TrakerDlg* pBkgndMeasureCritatiaIntertrig1 = (CApplication_Faro_TrakerDlg*)pParam;
//	/////////////////////////////////////////////////////////////////////////////////////
//
//	MeasurePointData* BkgndMeasurement = NULL;
//	StdDevFilter filter;
//	NullStartTrigger startTrigger;
//	/////////////////////////////////////////////////////////////////////////////////////
//	IntervalTrigger continueTrigger(pBkgndMeasureCritatiaIntertrig1->InterTrig);
//	/////////////////////////////////////////////////////////////////////////////////////
//	MeasureCfg  cfg( pBkgndMeasureCritatiaIntertrig1->SamPOb , &filter , &startTrigger , &continueTrigger);
//	gpTracker->setBkndMeasureEventRate(1);
//    gpTracker->setBlocking(true);
//    gpTracker->setBkndMeasureBlocking(true);
//	gpTracker->startBkndMeasurePoint( &cfg );
//
//	 while (true)
//	 {
//		 Point* Ptpoint;
//	     Ptpoint = new Point();
//		 try
//		 {
//			 BkgndMeasurement = gpTracker->readBkndMeasurePointData();		
//	         ASSERT(BkgndMeasurement);
//	         LPARAM lParam = (LPARAM)BkgndMeasurement;
//
//			 /////////////////////////////////////////////////////////////////////////////
//
//			 pBkgndMeasureCritatiaIntertrig1->az     =     (BkgndMeasurement->azimuth());
//			 pBkgndMeasureCritatiaIntertrig1->ze     =     (BkgndMeasurement->zenith());    
//			 pBkgndMeasureCritatiaIntertrig1->di     =     (BkgndMeasurement->distance())*1000;
//			
//
//			 pBkgndMeasureCritatiaIntertrig1->x      =      pBkgndMeasureCritatiaIntertrig1->di*sin(pBkgndMeasureCritatiaIntertrig1->ze)*cos(pBkgndMeasureCritatiaIntertrig1->az);
//			 pBkgndMeasureCritatiaIntertrig1->y      =      pBkgndMeasureCritatiaIntertrig1->di*sin(pBkgndMeasureCritatiaIntertrig1->ze)*sin(pBkgndMeasureCritatiaIntertrig1->az);
//			 pBkgndMeasureCritatiaIntertrig1->z      =      pBkgndMeasureCritatiaIntertrig1->di*cos(pBkgndMeasureCritatiaIntertrig1->ze);
//
//			 /////////////////////////////////////////////////////////////////////////////
//
//			 BkgndMeasurement->time();
//			 BkgndMeasurement->status();
//
//			 Ptpoint->X = pBkgndMeasureCritatiaIntertrig1->x;
//			 Ptpoint->Y = pBkgndMeasureCritatiaIntertrig1->y;
//			 Ptpoint->Z = pBkgndMeasureCritatiaIntertrig1->z;
//			 Ptpoint->Cont1_trig = "IntervalTrigger";
//			 Ptpoint->Cont2_trig = pBkgndMeasureCritatiaIntertrig1->m_InterTrig;
//			 Ptpoint->Cont3_trig ="";
//			 Ptpoint->filtter = "StdDevFilter";
//			 Ptpoint->Measurement_Mechanism = "Background Measurement";
//			 Ptpoint->Measurement_Mode = "Dynamic Measurement";
//			 Ptpoint->samperobs = pBkgndMeasureCritatiaIntertrig1->SamPOb;
//			 Ptpoint->SMR = pBkgndMeasureCritatiaIntertrig1->smr;
//			 Ptpoint->Star_tig = "Null Start Trigger";
//			  GetSystemTime(&Strtime);
//		 
//			 Ptpoint->Time.Format( _T("%04d%02d%02d%02d%02d%02d%03d")
//				 ,Strtime.wYear
//				 ,Strtime.wMonth
//				 ,Strtime.wDay
//				 ,Strtime.wHour
//				 ,Strtime.wMinute
//				 ,Strtime.wSecond
//				 ,Strtime.wMilliseconds);
//
//			 traject.push_back(*Ptpoint);
//
//			 /////////////////////////////////////////////////////////////////////////////
//
//			 pBkgndMeasureCritatiaIntertrig1->m_x.Format("%g", pBkgndMeasureCritatiaIntertrig1->x);
//			 pBkgndMeasureCritatiaIntertrig1->m_y.Format("%g", pBkgndMeasureCritatiaIntertrig1->y);
//			 pBkgndMeasureCritatiaIntertrig1->m_z.Format("%g", pBkgndMeasureCritatiaIntertrig1->z);
//		     
//			 CString Measureb = "x="+pBkgndMeasureCritatiaIntertrig1->m_x+"      y="+pBkgndMeasureCritatiaIntertrig1->m_y+"      z="+pBkgndMeasureCritatiaIntertrig1->m_z;
//
//			 pBkgndMeasureCritatiaIntertrig1->m_listeofmeasur.InsertString(0,Measureb);
//			 pBkgndMeasureCritatiaIntertrig1->m_listeofmeasur.UpdateWindow();
//
//		 }
//		 catch(TrackerException* e )
//		 {
//			 char* msg;
//			 msg = e->getText();
//			 AfxMessageBox(msg);
//			 delete e;
//		 }
//		 if (imad5 == -1)
//		 {
//			 gpTracker->stopBkndMeasurePoint();
//			 break;
//		 }
//	 }
//	 ExitThread(0); 
//	 return 0;
//}
//DWORD WINAPI MyCritariaBkgndMeasureDistTrig1( LPVOID pParam )
//{	
//	CApplication_Faro_TrakerDlg* pBkgndMeasureCritatiaDistTrig1 = (CApplication_Faro_TrakerDlg*)pParam;
//	/////////////////////////////////////////////////////////////////////////////////////
//
//	MeasurePointData* BkgndMeasurement = NULL;
//	StdDevFilter filter;
//	NullStartTrigger startTrigger;
//	/////////////////////////////////////////////////////////////////////////////////////
//	DistanceTrigger continueTrigger(pBkgndMeasureCritatiaDistTrig1->DitTrig);
//	/////////////////////////////////////////////////////////////////////////////////////
//	MeasureCfg  cfg( pBkgndMeasureCritatiaDistTrig1->SamPOb , &filter , &startTrigger , &continueTrigger);
//	gpTracker->setBkndMeasureEventRate(1);
//    gpTracker->setBlocking(true);
//    gpTracker->setBkndMeasureBlocking(true);
//	gpTracker->startBkndMeasurePoint( &cfg );
//
//	 while (true)
//	 {
//		 Point* Ptpoint;
//	     Ptpoint = new Point();
//		 try
//		 {
//			 BkgndMeasurement = gpTracker->readBkndMeasurePointData();		
//	         ASSERT(BkgndMeasurement);
//	         LPARAM lParam = (LPARAM)BkgndMeasurement;
//
//			 /////////////////////////////////////////////////////////////////////////////
//
//			 pBkgndMeasureCritatiaDistTrig1->az     =     (BkgndMeasurement->azimuth());
//			 pBkgndMeasureCritatiaDistTrig1->ze     =     (BkgndMeasurement->zenith());    
//			 pBkgndMeasureCritatiaDistTrig1->di     =     (BkgndMeasurement->distance())*1000;
//			
//
//			 pBkgndMeasureCritatiaDistTrig1->x      =      pBkgndMeasureCritatiaDistTrig1->di*sin(pBkgndMeasureCritatiaDistTrig1->ze)*cos(pBkgndMeasureCritatiaDistTrig1->az);
//			 pBkgndMeasureCritatiaDistTrig1->y      =      pBkgndMeasureCritatiaDistTrig1->di*sin(pBkgndMeasureCritatiaDistTrig1->ze)*sin(pBkgndMeasureCritatiaDistTrig1->az);
//			 pBkgndMeasureCritatiaDistTrig1->z      =      pBkgndMeasureCritatiaDistTrig1->di*cos(pBkgndMeasureCritatiaDistTrig1->ze);
//
//			 /////////////////////////////////////////////////////////////////////////////
//
//			 BkgndMeasurement->time();
//			 BkgndMeasurement->status();
//
//			 Ptpoint->X = pBkgndMeasureCritatiaDistTrig1->x;
//			 Ptpoint->Y = pBkgndMeasureCritatiaDistTrig1->y;
//			 Ptpoint->Z = pBkgndMeasureCritatiaDistTrig1->z;
//			 Ptpoint->Cont1_trig = "DistanceTrigger";
//			 Ptpoint->Cont2_trig ="";
//			 Ptpoint->Cont3_trig = pBkgndMeasureCritatiaDistTrig1->m_DitTrig;
//			 Ptpoint->filtter = "StdDevFilter";
//			 Ptpoint->Measurement_Mechanism = "Background Measurement";
//			 Ptpoint->Measurement_Mode = "Dynamic Measurement";
//			 Ptpoint->samperobs = pBkgndMeasureCritatiaDistTrig1->SamPOb;
//			 Ptpoint->SMR = pBkgndMeasureCritatiaDistTrig1->smr;
//			 Ptpoint->Star_tig = "Null Start Trigger";
//			  GetSystemTime(&Strtime);
//		 
//			 Ptpoint->Time.Format( _T("%04d%02d%02d%02d%02d%02d%03d")
//				 ,Strtime.wYear
//				 ,Strtime.wMonth
//				 ,Strtime.wDay
//				 ,Strtime.wHour
//				 ,Strtime.wMinute
//				 ,Strtime.wSecond
//				 ,Strtime.wMilliseconds);
//
//			 traject.push_back(*Ptpoint);
//
//			 /////////////////////////////////////////////////////////////////////////////
//
//			 pBkgndMeasureCritatiaDistTrig1->m_x.Format("%g", pBkgndMeasureCritatiaDistTrig1->x);
//			 pBkgndMeasureCritatiaDistTrig1->m_y.Format("%g", pBkgndMeasureCritatiaDistTrig1->y);
//			 pBkgndMeasureCritatiaDistTrig1->m_z.Format("%g", pBkgndMeasureCritatiaDistTrig1->z);
//		     
//			 CString Measureb = "x="+pBkgndMeasureCritatiaDistTrig1->m_x+"      y="+pBkgndMeasureCritatiaDistTrig1->m_y+"      z="+pBkgndMeasureCritatiaDistTrig1->m_z;
//
//			 pBkgndMeasureCritatiaDistTrig1->m_listeofmeasur.InsertString(0,Measureb);
//			 pBkgndMeasureCritatiaDistTrig1->m_listeofmeasur.UpdateWindow();
//
//		 }
//		 catch(TrackerException* e )
//		 {
//			 char* msg;
//			 msg = e->getText();
//			 AfxMessageBox(msg);
//			 delete e;
//		 }
//		  if (imad6 == -1)
//		 {
//			 gpTracker->stopBkndMeasurePoint();
//			 break;
//		 }
//	 }
//ExitThread(0); 
//return 0;
//}
//DWORD WINAPI MyCritariaBkgndMeasureExterConTrig2( LPVOID pParam )
//{
//	Point* Ptpoint;
//	Ptpoint = new Point();
//	CApplication_Faro_TrakerDlg* pBkgndMeasureCritatiaExterConTrig2 = (CApplication_Faro_TrakerDlg*)pParam;
//	/////////////////////////////////////////////////////////////////////////////////////
//	MeasurePointData* BkgndMeasurement = NULL;
//	AverageFilter filter;
//	ExternalStartTrigger startTrigger;
//	ExternalContinueTrigger continueTrigger;
//	/////////////////////////////////////////////////////////////////////////////////////
//	MeasureCfg  cfg( pBkgndMeasureCritatiaExterConTrig2->SamPOb , &filter , &startTrigger , &continueTrigger);
//	gpTracker->setBkndMeasureEventRate(1);
//    gpTracker->setBlocking(true);
//    gpTracker->setBkndMeasureBlocking(true);
//	gpTracker->startBkndMeasurePoint( &cfg );
//
//	/////////////////////////////////////////////////////////////////////////////////////
//
//	 try
//	 {
//		 BkgndMeasurement = gpTracker->readBkndMeasurePointData();		
//		 ASSERT(BkgndMeasurement);
//		 LPARAM lParam = (LPARAM)BkgndMeasurement;
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//		 pBkgndMeasureCritatiaExterConTrig2->az     =     (BkgndMeasurement->azimuth());
//		 pBkgndMeasureCritatiaExterConTrig2->ze     =     (BkgndMeasurement->zenith());    
//		 pBkgndMeasureCritatiaExterConTrig2->di     =     (BkgndMeasurement->distance())*1000;
//		
//
//		 pBkgndMeasureCritatiaExterConTrig2->x      =      pBkgndMeasureCritatiaExterConTrig2->di*sin(pBkgndMeasureCritatiaExterConTrig2->ze)*cos(pBkgndMeasureCritatiaExterConTrig2->az);
//		 pBkgndMeasureCritatiaExterConTrig2->y      =      pBkgndMeasureCritatiaExterConTrig2->di*sin(pBkgndMeasureCritatiaExterConTrig2->ze)*sin(pBkgndMeasureCritatiaExterConTrig2->az);
//		 pBkgndMeasureCritatiaExterConTrig2->z      =      pBkgndMeasureCritatiaExterConTrig2->di*cos(pBkgndMeasureCritatiaExterConTrig2->ze);
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//		 BkgndMeasurement->time();
//		 BkgndMeasurement->status();
//
//		 Ptpoint->X = pBkgndMeasureCritatiaExterConTrig2->x;
//		 Ptpoint->Y = pBkgndMeasureCritatiaExterConTrig2->y;
//		 Ptpoint->Z = pBkgndMeasureCritatiaExterConTrig2->z;
//		 Ptpoint->Cont1_trig = "External Continue Trigger";
//		 Ptpoint->Cont2_trig ="";
//		 Ptpoint->Cont3_trig ="";
//		 Ptpoint->filtter = "Average Filter";
//		 Ptpoint->Measurement_Mechanism = "Background Measurement";
//		 Ptpoint->Measurement_Mode = "Dynamic Measurement";
//		 Ptpoint->samperobs = pBkgndMeasureCritatiaExterConTrig2->SamPOb;
//		 Ptpoint->SMR = pBkgndMeasureCritatiaExterConTrig2->smr;
//		 Ptpoint->Star_tig = "External Start Trigger";
//		  GetSystemTime(&Strtime);
//		 
//		 Ptpoint->Time.Format( _T("%04d%02d%02d%02d%02d%02d%03d")
//			 ,Strtime.wYear
//			 ,Strtime.wMonth
//			 ,Strtime.wDay
//			 ,Strtime.wHour
//			 ,Strtime.wMinute
//			 ,Strtime.wSecond
//			 ,Strtime.wMilliseconds);
//
//		 traject.push_back(*Ptpoint);
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//		 pBkgndMeasureCritatiaExterConTrig2->m_x.Format("%g", pBkgndMeasureCritatiaExterConTrig2->x);
//		 pBkgndMeasureCritatiaExterConTrig2->m_y.Format("%g", pBkgndMeasureCritatiaExterConTrig2->y);
//		 pBkgndMeasureCritatiaExterConTrig2->m_z.Format("%g", pBkgndMeasureCritatiaExterConTrig2->z);
//	     
//		 CString Measureb = "x="+pBkgndMeasureCritatiaExterConTrig2->m_x+"      y="+pBkgndMeasureCritatiaExterConTrig2->m_y+"      z="+pBkgndMeasureCritatiaExterConTrig2->m_z;
//
//		 pBkgndMeasureCritatiaExterConTrig2->m_listeofmeasur.InsertString(0,Measureb);
//		 pBkgndMeasureCritatiaExterConTrig2->m_listeofmeasur.UpdateWindow();
//	 }
//	 catch(TrackerException* e )
//	 {
//		 char* msg;
//		 msg = e->getText();
//		 AfxMessageBox(msg);
//		 delete e;
//	 }
//	 if (imad12 == -1)
//	 {
//		 gpTracker->stopBkndMeasurePoint();
//	 }
//ExitThread(0); 
//return 0;
//}
//DWORD WINAPI MyCritariaBkgndMeasureIntertrig2( LPVOID pParam )
//{
//	CApplication_Faro_TrakerDlg* pBkgndMeasureCritatiaIntertrig2 = (CApplication_Faro_TrakerDlg*)pParam;
//	/////////////////////////////////////////////////////////////////////////////////////
//
//	MeasurePointData* BkgndMeasurement = NULL;
//	AverageFilter filter;
//	ExternalStartTrigger startTrigger;
//	/////////////////////////////////////////////////////////////////////////////////////
//	IntervalTrigger continueTrigger(pBkgndMeasureCritatiaIntertrig2->InterTrig);
//	/////////////////////////////////////////////////////////////////////////////////////
//	MeasureCfg  cfg( pBkgndMeasureCritatiaIntertrig2->SamPOb , &filter , &startTrigger , &continueTrigger);
//	gpTracker->setBkndMeasureEventRate(1);
//    gpTracker->setBlocking(true);
//    gpTracker->setBkndMeasureBlocking(true);
//	gpTracker->startBkndMeasurePoint( &cfg );
//
//	 while (true)
//	 {
//		 Point* Ptpoint;
//	     Ptpoint = new Point();
//		 try
//		 {
//			 BkgndMeasurement = gpTracker->readBkndMeasurePointData();		
//	         ASSERT(BkgndMeasurement);
//	         LPARAM lParam = (LPARAM)BkgndMeasurement;
//
//			 /////////////////////////////////////////////////////////////////////////////
//
//			 pBkgndMeasureCritatiaIntertrig2->az     =     (BkgndMeasurement->azimuth());
//			 pBkgndMeasureCritatiaIntertrig2->ze     =     (BkgndMeasurement->zenith());    
//			 pBkgndMeasureCritatiaIntertrig2->di     =     (BkgndMeasurement->distance())*1000;
//			
//
//			 pBkgndMeasureCritatiaIntertrig2->x      =      pBkgndMeasureCritatiaIntertrig2->di*sin(pBkgndMeasureCritatiaIntertrig2->ze)*cos(pBkgndMeasureCritatiaIntertrig2->az);
//			 pBkgndMeasureCritatiaIntertrig2->y      =      pBkgndMeasureCritatiaIntertrig2->di*sin(pBkgndMeasureCritatiaIntertrig2->ze)*sin(pBkgndMeasureCritatiaIntertrig2->az);
//			 pBkgndMeasureCritatiaIntertrig2->z      =      pBkgndMeasureCritatiaIntertrig2->di*cos(pBkgndMeasureCritatiaIntertrig2->ze);
//
//			 /////////////////////////////////////////////////////////////////////////////
//
//			 BkgndMeasurement->time();
//			 BkgndMeasurement->status();
//
//			 Ptpoint->X = pBkgndMeasureCritatiaIntertrig2->x;
//			 Ptpoint->Y = pBkgndMeasureCritatiaIntertrig2->y;
//			 Ptpoint->Z = pBkgndMeasureCritatiaIntertrig2->z;
//			 Ptpoint->Cont1_trig = "IntervalTrigger";
//			 Ptpoint->Cont2_trig = pBkgndMeasureCritatiaIntertrig2->m_InterTrig;
//			 Ptpoint->Cont3_trig ="";
//			 Ptpoint->filtter = "Average Filter";
//			 Ptpoint->Measurement_Mechanism = "Background Measurement";
//			 Ptpoint->Measurement_Mode = "Dynamic Measurement";
//			 Ptpoint->samperobs = pBkgndMeasureCritatiaIntertrig2->SamPOb;
//			 Ptpoint->SMR = pBkgndMeasureCritatiaIntertrig2->smr;
//			 Ptpoint->Star_tig = "External Start Trigger";
//			  GetSystemTime(&Strtime);
//		 
//			 Ptpoint->Time.Format( _T("%04d%02d%02d%02d%02d%02d%03d")
//				 ,Strtime.wYear
//				 ,Strtime.wMonth
//				 ,Strtime.wDay
//				 ,Strtime.wHour
//				 ,Strtime.wMinute
//				 ,Strtime.wSecond
//				 ,Strtime.wMilliseconds);
//
//			 traject.push_back(*Ptpoint);
//
//			 /////////////////////////////////////////////////////////////////////////////
//
//			 pBkgndMeasureCritatiaIntertrig2->m_x.Format("%g", pBkgndMeasureCritatiaIntertrig2->x);
//			 pBkgndMeasureCritatiaIntertrig2->m_y.Format("%g", pBkgndMeasureCritatiaIntertrig2->y);
//			 pBkgndMeasureCritatiaIntertrig2->m_z.Format("%g", pBkgndMeasureCritatiaIntertrig2->z);
//		     
//			 CString Measureb = "x="+pBkgndMeasureCritatiaIntertrig2->m_x+"      y="+pBkgndMeasureCritatiaIntertrig2->m_y+"      z="+pBkgndMeasureCritatiaIntertrig2->m_z;
//
//			 pBkgndMeasureCritatiaIntertrig2->m_listeofmeasur.InsertString(0,Measureb);
//			 pBkgndMeasureCritatiaIntertrig2->m_listeofmeasur.UpdateWindow();
//
//		 }
//		 catch(TrackerException* e )
//		 {
//			 char* msg;
//			 msg = e->getText();
//			 AfxMessageBox(msg);
//			 delete e;
//		 }
//		 if (imad22 == -1)
//		 {
//			 gpTracker->stopBkndMeasurePoint();
//			 break;
//		 }
//	 }
//	 ExitThread(0); 
//	 return 0;
//}
//
//DWORD WINAPI MyCritariaBkgndMeasureDistTrig2( LPVOID pParam )
//{	
//	CApplication_Faro_TrakerDlg* pBkgndMeasureCritatiaDistTrig2 = (CApplication_Faro_TrakerDlg*)pParam;
//	/////////////////////////////////////////////////////////////////////////////////////
//
//	MeasurePointData* BkgndMeasurement = NULL;
//	AverageFilter filter;
//	ExternalStartTrigger startTrigger;
//	/////////////////////////////////////////////////////////////////////////////////////
//	DistanceTrigger continueTrigger(pBkgndMeasureCritatiaDistTrig2->DitTrig);
//	/////////////////////////////////////////////////////////////////////////////////////
//	MeasureCfg  cfg( pBkgndMeasureCritatiaDistTrig2->SamPOb , &filter , &startTrigger , &continueTrigger);
//	gpTracker->setBkndMeasureEventRate(1);
//    gpTracker->setBlocking(true);
//    gpTracker->setBkndMeasureBlocking(true);
//	gpTracker->startBkndMeasurePoint( &cfg );
//
//	 while (true)
//	 {
//		 Point* Ptpoint;
//	     Ptpoint = new Point();
//		 try
//		 {
//			 BkgndMeasurement = gpTracker->readBkndMeasurePointData();		
//	         ASSERT(BkgndMeasurement);
//	         LPARAM lParam = (LPARAM)BkgndMeasurement;
//
//			 /////////////////////////////////////////////////////////////////////////////
//
//			 pBkgndMeasureCritatiaDistTrig2->az     =     (BkgndMeasurement->azimuth());
//			 pBkgndMeasureCritatiaDistTrig2->ze     =     (BkgndMeasurement->zenith());    
//			 pBkgndMeasureCritatiaDistTrig2->di     =     (BkgndMeasurement->distance())*1000;
//			
//
//			 pBkgndMeasureCritatiaDistTrig2->x      =      pBkgndMeasureCritatiaDistTrig2->di*sin(pBkgndMeasureCritatiaDistTrig2->ze)*cos(pBkgndMeasureCritatiaDistTrig2->az);
//			 pBkgndMeasureCritatiaDistTrig2->y      =      pBkgndMeasureCritatiaDistTrig2->di*sin(pBkgndMeasureCritatiaDistTrig2->ze)*sin(pBkgndMeasureCritatiaDistTrig2->az);
//			 pBkgndMeasureCritatiaDistTrig2->z      =      pBkgndMeasureCritatiaDistTrig2->di*cos(pBkgndMeasureCritatiaDistTrig2->ze);
//
//			 /////////////////////////////////////////////////////////////////////////////
//
//			 BkgndMeasurement->time();
//			 BkgndMeasurement->status();
//
//			 Ptpoint->X = pBkgndMeasureCritatiaDistTrig2->x;
//			 Ptpoint->Y = pBkgndMeasureCritatiaDistTrig2->y;
//			 Ptpoint->Z = pBkgndMeasureCritatiaDistTrig2->z;
//			 Ptpoint->Cont1_trig = "DistanceTrigger";
//			 Ptpoint->Cont2_trig ="";
//			 Ptpoint->Cont3_trig = pBkgndMeasureCritatiaDistTrig2->m_DitTrig;
//			 Ptpoint->filtter = "Average Filter";
//			 Ptpoint->Measurement_Mechanism = "Background Measurement";
//			 Ptpoint->Measurement_Mode = "Dynamic Measurement";
//			 Ptpoint->samperobs = pBkgndMeasureCritatiaDistTrig2->SamPOb;
//			 Ptpoint->SMR = pBkgndMeasureCritatiaDistTrig2->smr;
//			 Ptpoint->Star_tig = "External Start Trigger";
//			  GetSystemTime(&Strtime);
//		 
//			 Ptpoint->Time.Format( _T("%04d%02d%02d%02d%02d%02d%03d")
//				 ,Strtime.wYear
//				 ,Strtime.wMonth
//				 ,Strtime.wDay
//				 ,Strtime.wHour
//				 ,Strtime.wMinute
//				 ,Strtime.wSecond
//				 ,Strtime.wMilliseconds);
//
//			 traject.push_back(*Ptpoint);
//
//			 /////////////////////////////////////////////////////////////////////////////
//
//			 pBkgndMeasureCritatiaDistTrig2->m_x.Format("%g", pBkgndMeasureCritatiaDistTrig2->x);
//			 pBkgndMeasureCritatiaDistTrig2->m_y.Format("%g", pBkgndMeasureCritatiaDistTrig2->y);
//			 pBkgndMeasureCritatiaDistTrig2->m_z.Format("%g", pBkgndMeasureCritatiaDistTrig2->z);
//		     
//			 CString Measureb = "x="+pBkgndMeasureCritatiaDistTrig2->m_x+"      y="+pBkgndMeasureCritatiaDistTrig2->m_y+"      z="+pBkgndMeasureCritatiaDistTrig2->m_z;
//
//			 pBkgndMeasureCritatiaDistTrig2->m_listeofmeasur.InsertString(0,Measureb);
//			 pBkgndMeasureCritatiaDistTrig2->m_listeofmeasur.UpdateWindow();
//
//		 }
//		 catch(TrackerException* e )
//		 {
//			 char* msg;
//			 msg = e->getText();
//			 AfxMessageBox(msg);
//			 delete e;
//		 }
//		  if (imad32 == -1)
//		 {
//			 gpTracker->stopBkndMeasurePoint();
//			 break;
//		 }
//	 }
//ExitThread(0); 
//return 0;
//}
//DWORD WINAPI MyCritariaBkgndMeasureExterConTrig12( LPVOID pParam )
//{	
//	Point* Ptpoint;
//	Ptpoint = new Point();
//	CApplication_Faro_TrakerDlg* pBkgndMeasureCritatiaExterConTrig12 = (CApplication_Faro_TrakerDlg*)pParam;
//	/////////////////////////////////////////////////////////////////////////////////////
//	MeasurePointData* BkgndMeasurement = NULL;
//	StdDevFilter filter;
//	ExternalStartTrigger startTrigger;
//	ExternalContinueTrigger continueTrigger;
//	/////////////////////////////////////////////////////////////////////////////////////
//	MeasureCfg  cfg( pBkgndMeasureCritatiaExterConTrig12->SamPOb , &filter , &startTrigger , &continueTrigger);
//	gpTracker->setBkndMeasureEventRate(1);
//    gpTracker->setBlocking(true);
//    gpTracker->setBkndMeasureBlocking(true);
//	gpTracker->startBkndMeasurePoint( &cfg );
//
//	/////////////////////////////////////////////////////////////////////////////////////
//
//	 try
//	 {
//		 BkgndMeasurement = gpTracker->readBkndMeasurePointData();		
//		 ASSERT(BkgndMeasurement);
//		 LPARAM lParam = (LPARAM)BkgndMeasurement;
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//		 pBkgndMeasureCritatiaExterConTrig12->az     =     (BkgndMeasurement->azimuth());
//		 pBkgndMeasureCritatiaExterConTrig12->ze     =     (BkgndMeasurement->zenith());    
//		 pBkgndMeasureCritatiaExterConTrig12->di     =     (BkgndMeasurement->distance())*1000;
//		
//
//		 pBkgndMeasureCritatiaExterConTrig12->x      =      pBkgndMeasureCritatiaExterConTrig12->di*sin(pBkgndMeasureCritatiaExterConTrig12->ze)*cos(pBkgndMeasureCritatiaExterConTrig12->az);
//		 pBkgndMeasureCritatiaExterConTrig12->y      =      pBkgndMeasureCritatiaExterConTrig12->di*sin(pBkgndMeasureCritatiaExterConTrig12->ze)*sin(pBkgndMeasureCritatiaExterConTrig12->az);
//		 pBkgndMeasureCritatiaExterConTrig12->z      =      pBkgndMeasureCritatiaExterConTrig12->di*cos(pBkgndMeasureCritatiaExterConTrig12->ze);
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//		 BkgndMeasurement->time();
//		 BkgndMeasurement->status();
//
//		 Ptpoint->X = pBkgndMeasureCritatiaExterConTrig12->x;
//		 Ptpoint->Y = pBkgndMeasureCritatiaExterConTrig12->y;
//		 Ptpoint->Z = pBkgndMeasureCritatiaExterConTrig12->z;
//		 Ptpoint->Cont1_trig = "External Continue Trigger";
//		 Ptpoint->Cont2_trig ="";
//		 Ptpoint->Cont3_trig ="";
//		 Ptpoint->filtter = "StdDevFilter";
//		 Ptpoint->Measurement_Mechanism = "Background Measurement";
//		 Ptpoint->Measurement_Mode = "Dynamic Measurement";
//		 Ptpoint->samperobs = pBkgndMeasureCritatiaExterConTrig12->SamPOb;
//		 Ptpoint->SMR = pBkgndMeasureCritatiaExterConTrig12->smr;
//		 Ptpoint->Star_tig = "External Start Trigger";
//		  GetSystemTime(&Strtime);
//		 
//		 Ptpoint->Time.Format( _T("%04d%02d%02d%02d%02d%02d%03d")
//			 ,Strtime.wYear
//			 ,Strtime.wMonth
//			 ,Strtime.wDay
//			 ,Strtime.wHour
//			 ,Strtime.wMinute
//			 ,Strtime.wSecond
//			 ,Strtime.wMilliseconds);
//
//		 traject.push_back(*Ptpoint);
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//		 pBkgndMeasureCritatiaExterConTrig12->m_x.Format("%g", pBkgndMeasureCritatiaExterConTrig12->x);
//		 pBkgndMeasureCritatiaExterConTrig12->m_y.Format("%g", pBkgndMeasureCritatiaExterConTrig12->y);
//		 pBkgndMeasureCritatiaExterConTrig12->m_z.Format("%g", pBkgndMeasureCritatiaExterConTrig12->z);
//	     
//		 CString Measureb = "x="+pBkgndMeasureCritatiaExterConTrig12->m_x+"      y="+pBkgndMeasureCritatiaExterConTrig12->m_y+"      z="+pBkgndMeasureCritatiaExterConTrig12->m_z;
//
//		 pBkgndMeasureCritatiaExterConTrig12->m_listeofmeasur.InsertString(0,Measureb);
//		 pBkgndMeasureCritatiaExterConTrig12->m_listeofmeasur.UpdateWindow();
//	 }
//	 catch(TrackerException* e )
//	 {
//		 char* msg;
//		 msg = e->getText();
//		 AfxMessageBox(msg);
//		 delete e;
//	 }
//	 if (imad42 == -1)
//	 {
//		 gpTracker->stopBkndMeasurePoint();
//	 }
//ExitThread(0); 
//return 0;
//}
//DWORD WINAPI MyCritariaBkgndMeasureIntertrig12( LPVOID pParam )
//{
//	
//	CApplication_Faro_TrakerDlg* pBkgndMeasureCritatiaIntertrig12 = (CApplication_Faro_TrakerDlg*)pParam;
//	/////////////////////////////////////////////////////////////////////////////////////
//
//	MeasurePointData* BkgndMeasurement = NULL;
//	StdDevFilter filter;
//	ExternalStartTrigger startTrigger;
//	/////////////////////////////////////////////////////////////////////////////////////
//	IntervalTrigger continueTrigger(pBkgndMeasureCritatiaIntertrig12->InterTrig);
//	/////////////////////////////////////////////////////////////////////////////////////
//	MeasureCfg  cfg( pBkgndMeasureCritatiaIntertrig12->SamPOb , &filter , &startTrigger , &continueTrigger);
//	gpTracker->setBkndMeasureEventRate(1);
//    gpTracker->setBlocking(true);
//    gpTracker->setBkndMeasureBlocking(true);
//	gpTracker->startBkndMeasurePoint( &cfg );
//
//	 while (true)
//	 {
//		 Point* Ptpoint;
//	     Ptpoint = new Point();
//		 try
//		 {
//			 BkgndMeasurement = gpTracker->readBkndMeasurePointData();		
//	         ASSERT(BkgndMeasurement);
//	         LPARAM lParam = (LPARAM)BkgndMeasurement;
//
//			 /////////////////////////////////////////////////////////////////////////////
//
//			 pBkgndMeasureCritatiaIntertrig12->az     =     (BkgndMeasurement->azimuth());
//			 pBkgndMeasureCritatiaIntertrig12->ze     =     (BkgndMeasurement->zenith());    
//			 pBkgndMeasureCritatiaIntertrig12->di     =     (BkgndMeasurement->distance())*1000;
//			
//
//			 pBkgndMeasureCritatiaIntertrig12->x      =      pBkgndMeasureCritatiaIntertrig12->di*sin(pBkgndMeasureCritatiaIntertrig12->ze)*cos(pBkgndMeasureCritatiaIntertrig12->az);
//			 pBkgndMeasureCritatiaIntertrig12->y      =      pBkgndMeasureCritatiaIntertrig12->di*sin(pBkgndMeasureCritatiaIntertrig12->ze)*sin(pBkgndMeasureCritatiaIntertrig12->az);
//			 pBkgndMeasureCritatiaIntertrig12->z      =      pBkgndMeasureCritatiaIntertrig12->di*cos(pBkgndMeasureCritatiaIntertrig12->ze);
//
//			 /////////////////////////////////////////////////////////////////////////////
//
//			 BkgndMeasurement->time();
//			 BkgndMeasurement->status();
//
//			 Ptpoint->X = pBkgndMeasureCritatiaIntertrig12->x;
//			 Ptpoint->Y = pBkgndMeasureCritatiaIntertrig12->y;
//			 Ptpoint->Z = pBkgndMeasureCritatiaIntertrig12->z;
//			 Ptpoint->Cont1_trig = "IntervalTrigger";
//			 Ptpoint->Cont2_trig = pBkgndMeasureCritatiaIntertrig12->m_InterTrig;
//			 Ptpoint->Cont3_trig ="";
//			 Ptpoint->filtter = "StdDevFilter";
//			 Ptpoint->Measurement_Mechanism = "Background Measurement";
//			 Ptpoint->Measurement_Mode = "Dynamic Measurement";
//			 Ptpoint->samperobs = pBkgndMeasureCritatiaIntertrig12->SamPOb;
//			 Ptpoint->SMR = pBkgndMeasureCritatiaIntertrig12->smr;
//			 Ptpoint->Star_tig = "External Start Trigger";
//			  GetSystemTime(&Strtime);
//		 
//			 Ptpoint->Time.Format( _T("%04d%02d%02d%02d%02d%02d%03d")
//				 ,Strtime.wYear
//				 ,Strtime.wMonth
//				 ,Strtime.wDay
//				 ,Strtime.wHour
//				 ,Strtime.wMinute
//				 ,Strtime.wSecond
//				 ,Strtime.wMilliseconds);
//
//			 traject.push_back(*Ptpoint);
//
//			 /////////////////////////////////////////////////////////////////////////////
//
//			 pBkgndMeasureCritatiaIntertrig12->m_x.Format("%g", pBkgndMeasureCritatiaIntertrig12->x);
//			 pBkgndMeasureCritatiaIntertrig12->m_y.Format("%g", pBkgndMeasureCritatiaIntertrig12->y);
//			 pBkgndMeasureCritatiaIntertrig12->m_z.Format("%g", pBkgndMeasureCritatiaIntertrig12->z);
//		     
//			 CString Measureb = "x="+pBkgndMeasureCritatiaIntertrig12->m_x+"      y="+pBkgndMeasureCritatiaIntertrig12->m_y+"      z="+pBkgndMeasureCritatiaIntertrig12->m_z;
//
//			 pBkgndMeasureCritatiaIntertrig12->m_listeofmeasur.InsertString(0,Measureb);
//			 pBkgndMeasureCritatiaIntertrig12->m_listeofmeasur.UpdateWindow();
//
//		 }
//		 catch(TrackerException* e )
//		 {
//			 char* msg;
//			 msg = e->getText();
//			 AfxMessageBox(msg);
//			 delete e;
//		 }
//		 if (imad52 == -1)
//		 {
//			 gpTracker->stopBkndMeasurePoint();
//			 break;
//		 }
//	 }
//	 ExitThread(0); 
//	 return 0;
//}
//DWORD WINAPI MyCritariaBkgndMeasureDistTrig12( LPVOID pParam )
//{	
//	CApplication_Faro_TrakerDlg* pBkgndMeasureCritatiaDistTrig12 = (CApplication_Faro_TrakerDlg*)pParam;
//	/////////////////////////////////////////////////////////////////////////////////////
//
//	MeasurePointData* BkgndMeasurement = NULL;
//	StdDevFilter filter;
//	ExternalStartTrigger startTrigger;
//	/////////////////////////////////////////////////////////////////////////////////////
//	DistanceTrigger continueTrigger(pBkgndMeasureCritatiaDistTrig12->DitTrig);
//	/////////////////////////////////////////////////////////////////////////////////////
//	MeasureCfg  cfg( pBkgndMeasureCritatiaDistTrig12->SamPOb , &filter , &startTrigger , &continueTrigger);
//	gpTracker->setBkndMeasureEventRate(1);
//    gpTracker->setBlocking(true);
//    gpTracker->setBkndMeasureBlocking(true);
//	gpTracker->startBkndMeasurePoint( &cfg );
//
//	 while (true)
//	 {
//		 Point* Ptpoint;
//	     Ptpoint = new Point();
//		 try
//		 {
//			 BkgndMeasurement = gpTracker->readBkndMeasurePointData();		
//	         ASSERT(BkgndMeasurement);
//	         LPARAM lParam = (LPARAM)BkgndMeasurement;
//
//			 /////////////////////////////////////////////////////////////////////////////
//
//			 pBkgndMeasureCritatiaDistTrig12->az     =     (BkgndMeasurement->azimuth());
//			 pBkgndMeasureCritatiaDistTrig12->ze     =     (BkgndMeasurement->zenith());    
//			 pBkgndMeasureCritatiaDistTrig12->di     =     (BkgndMeasurement->distance())*1000;
//			
//
//			 pBkgndMeasureCritatiaDistTrig12->x      =      pBkgndMeasureCritatiaDistTrig12->di*sin(pBkgndMeasureCritatiaDistTrig12->ze)*cos(pBkgndMeasureCritatiaDistTrig12->az);
//			 pBkgndMeasureCritatiaDistTrig12->y      =      pBkgndMeasureCritatiaDistTrig12->di*sin(pBkgndMeasureCritatiaDistTrig12->ze)*sin(pBkgndMeasureCritatiaDistTrig12->az);
//			 pBkgndMeasureCritatiaDistTrig12->z      =      pBkgndMeasureCritatiaDistTrig12->di*cos(pBkgndMeasureCritatiaDistTrig12->ze);
//
//			 /////////////////////////////////////////////////////////////////////////////
//
//			 BkgndMeasurement->time();
//			 BkgndMeasurement->status();
//
//			 Ptpoint->X = pBkgndMeasureCritatiaDistTrig12->x;
//			 Ptpoint->Y = pBkgndMeasureCritatiaDistTrig12->y;
//			 Ptpoint->Z = pBkgndMeasureCritatiaDistTrig12->z;
//			 Ptpoint->Cont1_trig = "DistanceTrigger";
//			 Ptpoint->Cont2_trig ="";
//			 Ptpoint->Cont3_trig = pBkgndMeasureCritatiaDistTrig12->m_DitTrig;
//			 Ptpoint->filtter = "StdDevFilter";
//			 Ptpoint->Measurement_Mechanism = "Background Measurement";
//			 Ptpoint->Measurement_Mode = "Dynamic Measurement";
//			 Ptpoint->samperobs = pBkgndMeasureCritatiaDistTrig12->SamPOb;
//			 Ptpoint->SMR = pBkgndMeasureCritatiaDistTrig12->smr;
//			 Ptpoint->Star_tig = "External Start Trigger";
//			  GetSystemTime(&Strtime);
//		 
//			 Ptpoint->Time.Format( _T("%04d%02d%02d%02d%02d%02d%03d")
//				 ,Strtime.wYear
//				 ,Strtime.wMonth
//				 ,Strtime.wDay
//				 ,Strtime.wHour
//				 ,Strtime.wMinute
//				 ,Strtime.wSecond
//				 ,Strtime.wMilliseconds);
//
//			 traject.push_back(*Ptpoint);
//
//			 /////////////////////////////////////////////////////////////////////////////
//
//			 pBkgndMeasureCritatiaDistTrig12->m_x.Format("%g", pBkgndMeasureCritatiaDistTrig12->x);
//			 pBkgndMeasureCritatiaDistTrig12->m_y.Format("%g", pBkgndMeasureCritatiaDistTrig12->y);
//			 pBkgndMeasureCritatiaDistTrig12->m_z.Format("%g", pBkgndMeasureCritatiaDistTrig12->z);
//		     
//			 CString Measureb = "x="+pBkgndMeasureCritatiaDistTrig12->m_x+"      y="+pBkgndMeasureCritatiaDistTrig12->m_y+"      z="+pBkgndMeasureCritatiaDistTrig12->m_z;
//
//			 pBkgndMeasureCritatiaDistTrig12->m_listeofmeasur.InsertString(0,Measureb);
//			 pBkgndMeasureCritatiaDistTrig12->m_listeofmeasur.UpdateWindow();
//
//		 }
//		 catch(TrackerException* e )
//		 {
//			 char* msg;
//			 msg = e->getText();
//			 AfxMessageBox(msg);
//			 delete e;
//		 }
//		  if (imad62 == -1)
//		 {
//			 gpTracker->stopBkndMeasurePoint();
//			 break;
//		 }
//	 }
//ExitThread(0); 
//return 0;
//}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
//DWORD WINAPI MyCritariaFrgndMeasure1( LPVOID pParam )
//{	
//	Point* Ptpoint;
//	Ptpoint = new Point();
//	CApplication_Faro_TrakerDlg* pFrgndMeasureCritatia1 = (CApplication_Faro_TrakerDlg*)pParam;
//	/////////////////////////////////////////////////////////////////////////////////////
//	MeasurePointData* FrgndMeasurement = NULL;
//	AverageFilter filter;
//	NullStartTrigger startTrigger;
//	NullContinueTrigger continueTrigger;
//	/////////////////////////////////////////////////////////////////////////////////////
//	MeasureCfg  cfg( pFrgndMeasureCritatia1->SamPOb , &filter , &startTrigger , &continueTrigger);
//	gpTracker->startMeasurePoint( &cfg );
//
//	/////////////////////////////////////////////////////////////////////////////////////
//
//	 try
//	 {
//		 FrgndMeasurement = gpTracker->readMeasurePointData();		
//		 ASSERT(FrgndMeasurement);
//		 LPARAM lParam = (LPARAM)FrgndMeasurement;
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//		 pFrgndMeasureCritatia1->az     =     (FrgndMeasurement->azimuth());
//		 pFrgndMeasureCritatia1->ze     =     (FrgndMeasurement->zenith());    
//		 pFrgndMeasureCritatia1->di     =     (FrgndMeasurement->distance())*1000;
//		
//
//		 pFrgndMeasureCritatia1->x      =      pFrgndMeasureCritatia1->di*sin(pFrgndMeasureCritatia1->ze)*cos(pFrgndMeasureCritatia1->az);
//		 pFrgndMeasureCritatia1->y      =      pFrgndMeasureCritatia1->di*sin(pFrgndMeasureCritatia1->ze)*sin(pFrgndMeasureCritatia1->az);
//		 pFrgndMeasureCritatia1->z      =      pFrgndMeasureCritatia1->di*cos(pFrgndMeasureCritatia1->ze);
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//		 FrgndMeasurement->time();
//		 FrgndMeasurement->status();
//
//		 Ptpoint->X = pFrgndMeasureCritatia1->x;
//		 Ptpoint->Y = pFrgndMeasureCritatia1->y;
//		 Ptpoint->Z = pFrgndMeasureCritatia1->z;
//		 Ptpoint->Cont1_trig = "NullContinueTrigger";
//		 Ptpoint->Cont2_trig = "";
//		 Ptpoint->Cont3_trig = "";
//		 Ptpoint->filtter = "Average Filter";
//		 Ptpoint->Measurement_Mechanism = "Foreground Measurement";
//		 Ptpoint->Measurement_Mode = "Instantaneous Measurement";
//		 Ptpoint->samperobs = pFrgndMeasureCritatia1->SamPOb;
//		 Ptpoint->SMR = pFrgndMeasureCritatia1->smr;
//		 Ptpoint->Star_tig = "Null Start Trigger";
//		 GetSystemTime(&Strtime);
//		 
//		 Ptpoint->Time.Format( _T("%04d%02d%02d%02d%02d%02d%03d")
//			 ,Strtime.wYear
//			 ,Strtime.wMonth
//			 ,Strtime.wDay
//			 ,Strtime.wHour
//			 ,Strtime.wMinute
//			 ,Strtime.wSecond
//			 ,Strtime.wMilliseconds);
//
//		 traject.push_back(*Ptpoint);
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//		 pFrgndMeasureCritatia1->m_x.Format("%g", pFrgndMeasureCritatia1->x);
//		 pFrgndMeasureCritatia1->m_y.Format("%g", pFrgndMeasureCritatia1->y);
//		 pFrgndMeasureCritatia1->m_z.Format("%g", pFrgndMeasureCritatia1->z);
//
//		 ////////////////////////////////////////////////////////////////////
//
//		 pFrgndMeasureCritatia1->m_az.Format("%g", pFrgndMeasureCritatia1->az);
//		 pFrgndMeasureCritatia1->m_ze.Format("%g", pFrgndMeasureCritatia1->ze);
//		 pFrgndMeasureCritatia1->m_di.Format("%g", pFrgndMeasureCritatia1->di);
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//
//		 pFrgndMeasureCritatia1->GetDlgItem(IDC_EDIT_X)->SetWindowText(pFrgndMeasureCritatia1->m_x);
//		 pFrgndMeasureCritatia1->GetDlgItem(IDC_EDIT_Y)->SetWindowText(pFrgndMeasureCritatia1->m_y);
//		 pFrgndMeasureCritatia1->GetDlgItem(IDC_EDIT_Z)->SetWindowText(pFrgndMeasureCritatia1->m_z);
//
//
//		 pFrgndMeasureCritatia1->GetDlgItem(IDC_EDIT_X)->UpdateWindow();
//		 pFrgndMeasureCritatia1->GetDlgItem(IDC_EDIT_Y)->UpdateWindow();
//		 pFrgndMeasureCritatia1->GetDlgItem(IDC_EDIT_Z)->UpdateWindow();
//
//		 CString Measureb = "x="+pFrgndMeasureCritatia1->m_x+"      y="+pFrgndMeasureCritatia1->m_y+"      z="+pFrgndMeasureCritatia1->m_z;
//
//		 pFrgndMeasureCritatia1->m_listeofmeasur.InsertString(0,Measureb);
//		 pFrgndMeasureCritatia1->m_listeofmeasur.UpdateWindow();
//	 }
//	 catch(TrackerException* e )
//	 {
//		 char* msg;
//		 msg = e->getText();
//		 AfxMessageBox(msg);
//		 delete e;
//	 }
//	
//gpTracker->stopMeasurePoint();	 
//ExitThread(0); 
//return 0;
//}
//DWORD WINAPI MyCritariaFrgndMeasure2( LPVOID pParam )
//{	
//	Point* Ptpoint;
//	Ptpoint = new Point();
//	CApplication_Faro_TrakerDlg* pFrgndMeasureCritatia2 = (CApplication_Faro_TrakerDlg*)pParam;
//	/////////////////////////////////////////////////////////////////////////////////////
//	MeasurePointData* FrgndMeasurement = NULL;
//	AverageFilter filter;
//	ExternalStartTrigger startTrigger;
//	NullContinueTrigger continueTrigger;
//	/////////////////////////////////////////////////////////////////////////////////////
//	MeasureCfg  cfg( pFrgndMeasureCritatia2->SamPOb , &filter , &startTrigger , &continueTrigger);
//	gpTracker->startMeasurePoint( &cfg );
//
//	/////////////////////////////////////////////////////////////////////////////////////
//
//	 try
//	 {
//		 FrgndMeasurement = gpTracker->readMeasurePointData();		
//		 ASSERT(FrgndMeasurement);
//		 LPARAM lParam = (LPARAM)FrgndMeasurement;
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//		 pFrgndMeasureCritatia2->az     =     (FrgndMeasurement->azimuth());
//		 pFrgndMeasureCritatia2->ze     =     (FrgndMeasurement->zenith());    
//		 pFrgndMeasureCritatia2->di     =     (FrgndMeasurement->distance())*1000;
//		
//
//		 pFrgndMeasureCritatia2->x      =      pFrgndMeasureCritatia2->di*sin(pFrgndMeasureCritatia2->ze)*cos(pFrgndMeasureCritatia2->az);
//		 pFrgndMeasureCritatia2->y      =      pFrgndMeasureCritatia2->di*sin(pFrgndMeasureCritatia2->ze)*sin(pFrgndMeasureCritatia2->az);
//		 pFrgndMeasureCritatia2->z      =      pFrgndMeasureCritatia2->di*cos(pFrgndMeasureCritatia2->ze);
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//		 FrgndMeasurement->time();
//		 FrgndMeasurement->status();
//
//		 Ptpoint->X = pFrgndMeasureCritatia2->x;
//		 Ptpoint->Y = pFrgndMeasureCritatia2->y;
//		 Ptpoint->Z = pFrgndMeasureCritatia2->z;
//		 Ptpoint->Cont1_trig = "NullContinueTrigger";
//		 Ptpoint->Cont2_trig = "";
//		 Ptpoint->Cont3_trig = "";
//		 Ptpoint->filtter = "Average Filter";
//		 Ptpoint->Measurement_Mechanism = "Foreground Measurement";
//		 Ptpoint->Measurement_Mode = "Instantaneous Measurement";
//		 Ptpoint->samperobs = pFrgndMeasureCritatia2->SamPOb;
//		 Ptpoint->SMR = pFrgndMeasureCritatia2->smr;
//		 Ptpoint->Star_tig = "External Start Trigger";
//		  GetSystemTime(&Strtime);
//		 
//		 Ptpoint->Time.Format( _T("%04d%02d%02d%02d%02d%02d%03d")
//			 ,Strtime.wYear
//			 ,Strtime.wMonth
//			 ,Strtime.wDay
//			 ,Strtime.wHour
//			 ,Strtime.wMinute
//			 ,Strtime.wSecond
//			 ,Strtime.wMilliseconds);
//
//		 traject.push_back(*Ptpoint);
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//		 pFrgndMeasureCritatia2->m_x.Format("%g", pFrgndMeasureCritatia2->x);
//		 pFrgndMeasureCritatia2->m_y.Format("%g", pFrgndMeasureCritatia2->y);
//		 pFrgndMeasureCritatia2->m_z.Format("%g", pFrgndMeasureCritatia2->z);
//
//		 //////////////////////////////////////////////////////////////////////
//
//		 pFrgndMeasureCritatia2->m_az.Format("%g", pFrgndMeasureCritatia2->az);
//		 pFrgndMeasureCritatia2->m_ze.Format("%g", pFrgndMeasureCritatia2->ze);
//		 pFrgndMeasureCritatia2->m_di.Format("%g", pFrgndMeasureCritatia2->di);
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//
//		 pFrgndMeasureCritatia2->GetDlgItem(IDC_EDIT_X)->SetWindowText(pFrgndMeasureCritatia2->m_x);
//		 pFrgndMeasureCritatia2->GetDlgItem(IDC_EDIT_Y)->SetWindowText(pFrgndMeasureCritatia2->m_y);
//		 pFrgndMeasureCritatia2->GetDlgItem(IDC_EDIT_Z)->SetWindowText(pFrgndMeasureCritatia2->m_z);
//
//
//		 pFrgndMeasureCritatia2->GetDlgItem(IDC_EDIT_X)->UpdateWindow();
//		 pFrgndMeasureCritatia2->GetDlgItem(IDC_EDIT_Y)->UpdateWindow();
//		 pFrgndMeasureCritatia2->GetDlgItem(IDC_EDIT_Z)->UpdateWindow();
//
//		 CString Measureb = "x="+pFrgndMeasureCritatia2->m_x+"      y="+pFrgndMeasureCritatia2->m_y+"      z="+pFrgndMeasureCritatia2->m_z;
//
//		 pFrgndMeasureCritatia2->m_listeofmeasur.InsertString(0,Measureb);
//		 pFrgndMeasureCritatia2 ->m_listeofmeasur.UpdateWindow();
//	 }
//	 catch(TrackerException* e )
//	 {
//		 char* msg;
//		 msg = e->getText();
//		 AfxMessageBox(msg);
//		 delete e;
//	 }
//	
//gpTracker->stopMeasurePoint();	 
//ExitThread(0); 
//return 0;
//}
//
//DWORD WINAPI MyCritariaFrgndMeasure3( LPVOID pParam )
//{	
//	Point* Ptpoint;
//	Ptpoint = new Point();
//	CApplication_Faro_TrakerDlg* pFrgndMeasureCritatia3 = (CApplication_Faro_TrakerDlg*)pParam;
//	/////////////////////////////////////////////////////////////////////////////////////
//	MeasurePointData* FrgndMeasurement = NULL;
//	StdDevFilter filter;
//	NullStartTrigger startTrigger;
//	NullContinueTrigger continueTrigger;
//	/////////////////////////////////////////////////////////////////////////////////////
//	MeasureCfg  cfg( pFrgndMeasureCritatia3->SamPOb , &filter , &startTrigger , &continueTrigger);
//	gpTracker->startMeasurePoint( &cfg );
//
//	/////////////////////////////////////////////////////////////////////////////////////
//
//	 try
//	 {
//		 FrgndMeasurement = gpTracker->readMeasurePointData();		
//		 ASSERT(FrgndMeasurement);
//		 LPARAM lParam = (LPARAM)FrgndMeasurement;
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//		 pFrgndMeasureCritatia3->az     =     (FrgndMeasurement->azimuth());
//		 pFrgndMeasureCritatia3->ze     =     (FrgndMeasurement->zenith());    
//		 pFrgndMeasureCritatia3->di     =     (FrgndMeasurement->distance())*1000;
//		
//
//		 pFrgndMeasureCritatia3->x      =      pFrgndMeasureCritatia3->di*sin(pFrgndMeasureCritatia3->ze)*cos(pFrgndMeasureCritatia3->az);
//		 pFrgndMeasureCritatia3->y      =      pFrgndMeasureCritatia3->di*sin(pFrgndMeasureCritatia3->ze)*sin(pFrgndMeasureCritatia3->az);
//		 pFrgndMeasureCritatia3->z      =      pFrgndMeasureCritatia3->di*cos(pFrgndMeasureCritatia3->ze);
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//		 FrgndMeasurement->time();
//		 FrgndMeasurement->status();
//
//		 Ptpoint->X = pFrgndMeasureCritatia3->x;
//		 Ptpoint->Y = pFrgndMeasureCritatia3->y;
//		 Ptpoint->Z = pFrgndMeasureCritatia3->z;
//		 Ptpoint->Cont1_trig = "NullContinueTrigger";
//		 Ptpoint->Cont2_trig = "";
//		 Ptpoint->Cont3_trig = "";
//		 Ptpoint->filtter = "StdDevFilter";
//		 Ptpoint->Measurement_Mechanism = "Foreground Measurement";
//		 Ptpoint->Measurement_Mode = "Instantaneous Measurement";
//		 Ptpoint->samperobs = pFrgndMeasureCritatia3->SamPOb;
//		 Ptpoint->SMR = pFrgndMeasureCritatia3->smr;
//		 Ptpoint->Star_tig = "Null Start Trigger";
//		  GetSystemTime(&Strtime);
//		 
//		 Ptpoint->Time.Format( _T("%04d%02d%02d%02d%02d%02d%03d")
//			 ,Strtime.wYear
//			 ,Strtime.wMonth
//			 ,Strtime.wDay
//			 ,Strtime.wHour
//			 ,Strtime.wMinute
//			 ,Strtime.wSecond
//			 ,Strtime.wMilliseconds);
//
//		 traject.push_back(*Ptpoint);
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//		 pFrgndMeasureCritatia3->m_x.Format("%g", pFrgndMeasureCritatia3->x);
//		 pFrgndMeasureCritatia3->m_y.Format("%g", pFrgndMeasureCritatia3->y);
//		 pFrgndMeasureCritatia3->m_z.Format("%g", pFrgndMeasureCritatia3->z);
//
//		 //////////////////////////////////////////////////////////////////////
//
//		 pFrgndMeasureCritatia3->m_az.Format("%g", pFrgndMeasureCritatia3->az);
//		 pFrgndMeasureCritatia3->m_ze.Format("%g", pFrgndMeasureCritatia3->ze);
//		 pFrgndMeasureCritatia3->m_di.Format("%g", pFrgndMeasureCritatia3->di);
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//
//		 pFrgndMeasureCritatia3->GetDlgItem(IDC_EDIT_X)->SetWindowText(pFrgndMeasureCritatia3->m_x);
//		 pFrgndMeasureCritatia3->GetDlgItem(IDC_EDIT_Y)->SetWindowText(pFrgndMeasureCritatia3->m_y);
//		 pFrgndMeasureCritatia3->GetDlgItem(IDC_EDIT_Z)->SetWindowText(pFrgndMeasureCritatia3->m_z);
//
//		
//
//		 pFrgndMeasureCritatia3->GetDlgItem(IDC_EDIT_X)->UpdateWindow();
//		 pFrgndMeasureCritatia3->GetDlgItem(IDC_EDIT_Y)->UpdateWindow();
//		 pFrgndMeasureCritatia3->GetDlgItem(IDC_EDIT_Z)->UpdateWindow();
//
//		 CString Measureb = "x="+pFrgndMeasureCritatia3->m_x+"      y="+pFrgndMeasureCritatia3->m_y+"      z="+pFrgndMeasureCritatia3->m_z;
//
//		 pFrgndMeasureCritatia3->m_listeofmeasur.InsertString(0,Measureb);
//		 pFrgndMeasureCritatia3->m_listeofmeasur.UpdateWindow();
//	 }
//	 catch(TrackerException* e )
//	 {
//		 char* msg;
//		 msg = e->getText();
//		 AfxMessageBox(msg);
//		 delete e;
//	 }
//	
//gpTracker->stopMeasurePoint();	 
//ExitThread(0); 
//return 0;
//}
//DWORD WINAPI MyCritariaFrgndMeasure4( LPVOID pParam )
//{	
//	Point* Ptpoint;
//	Ptpoint = new Point();
//	CApplication_Faro_TrakerDlg* pFrgndMeasureCritatia4 = (CApplication_Faro_TrakerDlg*)pParam;
//	/////////////////////////////////////////////////////////////////////////////////////
//	MeasurePointData* FrgndMeasurement = NULL;
//	AverageFilter filter;
//	ExternalStartTrigger startTrigger;
//	NullContinueTrigger continueTrigger;
//	/////////////////////////////////////////////////////////////////////////////////////
//	MeasureCfg  cfg( pFrgndMeasureCritatia4->SamPOb , &filter , &startTrigger , &continueTrigger);
//	gpTracker->startMeasurePoint( &cfg );
//
//	/////////////////////////////////////////////////////////////////////////////////////
//
//	 try
//	 {
//		 FrgndMeasurement = gpTracker->readMeasurePointData();		
//		 ASSERT(FrgndMeasurement);
//		 LPARAM lParam = (LPARAM)FrgndMeasurement;
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//		 pFrgndMeasureCritatia4->az     =     (FrgndMeasurement->azimuth());
//		 pFrgndMeasureCritatia4->ze     =     (FrgndMeasurement->zenith());    
//		 pFrgndMeasureCritatia4->di     =     (FrgndMeasurement->distance())*1000;
//		
//
//		 pFrgndMeasureCritatia4->x      =      pFrgndMeasureCritatia4->di*sin(pFrgndMeasureCritatia4->ze)*cos(pFrgndMeasureCritatia4->az);
//		 pFrgndMeasureCritatia4->y      =      pFrgndMeasureCritatia4->di*sin(pFrgndMeasureCritatia4->ze)*sin(pFrgndMeasureCritatia4->az);
//		 pFrgndMeasureCritatia4->z      =      pFrgndMeasureCritatia4->di*cos(pFrgndMeasureCritatia4->ze);
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//		 FrgndMeasurement->time();
//		 FrgndMeasurement->status();
//
//		 Ptpoint->X = pFrgndMeasureCritatia4->x;
//		 Ptpoint->Y = pFrgndMeasureCritatia4->y;
//		 Ptpoint->Z = pFrgndMeasureCritatia4->z;
//		 Ptpoint->Cont1_trig = "NullContinueTrigger";
//		 Ptpoint->Cont2_trig = "";
//		 Ptpoint->Cont3_trig = "";
//		 Ptpoint->filtter = "Average Filter";
//		 Ptpoint->Measurement_Mechanism = "Foreground Measurement";
//		 Ptpoint->Measurement_Mode = "Instantaneous Measurement";
//		 Ptpoint->samperobs = pFrgndMeasureCritatia4->SamPOb;
//		 Ptpoint->SMR = pFrgndMeasureCritatia4->smr;
//		 Ptpoint->Star_tig = "External Start Trigger";
//		  GetSystemTime(&Strtime);
//		 
//		 Ptpoint->Time.Format( _T("%04d%02d%02d%02d%02d%02d%03d")
//			 ,Strtime.wYear
//			 ,Strtime.wMonth
//			 ,Strtime.wDay
//			 ,Strtime.wHour
//			 ,Strtime.wMinute
//			 ,Strtime.wSecond
//			 ,Strtime.wMilliseconds);
//
//		 traject.push_back(*Ptpoint);
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//		 pFrgndMeasureCritatia4->m_x.Format("%g", pFrgndMeasureCritatia4->x);
//		 pFrgndMeasureCritatia4->m_y.Format("%g", pFrgndMeasureCritatia4->y);
//		 pFrgndMeasureCritatia4->m_z.Format("%g", pFrgndMeasureCritatia4->z);
//
//		 //////////////////////////////////////////////////////////////////////
//
//		 pFrgndMeasureCritatia4->m_az.Format("%g", pFrgndMeasureCritatia4->az);
//		 pFrgndMeasureCritatia4->m_ze.Format("%g", pFrgndMeasureCritatia4->ze);
//		 pFrgndMeasureCritatia4->m_di.Format("%g", pFrgndMeasureCritatia4->di);
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//
//		 pFrgndMeasureCritatia4->GetDlgItem(IDC_EDIT_X)->SetWindowText(pFrgndMeasureCritatia4->m_x);
//		 pFrgndMeasureCritatia4->GetDlgItem(IDC_EDIT_Y)->SetWindowText(pFrgndMeasureCritatia4->m_y);
//		 pFrgndMeasureCritatia4->GetDlgItem(IDC_EDIT_Z)->SetWindowText(pFrgndMeasureCritatia4->m_z);
//
//
//		 pFrgndMeasureCritatia4->GetDlgItem(IDC_EDIT_X)->UpdateWindow();
//		 pFrgndMeasureCritatia4->GetDlgItem(IDC_EDIT_Y)->UpdateWindow();
//		 pFrgndMeasureCritatia4->GetDlgItem(IDC_EDIT_Z)->UpdateWindow();
//
//		 CString Measureb = "x="+pFrgndMeasureCritatia4->m_x+"      y="+pFrgndMeasureCritatia4->m_y+"      z="+pFrgndMeasureCritatia4->m_z;
//
//		 pFrgndMeasureCritatia4->m_listeofmeasur.InsertString(0,Measureb);
//		 pFrgndMeasureCritatia4 ->m_listeofmeasur.UpdateWindow();
//	 }
//	 catch(TrackerException* e )
//	 {
//		 char* msg;
//		 msg = e->getText();
//		 AfxMessageBox(msg);
//		 delete e;
//	 }
//	
//gpTracker->stopMeasurePoint();	 
//ExitThread(0); 
//return 0;
//}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
//DWORD WINAPI MyCritariaBkgndMeasure1( LPVOID pParam )
//{	
//	Point* Ptpoint;
//	Ptpoint = new Point();
//	CApplication_Faro_TrakerDlg* pBkgndMeasureCritatia1 = (CApplication_Faro_TrakerDlg*)pParam;
//	/////////////////////////////////////////////////////////////////////////////////////
//	MeasurePointData* BkgndMeasurement = NULL;
//	AverageFilter filter;
//	NullStartTrigger startTrigger;
//	NullContinueTrigger continueTrigger;
//	/////////////////////////////////////////////////////////////////////////////////////
//	MeasureCfg  cfg( pBkgndMeasureCritatia1->SamPOb , &filter , &startTrigger , &continueTrigger);
//	gpTracker->setBkndMeasureEventRate(1);
//    gpTracker->setBlocking(true);
//    gpTracker->setBkndMeasureBlocking(true);
//	gpTracker->startBkndMeasurePoint( &cfg );
//
//	/////////////////////////////////////////////////////////////////////////////////////
//
//	 try
//	 {
//		 BkgndMeasurement = gpTracker->readBkndMeasurePointData();		
//		 ASSERT(BkgndMeasurement);
//		 LPARAM lParam = (LPARAM)BkgndMeasurement;
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//		 pBkgndMeasureCritatia1->az     =     (BkgndMeasurement->azimuth());
//		 pBkgndMeasureCritatia1->ze     =     (BkgndMeasurement->zenith());    
//		 pBkgndMeasureCritatia1->di     =     (BkgndMeasurement->distance())*1000;
//		
//
//		 pBkgndMeasureCritatia1->x      =      pBkgndMeasureCritatia1->di*sin(pBkgndMeasureCritatia1->ze)*cos(pBkgndMeasureCritatia1->az);
//		 pBkgndMeasureCritatia1->y      =      pBkgndMeasureCritatia1->di*sin(pBkgndMeasureCritatia1->ze)*sin(pBkgndMeasureCritatia1->az);
//		 pBkgndMeasureCritatia1->z      =      pBkgndMeasureCritatia1->di*cos(pBkgndMeasureCritatia1->ze);
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//		 BkgndMeasurement->time();
//		 BkgndMeasurement->status();
//
//		 Ptpoint->X = pBkgndMeasureCritatia1->x;
//		 Ptpoint->Y = pBkgndMeasureCritatia1->y;
//		 Ptpoint->Z = pBkgndMeasureCritatia1->z;
//		 Ptpoint->Cont1_trig = "NullContinueTrigger";
//		 Ptpoint->Cont2_trig = "";
//		 Ptpoint->Cont3_trig = "";
//		 Ptpoint->filtter = "Average Filter";
//		 Ptpoint->Measurement_Mechanism = "Background Measurement";
//		 Ptpoint->Measurement_Mode = "Instantaneous Measurement";
//		 Ptpoint->samperobs = pBkgndMeasureCritatia1->SamPOb;
//		 Ptpoint->SMR = pBkgndMeasureCritatia1->smr;
//		 Ptpoint->Star_tig = "Null Start Trigger";
//		  GetSystemTime(&Strtime);
//		 
//		 Ptpoint->Time.Format( _T("%04d%02d%02d%02d%02d%02d%03d")
//			 ,Strtime.wYear
//			 ,Strtime.wMonth
//			 ,Strtime.wDay
//			 ,Strtime.wHour
//			 ,Strtime.wMinute
//			 ,Strtime.wSecond
//			 ,Strtime.wMilliseconds);
//
//		 traject.push_back(*Ptpoint);
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//		 pBkgndMeasureCritatia1->m_x.Format("%g", pBkgndMeasureCritatia1->x);
//		 pBkgndMeasureCritatia1->m_y.Format("%g", pBkgndMeasureCritatia1->y);
//		 pBkgndMeasureCritatia1->m_z.Format("%g", pBkgndMeasureCritatia1->z);
//
//		 //////////////////////////////////////////////////////////////////////
//
//		 pBkgndMeasureCritatia1->m_az.Format("%g", pBkgndMeasureCritatia1->az);
//		 pBkgndMeasureCritatia1->m_ze.Format("%g", pBkgndMeasureCritatia1->ze);
//		 pBkgndMeasureCritatia1->m_di.Format("%g", pBkgndMeasureCritatia1->di);
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//
//
//		 pBkgndMeasureCritatia1->GetDlgItem(IDC_EDIT_X)->SetWindowText(pBkgndMeasureCritatia1->m_x);
//		 pBkgndMeasureCritatia1->GetDlgItem(IDC_EDIT_Y)->SetWindowText(pBkgndMeasureCritatia1->m_y);
//		 pBkgndMeasureCritatia1->GetDlgItem(IDC_EDIT_Z)->SetWindowText(pBkgndMeasureCritatia1->m_z);
//
//		
//
//		 pBkgndMeasureCritatia1->GetDlgItem(IDC_EDIT_X)->UpdateWindow();
//		 pBkgndMeasureCritatia1->GetDlgItem(IDC_EDIT_Y)->UpdateWindow();
//		 pBkgndMeasureCritatia1->GetDlgItem(IDC_EDIT_Z)->UpdateWindow();
//
//		 CString Measureb = "x="+pBkgndMeasureCritatia1->m_x+"      y="+pBkgndMeasureCritatia1->m_y+"      z="+pBkgndMeasureCritatia1->m_z;
//
//		 pBkgndMeasureCritatia1->m_listeofmeasur.InsertString(0,Measureb);
//		 pBkgndMeasureCritatia1->m_listeofmeasur.UpdateWindow();
//	 }
//	 catch(TrackerException* e )
//	 {
//		 char* msg;
//		 msg = e->getText();
//		 AfxMessageBox(msg);
//		 delete e;
//	 }
//	
//gpTracker->stopBkndMeasurePoint();	 
//ExitThread(0); 
//return 0;
//}
//DWORD WINAPI MyCritariaBkgndMeasure2( LPVOID pParam )
//{	
//	Point* Ptpoint;
//	Ptpoint = new Point();
//	CApplication_Faro_TrakerDlg* pBkgndMeasureCritatia2 = (CApplication_Faro_TrakerDlg*)pParam;
//	/////////////////////////////////////////////////////////////////////////////////////
//	MeasurePointData* BkgndMeasurement = NULL;
//	AverageFilter filter;
//	ExternalStartTrigger startTrigger;
//	NullContinueTrigger continueTrigger;
//	/////////////////////////////////////////////////////////////////////////////////////
//	MeasureCfg  cfg( pBkgndMeasureCritatia2->SamPOb , &filter , &startTrigger , &continueTrigger);
//	gpTracker->setBkndMeasureEventRate(1);
//    gpTracker->setBlocking(true);
//    gpTracker->setBkndMeasureBlocking(true);
//	gpTracker->startBkndMeasurePoint( &cfg );
//
//	/////////////////////////////////////////////////////////////////////////////////////
//
//	 try
//	 {
//		 BkgndMeasurement = gpTracker->readBkndMeasurePointData();		
//		 ASSERT(BkgndMeasurement);
//		 LPARAM lParam = (LPARAM)BkgndMeasurement;
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//		 pBkgndMeasureCritatia2->az     =     (BkgndMeasurement->azimuth());
//		 pBkgndMeasureCritatia2->ze     =     (BkgndMeasurement->zenith());    
//		 pBkgndMeasureCritatia2->di     =     (BkgndMeasurement->distance())*1000;
//		
//
//		 pBkgndMeasureCritatia2->x      =      pBkgndMeasureCritatia2->di*sin(pBkgndMeasureCritatia2->ze)*cos(pBkgndMeasureCritatia2->az);
//		 pBkgndMeasureCritatia2->y      =      pBkgndMeasureCritatia2->di*sin(pBkgndMeasureCritatia2->ze)*sin(pBkgndMeasureCritatia2->az);
//		 pBkgndMeasureCritatia2->z      =      pBkgndMeasureCritatia2->di*cos(pBkgndMeasureCritatia2->ze);
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//		 BkgndMeasurement->time();
//		 BkgndMeasurement->status();
//
//		 Ptpoint->X = pBkgndMeasureCritatia2->x;
//		 Ptpoint->Y = pBkgndMeasureCritatia2->y;
//		 Ptpoint->Z = pBkgndMeasureCritatia2->z;
//		 Ptpoint->Cont1_trig = "NullContinueTrigger";
//		 Ptpoint->Cont2_trig = "";
//		 Ptpoint->Cont3_trig = "";
//		 Ptpoint->filtter = "Average Filter";
//		 Ptpoint->Measurement_Mechanism = "Background Measurement";
//		 Ptpoint->Measurement_Mode = "Instantaneous Measurement";
//		 Ptpoint->samperobs = pBkgndMeasureCritatia2->SamPOb;
//		 Ptpoint->SMR = pBkgndMeasureCritatia2->smr;
//		 Ptpoint->Star_tig = "External Start Trigger";
//		  GetSystemTime(&Strtime);
//		 
//		 Ptpoint->Time.Format( _T("%04d%02d%02d%02d%02d%02d%03d")
//			 ,Strtime.wYear
//			 ,Strtime.wMonth
//			 ,Strtime.wDay
//			 ,Strtime.wHour
//			 ,Strtime.wMinute
//			 ,Strtime.wSecond
//			 ,Strtime.wMilliseconds);
//
//		 traject.push_back(*Ptpoint);
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//		 pBkgndMeasureCritatia2->m_x.Format("%g", pBkgndMeasureCritatia2->x);
//		 pBkgndMeasureCritatia2->m_y.Format("%g", pBkgndMeasureCritatia2->y);
//		 pBkgndMeasureCritatia2->m_z.Format("%g", pBkgndMeasureCritatia2->z);
//
//		 //////////////////////////////////////////////////////////////////////
//
//		 pBkgndMeasureCritatia2->m_az.Format("%g", pBkgndMeasureCritatia2->az);
//		 pBkgndMeasureCritatia2->m_ze.Format("%g", pBkgndMeasureCritatia2->ze);
//		 pBkgndMeasureCritatia2->m_di.Format("%g", pBkgndMeasureCritatia2->di);
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//
//		 pBkgndMeasureCritatia2->GetDlgItem(IDC_EDIT_X)->SetWindowText(pBkgndMeasureCritatia2->m_x);
//		 pBkgndMeasureCritatia2->GetDlgItem(IDC_EDIT_Y)->SetWindowText(pBkgndMeasureCritatia2->m_y);
//		 pBkgndMeasureCritatia2->GetDlgItem(IDC_EDIT_Z)->SetWindowText(pBkgndMeasureCritatia2->m_z);
//
//
//		 pBkgndMeasureCritatia2->GetDlgItem(IDC_EDIT_X)->UpdateWindow();
//		 pBkgndMeasureCritatia2->GetDlgItem(IDC_EDIT_Y)->UpdateWindow();
//		 pBkgndMeasureCritatia2->GetDlgItem(IDC_EDIT_Z)->UpdateWindow();
//
//		 CString Measureb = "x="+pBkgndMeasureCritatia2->m_x+"      y="+pBkgndMeasureCritatia2->m_y+"      z="+pBkgndMeasureCritatia2->m_z;
//
//		 pBkgndMeasureCritatia2->m_listeofmeasur.InsertString(0,Measureb);
//		 pBkgndMeasureCritatia2 ->m_listeofmeasur.UpdateWindow();
//	 }
//	 catch(TrackerException* e )
//	 {
//		 char* msg;
//		 msg = e->getText();
//		 AfxMessageBox(msg);
//		 delete e;
//	 }
//	
//gpTracker->stopBkndMeasurePoint();	 
//ExitThread(0); 
//return 0;
//}
//
//DWORD WINAPI MyCritariaBkgndMeasure3( LPVOID pParam )
//{	
//	Point* Ptpoint;
//	Ptpoint = new Point();
//	CApplication_Faro_TrakerDlg* pBkgndMeasureCritatia3 = (CApplication_Faro_TrakerDlg*)pParam;
//	/////////////////////////////////////////////////////////////////////////////////////
//	MeasurePointData* BkgndMeasurement = NULL;
//	StdDevFilter filter;
//	NullStartTrigger startTrigger;
//	NullContinueTrigger continueTrigger;
//	/////////////////////////////////////////////////////////////////////////////////////
//	MeasureCfg  cfg( pBkgndMeasureCritatia3->SamPOb , &filter , &startTrigger , &continueTrigger);
//	gpTracker->setBkndMeasureEventRate(1);
//    gpTracker->setBlocking(true);
//    gpTracker->setBkndMeasureBlocking(true);
//	gpTracker->startBkndMeasurePoint( &cfg );
//
//	/////////////////////////////////////////////////////////////////////////////////////
//
//	 try
//	 {
//		 BkgndMeasurement = gpTracker->readBkndMeasurePointData();		
//		 ASSERT(BkgndMeasurement);
//		 LPARAM lParam = (LPARAM)BkgndMeasurement;
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//		 pBkgndMeasureCritatia3->az     =     (BkgndMeasurement->azimuth());
//		 pBkgndMeasureCritatia3->ze     =     (BkgndMeasurement->zenith());    
//		 pBkgndMeasureCritatia3->di     =     (BkgndMeasurement->distance())*1000;
//		
//
//		 pBkgndMeasureCritatia3->x      =      pBkgndMeasureCritatia3->di*sin(pBkgndMeasureCritatia3->ze)*cos(pBkgndMeasureCritatia3->az);
//		 pBkgndMeasureCritatia3->y      =      pBkgndMeasureCritatia3->di*sin(pBkgndMeasureCritatia3->ze)*sin(pBkgndMeasureCritatia3->az);
//		 pBkgndMeasureCritatia3->z      =      pBkgndMeasureCritatia3->di*cos(pBkgndMeasureCritatia3->ze);
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//		 BkgndMeasurement->time();
//		 BkgndMeasurement->status();
//
//		 Ptpoint->X = pBkgndMeasureCritatia3->x;
//		 Ptpoint->Y = pBkgndMeasureCritatia3->y;
//		 Ptpoint->Z = pBkgndMeasureCritatia3->z;
//		 Ptpoint->Cont1_trig = "NullContinueTrigger";
//		 Ptpoint->Cont2_trig = "";
//		 Ptpoint->Cont3_trig = "";
//		 Ptpoint->filtter = "StdDevFilter";
//		 Ptpoint->Measurement_Mechanism = "Background Measurement";
//		 Ptpoint->Measurement_Mode = "Instantaneous Measurement";
//		 Ptpoint->samperobs = pBkgndMeasureCritatia3->SamPOb;
//		 Ptpoint->SMR = pBkgndMeasureCritatia3->smr;
//		 Ptpoint->Star_tig = "Null Start Trigger";
//		  GetSystemTime(&Strtime);
//		 
//		 Ptpoint->Time.Format( _T("%04d%02d%02d%02d%02d%02d%03d")
//			 ,Strtime.wYear
//			 ,Strtime.wMonth
//			 ,Strtime.wDay
//			 ,Strtime.wHour
//			 ,Strtime.wMinute
//			 ,Strtime.wSecond
//			 ,Strtime.wMilliseconds);
//
//		 traject.push_back(*Ptpoint);
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//		 pBkgndMeasureCritatia3->m_x.Format("%g", pBkgndMeasureCritatia3->x);
//		 pBkgndMeasureCritatia3->m_y.Format("%g", pBkgndMeasureCritatia3->y);
//		 pBkgndMeasureCritatia3->m_z.Format("%g", pBkgndMeasureCritatia3->z);
//
//		 //////////////////////////////////////////////////////////////////////
//
//		 pBkgndMeasureCritatia3->m_az.Format("%g", pBkgndMeasureCritatia3->az);
//		 pBkgndMeasureCritatia3->m_ze.Format("%g", pBkgndMeasureCritatia3->ze);
//		 pBkgndMeasureCritatia3->m_di.Format("%g", pBkgndMeasureCritatia3->di);
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//		
//
//		 pBkgndMeasureCritatia3->GetDlgItem(IDC_EDIT_X)->SetWindowText(pBkgndMeasureCritatia3->m_x);
//		 pBkgndMeasureCritatia3->GetDlgItem(IDC_EDIT_Y)->SetWindowText(pBkgndMeasureCritatia3->m_y);
//		 pBkgndMeasureCritatia3->GetDlgItem(IDC_EDIT_Z)->SetWindowText(pBkgndMeasureCritatia3->m_z);
//
//		
//
//		 pBkgndMeasureCritatia3->GetDlgItem(IDC_EDIT_X)->UpdateWindow();
//		 pBkgndMeasureCritatia3->GetDlgItem(IDC_EDIT_Y)->UpdateWindow();
//		 pBkgndMeasureCritatia3->GetDlgItem(IDC_EDIT_Z)->UpdateWindow();
//
//		 CString Measureb = "x="+pBkgndMeasureCritatia3->m_x+"      y="+pBkgndMeasureCritatia3->m_y+"      z="+pBkgndMeasureCritatia3->m_z;
//
//		 pBkgndMeasureCritatia3->m_listeofmeasur.InsertString(0,Measureb);
//		 pBkgndMeasureCritatia3->m_listeofmeasur.UpdateWindow();
//	 }
//	 catch(TrackerException* e )
//	 {
//		 char* msg;
//		 msg = e->getText();
//		 AfxMessageBox(msg);
//		 delete e;
//	 }
//	
//gpTracker->stopBkndMeasurePoint();	 
//ExitThread(0); 
//return 0;
//}
//DWORD WINAPI MyCritariaBkgndMeasure4( LPVOID pParam )
//{	
//	Point* Ptpoint;
//	Ptpoint = new Point();
//	CApplication_Faro_TrakerDlg* pBkgndMeasureCritatia4 = (CApplication_Faro_TrakerDlg*)pParam;
//	/////////////////////////////////////////////////////////////////////////////////////
//	MeasurePointData* BkgndMeasurement = NULL;
//	AverageFilter filter;
//	ExternalStartTrigger startTrigger;
//	NullContinueTrigger continueTrigger;
//	/////////////////////////////////////////////////////////////////////////////////////
//	MeasureCfg  cfg( pBkgndMeasureCritatia4->SamPOb , &filter , &startTrigger , &continueTrigger);
//	gpTracker->setBkndMeasureEventRate(1);
//    gpTracker->setBlocking(true);
//    gpTracker->setBkndMeasureBlocking(true);
//	gpTracker->startBkndMeasurePoint( &cfg );
//
//	/////////////////////////////////////////////////////////////////////////////////////
//
//	 try
//	 {
//		 BkgndMeasurement = gpTracker->readBkndMeasurePointData();		
//		 ASSERT(BkgndMeasurement);
//		 LPARAM lParam = (LPARAM)BkgndMeasurement;
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//		 pBkgndMeasureCritatia4->az     =     (BkgndMeasurement->azimuth());
//		 pBkgndMeasureCritatia4->ze     =     (BkgndMeasurement->zenith());    
//		 pBkgndMeasureCritatia4->di     =     (BkgndMeasurement->distance())*1000;
//		
//
//		 pBkgndMeasureCritatia4->x      =      pBkgndMeasureCritatia4->di*sin(pBkgndMeasureCritatia4->ze)*cos(pBkgndMeasureCritatia4->az);
//		 pBkgndMeasureCritatia4->y      =      pBkgndMeasureCritatia4->di*sin(pBkgndMeasureCritatia4->ze)*sin(pBkgndMeasureCritatia4->az);
//		 pBkgndMeasureCritatia4->z      =      pBkgndMeasureCritatia4->di*cos(pBkgndMeasureCritatia4->ze);
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//		 BkgndMeasurement->time();
//		 BkgndMeasurement->status();
//
//		 Ptpoint->X = pBkgndMeasureCritatia4->x;
//		 Ptpoint->Y = pBkgndMeasureCritatia4->y;
//		 Ptpoint->Z = pBkgndMeasureCritatia4->z;
//		 Ptpoint->Cont1_trig = "NullContinueTrigger";
//		 Ptpoint->Cont2_trig = "";
//		 Ptpoint->Cont3_trig = "";
//		 Ptpoint->filtter = "Average Filter";
//		 Ptpoint->Measurement_Mechanism = "Background Measurement";
//		 Ptpoint->Measurement_Mode = "Instantaneous Measurement";
//		 Ptpoint->samperobs = pBkgndMeasureCritatia4->SamPOb;
//		 Ptpoint->SMR = pBkgndMeasureCritatia4->smr;
//		 Ptpoint->Star_tig = "External Start Trigger";
//		  GetSystemTime(&Strtime);
//		 
//		 Ptpoint->Time.Format( _T("%04d%02d%02d%02d%02d%02d%03d")
//			 ,Strtime.wYear
//			 ,Strtime.wMonth
//			 ,Strtime.wDay
//			 ,Strtime.wHour
//			 ,Strtime.wMinute
//			 ,Strtime.wSecond
//			 ,Strtime.wMilliseconds);
//
//		 traject.push_back(*Ptpoint);
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//		 pBkgndMeasureCritatia4->m_x.Format("%g", pBkgndMeasureCritatia4->x);
//		 pBkgndMeasureCritatia4->m_y.Format("%g", pBkgndMeasureCritatia4->y);
//		 pBkgndMeasureCritatia4->m_z.Format("%g", pBkgndMeasureCritatia4->z);
//
//		 //////////////////////////////////////////////////////////////////////
//
//		 pBkgndMeasureCritatia4->m_az.Format("%g", pBkgndMeasureCritatia4->az);
//		 pBkgndMeasureCritatia4->m_ze.Format("%g", pBkgndMeasureCritatia4->ze);
//		 pBkgndMeasureCritatia4->m_di.Format("%g", pBkgndMeasureCritatia4->di);
//
//		 ////////////////////////////////////////////////////////////////////////////////
//
//	
//
//		 pBkgndMeasureCritatia4->GetDlgItem(IDC_EDIT_X)->SetWindowText(pBkgndMeasureCritatia4->m_x);
//		 pBkgndMeasureCritatia4->GetDlgItem(IDC_EDIT_Y)->SetWindowText(pBkgndMeasureCritatia4->m_y);
//		 pBkgndMeasureCritatia4->GetDlgItem(IDC_EDIT_Z)->SetWindowText(pBkgndMeasureCritatia4->m_z);
//
//		
//
//		 pBkgndMeasureCritatia4->GetDlgItem(IDC_EDIT_X)->UpdateWindow();
//		 pBkgndMeasureCritatia4->GetDlgItem(IDC_EDIT_Y)->UpdateWindow();
//		 pBkgndMeasureCritatia4->GetDlgItem(IDC_EDIT_Z)->UpdateWindow();
//
//		 CString Measureb = "x="+pBkgndMeasureCritatia4->m_x+"      y="+pBkgndMeasureCritatia4->m_y+"      z="+pBkgndMeasureCritatia4->m_z;
//
//		 pBkgndMeasureCritatia4->m_listeofmeasur.InsertString(0,Measureb);
//		 pBkgndMeasureCritatia4 ->m_listeofmeasur.UpdateWindow();
//	 }
//	 catch(TrackerException* e )
//	 {
//		 char* msg;
//		 msg = e->getText();
//		 AfxMessageBox(msg);
//		 delete e;
//	 }
//	
//gpTracker->stopBkndMeasurePoint();	 
//ExitThread(0); 
//return 0;
//}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
CWinThread* m_ConnectThreadTracker;
CWinThread* m_ConnectThreadSimul;
CWinThread* m_DisconnectThreadTracker;
CWinThread* m_ConnectThreadSND_RCV;
CWinThread* m_Connect_server;
/////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////Normal Dynamic Measurement/////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
CWinThread* m_Frgnd_Measure_Aver_NullCont_Non_Stop;
CWinThread* m_Frgnd_Measure_StdD_NullCont_Non_Stop;
CWinThread* m_Frgnd_Measure_Aver_Inter_Non_Stop;
CWinThread* m_Frgnd_Measure_StdD_Inter_Non_Stop;
CWinThread* m_Frgnd_Measure_Aver_Dist_Non_Stop;
CWinThread* m_Frgnd_Measure_StdD_Dist_Non_Stop;
CWinThread* m_Frgnd_Measure_Aver_NullCont_Yes_Stop;
CWinThread* m_Frgnd_Measure_StdD_NullCont_Yes_Stop;
CWinThread* m_Frgnd_Measure_Aver_Inter_Yes_Stop;
CWinThread* m_Frgnd_Measure_StdD_Inter_Yes_Stop;
CWinThread* m_Frgnd_Measure_Aver_Dist_Yes_Stop;
CWinThread* m_Frgnd_Measure_StdD_Dist_Yes_Stop;
/////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////Measurement a long a distance//////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
CWinThread* m_Frgnd_Measure_Aver_NullCont_Yes_1Stop;
CWinThread* m_Frgnd_Measure_StdD_NullCont_Yes_1Stop;
CWinThread* m_Frgnd_Measure_Aver_Inter_Yes_1Stop;
CWinThread* m_Frgnd_Measure_StdD_Inter_Yes_1Stop;
CWinThread* m_Frgnd_Measure_Aver_Dist_Yes_1Stop;
CWinThread* m_Frgnd_Measure_StdD_Dist_Yes_1Stop;
/////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////Measurement during a period of time////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
CWinThread* m_Frgnd_Measure_Aver_NullCont_Period;
CWinThread* m_Frgnd_Measure_StdD_NullCont_Period;
CWinThread* m_Frgnd_Measure_Aver_Inter_Period;
CWinThread* m_Frgnd_Measure_StdD_Inter_Period;
CWinThread* m_Frgnd_Measure_Aver_Dist_Period;
CWinThread* m_Frgnd_Measure_StdD_Dist_Period;
/////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////Static Measurement/////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
CWinThread* m_Frgnd_Measure_Aver_PeriodView;
CWinThread* m_Frgnd_Measure_StdD_PeriodView;
CWinThread* m_Frgnd_Measure_Aver_NullCont_Yes_StopROB;
CWinThread* m_Frgnd_Measure_StdD_NullCont_Yes_StopROB;
CWinThread* m_Bknd_Measure_Aver_PeriodView;
/////////////////////////////////////////////////////////////////////////////////////////////
CWinThread* m_MyCritaria_MOVE_TO;
CWinThread* m_MyCritaria_SEARCH_FOR_SMR;
/////////////////////////////////////////////////////////////////////////////////////////////
//CWinThread* m_pFrgndMeasureCritatiaExterConTrig;
//CWinThread* m_pFrgndMeasureCritatiaIntertrig;
//CWinThread* m_pFrgndMeasureCritatiaDistTrig;
//CWinThread* m_pFrgndMeasureCritatiaExterConTrig1;
//CWinThread* m_pFrgndMeasureCritatiaIntertrig1;
//CWinThread* m_pFrgndMeasureCritatiaDistTrig1;
//CWinThread* m_pFrgndMeasureCritatiaExterConTrig2;
//CWinThread* m_pFrgndMeasureCritatiaIntertrig2;
//CWinThread* m_pFrgndMeasureCritatiaDistTrig2;
//CWinThread* m_pFrgndMeasureCritatiaExterConTrig12;
//CWinThread* m_pFrgndMeasureCritatiaIntertrig12;
//CWinThread* m_pFrgndMeasureCritatiaDistTrig12;
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
CWinThread* m_pFrgndMeasurePeriod;




/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
//CWinThread* m_pBkgndMeasureCritatiaExterConTrig;
//CWinThread* m_pBkgndMeasureCritatiaIntertrig;
//CWinThread* m_pBkgndMeasureCritatiaDistTrig;
//CWinThread* m_pBkgndMeasureCritatiaExterConTrig1;
//CWinThread* m_pBkgndMeasureCritatiaIntertrig1;
//CWinThread* m_pBkgndMeasureCritatiaDistTrig1;
//CWinThread* m_pBkgndMeasureCritatiaExterConTrig2;
//CWinThread* m_pBkgndMeasureCritatiaIntertrig2;
//CWinThread* m_pBkgndMeasureCritatiaDistTrig2;
//CWinThread* m_pBkgndMeasureCritatiaExterConTrig12;
//CWinThread* m_pBkgndMeasureCritatiaIntertrig12;
//CWinThread* m_pBkgndMeasureCritatiaDistTrig12;
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
//CWinThread* m_pFrgndMeasureCritatia1;
//CWinThread* m_pFrgndMeasureCritatia2;
//CWinThread* m_pFrgndMeasureCritatia3;
//CWinThread* m_pFrgndMeasureCritatia4;
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
//CWinThread* m_pBkgndMeasureCritatia1;
//CWinThread* m_pBkgndMeasureCritatia2;
//CWinThread* m_pBkgndMeasureCritatia3;
//CWinThread* m_pBkgndMeasureCritatia4;
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
BOOL CApplication_Faro_TrakerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//disable button/////////////////////////////////////////////////////////////////////////////////////

	AfxGetMainWnd() ->GetDlgItem(IDC_BUTTON_DISCONNECT)->EnableWindow(FALSE);
	AfxGetMainWnd() ->GetDlgItem(IDC_BUTTON_STATRTMEAS)->EnableWindow(FALSE);
    AfxGetMainWnd() ->GetDlgItem(IDC_BUTTON_STOPMEAS)->EnableWindow(FALSE);
	AfxGetMainWnd() ->GetDlgItem(IDC_BUTTON_EXPORT)->EnableWindow(FALSE);
    AfxGetMainWnd() ->GetDlgItem(IDC_BUTTON_TAKMEAS)->EnableWindow(FALSE);
	AfxGetMainWnd() ->GetDlgItem(IDC_RADIO_FRGNDMEAS)->EnableWindow(FALSE);
	AfxGetMainWnd() ->GetDlgItem(IDC_RADIO_BKGNDMEAS)->EnableWindow(FALSE);
	AfxGetMainWnd() ->GetDlgItem(IDC_RADIO_AVERAGEFILTER)->EnableWindow(FALSE);
	AfxGetMainWnd() ->GetDlgItem(IDC_RADIO_STDDEVFILTER)->EnableWindow(FALSE);
	AfxGetMainWnd() ->GetDlgItem(IDC_RADIO_NULLCONTTRIGGER)->EnableWindow(FALSE);
    AfxGetMainWnd() ->GetDlgItem(IDC_RADIO_INTERVALTRIGGER)->EnableWindow(FALSE);
    AfxGetMainWnd() ->GetDlgItem(IDC_RADIO_DISTANCE_TRIGGER)->EnableWindow(FALSE);
	AfxGetMainWnd() ->GetDlgItem(IDC_EDIT_INTTRIG)->EnableWindow(FALSE);
	AfxGetMainWnd() ->GetDlgItem(IDC_EDIT_DISTTRIG)->EnableWindow(FALSE);
    AfxGetMainWnd() ->GetDlgItem(IDC_EDIT_MEASU)->EnableWindow(FALSE);
    AfxGetMainWnd() ->GetDlgItem(IDC_EDIT_DISPL_PERIOD)->EnableWindow(FALSE);
    AfxGetMainWnd() ->GetDlgItem(IDC_EDIT_TOL_INTERV)->EnableWindow(FALSE);
    AfxGetMainWnd() ->GetDlgItem(IDC_EDIT_DIST_OF_MEASU)->EnableWindow(FALSE);
    AfxGetMainWnd() ->GetDlgItem(IDC_EDIT_PERIOD_OF_MEASU)->EnableWindow(FALSE);
    AfxGetMainWnd() ->GetDlgItem(IDC_NORM_DYN_MEAS)->EnableWindow(FALSE);
    AfxGetMainWnd() ->GetDlgItem(IDC_MEASUR_DISTANCE)->EnableWindow(FALSE);
    AfxGetMainWnd() ->GetDlgItem(IDC_MEASUR_PERIOD)->EnableWindow(FALSE);
    AfxGetMainWnd() ->GetDlgItem(IDC_STATIC_MEASU)->EnableWindow(FALSE);
	AfxGetMainWnd() ->GetDlgItem(IDC_BUTTON_TAKMEAS)->EnableWindow(TRUE);
	AfxGetMainWnd() ->GetDlgItem(IDC_RADIO_0_5)->EnableWindow(FALSE);
	AfxGetMainWnd() ->GetDlgItem(IDC_RADIO_0_875)->EnableWindow(FALSE);
	AfxGetMainWnd() ->GetDlgItem(IDC_RADIO_1_5)->EnableWindow(FALSE);
    AfxGetMainWnd() ->GetDlgItem(IDC_CHECK_STOP_COLLEC)->EnableWindow(FALSE);
    AfxGetMainWnd() ->GetDlgItem(IDC_BUTTON_DISCONNECT_V7000)->EnableWindow(FALSE);
    //////////////////////////
    ///////////////////////////
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_For_Back_measurement = 0;
	UpdateData(FALSE);
	m_Filter = 0;
	UpdateData(FALSE);
	m_ContinueTrig = 0;
	UpdateData(FALSE);
	m_Smr = 0;
	UpdateData(FALSE);
	m_Norm_Dyn_Meas = 0;
	UpdateData(FALSE);
	m_check_stop_collec = 0;
	UpdateData(FALSE);
	


	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CApplication_Faro_TrakerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CApplication_Faro_TrakerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CApplication_Faro_TrakerDlg::OnEnChangeEditIp1()
{
	CString text;
	GetDlgItem(IDC_EDIT_IP1)->GetWindowText(text);
	if(text.GetLength() == 3)
		GetDlgItem(IDC_EDIT_IP2)->SetFocus();
	else if(text.GetLength() > 3)
		GetDlgItem(IDC_EDIT_IP1)->SetWindowText(text.Left(3));
}

void CApplication_Faro_TrakerDlg::OnEnChangeEditIp2()
{
	CString text;
	GetDlgItem(IDC_EDIT_IP2)->GetWindowText(text);
	if(text.GetLength() == 3)
		GetDlgItem(IDC_EDIT_IP3)->SetFocus();
	else if(text.GetLength() > 3)
		GetDlgItem(IDC_EDIT_IP2)->SetWindowText(text.Left(3));
}

void CApplication_Faro_TrakerDlg::OnEnChangeEditIp3()
{
		CString text;
	GetDlgItem(IDC_EDIT_IP3)->GetWindowText(text);
	if(text.GetLength() == 3)
		GetDlgItem(IDC_EDIT_IP4)->SetFocus();
	else if(text.GetLength() > 3)
		GetDlgItem(IDC_EDIT_IP3)->SetWindowText(text.Left(3));
}

void CApplication_Faro_TrakerDlg::OnEnChangeEditIp4()
{
	CString text;
	GetDlgItem(IDC_EDIT_IP4)->GetWindowText(text);	
	if(text.GetLength() > 3)
		GetDlgItem(IDC_EDIT_IP4)->SetWindowText(text.Left(3));
}

void CApplication_Faro_TrakerDlg::OnBnClickedButtonConnect()
{
	//GetDlgItemText(IDC_EDIT_IP1,m_Ip1);
	//GetDlgItemText(IDC_EDIT_IP2,m_Ip2);
	//GetDlgItemText(IDC_EDIT_IP3,m_Ip3);
 //   GetDlgItemText(IDC_EDIT_IP4,m_Ip4);

	//////////////////////////////////////////////////////////////////////////////////////

	//
 //   Tracker  *trk;
 //   char *IPP = "128.128.128.100";
 //   char *IPO = "user";
 //   char *IPI = "";
 //   
 //   trk = new Tracker("Tracker4xxx");

 //   trk->setBlocking(true);	/* Each of the following methods will not return until the tracker  */
	//		/* completes the command.	  			     */

 //   trk->connect("128.128.128.100", "user", "");
 //   
 //   
 //   trk->home(false);
}

void CApplication_Faro_TrakerDlg::OnBnClickedButtonStatrtmeas()
{
	
	///////////////////////////////////////////////////
	AfxGetMainWnd() ->GetDlgItem(IDC_BUTTON_STATRTMEAS)->EnableWindow(FALSE);
    AfxGetMainWnd() ->GetDlgItem(IDC_BUTTON_STOPMEAS)->EnableWindow(TRUE);
	
    GetDlgItemText(IDC_EDIT_DISPL_PERIOD, m_Display_Period);
    diplay_period = strtod(m_Display_Period , NULL);
    if (diplay_period > 0)
    {
        STOP_DISPLAY = 0;
        HANDLE m_Frgnd_Measure_Aver_PeriodView;
        m_Frgnd_Measure_Aver_PeriodView = CreateThread(NULL,0,MyCritariaBknd_Measure_Aver_PeriodView, (LPVOID) this, 0 ,0); 
    }
    else 
    {
        SendERROR("DISP","PERIOD");
    }
}

void CApplication_Faro_TrakerDlg::OnBnClickedButtonStopmeas()
{
	//////////////
	imad1 = -1;
	imad2 = -1;
	imad3 = -1;
	imad4 = -1;
	imad5 = -1;
	imad6 = -1;
	imad12 = -1;
	imad22 = -1;
	imad32 = -1;
	imad42 = -1;
	imad52 = -1;
	imad62 = -1;
	//////////////
	imaad1 = -1;
	imaad2 = -1;
	imaad3 = -1;
	imaad4 = -1;
	imaad5 = -1;
	imaad6 = -1;
	imaad12 = -1;
	imaad22 = -1;
	imaad32 = -1;
	imaad42 = -1;
	imaad52 = -1;
	imaad62 = -1;
    STOP_DISPLAY = -1;
    CloseHandle(m_Frgnd_Measure_Aver_PeriodView);
	gpTracker->stopMeasurePoint();
	AfxGetMainWnd() ->GetDlgItem(IDC_BUTTON_STATRTMEAS)->EnableWindow(TRUE);
    AfxGetMainWnd() ->GetDlgItem(IDC_BUTTON_STOPMEAS)->EnableWindow(FALSE);
}

void CApplication_Faro_TrakerDlg::OnBnClickedButtonExport()
{
	/*if(osfDlg.FileSaveDlg(szFilter, szDefExtention, NULL))
	{
		hFile = fopen(osfDlg.GetFileName() ,"w");
		int n = traject.size();
		fprintf (hFile, "Time;Points;X;Y;Z;SMR;Measurement Mode;Measurement Mechanism;Samples Per Observation;Filter;Start Trigger;Continue Trigger;Interval Trigger;Distance Trigger\n");
		for (int i = 0 ; i < n ; i++)
		{
			fprintf (hFile, "%s;P%d;%f;%f;%f;%s;%s;%s;%d;%s;%s;%s;%s;%s\n"
				,traject[i].Time
				,i
				,traject[i].X
				,traject[i].Y
				,traject[i].Z
				,traject[i].SMR
				,traject[i].Measurement_Mode
				,traject[i].Measurement_Mechanism
				,traject[i].samperobs
				,traject[i].filtter
				,traject[i].Star_tig
				,traject[i].Cont1_trig
				,traject[i].Cont2_trig
				,traject[i].Cont3_trig);
		}
		fclose(hFile);
	}*/
	//FILE * f = NULL;
	///*if(osfDlg.FileSaveDlg(szFilter, szDefExtention, NULL))
	//{*/
	//f = fopen("C:/tatataat.txt","w");
	//	int n = traject.size();
	//	fprintf (f,"Time;Points;X;Y;Z;SMR;Measurement Mode;Measurement Mechanism;Samples Per Observation;Filter;Start Trigger;Continue Trigger;IntervalTrigger;DistanceTrigger\n");
	//	for (int i = 0 ; i < n ; i++)
	//	{
	//		fprintf (f,"%s;P%d;%f;%f;%f;%s;%s;%s;%d;%s;%s;%s;%s;%s\n"
	//			,traject[i].Time
	//			,i
	//			,traject[i].X
	//			,traject[i].Y
	//			,traject[i].Z
	//			,traject[i].SMR
	//			,traject[i].Measurement_Mode
	//			,traject[i].Measurement_Mechanism
	//			,traject[i].samperobs
	//			,traject[i].filtter
	//			,traject[i].Star_tig
	//			,traject[i].Cont1_trig
	//			,traject[i].Cont2_trig
	//			,traject[i].Cont3_trig);
	//	}
	//	fclose(f);

		/*string const nomFichier("C:/alema automation/tatataat.txt");
		ofstream monFlux(nomFichier.c_str());

		if(monFlux)    
		{
			monFlux << "Time;Points;X;Y;Z;SMR;Measurement Mode;Measurement Mechanism;Samples Per Observation;Filter;Start Trigger;Continue Trigger;IntervalTrigger;DistanceTrigger" << endl;
			int n = traject.size();
			for (int i = 0 ; i < n ; i++)
			{				
				monFlux <<traject[i].Time
					<<";P"<< i
					<<";"<<traject[i].X
					<<";"<<traject[i].Y
					<<";"<<traject[i].Z
					<<";"<<traject[i].SMR
					<<";"<<traject[i].Measurement_Mode
					<<";"<<traject[i].Measurement_Mechanism
					<<";"<<traject[i].samperobs
					<<";"<<traject[i].filtter
					<<";"<<traject[i].Star_tig
					<<";"<<traject[i].Cont1_trig
					<<";"<<traject[i].Cont2_trig
					<<";"<<traject[i].Cont3_trig << endl;
			}
			
		}
		else
		{
			AfxMessageBox("hihihihi");
		}
		*/

	/*}*/
}

void CApplication_Faro_TrakerDlg::OnBnClickedButtonTakmeas()
{
	take_meas = 1;
	UpdateData(TRUE);
	
}
void CApplication_Faro_TrakerDlg::OnBnClickedRadioIntervaltrigger()
{
    UpdateData(TRUE);
	AfxGetMainWnd() ->GetDlgItem(IDC_EDIT_INTTRIG)->EnableWindow(TRUE);
	AfxGetMainWnd() ->GetDlgItem(IDC_EDIT_DISTTRIG)->EnableWindow(FALSE);
}
void CApplication_Faro_TrakerDlg::OnBnClickedRadioDistanceTrigger()
{
    UpdateData(TRUE);
	AfxGetMainWnd() ->GetDlgItem(IDC_EDIT_DISTTRIG)->EnableWindow(TRUE);
	AfxGetMainWnd() ->GetDlgItem(IDC_EDIT_INTTRIG)->EnableWindow(FALSE);
}

void CApplication_Faro_TrakerDlg::OnBnClickedRadioNullconttrigger()
{
    UpdateData(TRUE);
	AfxGetMainWnd() ->GetDlgItem(IDC_EDIT_DISTTRIG)->EnableWindow(FALSE);
	AfxGetMainWnd() ->GetDlgItem(IDC_EDIT_INTTRIG)->EnableWindow(FALSE);
}

void CApplication_Faro_TrakerDlg::OnBnClickedButtonDisconnect()
{
	
	//gpTracker->disconnect();
	// if (gpTracker->connected()==false)
	//  {
	//	AfxGetMainWnd() ->GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(TRUE);
	//	AfxGetMainWnd() ->GetDlgItem(IDC_BUTTON_DISCONNECT)->EnableWindow(FALSE);
	//	AfxGetMainWnd() ->GetDlgItem(IDC_BUTTON_STATRTMEAS)->EnableWindow(FALSE);

	//	///////////////////////////////////////////////////////////////////////

	//	AfxGetMainWnd() ->GetDlgItem(IDC_EDIT_MEASU)->EnableWindow(FALSE);
	//	AfxGetMainWnd() ->GetDlgItem(IDC_RADIO_FRGNDMEAS)->EnableWindow(FALSE);
	//	AfxGetMainWnd() ->GetDlgItem(IDC_RADIO_BKGNDMEAS)->EnableWindow(FALSE);
	//	AfxGetMainWnd() ->GetDlgItem(IDC_BUTTON_TAKMEAS)->EnableWindow(FALSE);
	//	AfxGetMainWnd() ->GetDlgItem(IDC_RADIO_0_5)->EnableWindow(FALSE);
	//	AfxGetMainWnd() ->GetDlgItem(IDC_RADIO_0_875)->EnableWindow(FALSE);
	//	AfxGetMainWnd() ->GetDlgItem(IDC_RADIO_1_5)->EnableWindow(FALSE);
	//	
	//  }
	//else
	//  {
	//	 AfxGetMainWnd() ->GetDlgItem(IDC_BUTTON_DISCONNECT)->EnableWindow(TRUE);
	//  }

	
}





void CApplication_Faro_TrakerDlg::OnEnChangeEditIp5()
{
	CString text;
	GetDlgItem(IDC_EDIT_IP5)->GetWindowText(text);
	if(text.GetLength() == 3)
		GetDlgItem(IDC_EDIT_IP6)->SetFocus();
	else if(text.GetLength() > 3)
		GetDlgItem(IDC_EDIT_IP5)->SetWindowText(text.Left(3));
}

void CApplication_Faro_TrakerDlg::OnEnChangeEditIp6()
{
	CString text;
	GetDlgItem(IDC_EDIT_IP6)->GetWindowText(text);
	if(text.GetLength() == 3)
		GetDlgItem(IDC_EDIT_IP7)->SetFocus();
	else if(text.GetLength() > 3)
		GetDlgItem(IDC_EDIT_IP6)->SetWindowText(text.Left(3));
}

void CApplication_Faro_TrakerDlg::OnEnChangeEditIp7()
{
	CString text;
	GetDlgItem(IDC_EDIT_IP7)->GetWindowText(text);
	if(text.GetLength() == 3)
		GetDlgItem(IDC_EDIT_IP8)->SetFocus();
	else if(text.GetLength() > 3)
		GetDlgItem(IDC_EDIT_IP7)->SetWindowText(text.Left(3));
}

void CApplication_Faro_TrakerDlg::OnEnChangeEditIp8()
{
	CString text;
	GetDlgItem(IDC_EDIT_IP8)->GetWindowText(text);	
	if(text.GetLength() > 3)
		GetDlgItem(IDC_EDIT_IP8)->SetWindowText(text.Left(3));
}
void CApplication_Faro_TrakerDlg::OnBnClickedButtonConnectV7000()
{
	int lvWhhile = 0;
	GetDlgItemText(IDC_EDIT_IP5,m_Ip5);
	GetDlgItemText(IDC_EDIT_IP6,m_Ip6);
	GetDlgItemText(IDC_EDIT_IP7,m_Ip7);
    GetDlgItemText(IDC_EDIT_IP8,m_Ip8);	
    GetDlgItemText(IDC_EDIT_PORT,m_Port);
    //////////////////////////////////////////////////////////////////////////////////////////
    CString ipv7000 = m_Ip5+"."+m_Ip6+"."+m_Ip7+"."+m_Ip8;
	//////////////////////////////////////////////////////////////////////////////////////////
    M_PORT = atoi((const char*)m_Port);
    Initialize_server(ipv7000);
    AfxGetMainWnd() ->GetDlgItem(IDC_BUTTON_CONNECT_V7000)->EnableWindow(FALSE);
    AfxGetMainWnd() ->GetDlgItem(IDC_BUTTON_DISCONNECT_V7000)->EnableWindow(TRUE);
    
	//////////////////////////////////////////////////////////////////////////////////////////
    erreur=99; // Initiation de erreur pour être sur que l'on va rentrer dans la boucle
    while(erreur!=0)// Boucle tant qu'une demande de session (SYN) tcp n'a pas été reçu
	{
		erreur = listen(id_de_la_socket,1);
	}  
	m_List_V7000_Connexion.InsertString(0,"listen        : OK");
	m_List_V7000_Connexion.UpdateWindow();

	//// ********************************************************
	//// Acceptation de la demande d'ouverture de session
	//// ********************************************************
	m_List_V7000_Connexion.InsertString(0,"Pending receipt of request to open a TCP session (SYN)");
	m_List_V7000_Connexion.UpdateWindow();
	tempo = sizeof(information_sur_la_source); // Passe par une variable afin d'utiliser un pointeur
	id_de_la_nouvelle_socket = accept(id_de_la_socket,(struct sockaddr*)&information_sur_la_source,&tempo);
    try
    {
        if(id_de_la_nouvelle_socket==INVALID_SOCKET)
	    {
		    CString WSAGetLastError5;
		    WSAGetLastError5.Format("%d", WSAGetLastError());
		    m_List_V7000_Connexion.InsertString(0,"#9999[9999;2106]");
		    m_List_V7000_Connexion.UpdateWindow();
	    }
	    else
	    {
		    m_List_V7000_Connexion.InsertString(0,"accept        : OK");
            HANDLE m_ConnectThreadSND_RCV;
	        m_ConnectThreadSND_RCV = CreateThread(NULL,0,MyThreadConnectSND_RCV, (LPVOID)this, 0 ,0);
	    }
    }
    catch(CString e)
    {
            e = "#9999[5531;2106]";
            m_List_V7000_Connexion.InsertString(0,"#9999[9999;2106]");
		    m_List_V7000_Connexion.UpdateWindow();
    }
	
    

}
int CApplication_Faro_TrakerDlg::comparerbuffer(char tab1[],char tab2[])
{
	for (int i = 0 ; i< sizeof (tab1) ;i++)
	{
		if (tab1[i]!=tab2[i]) return 0;
	}
	return 1;

}
bool CApplication_Faro_TrakerDlg::findchar(char tab1[],char tab2[])
{
	CString Tab1 = tab1;
	CString Tab2 = tab2;
	const char *taB1 = (LPTSTR) (LPCTSTR) tab1;
	const char *taB2 = (LPTSTR) (LPCTSTR) tab2;
	if (strstr(taB1,taB2)!= NULL)
	{
		return true;
	}
	return false;
}

char *CApplication_Faro_TrakerDlg::str_sub (const char *s,unsigned int start,unsigned int end)
{
   char *new_s = NULL;
   if (s != NULL && start < end)
   {
      new_s = (char*) malloc (sizeof (*new_s) * (end - start + 1));
      if (new_s != NULL)
      {
         unsigned int i;
         for (i = start; i <= end; i++)
         {
            new_s[i-start] = s[i];
         }
         //new_s[i-start] = '\0';
      }
      else
      {
		 AfxMessageBox("Memoire insuffisante");
         exit (EXIT_FAILURE);
      }
   }
   return new_s;
}
CString CApplication_Faro_TrakerDlg::CStringTok(const char *szToken,const char *szDelimit,bool &rbEndParse) 
{ 
   CString strOrg,str; 
   static const char *pszOrg=NULL; 
  
   ASSERT(szDelimit!=NULL); 
  
   if(szToken) 
   { 
       pszOrg=szToken; 
       rbEndParse=false; 
   } 
   if(!pszOrg) 
   { 
        rbEndParse=true; 
        return ""; 
   } 
  
   strOrg=pszOrg; 
   int nPos=strOrg.Find(szDelimit); 
   if(nPos!=-1) 
   { 
      str=strOrg.Left(nPos); 
      pszOrg+=(nPos+strlen(szDelimit)); 
   } 
   else 
   { 
      str=strOrg; 
      pszOrg=NULL;       
   } 
   return str; 
} 
bool CApplication_Faro_TrakerDlg::Look_for(CString first,CString second)
{
	const char *first1 = (LPTSTR) (LPCTSTR) first;
	const char *second1 = (LPTSTR) (LPCTSTR) second;
	if (strstr(first1,second1)!= NULL)
	{
		return true;
	}
	else
	{
		return false;
	}
}
void CApplication_Faro_TrakerDlg::Initialize_server(CString IPV7000)
{
	char* Ipv7000;
    Ipv7000 = (LPTSTR) (LPCTSTR)IPV7000;
	//****************************************************************************************************
	m_List_V7000_Connexion.InsertString(0,"Start Connexion With V7000 Software ...");
	m_List_V7000_Connexion.UpdateWindow();
	// ********************************************************
	// Initialisation de Winsock
	// ********************************************************
	erreur = WSAStartup(MAKEWORD(2,2),&initialisation_win32);
	if (erreur!=0)
	{
		CString erreur1 , WSAGetLastError1;
		erreur1.Format("%d", erreur);
		WSAGetLastError1.Format("%d", WSAGetLastError());
		m_List_V7000_Connexion.InsertString(0,"#9999[5531;2100]");
		m_List_V7000_Connexion.UpdateWindow();
	}
	else
	{
		m_List_V7000_Connexion.InsertString(0,"WSAStartup  : OK");
		m_List_V7000_Connexion.UpdateWindow();
	}
	// ********************************************************
	// Ouverture d'une Socket
	// ********************************************************
	id_de_la_socket = socket(AF_INET,SOCK_STREAM,0);
	if (id_de_la_socket == INVALID_SOCKET)
	{
		CString WSAGetLastError2;
		WSAGetLastError2.Format("%d", WSAGetLastError());
		m_List_V7000_Connexion.InsertString(0,"#9999[5531;2101");
		m_List_V7000_Connexion.UpdateWindow();
	}
	else
	{
		m_List_V7000_Connexion.InsertString(0,"nsocket      : OK");
		m_List_V7000_Connexion.UpdateWindow();
	}
	// ********************************************************
    // Activation de l'option permettant d'activer l'algorithme de Nagle
    // ********************************************************
    tempo = 1;
    erreur = setsockopt(id_de_la_socket,IPPROTO_TCP,TCP_NODELAY,(char *)&tempo,sizeof(tempo));
    if (erreur!=0)
	{
		CString erreur2 , WSAGetLastError3;
		erreur2.Format("%d", erreur);
		WSAGetLastError3.Format("%d", WSAGetLastError());
		m_List_V7000_Connexion.InsertString(0,"#9999[5531;2102");
		m_List_V7000_Connexion.UpdateWindow();
	}
    else
	{
		m_List_V7000_Connexion.InsertString(0,"nsetsockopt  : OK");
		m_List_V7000_Connexion.UpdateWindow();
	}
	// ********************************************************
	// Lie la socket à une ip et un port d'écoute
	// ********************************************************
    unsigned short SHORT_PORT = (unsigned  short)M_PORT;
	information_sur_la_source.sin_family=AF_INET;
	information_sur_la_source.sin_addr.s_addr = INADDR_ANY;//inet_addr(Ipv7000);//INADDR_ANY;//inet_addr(Ipv7000);//("127.0.0.1");//INADDR_ANY; // Ecoute sur toutes les IP locales  
    information_sur_la_source.sin_port = htons(SHORT_PORT); // Ecoute sur le port 33333
	erreur = bind(id_de_la_socket,(struct sockaddr*)&information_sur_la_source,sizeof(information_sur_la_source));
	if (erreur!=0)
	{
		CString erreur3 , WSAGetLastError4;
		erreur3.Format("%d", erreur);
		WSAGetLastError4.Format("%d", WSAGetLastError());
		m_List_V7000_Connexion.InsertString(0,"#9999[5531;2103]");
		m_List_V7000_Connexion.UpdateWindow();
	}
	else
	{
		m_List_V7000_Connexion.InsertString(0,"bind        : OK");
		m_List_V7000_Connexion.UpdateWindow();
	}
}

void CApplication_Faro_TrakerDlg::Send_Recv_Tram()
{	
	int RCEV = 0;
	while (true)
	{
		memset(buffer,0,sizeof (buffer));
		nombre_de_caractere = recv(id_de_la_nouvelle_socket,buffer,1515,0);
		AfxBeginThread(TestMethodThreadCall, (LPVOID)GetSafeHwnd());
		if (nombre_de_caractere != -1)
		{
			CString CStringBuffer = buffer;
			if (buffer[0] == '#')
			{
				bool bEndParse;
					
				CString IDFonction = CStringBuffer.Mid(CStringBuffer.Find("#"),6);
				CString afterAcolOP = CStringBuffer.Mid(CStringBuffer.Find("[")+1);
				CString beforeAcolOP = IDFonction.Mid(IDFonction.Find("[")-4,4);
				CString beforeAcolCL = afterAcolOP.Left(afterAcolOP.Find("]"));
				if (IS_DOUBLE(beforeAcolOP))
				{
					int IbeforeAcolOP = atoi((const char*)beforeAcolOP);
					int IbeforeAcolCL = atoi((const char*)beforeAcolCL);
					
					if (IDFonction == "#5530[")
					{
						memset(buffer,0,sizeof (buffer));
						if (beforeAcolCL == "0")
						{
							SendACQUITT("5530","");
							HANDLE m_ConnectThreadSimul;
							m_ConnectThreadSimul = CreateThread(NULL,0,MyThreadConnectSimul, (LPVOID) this, 0 ,0);
							WaitForSingleObject(m_ConnectThreadSimul,INFINITE);
							CloseHandle(m_ConnectThreadSimul);
						}
						else if (beforeAcolCL == "1")
						{
							SendACQUITT("5530","");
							HANDLE m_ConnectThreadTracker;
							m_ConnectThreadTracker = CreateThread(NULL,0,MyThreadConnectTracker, (LPVOID) this, 0 ,0);
							WaitForSingleObject(m_ConnectThreadTracker,INFINITE);
							CloseHandle(m_ConnectThreadTracker);
						}
                        else if (beforeAcolCL == "-1")
						{
							SendACQUITT("5530","");
							HANDLE m_DisconnectThreadTracker;
							m_DisconnectThreadTracker = CreateThread(NULL,0,MyThreadDisconnectTracker, (LPVOID) this, 0 ,0);
							WaitForSingleObject(m_DisconnectThreadTracker,INFINITE);
							CloseHandle(m_DisconnectThreadTracker);
						}
						else if (Look_for(beforeAcolCL,";"))
						{
							SendERROR("5530","2207");
						}
						else if (beforeAcolCL == "")
						{
							SendERROR("5530","2202");
						}
						else
						{
							SendERROR("5530","2203");
						}
					}
					else if (CONNECT_TRACKER == 1)
                    {
                        
                        if (IbeforeAcolOP == 5538)
                        {
                            if(beforeAcolCL == "1" || beforeAcolCL == "0" /*GET_SMR_DIAMETRE(Inparams[0]) != 0*/ )
                            {
                                PARAM_HOME = beforeAcolCL;
                                HANDLE m_MyCritaria_HOME;
                                SendACQUITT(beforeAcolOP,"");
                                m_MyCritaria_HOME = CreateThread(NULL,0,MyCritaria_HOME, (LPVOID) this, 0 ,0);
                                WaitForSingleObject(m_MyCritaria_HOME,INFINITE);
                                CloseHandle(m_MyCritaria_HOME);
                            }
                            else
                            {
                                SendERROR("5538","2203");
                            }
                        }
                        else if (IbeforeAcolOP >5530 && IbeforeAcolOP <5538 )
						{	
							Inparams.clear();
							GetDlgItemText(IDC_EDIT_MEASU,m_Measu);
							SamPOb = atoi((const char*)m_Measu);
							if (Look_for(beforeAcolCL,";") == true && SamPOb > 0)
							{
								CString Inparam = CStringTok(beforeAcolCL,";",bEndParse); 	        
								while(!bEndParse) 
								{
									Inparams.push_back(Inparam);   
									Inparam = CStringTok(NULL,";",bEndParse); 
								}
								switch(IbeforeAcolOP)
								{
								case 5531:
									{
										if(Inparams.size() == 2)
										{
											switch(m_Filter)
											{
											case 0:
												{
													imaad2 = 0;
													HANDLE m_Frgnd_Measure_Aver_PeriodView;
													SendACQUITT("5531","");
													m_Frgnd_Measure_Aver_PeriodView = CreateThread(NULL,0,MyCritariaFrgnd_Measure_Aver_PeriodView, (LPVOID) this, 0 ,0);
													WaitForSingleObject(m_Frgnd_Measure_Aver_PeriodView,INFINITE);
													CloseHandle(m_Frgnd_Measure_Aver_PeriodView);
													break;
												}
											case 1:
												{
													imaad2 = 0;
													HANDLE m_Frgnd_Measure_StdD_PeriodView;
													SendACQUITT("5531","");
													m_Frgnd_Measure_StdD_PeriodView = CreateThread(NULL,0,MyCritariaFrgnd_Measure_StdD_PeriodView, (LPVOID) this, 0 ,0);
													WaitForSingleObject(m_Frgnd_Measure_StdD_PeriodView,INFINITE);
													CloseHandle(m_Frgnd_Measure_StdD_PeriodView);
													break;
												}
											}
										}
										else
										{
											SendERROR("5531","2207;2202");
										}
										break;
									}
								case 5532:
									{
										if(Inparams.size() == 4)
										{
											if (Inparams[0] == "STOP")
											{
												if(flag_5532[0] == 1)
												{
													STOP_DYN_MEAS(m_Frgnd_Measure_Aver_NullCont_Non_Stop,IDFonction);
													flag_5532[0] = -1;
												}
												if(flag_5532[1] == 1)
												{
													STOP_DYN_MEAS(m_Frgnd_Measure_Aver_Inter_Non_Stop,IDFonction);
													flag_5532[1] = -1;
												}
												if(flag_5532[2] == 1)
												{
													STOP_DYN_MEAS(m_Frgnd_Measure_Aver_Dist_Non_Stop,IDFonction);
													flag_5532[2] = -1;
												}
												////////////////////////////////////////////////////////////////////
												if(flag_5532[3] == 1)
												{
													STOP_DYN_MEAS(m_Frgnd_Measure_StdD_NullCont_Non_Stop,IDFonction);
													flag_5532[3] = -1;
												}
												if(flag_5532[4] == 1)
												{
													STOP_DYN_MEAS(m_Frgnd_Measure_StdD_Inter_Non_Stop,IDFonction);
													flag_5532[4] = -1;
												}
												if(flag_5532[5] == 1)
												{
													STOP_DYN_MEAS(m_Frgnd_Measure_StdD_Dist_Non_Stop,IDFonction);
													flag_5532[5] = -1;
												}
                                                ////////////////////////////////////////////////////////////////////
                                                if(flag_5532[6] == 1)
												{
													STOP_DYN_MEAS(m_Frgnd_Measure_Aver_NullCont_Non_Stop,IDFonction);
													flag_5532[6] = -1;
												}
												if(flag_5532[7] == 1)
												{
													STOP_DYN_MEAS(m_Frgnd_Measure_Aver_Inter_Non_Stop,IDFonction);
													flag_5532[7] = -1;
												}
												if(flag_5532[8] == 1)
												{
													STOP_DYN_MEAS(m_Frgnd_Measure_Aver_Dist_Non_Stop,IDFonction);
													flag_5532[8] = -1;
												}
												////////////////////////////////////////////////////////////////////
												if(flag_5532[9] == 1)
												{
													STOP_DYN_MEAS(m_Frgnd_Measure_StdD_NullCont_Non_Stop,IDFonction);
													flag_5532[9] = -1;
												}
												if(flag_5532[10] == 1)
												{
													STOP_DYN_MEAS(m_Frgnd_Measure_StdD_Inter_Non_Stop,IDFonction);
													flag_5532[10] = -1;
												}
												if(flag_5532[11] == 1)
												{
													STOP_DYN_MEAS(m_Frgnd_Measure_StdD_Dist_Non_Stop,IDFonction);
													flag_5532[11] = -1;
												}
											}
											else if (Inparams[0] == "RECV")
											{
												for(int g = 0 ; g<12 ;g++)
												{
													if(flag_5532[g] == -1)
													{
														RCEV = -1;
														flag_5532[g] = -2;
													}
												}
												if (RCEV == -1)
												{
													RECV_DYN_MEAS(IDFonction,0);
													RCEV = 0;
												}
											}
											else if (Inparams[0] == "START")
											{
												for(int g = 0 ; g<12 ;g++)
												{
													if(flag_5532[g] == -2)
													{
														RCEV = -2;
														flag_5532[g] = 0;
													}
												}
												if (RCEV == -2)
												{
													START_DYN_MEAS(PT_NAME,IDFonction,beforeAcolOP,0);
													RCEV = 0;
												}
											}
											else
											{
												CONVERT_PARAM();
												if (PARAM3 == 0 && PARAM4 ==0)
												{
													switch (m_Filter)
													{
													case 0:
														{
															switch (m_ContinueTrig)
															{
															case 0:
																{
																	imaad2 = 0;
																	HANDLE m_Frgnd_Measure_Aver_NullCont_Non_Stop;
																	SendACQUITT(beforeAcolOP,"");
																	flag_5532[0] = 1;
																	m_Frgnd_Measure_Aver_NullCont_Non_Stop = CreateThread(NULL,0,MyCritariaFrgnd_Measure_Aver_NullCont_Non_Stop, (LPVOID) this, 0 ,0);
																	break;
																}
															case 1:
																{
																	imaad2 = 0;
																	GetDlgItemText(IDC_EDIT_INTTRIG , m_InterTrig);
																	InterTrig = strtod(m_InterTrig , NULL);
																	HANDLE m_Frgnd_Measure_Aver_Inter_Non_Stop;
																	SendACQUITT(beforeAcolOP,"");
																	flag_5532[1] = 1;
																	m_Frgnd_Measure_Aver_Inter_Non_Stop = CreateThread(NULL,0,MyCritariaFrgnd_Measure_Aver_Inter_Non_Stop, (LPVOID) this, 0 ,0);
																	break;
																}
															case 2:
																{
																	imaad2 = 0;
																	GetDlgItemText(IDC_EDIT_DISTTRIG , m_DitTrig);
																	DitTrig = strtod(m_DitTrig,NULL);
																	HANDLE m_Frgnd_Measure_Aver_Dist_Non_Stop;
																	SendACQUITT(beforeAcolOP,"");
																	flag_5532[2] = 1;
																	m_Frgnd_Measure_Aver_Dist_Non_Stop = CreateThread(NULL,0,MyCritariaFrgnd_Measure_Aver_Dist_Non_Stop, (LPVOID) this, 0 ,0);
																	break;
																}
															}
															break;
														}
													case 1:
														{
															switch (m_ContinueTrig)
															{
															case 0:
																{
																	imaad2 = 0;
																	HANDLE m_Frgnd_Measure_StdD_NullCont_Non_Stop;
																	SendACQUITT(beforeAcolOP,"");
																	flag_5532[3] = 1;
																	m_Frgnd_Measure_StdD_NullCont_Non_Stop = CreateThread(NULL,0,MyCritariaFrgnd_Measure_StdD_NullCont_Non_Stop, (LPVOID) this, 0 ,0);
																	break;
																}
															case 1:
																{
																	imaad2 = 0;
																	GetDlgItemText(IDC_EDIT_INTTRIG , m_InterTrig);
																	InterTrig = strtod(m_InterTrig , NULL);
																	HANDLE m_Frgnd_Measure_StdD_Inter_Non_Stop;
																	SendACQUITT(beforeAcolOP,"");
																	flag_5532[4] = 1;
																	m_Frgnd_Measure_StdD_Inter_Non_Stop = CreateThread(NULL,0,MyCritariaFrgnd_Measure_StdD_Inter_Non_Stop, (LPVOID) this, 0 ,0);
																	break;
																}
															case 2:
																{
																	imaad2 = 0;
																	GetDlgItemText(IDC_EDIT_DISTTRIG , m_DitTrig);
																	DitTrig = strtod(m_DitTrig,NULL);
																	HANDLE m_Frgnd_Measure_StdD_Dist_Non_Stop;
																	SendACQUITT(beforeAcolOP,"");
																	flag_5532[5] = 1;
																	m_Frgnd_Measure_StdD_Dist_Non_Stop = CreateThread(NULL,0,MyCritariaFrgnd_Measure_StdD_Dist_Non_Stop, (LPVOID) this, 0 ,0);
																	break;
																}
															}
															break;
														}
													}
												}
												else if (PARAM3 == 1 && PARAM4 !=0)
												{
                                                    switch (m_Filter)
													{
													case 0:
														{
															switch (m_ContinueTrig)
															{
															case 0:
																{
																	imaad2 = 0;
																	HANDLE m_Frgnd_Measure_Aver_NullCont_Yes_Stop;
																	SendACQUITT(beforeAcolOP,"");
																	flag_5532[6] = 1;
																	m_Frgnd_Measure_Aver_NullCont_Yes_Stop = CreateThread(NULL,0,MyCritariaFrgnd_Measure_Aver_NullCont_Yes_Stop, (LPVOID) this, 0 ,0);
																	break;
																}
															case 1:
																{
																	imaad2 = 0;
																	GetDlgItemText(IDC_EDIT_INTTRIG , m_InterTrig);
																	InterTrig = strtod(m_InterTrig , NULL);
																	HANDLE m_Frgnd_Measure_Aver_Inter_Yes_Stop;
																	SendACQUITT(beforeAcolOP,"");
																	flag_5532[7] = 1;
																	m_Frgnd_Measure_Aver_Inter_Yes_Stop = CreateThread(NULL,0,MyCritariaFrgnd_Measure_Aver_Inter_Yes_Stop, (LPVOID) this, 0 ,0);
																	break;
																}
															case 2:
																{
																	imaad2 = 0;
																	GetDlgItemText(IDC_EDIT_DISTTRIG , m_DitTrig);
																	DitTrig = strtod(m_DitTrig,NULL);
																	HANDLE m_Frgnd_Measure_Aver_Dist_Yes_Stop;
																	SendACQUITT(beforeAcolOP,"");
																	flag_5532[8] = 1;
																	m_Frgnd_Measure_Aver_Dist_Yes_Stop = CreateThread(NULL,0,MyCritariaFrgnd_Measure_Aver_Dist_Yes_Stop, (LPVOID) this, 0 ,0);
																	break;
																}
															}
															break;
														}
													case 1:
														{
															switch (m_ContinueTrig)
															{
															case 0:
																{
																	imaad2 = 0;
																	HANDLE m_Frgnd_Measure_StdD_NullCont_Yes_Stop;
																	SendACQUITT(beforeAcolOP,"");
																	flag_5532[9] = 1;
																	m_Frgnd_Measure_StdD_NullCont_Yes_Stop = CreateThread(NULL,0,MyCritariaFrgnd_Measure_StdD_NullCont_Yes_Stop, (LPVOID) this, 0 ,0);
																	break;
																}
															case 1:
																{
																	imaad2 = 0;
																	GetDlgItemText(IDC_EDIT_INTTRIG , m_InterTrig);
																	InterTrig = strtod(m_InterTrig , NULL);
																	HANDLE m_Frgnd_Measure_StdD_Inter_Yes_Stop;
																	SendACQUITT(beforeAcolOP,"");
																	flag_5532[10] = 1;
																	m_Frgnd_Measure_StdD_Inter_Yes_Stop = CreateThread(NULL,0,MyCritariaFrgnd_Measure_StdD_Inter_Yes_Stop, (LPVOID) this, 0 ,0);
																	break;
																}
															case 2:
																{
																	imaad2 = 0;
																	GetDlgItemText(IDC_EDIT_DISTTRIG , m_DitTrig);
																	DitTrig = strtod(m_DitTrig,NULL);
																	HANDLE m_Frgnd_Measure_StdD_Dist_Yes_Stop;
																	SendACQUITT(beforeAcolOP,"");
																	flag_5532[11] = 1;
																	m_Frgnd_Measure_StdD_Dist_Yes_Stop = CreateThread(NULL,0,MyCritariaFrgnd_Measure_StdD_Dist_Yes_Stop, (LPVOID) this, 0 ,0);
																	break;
																}
															}
															break;
														}
													}
												}
                                                else
                                                {
                                                    SendERROR("5532","2205;3");
                                                }
											}
										}
                                        else 
                                        {
                                            SendERROR("5532","2207;2202");
                                        }
										break;
                                    }
								case 5533:
									{
                                        if(Inparams.size() == 5)
										{
											if (Inparams[0] == "STOP")
											{
												if(flag_5533[0] == 1)
												{
													STOP_DYN_MEAS(m_Frgnd_Measure_Aver_NullCont_Yes_1Stop,IDFonction);
													flag_5533[0] = -1;
												}
												if(flag_5533[1] == 1)
												{
													STOP_DYN_MEAS(m_Frgnd_Measure_Aver_Inter_Yes_1Stop,IDFonction);
													flag_5533[1] = -1;
												}
												if(flag_5533[2] == 1)
												{
													STOP_DYN_MEAS(m_Frgnd_Measure_Aver_Dist_Yes_1Stop,IDFonction);
													flag_5533[2] = -1;
												}
												////////////////////////////////////////////////////////////////////
												if(flag_5533[3] == 1)
												{
													STOP_DYN_MEAS(m_Frgnd_Measure_StdD_NullCont_Yes_1Stop,IDFonction);
													flag_5533[3] = -1;
												}
												if(flag_5533[4] == 1)
												{
													STOP_DYN_MEAS(m_Frgnd_Measure_StdD_Inter_Yes_1Stop,IDFonction);
													flag_5533[4] = -1;
												}
												if(flag_5533[5] == 1)
												{
													STOP_DYN_MEAS(m_Frgnd_Measure_StdD_Dist_Yes_1Stop,IDFonction);
													flag_5533[5] = -1;
												}
                                                ////////////////////////////////////////////////////////////////////
											}
											else if (Inparams[0] == "RECV")
											{
												for(int g = 0 ; g<6 ;g++)
												{
													if(flag_5533[g] == -1)
													{
														RCEV = -1;
														flag_5533[g] = -2;
													}
												}
												if (RCEV == -1)
												{
													RECV_DYN_MEAS(IDFonction,1);
													RCEV = 0;
												}
											}
											else if (Inparams[0] == "START")
											{
												for(int g = 0 ; g<6 ;g++)
												{
													if(flag_5533[g] == -2)
													{
														RCEV = -2;
														flag_5533[g] = 0;
													}
												}
												if (RCEV == -2)
												{
													START_DYN_MEAS(PT_NAME,IDFonction,beforeAcolOP,1);
													RCEV = 0;
												}
											}
											else
											{
												CONVERT_PARAM();
                                                if (PARAM3 != 0)
                                                {
                                                    if (PARAM3 > (PARAM4 + PARAM5))
                                                    {
                                                    	switch (m_Filter)
													    {
													    case 0:
														    {
															    switch (m_ContinueTrig)
															    {
															    case 0:
																    {
																	    imaad2 = 0;
																	    HANDLE m_Frgnd_Measure_Aver_NullCont_Yes_1Stop;
																	    SendACQUITT(beforeAcolOP,"");
																	    flag_5533[0] = 1;
																	    m_Frgnd_Measure_Aver_NullCont_Yes_1Stop = CreateThread(NULL,0,MyCritariaFrgnd_Measure_Aver_NullCont_Yes_1Stop, (LPVOID) this, 0 ,0);
																	    break;
																    }
															    case 1:
																    {
																	    imaad2 = 0;
																	    GetDlgItemText(IDC_EDIT_INTTRIG , m_InterTrig);
																	    InterTrig = strtod(m_InterTrig , NULL);
																	    HANDLE m_Frgnd_Measure_Aver_Inter_Yes_1Stop;
																	    SendACQUITT(beforeAcolOP,"");
																	    flag_5533[1] = 1;
																	    m_Frgnd_Measure_Aver_Inter_Yes_1Stop = CreateThread(NULL,0,MyCritariaFrgnd_Measure_Aver_Inter_Yes_1Stop, (LPVOID) this, 0 ,0);
																	    break;
																    }
															    case 2:
																    {
																	    imaad2 = 0;
																	    GetDlgItemText(IDC_EDIT_DISTTRIG , m_DitTrig);
																	    DitTrig = strtod(m_DitTrig,NULL);
																	    HANDLE m_Frgnd_Measure_Aver_Dist_Yes_1Stop;
																	    SendACQUITT(beforeAcolOP,"");
																	    flag_5533[2] = 1;
																	    m_Frgnd_Measure_Aver_Dist_Yes_1Stop = CreateThread(NULL,0,MyCritariaFrgnd_Measure_Aver_Dist_Yes_1Stop, (LPVOID) this, 0 ,0);
																	    break;
																    }
															    }
															    break;
														    }
													    case 1:
														    {
															    switch (m_ContinueTrig)
															    {
															    case 0:
																    {
																	    imaad2 = 0;
																	    HANDLE m_Frgnd_Measure_StdD_NullCont_Yes_1Stop;
																	    SendACQUITT(beforeAcolOP,"");
																	    flag_5533[3] = 1;
																	    m_Frgnd_Measure_StdD_NullCont_Yes_1Stop = CreateThread(NULL,0,MyCritariaFrgnd_Measure_StdD_NullCont_Yes_1Stop, (LPVOID) this, 0 ,0);
																	    break;
																    }
															    case 1:
																    {
																	    imaad2 = 0;
																	    GetDlgItemText(IDC_EDIT_INTTRIG , m_InterTrig);
																	    InterTrig = strtod(m_InterTrig , NULL);
																	    HANDLE m_Frgnd_Measure_StdD_Inter_Yes_1Stop;
																	    SendACQUITT(beforeAcolOP,"");
																	    flag_5533[4] = 1;
																	    m_Frgnd_Measure_StdD_Inter_Yes_1Stop = CreateThread(NULL,0,MyCritariaFrgnd_Measure_StdD_Inter_Yes_1Stop, (LPVOID) this, 0 ,0);
																	    break;
																    }
															    case 2:
																    {
																	    imaad2 = 0;
																	    GetDlgItemText(IDC_EDIT_DISTTRIG , m_DitTrig);
																	    DitTrig = strtod(m_DitTrig,NULL);
																	    HANDLE m_Frgnd_Measure_StdD_Dist_Yes_1Stop;
																	    SendACQUITT(beforeAcolOP,"");
																	    flag_5533[5] = 1;
																	    m_Frgnd_Measure_StdD_Dist_Yes_1Stop = CreateThread(NULL,0,MyCritariaFrgnd_Measure_StdD_Dist_Yes_1Stop, (LPVOID) this, 0 ,0);
																	    break;
																    }
															    }
															    break;
														    }
													    }
                                                    }
                                                    else
                                                    { 
                                                        SendERROR("5533","1108");
                                                        SendERROR("5533","1107");
                                                        SendERROR("5533","1109");
                                                    }
                                                }
                                                else 
                                                {
                                                    SendERROR("5533","2205;3");
                                                }

											}
										}
                                        else 
                                        {
                                            SendERROR("5533","2207;2202");
                                        }
										break;
									}
                                case 5534:
                                    {
                                        if(Inparams.size() == 5)
										{
											if (Inparams[0] == "STOP")
											{
												if(flag_5534[0] == 1)
												{
													STOP_DYN_MEAS(m_Frgnd_Measure_Aver_NullCont_Period,IDFonction);
													flag_5534[0] = -1;
												}
												if(flag_5534[1] == 1)
												{
													STOP_DYN_MEAS(m_Frgnd_Measure_Aver_Inter_Period,IDFonction);
													flag_5534[1] = -1;
												}
												if(flag_5534[2] == 1)
												{
													STOP_DYN_MEAS(m_Frgnd_Measure_Aver_Dist_Period,IDFonction);
													flag_5534[2] = -1;
												}
												////////////////////////////////////////////////////////////////////
												if(flag_5534[3] == 1)
												{
													STOP_DYN_MEAS(m_Frgnd_Measure_StdD_NullCont_Period,IDFonction);
													flag_5534[3] = -1;
												}
												if(flag_5534[4] == 1)
												{
													STOP_DYN_MEAS(m_Frgnd_Measure_StdD_Inter_Period,IDFonction);
													flag_5534[4] = -1;
												}
												if(flag_5534[5] == 1)
												{
													STOP_DYN_MEAS(m_Frgnd_Measure_StdD_Dist_Period,IDFonction);
													flag_5534[5] = -1;
												}
                                                ////////////////////////////////////////////////////////////////////
											}
											else if (Inparams[0] == "RECV")
											{
												for(int g = 0 ; g<6 ;g++)
												{
													if(flag_5534[g] == -1)
													{
														RCEV = -1;
														flag_5534[g] = -2;
													}
												}
												if (RCEV == -1)
												{
													RECV_DYN_MEAS(IDFonction,0);
													RCEV = 0;
												}
											}
											else if (Inparams[0] == "START")
											{
												for(int g = 0 ; g<6 ;g++)
												{
													if(flag_5534[g] == -2)
													{
														RCEV = -2;
														flag_5534[g] = 0;
													}
												}
												if (RCEV == -2)
												{
													START_DYN_MEAS(PT_NAME,IDFonction,beforeAcolOP,0);
													RCEV = 0;
												}
											}
											else
											{
												CONVERT_PARAM();
                                                if (PARAM3 != 0)
                                                {
                                                    if (PARAM3 > (PARAM4 + PARAM5))
                                                    {
                                                    	switch (m_Filter)
													    {
													    case 0:
														    {
															    switch (m_ContinueTrig)
															    {
															    case 0:
																    {
																	    imaad2 = 0;
																	    HANDLE m_Frgnd_Measure_Aver_NullCont_Period;
																	    SendACQUITT(beforeAcolOP,"");
																	    flag_5534[0] = 1;
																	    m_Frgnd_Measure_Aver_NullCont_Period = CreateThread(NULL,0,MyCritariaFrgnd_Measure_Aver_NullCont_Period, (LPVOID) this, 0 ,0);
																	    break;
																    }
															    case 1:
																    {
																	    imaad2 = 0;
																	    GetDlgItemText(IDC_EDIT_INTTRIG , m_InterTrig);
																	    InterTrig = strtod(m_InterTrig , NULL);
																	    HANDLE m_Frgnd_Measure_Aver_Inter_Period;
																	    SendACQUITT(beforeAcolOP,"");
																	    flag_5534[1] = 1;
																	    m_Frgnd_Measure_Aver_Inter_Period = CreateThread(NULL,0,MyCritariaFrgnd_Measure_Aver_Inter_Period, (LPVOID) this, 0 ,0);
																	    break;
																    }
															    case 2:
																    {
																	    imaad2 = 0;
																	    GetDlgItemText(IDC_EDIT_DISTTRIG , m_DitTrig);
																	    DitTrig = strtod(m_DitTrig,NULL);
																	    HANDLE m_Frgnd_Measure_Aver_Dist_Period;
																	    SendACQUITT(beforeAcolOP,"");
																	    flag_5534[2] = 1;
																	    m_Frgnd_Measure_Aver_Dist_Period = CreateThread(NULL,0,MyCritariaFrgnd_Measure_Aver_Dist_Period, (LPVOID) this, 0 ,0);
																	    break;
																    }
															    }
															    break;
														    }
													    case 1:
														    {
															    switch (m_ContinueTrig)
															    {
															    case 0:
																    {
																	    imaad2 = 0;
																	    HANDLE m_Frgnd_Measure_StdD_NullCont_Period;
																	    SendACQUITT(beforeAcolOP,"");
																	    flag_5534[3] = 1;
																	    m_Frgnd_Measure_StdD_NullCont_Period = CreateThread(NULL,0,MyCritariaFrgnd_Measure_StdD_NullCont_Period, (LPVOID) this, 0 ,0);
																	    break;
																    }
															    case 1:
																    {
																	    imaad2 = 0;
																	    GetDlgItemText(IDC_EDIT_INTTRIG , m_InterTrig);
																	    InterTrig = strtod(m_InterTrig , NULL);
																	    HANDLE m_Frgnd_Measure_StdD_Inter_Period;
																	    SendACQUITT(beforeAcolOP,"");
																	    flag_5534[4] = 1;
																	    m_Frgnd_Measure_StdD_Inter_Period = CreateThread(NULL,0,MyCritariaFrgnd_Measure_StdD_Inter_Period, (LPVOID) this, 0 ,0);
																	    break;
																    }
															    case 2:
																    {
																	    imaad2 = 0;
																	    GetDlgItemText(IDC_EDIT_DISTTRIG , m_DitTrig);
																	    DitTrig = strtod(m_DitTrig,NULL);
																	    HANDLE m_Frgnd_Measure_StdD_Dist_Period;
																	    SendACQUITT(beforeAcolOP,"");
																	    flag_5534[5] = 1;
																	    m_Frgnd_Measure_StdD_Dist_Period = CreateThread(NULL,0,MyCritariaFrgnd_Measure_StdD_Dist_Period, (LPVOID) this, 0 ,0);
																	    break;
																    }
															    }
															    break;
														    }
													    }
                                                    }
                                                    else
                                                    { 
                                                        SendERROR("5533","1104");
                                                        SendERROR("5533","1105");
                                                        SendERROR("5533","1106");
                                                    }
                                                }
                                                else 
                                                {
                                                    SendERROR("5533","2205;3");
                                                }

											}
										}
                                        else 
                                        {
                                            SendERROR("5534","2207;2202");
                                        }
										break;
                                    }
                                case 5535:
                                    {
                                        if(Inparams.size() == 3)
                                        {
                                            if (Inparams[0] == "STOP")
                                            {
                                            	if(flag_5535[0] == 1)
												{
													STOP_DYN_MEAS(m_Frgnd_Measure_Aver_NullCont_Yes_StopROB,IDFonction);
													flag_5535[0] = 0;
												}
												if(flag_5535[1] == 1)
												{
													STOP_DYN_MEAS(m_Frgnd_Measure_Aver_NullCont_Yes_StopROB,IDFonction);
													flag_5535[1] = 0;
												}
                                            }
                                            else
                                            {
                                                CONVERT_PARAM();
                                                switch (m_Filter)
                                                {
                                                case 0:
                                                    {
                                                        imaad2 = 0;
													    HANDLE m_Frgnd_Measure_Aver_NullCont_Yes_StopROB;
													    SendACQUITT(beforeAcolOP,"");
													    flag_5535[0] = 1;
													    m_Frgnd_Measure_Aver_NullCont_Yes_StopROB = CreateThread(NULL,0,MyCritariaFrgnd_Measure_Aver_NullCont_Yes_StopROB, (LPVOID) this, 0 ,0);
                                                        break;
                                                    }
                                                case 1:
                                                    {
                                                        imaad2 = 0;
													    HANDLE m_Frgnd_Measure_StdD_NullCont_Yes_StopROB;
													    SendACQUITT(beforeAcolOP,"");
													    flag_5535[0] = 1;
													    m_Frgnd_Measure_StdD_NullCont_Yes_StopROB = CreateThread(NULL,0,MyCritariaFrgnd_Measure_StdD_NullCont_Yes_StopROB, (LPVOID) this, 0 ,0);
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                        else
                                        {
                                            SendERROR("5535","2207;2202");
                                        }
                                        break;
                                    }
                                case 5536:
                                    {
                                        flag_5536=0;
                                        if(Inparams.size() == 5)
                                        {
                                            for (int g = 0 ; g < 3 ; g++)
                                            {
                                                if (IS_DOUBLE(Inparams[g]) == false)
                                                {
                                                    flag_5536 = 1;
                                                }
                                            }
                                            if (flag_5536 == 0)
                                            {
                                                if ((Inparams[3] != "0" && Inparams[3] != "1")||(Inparams[4] != "0" && Inparams[4] != "1"))
                                                {
                                                    SendERROR("5536","2205;3;4");
                                                }
                                                else
                                                {
                                                    XM = strtod(Inparams[0] , NULL);
                                                    YM = strtod(Inparams[1] , NULL);
                                                    ZM = strtod(Inparams[2] , NULL);
                                                    isBacksight = CONVERT_BOOL(Inparams[3]);
                                                    isRelative  = CONVERT_BOOL(Inparams[4]);
                                                    ////////////////////////////////////////
												    HANDLE m_MyCritaria_MOVE_TO;
												    SendACQUITT(beforeAcolOP,"");
												    m_MyCritaria_MOVE_TO = CreateThread(NULL,0,MyCritaria_MOVE_TO, (LPVOID) this, 0 ,0);
                                            		WaitForSingleObject(m_MyCritaria_MOVE_TO,INFINITE);
		                                            CloseHandle(m_MyCritaria_MOVE_TO);
                                                }
                                            }
                                            else
                                            {
                                                SendERROR("5536","2203");
                                            }
                                        }
                                        else
                                        {
                                            SendERROR("5536","2207;2202");
                                        }
                                        break;
                                    }                 
                                case 5537:
                                    {
                                        if(Inparams.size() == 2)
                                        {
                                            if(IS_DOUBLE(Inparams[0]) && IS_DOUBLE(Inparams[1]))
                                            {
                                                RADIUS = strtod(Inparams[0] , NULL);
                                                DISTAN = strtod(Inparams[1] , NULL);
                                                HANDLE m_MyCritaria_SEARCH_FOR_SMR;
                                                SendACQUITT(beforeAcolOP,"");
                                                m_MyCritaria_SEARCH_FOR_SMR = CreateThread(NULL,0,MyCritaria_SEARCH_FOR_SMR, (LPVOID) this, 0 ,0);
                                                WaitForSingleObject(m_MyCritaria_SEARCH_FOR_SMR,INFINITE);
                                                CloseHandle(m_MyCritaria_SEARCH_FOR_SMR);
                                            }
                                            else
                                            {
                                                SendERROR("5537","2203");
                                            }
                                        }
                                        else
                                        {
                                            SendERROR("5537","2207;2202");
                                        }
                                        break;
                                    }
                                }
                            }
						    else
						    {
							    SendERROR("9999","2202");
						    }
					    }
					    else
					    {
						    SendERROR("9999","2204");
					    }
				    }
				    else
				    {
					    SendERROR("9999","1100");
                    }
			    }
			    else
			    {
				    SendERROR("9999","2204");
                }
		    }
		    else if (buffer[0] == '!')
		    {}
		    else 
		    {
			    SendERROR("9999","2200");
		    }
	    }
	    else
	    {
		    SendERROR("9999","2107");
	    }
    }
}
void CApplication_Faro_TrakerDlg::SendTramPtPosition(CString PT_NAME,CString FCT_NUM,int STRT)
{
	CString mX;
	CString mY;
	CString mZ;
	CString J;
	for(int j = STRT ; j < traject.size() ; j++)
	{
		mX.Format("%g", traject[j].X);
		mY.Format("%g", traject[j].Y);
		mZ.Format("%g", traject[j].Z);
		J.Format("%d", j);
        Sleep(10);
		
		memset(buffer,0,sizeof (buffer));					
		strcpy_s(buffer,"#"+FCT_NUM+"["+PT_NAME+""+J+";"+mX+";"+mY+";"+mZ+"]"); // Copie la chaine de caractère dans buffer
		nombre_de_caractere = send(id_de_la_nouvelle_socket,buffer,strlen(buffer),0);
	}
}

void CApplication_Faro_TrakerDlg::SendERROR(CString ID_FCT,CString CD_ERROR)
{
	memset(buffer,0,sizeof (buffer));
	strcpy_s(buffer,"#9999["+ID_FCT+";"+CD_ERROR+"]");
	nombre_de_caractere = send(id_de_la_nouvelle_socket,buffer,strlen(buffer),0);
}
void CApplication_Faro_TrakerDlg::SendACQUITT(CString ID_FCT,CString MSGS)
{
	memset(buffer,0,sizeof (buffer));
	strcpy_s(buffer,"$"+ID_FCT+"["+MSGS+"]"); // Copie la chaine de caractère dans buffer
	nombre_de_caractere = send(id_de_la_nouvelle_socket,buffer,strlen(buffer),0);
}

double dtest;
bool CApplication_Faro_TrakerDlg::IS_DOUBLE(CString myCString)
{  
   
   int bad = 0;
   int dotCounter = 0;
   if (myCString[0] == '-'|| isdigit(myCString[0]))
   {
       for(int i = 1; i < myCString.GetLength(); i++) 
       {   
           
          if(myCString[i] != '.') 
	      {
             if(!isdigit(myCString[i])) 
		     {
                bad++;
                break;
             }
          }     
          else 
	      {
             dotCounter++;
             if(dotCounter > 1) 
		     {
                bad++;
                break;
             }
          }
       }
   }
   else
   {
       bad++;
   }
 
   if(bad != 0)
   {
	   return false;
   }
   else
   {  
	   return true;
   }
}

void CApplication_Faro_TrakerDlg::CONVERT_PARAM()
{
	int s = Inparams.size();
	if (s < 6)
	{
		PT_NAME = Inparams[0];
		SMR_TYPE = Inparams[1];
		switch (s)
		{
		case 2:
			{
				PARAM3 = 0;
				PARAM4 = 0;
				PARAM5 = 0;
				break;
			}
		case 3:
			{
				if (IS_DOUBLE(Inparams[2]) == true)
				{
					PARAM3 = strtod(Inparams[2] , NULL);
					PARAM4 = 0;
					PARAM5 = 0;
				}
				else
				{
					SendERROR("9999","2203");
				}
				break;
			}
		case 4:
			{
				if (IS_DOUBLE(Inparams[2]) == true && IS_DOUBLE(Inparams[3]) == true)
				{
					PARAM3 = strtod(Inparams[2] , NULL);
					PARAM4 = strtod(Inparams[3] , NULL);
					PARAM5 = 0;
				}
				else
				{
					SendERROR("9999","2203");
				}
				break;
			}
		case 5:
			{
				if (IS_DOUBLE(Inparams[2]) == true && IS_DOUBLE(Inparams[3]) == true && IS_DOUBLE(Inparams[4]) == true)
				{
					PARAM3 = strtod(Inparams[2] , NULL);
					PARAM4 = strtod(Inparams[3] , NULL);
					PARAM5 = strtod(Inparams[4] , NULL);
				}
				else
				{
					SendERROR("9999","2203");
				}
				break;
			}
		default:
			{
				SendERROR("9999","2200,2201,2203");
			}
		}
	}
	else 
	{
		SendERROR("9999","MAX IMPUT IS 5");
	}
}

void CApplication_Faro_TrakerDlg::STOP_DYN_MEAS(HANDLE MY_THREAD_FCT,CString IDFCT)
{
	int TEST = 1;
	for (int SIIN = 1 ; SIIN < Inparams.size() ; SIIN++)
	{
		if (Inparams[SIIN] != "0")
		{
			TEST = -1;
		}
	}
	if (TEST == 1)
	{
		imaad2 = -1;
		WaitForSingleObject(MY_THREAD_FCT,INFINITE);
		CloseHandle(MY_THREAD_FCT);
		memset(buffer,0,sizeof (buffer));
		strcpy_s(buffer,""+IDFCT+"STOPED]"); // Copie la chaine de caractère dans buffer
		nombre_de_caractere = send(id_de_la_nouvelle_socket,buffer,strlen(buffer),0);
	}
	else
	{
		memset(buffer,0,sizeof (buffer));
		strcpy_s(buffer,""+IDFCT+"STOP ERROR]"); // Copie la chaine de caractère dans buffer
		nombre_de_caractere = send(id_de_la_nouvelle_socket,buffer,strlen(buffer),0);
	}
}
void CApplication_Faro_TrakerDlg::RECV_DYN_MEAS(CString IDFCT,int STRT)
{
	int TOST = 1;
	for (int SIIN = 1 ; SIIN < Inparams.size() ; SIIN++)
	{
		if (Inparams[SIIN] != "0")
		{
			TOST = -1;	
		}
	}
	if(TOST == 1)
	{
		int sz = traject.size();
		CString isz;
		isz.Format("%d", (sz - STRT));
		memset(buffer,0,sizeof (buffer));
		strcpy_s(buffer,"#5532[START RCEV:"+isz+";points]"); // Copie la chaine de caractère dans buffer
		nombre_de_caractere = send(id_de_la_nouvelle_socket,buffer,strlen(buffer),0);
	}
	else
	{
		memset(buffer,0,sizeof (buffer));
		strcpy_s(buffer,""+IDFCT+"RECV ERROR]"); // Copie la chaine de caractère dans buffer
		nombre_de_caractere = send(id_de_la_nouvelle_socket,buffer,strlen(buffer),0);
	}
}
void CApplication_Faro_TrakerDlg::START_DYN_MEAS(CString PT_NAME,CString IDFCT,CString FCT_NUM,int STRT)
{
	int TIST = 1;
	for (int SIIN = 1 ; SIIN < Inparams.size() ; SIIN++)
	{
		if (Inparams[SIIN] != "0")
		{
			TIST = -1;	
		}
	}
	if(TIST == 1)
	{
		SendTramPtPosition(PT_NAME,FCT_NUM,STRT);
	}
	else
	{
		memset(buffer,0,sizeof (buffer));
		strcpy_s(buffer,""+IDFCT+"START ERROR]"); // Copie la chaine de caractère dans buffer
		nombre_de_caractere = send(id_de_la_nouvelle_socket,buffer,strlen(buffer),0);
	}	
}
LRESULT CApplication_Faro_TrakerDlg::OnMyMessage1(UINT wParam, LONG lParam)
{
    UpdateData(TRUE);
    return LRESULT(); // I handled this message
}
bool CApplication_Faro_TrakerDlg::CONVERT_BOOL(CString INDEX)
{
    if (INDEX == "0")
    {
        return false;
    }
    else if (INDEX == "1")
    {
        return true;
    }
}
double CApplication_Faro_TrakerDlg::GET_SMR_DIAMETRE(int D_SMR)
{
    switch (D_SMR)
    {
    case 0:
        {
            return 0.0127;
            break;
        }
    case 1:
        {
            return 0.022225;
            break;
        }
    default:
        {
            return 0.0381;
            break;
        }
    }
}
int CApplication_Faro_TrakerDlg::CloseWindow_TCP()
{
    erreur=shutdown(id_de_la_nouvelle_socket,2);
    if (erreur = 0)
    {
        return 0;
    }
    erreur=closesocket(id_de_la_nouvelle_socket);
    if (erreur = 0)
    {
        return 0;
    }
    erreur=closesocket(id_de_la_socket);
    if (erreur = 0)
    {
        return 0;
    }
    erreur=WSACleanup();
    if (erreur = 0)
    {
        return 0;
    }
    return 1;

}

void CApplication_Faro_TrakerDlg::OnBnClickedButtonClearList()
{
    m_listeofmeasur.ResetContent();
    // TODO : ajoutez ici le code de votre gestionnaire de notification de contrôle
}


void CApplication_Faro_TrakerDlg::OnBnClickedButtonDisconnectV7000()
{
    CloseHandle(m_ConnectThreadSND_RCV);
    int close = CloseWindow_TCP();
    if (close == 1)
    {
         m_List_V7000_Connexion.InsertString(0,"TCP Closed");
         m_List_V7000_Connexion.UpdateWindow();
    }
    else
    {
        m_List_V7000_Connexion.InsertString(0,"TCP Closed _ERROR");
        m_List_V7000_Connexion.UpdateWindow();
        AfxGetMainWnd() ->GetDlgItem(IDC_BUTTON_CONNECT_V7000)->EnableWindow(TRUE);
        AfxGetMainWnd() ->GetDlgItem(IDC_BUTTON_DISCONNECT_V7000)->EnableWindow(FALSE);
    }
    // TODO : ajoutez ici le code de votre gestionnaire de notification de contrôle
}
