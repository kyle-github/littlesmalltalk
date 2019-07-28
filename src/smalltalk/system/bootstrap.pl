#!/usr/bin/perl

use strict;
use Data::Dumper;



# MAIN

my @classes = [];

@classes = read_files(@ARGV);

# sort based on parent class
@classes = sort_by_parent(@classes);

# print out the create strings first.
foreach my $class (@classes) {
    print "\" Class $class->{name} \"\n";
    print "$class->{createString}\n";
}

#print out the class methods
foreach my $class (@classes) {
    my $lines_ref = $class->{lines};
    my @lines = @$lines_ref;
    foreach my $line (@lines) {
        unless($line =~ m/^\+/) {
            print "$line\n"
        }
    }
}

#print Dumper(@classes);

# subroutines

sub read_files() {
    my @sources = @_;
    my @classes = ();

    foreach my $source (@sources) {
        #print "reading $source.\n";

        # read in the whole file.
        open my $in, "<:encoding(utf8)", $source or die "File $source error: $!";
        my @lines = <$in>;
        close $in;
        chomp @lines;

        my $class = { 'lines' => \@lines };

        # find the class declaration.  It will look like:
        #  "+ParserNode subclass: #ArgumentNode variables: #( position ) classVariables: #( )"
        foreach my $line (@lines) {
            chomp $line;

            if($line =~ m/\+(\w+)\s+subclass:\s+#(\w+)\s+variables:\s+#\(\s*([^\)]+)\s*\)\s+classVariables:\s+#\(\s*([^\)]+)\s*\)\s*$/) {
                my $parent_name = $1;
                my $class_name = $2;
                my $variables = ($3 || '');
                my $classVariables = ($4 || '');

                $class->{parent} = $parent_name;
                $class->{name} = $class_name;
                $class->{variables} = $variables;
                $class->{classVariables} = $classVariables;
                $class->{createString} = $line;

                push(@classes, $class);

                #print "Found class $class_name subclass of $parent_name with instance vars $variables and class vars $classVariables.\n";

                last;
            }
        }
    }

    return @classes;
}


sub sort_by_parent() {
    my @classes = @_;
    my $Object_Class;

    # first find the Object class.
    for(my $i=0; $i < $#classes; $i++) {
        my $class = $classes[$i];

        # make sure that Object is the first entry.
        if($class->{name} eq "Object") {
            my $tmp = $classes[0];
            $classes[0] = $class;
            $classes[$i] = $tmp;

            #print "Found $classes[0]->{name} at index $i.\n";

            last;
        }
    }

    # now sequence along the array with two indexes.
    # the first points to the parent, the second collects the children.
    my $parent_idx = 0;
    my $child_idx = 1;

    for($parent_idx = 0; $parent_idx < $#classes; $parent_idx++) {
        my $parent = $classes[$parent_idx];

        #print "Finding subclasses of $parent->{name}.\n";

        for(my $possible_child_idx = $child_idx; $possible_child_idx < $#classes; $possible_child_idx++) {
            # if this possible child is an immediate child of the parent, then swap it into place.
            if($classes[$possible_child_idx]->{parent} eq $parent->{name}) {
                #print "  Found $parent->{name} subclass $classes[$possible_child_idx]->{name} and swapping it for element $child_idx.\n";

                my $swap_child = $classes[$child_idx];
                $classes[$child_idx] = $classes[$possible_child_idx];
                $classes[$possible_child_idx] = $swap_child;
                $child_idx++;
            }
        }
    }

    return @classes;
}
