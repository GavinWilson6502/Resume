Personal Website
================

These are the source files for a website I made. The files themselves are from assignments
I completed for a web technologies course, though many aspects of the site were a matter
of personal initiative. Since I am no longer actively updating it, I elected to shut it
down and archive the project here.

Setup
-----

If you wish to see the website in action, you will need a server with nginx installed and
configured. The contents of this repository's `html` directory will then need to be placed
in nginx's public web root directory on your server.

Much of the site will require PHP to be installed and enabled as well.

Furthermore, a few pages are programmed to access a MySQL server. For proper
functionality, ensure MySQL Server is installed on the same machine as the web server.
Occurrences of passwords in the code have been replaced with the string `[redacted]`. They
will have to be replaced with the appropriate password for your own database system.

phpMyAdmin was used to manage and debug the SQL server through the website. As such, the
files present include `html/pma`, a symbolic link to `/usr/share/phpmyadmin/`. Ensure this
link points to the correct location for a browser to access phpMyAdmin's interface, or
remove it altogether if phpMyAdmin is not configured.

Alternatively, pages not requiring back-end services can be viewed locally by opening
their HTML files in a web browser, with their other files in the correct relative
locations.

### Version Information

This website was developed using Ubuntu 24.04.1 LTS with the following package versions:
- nginx 1.24.0-2ubuntu7.3
- php-fpm 2:8.3+93ubuntu2
- mysql-server 8.0.42-0ubuntu0.24.04.1
- phpmyadmin 4:5.2.1+dfsg-3

Webpage Explanations
--------------------

The website itself does not provide detailed descriptions of most of its pages, so
information will be provided here. Each page will be referred to herein by the descriptive
name the Home Page uses for it.

### Home Page (`html/index.html`)

The Home Page explains that the website hosts assignments for a web technologies course
and provides links to the pages created for each, grouped and ordered by assignment.

This page was created for Assignment #2, which required the creation of a home page that
provides an introduction to the site and links to every page of every assignment, using
only HTML. Accordingly, it links to itself under "Assignment #2." Since its creation, this
page has been continually updated for completeness.

### NES Programming (`html/hw01/index.html`)

The NES Programming page provides information on the subject of programming for the
Nintendo Entertainment System. It is structured loosely like a wiki page.

This page was created for Assignment #1, which required the creation of a page about a
topic that interests the student, using only HTML and demonstrating proficiency with
various elements.

### Company Home Page (`html/hw03/index.html`)

The Company Home Page is a home page for a fictional company called "Wing Cap, Inc." The
company bears similarities to multiple existing organizations, and their website, which
this page is imagined to be part of, betrays their comical defensiveness about their
unoriginality.

This page was created for Assignment #3, which required the creation of several webpages
for the fictional website of a fictional company made up by the student, including a home
page that briefly describes the company, its products, and its services, using only HTML.
Note that the assignment required that these pages not be styled, and that images not be
resized using HTML, so please excuse any awkward image sizing.

The Wing Cap, Inc. logo image used in this page is located at
`html/hw03/images/wingcapinc.png`.

### Product Page 1 (`html/hw03/products/wingcapos/index.html`)

Product Page 1 provides information on Wing Cap OS, one of the fictional products of the
fictional company Wing Cap, Inc. (see "Company Home Page").

This page was created for Assignment #3, which required the creation of several webpages
for the fictional website of a fictional company made up by the student, including a
product page for each of multiple fictional products, using only HTML. Note that the
assignment required that these pages not be styled, and that images not be resized using
HTML, so please excuse any awkward image sizing.

The Wing Cap OS logo image used in this page is located at
`html/hw03/images/wingcapos.png`.

### Product Page 2 (`html/hw03/products/mserieshw/index.html`)

Product Page 2 provides information on M Series Hardware, a fictional product line of the
fictional company Wing Cap, Inc. (see "Company Home Page").

This page was created for Assignment #3, which required the creation of several webpages
for the fictional website of a fictional company made up by the student, including a
product page for each of multiple fictional products, using only HTML. Note that the
assignment required that these pages not be styled, and that images not be resized using
HTML, so please excuse any awkward image sizing.

The M Series Hardware product image used in this page is located at
`html/hw03/images/mserieshw.png`.

### Service Page (`html/hw03/services/consulting/index.html`)

The Service Page provides information on the fictional consulting services of the
fictional company Wing Cap, Inc. (see "Company Home Page").

This page was created for Assignment #3, which required the creation of several webpages
for the fictional website of a fictional company made up by the student, including an
informational page about a fictional service, using only HTML. Note that the assignment
required that these pages not be styled, and that images not be resized using HTML, so
please excuse any awkward image sizing.

The consulting service clipart image used in this page is located at
`html/hw03/images/consulting.jpg`.

### Community College Homepage (`html/hw04/index.html`)

The Community College Homepage is a home page for the apparently fictional St. Huxley's
Community College.

This page was styled for Assignment #4, which required giving a provided webpage a
particular look by adding rules to its minimal CSS file without modifying any existing
code. Creative liberties were taken where allowed. Note that the only original code for
this page is that found below the line in `html/hw04/style.css` that reads
`/* WRITE YOUR CODE BELOW HERE */`.

The main stylesheet used in this page is located at `html/hw04/style.css`.  
The Liberation fonts used in this page and their license are located in
`html/hw04/fonts`.  
The external link icon image used in this page is located at
`html/hw04/external-link-52.png`.

### JavaScript Basics (`html/hw05/index.html`)

The JavaScript Basics page calculates the first 100 odd numbers, the first 100 Fibonacci
numbers, and the prime numbers less than 100, and displays them according to a particular
format.

This page was created for Assignment #5, which required the creation of a page that
behaves as described above using only JavaScript and minimal HTML.

The calculation and display script used in this page is located at `html/hw05/hw05.js`.

### JS Event Handling (`html/hw06/index.html`)

The JS Event Handling page prompts the user for a number of color segments, and then
generates a page with a "Hello, World!" message and several numbered buttons. The buttons
will each be a different color that can be represented with the user-provided color depth,
with there being exactly as many as there are possible colors. Each one will change the
"Hello, World!" message's background color to its own when clicked.

This page was created for Assignment #6, which required the creation and styling of a page
that behaves as described above using only JavaScript, minimal HTML, and CSS.

The main stylesheet used in this page is located at `html/hw06/hw06.css`.  
The event handling script used in this page is located at `html/hw06/hw06.js`.

### 15-Piece Puzzle (`html/hw07/index.html`)

The 15-Piece Puzzle page generates and shuffles a functioning 15-piece sliding tile puzzle
of *Starry Night* by Vincent Van Gogh.

Note: In the unlikely event that the page loads incorrectly at first, try reloading.

This page was created for Assignment #7, which required the creation of a page that
generates and shuffles a functioning 15-piece sliding tile puzzle of an image of the
student's choosing using only JavaScript, CSS, and minimal HTML.

The *Starry Night* image used in this page is located at `html/hw07/hw07.jpg`.  
The main stylesheet used in this page is located at `html/hw07/hw07.css`.  
The puzzle script used in this page is located at `html/hw07/hw07.js`.

### PHP Basics (`html/hw08/index.php`)

The PHP Basics page calculates the first 100 odd numbers, the first 20 Fibonacci numbers,
and the prime numbers less than 100, and displays them according to a particular format.

This page was created for Assignment #8, which required the creation of a page that
behaves as described above using only PHP.

### Server-Side Application (`html/hw09/index.php`)

The Server-Side Application page provides an interface to a server-side application that
generates, manipulates, and performs checks on sequences of numbers.

This page was created for Assignment #9, which required the creation of a server-side
application that behaves as described above using only PHP, and the styling of its
interface using CSS.

The main stylesheet used in this page is located at `html/hw09/style.css`.

### Client/Server Input Validation (`html/hw10/index.php`)

The Client/Server Input Validation page presents the user with a form and validates their
inputs. The page will indicate what is invalid, if anything, and will replace the form
with a confirmation message if all inputs are valid. 

This page was created for Assignment #10, which required the creation of a page, styled
using CSS, that validates form inputs on the client side using HTML and JavaScript, and
also on the server side using PHP.

The main stylesheet used in this page is located at `html/hw10/style.css`.  
The client-side validation script used in this page is located at
`html/hw10/validation.js`.

### Connecting to MySQL Server (`html/hw11/partone.php`)

The Connecting to MySQL Server page presents the contents of a database table consisting
of a fictional company's (see "Company Home Page") records of its fictional employees.

Note: Database not included.

This page was created for Assignment #11a, which required the setup of a MySQL server, and
the insertion of its password into a provided PHP file to create a page that retrieves
data from it. Note that this page does not contain original code.

### Customer Page (`html/hw11/index.php`)

The Customer Page allows the user to select one of a fictional company's (see "Company
Home Page") fictional customers and retrieve information about that customer from a
database (see "Connecting to MySQL Server"). It then allows the user to select and submit
one of the customer's orders, but no resulting behavior is implemented.

Note: Database not included.

This page was created for Assignment #11b, which required the modification of a provided
PHP file to present particular customer information from the database in a particular way
when a customer selection is submitted, resulting in the behavior described above, as well
as to display a banner and a company logo, styled using CSS, before and after a customer
is selected. Note that the only original code in `html/hw11/index.php` is that relating to
the stylesheet, banner, and logo in the `open_page` function, and most of the uncommented
code in the `generateCustomerPage` function.

The main stylesheet used in this page is located at `html/hw11/style.css`.  
The Wing Cap, Inc. logo image used in this page is located, redundantly, at
`html/hw11/logo.png`.

### Bringing It All Together (`html/hw12/index.php`)

The Bringing It All Together page presents the contents of a database table (see
"Connecting to MySQL Server") consisting of a fictional company's (see "Company Home
Page") records of its fictional employees, followed by a button that allows the user to
add a new entry to the table.

Note: Database not included.

This page was created for Assignment #12, which required the creation of a page identical
to the Connecting to MySQL Server page except that it includes the aforementioned button.
Note that the only original code for this page is the line immediately following the
comment in `html/hw12/index.php` that reads
`// [Add New Employee] button`.

### Add New Employee (`html/hw12/new.php`)

The Add New Employee page presents the user with a form that allows the addition of new
entries to a database table (see "Connecting to MySQL Server") consisting of a fictional
company's (see "Company Home Page") records of its fictional employees, and validates
their inputs.

Note: Database not included.

This page was created for Assignment #12, which required the creation of a page providing
the functionality described above using PHP on the server side, including for server-side
validation, and using HTML and AJAX for client-side validation.

The client-side AJAX script used in this page is located at `html/hw12/ajax.js`.  
The server-side AJAX script used by this page is located at `html/hw12/ajax.php`.

### Miscellaneous

`html/index.nginx-debian.html` was the default index page placed at the public web root
when nginx was installed.

`html/info.php` is, predictably, a simple info.php script used to test whether PHP was set
up correctly.

`html/pma` is a symbolic link created to provide access to phpMyAdmin's interface.
