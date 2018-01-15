if [ "$1" == "" ]; then
        echo "Don't start this file as standalone!"
        echo "Use build.sh instead"
        echo "Example: ./build.sh 3.2.2 0"
        exit
fi

VERSIONNUMBER=$1
BUILDNUMBER=$2

BUILDNUMBER=$((BUILDNUMBER+1))

plutil -replace CFBundleShortVersionString -string $VERSIONNUMBER ../proj.ios_mac/ios/Info.plist
plutil -replace CFBundleVersion -string $BUILDNUMBER ../proj.ios_mac/ios/Info.plist

rm -rf ../_builds/azoomee2.xcarchive

xcodebuild clean -project ../proj.ios_mac/azoomee2.xcodeproj -configuration Release -alltargets
xcodebuild archive -project ../proj.ios_mac/azoomee2.xcodeproj -scheme azoomee2-mobile -archivePath ../_builds/azoomee2.xcarchive

osascript -e 'tell app "System Events" to display notification "iOS build is created" with title "Azoomee"'
