#!/usr/bin/env perl

use LibIdmefv2;

sub log_func {
	($level, $str) = @_;
	print("log: " . $str);
}

sub print_array($)
{
    my $arrRef = shift;
    for (my $i = 0; $i < scalar(@$arrRef); ++$i) {
        if (ref($arrRef->[$i]) eq 'ARRAY') {
            print ', ' if ($i);
            print '[';
            print_array($arrRef->[$i]);
            print ']';
        } else {
            print ', ' if ($i);
            print $arrRef->[$i];
        }
    }
    print ' ' if (!scalar(@$arrRef));
}


LibIdmefv2::LibIdmefv2Log::setCallback(\&log_func);

$idmefv2 = new LibIdmefv2::IDMEFV2;

print "*** IDMEFV2->Set() ***\n";
$idmefv2->set("alert.classification.text", "My Message");
$idmefv2->set("alert.source(0).node.address(0).address", "s0a0");
$idmefv2->set("alert.source(0).node.address(1).address", "s0a1");
$idmefv2->set("alert.source(1).node.address(0).address", "s1a0");
$idmefv2->set("alert.source(1).node.address(1).address", "s1a1");
$idmefv2->set("alert.source(1).node.address(2).address", undef);
$idmefv2->set("alert.source(1).node.address(3).address", "s1a3");
print $idmefv2->toString();


print "\n*** IDMEFV2->Get() ***\n";
print $idmefv2->get("alert.classification.text") . "\n";

print "\n\n*** Value IDMEFV2->Get() ***\n";
print $idmefv2->get("alert.classification.text");

print "\n\n*** Listed Value IDMEFV2->Get() ***\n";
print_array($idmefv2->get("alert.source(*).node.address(*).address"));

print "\n\n*** Object IDMEFV2->Get() ***\n";
print $idmefv2->get("alert.source(0).node.address(0)");

print "\n\n*** Listed Object IDMEFV2->Get() ***\n";
print_array($idmefv2->get("alert.source(*).node.address(*)"));


open FILE, ">foo.bin" or die "arg";
$idmefv2->write(FILE);
close FILE;

print "\n\n*** IDMEFV2->Read() ***\n";
open FILE2, "<foo.bin" or die "arg2";
my $idmefv22 = new LibIdmefv2::IDMEFV2;
while ( $idmefv22->read(FILE2) ) {
	print $idmefv22->toString();
}

close FILE2;

print "\n*** Client ***";
$client = new LibIdmefv2::ClientEasy("libidmefv2-lml");
$client->start();

$client->sendIDMEFV2($idmefv2);
