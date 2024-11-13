> all_res.hpp

echo "#pragma once" >> all_res.hpp;

if [ ! -d c ]
then
    mkdir c
fi

for e in $(ls img/)
do
    filepath="c/${e::-4}.h";
    xxd -i "img/${e}" > "$filepath";
    echo "#include \"../res/${filepath}\"" >> all_res.hpp
done