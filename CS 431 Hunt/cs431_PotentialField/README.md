compile everything in hw2 package
linux: "find . -name "*.java" -print | xargs javac" 
then run "java hw2/ui/FieldDemo /path/to/fieldfile"

field description file format grammar:
//<FILE> → { <PFIELD> <EOL> } 
//<PFIELD> → <UNIFORM> | <ATTRACTIVE> | <REPULSIVE> | <TANGENTIAL> | <RANDOM> 
//<UNIFORM> → UNIFORM <XLOC> <YLOC> <ORIENTATION> <DROPOFF> 
//<ATTRACTIVE> → ATTRACTIVE <XLOC> <YLOC> <DROPOFF> 
//<REPULSIVE> → REPULSIVE <XLOC> <YLOC> <DROPOFF> 
//<TANGENTIAL> → TANGENTIAL <XLOC> <YLOC> [CCW | CW] <DROPOFF> 
//<RANDOM> → RANDOM <XLOC> <YLOC> <CHANGE_INTERVAL> <DROPOFF> 
//<DROPOFF> → <LINEAR> | <CONSTANT> | <EXPONENTIAL> | <HANNING> 
//<LINEAR> → LINEAR <LA> <MAX> 
//<CONSTANT> → CONSTANT <K> 
//<EXPONENTIAL> → EXPONENTIAL <EA> <EB> 
//<HANNING> HANNING <MAX> <A> 
//<EOL> →‘\n’

