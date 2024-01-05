nocrt_link="kernel32.lib -nodefaultlib -incremental:no -opt:ref -manifest:no"
nocrt_cl="-Zi -GF -GS- -GR- -EHsca- -nologo -fp:fast -fp:except- -Oi -Ob3"

link="-c"
if [[ "$@" == *"l"* ]];
then
    # link="-link user32.lib Gdi32.lib -SUBSYSTEM:WINDOWS $nocrt_link"
    link="-link -SUBSYSTEM:CONSOLE $nocrt_link"
fi

# output="-Fox64/Debug/$(basename $PWD) -Fex64/Debug/$(basename $PWD)_cl"

filename="run.c"

cl $nocrt_cl -Wall $output $filename $link

if [[ "$@" == *"r"* ]];
then
    # x64/Debug/$(basename $PWD)_cl.exe
    ./run.exe lr
fi