all: eshop

eshop: eshop.o
  echo "Compiling and producing eshop......."
  gcc eshop.o -o eshop
  chmod +x eshop

eshop.o: eshop.c
  echo "Compiling the main file....."
  gcc -c eshop.c

clean:
  echo "Removing....."
  rm eshop.o eshop
