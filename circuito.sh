function ccmake
{
  cd build
  # env CC=/usr/bin/clang CXX=/usr/bin/clang++ cmake -G Ninja ..
  cmake -G Ninja ..
  cd ..
}

function cccmake
{
  rm build/* -r
}

function cbuild
{

  printf "####### compiling ########\n\n"
  ninja -C build
}

function crun
{

  exe=`find build -name $1.exe`

  if [ ! $exe ]; then
    echo "$1 not found"
    return
  fi

  if [ $2 ]; then
    eval $exe $2
  else
    eval $exe
  fi
}

function ccscope
{

  cscope -b -R
}

function crtests
{
  build/tests/catch/test_catch
  build/tests/others/test_0 $1
}

function cdocs
{
  doxygen
}

function crcom
{
  python remove_comments.py $1
}

echo "functions loadedddddddddddddddddd"
