use File::Find;
my $dir = '.'; # 또는 @ARGV로 받든지...
$totalCounter = 0;
open OUT, ">>report.txt";		
######################
find( {wanted => \&print_if_file, no_chdir => 1}, $dir);
###################
my ($sec, $min, $hour, $mday, $mon, $year, $wday, $yday, $isdst) = localtime();
print "\t\tTotal Source Count : $totalCounter lines\n\t\t".($year+1900) ."-$mon-$mday $hour:$min:$sec\n";
print OUT "\t\tTotal Source Count : $totalCounter lines\n\t\t".($year+1900) ."-$mon-$mday $hour:$min:$sec\n";
close OUT;
<STDIN>;

sub print_if_file {
	#print $_."\n";
	
	if(-f $_ && $_ =~ /\.cpp$|\.h$|\.hpp$|\.mm$|\.c$|\.m$/ )
	{
		print "$File::Find::name - ";
		print OUT "$File::Find::name - ";
		open IN, "$File::Find::name";
		
		
		@arr = <IN>;
		print ($#arr + 1);
		print OUT ($#arr + 1);
		$totalCounter += ($#arr + 1);
		print " lines\n";
		print OUT " lines\n";
		close IN;
		
	}	
}