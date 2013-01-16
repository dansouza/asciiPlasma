asciiPlasma
-----------

**Mesmerizing eye-candy plasma goodness now in your console!**

asciiPlasma is a C port of [Lee W. Fastenau's javascript asciiPlasma](http://ioyu.com/io/javascript/asciiPlasma.asp).
You might also want to check out a [reddit discussion](http://www.reddit.com/r/programming/comments/16mmon/made_this_ages_ago_its_been_fun_to_see_it_run/) that inspired this port.

running
-------

	$ gcc -Wall -g -O2 -o asciiPlasma asciiPlasma.c -lm -lncurses
	$ ./asciiPlasma

coming soon
-----------
* custom terminal dimensions as parameters (-w and -h)
* option to switch between PAL1 and PAL2 at runtime (-1 and -2)
* use colors/shades of gray on the palette (256 color terminals?)
* optional text message to be displayed in the middle ([-m &lt;message&gt;])
* handle terminal resizing gracefully
* redirect output to another terminal (mesmerize your co-workers in troll-mode!)

**keywords:** linux ascii art animation console ncurses plasma xterm
