SUCCESS_COUNT=0
FAIL_COUNT=0
DIFF_RES=""
echo "" >> log.txt

declare -a tests=(
"Hell test1.txt VAR"
"VAR Hell test1.txt"
"Hell VAR test1.txt test2.txt"
"VAR -e sed -e Hell test1.txt test2.txt"
"VAR -f reg.txt test1.txt"
"-f reg.txt VAR test1.txt test2.txt"
)

declare -a plus_tests=(
"-lc Hell test1.txt test2.txt"
"-e 'sd\|e' test1.txt"
"test1.txt -e Hell -f reg.txt"
"test1.txt -e Hell -iv"
"test1.txt -e Hell -in"
)

testing()
{
    t=$(echo $@ | sed "s/VAR/$var/")
    ./s21_grep $t > test_s21_grep.txt
    grep $t > test_grep.txt

    if [ -z "$(diff -q test_s21_grep.txt test_grep.txt)" ]
    then
        (( SUCCESS_COUNT++ ))
        #echo "$t" >> log.txt
    else
        (( FAIL_COUNT++ ))
        #echo "$t" >> log.txt
    fi
    rm test_s21_grep.txt test_grep.txt
}



for var1 in i v c l n h o
do
    for i in "${tests[@]}"
    do
        var="-$var1"
        testing $i
    done
done


for var1 in i v c l n h o
do
    for var2 in i v c l n h o
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1 -$var2"
                testing $i
            done
        fi
    done
done

for var1 in i v c l n h o
do
    for var2 in i v c l n h o
    do
        for var3 in i v c l n h o
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then
                for i in "${tests[@]}"
                do
                    var="-$var1 -$var2 -$var3"
                    testing $i
                done
            fi
        done
    done
done

for i in "${plus_tests[@]}"
do
    var="-"
    testing $i
done

echo "FAIL: $FAIL_COUNT"