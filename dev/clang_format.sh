#!/bin/bash
find . \( -iname \*.cpp -o -iname \*.h \) -exec clang-format -i -style=file {} \;




