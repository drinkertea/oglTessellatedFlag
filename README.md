## oglTessellatedFlag
### Simple OpenGL CPU tessellation sample
![NewFlag](https://user-images.githubusercontent.com/23703391/115838930-d556a200-a422-11eb-80a8-06d1476e49fc.png)
  
### Hotkeys
* **`ctrl + shift + w`** - toggle wireframe.
* **`-`**, **`=`** - decrase/increase value depended on combination:
    * **`shift`** - change amplitude.
    * **`ctrl`** - change wave count.
    * **`alt`** - change speed.
    * **`ctrl + shift`** - change tessellation level.
    * **`ctrl + alt`** - change angle degrees.
*  **`ctrl/shift + 1/2/3`** - decrase/increase rotation coefficients for x/y/z.

### Clone and Build
```
git clone --recursive https://github.com/drinkertea/oglTessellatedFlag.git
mkdir _build
cd _build
cmake .. -A x64
```
