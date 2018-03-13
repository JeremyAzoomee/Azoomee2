if [ "$1" == "" ]; then
        echo "Don't start this file as standalone!"
        echo "Use build.sh instead"
        echo "Example: ./build.sh -v 3.2.2 -b 0 [-p ios|android|google|google32|google64|googlex86|amazon|samsung] [-u]"
        exit
fi

STRUCTURENAME=$1
VERSIONNUMBER=$2
BUILDNUMBER=$3
XWALKSTRUCTURENAME=$4
AMAZON=$5

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

if [ "$AMAZON" == "amazon" ]; then
  cp ../proj.android-studio/app/AndroidManifest.xml ../_replaceTestResults/AndroidManifest.xml
	sed -i.bak 's/<!--appsflyerMetaPlace-->.*/<!--appsflyerMetaPlace--><meta-data android:name="CHANNEL" android:value="Amazon"\/>/g' ../_replaceTestResults/AndroidManifest.xml
  cp -r ../_replaceTestResults/AndroidManifest.xml ../proj.android-studio/app/AndroidManifest.xml
fi

cocos compile -p android --android-studio -m release

cp -r ../bin/release/android/app-release-signed.apk ../_builds/build-$BUILDNUMBER$AMAZON.apk
rm -rf ../bin/release/android/app-release-signed.apk

if [ "$AMAZON" == "amazon" ]; then
  cp -r ../_replaceTestResults/AndroidManifest.xml.bak ../proj.android-studio/app/AndroidManifest.xml
fi

rm -rf ../_replaceTestResults

osascript -e 'tell app "System Events" to display notification "Build '$BUILDNUMBER' '$AMAZON' is created" with title "Azoomee"'
