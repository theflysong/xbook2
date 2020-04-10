#include <stdarg.h>
#include <vsprintf.h>
#include <string.h>
#include <conio.h>
#include <sys/xcore.h>

/** 
 * printf - 格式化打印输出
 * @fmt: 格式以及字符串
 * @...: 可变参数
 */
int printf(const char *fmt, ...)
{
	//int i;
	char buf[STR_DEFAULT_LEN];
	va_list arg = (va_list)((char*)(&fmt) + 4); /*4是参数fmt所占堆栈中的大小*/
	vsprintf(buf, fmt, arg);
	
    /* 输出到控制台 */
    writeres(1, 0, buf, strlen(buf));
	return 0;
}

void logger(char *str)
{
	writeres(1, 0, str, strlen(str));
}
