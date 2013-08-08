/*
 * Generates catalog.html
 *
 * Anticipating the possibility of needing to automatically generate a catalog
 * page in a future assignment, I created this program to automatically
 * generate a catalog given a (hard-coded) list of products.
 */

#include <stdio.h>

typedef struct
{
	const char *code;
	const char *title;
	const char *description;
} Product;

static Product db[] =
{
	{
		"10231",
		"Love Potion Number 9",
		"So famous it's become a major motion picture! Love Potion "
			"Number 9 kicked off Potions Incorporated's success. "
			"Give it to your dream mate and you will never be "
			"lonely again!"
	},
	{
		"13234",
		"Laugh Potion",
		"Depressed? The laugh potion will assuage your despair. For "
			"best effect, administer our Laugh Potion to somebody "
			"who tries very hard to be constantly serious."
	},
	{
		"16231",
		"Constipation Clearer",
		"Cheap, quick, and effective. Be sure to stay home for at "
			"least an hour after administering."
	}
};
static int db_num_elements = sizeof (db) / sizeof (Product);

static void
print_header (void)
{
	/* XXX: Wrong content-type but IE balks on proper XHTML one */
	printf ("Content-type: text/html; charset=utf-8\n\n");

	printf ("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
		"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.1//EN\"\n"
		"          \"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\">\n"
		"<html xmlns=\"http://www.w3.org/1999/xhtml\""
			" xml:lang=\"en\">\n"
		"<head>\n"
		" <title>Product Catalog</title>\n"
		" <link rel=\"stylesheet\" type=\"text/css\""
			" href=\"style.css\"/>\n"
		" <meta name=\"Author\" content=\"Adam Hooper\"/>\n"
		"</head>\n"
		"<body>\n"
		" <h1>Potions Incorporated</h1>\n"
		" <p>Here is our product catalog. We are confident you will "
			"find something which satisfies.</p>\n"
		" <p>You may also return to <a href=\"index.html\">our front "
			"page</a>.</p>\n");
}

static void
print_table_header (void)
{
	printf (" <form method=\"get\" action=\"\">\n"
		"  <table>\n"
		"   <thead>\n"
		"    <tr>\n"
		"     <th>Purchase?</th>\n"
		"     <th>Description</th>\n"
		"     <th>Qty</th>\n"
		"   </thead>\n"
		"   <tbody>\n");
}

static void
print_table_footer (void)
{
	printf ("   </tbody>\n"
		"  </table>\n"
		"  <p>\n"
		"   <input type=\"submit\" value=\"Update Quantities\"/>\n"
		"   <input type=\"reset\" value=\"Reset Quantities\"/>\n"
		"  </p>\n"
		" </form>\n");
}

static void
print_product (const Product *product)
{
	/* XXX: Doesn't handle HTML entities */
	printf ("    <tr>\n"
		"     <td class=\"purchase\">\n"
		"      <input type=\"checkbox\" name=\"purchase_%s\"/>\n"
		"     </td>\n"
		"     <td class=\"description\">\n"
		"      <h2>%s</h2>\n"
		"      <img src=\"images/%s.png\" alt=\"%s\"/>\n"
		"      <p>%s</p>\n"
		"     </td>\n"
		"     <td class=\"qty\">\n"
		"      <input type=\"text\" size=\"5\" name=\"qty_%s\""
		         " value=\"0\"/>\n"
		"     </td>\n"
		"    </tr>\n",
		product->code,
		product->title,
		product->code,
		product->title,
		product->description,
		product->code);
}

static void
print_footer (void)
{
	printf (" </body>\n"
		"</html>");
}

static void
print_db (void)
{
	int i;

	for (i = 0; i < db_num_elements; i++)
	{
		print_product (&db[i]);
	}
}

int main (int argc, char **argv)
{
	print_header ();

	print_table_header ();

	print_db ();

	print_table_footer ();

	print_footer ();

	return 0;
}
