#!/usr/bin/perl
# instock.pl - order confirmation page

use CGI 'param', 'escapeHTML', 'header';
use strict;

print header(-type=>'text/html',-expires=>0,-charset=>'utf-8');

print <<EOT;
<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN"
          "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en">
<head>
 <title>Order Confirmation</title>
 <link rel="stylesheet" type="text/css" href="style.css"/>
 <meta name="Author" content="Adam Hooper"/>
</head>
<body>
 <h1>Order Confirmation</h1>
EOT

# Returns hash of product ID => quantity
sub get_quantities {
	# The assignment asks that the user checks a checkbox *and* inserts a
	# quantity. Weird, but whatever. Only if the user does both of those
	# will this function return an element in the hash table.

	my @purchase = (); # wanted product IDs
	my %qty = ();      # wanted quantities, keyed by ID
	my %ret = ();      # qty, with *only* the IDs specified in @purchase

	foreach (param()) {
		/purchase_(\d+)/ && push @purchase, $1;
		/qty_(\d+)/ && ($qty{$1} = int(param($_)));
	}

	foreach (@purchase) {
		if (exists $qty{$_} && $qty{$_} gt 0) {
			$ret{$_} = $qty{$_};
		}
	}

	return %ret;
}

# Returns hash of product ID => (name, qty, price)
sub get_db {
	my %ret = ();

	open DB, '<inventory.txt' or die "Could not open inventory: $!\n";
	while (<DB>) {
		if (/(\d+),(.+),(\d+),([\d\.]+)/) {
			$ret{$1} = [$2, int($3), $4];
		}
	}
	close DB;

	return %ret;
}

# Returns the minimum value from the given arguments
sub min {
	my $ret = int(pop(@_));
	foreach (@_) {
		if (int($_) < $ret) {
			$ret = int($_);
		}
	}
	return $ret;
}

my %db;
my %qty = get_quantities();
my %unavailable = (); # Hash of product ID => number of unavailable items

# Exit if empty input
unless (%qty) {
	print <<EOT;
 <p>You did not request any products. Please <a href="catalog.html">go back</a>
    and select some.</p>
</body>
</html>
EOT
	exit;
}

%db = get_db();

print <<EOT;
 <table class="confirm">
  <tr>
   <th>Name</th>
   <th>Requested</th>
   <th>Available</th>
   <th>Unit</th>
   <th>Total</th>
  </tr>
EOT

# Print table rows
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
	my $qty_available = $qty{$id} = min($qty_in_stock, $qty_req);
	if ($qty_available != $qty_req) {
		$unavailable{$id} = $qty_req - $qty_available;
	}
	my $total_price = $unit_price * $qty_available;

	printf <<EOT, escapeHTML($name), $unit_price, $total_price;
  <tr>
   <td>%s</td>
   <td>$qty_req</td>
   <td>$qty_available</td>
   <td>\$%0.2f</td>
   <td>\$%0.2f</td>
  </tr>
EOT
}

print "</table>\n";

# Now print a list of unavailable items
if (%unavailable) {
	print <<EOT;
 <div class="unavailable">
  <p>The following items cannot be shipped:</p>
  <ul>
EOT
	while (my ($id, $qty) = each(%unavailable)) {
		printf "   <li>(%d) %s</li>\n", $qty, $db{$id}[0];
	}

	print "  </ul>\n";
	print " </div>\n";
}

# The rest of this file is the footer
print <<EOT;
 <div class="confirm">
  <form method="post" action="invoice.pl">
   <div style="display: none;">
EOT
# Pass through quantities
while (my ($i, $q) = each(%qty)) {
	print qq{  <input type="hidden" name="qty_$i" value="$q"/>\n};
}
print <<EOT;
   </div>
   <p>Please enter some personal information to complete your order:</p>
   <p>Name: <input type="text" name="name"/></p>
   <p>Credit Card Number: <input type="text" name="cc"/></p>
   <p><input type="submit" value="Confirm"/></p>
  </form>
 </div>
 <p>Alternatively, return to the <a href="catalog.html">catalog</a>.</p>
</body>
</html>
EOT
