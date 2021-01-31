The `Instance::execute(std::string input)` method takes a string input. Input may be:

## ABNF Variable Declaration

See `ABNF_syntax.md` for more

## Command

Commands are prefixed by `!`

### `!list`

Lists all variables loaded in parent `Instance`

### `!details <var>`

Lists lexed tokens inside `<var`

### `!load <file>`

Loads `<file>`.

File is interpreted line-by-line, with each line being executed on `Interpret::execute()` itself.

File is also pre-processes.

### `!test <var>, <input>`

Tests `<var>` against said `<input>` (*N.B . leading whitespace in `<input>` is consumed*)


### `!testfile <var>, <file>`

Tests `<var>` against contents of `<file>`.
