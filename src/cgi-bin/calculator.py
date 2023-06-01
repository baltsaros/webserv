#!/bin/python

import cgi

form = cgi.FieldStorage()

number1 = form.getValue("num1")
number2 = form.getValue("num2")
operator = form.getValue("operator")


print "Content-type:text/html\r\n\r\n"
print "<html>"
print "<head>"
print "<title>Hello - Second CGI Program</title>"
print "</head>"
print "<body>"
print "<h2>%d %s %d = un nombre</h2>" % (number1, number2, operator)
print "</body>"
print "</html>"
