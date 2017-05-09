if [ "$1" == "" ] || [ "$2" == "" ] ; then
	echo "No version number given!"
	echo "Usage: ./build.sh versionnumber buildnumber"
	echo "Example: ./build.sh 3.2.2 0"
	exit
fi

git checkout master
git pull

FILE1=$(md5 -q ./referenceiap.json)
FILE2=$(md5 -q ../Resources/res/configuration/IapConfiguration.json)

if [ "$FILE1" != "$FILE2" ]; then
	echo "IAP configuration mismatch!"
	exit
fi

VER1=$(echo $1| cut -d'.' -f 1)
VER2=$(echo $1| cut -d'.' -f 2)
VER3=$(echo $1| cut -d'.' -f 3)

ARMBUILD=$VER1$VER2$VER3$2"01"
AMAZONBUILD=$VER1$VER2$VER3$2"04"
ARM64BUILD=$VER1$VER2$VER3$2"02"
X86BUILD=$VER1$VER2$VER3$2"03"

COMMITID=$(git log --format="%H" -n 1 |cut -c1-8)

echo "Version number for build: $1 ($COMMITID)"

echo "{\"version\": \"$1 ($COMMITID)\"}" > ../Resources/res/configuration/Version.json

#----------------------------------------------------------------------------------------
#START BUILDING

#$VERSIONNUMBER$ - version number
#$BUILDNUMBER$ - build number
#$STRUCTURENAME$ - structure name (armeabi-v7a, arm64-v8a, x86)
#$XWALKSTRUCTURENAME$ - structure name for xwalk (arm, arm64, x86)

./subbuilder_android.sh armeabi-v7a $1 $ARMBUILD arm
./subbuilder_android.sh arm64-v8a $1 $ARM64BUILD arm64
./subbuilder_android.sh x86 $1 $X86BUILD x86
./subbuilder_android.sh armeabi-v7a $1 $AMAZONBUILD arm