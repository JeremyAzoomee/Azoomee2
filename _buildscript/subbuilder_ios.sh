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

