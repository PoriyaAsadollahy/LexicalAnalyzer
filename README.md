# Lexical Analyzer

This project is a lexical analyzer written in C++. It processes input source code to generate a list of tokens, which are the basic elements of the source code's syntax.

## Features

- Identifies keywords, identifiers, integers, doubles, operators, strings, delimiters, semicolons, and comments.
- Detects errors in token formation, such as invalid operators or malformed numbers.
- Supports a set of predefined keywords and operators.

## Usage

### Compilation

To compile the project, use the following command with MinGW:

```bash
g++ -o lexical_analyzer lexical_analyzer.cpp
```

### Running the Analyzer

1- **Input Preparation:** Prepare a text file named `command.txt` with the source code you want to analyze.

2- **Execution:** Run the executable:

```bash
./lexical_analyzer
```

### Example

Given the input file `command.txt`:

```cpp
double len; \* length of rectangle*\
double wid; \* width of rectangle*\
len = get(double);
wid = get(double);

if(len == wid) 
{
    print("The shape is a square! ");
    print("perimeter= ");
    print(4*len);
    print(" area= ");
    print(len*len);
}
else
{
    print("The shape is a rectangle!" );
    print("perimeter= ");
    perimeter=2*(len+ wid);
    print(perimeter);
    print(" area= ");
    print(len*wid);
}
```

The output file `output.txt` will contain:

```vbnet
<KEYWORD, "double">
<IDENTIFIER, "len">
<SEMICOLON, ";">
<KEYWORD, "double">
<IDENTIFIER, "wid">
<SEMICOLON, ";">
<IDENTIFIER, "len">
<OPERATOR, "=">
<KEYWORD, "get">
<DELIMITER, "(">
<KEYWORD, "double">
<DELIMITER, ")">
<SEMICOLON, ";">
<IDENTIFIER, "wid">
<OPERATOR, "=">
<KEYWORD, "get">
<DELIMITER, "(">
<KEYWORD, "double">
<DELIMITER, ")">
<SEMICOLON, ";">
<KEYWORD, "if">
<DELIMITER, "(">
<IDENTIFIER, "len">
<OPERATOR, "==">
<IDENTIFIER, "wid">
<DELIMITER, ")">
<DELIMITER, "{">
<KEYWORD, "print">
<DELIMITER, "(">
<STRING, ""The shape is a square! "">
<DELIMITER, ")">
<SEMICOLON, ";">
<KEYWORD, "print">
<DELIMITER, "(">
<STRING, ""perimeter= "">
<DELIMITER, ")">
<SEMICOLON, ";">
<KEYWORD, "print">
<DELIMITER, "(">
<INT, "4">
<OPERATOR, "*">
<IDENTIFIER, "len">
<DELIMITER, ")">
<SEMICOLON, ";">
<KEYWORD, "print">
<DELIMITER, "(">
<STRING, "" area= "">
<DELIMITER, ")">
<SEMICOLON, ";">
<KEYWORD, "print">
<DELIMITER, "(">
<IDENTIFIER, "len">
<OPERATOR, "*">
<IDENTIFIER, "len">
<DELIMITER, ")">
<SEMICOLON, ";">
<DELIMITER, "}">
<KEYWORD, "else">
<DELIMITER, "{">
<KEYWORD, "print">
<DELIMITER, "(">
<STRING, ""The shape is a rectangle!"">
<DELIMITER, ")">
<SEMICOLON, ";">
<KEYWORD, "print">
<DELIMITER, "(">
<STRING, ""perimeter= "">
<DELIMITER, ")">
<SEMICOLON, ";">
<IDENTIFIER, "perimeter">
<OPERATOR, "=">
<INT, "2">
<OPERATOR, "*">
<DELIMITER, "(">
<IDENTIFIER, "len">
<OPERATOR, "+">
<IDENTIFIER, "wid">
<DELIMITER, ")">
<SEMICOLON, ";">
<KEYWORD, "print">
<DELIMITER, "(">
<IDENTIFIER, "perimeter">
<DELIMITER, ")">
<SEMICOLON, ";">
<KEYWORD, "print">
<DELIMITER, "(">
<STRING, "" area= "">
<DELIMITER, ")">
<SEMICOLON, ";">
<KEYWORD, "print">
<DELIMITER, "(">
<IDENTIFIER, "len">
<OPERATOR, "*">
<IDENTIFIER, "wid">
<DELIMITER, ")">
<SEMICOLON, ";">
<DELIMITER, "}">
```



### Design

We define the token classes for the target language:
- identifier
- int
- double
- operator
- keyword
- string
- delimiter
- comment
- semicolon
- error

These tokens are defined as an `enum class` at the beginning of the program.

Now, for each class, we write a regular expression:

**Alphabet (Σ)** = {a, b, c, …, z, A, B, …, Z, 0, 1, 2, …, 9, (, ), {, }, ;, ", ., +, -, *, /, \\, =, >, <, !}

- **Letter** = ‘a’ + ‘b’ + ‘c’ + … + ‘z’ + ‘A’ + … + ‘Z’
- **Operator** = ‘+’ + ‘-’ + ‘/’ + ‘*’ + ‘=’ + ‘<’ + ‘>’ + ‘!’
- **NumbersNoZero** = ‘1’ + … + ‘9’
- **Numbers** = ‘0’ + NumbersNoZero
- **Delimiter** = ‘(’ + ‘)’ + ‘{’ + ‘}’
- **Space** = ‘ ’ + ‘\n’ + ‘\t’
- **Keywords** = ‘if’ + ‘else’ + ‘print’ + ‘get’ + ‘int’ + ‘double’ + ‘for’
- **Operators** = ‘++’ + ‘+’ + ‘*’ + ‘-’ + ‘/’ + ‘=’ + ‘<’ + ‘<=’ + ‘>’ + ‘>=’ + ‘==’ + ‘!=’
- **Identifier** = Letter . (Letter + Number)*
- **Int** = NumberNoZero . Number*
- **Double** = Int . ‘.’ . Int
- **Comment** = ‘\’ . ‘\*’ . Σ\*-{“\*\”}. ‘\*’ . ‘\’
- **Delta** = Σ – {‘”’}
- **String** = ‘”’ + Delta* + ‘”’
- **Semicolon** = ‘;’

Now we design a diagram for the regular expressions:

<img src="https://github.com/PoriyaAsadollahy/LexicalAnalyzer/blob/main/Diagram.jpg" width="800" alt="Main DFA">

### Implementation

The implementation is done using C++ and object-oriented programming.

**Token Class:**
- Defines a token with a `type` and a `value`.
- Provides methods for printing and comparing tokens.

**Lexer Class:**
- Responsible for converting a string into tokens.
- Defines keywords and operators within the class.
- Contains a method for performing the tokenization of the input string.

**Helper Functions:**
- **IsOperator**: Determines if a character is an operator.
- **IsDelimiter**: Determines if a character is a delimiter.

**Main Function:**
1. Reads the program code from the `command.txt` file and stores it in a string (essentially in an `ostringstream`).
2. Creates a `Lexer` object with the input string and calls the `tokenize` method on it.
3. If errors are present, prints the word "error" to the terminal.
4. If there are no errors, writes the program output to the `output.txt` file.
