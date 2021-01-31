`Interpret` class

This is the holder for ABNF variables and gives the ability to evaluate them

- `load_file`
  - Load a file to parse
- `execute`
  - Executes a line of input. may be a definition or command
- `test`
  - Tests input against a variable

## Other

- `define_var`
  - Defines an ABNF variable
  - **NB does not lex it**
- `Variable::lex`
  - Lexes the variable
