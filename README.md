# lat
> lat | lazy cat - a cat clone with some quality-of-life embellishments

[![Build](https://github.com/secondary-smiles/lat/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/secondary-smiles/lat/actions/workflows/c-cpp.yml)

![lat demo](social/render/lat_demo.gif)

## About

`lat` is a solution to a problem that doesn't exist. It's the awkward middle child in-between `cat` and `bat`. It was created because the author didn't want to have to configure `bat` just to get decent file printing, but wanted a little more control than `cat` offered.

`lat` does not expect to be used. `lat` expects to be forgotten and shunted into a corner because it does not belong in the slightest.

`lat` is fast. Here are the hyperfine results comparing `cat`, `lat`, and `bat` on a `lat`'s `main.c` source file:

```text
❯ hyperfine "./build/lat src/main.c" "cat src/main.c" "bat src/main.c"  -N --warmup 200
Benchmark 1: ./build/lat src/main.c
  Time (mean ± σ):       0.2 ms ±   0.0 ms    [User: 0.1 ms, System: 0.0 ms]
  Range (min … max):     0.1 ms …   0.7 ms    19651 runs
 
  Warning: Statistical outliers were detected. Consider re-running this benchmark on a quiet system without any interferences from other programs. It might help to use the '--warmup' or '--prepare' options.
 
Benchmark 2: cat src/main.c
  Time (mean ± σ):       0.2 ms ±   0.0 ms    [User: 0.2 ms, System: 0.0 ms]
  Range (min … max):     0.2 ms …   0.4 ms    13717 runs
 
  Warning: Statistical outliers were detected. Consider re-running this benchmark on a quiet system without any interferences from other programs. It might help to use the '--warmup' or '--prepare' options.
 
Benchmark 3: bat src/main.c
  Time (mean ± σ):       1.4 ms ±   0.7 ms    [User: 1.1 ms, System: 0.3 ms]
  Range (min … max):     1.1 ms …   6.5 ms    1057 runs
 
  Warning: The first benchmarking run for this command was significantly slower than the rest (2.8 ms). This could be caused by (filesystem) caches that were not filled until after the first run. You are already using the '--warmup' option which helps to fill these caches before the actual benchmark. You can either try to increase the warmup count further or re-run this benchmark on a quiet system in case it was a random outlier. Alternatively, consider using the '--prepare' option to clear the caches before each timing run.
 
Summary
  './build/lat src/main.c' ran
    1.29 ± 0.28 times faster than 'cat src/main.c'
    9.14 ± 4.56 times faster than 'bat src/main.c'
```
~~as you can see, the extra features do take their toll on performance, but overall, `lat` is generally nearly on-par with `cat`, and *far* faster than `bat`.~~

**As of v0.12.2 `lat` is the new fastest of the three!** (on my machine)

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

`lat` can, for the most part, be a direct drop in for `cat`. However it really shines when embedded into another program. For example, in `fzf`, `lat` makes an excellent viewer with the command `fzf --command lat -r {}`.

![Using lat in fzf](social/render/fzf_embed.gif)

Soon, I plan to add the ability to live-write to `lat`. That means that `lat` can become yet another output stream.

For example, this should be possible in a future version of `lat`

```c
int main(void) {
  FILE *st = popen("lat -r", "w");
  if (st == NULL)
  	exit(1);

  fprintf(st, "look ma, i'm formatted!")

  pclose(st);
}
```
> UPDATE: this feature is possible, however the data is only printed upon `pclose`.

### Features and Flags

#### `-c` color
Completely disables or enables all colored output from `lat`.

#### `-l` line numbers
Print numbers for each line of the file.

#### `-t` file title
Shows or hides formatted file data headers.

#### `-b` binary mode
By default, `lat` will attempt to detect if the file is printable or not. If the file isn't (e.g. you ran `lat file.pdf`) then lat will enter *`binary mode`*. In *`binary mode`*, `lat` will skip a lot of processing in favor of speed.
You can force *`binary mode`* to be `on` or `off` with the flags `-b` and `-bb` respectively.

#### `-r` raw output
`lat` is smart enough to print all non-file characters to a separate filestream. That way, the output of `lat` can be used to con**cat**enate files, the way it was originally meant to be (with `>` or `|`).
However, sometimes you want those extra symbols.
`-r` prints everything out to the primary filestream (usually `stdout`)

#### `-p` pager
`lat` comes with out-of-the-box support for paging in `less`. If you don't want your file messing up your terminal, just `-p` it.

### `-n` name
`lat` allows you to customize the name of the file shown. This can be useful in demonstrations or when `lat` is embedded in another program.

#### If there is a feature you'd like to see, feel free to make an issue (1x points). If you're feeling especially savvy, make a PR with the feature (10x points).

 
## Helptext
> `lat -h`

```text
lat | lazy cat - a cat clone with some quality-of-life embellishments

usage: lat [-cltbrpneVh] [file...]

options:
	-c           toggle color
	-l           toggle line numbers
	-t           toggle file info headers
	-b           set binary mode, -b forces binary and -bb forces NOT binary
	-r           print everything (headers, line numbers, etc.) to stdout (or equivalent)
	-p           print file with the pager (uses less)
	-n <name>    manually set the name of the file shown in the title
	-e <program> link extension to lat
	-V           show program version
	-h           display this help text

environment:
	NO_COLOR, see https://no-color.org/
```
