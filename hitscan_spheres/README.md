# Hitscan detection of spheres without raymarching
## Function
`code.ow` is a Boolean expression. It returns `True` only if an object with center `hs_center` and radius `hs_radius` would be hit by the event player firing a hitscan weapon with a range of `hs_range` at the moment of execution.



## Usage
Example modes include aim trainers and prop hunt.

To use this code, you might create a 2D array of objects containing `{center, radius}` at each index. You could then filter it with this code as the condition every time a player fires something hitscan. This will give an array of objects that have been hit.

Note: the spheres created by `Create Effect` have a radius that is visually 98.472% smaller than you specify, stat courtesy of LazyLion#5902 on Discord. Also, the function `Is In Line of Sight` isn't completely accurate and you'll register hits on underground sections of spheres. I think this is an issue on Blizzard's end.



## Demo
The file `demo_code.ow` contains the code for a small demo of the action. In it, you can press `ability 1` to move the object to your feet.

Alternatively, you can watch the demo [here](https://giant.gfycat.com/DirectDizzyFrog.webm) or import it using `76X8K`.



## Motivation
You can't hit effects with the raycast actions, so to implement the hitscan detection of spheres I'd imagine you'd either use raymarching or this method. Raymarching is bound to use loops, which are awkward, especially where multiple objects have to be checked. This method is simply one action that returns either `True` or `False` for hit/not hit.


