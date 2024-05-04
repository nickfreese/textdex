# textdex-lib

A zero dependency key-value search tool for your text based projects

textdex will provides a flexible way to search projects that may have gotten a little loosy goosey with their formatting. Your key value searches can have flexible delimiters with unlimited spaces on either side.  searches are also case insensitive.  meaning all the following formats will be found  with this search `./textdex keyval=name:john`

1. `name: john`
2. `name John`
3. `name | john`
4. `name : joHn`

This software is geared toward writers and worldbuilders who have the occassional need to sort their projects, or who are retroactively trying to apply standardized formatting to their project.   With textdex, you can write first and format later.


## compile and run


(g++ -std=c++17 -o textdex textdex-lib.cpp;./textdex)


### MacOS install

```
g++ -std=c++17 -o textdex textdex-lib.cpp
sudo ln textdex /usr/local/bin/textdex
```


## example searches
textdex keyval=name:dad;
textdex keyval=species:orc;
textdex keyval=species:orc d=-;



## Contributing
 textdex is opensource and can always be made better.  Here are a few of the things this repos needs help with:
   - Documentation
   - Casting a wider net for search patterns.
   - Pattern cleanup:  while textdex is small, it still requires some reformatting to make it easy to work with and as flexible as possible


## dev commands

compile and run test searches
```
(g++ -std=c++17 -o textdex textdex-lib.cpp;./textdex folder=./ s=apple keyval=apple:dad d=asdf;./textdex folder=./ s=apple; ./textdex keyval=apple:dad d=asdf; )
```


### code formatting
clang-format -i textdex-lib.cpp --style="{BasedOnStyle: mozilla, IndentWidth: 4}"
