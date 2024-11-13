# > all_res.hpp
# echo "#pragma once" >> all_res.hpp;

FinalIncludeFile=all_res.hpp

if [ ! -d c ]
then
    mkdir c
fi

for e in $(ls img/)
do
    filepath="c/${e::-4}.h";
    incline="#include \"../res/${filepath}\"";

    if [ ! -e $filepath ]
    then
        echo "[+] Packing img/$e --> $filepath";
        xxd -i "img/${e}" > "$filepath";
        if grep -Fxq "$incline" $FinalIncludeFile; then
            echo "[!] $incline alredy in $FinalIncludeFile";
        else
            echo $incline >> all_res.hpp
        fi
    fi
done