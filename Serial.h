#ifndef SERIAL_H
#define  SERIAL_H
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

using namespace std;
enum {
  Com2_Timeout = 1000,               // [msec]
  Com2_EachTimeout = 2,              // [msec]
  Com2_LineLength = 64 + 3 + 1 + 1 + 1 + 16,
};

static HANDLE HCom2 = INVALID_HANDLE_VALUE;//用于和主控制器进行通信
static int Com2_ReadableSize = 0;
static char* Com2_ErrorMessage = "no error.";
static char message_buffer[Com2_LineLength];
static char receive_buffer[Com2_LineLength];

static int Com2_SendBufferSize = 64;
static char Com2_SendBuffer[64];
static int  Com2_SendLength = 0;



// Com2_delay
static void Com2_delay(int msec)
{
  Sleep(msec);
}

//改变波特率
static int com2_changeBaudrate(long baudrate)
{
  DCB dcb;

  GetCommState(HCom2, &dcb);
  dcb.BaudRate = baudrate;
  dcb.ByteSize = 8;
  dcb.Parity = NOPARITY;
  dcb.fParity = FALSE;
  dcb.StopBits = ONESTOPBIT;
  SetCommState(HCom2, &dcb);
  return 0;
}


// Serial transceiver
static int com2_connect(const char* device, long baudrate)
{

  char adjust_device[16];
  _snprintf(adjust_device, 16, "\\\\.\\%s", device);
  HCom2 = CreateFileA(adjust_device, GENERIC_READ | GENERIC_WRITE, 0,
                     NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

  if (HCom2 == INVALID_HANDLE_VALUE) {
    return -1;
  }

  // Baud rate setting
  return com2_changeBaudrate(baudrate);
}


static void com2_disconnect(void)
{
  if (HCom2 != INVALID_HANDLE_VALUE) {
    CloseHandle(HCom2);
    HCom2 = INVALID_HANDLE_VALUE;
  }
}

//串口发送数据
static int com2_send(const char* data, int size)
{
  DWORD n;
  WriteFile(HCom2, data, size, &n, NULL);
  return n;
}

//串口接收数据
static int com2_recv(char* data, int max_size, int Com2_Timeout)
{
  if (max_size <= 0) {
    return 0;
  }

  if (Com2_ReadableSize < max_size) {
    DWORD dwErrors;
    COMSTAT ComStat;
    ClearCommError(HCom2, &dwErrors, &ComStat);
    Com2_ReadableSize = ComStat.cbInQue;
  }

  if (max_size > Com2_ReadableSize) {
    COMMTIMEOUTS pcto;
    int each_timeout = 2;

    if (Com2_Timeout == 0) {
      max_size = Com2_ReadableSize;

    } else {
      if (Com2_Timeout < 0) {
        /* If Com2_Timeout is 0, this function wait data infinity */
        Com2_Timeout = 0;
        each_timeout = 0;
      }

      /* set Com2_Timeout */
      GetCommTimeouts(HCom2, &pcto);
      pcto.ReadIntervalTimeout = Com2_Timeout;
      pcto.ReadTotalTimeoutMultiplier = each_timeout;
      pcto.ReadTotalTimeoutConstant = Com2_Timeout;
      SetCommTimeouts(HCom2, &pcto);
    }
  }

  DWORD n;
  ReadFile(HCom2, data, (DWORD)max_size, &n, NULL);

  if (n > 0) {
    Com2_ReadableSize -= n;
  }

  return n;
}

static int Com2_checkSum(char buffer[], int size, char actual_sum)
{
  char expected_sum = 0x00;
  int i;

  for (i = 0; i < size; ++i) {
    expected_sum += buffer[i];
  }
  expected_sum = (expected_sum & 0x3f) + 0x30;

  return (expected_sum == actual_sum) ? 0 : -1;
}

static int Serial_main()
{
  // 串口号
  // 设置波特率
  const char com_port[] = "COM9";
  const long com_baudrate = 115200;
 

  if (com2_connect(com_port, com_baudrate) < 0) 
  {
	  _snprintf(message_buffer, Com2_LineLength,
		  "Cannot connect COM device: %s", com_port);
	  Com2_ErrorMessage = message_buffer;
	  printf(Com2_ErrorMessage);
	  return -1;
  }

  _snprintf(message_buffer, Com2_LineLength,
	  "sending\n");
  for (int i = 0; i <100;i++)
  {
	  com2_send(message_buffer,Com2_LineLength);
	  com2_recv(receive_buffer,Com2_LineLength,Com2_Timeout);
	  printf("receive: %s\n",receive_buffer);
  }
  com2_disconnect();
  printf("end.\n");
  // 任意键结束
  getchar();
  return 0;
}

#endif
