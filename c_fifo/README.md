# FIFO Simple
An attempt to put together a very simple fifo example in C.

# Compiling and running
```sh
reset; gcc fifo_simple.c -o fifo_simple && ./fifo_simple
```

## GDB reminder
It is possible you may want to use a debugger with this.

For GDB to find its way in the code, you need to compile with debug information. Add "-g" to the previous command:

```sh
reset; gcc fifo_simple.c -g -o fifo_simple
```

Now we are ready to roll!

```sh
gdb fifo_simple
```

A nice cheat-sheet can be found in [Tutorials Point](https://www.tutorialspoint.com/gnu_debugger/gdb_quick_guide.htm)

After a while you may realize that you would like your breakpoints or variables to be set when you initialize GDB. Of course you can do this from a script! Launch GDB with the command parameter:

```sh
gdb --command=gdb_script fifo_simple
```

see our example in gdb_script.