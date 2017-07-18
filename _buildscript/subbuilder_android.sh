if [ "$1" == "" ]; then
        echo "Don't start this file as standalone!"
        echo "Use build.sh instead"
        echo "Example: ./build.sh -v 3.2.2 -b 0 [-p ios|android] [-u]"
        exit
fi

STRUCTURENAME=$1
VERSIONNUMBER=$2
BUILDNUMBER=$3
XWALKSTRUCTURENAME=$4
UPLOAD=$5

rm -rf ../_replaceTestResults
mkdir ../_replaceTestResults

cp ../proj.android-studio/app/jni/Application.mk ../_replaceTestResults/Application.mk
cp ../proj.android-studio/app/build.gradle ../_replaceTestResults/build.gradle

sed -i.bak 's/APP_ABI :=.*/APP_ABI := '$STRUCTURENAME'/g' ../_replaceTestResults/Application.mk
sed -i.bak 's/APP_ABI :=.*/APP_ABI := '$STRUCTURENAME'/g' ../_replaceTestResults/build.gradle
sed -i.bak 's/versionName .*/versionName '\'$VERSIONNUMBER\''/g' ../_replaceTestResults/build.gradle
sed -i.bak 's/versionCode .*/versionCode '$BUILDNUMBER'/g' ../_replaceTestResults/build.gradle
sed -i.bak 's/include .*/include '\'$STRUCTURENAME\''/g' ../_replaceTestResults/build.gradle

sed -i.bak 's/org.xwalk:xwalk_core_library:\([0-9.]*\)[^'\'']*/org.xwalk:xwalk_core_library:\1-'$XWALKSTRUCTURENAME'/g'  ../_replaceTestResults/build.gradle
sed -i.bak 's/include '\''[^'\'']'\''$/include '\'$STRUCTURENAME\''/g' ../_replaceTestResults/build.gradle

cp -r ../_replaceTestResults/Application.mk ../proj.android-studio/app/jni/Application.mk
cp -r ../_replaceTestResults/build.gradle ../proj.android-studio/app/build.gradle

cocos compile -p android --android-studio -m release

if [ "$UPLOAD" == "upload" ]; then
	( cd ../proj.android-studio/ ; ./gradlew publishApkRelease )
fi

cp -r ../bin/release/android/app-release-signed.apk ../_builds/build-$BUILDNUMBER.apk
rm -rf ../bin/release/android/app-release-signed.apk
rm -rf ../_replaceTestResults