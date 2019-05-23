#!/usr/bin/python
import sys, os, re
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
    ANDROID_PROJECT_DIR = os.path.join( os.path.dirname( os.path.abspath( __file__ ) ), 'proj.android' )
    ANDROID_REQUIRED_NDK_VERSION = '16.1'
    ANDROID_BUILD_OUTPUTS_DIR = os.path.join( ANDROID_PROJECT_DIR, 'app', 'build', 'outputs' )

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
        'test_groups' : 'mitch-test', # azoomee-internal
        # Path to the iOS Framework
        'ios_framework_path' : os.path.join( IOS_PROJECT_DIR, 'Crashlytics.framework' )
    }

    # Directory to place built binaries
    BUILD_DIR = os.path.join( os.path.dirname( os.path.abspath( __file__ ) ), 'build' )

    # The root of the Git repo
    GIT_ROOT = None
    git_repo = None
    auto_push = True

    # Platforms
    PLATFORMS = (
        'ios',
        'googleplay',
        'amazon'
        'vodacom'
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

        current_version = self._get_current_version()

        # Check we are on the correct branch to create a new release
        major_minor_version = current_version[ 0 : current_version.rfind('.') ]
        valid_branches = [ 'release/' + major_minor_version ]

        # Major/minor releases can also be created from Master
        if args.major or args.minor:
            valid_branches += [ 'master' ]

        current_branch = self._get_current_branch()
        if current_branch not in valid_branches:
            return self.exit_with_error( 'You must be on one of the following branches in order to create a new release: {branches}', branches=', '.join( valid_branches ) )


        # Update version
        if args.patch:
            new_version = self._increment_version( current_version, patch=True )
            self._set_current_version( new_version, commit=True )
        else:
            new_version = self._increment_version( current_version, major=args.major, minor=args.minor )

            # Create a new release branch
            new_major_minor_version = new_version[ 0 : new_version.rfind('.') ]
            new_release_branch = 'release/' + new_major_minor_version
            self.exec_system_command( 'git checkout -b "{branch}"', branch=new_release_branch )
            self.exec_system_command( 'git push --set-upstream origin "{branch}"', branch=new_release_branch )

            self._set_current_version( new_version, commit=True )

    
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
            self._perform_build( p, force_rebuild=args.rebuild )
    

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
            preturn self.exit_with_error( 'You must be on the branch "{branch}" in order to deploy this version.', branch=required_branch )

        # Do we need to update the version?
        if args.patch:
            current_version = self._increment_version( current_version, patch=True )
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
        elif platform == 'googleplay':
            self._check_android_environment()

            restore_cwd = os.getcwd()
            os.chdir( self.ANDROID_PROJECT_DIR )

            try:
                success = self.exec_system_command( './gradlew {gradle_command}', gradle_command='clean' )
                if success != 0:
                    return self.exit_with_error( 'Gradle clean failed with error {code}', code=success )
            except Exception as e:
                return self.exit_with_error( '{exception}', exception=str(e) )
            finally:
                os.chdir( restore_cwd )
        else:
            print '{platform} clean not implemented yet'.format( platform=platform )
    

    def _perform_build( self, platform, force_rebuild=False ):
        """
        Builds app for a single platform.
        """
        platform_build_dir = os.path.join( self.BUILD_DIR, platform )
        if not os.path.exists( platform_build_dir ):
            os.makedirs( platform_build_dir )
        
        build_name = 'Azoomee-v' + self._get_current_version()

        if platform == 'ios':
            build_config = 'Release'

            # Delete any existing built files for this version
            shutil.rmtree( os.path.join( platform_build_dir, build_name + '.ipa' ), ignore_errors=True )
            shutil.rmtree( os.path.join( platform_build_dir, build_name + '.xcarchive' ), ignore_errors=True )

            # If we're forcing a rebuild, do a clean
            if force_rebuild:
                self._perform_clean( platform )

            # Build archive
            archive_path = os.path.join( platform_build_dir, build_name + '.xcarchive' )
            try:
                success = self.exec_system_command( 'xcodebuild archive -project "{project_path}" -configuration "{config}" -scheme "{target}" -archivePath "{archive_path}"', 
                                                    project_path=self.IOS_PROJECT_PATH, 
                                                    target=self.IOS_PROJECT_TARGET, 
                                                    config=build_config, 
                                                    archive_path=archive_path )
                if success != 0:
                    return self.exit_with_error( 'Xcode build failed with error {code}', code=success )
            except Exception as e:
                return self.exit_with_error( '{exception}', exception=str(e) )
            
            # Generate IPA
            ipa_path = os.path.join( platform_build_dir, build_name + '.ipa' )
            try:
                success = self.exec_system_command( 'xcodebuild -exportArchive -allowProvisioningUpdates -archivePath "{archivePath}" -exportPath "{exportPath}" -exportOptionsPlist "{exportOptions}"', 
                                                    archivePath=archive_path, 
                                                    exportPath=ipa_path,
                                                    exportOptions=self.IOS_EXPORT_OPTIONS_PATH )
                if success != 0:
                    return self.exit_with_error( 'Xcode build failed with error {code}', code=success )
            except Exception as e:
                return self.exit_with_error( '{exception}', exception=str(e) )
        elif platform == 'googleplay':
            self._check_android_environment()
            build_config = 'release'

            # Delete any existing built files for this version
            shutil.rmtree( os.path.join( platform_build_dir, build_name + '.apk' ), ignore_errors=True )
            shutil.rmtree( os.path.join( platform_build_dir, build_name + '.aab' ), ignore_errors=True )

            restore_cwd = os.getcwd()

            try:
                os.chdir( self.ANDROID_PROJECT_DIR )
                gradle_cmd = 'assemble{build_config} crashlyticsUploadSymbols{build_config}'.format( build_config=build_config.capitalize() )
                success = self.exec_system_command( './gradlew {gradle_command}', gradle_command=gradle_cmd )
                if success != 0:
                    return self.exit_with_error( 'Gradle APK build failed with error {code}', code=success )
            except Exception as e:
                return self.exit_with_error( '{exception}', exception=str(e) )
            finally:
                os.chdir( restore_cwd )

            # Copy APK
            apk_name = 'Azoomee-%s.apk' % build_config
            apk_source_path = os.path.join( self.ANDROID_BUILD_OUTPUTS_DIR, 'apk', build_config, apk_name )
            apk_dest_path = os.path.join( platform_build_dir, build_name + '.apk' )
            shutil.copyfile( apk_source_path, apk_dest_path )

            try:
                os.chdir( self.ANDROID_PROJECT_DIR )
                success = self.exec_system_command( './gradlew {gradle_command}', gradle_command=':Azoomee:bundle' + build_config.capitalize() )
                if success != 0:
                    return self.exit_with_error( 'Gradle BUNDLE build failed with error {code}', code=success )
            except Exception as e:
                return self.exit_with_error( '{exception}', exception=str(e) )
            finally:
                os.chdir( restore_cwd )

            # Copy Bundle
            bundle_name = 'Azoomee.aab'
            bundle_source_path = os.path.join( self.ANDROID_BUILD_OUTPUTS_DIR, 'bundle', build_config, bundle_name )
            bundle_dest_path = os.path.join( platform_build_dir, build_name + '.aab' )
            shutil.copyfile( bundle_source_path, bundle_dest_path )
        else:
            print '{platform} build not implemented yet'.format( platform=platform )
    

    def _perform_deploy( self, platform, force_rebuild=False ):
        """
        Deploy app for testing for a single platform.
        """
        # TODO: Create changelog notes file by summarising all commits from the current release branch
        notes_path = ""
        
        if platform == 'ios':
            platform_build_dir = os.path.join( self.BUILD_DIR, platform )
            build_name = 'Azoomee-v' + self._get_current_version()
            
            # If we're forcing a rebuild, do a clean and delete the existing archive and ipa
            if force_rebuild:
                self._perform_clean( platform )
                shutil.rmtree( os.path.join( platform_build_dir, build_name + '.ipa' ), ignore_errors=True )
                shutil.rmtree( os.path.join( platform_build_dir, build_name + '.xcarchive' ), ignore_errors=True )

            ipa_path = os.path.join( platform_build_dir, build_name + '.ipa', self.IOS_PROJECT_TARGET + '.ipa' )

            # Build if needed
            if not os.path.isfile( ipa_path ):
                self._perform_build( platform )

            # If we don't have an ipa file now, we can't deploy anything
            if not os.path.isfile( ipa_path ):
                return self.exit_with_error( 'Unable to deploy, file not found: {file}', file=ipa_path )

            # Now upload the IPA file

            # Do Fabric upload
            try: 
                # TODO: Include all options in command
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
                    return self.exit_with_error( 'Fabric submit iOS buildfailed with error {code}', code=success )
            except Exception as e:
                return self.exit_with_error( '{exception}', exception=str(e) )
        elif platform == 'googleplay':
            self._check_android_environment()
            build_config = 'release'

            # If we're forcing a rebuild, do a clean
            if force_rebuild:
                self._perform_clean( platform )

            restore_cwd = os.getcwd()

            try:
                os.chdir( self.ANDROID_PROJECT_DIR )
                gradle_cmd = 'assemble{build_config} crashlyticsUploadSymbols{build_config} crashlyticsUploadDistribution{build_config}'.format( build_config=build_config.capitalize() )
                success = self.exec_system_command( './gradlew {gradle_command}', gradle_command=gradle_cmd )
                if success != 0:
                    return self.exit_with_error( 'Fabric submit Android build failed with error {code}', code=success )
            except Exception as e:
                return self.exit_with_error( '{exception}', exception=str(e) )
            finally:
                os.chdir( restore_cwd )
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
        gradle_file = os.path.join( self.ANDROID_PROJECT_DIR, 'app', 'build.gradle' )
        gradle_str = None
        with open( gradle_file, 'r' ) as fh:
            gradle_str = fh.read()
        
        version_code_match = re.search( r'versionCode ([^\n]+)', gradle_str, flags=re.MULTILINE )
        version_name_match = re.search( r'versionName "([\d.]+)"', gradle_str, flags=re.MULTILINE )
        
        version_code_new = 'versionCode %s' % build_version
        version_name_new = 'versionName "%s"' % new_version

        # Replace
        gradle_str = gradle_str.replace( version_code_match.group( 0 ), version_code_new )
        gradle_str = gradle_str.replace( version_name_match.group( 0 ), version_name_new )

        # Save out new gradle file
        with open( gradle_file, 'w+' ) as fh:
            fh.write( gradle_str )

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
        build_commands.add_argument( '--rebuild', help='force a rebuild, otherwise a build will only happen if needed', action='store_true' )

        # Clean build files
        clean_commands = subparsers.add_parser( 'clean', help='clean the build files' )
        clean_commands.add_argument( '-p', '--platform', help='platform(s) to clean', choices=platform_options, nargs='+', required=True )

        # Deploy the app to Fabric
        deploy_commands = subparsers.add_parser( 'deploy', help='deploy the app to Fabric' )
        deploy_commands.add_argument( '-p', '--platform', help='platform(s) to deploy', choices=platform_options, nargs='+', required=True )
        deploy_commands.add_argument( '--patch', help='automatically create a new Patch version (recommended)', action='store_true' )
        deploy_commands.add_argument( '--rebuild', help='force a rebuild, otherwise a build will only happen if needed', action='store_true' )

        # Create a new release
        release_commands = subparsers.add_parser( 'new_release', help='create a new release' )
        release_commands.add_argument( '--major', help='create a new Major version release', action='store_true' )
        release_commands.add_argument( '--minor', help='create a new Minor version release', action='store_true' )
        release_commands.add_argument( '--patch', help='create a new Patch version release', action='store_true' )

        # Get current version
        subparsers.add_parser( 'version', help='get the app version' )

        return parser
    
    
    def exec_system_command( self, command, **kwargs ):
        """
        Runs os.system
        """
        full_command = command.format( **kwargs )
        print full_command
        return os.system( full_command )
    
    
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
    

    def _check_android_environment( self ):
        """
        Checks the Android build environment is setup correctly.
        """
        os.environ["_JAVA_OPTIONS"] = '-Djava.awt.headless=true'

        local_props_path = os.path.join( self.ANDROID_PROJECT_DIR, 'local.properties' )
        local_props = self._load_gradle_properties( local_props_path )
        ndk_root = local_props['ndk.dir']

        if not self._verify_android_ndk_version( ndk_root ):
            return self.exit_with_error( 'Error validating NDK version. Expected NDK version is: {ndk}', ndk=self.ANDROID_REQUIRED_NDK_VERSION )
    

    def _verify_android_ndk_version( self, ndk_root ):
        """
        Checks the installd ndk version is valid.
        """
        source_props_path = os.path.join( ndk_root, 'source.properties' )
        if os.path.exists( source_props_path ):
            file_data = None
            with open( source_props_path, 'r' ) as fh:
                file_data = fh.read().strip()
            
            expected_match = 'Pkg.Revision = ' + self.ANDROID_REQUIRED_NDK_VERSION
            return expected_match in file_data
        
        return False
    

    def _load_gradle_properties( self, path ):
        """
        Loads a dictionary representation of a gradle properties file.
        """
        props = {}
        with open( path, 'rt' ) as f:
            for line in f:
                l = line.strip()
                if l and not l.startswith( '#' ):
                    key_value = l.split( '=' )
                    props[key_value[0].strip()] = key_value[1].strip( '" \t' ) 
        return props


if __name__ == '__main__':
    project = AzoomeeApp()
    project.main()
