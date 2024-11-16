FinalIncludeFile=all_res.hpp

rm -r c/;

> $FinalIncludeFile

echo "#pragma once" >> $FinalIncludeFile;

echo "" >> $FinalIncludeFile;

echo "enum R_type {" >> $FinalIncludeFile;
echo "    IMG," >> $FinalIncludeFile;
echo "    AUDIO," >> $FinalIncludeFile;
echo "};" >> $FinalIncludeFile;

echo "" >> $FinalIncludeFile;

echo "struct Resource {" >> $FinalIncludeFile;
echo "    int id;" >> $FinalIncludeFile;
echo "    R_type type;" >> $FinalIncludeFile;
echo "    const char* path;" >> $FinalIncludeFile;
echo "    unsigned char* data;" >> $FinalIncludeFile;
echo "    unsigned int size;" >> $FinalIncludeFile;
echo "};" >> $FinalIncludeFile;


echo "" >> $FinalIncludeFile;

if [ ! -d c ]
then
    mkdir c;
fi

for d in $(find img  -maxdepth 1 -type d -exec basename {} \;)
do
    if [ "img" = "$d" ]
    then
        continue;
    fi
    mkdir -p "c/img/$d";
done

for e in $(find img -type f)
do
    trimmed="${e%.png}";
    filepath="c/${trimmed}.h";
    incline="#include \"../res/${filepath}\"";
    
    echo "[+] Packing $e --> $filepath";
    xxd -i "${e}" > "$filepath";
    echo $incline >> $FinalIncludeFile;
done

echo "" >> $FinalIncludeFile;

echo "inline static Resource resources[] {" >> $FinalIncludeFile;

id=0;
for e in $(find img -type f)
do
    trimmed="${e%.png}";
    sanitized="${trimmed//\//_}";
    echo "  {$id, IMG, \"$e\", ${sanitized}_png, ${sanitized}_png_len}," >> $FinalIncludeFile;
    let id++;
done
echo "};" >> $FinalIncludeFile;
echo "constexpr unsigned int resources_count = sizeof(resources)/ sizeof(resources[0]);" >> $FinalIncludeFile;