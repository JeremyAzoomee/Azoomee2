#!/usr/bin/python
import sys, os
import argparse
import shutil
import plistlib

try:
    import git
except ImportError as e:
    print "ImportError:", e
    print "You need to install GitPython:"
    print "  https://github.com/gitpython-developers/GitPython"
    print "Try this:"
    print "  sudo easy_install gitpython"
    sys.exit(1)


class AzoomeeApp:
    """
    Builds the Azoomee app on various platforms.
    """

    # Android project paths & settings
    ANDROID_PROJECT_DIR = os.path.join( os.path.dirname( os.path.abspath( __file__ ) ), 'proj.android-studio' )

    # iOS project paths & settings
    IOS_PROJECT_DIR = os.path.join( os.path.dirname( os.path.abspath( __file__ ) ), 'proj.ios_mac' )
    IOS_PROJECT_PATH = os.path.join( IOS_PROJECT_DIR, 'azoomee2.xcodeproj' )
    IOS_PROJECT_TARGET = 'azoomee2-mobile'
    IOS_PLIST_PATH = os.path.join( IOS_PROJECT_DIR, 'ios', 'Info.plist' )
    IOS_EXPORT_OPTIONS_PATH = os.path.join( IOS_PROJECT_DIR, 'ExportOptions.plist' )

    # Fabric info
    FABRIC_INFO = {
        # Fabric Account API Key
        'api_key' : '3885c5b5aa7470cf4b9263fce3dbf7d6f3c2afc8',
        # Fabric Account Build Secret
        'build_secret' : '79ec75f3c0ed8eccb593d245acdbc2351770496a76f8da2fd96245095eba920c',
        # Comma seperated Beta group names to be used when uploading builds to Fabric
        'test_groups' : '',
        # Path to the iOS Framework
        'ios_framework_path' : os.path.join( IOS_PROJECT_DIR, 'Crashlytics.framework' )
    }

    # Directory to place built binaries
    BUILD_DIR = os.path.join( os.path.dirname( os.path.abspath( __file__ ) ), 'build' )

    # The root of the Git repo
    GIT_ROOT = None
    git_repo = None
    auto_push = False

    # Platforms
    PLATFORMS = (
        'ios',
        'android',
        'android-vodacom'
    )


    def __init__( self ):
        self.GIT_ROOT = self._find_git_root( os.path.dirname( os.path.abspath( __file__ ) ) )
        self.git_repo = git.Repo( self.GIT_ROOT )


    def main( self ):
        """
        Main entry point for command line.
        """
        parser = self._get_command_args_parser()
        args = parser.parse_args()

        actions = {
            'build' : self.build,
            'clean' : self.clean,
            'deploy' : self.deploy,
            'new_release' : self.new_release,
            'version' : self.get_version
        }
        action_func = actions.get( args.action )
        if action_func is None:
            return self.exit_with_error( 'Command not recognised: {command}', command=args.action )
        
        action_func( args )
    

    def new_release( self, args ):
        """
        Create a new version.
        """
        print 'new_release:', args

    
    def get_version( self, args ):
        """
        Print the current version of the app.
        """
        print 'Current version is:', self._get_current_version()
    

    def clean( self, args ):
        """
        Clean build files.
        """
        print 'clean:', args

        platforms = self.PLATFORMS if 'all' in args.platform else args.platform
        for p in platforms:
            self._perform_clean( p )
    

    def build( self, args ):
        """
        Run a build.
        """
        print 'build:', args

        platforms = self.PLATFORMS if 'all' in args.platform else args.platform
        for p in platforms:
            self._perform_build( p )
    

    def deploy( self, args ):
        """
        Build and deploy to Fabric.
        """
        print 'deploy:', args

        current_version = self._get_current_version()

        # Get the current branch in git
        current_branch = self._get_current_branch()

        # Check we are on the release branch for this version
        major_minor_version = current_version[ 0 : current_version.rfind('.') ]
        required_branch = 'release/' + major_minor_version
        if current_branch != required_branch:
            pass #return self.exit_with_error( 'You must be on the branch "{branch}" in order to deploy this version.', branch=required_branch )

        # Do we need to update the version?
        if args.patch:
            current_version = self._increment_version( current_version, patch=True )
            print 'New Version:', current_version
            self._set_current_version( current_version, commit=True )

        # Now run deploy for each platform
        platforms = self.PLATFORMS if 'all' in args.platform else args.platform
        for p in platforms:
            self._perform_deploy( p, force_rebuild=args.rebuild )
    

    def _perform_clean( self, platform ):
        """
        Cleans build directory and project temp data for platform.
        """
        # Remove build dir
        build_dir = os.path.join( self.BUILD_DIR, platform )
        shutil.rmtree( build_dir, ignore_errors=True )

        # Run clean on project to clean temp build files
        if platform == 'ios':
            try:
                success = self.exec_system_command( 'xcodebuild clean -project "{project_path}" -configuration Release -target "{target}"', 
                                                    project_path=self.IOS_PROJECT_PATH, 
                                                    target=self.IOS_PROJECT_TARGET )
                if success != 0:
                    return self.exit_with_error( 'Xcode clean failed with error {code}', code=success )
            except Exception as e:
                return self.exit_with_error( '{exception}', exception=str(e) )
        else:
            print '{platform} clean not implemented yet'.format( platform=platform )
    

    def _perform_build( self, platform ):
        """
        Builds app for a single platform.
        """
        platform_build_dir = os.path.join( self.BUILD_DIR, platform )
        if not os.path.exists( platform_build_dir ):
            os.makedirs( platform_build_dir )
        
        build_name = 'Azoomee-v' + self._get_current_version()

        if platform == 'ios':
            build_config = 'Release'
            build_name += '.xcarchive'
            build_path = os.path.join( platform_build_dir, build_name )

            #if os.path.exists( build_path ):
            #    shutil.rmtree( build_path, ignore_errors=True )

            # Build archive
            try:
                success = self.exec_system_command( 'xcodebuild archive -project "{project_path}" -configuration "{config}" -scheme "{target}" -archivePath "{build_path}"', 
                                                    project_path=self.IOS_PROJECT_PATH, 
                                                    target=self.IOS_PROJECT_TARGET, 
                                                    config=build_config, 
                                                    build_path=build_path )
                if success != 0:
                    return self.exit_with_error( 'Xcode build failed with error {code}', code=success )
            except Exception as e:
                return self.exit_with_error( '{exception}', exception=str(e) )
            
            # Generate IPA
            ipa_path = os.path.join( platform_build_dir, build_name.replace( '.xcarchive', '.ipa' ) )
            try:
                success = self.exec_system_command( 'xcodebuild -exportArchive -allowProvisioningUpdates -archivePath "{archivePath}" -exportPath "{exportPath}" -exportOptionsPlist "{exportOptions}"', 
                                                    archivePath=build_path, 
                                                    exportPath=ipa_path,
                                                    exportOptions=self.IOS_EXPORT_OPTIONS_PATH )
                if success != 0:
                    return self.exit_with_error( 'Xcode build failed with error {code}', code=success )
            except Exception as e:
                return self.exit_with_error( '{exception}', exception=str(e) )
        else:
            print '{platform} build not implemented yet'.format( platform=platform )
    

    def _perform_deploy( self, platform, force_rebuild=False ):
        """
        Deploy app for testing for a single platform.
        """
        if platform == 'ios':
            platform_build_dir = os.path.join( self.BUILD_DIR, platform )
            build_name = 'Azoomee-v' + self._get_current_version()
            ipa_path = os.path.join( platform_build_dir, build_name + '.ipa', self.IOS_PROJECT_TARGET + '.ipa' )
            
            # If we're forcing a rebuild, do a clean and delete the existing archive and ipa
            if force_rebuild:
                self._perform_clean( platform )
                shutil.rmtree( os.path.join( platform_build_dir, build_name + '.ipa' ), ignore_errors=True )
                shutil.rmtree( os.path.join( platform_build_dir, build_name + '.xcarchive' ), ignore_errors=True )

            # Build if needed
            if not os.path.isfile( ipa_path ):
                self._perform_build( platform )

            # If we don't have an ipa file now, we can't deploy anything
            if not os.path.isfile( ipa_path ):
                return self.exit_with_error( 'Unable to deploy, file not found: {file}', file=ipa_path )

            # Now upload the IPA file

            # TODO: Create changelog notes file by summarising all commits from the current release branch
            notes_path = ""

            # Do Fabric upload
            try: 
                # "{framework_path}/submit" {api_key} {build_secret} -ipaPath "{ipa_path}" -groupAliases "{group_aliases}" -notifications {notifications} -notesPath "{notes_path} -debug {debug}"
                success = self.exec_system_command( '"{framework_path}/submit" {api_key} {build_secret} -ipaPath "{ipa_path}" -notifications {notifications} -debug {debug}', 
                                                    framework_path=self.FABRIC_INFO['ios_framework_path'], 
                                                    api_key=self.FABRIC_INFO['api_key'], 
                                                    build_secret=self.FABRIC_INFO['build_secret'], 
                                                    ipa_path=ipa_path,
                                                    group_aliases=self.FABRIC_INFO['test_groups'],
                                                    notifications="NO",
                                                    notes_path=notes_path,
                                                    debug="YES" )
                if success != 0:
                    return self.exit_with_error( 'Fabric submit failed with error {code}', code=success )
            except Exception as e:
                return self.exit_with_error( '{exception}', exception=str(e) )
        else:
            print '{platform} deploy not implemented yet'.format( platform=platform )
    

    def _get_current_version( self ):
        """
        Returns the current version of the app.
        This is taken from the Xcode project.
        """
        plist = plistlib.readPlist( self.IOS_PLIST_PATH )
        return str( plist.get( 'CFBundleShortVersionString', '' ) )
    

    def _set_current_version( self, new_version, commit=False ):
        """
        Update the current version on all platforms.
        """
        # Convert version into a build number for iOS and Android
        build_version = self._get_build_number_from_version( new_version )

        print 'New version:', new_version, '(' + build_version + ')'

        # ios
        plist = plistlib.readPlist( self.IOS_PLIST_PATH )
        plist['CFBundleVersion'] = build_version
        plist['CFBundleShortVersionString'] = new_version
        plistlib.writePlist( plist, self.IOS_PLIST_PATH )

        # android
        print 'SET ANDROID VERSION NOT IMPLEMENTED YET'

        # Commit automatically?
        if commit:
            self.exec_system_command( 'git commit -a -m "Version {version}"', version=new_version )

            if self.auto_push:
                self.exec_system_command( 'git push origin {branch}', branch=self._get_current_branch() )
    

    def _increment_version( self, version, patch=False, minor=False, major=False ):
        """
        Increment a version by patch, minor or major version number.
        """
        if patch or minor or major:
            version_parts = [ int(v) for v in version.split( '.' ) ]

            # The most prominent version bump takes precendence
            if major:
                version_parts[0] += 1
                version_parts[1] = 0
                version_parts[2] = 0
            elif minor:
                version_parts[1] += 1
                version_parts[2] = 0
            elif patch:
                version_parts[2] += 1

            # Convert back to string
            version = '.'.join( str(v) for v in version_parts )

        return version
    

    def _get_build_number_from_version( self, version ):
        """
        Returns a build number for version.
        The build number is a numerical value which always increases in value.
        """
        # Compute code from version. Each version part is considered an integer between 0 and 999
        version_parts = version.split( '.' )
        def pad_version_num( value ):
            while len( value ) < 3:
                value = '0' + value
            return value
        return '1' + pad_version_num( version_parts[0] ) + pad_version_num( version_parts[1] ) + pad_version_num( version_parts[2] )
    

    def _get_command_args_parser( self ):
        """
        Returns an ArgumentParser that can be used to parse command line args.
        """
        parser = argparse.ArgumentParser()
        subparsers = parser.add_subparsers( dest='action', title='action to perform', help='action to perform' )
        subparsers.required = True

        platform_options = self.PLATFORMS + ( 'all', )

        # Build the app
        build_commands = subparsers.add_parser( 'build', help='build the app' )
        build_commands.add_argument( '-p', '--platform', help='platform(s) to build', choices=platform_options, nargs='+', required=True )

        # Clean build files
        clean_commands = subparsers.add_parser( 'clean', help='clean the build files' )
        clean_commands.add_argument( '-p', '--platform', help='platform(s) to clean', choices=platform_options, nargs='+', required=True )

        # Deploy the app to Fabric
        deploy_commands = subparsers.add_parser( 'deploy', help='deploy the app to Fabric' )
        deploy_commands.add_argument( '-p', '--platform', help='platform(s) to deploy', choices=platform_options, nargs='+', required=True )
        deploy_commands.add_argument( '--patch', help='automatically create a new Path version', action='store_true' )
        deploy_commands.add_argument( '--rebuild', help='force a rebuild, otherwise a build will only happen if needed', action='store_true' )

        # Create a new release
        release_commands = subparsers.add_parser( 'new_release', help='create a new release' )
        release_commands.add_argument( '--major', help='create a new Major version release', action='store_true' )
        release_commands.add_argument( '--minor', help='create a new Minor version release', action='store_true' )

        # Get current version
        subparsers.add_parser( 'version', help='get the app version' )

        return parser
    
    
    def exec_system_command( self, command, **kwargs ):
        """
        Runs os.system
        """
        full_command = command.format( **kwargs )
        print full_command
        return 0 # return os.system( full_command )
    
    
    def exit_with_error( self, error, **kwargs ):
        """
        Prints an error message to stderr and exit the script.
        """
        print >> sys.stderr, 'ERROR:', error.format( **kwargs )
        return sys.exit( 1 )
    

    def _find_git_root( self, current_dir ):
        """
        Returns the root path of the git repository.
        """
        def find_git_dir( path ):
            # Does the .git directory exist?
            if os.path.isdir( os.path.join( path, '.git' ) ):
                return path
            # Search next parent up
            return find_git_dir( os.path.dirname( path ) )
        
        return find_git_dir( current_dir )
    
    
    def _get_current_branch( self ):
        """
        Returns the active branch on repo.
        """
        if self.git_repo.head.is_detached:
            return str( self.git_repo.git.describe( tags=True ) )
        else:
            return str( self.git_repo.active_branch )


if __name__ == '__main__':
    project = AzoomeeApp()
    project.main()
