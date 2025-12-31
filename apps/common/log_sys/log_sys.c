#include "log_sys.h"
#include "zb_common.h"
#include "../proj/os/ev_poll.h"
#include "string.h"
#include "tl_common.h"
#include "../drivers/drv_uptime.h"
#include "../drivers/drv_putchar.h"

typedef char *va_list;

#define _INTSIZEOF(n)   	( (sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1) )

#define va_start(ap,v)  	( ap = (va_list)&v + _INTSIZEOF(v) )
#define va_arg(ap,t)    	( *(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)) )
#define va_end(ap)      	( ap = (va_list)0 )

#define	DECIMAL_OUTPUT		10
#define	OCTAL_OUTPUT		8
#define	HEX_OUTPUT			16


static void buf_putchar(char **buf, char c)
{
    **buf = c;
    (*buf)++;
}

static void buf_puts(char **buf, const char *s)
{
    while (*s) {
        buf_putchar(buf, *s++);
    }
}

static void buf_puti(char **buf, unsigned long val, int base)
{
    char tmp[16];
    int i = 0;

    if (val == 0) {
        buf_putchar(buf, '0');
        return;
    }

    while (val > 0) {
        int digit = val % base;
        tmp[i++] = (digit < 10) ? ('0' + digit) : ('a' + digit - 10);
        val /= base;
    }

    while (i--) {
        buf_putchar(buf, tmp[i]);
    }
}

int user_vsprintf(char *buf, const char *format, va_list ap)
{
    char *p = buf;
    char ch;
    long m;
    unsigned long u;
    char *s;

    while ((ch = *format++)) {
        if (ch != '%') {
            buf_putchar(&p, ch);
        } else {
            ch = *format++;
            switch (ch) {
            case 'c':
                buf_putchar(&p, va_arg(ap, int));
                break;

            case 'd':
                m = va_arg(ap, int);
                if (m < 0) {
                    buf_putchar(&p, '-');
                    m = -m;
                }
                buf_puti(&p, m, 10);
                break;

            case 's':
                s = va_arg(ap, char *);
                buf_puts(&p, s ? s : "(null)");
                break;

            case 'o':
                u = va_arg(ap, unsigned int);
                buf_puti(&p, u, 8);
                break;

            case 'x':
                u = va_arg(ap, unsigned int);
                buf_puti(&p, u, 16);
                break;

            case '%':
                buf_putchar(&p, '%');
                break;

            default:
                buf_putchar(&p, ch);
                break;
            }
        }
    }

    *p = '\0';   // kết thúc chuỗi
    return (p - buf);   // số ký tự ghi
}

int user_sprintf(char *buf, const char *format, ...)
{
    va_list ap;
    int len;

    va_start(ap, format);
    len = user_vsprintf(buf, format, ap);
    va_end(ap);

    return len;
}

char buf_temp[256];

void log_output(const char *level, const char *fmt, ...)
{
    va_list ap;
    char *p = buf_temp;

    /* ---- prefix: [time][level]: ---- */
    p += user_sprintf(p,
                    "[%d ms][%s]: ",
                    (unsigned long)drv_uptime_get_ms(),
                    level);

    /* ---- log content ---- */
    va_start(ap, fmt);
    p += user_vsprintf(p, fmt, ap);
    va_end(ap);

    /* ---- newline ---- */
    p += user_sprintf(p, "\r\n");
    /* ---- output ---- */
    printf("%s", buf_temp);
}


void format_hex_string(char *out, const unsigned char *data, unsigned int len)
{
    for (unsigned int i = 0; i < len; i++) {
        unsigned char byte = data[i];
        char high = (byte >> 4) & 0x0F;
        char low  = byte & 0x0F;
        *out++ = (high < 10) ? ('0' + high) : ('A' + high - 10);
        *out++ = (low  < 10) ? ('0' + low)  : ('A' + low  - 10);
        *out++ = ' ';
    }
    *out = '\0';
}

void log_init(){
	LOGI("Start Program...............");
	LOGI("MAC: ");
	LOGD_HEX(((u8 *)&g_zbMacPib.extAddress),8);
}

