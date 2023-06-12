#!/usr/bin/python3

import cgi, os
import cgitb

cgitb.enable()

form = cgi.FieldStorage()

fileItem = form['filename']
path = "upload_files/"
isExist = os.path.exists(path)

if not isExist:
	os.makedirs(path)

temp = """
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
				<li><a href="/cgi-tester.html">CGI Tester</a></li>
				<li><a href="/upload-file.html">Upload File</a></li>
				<li><a href="/delete-file.html">Delete File</a></li>
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

if  fileItem.filename:
	fn = os.path.basename(fileItem.filename)
	dest_path = os.path.join(path, fn)
	open(path + fn, 'wb').write(fileItem.file.read())
	message = 'The file "' + fn + '" was uploaded successfully'
	count = len(temp) + len(message) + len(end)
else:
	message = 'No file was uploaed'
	count = len(temp) + len(message) + len(end)

print ("HTTP/1.1 200 OK")
print ("Content-Type: text/html")
print ("Content-Length: {}\r\n\r\n".format(count))
print()
print(temp, end=" ")
print(message)
print(end)