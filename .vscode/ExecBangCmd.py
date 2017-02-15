#!echo "hello world"
import sys
import os
import os.path

#print sys.argv
sourcePath = sys.argv[1] # full path
sourceBasename = os.path.basename(sourcePath)
sourceDirname = os.path.dirname(sourcePath)
#print sourcePath, sourceBasename, sourceDirname

with open(sourcePath, 'rt') as f:
    firstLine = f.readline()

parts = firstLine.split('!', 1)
if len(parts) != 2:
    print "error: !-command not found on first line of file"
else:
    command = parts[1]

    if not sourceBasename in command:
        print "warning: !-command doesn't contain the source file name `" + sourceBasename + "` (did you copy-paste without editing?)"

    print "$", command
    sys.stdout.flush()

    os.chdir(sourceDirname)
    os.system(command)