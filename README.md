
# Convol --- tool for BMP image filtering.


## Installation

```bash
# Clone project
git clone https://github.com/suvorovrain/Convol.git && cd Convol

# Build
make build
```

## Usage
### Single image mode
```
Usage: ./convol <src_image> <effect> <strength> <dest> <algo_type>
```
*src_image:* Source `.bmp` file

*Effects:* Possible values: `blur`,`motion_blur`,`find_edges`,`emboss`

*Strength of effect:* Possible values: `1`,`2`,`3`

*Algorithm type:* Possible values: `linear`,`parallel_pixel`,`parallel_row`,`parallel_column`

*dest:* New `.bmp` file
### Multiple image mode
```
Usage: ./convol <src_dir> <effect> <strength> <dest_dir> <algo_type>
```
*src_dir:* Folder that contains at least one `.bmp` file

*Effects:* `blur`,`motion_blur`,`find_edges`,`emboss`

*Strength of effect:* `1`,`2`,`3`

*Algorithm type:* `stream`

*dest_dir:* Folder that will store result `.bmp` files