# random name generator

This is a small project, aiming at implementing a machine learning-like algorithm
in order to randomly generate names.

For the sake of simplicity, it has been designed to work with \*.csv files, both with input files and internal storage files.

## installation

NOTE: this project was made and built on linux. although it can be built and
works on other platforms, there is no guide for it.

1. download/clone the project onto your computer

```bash
git clone https://github.com/starmlerp/rnamegen
```

2. move to the project directory and install the project using `make`

```bash
cd rnamegen/
sudo make install 
```

3. run the project using the produced executable

```bash
rnamegen
```
## uninstallation

project also provides an uninstall `make` script, which can be used to remove the executable and config file from the system

```bash
sudo make uninstall
```

## description

### synopsis

```bash
rnamegen [-f <file>] [-o <file>] [-i <file>] [-n <num>]
```

### options

Keep in mind that the order of options is important; if you specify multiple -f or -i options, they will be loaded in that order. -o options should also be used after any -f or -i option, otherwise the program will seek for a file in the default path etc.

`-f <file>`

Read file with training values. The program expects this to be a CSV file containing names/words used for analysis.
names from the file will be analyzed and the results will be appended to an internal state

It is reccomended to have at least 1000 unique datapoints, as anything less can produce poor results.

Default value is "/etc/rnamegen/names.csv".

`-o <file>`
      
Outputs so-far generated data into a file.
Although the outputted file is in the CSV form, it is not very human readable. The file describes internal per-letter statistics data used to generate names, and as such requires a particular number of entries.

**The program will return an error while reading with `-i` if this file is mishandled, so it is not reccomended to manually edit it**

`-i <file>`
      
Reads statistics data from a file.
All of the statistics data will be appended to an internal state.

Expects a file generated with an `-o` option as input.

`-g <num>`

Number of generations to run the names through.
This will internally generate a batch of names, then grade each name using a predefined criterion based on the initial state (it is assumed that this state is consisting of only organic names, and as such is the only paramount state), pick a smaller subset of those names with highest grades, and generate a new state based on those names, for `num` turns.

This option must be speficied after any `-f` or `-i` flags, otherwise names from the default location will be loaded!

`-b <num>`

Size of the *b*atch of names to generate for each generation.
Default value is 10000.

This option must be specified before any `-g` flags to take effect, for any `-g` option, last appearing `-b` option is in effect.

`-s <num>`

Number of names to *s*elect from a batch for the next generation. this selection is done by finding the smallest element in the first `num` names from the batch, then while passing through the remainder of the batch, replacing any first element that is bigger than that one with it. Following that, next smallest element in the first part of the batch, and repeating until the end is reached.
Default value is 1000.

This option must be specified before any `-g` flags to take effect, for any `-g` option, last appearing `-s` option is in effect.

`-n <num>`

Number of names to generate. Names will be separated by newline.
