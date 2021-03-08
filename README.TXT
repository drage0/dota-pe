DotA parameter editor
--------------------------------

what it can do:
list and change parameters of all heroes in the game (excluding bot and item parameters)
what it still cant do:
change spell parameters
change summon parameters

prerequisite (microsoft windows):
visual studio and clang prebuilt binaries for windows (https://releases.llvm.org/download.html or direct link https://github.com/llvm/llvm-project/releases/download/llvmorg-11.0.0/LLVM-11.0.0-win64.exe)

compilation (microsoft windows):
open command prompt and navigate to `environment` folder. (this can be done by starting `command_prompt` program in the same folder.)
to compile the program, run command `.\compile_win32.bat` or `clang++ ..\src\main.cpp -o dotape.exe`

usage:
hero ids can be found in environment/hero_ids.txt
$	dotape.exe -h HERO_ID [-p ParameterName:Value] [-p Parameter:Value or select new hero -h HERO_ID] [-l list selected hero's parameter up to this point] [...]

example usage (note the order of parameters is important!):
set Doom's armour to 8 and magic resistance to 50% and write changes to game folder
$	dotape.exe -h 69 -p ArmorPhysical:8 -p MagicalResistance:50 -o "C:\Program Files (x86)\Steam\steamapps\common\dota 2 beta\game\dota\scripts\npc\npc_heroes.txt"
set Phoenix's base health regen to 16 and model scale to 1.2 and set Centaur's movement speed to 360 with strength gain of 9.6
$	dotape.exe -h 110 -p StatusHealthRegen:16 -p ModelScale:1.2 -h 96 -p MovementSpeed:360 -p AttributeStrengthGain:9.6
list all Wyvern's attributes
$	dotape.exe -h 112 -l
set Nyx's armour to 64 and list all atribute's (this will show the changed armour value as well) and write to game folder
$	dotape.exe -h 88 -p ArmorPhysical:64 -l -o "C:\Program Files (x86)\Steam\steamapps\common\dota 2 beta\game\dota\scripts\npc\npc_heroes.txt"

revert changes:
to revert changes either copy the correct version of .txt files from `environment/dota/$PATCH/` to `steamapps/common/dota 2 beta/game/dota/scripts/npc/` folder or use steam and check validity of game files.
the changes made do not have any effect on multiplayer games, it only affects local games (solo vs. bots for example).
