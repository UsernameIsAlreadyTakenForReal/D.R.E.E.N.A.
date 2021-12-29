# What is this?
A myoelectric robot hand, initially designed to be a prosthesis. Each finger is individually operated by a SG-90 servomotor. Wrist is not rotating

This is my bachelor's degree project. As with all projects, it turned out way worse than I had expected; these kinds of projects require multiple iterations to get right, especially if you don't have experience with designing 3D objects that can be actually used. I only had one shot. Thanks, Eminem. I like how GitKraken looks, so I made a repo as well.

# If, for any reason, you want to get this working too...
which you shouldn't, since it requires a lot of stuff and there are easier projects to set up out there, here are some guides:

## *Pre- stuff that you need:*
- Blender or convertor for .blend files if you want to change the 3D models;
- A 3D printer. Project was printed with Ender 3 Pro, via software Ultimaker Cura 4.8.0, with PLA 1.75mm grey material, with 0.16mm step
- Arduino IDE, really. Project was created with Visual Studio 2019 and vMicro
- .blend files go big or go home. Need LFS: https://git-lfs.github.com/

## *For printing:*
For simpletons, each .STL file has to be printed ONCE. The rest of this section is me licking my own arse. The palm is made up of three segments: the knuckles, the back of the hand and the knuckles. Thumb is one single object. Fingers are made up of two segments: proximal and distal. Distal has two segments, a _Body and a _Guard. 

1. Fingers:
   1. Index finger and Middle finger have the same Proximal model (the longest one);
   2. Index finger, Ring finger and Pinky finger have the same Distal model (_Body + _Guard);
   3. Ring finger has its own Proximal model (the middle-sized one);
   4. Pinky finger has its own Proximal model (the shortes one);
2. Thumb finger is the only one made up of one single segment, is a singular object, so that's fine. Thumb is referred to as 'thumb', not 'finger';
3. Palm + Wrist:
   1. Palm has Palm_Base and Palm_Interior; these two have little holes in them for magnets of ø 2mm;
   2. Knuckles were meant to be part of Palm_Interior, but due to it being impossible to print, they are now separated;
   3. Wrist is just an object, so that's fine. 

## *Components needed:*
Oh, boy...
1. Arduino Mega 2560 or any board with at least 5 PWM pins, 13 digital pins and 1 analog pin;
2. 1 myoelectric sensor. The code is written for a MyoWare AT-04-001;
3. 5 SG-90 servomotors;
4. 1 button to switch between function modes;
5. 1 4-digit 7-segment display;
6. Connectors (11 for the display, 3 for each servomotor, 3 for the myoelectric sensor, 3 for the button, 2 for the led;
7. 1 led;
8. Power supply for the servomotors;

Physical components...
1. Elastic string of at most ø 2mm (recommend ø 1.5mm)
2. Non-elastic string of at most ø 0.5mm
3. 10 ø 2mm cylindric magnets

![Note that servomotors are powered separately, though a 9V battery will not suffice. They need much, much more power](/Extra Files/Schema.png)

## *For other wirings:*
Oh, boy 2.0... If you really made this this far and wish to continue (don't) please read everything before trying anything, and please test every component, electronic or not, before actually tying, knotting and glueing anything!
1. Run a non-elastic chord (like fishing strings) through the small hole in each finger; make a knot at the fingertip. The knot will be hidden by the _distal_guard_ segments;
2. Run an elastic chord through each finger's two big holes as to make an elongated elipsis that holds the finger together WITH the knuckles as well (knots!); by now you should have all 4 fingers attached to the knuckles and be able to pull the non-elastic chords to make the fingers flex;
3. Knuckles go into the small entrance of the Palm_Base segment; glue it if you want;
4. Servomotors go into the square-ish holes in the Palm_Base. Attach the shortest white extension; you will attach the non-elastic chord to it (tie it. Suffer like I suffered). Experiment with the 0 rotation of each that means finger is extended (since they are rotated IRL, their '0' rotation can be either 0 or 180 from the servo's view). For the motors associated with the pinky and ring finger you have to lay them on a side. To do this, you need to cut one wing off of each of these two motors. As for the layout, each motor can be placed in one way as to make the non-elastic chord take the shortest path;
5. Hope you have ø 2mm cylindric magnets lying around, 'cause you need them to hold Palm_Base and Palm_Interior together and still be able to open them up when needed;
6. Run a non-elastic chord and an elastic chord through the thumb and Palm_Interior; good luck at making that knot with the servo's white extension!;
7. Take all connectors from the servomotors out through the whole at the base of the palm. Add the wrist. And that's it.

## *For understanding the project:*

There are 2 modes of operation: with predefined grips and free mode. There are 4 groups of predefined grips, each with 2 grips:
1. Basic
   1. Fist
   2. Grip
2. Pinches
   1. Pinch
   2. PinchNoFingers
3. Tripods
   1. Tripod
   2. TripodNoFingers
4. Extra (programmable)
   1. RockOn
   2. Extra2 

The button has these possible functions, based on pressing length and number of presses:
   rapid (<1 sec) | 1 press   - change grip mode (two grip modes in a group)
   rapid (<1 sec) | 2 presses - next grip group

   slow (1 - 3 sec) | 1 press - lock servos

   v slow (3-5 sec) | 1 press - change op mode

   vv slow (>5 sec) | 1 press - shut down