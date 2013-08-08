#include <string.h>

#include "bank.h"

struct _BankAccount
{
	char *customer_name;
	float balance;
};

BankAccount *
bank_account_new (const char *customer_name)
{
	BankAccount *ret;

	ret = malloc (sizeof (BankAccount));

	ret->customer_name = strdup (customer_name);
	ret->balance = 0.0;

	return ret;
}

void
bank_account_free (BankAccount *acct)
{
	free (acct->customer_name);
	free (acct);
}

const char *
bank_account_customer (BankAccount *acct)
{
	return acct->customer_name;
}

float
bank_account_balance (BankAccount *acct)
{
	return acct->balance;
}

void
bank_account_deposit (BankAccount *acct,
		      float amt)
{
	acct->balance += amt;
}

int
bank_account_withdraw (BankAccount *acct,
		       float amt)
{
	if (amt <= acct->balance)
	{
		acct->balance -= amt;
		return 0;
	}
	else
	{
		return 1;
	}
}
