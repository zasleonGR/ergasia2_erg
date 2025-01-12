all: eshop

eshop: version2.o
  echo "Compiling and producing eshop......."
  gcc version.o -o eshop
  chmod +x eshop

version2.o: version2.c
  echo "Compiling the main file....."
  gcc -c version2.c

clean:
  echo "Removing....."
  rm version2.o eshop
