#include "shell.h"
#include "rprintf.h"
#include "input.h"
#include "mortgage.h"

#define SHELL_BUF 128

static int str_eq(const char *a, const char *b)
{
    while (*a && *b)
    {
        if (*a != *b) return 0;
        a++; b++;
    }
    return *a == 0 && *b == 0;
}

static void cmd_help(void)
{
    esp_printf(kputc, "Commands:\n");
    esp_printf(kputc, "  help      - show this help\n");
    esp_printf(kputc, "  clear     - clear screen\n");
    esp_printf(kputc, "  mortgage  - run mortgage calculator\n");
}

static void cmd_clear(void)
{
    for (int i = 0; i < 25; i++)
        esp_printf(kputc, "\n");
}

static void cmd_mortgage(void)
{
    char loan_s[32];
    char down_s[32];
    char rate_s[32];
    char years_s[32];

    esp_printf(kputc, "=== Mortgage Calculator ===\n");

    esp_printf(kputc, "Loan amount: ");
    get_line(loan_s, 32);

    esp_printf(kputc, "Down payment: ");
    get_line(down_s, 32);

    esp_printf(kputc, "Interest rate (whole): ");
    get_line(rate_s, 32);

    esp_printf(kputc, "Years: ");
    get_line(years_s, 32);

    int loan = to_int(loan_s);
    int down = to_int(down_s);
    int rate = to_int(rate_s);
    int years = to_int(years_s);

    int payment = mortgage_payment(loan, down, rate, years);

    esp_printf(kputc, "\nMonthly payment: $%d\n", payment);
}

void shell_run(void)
{
    char buf[SHELL_BUF];

    

    while (1)
    {
        esp_printf(kputc, "> ");
        get_line(buf, SHELL_BUF);

        if (str_eq(buf, "help"))
            cmd_help();
        else if (str_eq(buf, "clear"))
            cmd_clear();
        else if (str_eq(buf, "mortgage"))
            cmd_mortgage();
        else if (buf[0] != 0)
            esp_printf(kputc, "Unknown command: %s\n", buf);
    }
}
