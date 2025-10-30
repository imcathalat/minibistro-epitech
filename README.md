# Super Little Computer

**Super Little Computer** is a calculator written in pure C that can handle arbitrarily large numbers (limited only by your system memory).
It uses Dijkstra's Shunting-Yard algorithm to convert mathematical expressions from infix notation to postfix notation, also known as Reverse Polish Notation (RPN), and then evaluates them.

#### Infix Notation
```bash
5 * (3 + 4) - 2
```

#### Postfix notation
```bash
5 3 4 + * 2  -
```

To know more about RPN, recommend the following videos:

- [Shunting Yard Algorithm](https://youtu.be/Wz85Hiwi5MY?si=Xrb2mIzXaFi3LnOE)  
- [Post Fix Stack Evaluator](https://youtu.be/bebqXO8H4eA?si=PhxOmwOFzhxuFtot)

### Instalation / Compilation

1. Clone this repository
2. Open your terminal.
3. Navigate to the project folder.
4. Run 
```bash
make
```
This will generate the executable calc.exe (or simply calc).]

### Clean Build Files

```bash
make clean
make fclean
make re
```

### Usage
You can run the calculator and type expressions directly:
```bash
./calc
```
Or pass expressions through standard input:
```bash
echo '(3+6)*2' | ./calc
```
To view a help message:
```bash
./calc -h
```

### Author
Isadora Maria de Araujo Cathalat
imcathalat@gmail.com
