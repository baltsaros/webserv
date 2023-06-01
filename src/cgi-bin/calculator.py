#!/bin/python

import cgi, cgitb

form = cgi.FieldStorage()

error = False
number1 = form.getValue("num1")
number2 = form.getValue("num2")
operator = form.getValue("operator")

if operator == "+":
	result = number1 + number2
elif operator == "-":
	result = number1 - number2
elif operator == "/":
	if (number2 == 0):
		error = True
	else:
		result = number1 / number2
elif operator == "*":
	result = number1 * number2
else:
	error = True

if error:
	print "Content-type:text/html\r\n\r\n"
	print "<html>"
	print "<head>"
	print "<title>Hello - Second CGI Program</title>"
	print "</head>"
	print "<body>"
	print "<h2>Error with the calculation</h2>"
	print "</body>"
	print "</html>"
else:
	print "Content-type:text/html\r\n\r\n"
	print "<html>"
	print "<head>"
	print "<title>Hello - Second CGI Program</title>"
	print "</head>"
	print "<body>"
	print "<h2>%d %s %d = %d</h2>" % (number1, operator, number2, result)
	print "</body>"
	print "</html>"
