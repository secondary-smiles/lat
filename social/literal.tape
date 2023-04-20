Require lat
Output social/render/literal.gif

Set Theme "Gruvbox Dark"
Set WindowBar Rings
Set BorderRadius 10
Set Margin 10

Set FontSize 32
Set Width 2400
Set Height 1200
Set TypingSpeed 0.2
Set PlaybackSpeed 0.5

Type "lat src/main.c > example.c"
Sleep 2s
Enter

Sleep 5s

Type "cat example.c"
Sleep 2s
Enter

Sleep 5s

Type "clear"
Sleep 1s
Enter

Sleep 2s

Type "lat --literal src/main.c > example.c"
Sleep 2s
Enter

Sleep 3s

Type "cat example.c"
Sleep 2s
Enter

Sleep 5s

Hide
Type "rm example.c"
Enter
