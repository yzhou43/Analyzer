# Specification Document
### 1. Lexical elements  
#### a) Comments
There are two forms of comments in the programming language:  
i.  Line comments start with the character sequence '//' and stop at the end of the line.  
ii. General comments start with the character sequence '/\*' and stop with the first subsequent character sequence '\*/'.

#### b) Keyword Lists
Keyword_list = {"auto","break","case","char","const", "continue","default","do","double","else",
"enum","extern","float","for","goto", "if","int","long","register","return",
"short","signed","sizeof","static","struct", "switch","typedef","union","unsigned","void", "volatile","while","begin","end"};

#### c) Operators and punctuation

Operator_list = {‘+’  ‘++’  ‘-’  ‘--’  ‘\*’  ‘/’  ‘//’  ‘/\*’  ‘\*/’  ‘:’  ‘:=’  ‘<’  ‘<=’  ‘>’  ‘>=’ ‘=’  ‘==’  ‘;’  ‘,’   ‘(’   ‘)’  ‘{’  ‘}’  ‘[’  ‘]’  ‘&’  ‘&&’  ‘\’  ‘\\’  ‘|’  ‘||’  ‘!=’  ‘#’  ‘%’  ‘.’  }
	
#### d)	Notations
Productions are expressions constructed from terms and the following operators, in increasing precedence:  
|   alternation  
()  grouping  
[]  option (0 or 1 times)  
{}  repetition (0 to n times)  

#### e)	Literals
In regular expression,  
Int_literal = [0-9]+  
float_ literal = \[0-9\]( _?[0-9])\*. ((e-|e+|E-|E+)? [0-9] ( _?[0-9])\*)\*  
boolean_literal = true | false  

In BNF,  
string_ literal &rarr; \" { unicode_value | byte_value } \"   
byte_value &rarr; int_literals {byte_value } | float_ literals{ byte_value }  
unicode_value &rarr; { letters | operator_list }  
letters &rarr; [a-zA-Z] | [a-zA-Z]{ letters }  


#### f)	Identifiers
Identifier = [a-zA-Z]+[ a-zA-Z0-9]\*  
The indentifiers can only consist of letters and digits in my programming language. The reason for this design is to help the lexical analyzer recognize operators. 
For example, a0bc\*b4a means the variable a0bc times the variable b4a rather than the whole string being recognized as an identifier "a0bc\*b4a". 
If the identifiers are allowed to contain the operator, then it will be hard for the analyzer to differentiate a single variable and an operation containing multiple variables.


### 2. Types
Type = int | float | string | boolean  
int &larr; int_literal. type  
float &larr; float_ literal. type  
boolean &larr; boolean_literal. type  
string &larr; string_ literal. type  
Both the int and float types only contain decimal digits. The drawback for this desgin is that the programming language cannot accept octal or hexadecimal digits, 
but it simplifies the task for the lexical analyzer. If integer literals or float literals are allowed to contain letters like “A” “B”, 
then it’s difficult to design a lexical analyzer to distinguish literals and identifiers.


### 3. Grammars

##### 1)	\<PROGRAM> :: = "begin" <STMT_LIST> "end"  
##### 2)	<STMT_LIST> :: = < STMT >{; < STMT >}  
##### 3)	< STMT > :: = < EXPR > {(">"| "<" |"="|"!"|"<="|">="|"==") < EXPR > }  
##### 4)	< EXPR > :: = < TERM >  {("+"|"-") < TERM >}  
##### 5)	< TERM > :: = < FACTOR > {("\*"|"/"|"%") < FACTOR >}  
##### 6)	< FACTOR > :: = Identifier | Int_literal | Float_ literal | Boolean_literal | String_ literal "(" < STMT > ")"
&nbsp;
##### 3)	< STMT > :: = < EXPR > {(">"| "<" |"="|"!"|"<="|">="|"==") < EXPR > }
Syntax rule:  
< STMT > :: = < EXPR >[1] {(">"| "<" |"="|"!"|"<="|">="|"==") < EXPR > [2]}  
Semantic rule:   
< EXPR >[1]. expected_type ← < EXPR > [2].actual_type  
Predicate:   
< EXPR >[1]. actual_type == < EXPR > [2]. actual_type  

##### 4)	< EXPR > :: = < TERM >  {("+"|"-") < TERM >}  
Syntax rule:   
< EXPR > :: = < TERM >[1]  {("+"|"-") < TERM >[2]}  
Semantic rule:   
	< EXPR >.actual_type ←   
if (< TERM >[1]. actual_type == int) and
(< TERM >[2].actual_type == int)  
			then int  
			else if (< TERM >[1]. actual_type == boolean) and
(< TERM >[2].actual_type == boolean)  
			then boolean  
			else if [(< TERM >[1]. actual_type == int) or
(< TERM >[1].actual_type == float)]  
			and [(< TERM >[2]. actual_type == int) or 
(< TERM >[2].actual_type == float)]  
			then float  
			else string  
			end if  
Predicate:  
	< EXPR >.actual_type == < EXPR >.expected_type  

##### 5)	< TERM > :: = < FACTOR > {("\*"|"/"|"%") < FACTOR >}  
Syntax rule:   
< TERM > :: = < FACTOR >[1] {("*"|"/"|"%") < FACTOR >[2]}  
Semantic rule:   
	< TERM >.actual_type ←   
if (< FACTOR >[1]. actual_type == int) and
(< FACTOR >[2].actual_type == int)  
			then int  
			else float  
			end if     
Predicate:   
	< TERM >[1].actual_type == < TERM >[1].expected_type  
	If (< FACTOR >[2])  
  then (< FACTOR >[2] . actual_type == int) or
 							(< FACTOR >[2] . actual_type == float) 
              
##### 6)	< FACTOR > :: = Identifier | Int_literal | Float_ literal | Boolean_literal | String_ literal "(" < STMT > ")"
Syntax rule:  
< FACTOR > :: = Identifier | Int_literal | Float_ literal | Boolean_literal 
| String_ literal "(" < STMT > ")"

Semantic rule:  
		< FACTOR >. actual_type ← look-up (<FACTOR >. string)

In the programming language, it is required to begin with the keyword “begin” and end with the keyword “end”. The benefit is that when the user inputs a code block, which contains multiple lines, the analyzer can know where the code starts and ends. In Rule 5, if < FACTOR >[2] exists, then the type for both < FACTOR >[1] and < FACTOR >[2] have to be int oe float because only numbers can conduct the operations in Rule 5. The drawback for this design is the lack of operations for string type. Although string type is allowed in the programming language, it cannot conduct complicated operation like comparison or truncation in many other languages.


### 4. Statements
#### a)	If Statement
IfStmt :: = "if" < STMT > 
"{" <STMT_LIST> "}"
[ "else" ( IfStmt | "{" <STMT_LIST> "}") ] .

Predicate: < STMT >. actual_type == boolean

#### b)	For Statement
ForStmt = "for" [ < STMT >[1] | < STMT >[2] | < EXPR > ] 
	"{" <STMT_LIST> "}"

Predicate: < STMT >[2]. actual_type == boolean

#### c)	While Statement
WhileStmt = "while" < STMT > 
"{" <STMT_LIST> "}"

Predicate: < STMT >. actual_type == boolean


### 5. Valid Test Code
A simple test code is shown below, which is valid for the programming language. The test file is contained in the submitted files.

begin  
<ul>  area=3.14\*a\*a;  </ul> 
<ul>  s= 2\*3.1416\*r\*(h+r);  </ul>   
end#  
