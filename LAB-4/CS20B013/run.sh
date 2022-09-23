read directory

rm -f ./main
mkdir "output_$directory"
outputDirectory="output_$directory"

g++ main.cpp -o main -Wall -g

for file in $directory/*.txt
do
    echo "Running $(basename $file .txt).txt"
    ./main < $file > "./$outputDirectory/output_$(basename $file .txt).txt"
done

rm -f ./main
echo "Done"