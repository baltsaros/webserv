#!/usr/bin/python3

import cgi, cgitb

form = cgi.FieldStorage()
result = 0
error = False
number1 = form.getvalue('num1', '')
number2 = form.getvalue('num2', '')
operator = form.getvalue('operator', '')
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

#if error:
#	print ("HTTP/1.1 200 OK")
#	print ("Content-type:text/html\r\n\r\n")
#	print ("<html>")
#	print ("<head>")
#	print ("<title>Hello - Second CGI Program</title>")
#	print ("</head>")
#	print ("<body>")
#	print ("<h2>Error with the calculation</h2>")
#	print ("</body>")
#	print ("</html>")
#else:
	print ("HTTP/1.1 200 OK")
	print ("Content-type:text/html\r\n\r\n")
	print ("<html>")
	print ("<head>")
	print ("<title>Hello - Second CGI Program</title>")
	print ("</head>")
	print ("<body>")
	print ("<h2>%s %s %s = %s</h2>" % (number1, operator, number2, result))
	print ("</body>")
	print ("</html>")
