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

if [ "$AMAZON" == "samsung" ]; then
  cp ../proj.android-studio/app/AndroidManifest.xml ../_replaceTestResults/AndroidManifest.xml
	sed -i.bak 's/<!--manifestHeader-->.*/<!--manifestHeader--><manifest xmlns:android="http://schemas.android.com/apk/res/android" package="com.tinizine.azoomee" android:versionCode="'$BUILDNUMBER'" android:versionName="'$VERSIONNUMBER'" android:installLocation="auto">/g' ../_replaceTestResults/AndroidManifest.xml
  sed -i.bak 's/<!--samsungFeaturesPlace-->.*/<!--samsungFeaturesPlace--><uses-feature android:name="android.hardware.nfc " android:required="false"/><uses-feature android:name="android.hardware.touchscreen" android:required="true"/>/g' ../_replaceTestResults/AndroidManifest.xml
  sed -i.bak 's/<!--samsungPermissionsPlace-->.*/<!--samsungPermissionsPlace--><uses-permission android:name="com.samsung.android.providers.context.permission.WRITE_USE_APP_FEATURE_SURVEY"/>/g' ../_replaceTestResults/AndroidManifest.xml
  cp -r ../_replaceTestResults/AndroidManifest.xml ../proj.android-studio/app/AndroidManifest.xml
fi

cocos compile -p android --android-studio -m release

cp -r ../bin/release/android/app-release-signed.apk ../_builds/build-$BUILDNUMBER$AMAZON.apk
rm -rf ../bin/release/android/app-release-signed.apk

if [ "$AMAZON" == "amazon" ] || [ "$AMAZON" == "samsung" ] ; then
  cp -r ../_replaceTestResults/AndroidManifest.xml.bak ../proj.android-studio/app/AndroidManifest.xml
fi

rm -rf ../_replaceTestResults
