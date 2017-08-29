while getopts v:b:p:ud option
do
 case "${option}"
 in
 v) VERSION=${OPTARG};;
 b) BUILD=${OPTARG};;
 p) PLATFORM=${OPTARG};;
 d) DEVMODE="devmode";;
 u) UPLOAD="upload";;
 esac
done

if [ "$VERSION" == "" ] || [ "$BUILD" == "" ] ; then
	echo "No version number given!"
	echo "Usage: ./build.sh versionnumber buildnumber"
	echo "Example: ./build.sh -v 3.2.2 -b 0 [-p ios|android] [-u] [-d]"
	exit
fi

if [ "$DEVMODE" != "devmode" ] ; then
	echo "THIS PROCESS WILL RESET ALL UNSTAGED AND NEW FILES FROM CURRENT BRANCH! PRESS CTRL+C IN 15 SECONDS TO CANCEL!"
	sleep 15
	git reset --hard
	git checkout master
	git pull
fi

FILE1=$(md5 -q ./referenceiap.json)
FILE2=$(md5 -q ../Resources/res/configuration/IapConfiguration.json)

if [ "$FILE1" != "$FILE2" ]; then
	echo "IAP configuration mismatch!"
	exit
fi

VER1=$(echo $VERSION| cut -d'.' -f 1)
VER2=$(echo $VERSION| cut -d'.' -f 2)
VER3=$(echo $VERSION| cut -d'.' -f 3)

ARMBUILD=$VER1$VER2$VER3$BUILD"01"
AMAZONBUILD=$VER1$VER2$VER3$BUILD"04"
ARM64BUILD=$VER1$VER2$VER3$BUILD"02"
X86BUILD=$VER1$VER2$VER3$BUILD"03"

COMMITID=$(git log --format="%H" -n 1 |cut -c1-8)

echo "Version number for build: $VERSION ($COMMITID)"

echo "{\"version\": \"$VERSION ($COMMITID)\"}" > ../Resources/res/configuration/Version.json

#----------------------------------------------------------------------------------------
#START BUILDING

#$VERSIONNUMBER$ - version number
#$BUILDNUMBER$ - build number
#$STRUCTURENAME$ - structure name (armeabi-v7a, arm64-v8a, x86)
#$XWALKSTRUCTURENAME$ - structure name for xwalk (arm, arm64, x86)

if [ "$PLATFORM" == "" ] || [ "$PLATFORM" == "android" ] ; then
	./subbuilder_android.sh armeabi-v7a $VERSION $ARMBUILD arm $UPLOAD
	./subbuilder_android.sh arm64-v8a $VERSION $ARM64BUILD arm64 $UPLOAD
	./subbuilder_android.sh x86 $VERSION $X86BUILD x86 $UPLOAD
	./subbuilder_android.sh armeabi-v7a $VERSION $AMAZONBUILD arm
fi

if [ "$PLATFORM" == "" ] || [ "$PLATFORM" == "ios" ] ; then
	./subbuilder_ios.sh $VERSION $BUILD
fi
