#!/usr/bin/perl

use File::Find;
    
# Traverse desired filesystems
sub wanted {
    my $file = $_;
    my $dir = $File::Find::dir;
    if( $file =~ /.xcodeproj$/ )
    {
        $cmd = "../../../scripts/upgrade-xcode-vst.sh $file; cd ../../..; mv xcode-output/$file/build/Release/*vst BUILT_VST";
        print $cmd . "\n";
        system( $cmd );
    }

}

system( "mkdir -p BUILT_VST" );
find(\&wanted, "plugins/MacVST");

