# A new kind of Image viewer 
<img src=http://rt2.me/out.gif>

(bubble girl from the terminal - <a href=http://youtu.be/Opu6qvvbTOs>Youtube version</a>)

A life-simulation where the greyscale intensity of each pixel of your image is the likelihood of survival for the computer "life" living there.  

Lighter values prosper giving fuller characters.
Darker pixels perish and die.

The results are your image - as a festering stew of artificial life.

The parameters of the simulation get tossed around randomly each time you use it.

The colonies will grow into your image in different ways; crawling across the screen slowly and rapidly devolving yielding nice results:

<a href=http://i.imgur.com/tst3rYc.png><img src=http://i.imgur.com/tst3rYcl.jpg></a>

Vermeer's *Music Lesson*
<a href=http://i.imgur.com/J9rloTj.png><img src=http://i.imgur.com/J9rloTjl.jpg></a>

Buzz Aldrin on the Moon. Look at the character details:
<a href=http://i.imgur.com/R8NIQfR.png><img src=http://i.imgur.com/R8NIQfRl.jpg></a>

Generating Michelangelo's *Creation of Adam*: [on youtube](http://youtu.be/H4j-BkwMB20)

I also suggest checking out pointillism works such as Seurat's *La Parade de Cirque*.

### You should really try it yourself.  It runs at 45 frames a second. This is higher than youtube will do playback so these rendered screen-caps are just inferior approximations of the actual viewing experience.

## Installing

### Linux
ascsee is written in C and requires, on debian systems, `libmagickwand-dev`. Try running make.

### OS X & Windows
I don't use these operating systems every day.  I think on OSX you can use Xcode and macports to generate the binary.  On Windows, my guesses are even flimsier - I really don't know.  If you are more familiar with these platforms, then please contact me and I'll gladly add you as a contributor.

## Usage

ascsee [image file to see] [options]

ctrl+c when you are done.  That is all.  Keepin' life simple.

The *options* permit you to non-randomize the variables of the simulation in the following way:

    ascsee [image file] [reproduce] [litter] [dieoff] [move] [grow]

Where:

 * reproduce - The chance of a mature life ( > `MATURE` ) to have offspring 
 * litter - The chance of having a child in the surrounding empty squares ( 1 in `litter` )
 * dieoff - The chance of dying given the the pixel value (if 1 in `brightness of pixel` < `dieoff`)
 * move - The chance of moving to a better square (higher pixel value) at distance `moveradius`.
 * grow - The speed of maturity (1 in grow -> next value on a 256 scale)

Any unspecified option falls back to randomization

## Image support

Under the cover we are using the powerful imagemagick library so almost every format under the sun is viewable.

## Future Dev

I realize that not many people use desktop linux so I plan to make an HTML canvas version in the near future or at leasst provide installation packages for more common desktop OS's.  Thanks.
