#!/usr/bin/ruby

require "cgi"
cgi = CGI.new
value = cgi['field_name']   # <== value string for 'field_name'
  # if not 'field_name' included, then return "".
fields = cgi.keys            # <== array of field names

# returns true if form has 'field_name'
cgi.has_key?('field_name')
cgi.has_key?('field_name')
cgi.include?('field_name')
