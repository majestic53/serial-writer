Superboard III Serial Writer
============================

A tool to slow serial traffic between the host machine and Superboard III, allowing for clean data transfers

Table of Contents
=================

1. [Build](https://github.com/majestic53/serial-writer##build)
2. [Usage](https://github.com/majestic53/serial-writer##usage)
3. [Changelog](https://github.com/majestic53/serial-writer##changelog)
4. [License](https://github.com/majestic53/serial-writer##license)

Build
=====

__NOTE__: Tested with the GNU and Clang c compilers. Modification to the makefiles might be required to build with a different compiler.

To build the tool run the following command from the root directory:

```
make
```

Usage
=====

###Serial Writer

Slows serial traffic between the host machine and Superboard III, allowing for clean data transfers

####Usage
```
serial-writer [PATH]
```

####Arguments
```
PATH                Path to basic (.bas) file
```

Changelog
=========

###Version 0.1.1702
*Updated: 1/10/2017*

* Updated documentation
* Added makefile

*Changelog truncated (see file history for full log)*

License
=======

Copyright(C) 2015-2017 David Jolly <majestic53@gmail.com>

cc65c is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

cc65c is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
