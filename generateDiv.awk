#!/usr/bin/gawk -f
BEGIN{
	print "" > "optiondivs.txt"
	}
NF > 0 {
	option = $2;
	print "<div>\n\
        	<input id=\""option"\" class=\"slider\" type="range" min=0 max=2000 value={{"option"}}>\n\
                <label for=\""option"\">"option"</label>\n\
                <span id=\""option"\">{{"option"}}</span>\n\
        </div>\n" >> "optiondivs.txt"

	}


