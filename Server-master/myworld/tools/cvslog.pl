#!/usr/bin/perl

my $args = '';#join ' ',@ARGV;
my $sep = '=============================================================================';
foreach (@ARGV) {
	my $tmp = $_;
	if ($tmp =~ /\</ or $tmp =~ /\>/) {
		$tmp = "\"$_\"";
	}
	$args .= "$tmp ";
}
#print $args;
#my $n = @ARGV;
#print $n;
my $raw = 0;
my $help = 0;
if ($args =~ s/--raw//) {
	
	$raw = 1;
}

if ($args =~ s/--help//) {
	$help = 1;
}
#print $raw.$help;
#exit;
sub parselog($);

my $ret = '';
my $usage = 'Usage: 
 --raw <options>	Equal "cvs log <options>"
 --help			Display this information
 <options>		Print as cvslog.pl style,<options> are same as cvs log options

Examples:
 cvslog -d "2012-1-1<2012-3-9"
';
if (1 == $raw) {
	$ret = `cvs log $args`;
}
else
{
	if ($help == 1) {
		my $tmp = `cvs --help log 2>&1`;
		$ret = "==========\ncvs log \n==========\n$tmp==========\ncvslog.pl\n==========\n$usage";
	}
	else
	{
		my $tmp = `cvs log -N $args 2>&1`;
		print "$sep\nFile list\n$sep\n";
		$tmp = parselog($tmp);
		print "$sep\nChange list\n$sep\n";
		$ret = $tmp;
	}
}


print $ret;


sub parselog($)
{
	my $str = shift;

	my @arr = split /$sep/,$str;

	my $n = 1;
	my @ret = ();
	print ("File\tRevision\tDate\tAuthor\tLog\n");
	my %log;
	foreach (@arr) {
		if (not /revision(.*)\ndate:(.*);/) {
			next;
		}
		my $file = '';
		if (/Working file:\s+(.*)\n/) {
#			print ("$1\n");
			$file = $1;
		}

		while ($_ =~ /revision(.*)\ndate:\s+(.*);  author:\s+(.*);\s+state:.*;.*;\n(.*)\n[-]+/g) {
			my $comment = $4;
			print ("$file\t$1\t$2\t$3\t$comment\n");
			if (exists $log{$comment}) {
				$log{$comment}++;
			}
			else
			{
				push @ret,"$4\n";
				$log{$comment} = 1;
			}
			$n++
		}
	}

	return join "",@ret;
}