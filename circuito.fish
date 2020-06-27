function crun

  if [ $argv ]
    set exe (find build -name $argv[1])

    if [ ! $exe ]
      echo "$argv[1] not found"
      return
    end

    if [ (count $argv) -gt 1 ]
      set arg $argv[2]
    end

    echo "$exe $arg" > run.cir
  end

  if [ ! -e run.cir ]

    echo "Missing Argument"

    return

  end

  eval (cat run.cir)

end

function ccmake
  cd build
  env CC=/usr/bin/clang CXX=/usr/bin/clang++ cmake -G Ninja ..
  # cmake -G Ninja ..
  cd ..
end

function cccmake
  rm build/* -r
end

function ccscope
  cscope -b -R
end

function cbuild
  ninja -C build
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
