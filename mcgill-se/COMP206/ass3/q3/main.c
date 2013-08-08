#include <stdio.h>
#include <string.h>
#include "bank.h"

#define ACCOUNT_LIMIT 10
#define INPUT_BUFFER_SIZE 1024

static float
prompt_for_float (const char *prompt)
{
	char input[INPUT_BUFFER_SIZE];
	char *t;
	int valid;

	while (1)
	{
		printf (prompt);
		fflush (stdout);

		fgets (input, INPUT_BUFFER_SIZE, stdin);

		valid = 1;
		for (t = input; t < input + INPUT_BUFFER_SIZE; t++)
		{
			if (*t == 0) break;

			if (*t < 20)
			{
				*t = '\0';
				break;
			}

			if (*t != '.' && (*t < '0' || *t > '9'))
			{
				valid = 0;
				break;
			}
		}

		if (strlen (input) == 0)
		{
			valid = 0;
		}

		if (valid)
		{
			return (float) atof (input);
		}

		printf ("Please enter a number\n");
	}

	/* impossible */
	return 0.0;
}

static int
prompt_for_int (const char *prompt,
		int min,
		int max)
{
	char input[INPUT_BUFFER_SIZE];
	char *t;
	int valid;
	int ret;

	while (1)
	{
		printf (prompt);
		fflush (stdout);

		fgets (input, INPUT_BUFFER_SIZE, stdin);

		valid = 1;
		for (t = input; t < input + INPUT_BUFFER_SIZE; t++)
		{
			if (*t == 0) break;

			if (*t < 20)
			{
				*t = '\0';
				break;
			}

			if (*t == 'q' || *t == 'Q')
			{
				return -1;
			}

			if (*t < '0' || *t > '9')
			{
				valid = 0;
				break;
			}
		}

		if (strlen (input) == 0)
		{
			valid = 0;
		}

		if (valid)
		{
			ret = atoi (input);

			if (ret >= min && ret <= max)
			{
				return ret;
			}
		}

		printf ("Please enter a number from %d to %d\n", min, max);
	}

	/* impossible */
	return 0;
}

static int
prompt_for_account (void)
{
	return prompt_for_int ("Enter an account number to edit (0-9 or q): ",
			       0, ACCOUNT_LIMIT);
}

static BankAccount *
menu_create_account (void)
{
	char input[INPUT_BUFFER_SIZE];
	char *t;

	printf ("Enter the customer name for this account: ");
	fflush (stdout);

	fgets (input, INPUT_BUFFER_SIZE, stdin);

	/* Put '\0' in place of whitespace */
	for (t = input; t < input + INPUT_BUFFER_SIZE; t++)
	{
		if (*t == 0 || *t == '\n' || *t == '\r') break;
	}
	*t = '\0';

	return bank_account_new (input);
}

static void
menu_deposit (BankAccount *account)
{
	float amt;

	amt = prompt_for_float ("Enter an amount to deposit: ");

	bank_account_deposit (account, amt);
}

static void
menu_withdraw (BankAccount *account)
{
	float amt;

	amt = prompt_for_float ("Enter an amount to withdraw: ");

	if (bank_account_withdraw (account, amt) != 0)
	{
		printf ("Insufficient funds!\n");
	}
	else
	{
		printf ("You just withdrew $%0.2f\n", amt);
	}
}

static void
menu_for_account (BankAccount *account)
{
	int input;

	while (1)
	{
		printf ("=== Account Menu ===\n"
			"Customer name: %s\n"
			"[1] Display balance\n"
			"[2] Deposit\n"
			"[3] Withdraw\n",
			bank_account_customer (account));

		input = prompt_for_int ("Enter an option (1-3 or q): ", 1, 3);

		switch (input)
		{
			case 1:
				printf ("Balance: $%0.2f\n",
					bank_account_balance (account));
				break;
			case 2:
				menu_deposit (account);
				break;
			case 3:
				menu_withdraw (account);
				break;
			case -1:
				return;
		}
	}
}

static void
print_account (BankAccount *account)
{
	printf ("Account for %s (balance: $%0.2f)\n",
		bank_account_customer (account),
		bank_account_balance (account));
}

static void
print_accounts (BankAccount *accounts[])
{
	int i;
	int printed_header = 0;

	for (i = 0; i < ACCOUNT_LIMIT; i++)
	{
		if (accounts[i] != NULL)
		{
			if (printed_header == 0)
			{
				printf ("List of accounts:\n");
				printed_header = 1;
			}

			printf (" [%d] ", i);
			print_account (accounts[i]);
		}
	}
}

static void
main_menu (BankAccount *accounts[])
{
	int choice;

	while (1)
	{
		printf ("=== Main Menu ===\n");

		print_accounts (accounts);

		choice = prompt_for_account ();

		if (choice == -1) return;

		if (accounts[choice] == NULL)
		{
			accounts[choice] = menu_create_account ();
		}

		menu_for_account (accounts[choice]);
	}

	/* impossible */
	return;
}

int
main (int argc, char **argv)
{
	int i;

	BankAccount *accounts[ACCOUNT_LIMIT];

	for (i = 0; i < ACCOUNT_LIMIT; i++)
	{
		accounts[i] = NULL;
	}

	main_menu (accounts);

	for (i = 0; i < ACCOUNT_LIMIT; i++)
	{
		if (accounts[i] != NULL)
		{
			bank_account_free (accounts[i]);
		}
	}

	return 0;
}
