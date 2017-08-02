
#include <stdio.h>
#include "uecli.h"

int uecli_port_in(void* buff, uint16_t maxnum);

void testfun(int argc, char** argv)
{
    uecli_PrintString("hellptest\n");

    argc = argc;
    argv = argv;
}
void testprtd(int argc, char** argv)
{
    if (argc > 2)
    {
        int valu = 0;
        if (estrtoi(argv[2], &valu))
            eprintf(argv[1], valu);
        else
            eprintf("��������ȷ�Ĳ���\r\n");
    }
    argc = argc;
    argv = argv;
}

void menufun(int argc, char** argv)
{
    uecli_PrintString("testmenu\n");
}

const uecli_Handle submenu[] =
{
    UECLI_DECLARE_COMMAND(menufun,"testmenu", "�Ӳ˵�����",""),
    UECLI_ITEM_END()
};


const uecli_Handle handtalbe[]=
{
    UECLI_DECLARE_COMMAND(testfun,"test", "һ�����Ժ���"),
    UECLI_DECLARE_COMMAND(testfun,"testprint", "һ�����Ժ���"),
    UECLI_DECLARE_SUBMENU(submenu,"menu1", "�Ӳ˵�����"),
    UECLI_ITEM_END()
};

int main(void)
{
    char buff[1024];
    int len;

    uecli_Initialize(handtalbe);

    for (;;)
    {
        len = uecli_port_in(buff, 1024);	/* ��ȡ���� */
        uecli_ProcessRecChar(buff, len);
    }
}


