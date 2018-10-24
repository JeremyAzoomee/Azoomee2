#!/usr/bin/python

import csv, json, os, io

LANGUAGES_FILE = "languages.csv"
INSTALL_DIR = "../Resources/res/languages"

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
    
    print languages['en-GB']

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