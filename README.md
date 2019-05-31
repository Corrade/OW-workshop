# Hitscan detection of spheres without raymarching
## code.txt
This file contains the code snippet that performs the action.

In this code I'm assuming you'd like to detect hitscan from an event player's eye position in the direction they're facing. If this isn't suitable for your project, please check the [explanation](#explanation) section and make the appropriate changes.

Let player variable `C` (`PV'C`) be the center of the object, `PV'R` be the radius of the object and `PV'A` be the player's maximum range. Then `code.txt` will return `True` if the object is being hit by the event player at the moment of execution, and will return `False` otherwise.



## demo.txt
This file contains the code for a small demo of the action.

Additionally you can watch the demo [here](https://giant.gfycat.com/DirectDizzyFrog.webm) or import it using `76X8K` if you're on PTR (1.37).

In the demo you can press `ability 1` to update the position of the object, which is a blue sphere.



## Notes
### Motivation
You can't hit effects with the raycast actions, so to implement the hitscan detection of spheres I'd imagine you'd either use raymarching or this method. Raymarching is bound to use loops, which create small delays, pollute the inspector and complicate situations where multiple objects have to be checked. This method is simply one action that returns either `True` or `False` for hit/not hit. It's far cleaner and reduces the task of considering multiple objects to a simple filtered array.



### Usage
Example applications include aim trainers and prop hunt.

To integrate this module into a gamemode, you might create a 2D array of objects containing `{center, radius}` at each index. You could then filter it using this code each time a player were to fire some hitscan weapon/ability. This would return an array of objects that would've been hit. You might prune this array if you only wanted a certain amount of objects to be hittable by a single shot, i.e. some restriction on piercing.

The spheres created by `Create Effect` have a radius that is 98.472% smaller than you specify (stat courtesy of LazyLion#5902 on Discord). You may want to account for this if you're using `Create Effect` for your objects. An example of a correction is provided in `demo.txt`.

Furthermore `Is In Line of Sight` doesn't seem to be completely accurate and you may register hits on underground sections of spheres. Blame Blizz (I think)!

### Explanation
The code has three `compare`s that represent three sections. We'll start with the first.

Let `C` be the center of the object, `A` be a vector on the ray and `V` be a vector parallel to the ray.

Then the shortest distance `D` from `C` to the ray is given by

    D = |C - A - projection(C - A onto V)|
      = |C - A - abs[(C - A).V / |V|] * normalize(V)|.

Now let the ray be the facing direction of the player passing through eye level. That is, `V = facing direction` and `A = some point along the ray through eye position in facing direction`. Then the goal is to check whether or not `D <= r` where `r` is the radius of some hittable object with center `C`. This will correspond to whether or not the player's crosshair is on the object.

Since our choice for `V` is always internally normalized, we can simplify our expression for `D`.

    D = |C - A - abs[(C - A).V] * V|.

There are a couple of extra conditions we must follow.

1. The player must be facing the object front-on. This is because our ray projects both forwards and backwards from the eye position. So, if a player were to face in the opposite direction of an object, we would normally detect a hit. This check will be implemented shortly in the first `compare`.
2. The player must have line of sight with the object. This is to avoid detecting hits through walls. We implement this in the second `compare` by finding the closest point on our ray to `C`, given by `A + projection(C - A onto V)`. This is almost the same as finding `D`.
3. The player must be in a specified range of the object. We implement this in the third `compare`.

Finally we must choose a value for `A`. All points on our ray, and hence all possible choices for `A`, can be found by modifying the real number `x` in the expression `Add(Eye Position(Event Player), Multiply(Facing Direction Of (Event Player), x))`.

A logical choice is setting `x = 0` as this will give the eye position, however we encounter unexpected results when doing so. Firstly, let's note that the shortest distance between a line and a point `P` must be less than or equal to the distance between `P` and any point on the line. In our situation this means `D <= Distance Between(Eye Position(Event Player), C)` since the eye position is some point on the line/ray.

Setting `x = 0` i.e. `A = Eye Position(Event Player)` does not comply with this bound. It turns out that only some values of `x`, for example `x = -10`, will satisfy the bound.

However, if we choose `x = 0`, it turns out that the error makes the hitscan detect forwards but not backwards (refer to the first condition). I'm not sure why this is, but it allows us to neatly integrate the first condition into the first `compare`. We can therefore go ahead and choose `x = 0` to complete the code.

As an aside, here's some rough data on what different values of `x` yield where `Y` means yes and `.` means no. I don't how know to interpret these findings.

    x       Forwards    Backwards
    0       Y           .
    1       Y           .
    10      .           .
    100     .           .
    -1      Y           .
    -10     Y           Y
    -100    Y           Y