/**
 * @file     uecli.h
 * @brief    �ĵ�ģ������˵�� 
 * @author   ��ʫ��
 * @par
 * (C) Copyright ���ݴ��ʿƼ����޹�˾
 * @version
 * 2017/07/24 ��ʫ�� �޶�˵��\n
 *
 */ 

#ifndef _U_E_CLI_H_
#define _U_E_CLI_H_

// ********************************************************************************************
// ͷ�ļ�

#include <stdint.h>
#include <stdbool.h>
#include "uecli_cfg.h"

// ********************************************************************************************
// ���Ͷ���

/**
 * @brief ����˵�����Ŀ����.  
 */
typedef enum
{
    UECLI_TYPE_FUN = 0,     /*!< ����ĿΪһ�������Ե����� */
    UECLI_TYPE_SUBMENU      /*!< ����ĿΪ�Ӳ˵� */
}uecli_ItemType;

/**
 * @brief �����б����Ͷ���.
 *
 * �û����붨�������б����飬���ڳ�ʼ��ʱ���ø�����
 * ����Ϊ����֧�ֵĸ���������б�
 */
typedef struct
{
    const void* pdata;          /*!< ָ����������Ӳ˵�����������ָ�� */
    uecli_ItemType itemType;    /*!< ��ʾ��ǰ��Ŀ���ͣ�����or�Ӳ˵� */
    const char* exename;        /*!< ����ƥ��ִ������������ַ��� */
    const char* desc;           /*!< ��ʾ��help�����е�˵���ַ��� */
#if UECLI_CFG_LONG_HELP>0
    const char* helpstr;        /*!< ʹ��help+����ʱ��ʾ�ĳ�������Ϣ */
#endif
}uecli_Handle;

/**
* @brief ����������.
*/
#if UECLI_CFG_LONG_HELP>0     
#define UECLI_DECLARE_COMMAND(PDATA, EXENAME, HELPSTR, ...) \
    {(void*)(long)PDATA, UECLI_TYPE_FUN, EXENAME, HELPSTR, ##__VA_ARGS__}

#define UECLI_DECLARE_SUBMENU(PDATA, MENUNAME, HELPSTR) \
    {(void*)(long)PDATA, UECLI_TYPE_SUBMENU, MENUNAME, HELPSTR, 0}
#else
#define UECLI_DECLARE_COMMAND(PDATA, EXENAME, HELPSTR, ...) \
    {(void*)(long)PDATA, UECLI_TYPE_FUN, EXENAME, HELPSTR}

#define UECLI_DECLARE_SUBMENU(PDATA, MENUNAME, HELPSTR) \
    {(void*)(long)PDATA, UECLI_TYPE_SUBMENU, MENUNAME, HELPSTR}
#endif

/**
* @brief �����б��β.
*/
#define UECLI_ITEM_END() {0}

// ********************************************************************************************
// �ӿں���

//����C C++��ϱ��
#ifdef __cplusplus
extern "C" {
#endif

void uecli_Initialize(const uecli_Handle* phand);
void uecli_ProcessRecChar(const char recchar[], int len);
void uecli_PrintString(const char* str);

//����C C++��ϱ��
#ifdef __cplusplus
}
#endif

#endif