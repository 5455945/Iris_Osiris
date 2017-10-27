 
---
| **`Linux`** | **`Windows`** |
|-----------------|---------------------|
| [![Build Status](https://www.travis-ci.org/5455945/Iris_Osiris.svg?branch=master)](https://www.travis-ci.org/5455945/Iris_Osiris) | [![Build Status](https://ci.appveyor.com/project/5455945/iris-osiris)](https://ci.appveyor.com/project/5455945/iris-osiris) |


Build
---

1. Download Iris_Osiris(https://github.com/5455945/Iris_Osiris.git)
```bash
git clone --recursive https://github.com/5455945/Iris_Osiris.git
```

2. Build Iris_Osiris.

on windows
```bash
cd Iris_Osiris

mkdir build & cd build

cmake -G "Visual Studio 14 2015 Win64" -DOpenCV_DIR="OpenCV 3.0_Path' or OpenCV 2.4.13 Path" ..

cmake --build . --config Release

cmake --build . --config Debug

```

on unix
```bash
cd Iris_Osiris

mkdir build; cd build

cmake -G -DOpenCV_DIR="OpenCV 3.0_Path' or OpenCV 2.4.13 Path" ..

make
```

3. run
on windows
```bash
Debug\Osiris.exe ../data
```

```bash
#Output file: ../data/Output
>Debug\Osiris.exe ../data
=============
Configuration
=============

- Process : | segmentation || normalization || encoding |
- List of images ../data/process_CASIA-IrisV2.txt contains 8 images

- Original images will be loaded from : ../data/CASIA-IrisV2/

- Segmented images will be saved as : ../data/Output/SegmentedImages/XXX_segm.bmp
- Parameters will be saved as : ../data/Output/CircleParameters/XXX_para.txt
- Masks will be saved as : ../data/Output/Masks/XXX_mask.bmp
- Normalized images will be saved as : ../data/Output/NormalizedImages/XXX_imno.bmp
- Normalized masks will be saved as : ../data/Output/NormalizedMasks/XXX_mano.bmp
- Iris codes will be saved as : ../data/Output/IrisCodes/XXX_code.bmp

- Pupil diameter ranges from 50 to 160
- Iris diameter ranges from 160 to 280
- Size of normalized iris is 512 x 64

- 6 Gabor filters : 9x15 9x15 9x27 9x27 9x51 9x51

================
Start processing
================

1 / 8
2 / 8
3 / 8
4 / 8
5 / 8
6 / 8
7 / 8
8 / 8

==============
End processing
==============
```

on unix
```bash
Debug/Osiris.exe ../data

#Output file: ../data/Output

```

Update Log
---
support OpenCV3.3,OpenCV2.4.13

---
[![license](https://img.shields.io/github/license/mashape/apistatus.svg?maxAge=2592000)](https://github.com/5455945/Iris_Osiris/blob/master/LICENSE)

