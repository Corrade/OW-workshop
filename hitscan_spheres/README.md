# Hitscan detection of spheres without raymarching
## Function
In the code I'm assuming you'd like to detect hitscan from an event player's eye position in the direction they're facing.

Let player variable `C` (`pC`) be the center of the object, `pR` be the radius of the object and `pA` be the player's maximum range. Then `code.ow` will return `True` if the object is being hit by the event player at the moment of execution, and will return `False` otherwise.



## Demo
The file `demo_code.ow` contains the code for a small demo of the action.

Additionally you can watch the demo [here](https://giant.gfycat.com/DirectDizzyFrog.webm) or import it using `76X8K`.`

In the demo you can press `ability 1` to update the position of the object, which is a blue sphere.



## Motivation
You can't hit effects with the raycast actions, so to implement the hitscan detection of spheres I'd imagine you'd either use raymarching or this method. Raymarching is bound to use loops, which are slow, messy and fussy, especially where multiple objects have to be checked. This method is simply one action that returns either `True` or `False` for hit/not hit.



## Usage
Example applications include aim trainers and prop hunt modes.

To integrate this module into a gamemode, you might create a 2D array of objects containing `{center, radius}` at each index. You could then filter it using this code each time a player were to fire some hitscan weapon/ability. This would return an array of objects that would've been hit. You might prune this array if you only wanted a certain amount of objects to be hittable by a single shot, i.e. some restriction on piercing.

The spheres created by `Create Effect` have a radius that is 98.472% smaller than you specify (stat courtesy of LazyLion#5902 on Discord). You may want to account for this if you're using `Create Effect` for your objects. An example of a correction is provided in `demo_code.ow`.

Furthermore `Is In Line of Sight` doesn't seem to be completely accurate and you may register hits on underground sections of spheres. Blame Blizz, probably.