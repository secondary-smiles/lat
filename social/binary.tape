Require lat
Output social/render/binary.gif

Set Theme "Gruvbox Dark"
Set WindowBar Rings
Set BorderRadius 10
Set Margin 10

Set FontSize 32
Set Width 2400
Set Height 1200
Set TypingSpeed 0.2
Set PlaybackSpeed 0.5

Type "lat src/main.c"
Sleep 2s
Enter

Sleep 5s

Type "clear"
Sleep 1s
Enter

Sleep 2s

Type "lat -b src/main.c"
Sleep 2s
Enter

Sleep 5s

Type "clear"
Sleep 1s
Enter

Sleep 2s

Type "lat ./build/lat"
Sleep 2s
Enter

Sleep 5s

Type "clear"
Sleep 1s
Enter

Sleep 2s

Type "lat -bb ./build/lat"
Sleep 2s
Enter

Sleep 5s
