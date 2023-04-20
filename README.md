# lat
> lat | lazy cat - a cat clone with some quality-of-life embellishments

[![Build](https://github.com/secondary-smiles/lat/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/secondary-smiles/lat/actions/workflows/c-cpp.yml)

## About

`lat` is a solution to a problem that doesn't exist. It's the awkward middle child in-between `cat` and `bat`. It was created because the author didn't want to have to configure `bat` just to get decent file printing, but wanted a little more control than `cat` offered.

`lat` does not expect to be used. `lat` expects to be forgotten and shunted into a corner because it does not belong in the slightest.

`lat` is fast. Here are the hyperfine results comparing `cat`, `lat`, and `bat` on a ~24MB text file:

```test
❯ hyperfine "cat log.log" "lat log.log" "bat log.log" -N --warmup 200
  Benchmark 1: cat log.log
    Time (mean ± σ):       3.6 ms ±   0.3 ms    [User: 0.4 ms, System: 2.3 ms]
    Range (min … max):     3.1 ms …   5.0 ms    851 runs

  Benchmark 2: lat log.log
    Time (mean ± σ):       6.2 ms ±   0.7 ms    [User: 0.4 ms, System: 4.2 ms]
    Range (min … max):     5.5 ms …  14.5 ms    496 runs

    Warning: Statistical outliers were detected. Consider re-running this benchmark on a quiet system without any interferences from other programs. It might help to use the '--warmup' or '--prepare' options.

  Benchmark 3: bat log.log
    Time (mean ± σ):     103.9 ms ±   0.5 ms    [User: 37.5 ms, System: 65.6 ms]
    Range (min … max):   103.2 ms … 104.8 ms    28 runs

  Summary
    'cat log.log' ran
    1.74 ± 0.26 times faster than 'lat log.log'
    29.10 ± 2.85 times faster than 'bat log.log'
```
> tested on a 2020 M1 MacBook Pro 16GB

as you can see, the extra features do take their toll on performance, but overall, `lat` is generally nearly on-par with `cat`, and *far* faster than `bat`.

## Install
`lat` is not yet at a stable version (`v1.0.0`), so it is not on any package managers. 

```bash
git clone https://github.com/secondary-smiles/lat.git

cd lat

make prep
make

./build/lat -V

# now add lat to your $PATH/do whatever you want with the binary
```

## Use

`lat` can, for the most part, be a direct drop in for `cat`. However it really shines when embedded into another program. For example, in `fzf`, `lat` makes an excellent viewer with the command `fzf --command lat -l {}`.

![Using lat in fzf](social/render/fzf_embed.gif)

Soon, I plan to add the ability to live-write to `lat`. That means that `lat` can become yet another output stream.

For example, this should be possible in a future version of `lat`

```c
int main(void) {
  FILE *st = popen("lat -l", "w");
  if (st == NULL)
  	exit(1);

  fprintf(st, "look ma, i'm formatted!")

  pclose(st);
}
```
> UPDATE: this feature is possible, however the data is only printed upon `pclose`.

### Features and Flags

#### `--color`
Completely disables or enables all colored output from `lat`.
##### Example
![Color in lat](social/render/color.gif)
> `lat` also respects [NO_COLOR](https://no-color.org/), but `--color` overrides it

#### `--lines`
Print numbers for each line of the file.
##### Example
![Line numbers in lat](social/render/line_numbers.gif)

#### `--headers`
Shows or hides formatted file data headers.
##### Example
![Headers in lat](social/render/headers.gif)

#### `--binary`
By default, `lat` will attempt to detect if the file is printable or not. If the file isn't (e.g. you ran `lat file.pdf`) then lat will enter *`binary mode`*. In *`binary mode`*, `lat` will skip a lot of processing in favor of speed.
You can force *`binary mode`* to be `on` or `off` with the flags `-b` and `-bb` respectively.
##### Example
![Binary Mode in lat](social/render/binary.gif)

#### `--literal`
`lat` is smart enough to print all non-file characters to a separate filestream. That way, the output of `lat` can be used to con**cat**enate files, the way it was originally meant to be.
However, sometimes you want those extra symbols.
`--literal` prints everything out to the primary filestream (usually `stdout`)
##### Example
![Literal output in lat](social/render/literal.gif)

#### `--pager`
`lat` comes with out-of-the-box support for paging in `less`. If you don't want your file messing up your terminal, just `-p` it.
##### Example
![Paging in lat](social/render/pager.gif)

#### If there is a feature you'd like to see, feel free to make an issue (1x points). If you're feeling especially savvy, make a PR with the feature (10x points).

 
## Helptext
> `lat --help`

```text
lat | lazy cat - a cat clone with some quality-of-life embellishments

usage: lat [-cntblpVh] [file...]

options:
  -c, --color     toggle color
  -n, --lines     toggle line numbers
  -t, --headers     toggle file info headers
  -b, --binary     toggle binary mode, -b forces binary and -bb forces NOT binary
  -l, --literal     print everything to stdout (or equivalent)
  -p, --pager     print file with the pager (uses less)
  -V, --version     show program version
  -h, --help     display this help text (--help shows additional info)

environment:
  NO_COLOR, see https://no-color.org/

examples:
lat file1
  print the contents of file1 with the default formatting
lat - file1
  read from stdin (the '-' flag reads from stdin) and then print the contents of stdin and file1
lat -nc file1 file2
  print the contents of file1 and file2 without printing line numbers or colors
lat --binary file.txt
  force file.txt to be treated as a binary file
lat -bb --pager file.txt
  force file.txt to NOT be treated as a binary file and print it in the pager
curl example.com | lat
  pipe the results of 'curl example.com' into lat
fzf --preview 'lat -l {}'
  use lat as the file viewer in fzf

```


