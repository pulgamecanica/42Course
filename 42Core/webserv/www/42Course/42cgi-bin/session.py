#!/usr/bin/python3
import cgi, cgitb
import hashlib, shelve, time
import os, sys, stat
from datetime import datetime
from http import cookies

class Session(object):

    def __init__(self, expires=None, cookie_path=None):
        string_cookie = os.environ.get('HTTP_COOKIE')
        self.cookie = cookies.SimpleCookie()
        if string_cookie:
            self.cookie.load(string_cookie)

        if string_cookie and self.cookie.get('sid'):
            sid = self.cookie['sid'].value
            self.cookie.clear() 
        else:
            self.cookie.clear()
            sid = hashlib.sha224(repr(time.time()).encode('utf-8')).hexdigest()

        self.cookie['sid'] = sid

        if cookie_path:
            self.cookie['sid']['path'] = cookie_path

        session_dir = os.environ['DOCUMENT_ROOT'] + 'session'
        #session_dir = "./session"

        self.data = shelve.open (
            '%s/sess_%s' % (session_dir, sid), 
            writeback=True
        )

        os.chmod('%s/sess_%s' % (session_dir, sid),  stat.S_IRUSR | stat.S_IWUSR | stat.S_IRGRP | stat.S_IWGRP | stat.S_IROTH)

        # Initializes the expires data
        if not self.data.get('cookie'):
            self.data['cookie'] = {'expires':''}

        self.set_expires(expires)

    def close(self):
        self.data.close()

    def set_expires(self, expires=None):
        if expires == '':
            self.data['cookie']['expires'] = ''
        elif isinstance(expires, int):
            self.data['cookie']['expires'] = expires

        self.cookie['sid']['expires'] = self.data['cookie']['expires']

cgitb.enable()
sys.path.append(os.environ['DOCUMENT_ROOT'])

sess = Session(expires=24*60*60, cookie_path='/')
# expires can be reset at any moment:
sess.set_expires('')
# or changed:
sess.set_expires(30*24*60*60)

# Session data is a dictionary like object
lastvisit = sess.data.get('lastvisit')
if lastvisit:
    message = 'Welcome back. Your last visit was at ' + \
        time.asctime(time.gmtime(float(lastvisit)))
else:
    message = 'New session'

# Save the current time in the session
sess.data['lastvisit'] = repr(time.time())
print ("""%s\nContent-Type: text/plain\n
sess.cookie = %s
sess.data = %s
%s
""" % (sess.cookie, sess.cookie, sess.data, message))
sess.close()
