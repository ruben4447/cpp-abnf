# ABNF

ABNF stands for "Augmented Backus-Naur Form" and is an extended version of BNF.

## Console Output

To get coloured outputs (used in debug)...

Powershell: `Set-ItemProperty HKCU:\Console VirtualTerminalLevel -Type DWORD 1`

Cmd: `reg add HKCU\Console /v VirtualTerminalLevel /t REG_DWORD /d 1`

and restart the respective console

This allows ANSI escape sequences to be evaluated

About: [Coloured Text Output In Powershell](https://stackoverflow.com/questions/51680709/colored-text-output-in-powershell-console-using-ansi-vt100-codes)

## Interpret files

Command Line: `./main.exe [file].abnf`

- `file` : file containing ABNF rules

## Syntax

Comments start with `;` and continue until end of the line

Directives: instructions to pre-processor
- `#include <file>` : include a file into the script. Looks in same directory as entry file. If file is surrounded in `<>`, the name will be considered a library and looked for in the appropriate directory
  - e.g. `#include test.abnf` : Dumps the file `./test.abnf` into the source file
  - e.g. `#include <core>` : DUmps the library file `core.abnf` into the source file

Declaration: `rule op definition [; comment] CR LF`

### op

- Assignment operator `=`: assigned `definition` to `rule`
- Concatenation operator `=/`: adds `definition` as an alternate rule to `rule` (i.e. `/` + `definition`)
- [*Non-spec*] Constant assignment `:=`: acts as a `=`, but declares `rule` as a constant; re-declaration or modification is not allowed.

### Rules

- May be surrounded by `<>`
- Are case-insentitive: `<Hello>`, `hello` and `HeLlO` are considered the same
- Rules may appear in definitions (even if it has not been defined yet)

### Literal Values

#### Strings

- Surrounded by `""`.
- By default, strings are case-insensitive; the string `hello` will match `HELLO` and `Hello`.
- May be preceded by a flag: `%i` for case-insensitivity (default) or `%s` for case-sensitivity (NB flag must be directly before a string e.g. `%s"Hi"`)

#### Numerical

Format: `%<base><value>`

- `base` : base of number (`d` = decimal, `b` = binary, `x` = hexadecimal)
- `value` : numerical characters matching base

Operations:

- Concatenate
  - Format: `%<base><value>.<value>` etc...
  - Quick definition of multiple numerical values of same base in sequence
- Range
  - Format: `%<base><lower>-<upper>`
  - Specifies any char in the range `lower <= char < upper` [Exclusive]
  - NB range cannot be negative (i.e. `upper - lower > 0`)

### Operators

- Alternative
  - Syntax: `<expr> / <expr>`
  - Basically an OR operation
- Sequence Group
  - Syntax: `(<expr>)`
  - Groups rules in a definition
- Optional Group
  - Syntax: `[<expr>]`
  - `<expr>` is optional
    - Same as `*1(<expr>)`
- Repetition
  - Syntax: `<a>*<b><expr>`
    - ... Where `<expr`> is repeated a to b (exclusive) times
    - `a` and/or `b` may be excluded. `a` default to `0`, and `b` to `Infinity`
  - Syntax: `<a><expr>`
    - `<expr>` must appear exactly `<a>` times
