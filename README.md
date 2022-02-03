pypag is the python binding for tencent libpag
# requirement
* CMake 3.22.2
* conda
* pybind11

# build
```
mkdir build
cd build
cmake ..
make
```

# run sample
in build directory, run
```
python ../sample/sample.py
```

# implemented APIs
## PAGFont
### static methods
* register_font
### properties
* font_family (readonly)
* font_style (readonly)

## PAGFile
### static methods
* load
### instance methods
* num_images
* replace_images
* num_texts
* get_text_data
* replace_text
### instance properties
* width (readonly)
* height (readonly)
* duration
* frame_rate (readonly)

## PAGImage
### static methods
* from_path

## TextDocument
### instance methods
* text
* font_family
* font_style

## PAGSurface
### static methods
* make_offscreen
### instance methods
* read_pixels

## PAGPlayer
### instance properties
* surface
* composition
* progress
### instance methods
* flush
* next_frame
