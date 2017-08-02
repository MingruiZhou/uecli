#ifndef _U_E_CLI_VT100_H_
#define _U_E_CLI_VT100_H_

// VT100���ⰴ��
#define UEVT100_KEY_UP      "\033[A"
#define UEVT100_KEY_DOWN    "\033[B"
#define UEVT100_KEY_RIGHT   "\033[C"
#define UEVT100_KEY_LEFT    "\033[D"
#define UEVT100_KEY_ESC     "\033"
#define UEVT100_KEY_F1      "\033OP"
#define UEVT100_KEY_F2      "\033OQ"
#define UEVT100_KEY_F3      "\033OR"
#define UEVT100_KEY_F4      "\033OS"

// ��ɫ����
#define UEVT100_COLOR_BLACK         "0"
#define UEVT100_COLOR_RED           "1"
#define UEVT100_COLOR_GREEN         "2"
#define UEVT100_COLOR_YELLOW        "3"
#define UEVT100_COLOR_BLUE          "4"
#define UEVT100_COLOR_MAGENTA       "5"
#define UEVT100_COLOR_CYAN          "6"
#define UEVT100_COLOR_WHITE         "7"

// ����

// ����������ɫ
#define UETV100_TXTCOLOR(aCOLOR)   "\033[3" aCOLOR "m"
// �������ֱ���ɫ
#define UETV100_BKCOLOR(aCOLOR)    "\033[4" aCOLOR "m"
// ���������Ļ�����ص�ԭ��
#define UETV100_CLEARS()    "\033[2J\033[0;0H"

#endif