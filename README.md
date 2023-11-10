# GUI-Framework
Universal C++ GUI Framework.<br>
This is a fully featured and usable GUI framework on a D2D1 Renderer. All you need to do is change the drawing functions to your desired renderer.<br>
[For an Xbox Gamebar Implementation](https://github.com/IntelSDM/GameBarGUI)

## Images:
<p align="center">
  <img src="./Images/2.png"
    style="width: 60%;" />
</p>

<p align="center">
  <img src="./Images/1.png"
    style="width: 60%;" />
</p>

## Features:
* Input Management
  * Character Input
  * Keyboard Press Up, Keyboard Press Down
  * Mouse Press Up, Mouse Press Down
  * Relative Window Mouse Position
* Drawing System
  * Circles
  * Rectangles
  * Gradient Rectangles
  * Triangles
  * Text
  * Clipped Text
  * Rounded Rectangle
  * Lines
  * Sprite Batch Drawing (Optimised Drawing For Many Draw Calls)
* Font System
  * Create Fonts
  * Retrieve Fonts From Dictionary
  * Measure Text Width
  * Measure Text Height
* GUI Framework
  * Toggle
  * Slider(Templated)
    * Int
    * Float
    * Double
  * Button
  * TextBox
    * Select Text(Select Regions Of Text)
    * Copy Text (Copy A Region Of Text)
    * Paste Text (Paste At Point)
    * Delete Text (Delete Character Or Region Of Text)
  * Label
  * Tabs
  * List Box Tabs
    * Scroll Bar To Navigate Many Tabs
    * Text Culling / Revealing
  * Group Box
    * Scroll Bar to Navigate Many Elements
    * Dynamic Size To Fit Text
    * Text Culling 
  * Combo Box
    * Scroll Bar To Navigate
    * Dynamic Size To Fit Text
    * Text Culling
  * Event Handler
    * Value Changed Event
