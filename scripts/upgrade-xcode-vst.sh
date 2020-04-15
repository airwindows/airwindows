#!/bin/sh

if [[ -z $VST2SDK_DIR ]]; then
    echo "VST2SDK_DIR must be set to a valid VST location"
    exit 1
fi

if [[ ! -d $1 ]]; then
    echo "USAGE: upgrade-xcode.sh foo.xcodeproj"
    exit 2
fi

if [[ ! -f $1/project.pbxproj ]]; then
    echo "$1 is not an xcode project. project.pbxproj does not exist"
    exit 3
fi


perl -i -p -e "s:([\"\s])/vstsdk2.4:\1$VST2SDK_DIR:" $1/project.pbxproj
perl -i -p -e 's/= 10\.4/= 10.11/' $1/project.pbxproj
perl -i -p -e 's:SDKROOT\s*=.*;:SDKROOT = /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.15.sdk;:' $1/project.pbxproj
perl -i -p -e 's/i386/x86_64/' $1/project.pbxproj

xcodebuild build -configuration Release -project $1 SYMROOT=../../../xcode-output/$1/build

echo "Restoring xcode project to prior state"
git checkout master $1
