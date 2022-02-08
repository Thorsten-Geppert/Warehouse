#!/usr/bin/env sh
/usr/bin/defaults write com.apple.dock persistent-apps -array-add '<dict><key>tile-data</key><dict><key>file-data</key><dict><key>_CFURLString</key><string>/Applications/LicenceGenerator.app</string><key>_CFURLStringType</key><integer>0</integer></dict></dict></dict>'
osascript -e 'tell application "Dock" to quit'
