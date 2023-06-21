#!/usr/bin/python3

import cgi, cgitb, os, sys

sign = "+"
error = False
form = cgi.FieldStorage()
number1 = int(form.getvalue('num1'))
number2 = int(form.getvalue('num2'))
operator = form.getvalue('operator')
if operator == "plus":
	result = number1 + number2
elif operator == "minus":
	result = number1 - number2
	sign = "-"
elif operator == "divide":
	if (number2 == 0):
		error = True
	else:
		result = number1 / number2
	sign = "/"
elif operator == "multiply":
	result = number1 * number2
	sign = "*"
else:
	error = True

header = """
	HTTP/1.1 200 OK
	Content-type:text/html\r\n\r\n
"""

temp = """
	<!DOCTYPE html>
	<head>
		<meta charset="utf-8">
		<meta name="viewport" content="width=device-width, initial-scale=1.0" />
		<title>Webserv</title>
		<link rel="icon" type="image/x-icon" href="/assets/logo19.ico">
		<link rel="stylesheet" href="/assets/style.css">
	</head>

	<header>
		<div class="logo">
			WEBSERV
		</div>
		<div>
			<ul class="navbar">
				<li><a href="/index.html">Home</a></li>
				<li><a href="/cgi-tester.html">CGI Tester POST</a></li>
				<li><a href="/cgi-tester-get.html">CGI Tester GET</a></li>
				<li><a href="/upload-file.html">Upload File</a></li>
				<li class="right-align"><a href="/about.html">About</a></li>
			</ul>
		</div>
	</header>
	<body class="body-style">
	<br><br><br><br><br>
		"""
end = """
		</body>
		<footer>
		<ul class="footbar">
			<li> <a href="#">webserv</a></li>
			<ul class="right-align">
				<li><a href="https://profile.intra.42.fr/users/abuzdin">abuzdin</a></li>
				<li><a href="https://profile.intra.42.fr/users/ejoo-tho">ejoo-tho</a></li>
				<li><a href="https://profile.intra.42.fr/users/jvander-">jvander-</a></li>
			</ul>
		</ul>
	</footer>
	</html>
"""

if error:
	temp2 = "<h2>Something went wrong with the calculation...</h2>"
	count = len(temp) + len(temp2) + len(end)
else:
	temp2 = "<h2>{} {} {} = {}</h2>".format(number1, sign, number2, result)
	count = len(temp) + len(temp2) + len(end)

print ("HTTP/1.1 200 OK")
print ("Content-type:text/html")
print ("Content-Length: {}\r\n\r\n".format(count))
print()
print(temp, end=" ")
print(temp2)
print(end)

