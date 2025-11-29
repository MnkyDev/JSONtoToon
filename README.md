![C++ Standard](https://img.shields.io/badge/C%2B%2B-17-blue.svg?style=flat&logo=c%2B%2B)

# JSON -> TOON Converter
This is a simple converter to swap **JSON** to **TOON (Token Object Orientated Notation)**

> [!TIP]
> **What is TOON?**
> You can find more information about it [here](https://github.com/toon-format/toon).

## How to run
```bash
g++ src/main.cpp -o output/converter -std=c++17
```

## Limitations
Currently, the parser requires double quotes (""). Single quotes ('') wont work and will result in errors.
