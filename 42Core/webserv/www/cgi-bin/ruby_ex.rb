#!/usr/bin/ruby

require "cgi"
cgi = CGI.new("html4")  # add HTML generation methods
cgi.out do
  cgi.html do
    cgi.head do
      cgi.title { "TITLE" }
    end +
    cgi.body do
      cgi.form("ACTION" => "uri") do
        cgi.p do
          cgi.textarea("get_text") +
          cgi.br +
          cgi.submit
        end
      end +
      cgi.pre do
        CGI::escapeHTML(
          "params: #{cgi.params.inspect}\n" +
          "cookies: #{cgi.cookies.inspect}\n" +
          ENV.collect do |key, value|
            "#{key}: #{value}\n"
          end.join("")
        )
      end
    end
  end
end

# add HTML generation methods
CGI.new("html3")    # html3.2
CGI.new("html4")    # html4.01 (Strict)
CGI.new("html4Tr")  # html4.01 Transitional
CGI.new("html4Fr")  # html4.01 Frameset
