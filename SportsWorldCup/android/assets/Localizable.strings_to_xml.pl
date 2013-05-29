use File::Find;
my $dir = '.'; # 또는 @ARGV로 받든지...
find( {wanted => \&print_if_file, no_chdir => 1}, $dir);

sub print_if_file {
	#print $_."\n";
	print "$File::Find::name\n";
	if(-f $_ && $_ =~ /Localizable.strings$/ )
	{
		#print "$File::Find::name\n";
		open IN, "$File::Find::name";
		open OUT, ">$File::Find::dir.txt";		
		
		@arr = <IN>;
		foreach $aline (@arr)
		{	
		  if( $aline =~ /^"(.*)" = "(.+)";/)
		    {
		        print OUT "<string name=\"$1\">$2</string>\n";
		    }else{
		        print "error\n";
		    }
		}
		close IN;
		close OUT;
	}	
}