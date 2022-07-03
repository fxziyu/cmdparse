/**
 * \file      cmdparse.h
 * \brief     简单字符串命令解析,A simple command line parsing library
 * \author    鲁良(leonlu)
 * \version   V1.0
 * \date      2018/06/15
 * \note      每条命令必须以换行符结束,The command must end with a newline character
 * -------------------------------------------------------------------------------
 * AT指令
 *	    建议命令格式
 *		    执行: AT+CMD\r\n
 *		    查询: AT+CMD?\r\n
 *		    设置: AT+CMD=par1,pra2,par3,par4,par5,...\r\n
 *
 *	    建议响应格式
 *		    执行: AT+CMD=OK\r\n  or  AT+CMD=ERR\r\n
 *		    查询: AT+CMD=par1,par2,par3,...\r\n
 *		    设置: AT+CMD=OK\r\n  or  AT+CMD=ERR\r\n
 * ------------------------------------------------------------------------------
 * 简单命令行
 *     建议命令格式 cmd par1 par2 par3 ...\r\n
 *     建议响应格式 cmd ok\r\n  or  cmd err\r\n
 */
#include <stdio.h>
#include <string.h>
#include "cmdparse.h"

static const cmd_t * commands = NULL;				/**< 指令集 */
static int  cmd_echo_enable = 1;					/**< 指令回显使能 */

static char cmd_buff[CMD_BUFF_NUM][CMD_BUFF_LEN+1];	/**< 指令缓存 */
static int  cmd_push_cnt = 0;						/**< 指令缓存计数 */
static int  cmd_push_cur = 1;						/**< 指令缓存位置 */
static int  cmd_pull_cur = 0;						/**< 指令解析位置 */

/** 指令集加载，最后一条指令集必须为{NULL} */
void cmd_load(const cmd_t* cmds)
{
	commands = cmds;
}

/** 指令集卸载 */
void cmd_unload(void)
{
	commands = NULL;
}

/** 指令数据缓存，适合放在数据接收事件中，1成功/0失败 */
int cmd_push_char(char ch)
{
	if (cmd_push_cur == cmd_pull_cur)
	{
		/* buff已满，指令装不下，只能丢弃 */
		cmd_push_cnt = CMD_BUFF_LEN;
		return 0;
	}

	if (cmd_push_cnt == CMD_BUFF_LEN)
	{
		/* buff已满，指令装不下，只能丢弃 */
		if (ch == '\n') cmd_push_cnt = 0;
		return 0;
	}

	cmd_buff[cmd_push_cur][cmd_push_cnt++] = ch;
	if (ch == '\n')
	{
		cmd_buff[cmd_push_cur][cmd_push_cnt] = '\0';
		cmd_push_cnt = 0;
		if (++cmd_push_cur == CMD_BUFF_NUM)
			cmd_push_cur = 0;
	}

	return 1;
}

/** 指令数据缓存，适合放在数据接收事件中，1成功/0失败 */
int cmd_push(const char* data, int len)
{
	int i, ret = 1;
	for (i = 0; i < len; i++)
		ret &= cmd_push_char(data[i]);
	return ret;
}

/** 指令解析 */
void cmd_parse(void)
{
	int i, n;
	char *ret;
	while (NULL != commands)
	{
		n = cmd_pull_cur + 1;
		if (n == CMD_BUFF_NUM) n = 0;
		if (n == cmd_push_cur) return;
		cmd_pull_cur = n;
		for (i = 0; commands[i].cmd; i++)
		{
			ret = strstr(&cmd_buff[n][0], commands[i].cmd);
			if (ret)
			{
				if (cmd_echo_enable) printf("%s", ret);
				if (commands[i].func) commands[i].func(ret + strlen(commands[i].cmd));
				break;
			}
		}
	}
}

/** 指令回显设置 */
void cmd_echo(int enable)
{
	cmd_echo_enable = enable;
}

/* END OF FILE */
