if [ "$1" == "" ]; then
        echo "Don't start this file as standalone!"
        echo "Use build.sh instead"
        echo "Example: ./build.sh 3.2.2 0"
        exit
fi

STRUCTURENAME=$1
VERSIONNUMBER=$2
BUILDNUMBER=$3
XWALKSTRUCTURENAME=$4

sed 's/_STRUCTURENAME_/'"$STRUCTURENAME"'/g' ./exampleapplication > ./Application.mk
sed 's/_STRUCTURENAME_/'"$STRUCTURENAME"'/g' ./examplebuildgradle > ./tempgradle.temp
sed 's/_VERSIONNUMBER_/'"$VERSIONNUMBER"'/g' ./tempgradle.temp > ./temp2gradle.temp
sed 's/_BUILDNUMBER_/'"$BUILDNUMBER"'/g' ./temp2gradle.temp > ./temp3gradle.temp
sed 's/_XWALKSTRUCTURENAME_/'"$XWALKSTRUCTURENAME"'/g' ./temp3gradle.temp > build.gradle 

rm -rf *.temp

cp -r ./Application.mk ../proj.android-studio/app/jni
rm -rf ./Application.mk

cp -r ./build.gradle ../proj.android-studio/app
rm -rf ./build.gradle

cocos compile -p android --android-studio -m release

cp -r ../bin/release/android/app-release-signed.apk ../_builds/build-$BUILDNUMBER.apk
rm -rf ../bin/release/android/app-release-signed.apk
