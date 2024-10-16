#!/bin/bash

# Variables
APP_PATH="./out/turtle"
ZIP_PATH="./out/turtle.zip"
APP_BUNDLE_ID="com.replicat.turtle"
# Load environment variables from env.src
source /Users/mathieu/Lab/turtle/env.src

# Apple Developer Credentials
NOTARIZE_USERNAME=$APPLE_ID
NOTARIZE_PASSWORD=$APP_SPECIFIC_PASSWORD
# Sign the 
echo "Signing the application..."
# Sign
echo "Signing..."
codesign --force -s "$DEVELOPER_ID" -v $APP_PATH --deep --strict --options=runtime --timestamp || exit 1

# Check signature
echo "Check Signature..."
codesign -dv --verbose=4 $APP_PATH || exit 1

 # Zip 
echo "Zipping..."
rm -f $ZIP_PATH
zip -r $ZIP_PATH $APP_PATH || exit 1

# Cleanup
echo "Cleaning..."
rm -r $APP_PATH || exit 1

# Create a notarization request using notarytool
echo "Notarizing..."
xcrun notarytool submit "$ZIP_PATH" --apple-id "$NOTARIZE_USERNAME" --password "$NOTARIZE_PASSWORD" --team-id "4LHR8CQ7S4" --wait || exit 1

# Unzip!
echo "Unzipping..."
unzip $ZIP_PATH || exit 1

# Staple the notarization ticket to the application
# echo "Stapling..."
# xcrun stapler staple "$APP_PATH" || exit 1

# # Verify the stapling
# echo "Verifying..."
# spctl -a -v "$APP_PATH"