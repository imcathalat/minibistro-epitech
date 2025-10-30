# Super Little Computer

**Super Little Computer** is a calculator written in pure C that can handle arbitrarily large numbers (limited only by your system memory).
It uses Dijkstra's Shunting-Yard algorithm to convert mathematical expressions from infix notation to postfix notation, also known as Reverse Polish Notation (RPN), and then evaluates them.

#### Infix Notation
```
5 * (3 + 4) - 2
```

#### Postfix notation
```
5 3 4 + * 2  -
```

To know more about RPN, recommend the following videos:

https://youtu.be/Wz85Hiwi5MY?si=Xrb2mIzXaFi3LnOE
https://youtu.be/bebqXO8H4eA?si=PhxOmwOFzhxuFtot

### Instalation / Compilation

1. Open your terminal.
2. Navigate to the project folder.
3. Run 
```
make
```
This will generate the executable calc.exe (or simply calc).]

### Clean Build Files

```terminal
make clean
make fclean
make re
```

### Usage
You can run the calculator and type expressions directly:
```
./calc
```
Or pass expressions through standard input:
```
echo '(3+6)*2' | ./calc
```
To view a help message:
```
./calc -h
```

### Author
Isadora Maria de Araujo Cathalat
imcathalat@gmail.com
