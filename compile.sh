if [ ! -d "build" ]; then
  mkdir build
fi
cd build

echo "Now start compile."
cmake ..
if [ $? != 0 ]
then
    echo "Compile Error!"
    exit 1
fi
make
if [ $? != 0 ]
then
    echo "Compile Error!"
    exit 1
fi
mv main ../
echo "Compile done."