#!/usr/bin/perl -w
#

use strict;

push @INC,".";
push @INC,"./perl";
push @INC,"./.libs";

eval  { require LibIdmefv2Easy; };
die "Could not load LibIdmefv2Easy ($@).\nTry 'cd ./.libs && ln -s libidmefv2_perl.so LibIdmefv2Easy.so'" if $@;

sub PrintUID
{
	print "UID is $<\n";
}

LibIdmefv2Easy::set_perlmethod(\&PrintUID);

LibIdmefv2Easy::test_fct();
