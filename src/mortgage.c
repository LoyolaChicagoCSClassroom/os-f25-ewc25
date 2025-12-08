#include "mortgage.h"

#define SCALE 10000

static int remaining_balance(int principal, int rate, int years, int p)
{
    int months = years * 12;
    int balance = principal;

    int r = (rate * SCALE) / (12 * 100);

    for (int i = 0; i < months; i++)
    {
        balance = balance + (balance * r) / SCALE;
        balance -= p;
        if (balance <= 0)
            return 0;
    }
    return balance;
}

int mortgage_payment(int loan, int downpayment, int rate, int years)
{
    int principal = loan - downpayment;
    if (principal <= 0)
        return 0;

    if (rate == 0)
        return principal / (years * 12);

    int low = 0;
    int high = principal * 2;

    while (low < high)
    {
        int mid = (low + high) / 2;
        if (remaining_balance(principal, rate, years, mid) == 0)
            high = mid;
        else
            low = mid + 1;
    }

    return low;
}
