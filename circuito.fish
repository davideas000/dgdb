function ccmake
    cd build
    env CC=/usr/bin/clang CXX=/usr/bin/clang++ cmake -G Ninja ..
    cd ..
end

function cccmake
    rm build/* -r
end

function cbuild 
    printf "####### compiling ########\n\n"
    ninja -C build
end

function crun
    set exe (find build -name $argv[1]) 

    if [ ! $exe ]
        echo "$argv[1] not found"
        return
end

if [ (count $argv) -gt 1 ] 
    eval $exe $argv[2]
else
    eval $exe
end
end

function ccscope
    cscope -b -R
end

function crtests
    build/tests/catch/test_catch
    build/tests/others/test_0 $argv
end

function cdocs
end

function crcom
    python3 remove_comments.py $argv 
end

function crcomall
    for i in (find . -name \*.cpp)
        python3 remove_comments.py $i
    end
end

echo "functions loaded"
