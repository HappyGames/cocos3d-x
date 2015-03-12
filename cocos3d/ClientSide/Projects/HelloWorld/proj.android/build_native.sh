APPNAME="CrazyTetris"

# options

buildexternalsfromsource=

usage(){
cat << EOF
usage: $0 [options]

Build C/C++ code for $APPNAME using Android NDK

OPTIONS:
-s	Build externals from source
-h	this help
EOF
}

while getopts "sh" OPTION; do
case "$OPTION" in
s)
buildexternalsfromsource=1
;;
h)
usage
exit 0
;;
esac
done

# paths

if [ -z "${NDK_ROOT+aaa}" ];then
echo "please define NDK_ROOT"
exit 1
fi

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# common macros definition go here
# ... use paths relative to current directory
COCOS2DX_ROOT="$DIR/../../../../engine/cocos2dx/"
MMUTIL_ROOT="$DIR/../../../../engine/mmUtil/"
ZLIB_ROOT="$DIR/../../../../engine/dependencies/zlib/"
COCOSLIB_ROOT="$DIR/../../../../engine/utilities/CocosLib/"
UTILITIES_ROOT="$DIR/../../../../engine/utilities/"
HAWKUTIL_ROOT="$DIR/../../../../engine/utilities/HawkUtil/"
APP_ROOT="$DIR/.."
APP_ANDROID_ROOT="$DIR"
DEPENDENCIES_ROOT="$DIR/../../../../engine/dependencies/"

echo "NDK_ROOT = $NDK_ROOT"
echo "COCOS2DX_ROOT = $COCOS2DX_ROOT"
echo "APP_ROOT = $APP_ROOT"
echo "APP_ANDROID_ROOT = $APP_ANDROID_ROOT"
echo "MMUTIL_ROOT = $MMUTIL_ROOT"
echo "ZLIB_ROOT = $ZLIB_ROOT"
echo "COCOSLIB_ROOT = $COCOSLIB_ROOT"

# make sure assets is exist
if [ -d "$APP_ANDROID_ROOT"/assets ]; then
    rm -rf "$APP_ANDROID_ROOT"/assets
fi

mkdir "$APP_ANDROID_ROOT"/assets

# copy resources
for file in "$APP_ROOT"/Resources/*
do
if [ -d "$file" ]; then
    cp -rf "$file" "$APP_ANDROID_ROOT"/assets
fi

if [ -f "$file" ]; then
    cp "$file" "$APP_ANDROID_ROOT"/assets
fi
done

# copy icons (if they exist)
file="$APP_ANDROID_ROOT"/assets/Icon-72.png
if [ -f "$file" ]; then
	cp "$file" "$APP_ANDROID_ROOT"/res/drawable-hdpi/icon.png
fi
file="$APP_ANDROID_ROOT"/assets/Icon-48.png
if [ -f "$file" ]; then
	cp "$file" "$APP_ANDROID_ROOT"/res/drawable-mdpi/icon.png
fi
file="$APP_ANDROID_ROOT"/assets/Icon-32.png
if [ -f "$file" ]; then
	cp "$file" "$APP_ANDROID_ROOT"/res/drawable-ldpi/icon.png
fi


if [[ "$buildexternalsfromsource" ]]; then
    echo "Building external dependencies from source"
    "$NDK_ROOT"/ndk-build -C "$APP_ANDROID_ROOT" $* \
        "NDK_MODULE_PATH=${COCOS2DX_ROOT}:\
${COCOS2DX_ROOT}/cocos2dx/platform/third_party/android/source:\
${MMUTIL_ROOT}:\
${MMUTIL_ROOT}/mmUtil:\
${UTILITIES_ROOT}:\
${COCOSLIB_ROOT}:\
${COCOSLIB_ROOT}/common:\
${COCOSLIB_ROOT}/effect:\
${COCOSLIB_ROOT}/extension:\
${COCOSLIB_ROOT}/ui:\
${HAWKUTIL_ROOT}:\
${DEPENDENCIES_ROOT}:\
${ZLIB_ROOT}"

else
    echo "Using prebuilt externals"
    "$NDK_ROOT"/ndk-build -C "$APP_ANDROID_ROOT" $* \
        "NDK_MODULE_PATH=${COCOS2DX_ROOT}:\
${COCOS2DX_ROOT}/cocos2dx/platform/third_party/android/prebuilt:\
${MMUTIL_ROOT}:\
${MMUTIL_ROOT}/mmUtil:\
${UTILITIES_ROOT}:\
${COCOSLIB_ROOT}:\
${COCOSLIB_ROOT}/common:\
${COCOSLIB_ROOT}/effect:\
${COCOSLIB_ROOT}/extension:\
${COCOSLIB_ROOT}/ui:\
${HAWKUTIL_ROOT}:\
${DEPENDENCIES_ROOT}:\
${ZLIB_ROOT}"

fi
