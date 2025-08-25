
# Convol --- tool for BMP image filtering.


## Installation

```bash
# Clone project
git clone https://github.com/suvorovrain/Convol.git && cd Convol

# Build
make build
```

## Usage
```
Usage: ./convol <src_image> <effect> <strength> <dest> <algo_type>
```
*Effects:* `blur`,`motion_blur`,`find_edges`,`emboss`

*Strength of effect:* `1`,`2`,`3`

*Algorithm type:* `linear`,`parallel_pixel`,`parallel_row`,`parallel_column`,`split`