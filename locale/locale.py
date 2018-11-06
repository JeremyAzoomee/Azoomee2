#!/usr/bin/python

import sys, os

# Modify the import path since we have naming conflicts with using "locale"
del sys.path[0]
SCRIPTS_DIR = os.path.join( os.path.dirname( os.path.abspath( __file__ ) ), 'azoomee' )
sys.path.append( SCRIPTS_DIR )

if __name__ == '__main__':
    import azoomee
    azoomee.HandleCommands()
