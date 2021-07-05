# Termadore

A nod to times past, in homage to the demo scene days of the Commodore64.
Not to be taken seriously.

You are provided with a "screen" buffer based on your terminal dimensions and font size,
along with some basic drawing tools.  
What will you come up with?

Plus who doesn't love a bad play on words - we adore the terminal and the commodore.


## Description

The draw buffer is made up of "pixels" which are pairs of `utf-8` characters, referred to as shades.  
Pairs are used to more closely resemble square pixels.

```
Black: '██'
Dark:  '▓▓'
Mid:   '▒▒'
Light: '░░'
Clear: '  '
```
Note: If your terminal is using a dark-mode colour scheme, then the above shade names may appear as if reversed.


## Building

Currently in alpha testing. Default build is debug.  
Build options are:
```
$> make
$> make BUILD=release
$> make screen
$> make demo
$> make test
$> make test-<test-name>
```
Any test file added to `tests` directory, with the format `test-somename.c` should automatically compile with `make`, or can be done individually as above. 
You may have to include `-B` for the time being.


## Usage

The demos provide a reasonable example of usage.  

### General Approach

- `init_screen()`
  - Analyse the terminal and allocate our "screen" buffer
- `while(1)`
  - Create a "game loop" (traditionally an infinite `while`)
- `update()`
  - Write your own update function(s) that draw to the buffer
- `render()`
  - Draw to `stdout`
  - Frame rate is currently throttled to 30fps
- `kbhit()`
  - Detect if/when the user has pressed a key
  - Good for a clean way to quit
- `cleanup()`

### Helper Functions

- `int get_width()`
  - Current buffer width. May differ from actual terminal width.
- `int get_height()`
  - Current buffer height. May differ from actual terminal height.
- `float get_dt()`
  - Time in seconds since the last frame
- `void fill(shade)`
  - Fill the entire buffer with a specific shade
- `void draw_line(x, y, len, shade)`
  - _Currently only horizontal_
- `void set_pixel(x, y, shade)`
- `void set_pixel_at_pen(shade)`
  - Each write also steps the "pen" forward
- `void set_pen(x, y)`


## Todo

- Write proper tests
- Add colours
- Allow user to set max FPS
