# to install pytest: sudo pip3 install pytest
# need to create venv: python3 -m venv env
# then activate it: source env/bin/activate
# to run: pytest testing.py

import unittest
import requests

url = "http://localhost:9999"
error4 = "http://localhost:9999/bad"

def test_root_url():
	response = requests.get(url)
	assert response.status_code == 200
	assert response.text == "Testing"

def test_nonexistent_page():
	response = requests.get(error4)
	with open("../web/error_pages/404.html") as file:
		expected_content = file.read()
	assert response.status_code == 404
	assert response.text == expected_content

def test_invalid_method():
	with open("../web/error_pages/405.html") as file:
		expected_content = file.read()
	response = requests.request("TTT", url)
	assert response.status_code == 405
	assert response.text == expected_content