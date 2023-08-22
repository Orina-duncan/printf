#include "main.h"
/**
*handle_print - Prints arguments based on their type
*@fmt: Formatted string in which to print the arguments.
*@list: List of arguments to be printed.
*@ind: ind.
*@buffer: Buffer array to handle print.
*@flags: active flags calculator
*@width: get width.
*@precision: Precision specification
*@size: Size specifier
*Return: 1 or 2;
*/
int handle_print(const char *fmt, int *ind, va_list list, char buffer[],
        int flags, int width, int precision, int size)
{
        int i, unknow_len = 0, printed_chars = -1;
        fmt_t fmt_types[] = {
                {'c', print_char}, {'s', print_string}, {'%', print_percent},
                {'i', print_int}, {'d', print_int}, {'b', print_binary},
                {'u', print_unsigned}, {'o', print_octal}, {'x', print_hexadecimal},
                {'X', print_hexa_upper}, {'p', print_pointer}, {'S', print_non_printable},
                {'r', print_reverse}, {'R', print_rot13string}, {'\0', NULL}
        };
        for (i = 0; fmt_types[i].fmt != '\0'; i++)
                if (fmt[*ind] == fmt_types[i].fmt)
                        return (fmt_types[i].fn(list, buffer, flags, width, precision, size));

        if (fmt_types[i].fmt == '\0')
        {
                if (fmt[*ind] == '\0')
                        return (-1);
                unknow_len += write(1, "%%", 1);
                if (fmt[*ind - 1] == ' ')
                        unknow_len += write(1, " ", 1);
                else if (width)
                {
                        --(*ind);
                        while (fmt[*ind] != ' ' && fmt[*ind] != '%')
                                --(*ind);
                        if (fmt[*ind] == ' ')
                                --(*ind);
                        return (1);
                }
                unknow_len += write(1, &fmt[*ind], 1);
                return (unknow_len);
        }
        return (printed_chars);
}


9.print.c

#include "main.h"

void print_buffer(char buffer[], int *buff_ind);

/**
*_printf - Printf function
*@format: format.
*Return: Printed chars.
*/
int _printf(const char *format, ...)
{
        int i, printed = 0, printed_chars = 0;
        int flags, width, precision, size, buff_ind = 0;
        va_list list;
        char buffer[BUFF_SIZE];

        if (format == NULL)
                return (-1);

        va_start(list, format);

        for (i = 0; format && format[i] != '\0'; i++)
        {
                if (format[i] != '%')
                {
                        buffer[buff_ind++] = format[i];
                        if (buff_ind == BUFF_SIZE)
                                print_buffer(buffer, &buff_ind);
                        /* write(1, &format[i], 1);*/
                        printed_chars++;
                }
                else
                {
                        print_buffer(buffer, &buff_ind);
                        flags = get_flags(format, &i);
                        width = get_width(format, &i, list);
                        precision = get_precision(format, &i, list);
                        size = get_size(format, &i);
                        ++i;
                        printed = handle_print(format, &i, list, buffer,
                                flags, width, precision, size);
                        if (printed == -1)
                                return (-1);
                        printed_chars += printed;
                }
        }

        print_buffer(buffer, &buff_ind);

        va_end(list);

        return (printed_chars);
}

/**
*print_buffer - will print the contents of the buffer if it exists
*@buffer: Array of chars
*@buff_ind: Index at which to add next char, it represents the length.
*/
void print_buffer(char buffer[], int *buff_ind)
{
        if (*buff_ind > 0)
                write(1, &buffer[0], *buff_ind);

        *buff_ind = 0;
}
