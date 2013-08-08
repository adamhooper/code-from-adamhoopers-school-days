#!/usr/bin/perl
#
# viewinv.pl - view inventory
#
# Basically, translates inventory.txt into a pretty web page

use CGI 'header', 'escapeHTML';
use strict;

print header(-type=>'text/html',-expires=>0,-charset=>'utf-8');

print <<EOT;
<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN"
          "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en">
<head>
 <title>Inventory</title>
 <link rel="stylesheet" type="text/css" href="style.css"/>
 <meta name="Author" content="Adam Hooper"/>
</head>
<body>
 <h1>Inventory</h1>
 <table class="inventory">
  <tr>
   <th>Code</th>
   <th>Name</th>
   <th>In Stock</th>
   <th>Price</th>
  </tr>
EOT

open DB, '<inventory.txt';
while (<DB>) {
	if (/(\d+),(.+),(\d+),([\d\.]+)/) {
		printf <<EOT, $1, escapeHTML($2), $3, $4;
  <tr>
   <td>%d</td>
   <td>%s</td>
   <td>%d</td>
   <td>%0.2f</td>
  </tr>
EOT
	}
}
close DB;

print <<EOT;
 </table>
 <p>You may return to the <a href="index.html">home page</a>.</p>
</body>
</html>
EOT
