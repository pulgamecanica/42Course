#!/usr/bin/python3

# Import modules for CGI handling 

import cgi, cgitb 
import requests
import os
from pathlib import Path
from bs4 import BeautifulSoup

# Create instance of FieldStorage 
form = cgi.FieldStorage() 

print ("Content-type:text/html\r\n\r\n")
print ("<html>")
print ("<head>")
print ("<title>Query Results</title>")
print ("</head>")
print ("<body>")
print ("<div class=\"results-container\">")

if form.getvalue('query'):
	query = form.getvalue('query')
	os.chdir("../")
	results = sorted(Path('.').rglob("*" + query + "*"))
	headers = {'Access-Control-Allow-Origin': '*', 'Access-Control-Allow-Methods': 'GET', 'Access-Control-Allow-Headers': 'Content-Type', 'Access-Control-Max-Age': '3600', 'User-Agent': 'Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:52.0) Gecko/20100101 Firefox/52.0'}
	url = "https://github.com/search?q=user%3Apulgamecanica+" + query
	req = requests.get(url, headers)
	soup = BeautifulSoup(req.content, 'html.parser') 
	web_results = soup.findAll('li', 'repo-list-item')
	print ("<h3> Founded <span class=\"result-num\">{}</span> result(s) for: <span class=\"query\">{}</span> </h3>".format(len(results) + len(web_results), query))
	for result in results:
		print ("<div class=\"result\">")
		print ("<span class=\"result-legend\">webserv: {}</span>".format(os.fspath(result)))
		result_path = os.fspath(result)
		if (result.is_dir()):
			print ("<p class=\"directory\"><a href=\"" + os.fspath(result) + "/\">" + result.name + "/</a></p>")
		else:
			print ("<p class=\"file\"><a href=\"" + os.fspath(result) + "\">" + result.name + "</a></p>")
		print ("</div>")
	for web_result in web_results:
		print ("<div class=\"result\">")
		print ("<span class=\"result-legend\">github: pulgamecanica</span>")
		print ("<p class=\"file\"><a href=\"https://github.com/" + web_result.a.get('href') + "\">" + web_result.a.get('href') + "</a></p>")
		print ("</div>")
		#print(web_result.get('href'))
#	if (query.lower().find("libft") != -1):
#		print("<p>Web Scrapper [1 result, from 1 page(s)] </p><hr>\n<a href=\"https://github.com/pulgamecanica/42Course/tree/main/42Core/Libft\">github.com: pulgamecanica/Libft </a>")
#	elif (query.lower().find("php") != -1):
#		print("<p>webserv [1 result, from 1 route(s)] </p><hr>\n<a href=\"http://localhost:4242/42cgi-bin/info.php\">webserv: 42cgi-bin/info.php </a>")
#	elif (query.lower().find("perl") != -1):
#		print("<p>webserv [1 result, from 1 route(s)] </p><hr>\n<a href=\"http://localhost:4242/42cgi-bin/test.pl?some=value&id=99\">webserv: 42cgi-bin/test.pl </a>")
else:
   print("No query available...")

print ("</div>")
print ("</body>")
print ("</html>")
