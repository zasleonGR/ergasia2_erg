all: finaleshop

finaleshop: eshop.o
  echo "Compiling and producing eshop......."
  gcc eshop.o -o finaleshop
  chmod +x finaleshop

eshop.o: eshop.c
  echo "Compiling the main file....."
  gcc -c eshop.c

clean:
  echo "Removing....."
  rm eshop.o finaleshop
