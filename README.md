# random name generator

This is a small project, aiming at implementing machine learning-like algorythm
in order to randomly generate names.

For the sake of simplicity, it has been designed to mostly work with \*.csv files, both with input files and internal storage files

## installation

NOTE: this project was made and build on linux! although it can be built and
work on other platforms, no guide for so is present!

1. download/clone the project onto your computer

```bash
git clone https://github.com/starmlerp/rnamegen
```

2. move to the project directory and build the project using `make`

```bash
cd rnamegen/
make
```

3. run the project using the produced `main` file

```bash
chmod +x main
./main
```

4. (optional) you can rename and move the executable to your binaries folder

```bash
mv main /usr/bin/rnamegen
```

## description

### synopsis

```bash
rnamegen [-f <file>] [-o <file>] [-i <file>] [-n <num>]
```

### options

`-f <file>`

Read file with training values. Program expects this to be a CSV file containing names/words used for analysis. It is reccomended to have at least 1000 unique datapoints, as anything less can produce poor results.

Default value is "data/names.csv".

`-o <file>`
      
Output generated data into a file. Although the outputted file is in the CSV form, it is not very human readable. The file describes internal per-letter statistics data used to generate names, and as such requires particular number of entries.

The program will return an error if this file is mishandled, so it is not reccomended to manually edit it

`-i <file>`
      
Read statistics data from a file. Expects a file generated with an `-i` option as input.

if the `-f` option is also provided, the statistics from this file will be appended to the ones generated using the other.

`-n <num>`

Number of names to generate. Names will be separated by newline.
