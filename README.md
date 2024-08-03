# Pargen
Very simple parser generator.

## Why?

I built this because I don't like PEG parsers and LALR parsers are not flexible enough. I don't like PEG because it attempts to combine the scanner and the parser into a single input file, which clouds the actual grammar that is being developed. LALR parsers cannot parse syntax that is irregular, but there are many perfectly reasonable language constructs that are irregular. 

## How?

This parser generator creates a simple recursive decent parser with a separate scanner. The scanner is normally implemented using GNU Flex. The input language is very simple that supports a simplified regular expression syntax to allow for looping constructs to create lists. There is no particular attention payed to any particular scholarly work.

The input language for the parser generator is regular and so the parser is implemented in GNU Bison. The scanner is implemented using GNU Flex. There is quite a bit of auxiliary code to handle things like strings and lists. No complicated symbol tables or code analysis is used. Simplicity is paramount. 

### Design priorities

* Simplicity. This should be easy to understand and the output of the parser generator should be readable by a unsophisticated developer. Very little checking is done to try and make sure that the generated parser "makes sense".
* Stability. Correct input is always accepted and incorrect input is rejected with a useful error message. There is no possibility of producing an invalid output.
* The output of the generated parser is an Abstract Syntax Tree (AST). There is no prevision to execute code when a rule is recognized. It is intended that the output of a generated compiler will be created by writing "passes" on the AST. The parser generator follows this strategy and can be used as a template for the generated parser.
* The code for both the parser and the AST are generated and can simply be added to the translator for the intended language. 
* Passes on the AST are implemented using callbacks that are called before and after the AST node is traversed.

### Input language

The input language is focused on making it simple to create a grammar that represents the intended target language. Since simplicity is a primary goal of the system, the syntax that is accepted is also simple and not very flexible.  The input grammar is a list of rules. A rule is a list of productions. A production is a list of terminal and non-terminal symbols. Comments are also supported and can appear in the grammar on an empty line or at the end of a line. There is no prevision for defining code to be executed when a production is matched. It's intended that such code would be executed when the AST is traversed.

#### Comments

A comment can appear anywhere in the grammar and starts with a ``#`` character and ends at the end of the line.

##### expression

```"#".*\n```

#### Rules

A rule starts with a non-terminal symbol that must have a lower case character between "a" and "z". After the first character, any alpha-numeric character can be used. After the non-terminal symbol, a ``:`` follows. After that a list of productions. Each production is separated by a ``|`` character, and the end of the rule is marked by a ``;`` character. See below for the complete grammar.

##### non-terminal symbol expression

```[a-z_][a-zA-Z_0-9]*```

##### terminal symbol expression

Terminal symbols can be defined as quoted strings or as a symbol that starts with a capital letter. 

```
[A-Z][a-zA-Z_0-9]*
\'([^\'\n]*)\'
\"([^\"\n])*\"
```

#### Grammar for the input language

```
# This is a raw grammar for the parser generator grammar grammar.

grammar
    : ( rule )+

rule
    : ':' production ( '|' production )* ';'

production
    : ( prod_elem )+

prod_elem
    : TERMINAL
    | NON_TERMINAL
    | zero_or_more
    | one_or_more
    | one_or_zero
    | group

zero_or_more
    : group '*'

one_or_more
    : group '+'

one_or_zero
    : group '?'

group
    : '(' production ')'

```

In the language, the ``*`` means "zero or more", ``+`` means "one or more", and ``?`` means "zero or one". Any combination of TERMINALs and NON_TERMINALs can make up a production.
