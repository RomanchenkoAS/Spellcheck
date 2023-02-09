# Spellcheck
- A spellcheker program with no GUI implemented in C for the fastest operating speed
- Usage: Usage: ./speller [DICTIONARY] text
- There are tools for measuring operating speed, but avaliable only on linux, since they require <sys/resource.h>. They are commented-out by default in speller.c
- Example output for Tolstoy's "War and peace"

![image](https://user-images.githubusercontent.com/119735427/217814669-ccc4db12-78a0-45a6-ad5a-94f4759bd63c.png)

*Misspell count is high mostly because of the names*

Here **"time in load"** - how long it takes to initialize a dictionary (143k words), **check** - actual spell-checking the book, **size** - calculating number of words in dictionary, **unload** - freeing memory allocated for the dictionary.

- Dictionary file is optional
- Texts for testing are located in /texts directory
- For testing you may call "./speller dictionaries/large texts/holmes.txt"
