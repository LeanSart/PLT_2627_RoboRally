#!/bin/sh
/usr/bin/cmake -E copy "/home/leand/PLT_2627_game/DartConfiguration.tcl" "/home/leand/PLT_2627_game/test/DartConfiguration.tcl"
/usr/bin/ctest -j --output-on-failure --timeout 300 --test-dir "/home/leand/PLT_2627_game/test" || /bin/true