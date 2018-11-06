#!/usr/bin/python

import argparse
import csv
import io
import json
import os
import re


## - Common

LANGUAGES_FILE = 'languages.csv'
INSTALL_DIR = '../Resources/res/languages'
CODE_DIRS = [ 
    '../Classes', 
    '../ArtApp/Classes', 
    '../chat/Classes', 
    '../oomeemaker/Classes'
]
PARSE_FILES = [ '.cpp', '.mm' ]

QUOTED_STRING_REGEX = re.compile( r"(?P<quote>['\"])(?P<string>.*?)(?<!\\)(?P=quote)" )
GET_TEXT_REGEX = re.compile( r"_\(\s*(?P<quote>['\"])(?P<string>.*?)(?<!\\)(?P=quote)\s*\)" )


def GetLanguagesFromCSV():
    """
    Return a languages dictionary with data from the language CSV file.
    """
    languages = {}

    # Open the csv file and grab the language text
    with open( LANGUAGES_FILE ) as csvfile:
        reader = csv.DictReader( csvfile )
        
        # Initialise dictionaries
        for header in reader.fieldnames:
            if header == 'key':
                continue
            languages[header] = {}

        # For each language, map key to the language text
        for row in reader:
            for header in reader.fieldnames:
                if header == 'key':
                    continue
                languageDict = languages[header]
                key = unicode(row['key'], 'utf-8')
                languageText = unicode(row[header], 'utf-8')
                languageDict[key] = languageText
    
    return languages


def GetDataFromCSV():
    """
    Return csv data from languages file.
    """
    rows = []
    fieldNames = []

    with open( LANGUAGES_FILE ) as csvfile:
        reader = csv.DictReader( csvfile )
        fieldNames = reader.fieldnames

        for row in reader:
            unicode_row = {}
            for fieldName in fieldNames:
                unicode_row[fieldName] = unicode(row[fieldName], 'utf-8')
            rows.append( unicode_row )

    return fieldNames, rows


def SaveLanguagesToJSON( languages ):
    """
    Save languages to their corresponding JSON files.
    """
    # Now save out each language in JSON format
    for language, stringsDict in languages.iteritems():
        # Make sure language directory exists
        languageDir = os.path.join( INSTALL_DIR, language )
        try:
            os.makedirs( languageDir )
        except:
            pass

        jsonFilePath = os.path.join( languageDir, 'strings.json' )

        # Create the json file
        with io.open( jsonFilePath, 'w', encoding='utf8' ) as fp:
            data = json.dumps( stringsDict, ensure_ascii=False, sort_keys=True )
            fp.write( data )


def GetTextFromSourceCode():
    """
    Parses locale text from source code.
    """
    strings = set()

    for code_dir in CODE_DIRS:
        # Find all source files to parse
        for root, dirs, files in os.walk( code_dir ):
            for file in files:
                extension = os.path.splitext( file )[1]
                if extension in PARSE_FILES:
                    print( os.path.join( root, file ) )

                    fileData = None
                    with open( os.path.join( root, file ), 'r' ) as fh:
                        fileData = fh.read()
                    
                    matches = GET_TEXT_REGEX.findall( fileData )
                    for match in matches:
                        strings.add( unicode(match[1], 'utf-8') )
    
    return strings


## - Install

def InstallCommand( args ):
    """
    Takes the data from the languages csv file and saves it into the JSON files for use in the app.
    """
    print 'Install'
    languages = GetLanguagesFromCSV()
    SaveLanguagesToJSON( languages )


## - Update

def UpdateCommand( args ):
    """
    Parse the source code and update the languages csv file with new strings.
    """
    print 'Update'

    strings = list(GetTextFromSourceCode())
    fieldNames, rows = GetDataFromCSV()

    existing_rows = []
    unused_rows = []

    for row in rows:
        if row['key'] in strings:
            existing_rows.append( row )
            strings.remove( row['key'] )
        else:
            unused_rows.append( row )
    
    print 'Existing rows:', len(existing_rows)
    print 'Unused rows:', len(unused_rows)
    # for row in unused_rows:
    #     print row['key']
    
    print 'New rows:', len(strings)
    for string in strings:
        print string

    #for string in strings:
    #    print string


## - Main

Commands = {
    'install': InstallCommand,
    'update': UpdateCommand,
}

def HandleCommands():
    # Parse arguments 
    parser = argparse.ArgumentParser()
    parser.add_argument( 'command', choices=Commands.keys(), help='which command to run' )
    args = parser.parse_args()

    func = Commands.get( args.command, None )
    if func is None:
        print 'Command not recognised:', args.command
        exit(1)
    
    # Run the function
    func( args )
