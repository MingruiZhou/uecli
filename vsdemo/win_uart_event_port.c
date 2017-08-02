/**
 * @file     uart_event_port.c
 * @brief    �����շ��¼��Ĵ��ڵײ�����
 * @author   ��ʫ��
 * @par
 * (C) Copyright ���ݴ��ʿƼ����޹�˾
 * @version
 * 2017/07/28 ��ʫ�� �޶�˵��\n
 *
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <windows.h>
#include <tchar.h>
#include <assert.h>

#include "uecli_cfg.h"

// ********************************************************************************************
// �ڲ�����

// ���ھ������
HANDLE _hCom = NULL;
// ��ʼ���ʹ�
static bool _sopenBool = false;
// ���첽����
static OVERLAPPED _ro;
// д�첽����
static OVERLAPPED _wo;

// ********************************************************************************************
// �ӿں���

// ��ʼ��


/**
* \brief      �ײ��ʼ������
* \details    �ϲ�Ӧ�õ��øĺ�������ɵײ�ӿڵĳ�ʼ��
* \return     ������ɷ���true�����򷵻�false
*/
bool uecli_port_init(void)
{
    //ִ�н��
    bool result;
    BOOL fSuccess;
    //COM�˿��ַ���
    const char* comName = "COM4";
    DWORD dwErrorFlags;

    ////////////////////////////////////////////////////////
    //�򿪴���
    _hCom = CreateFileA(comName,        /* ���� */
        GENERIC_READ | GENERIC_WRITE,   /* �������д  */
        0,                              /* ��ռ��ʽ*/
        NULL,                           /* ��ȫ���� */
        OPEN_EXISTING,                  /* �򿪶����Ǵ���  */
        FILE_FLAG_OVERLAPPED,           /* �첽��ʽ  */
        NULL);
    if (INVALID_HANDLE_VALUE == _hCom || NULL == _hCom)
    {
        _hCom = NULL;
        UECLI_LOG("�򿪴����豸����%s����,������%d.", comName, GetLastError());
        return false;
    }

    // �����շ��¼�
    fSuccess = SetCommMask(_hCom, EV_RXCHAR);
    if (!fSuccess)
    {
        CloseHandle(_hCom);
        UECLI_LOG("�����¼�����ʱ����,������%d.", GetLastError());
        return false;
    }

    // �����첽��������
    _ro.Internal = 0;
    _ro.InternalHigh = 0;
    _ro.Offset = 0;
    _ro.OffsetHigh = 0;
    _ro.hEvent = CreateEvent(
        NULL,       // Ĭ�ϰ�ȫ����
        TRUE,       // �ֶ���λ����
        FALSE,      // ��ʼ��״̬-���ź�
        NULL);      // ���¼���

    _wo.Offset = 0;
    _wo.OffsetHigh = 0;
    _wo.hEvent = CreateEvent(
        NULL,       // Ĭ�ϰ�ȫ����
        TRUE,       // �ֶ���λ����
        FALSE,      // ��ʼ��״̬-���ź�
        NULL);      // ���¼���

    //���ô���
    DCB dcb;
    SecureZeroMemory(&dcb, sizeof(DCB));
    GetCommState(_hCom, &dcb);  // ��ȡ��ǰ����ֵ
    dcb.DCBlength = sizeof(DCB);
    dcb.BaudRate = CBR_115200;  // ������
    dcb.ByteSize = 8;           // ���ݳ���
    dcb.Parity = NOPARITY;      // У��λ
    dcb.StopBits = ONESTOPBIT;  // ֹͣλ
    if (!SetCommState(_hCom, &dcb))
    {
        CloseHandle(_hCom);
        _hCom = NULL;
        UECLI_LOG("��������DCB����,������%d.", GetLastError());
        return false;
    }
    SetupComm(_hCom, 2048, 2048);
    result = ClearCommError(_hCom, &dwErrorFlags, NULL);// ���ͨѶ����

//#if 0
    // ���ó�ʱ�ṹ
    COMMTIMEOUTS TimeOuts;
    GetCommTimeouts(_hCom, &TimeOuts);
    TimeOuts.ReadIntervalTimeout = 0;           /* �������ʱ */
    TimeOuts.ReadTotalTimeoutMultiplier = 0;    /* ����ʱϵ��ms */
    TimeOuts.ReadTotalTimeoutConstant = 0;   /* ����ʱ����ms */
    TimeOuts.WriteTotalTimeoutMultiplier = 0;   /* д��ʱϵ��ms */
    TimeOuts.WriteTotalTimeoutConstant = 0;     /* д��ʱ����ms */
    if (!SetCommTimeouts(_hCom, &TimeOuts)) return 0;
//#endif
    //��ʼ���ٽ���
    UECLI_LOG("�򿪶˿� %s �ɹ�", comName);
    return true;
}

/**
* \brief      ���øú����Է�������
* \param[in] buff   �������ڴ���
* \param[in] num    ������������
*/
void uecli_port_out(const void* buff, uint32_t num)
{
    // ���ͨѶ����
    DWORD dwErrorFlags;
    ClearCommError(_hCom, &dwErrorFlags, NULL); /* ���ͨѶ���� */
    ResetEvent(_wo.hEvent);
    WriteFile(_hCom, buff, num, (LPDWORD)&num, &_wo);    /* �������� */
    GetOverlappedResult(_hCom, &_wo, &dwErrorFlags, TRUE);
}

/**
* \brief      ���øú����Է�������
* \param[in] buff ������������ڴ���
* \param[in] maxnum ��������������
* \return     ����ʵ�ʶ�ȡ��������
*/
int uecli_port_in(void* buff, uint16_t maxnum)
{
    DWORD len = 0;
    DWORD dwEvtMask;
    DWORD dwErrorFlags;

    ClearCommError(_hCom, &dwErrorFlags, NULL); /* ���ͨѶ���� */
    ResetEvent(_ro.hEvent);
    if (!WaitCommEvent(_hCom, &dwEvtMask, &_ro))
    {
        dwEvtMask = GetLastError();
        if (ERROR_IO_PENDING == dwEvtMask)
        {
            if (WAIT_OBJECT_0 == WaitForSingleObject(_ro.hEvent, INFINITE))
            {
                ReadFile(_hCom, (LPVOID)buff, maxnum, &len, &_ro);
                GetOverlappedResult(_hCom, &_ro, &len, TRUE);
            }
        }
        else
        {
            printf("Wait failed with error %d.\n", GetLastError());
            assert(0);
        }
    }

    return len;
}
