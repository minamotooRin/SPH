cd src

echo "Now start compile."
g++ *cpp -o main -std=c++17
if [ $? != 0 ]
then
    echo "Compile Error!"
    exit 1
fi
echo "Compile done."

mv main ../

cd ..
echo "Now start running..."
./main
echo "Execute done."