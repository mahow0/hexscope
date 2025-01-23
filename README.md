# Hexscope
A compact hex dump CLI utility that I wrote to get my feet wet with C++. Similar to the ``xxd`` command-line program.

Given an input file, ``hexsc`` produces a view of the file's hexadecimal representation. The hex dump is organized into rows of whitespace-separated groups of bytes. 
In addition, the hexadecimal address is printed at the beginning of each row. 

## Setup
Run ``make`` from the project directory to build and compile the project. The ``output`` folder will hold the resulting ``hexsc`` executable.

## Usage
``hexsc [options] infile [outfile]``

Options include:
  * -c: number of cols to display per line
  * -g: size of each group (in bytes)
  * -p: flag to print output in postscript plain dump style (no columns, groups, or addresses)


