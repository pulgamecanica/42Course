#!/usr/bin/python3

# Import modules for CGI handling 
import cgi, cgitb 

# Create instance of FieldStorage 
form = cgi.FieldStorage() 

# Get data from fields
if form.getvalue('country'):
   subject = form.getvalue('country')
else:
   subject = "Not entered"

print ("Content-type:text/html\r\n\r\n", end='\n')
print ("<html>", end='\n')
print ("<head>", end='\n')
print ("<title>Dropdown Box - Sixth CGI Program</title>", end='\n')
print ("</head>", end='\n')
print ("<body>", end='\n')
if (subject == "Mexico"):
    print ("<img src=\"/random/content/mexico.jpg\">", end='\n')
elif (subject == "Brasil"):
    print ("<img src=\"/random/content/brasil.jpg\">", end='\n')
else:
    print ("<img src=\"/random/content/Polen.webp\">", end='\n')
print ("</body>", end='\n')
print ("</html>", end='\n')