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

- `file` :
