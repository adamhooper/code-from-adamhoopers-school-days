#include <stdlib.h>

#ifndef BANK_H
#define BANK_H

typedef struct _BankAccount BankAccount;

BankAccount	*bank_account_new	(const char *customer_name);

void		 bank_account_free	(BankAccount *acct);

const char	*bank_account_customer	(BankAccount *acct);

float		 bank_account_balance	(BankAccount *acct);

void		 bank_account_deposit	(BankAccount *acct,
					 float amt);

int		 bank_account_withdraw	(BankAccount *acct,
					 float amt);

#endif /* BANK_H */
