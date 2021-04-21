## oglTessellatedFlag
### Simple OpenGL CPU tessellation sample
![flagScreen](https://user-images.githubusercontent.com/23703391/115539615-d8785380-a2a5-11eb-8564-77933310fe61.png)  
  
### Hotkeys
* **`1`**, **`2`**, **`3`** - change flag texture.
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
