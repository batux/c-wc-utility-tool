# c-wc-utility-tool
The wc utility displays the number of lines, words, bytes and max line info contained in each input file.

Compile C project.

```console
$ gcc -Wall -o mywc mywc.c
```


The line count of each file
```console
$ ./mywc -l deneme.txt 
11  deneme.txt 
```

The word count of each file
```console
$ ./mywc -w deneme.txt 
38  deneme.txt 
```

The byte count of each file
```console
$ ./mywc -c deneme.txt 
292  deneme.txt 
```

The max line length of each file
```console
$ ./mywc -L deneme.txt 
82  deneme.txt 
```

The line count, word count, byte count, max line length of each file
```console
$ ./mywc -l -w -c -L deneme.txt 
11 38 292 82  deneme.txt 
```

The line count, word count, byte count, max line length of each file
```console
$ ./mywc -l -w -c -L deneme.txt mywc.c
11 38 292 82  deneme.txt 
358 1188 10875 264  mywc.c 
```

With no given option, you can show line count, word count, byte count
```console
$ ./mywc deneme.txt mywc.c
11 38 292  deneme.txt 
358 1188 10875  mywc.c 
```

Show help information
```console
$ ./mywc --help
```

Show version information
```console
$ ./mywc --version
@Copyright 2021 - Version 1.0 - MIT License Custom head software module. Developed by Batuhan Düzgün
```
