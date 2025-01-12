all: eshop

eshop: Version2.o
  echo "Compiling and producing eshop......."
  gcc Version2.o -o eshop
  chmod +x eshop

Version2.o: Version2.c
  echo "Compiling the main file....."
  gcc -c Version2.c

clean:
  echo "Removing....."
  rm Version2.o eshop
