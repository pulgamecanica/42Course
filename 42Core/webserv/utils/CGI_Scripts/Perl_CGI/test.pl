#!/usr/bin/perl
use strict;
use warnings;
use CGI;

my $cgi = CGI->new;
my %param = map { $_ => scalar $cgi->param($_) } $cgi->param() ;
print $cgi->header( -type => 'text/plain' );
print qq{PARAM:\n};
for my $k ( sort keys %param ) {
    print join ": ", $k, $param{$k};
    print "\n";
}
# PARAM:
# foo: bar

