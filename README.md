# lat
> lat | lazy cat - a cat clone with some quality-of-life embellishments

## About

`lat` is a solution to a problem that doesn't exist. It's the awkward middle child in-between `cat` and `bat`. It was created because the author didn't want to have to configure `bat` just to get decent file printing, but wanted a little more control than `cat` offered.

`lat` does not expect to be used. `lat` expects to be forgotten and shunted into a corner because it does not belong in the slightest.

## Install
`lat` is not yet at a stable version (`v1.0.0`), so it is not on any package managers. 

```bash
git clone https://github.com/secondary-smiles/lat.git

cd lat

make

./build/lat -V

# now add lat to your $PATH/do whatever you want with the binary
```

## Use

`lat` can, for the most part, be a direct drop in for `cat`. However it really shines when embedded into another program. For example, in `fzf`, `lat` makes an excellent viewer with the command `fzf --command lat -r {}`.

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
	-V           show program version
	-h           display this help text

environment:
	NO_COLOR, see https://no-color.org/
```
