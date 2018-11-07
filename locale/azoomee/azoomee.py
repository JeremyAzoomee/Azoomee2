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
RESERVED_KEYS = [ 'key', 'used' ]
YES_WORDS = [ 'yes', 'y' ]

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
            if header in RESERVED_KEYS:
                continue
            languages[header] = {}

        # For each language, map key to the language text
        for row in reader:
            key = unicode(row['key'], 'utf-8')

            for header in reader.fieldnames:
                if header in RESERVED_KEYS:
                    continue
                languageDict = languages[header]
                languageDict[key] = unicode(row[header], 'utf-8')
    
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
            unicode_row = { k: unicode( v, 'utf-8' ) for k,v in row.items() }
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
            data = json.dumps( stringsDict, ensure_ascii=False, sort_keys=True, indent=4, separators=(',',': ') )
            fp.write( data )

            print 'Saved', jsonFilePath


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
                    #print( os.path.join( root, file ) )

                    fileData = None
                    with open( os.path.join( root, file ), 'r' ) as fh:
                        fileData = fh.read()
                    
                    matches = GET_TEXT_REGEX.findall( fileData )
                    for match in matches:
                        strings.add( unicode(match[1], 'utf-8') )
    
    return strings


def GetTextFromCTAFiles():
    """
    Parses locale text from CTA files.
    """
    strings = set()

    # TODO: Implement this

    return strings


## - Install

def InstallCommand( args ):
    """
    Takes the data from the languages csv file and saves it into the JSON files for use in the app.
    """
    print '**Install**'
    languages = GetLanguagesFromCSV()
    SaveLanguagesToJSON( languages )


## - Update

def UpdateCommand( args ):
    """
    Parse the source code and update the languages csv file with new strings.
    """
    print '**Update**'

    # Get strings from source code and cta files
    strings = GetTextFromSourceCode()
    ctaStrings = GetTextFromCTAFiles()
    strings |= ctaStrings;

    # Get existing spreadsheet data
    fieldNames, rows = GetDataFromCSV()
    hasUsedData = 'used' in fieldNames
    if not hasUsedData:
        fieldNames.insert( 1, 'used' )

    existing_rows = []
    unused_rows = []

    for row in rows:
        used = 'no'
        if row['key'] in strings:
            used = 'yes'
        elif hasUsedData:
            used = row['used']
        
        row['used'] = used

        if used == 'yes' or used == 'dyn':
            existing_rows.append( row )
            strings.remove( row['key'] )
        else:
            unused_rows.append( row )
    
    new_rows = []
    for string in strings:
        new_rows.append( { 'key': string, 'used': 'yes' } )
    
    print 'Existing rows:', len(existing_rows)
    print 'Unused rows:', len(unused_rows)
    print 'New rows:', len(strings)

    # Write out new CSV file
    with open( LANGUAGES_FILE, 'w' ) as csvfile:
        writer = csv.DictWriter( csvfile, fieldnames=fieldNames )
        writer.writeheader()

        allRows = new_rows + existing_rows + unused_rows

        for row in allRows:
            writer.writerow( { k: v.encode( 'utf-8' ) if isinstance(v, unicode) else v for k,v in row.items() } )
    

    # Run report command automatically
    print ''
    ReportCommand( args )


## - Report

def ReportCommand( args ):
    """
    Print a report about the language translations.
    """
    print '**Report**'

    languages = GetLanguagesFromCSV()

    print 'Missing Translations:'
    for language, stringsDict in languages.iteritems():
        missingTranslationCount = 0
        for key, value in stringsDict.iteritems():
            if value is None or value.strip() == "":
                missingTranslationCount += 1
        
        print language + ': ' + str(missingTranslationCount)


## - Main

Commands = {
    'install': InstallCommand,
    'update': UpdateCommand,
    'report': ReportCommand
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
