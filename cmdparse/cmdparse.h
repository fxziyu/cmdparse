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
#ifndef __CMD_PAESE_H
#define __CMD_PAESE_H

#ifndef NULL
#define NULL	(void)0
#endif

#define CMD_BUFF_NUM	10					/**< 指令缓存数量 */
#define CMD_BUFF_LEN	128					/**< 指令最大长度 */

/** 指令集定义 */
typedef struct {
	char* cmd;								/**< 指令 */
	void (*func)(const char* arg);			/**< 处理函数 */
	const char* desc;						/**< 指令描述 */
}cmd_t;

void cmd_load(const cmd_t *cmds);			/**< 指令集加载，最后一条指令集必须为{NULL} */
void cmd_unload(void);						/**< 指令集卸载 */

int  cmd_push_char(char ch);				/**< 指令数据缓存，适合放在数据接收事件中，1成功/0失败 */
int  cmd_push(const char *data, int len);	/**< 指令数据缓存，适合放在数据接收事件中，1成功/0失败 */

void cmd_parse(void);						/**< 指令解析，适合放在主循环或定时处理中 */

void cmd_echo(int enable);					/**< 指令回显设置 */

#endif

/* END OF FILE */
