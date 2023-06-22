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
import os
from bs4 import BeautifulSoup

url = "http://localhost:9999/index"
error4 = "http://localhost:9999/bad"

def remove_html_tags(html):
	soup = BeautifulSoup(html, 'html.parser')
	text = soup.get_text()
	return text

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
	url2 = "http://localhost:9998/index"
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
	url2 = "http://127.0.0.1:9997/index"
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

def	test_cgi_calculator_plus():
	url2 = "http://localhost:9999/cgi-bin/calculator.py"
	input_data = {
		"num1": "10",
		"num2": "5",
		"operator": "plus"
	}
	expected_output = "10 + 5 = 15"
	response = requests.post(url2, data=input_data)
	clean_text = remove_html_tags(response.text)
	lines = clean_text.splitlines()
	print(lines[25])
	assert response.status_code == 200
	assert lines[25] == expected_output

def	test_cgi_calculator_minus():
	url2 = "http://localhost:9999/cgi-bin/calculator.py"
	input_data = {
		"num1": "10",
		"num2": "5",
		"operator": "minus"
	}
	expected_output = "10 - 5 = 5"
	response = requests.post(url2, data=input_data)
	clean_text = remove_html_tags(response.text)
	lines = clean_text.splitlines()
	print(lines[25])
	assert response.status_code == 200
	assert lines[25] == expected_output

def	test_cgi_calculator_multiply():
	url2 = "http://localhost:9999/cgi-bin/calculator.py"
	input_data = {
		"num1": "10",
		"num2": "5",
		"operator": "multiply"
	}
	expected_output = "10 * 5 = 50"
	response = requests.post(url2, data=input_data)
	clean_text = remove_html_tags(response.text)
	lines = clean_text.splitlines()
	print(lines[25])
	assert response.status_code == 200
	assert lines[25] == expected_output

def	test_cgi_calculator_division():
	url2 = "http://localhost:9999/cgi-bin/calculator.py"
	input_data = {
		"num1": "10",
		"num2": "5",
		"operator": "divide"
	}
	expected_output = "10 / 5 = 2.0"
	response = requests.post(url2, data=input_data)
	clean_text = remove_html_tags(response.text)
	lines = clean_text.splitlines()
	print(lines[25])
	assert response.status_code == 200
	assert lines[25] == expected_output

def test_autoindex():
	url2 = "http://localhost:9999/autoindex"
	expected_file_path = "../website/html/autoindex"
	expected_files = os.listdir(expected_file_path)

	response = requests.get(url2)
	clean_text = remove_html_tags(response.text)
	assert response.status_code == 200
	for file in expected_files:
		assert file in clean_text

def test_autoindex_off():
	url2 = "http://localhost:9999/test"
	with open("../website/html/test/index.html") as file:
		expected_content = file.read()

	response = requests.get(url2)
	clean_text = remove_html_tags(response.text)
	assert response.status_code == 200
	assert response.text == expected_content


# def test_http2_request():
# 	with httpx.Client(http2=True) as client:
# 		response = client.get(url)
# 	with open("../website/html/error_pages/400.html") as file:
# 		expected_content = file.read()
# 	assert response.status_code == 400
# 	assert response.text == expected_content
	