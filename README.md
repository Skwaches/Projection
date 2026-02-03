This project is supposed to focus on 3d rendering.
SDL is the only library I plan on using.
The idea is inspired by this wonderful video by Tsoding on youtube:
https://www.youtube.com/watch?v=qjWkNZ0SXfo
Be sure to watch it if you haven't already.
The idea is that the projection of a point is easily calculated by this simple formulae:
    x' = x/z
    y' = y/z

It's very easy to prove! Suprised I hadn't thought about it before!
It assumes that the screen coordinates are relative to the center of the screen and that x and y go to the right and upwards respectively.
And that the screen is of unit length and height.
The 3d point should be converted to the screen coordinate system before the render for it to comeout well.

