/**
 * \file      main.h
 * \brief     简单命令行演示DEMO,simple command line demo
 * \author    鲁良(leonlu)
 * \version   V1.0
 * \date      2018/06/15
 * \note      每条命令必须以换行符结束,The command must end with a newline character
 *            建议命令格式 cmd par1 par2 par3 ...\r\n
 *            建议响应格式 cmd ok\r\n  or  cmd err\r\n
 */
#include <stdio.h>
#include <string.h>
#include "cmdparse.h"

static void cmd_version(const char* arg)
{
    printf("VERSION cmd_parse_v1.0.0\r\n");
}

static void cmd_echo_set(const char* arg)
{
    int enable;
    if (1 == sscanf(arg, "%d", &enable))
    {
        cmd_echo(enable);
        printf("ECHO OK\r\n");
    }
    else printf("ECHO ERR\r\n");
}

static void cmd_set1(const char* arg)
{
    int val1, val2;
    float val3;
    if (3 == sscanf(arg, "%d %d %f", &val1, &val2, &val3))
        printf("SET1 OK (%d, %d, %0.2f)\r\n", val1, val2, val3);
    else printf("SET1 ERR\r\n");
}

static void cmd_set2(const char* arg)
{
    int val;
    char str[64];
    if (2 == sscanf(arg, "%s %d", str, &val))
        printf("SET2 OK (%s, %d)\r\n", str, val);
    else printf("SET2 ERR\r\n");
}

/** 指令集，最后一条指令集必须为{NULL} */
static const cmd_t commands[] = {

    {"VERSION",     cmd_version,    "版本查询" },
    {"ECHO",        cmd_echo_set,   "指令回显设置" },
    {"SET1",        cmd_set1,       "设置指令示例1：(设置3个参数：无符号整数、整数、小数) AT+SET1=1,-2,0.3" },
    {"SET2",        cmd_set2,       "设置指令示例2：(设置2个参数：字符串，数字) AT+SET1=abc,123" },

    { NULL }
};

int main(int argc, char* argv[])
{
    char commands1[] = "VERSION\r\n SET1 1 -2 0.3\r\n SET2 abc 123\r\n ECHO 0\r\n ";
    char commands2[] = "VERSION\r\n SET1 123 -456 3.14\r\n SET2 hello,world 123456789\r\n";

    /** 简单命令行指令集加载 */
    cmd_load(commands);

    printf("简单命令行测试1\r\n");
    /* 指令数据批量缓存 */
    cmd_push(commands1, strlen(commands1));
    /* 指令解析 */ 
    cmd_parse();

    printf("-----------\r\n");

    printf("简单命令行测试2\r\n");
    /* 模拟指令数据单个单个输入缓存，如单片机串口中断单个单个接收处理 */
    for (int i = 0; i < strlen(commands2); i++)
        cmd_push_char(commands2[i]);
    cmd_parse();

    return 1;
}
