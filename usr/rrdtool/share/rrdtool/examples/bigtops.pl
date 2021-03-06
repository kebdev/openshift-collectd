#! /usr/bin/perl
# this is for after install
use lib qw( /data/openshift/5335fc54d561c1410e000001/app-root/runtime/build-dependencies/rrdtool/lib/perl );

use RRDs;
my $start=time;
my $rrd="randome.rrd";
my $name = $0;
$name =~ s/.*\///g;
$name =~ s/\.pl.*//g;

RRDs::create ($rrd, "--start",$start-1, "--step",300,
	      "DS:a:GAUGE:600:U:U",
	      "DS:b:GAUGE:600:U:U",
	      "RRA:AVERAGE:0.5:1:300");
my $ERROR = RRDs::error;
die "$0: unable to create `$rrd': $ERROR\n" if $ERROR;

# dropt some data into the rrd
my $t;
for ($t=$start; $t<$start+300*300; $t+=300){
  RRDs::update $rrd, "$t:".rand(100).":".(sin($t/800)*50+50);
  if ($ERROR = RRDs::error) {
    die "$0: unable to update `$rrd': $ERROR\n";
  }
}

RRDs::graph "$name.png",
  "--title", uc($name)." Demo", 
  "--start", "$start + 1 h",
  "--end", "start + 1000 min",
  "--interlace", 
  "--imgformat","PNG",
  "--width=450",
  "DEF:a=$rrd:a:AVERAGE",
  "DEF:b=$rrd:b:AVERAGE",
  "CDEF:line=TIME,2400,%,300,LT,a,UNKN,IF",
  "AREA:b#00b6e4:beta",
  "AREA:line#0022e9:alpha",
  "LINE3:line#ff0000",

;

if ($ERROR = RRDs::error) {
  die "ERROR: $ERROR\n";
};


print "This script has created $name.png in the current directory\n";
print "This demonstrates the use of the TIME and % RPN operators\n";
