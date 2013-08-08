#!/usr/bin/perl
#
# invoice.pl - invoice page (completes order and displays the invoice)
#
# This script assumes all will go according to plan -- it has lousy error
# handling. Also, it doesn't redirect after the POST, so refreshing the page
# will remove MORE items from the database. I've got tons of work to do and the
# finishing touches won't get me any more marks, so... screw it :).

use strict;
use CGI 'param', 'escapeHTML', 'header';

sub deduct {
	my ($id, $qty) = @_;
	my $new_db = "";

	open(DB, '<inventory.txt') or die("Could not open inventory: $!\n");
	while (<DB>) {
		if (/($id,.*),(\d+),(.*)/) {
			$_ = "$1," . ($2 - $qty) . ",$3\n";
		}
		$new_db .= $_;
	}
	close DB;

	open(DB, '>inventory.txt') or die("Could not write inventory: $!\n");
	print DB $new_db;
	close DB;
}

# Returns hash of product ID => quantity
sub get_quantities {
	# Unlike in instock.pl, we don't need to deal with checkboxes any more.

	my %ret = ();

	foreach (param()) {
		/qty_(\d+)/ && ($ret{$1} = int(param($_)));
	}

	return %ret;
}

# Returns hash of product ID => (name, qty, price)
# Copied from instock.pl
sub get_db {
	my %ret = ();

	open DB, '<inventory.txt';
	while (<DB>) {
		if (/(\d+),(.+),(\d+),([\d\.]+)/) {
			$ret{$1} = [$2, int($3), $4];
		}
	}
	close DB;

	return %ret;
}

print header(-type=>'text/html',-expires=>0,-charset=>'utf-8');

printf <<EOT, escapeHTML(param('name')), escapeHTML(param('cc'));
<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN"
          "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en">
<head>
 <title>Potions Incorporated: Your Bill</title>
 <link rel="stylesheet" type="text/css" href="style.css"/>
 <meta name="Author" content="Adam Hooper"/>
</head>
<body>
 <h1>Potions Incorporated: Your Bill</h1>
 <h2>Name: %s</h2>
 <h3>Credit Card: %s</h3>
EOT

my %qty = get_quantities();

while (my ($i, $q) = each(%qty)) {
	deduct($i, $q);
}

# Okay, the rest of this file is pretty much copy/pasted from instock.pl...

my %db = get_db();

print <<EOT;
 <table class="confirm">
  <tr>
   <th>Name</th>
   <th>Qty</th>
   <th>Total</th>
  </tr>
EOT

while (my ($id, $qty_req) = each(%qty)) {
	unless (exists $db{$id}) {
		print <<EOT;
 </table>
 <p>The product with id &quot;$id&quot; does not exist.
    Please <a href="catalog.html">go back</a> and select real products.</p>
</body>
</html>
EOT
		exit;
	}

	my ($name, $qty_in_stock, $unit_price) = @{$db{$id}};
	my $total_price = $unit_price * $qty_req;

	printf <<EOT, escapeHTML($name), $total_price;
  <tr>
   <td>%s</td>
   <td>$qty_req</td>
   <td>\$%0.2f</td>
  </tr>
EOT
}

print <<EOT;
 </table>
 <p>You can now return to our <a href="index.html">home page</a>.</p>
</body>
</html>
EOT
