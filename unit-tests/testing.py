# to install pytest: sudo pip3 install pytest
# need to create venv: python3 -m venv env
# then activate it: source env/bin/activate
# to run: pytest testing.py
# might also need to install requests module: pip3 install requests
# and pip3 install httpx
# on school computers i also had to install with brew: brew install httpx

import unittest
# import httpx
import requests

url = "http://localhost:9999"
error4 = "http://localhost:9999/bad"

def test_root_url():
	response = requests.get(url)
	assert response.status_code == 200
	with open("../website/html/index.html") as file:
		expected_content = file.read()
	assert response.text == expected_content

def test_nonexistent_page():
	response = requests.get(error4)
	with open("../website/html/error_pages/404.html") as file:
		expected_content = file.read()
	assert response.status_code == 404
	assert response.text == expected_content

def test_invalid_method():
	with open("../website/html/error_pages/405.html") as file:
		expected_content = file.read()
	response = requests.request("TTT", url)
	assert response.status_code == 405
	assert response.text == expected_content

def test_several_ports():
	url2 = "http://localhost:9998"
	response = requests.get(url)
	assert response.status_code == 200
	with open("../website/html/index.html") as file:
		expected_content = file.read()
	assert response.text == expected_content
	
	response = requests.get(url2)
	assert response.status_code == 200
	with open("../website/html/index.html") as file:
		expected_content = file.read()
	assert response.text == expected_content

def test_several_servers():
	url2 = "http://localhost:9997"
	response = requests.get(url)
	assert response.status_code == 200
	with open("../website/html/index.html") as file:
		expected_content = file.read()
	assert response.text == expected_content
	
	response = requests.get(url2)
	assert response.status_code == 200
	with open("../website/html/index.html") as file:
		expected_content = file.read()
	assert response.text == expected_content

# def test_hhtp2_request():
# 	with httpx.Client(http2=True) as client:
# 		response = client.get(url)
# 	with open("../website/html/error_pages/400.html") as file:
# 		expected_content = file.read()
# 	assert response.status_code == 400
# 	assert response.text == expected_content
	