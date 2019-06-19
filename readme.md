# Azoomee Mobile App

## Getting Started

The latest Mac OS X is the recommended development environment. While Windows is technically possible for Android, it isn't recommended nor supported for Azoomee development.

Clone repo recursively

    $ git clone --recurse-submodules git@bitbucket.org:azoomee/app-cocos-azoomee2.git

In cocos2d repo download the dependencies (stored locally and not in git) 

    $ cd cocos2d
    $ ./download-deps.py

## iOS

Download Xcode. You should always be running the latest (non Beta) version of Xcode and iOS SDK. 

Open the project at: `proj.ios_mac/Azoomee.xcodeproj`

### Targets

There are two main targets:

* Azoomee Prod (live environment)
* Azoomee CI (CI environment)

### Some important notes
* When adding files, be sure to add them to **both** targets. The same applies to **AzoomeeCommon**, which also has a *Prod* and *CI* target.
* When editing build settings, also keep in mind you may need to edit both *Prod* and *CI* targets.
* The Info.plist file is shared between both targets.


## Android

Download Android Studio. You should always be running the latest (non Beta) version of Android Studio.

### Android Studio first use

On first launch of Android Studio, it's recommended to edit the VM settings to increase the available RAM. The Azoomee project may not open otherwise.

From the launch screen click **configure**, or in the main app under **help**, find *"Edit Custom VM Options"*.

Edit or add an entry for **"-Xmx2048m"**. 2048m = 2048mb. You can edit this to whatever value works for your machine but 2048mb or more is recommended for Azoomee project. You might need more.

### Android SDK

Open the SDK Manager to download the Android SDK. Remember to set a sensible location for the Android SDK files, e.g `/Users/XXX/Library/Develop/Android/sdk`

Always update the Android SDK and Build tools when new versions become available. However please note about NDK upgrading below.

### Android NDK

Download Android **NDK r19c** https://developer.android.com/ndk/downloads/older_releases.html

Note the exact version of the NDK. It's very important to be running the same version as the rest of the team, so please don't update automatically to the latest version of the NDK and don't download or upgrade the NDK through Android Studio.

Unzip the ndk files to a sensible location, e.g `/Users/XXX/Library/Develop/Android/ndk`

Edit the Android SDK soft link to point to the ndk files at a custom location:

    $ cd /Users/XXX/Library/Develop/Android/sdk
    $ rm ndk-bundle
    $ ln -s /Users/XXX/Library/Develop/Android/ndk/android-ndk-r19c ndk-bundle

You may need to restart Android Studio after doing this.

### Build Variants

You'll find various targets under the "Build Variants" window, based on different builds of Azoomee:


`<build type><environment><Debug|Release>-<arch>`

e.g

* `googleplayProdRelease-armeabi-v7a`
* `googleplayCIDebug-armeabi-v7a`
* `amazonProdRelease-armeabi-v7a`

These are configured in the Azoomee build.gradle file, as productFlavors. Each version is very similar, the main build difference being the extra cpp flags passed, such as AZOOMEE_AMAZON_BUILD define for an Amazon build, for example.

When building a build variant, please note: **The arch variant selected does NOT dictate which arch is built**. The build system builds ALL archs for that productFlavor, and archs are only displayed due to a bug/feature in Android Studio.

Since Google Play includes 32bit and 62bit archs, it will take twice the time to build. For day to development, it's recommend you use the "standard" variants which only build 32bit.

### local.properties not found

If you get an error relating to **local.properties** and the file doesn't exist, open the project in Android Studio. Android Studio should generate this file automatically. 

If the file exists and you're having problems, try deleting the file and re-openingt the project to generate a fresh version.


# Preprocessor macros

The following macros are worth noting for development:

* AZOOMEE_ENVIRONMENT_CI (built to use CI server environment)

The omission of this define means we're built to use the production servers).

This define is available in the main app and common targets. **NOTE: It isn't available in any other sub projects.**

Android only:

* AZOOMEE_AMAZON_BUILD
* AZOOMEE_VODACOM_BUILD
* AZOOMEE_HUAWEI_BUILD

The omission of any of these means we're built for a standard Google Play device.
These defines are only available in the main app. **NOTE: Not available in common or any other sub projects.**


# Deployment


iOS and Android builds are deployed for testing through Fabric/Crashlytics.
https://fabric.io/home

Please ask a member of the development team for an invite to the Fabric Team in order to manage builds and review Crashlytics (Crash reporting) data.


Deployment of the app is done through the `project.py` script, located in the root of the repo.

Tip: You can run `./project.py --help` to see a list of available commands. You can also run this for each command, e.g `./project.py new_release --help`

## Common tasks

### 1. End of a release.

When a release has finished, it as recommended to:

1. Create a git tag named after the version, e.g 4.5.1
2. PR the release branch into master, closing the release branch on merge.


### 2. Starting a new release. 

We do this at the start of each sprint. Usually we bump the minor version, which is done using the following command:

    $ ./project.py new_release --minor

Note: You must be on master, or a release branch, in order to create the new release. In addition, please make sure the "end of a release" tasks have been followed and you're on the latest release before running this command. Only one developer needs to run this so please co-ordinate with development team members.

Running this command will:

1. Bump the version number, updating the Android and iOS projects.
2. Clear the changelog file.
3. Create a commit, and push a new release branch named after the version, e.g `release/3.4`.

All work for this release should then by done from that branch.


### 3. Deploying builds to Fabric for testing.

Each build uploaded to Fabric should have it's version number bumped. This can be done in a couple of ways.

Either you can run the new_release command (recommended):

    $ ./project.py new_release --patch

Or you can append `--patch` to the deploy command. Please note if you're bumping the patch number as part of the deploy and running deploy multiple times, only add it for the first deploy command you issue.

When the patch number is increased, you will be asked in the terminal for the change notes.
It's recommended you check the git history and provide a high level summary. Keep in mind this change log is visible by developers and non developers alike. 

The entire changelog for this major.minor version is included in every build, so you only need to summarise what's new since the last build.

When you're finished entering the changelog, enter the character ":" on a new line. 
After doing this, your entries will be added to the changelog file and a commit will be created automatically.


As for deploying, here are some common commands.

Deploy iOS build only:

    $ ./project.py deploy -p ios

Deploy Google Play Android build only:

    $ ./project.py deploy -p googleplay

Deploy Google Play and iOS builds:

    $ ./project.py deploy -p googleplay ios

Deploy all platforms (as of writing: Google Play, Amazon, Huawei, Vodacom, iOS):

    $ ./project.py deploy -p all

Generally it's recommended for day-to-day testing to deploy only Google Play and iOS builds, since they are the most common. This might take around 1 hour to complete.

If the deploy fails, please check carefully for errors in the terminal. You might need to scroll up to find the cause. Common failures are usually compile errors.

If you need to make any fixes, don't forget you'll need to raise a PR and restart the deployment process again after merging into the release branch.