Letter [a-z]
Digit [0-9]

%%
int             { return 2; }
chr             { return 3; }
{Letter}+    		{ return 1; }
\,       		    { return ','; }
\:       		    { return ':'; }
\;              { return ';'; }

[ \n\t]		{ }
.					{ }
%%
