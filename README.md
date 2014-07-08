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

## Installing

ascsee is written in C and requires, on debian systems, `libmagickwand-dev`. Try running make.

## Usage

ascsee [image file to see]

ctrl+c when you are done.  That is all.  Keepin' life simple.

## Image support

Under the cover we are using the powerful imagemagick library so almost every format under the sun is viewable.
