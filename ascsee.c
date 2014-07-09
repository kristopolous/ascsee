#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <wand/MagickWand.h>

#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#define MIN(a,b) (((a) < (b)) ? (a) : (b))

// grab an argv.
#define ARG(x, y) (argc > (x) ? atoi(argv[x]) : (y))

// (death) larva stage -> pupa -> maturity 
//#define MAP " .',:;=*i!|lX@#"
//#define MAP " .',:;=*i!|lX@# .',:;=*i!|lX@#"
#define MAP " .`,'\"^~-_+=*/<ruvi!1lYVMOX@&#"

unsigned char
  *g_life, 
  *g_pixel;

int 
  g_width, 
  g_height;

unsigned char* pLife(int x, int y) { return &g_life[x * g_width + y]; }
unsigned char Life(int x, int y) { return g_life[x * g_width + y]; }

unsigned char* pPixel(int x, int y) { return &g_pixel[x * g_width + y]; }
unsigned char Pixel(int x, int y) { return g_pixel[x * g_width + y]; }

int main(int argc, char*argv[]) {
  fd_set fd_r;
  srand(time(0));
  struct timeval ts;

  MagickBooleanType stat;
  MagickWand *wand;

  int 
    cutoff = 225, 
    contrastRounds = rand() % 8 + 1,
    moveradius = -1,
    MATURE = 16,

    // chance of reproducing
    reproduce = ARG(2, rand() % 40 + 10), 

    // number of children bigger means fewer children
    litterSize = ARG(3, rand() % 8 + 8),

    // chance of dying
    dieoff = ARG(4, rand() % 10 + 2),

    // chance of trying to move around
    move = ARG(5, rand() % 50 + 2), 

    // chance of growth
    growth = ARG(6, rand() % 40 + 2), 

    // How many rounds to do before displaying to the screen
    viewEvery = ARG(7, 15),

    // frames per second
    fps = 45;

  FILE *fdesc;

  // critical population size
  // If the population falls below this amount, 
  // it gets seeded up to it.
  float criticalFrac = 0.1 / 100;

  int  
    fd,
    ix,
    iy,
    iz,
    c_height,
    c_width,
    critical,
    population = 0,
    landSize,
    i,
    j,
    maturity,
    turn = 0,
    limI, 
    limJ;

  if(argc > 1) {
    fd = open(argv[1], O_RDONLY);
    if(!fd) {
      printf("Can't open file: %s\n", argv[1]);
      exit(-1);
    }
  } else {
    printf("Please tell me the image file to use.\n");
    exit(-2);
  }

  ts.tv_sec = 0;

  // Determine the height and width of the users' current terminal
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  c_height = g_height = w.ws_row - 2;
  c_width =g_width = w.ws_col;
  landSize = g_height * g_width;

  // our critical population is a function of our landsize
  critical = MAX((int)( criticalFrac * (float)landSize ), 1);

  // the move radius is a function of the image height
  moveradius = MAX(0.07 * g_height, 1);

  // Load the file {
  MagickWandGenesis();

  wand = NewMagickWand();
  fdesc = fdopen(fd, "rb");
  stat = MagickReadImageFile(wand, fdesc);

  if (stat == MagickFalse) {
    printf("Image Magick can't read %s. It can usually read just about anything!\nIs this an image file? Are you sure?\n", argv[1]);
    return 0;
  }
  // }}

  g_life = (unsigned char*)malloc(sizeof(unsigned char) * landSize);
  g_pixel = (unsigned char*)malloc(sizeof(unsigned char) * landSize);
  memset(g_life, 0, landSize);

  MagickResetIterator(wand);

  while (MagickNextImage(wand) != MagickFalse);

  // increase the contrast of the image a few times
  for(ix = 0; ix < contrastRounds; ix++) {
    MagickContrastImage(wand, MagickTrue);
  }

  //MagickWriteImage(wand, "/tmp/file.png");

  MagickResizeImage(
    wand,
    g_width, g_height,
    LanczosFilter,
    1.0
  );
  MagickExportImagePixels(wand,
    0, 0,
    g_width, g_height,
    "I",
    CharPixel,
    g_pixel
  );

  printf("reproduce:%d litter:%d die:%d move:%d (rad:%d) grow:%d (crit:%d)\n",
    reproduce,
    litterSize,
    dieoff,
    move,
    moveradius,
    growth,
    critical
  );

  // We don't generate on screen at every step because that would be very tedious.
  for(;;turn++) {
    if(turn % viewEvery == 0) { 

      // seed if necessary
      for(ix = population; ix < critical; ix++) {
        for(;;) {
          i = rand() % g_height;
          j = rand() % g_width;
          if(Life(i, j) == 0) {
            *pLife(i, j) = MATURE;
            break;
          }
        }
      }
      ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

      // This makes sure that if the user resizes less then
      // the initial size, it won't screw up the generation into some
      // garbled mess.
      c_height = MIN(g_height, w.ws_row - 2);
      c_width = MIN(g_width, w.ws_col);
    }

    population = 0;

    for(ix = 0; ix < g_height; ix++) {
      for(iy = 0; iy < g_width; iy++) {

        if(turn % viewEvery == 0 && iy < c_width && ix < c_height) { 
          putchar(MAP[Life(ix, iy) >> 3]);
        }

        maturity = Life(ix, iy);

        if(maturity > 0) {

          population ++;

          // The chance of death is 
          //
          // random number % the brightness value of the pixel being < the dieoff
          // number.
          if(maturity >= Pixel(ix,iy) && (rand() % (Pixel(ix,iy) + 1)) < dieoff ) { 
            g_life[g_width * ix + iy] = 0;
            // reproduces 
          } else {
            if(maturity > MATURE && rand() % reproduce == 0) { 
              limI = MIN(ix + 2, g_height);
              limJ = MIN(iy + 2, g_width);

              for(i = MAX(ix - 2, 0); i < limI; i++){
                for(j = MAX(iy - 2, 0); j < limJ; j++){
                  if(Life(i,j) == 0 && (rand() % litterSize == 0)) {
                    *pLife(i,j) = 1;
                  }
                }
              }
            }
            // randomly move to a new empty spot 
            if(rand() % move == 0) { 

              limI = MIN(ix + moveradius, g_height);
              limJ = MIN(iy + moveradius, g_width);

              for(iz = 0; iz < 7; iz++) {
                i = ix + rand() % (moveradius * 2) - moveradius;
                j = iy + rand() % (moveradius * 2) - moveradius;

                i = MAX(i, 0);
                j = MAX(j, 0);

                i = MIN(i, g_height);
                j = MIN(j, g_width);
                
                if(Life(i, j) == 0) {
                  if(Pixel(i, j) > Pixel(ix, iy)) {
                    *pLife(i, j) =  Life(ix, iy);
                    *pLife(ix, iy) = 0;
                    break;
                  }
                }
              }
            }
            // growing up
            if(rand() % growth == 0) { 
              
              if(Life(ix, iy) < cutoff) {
                pLife(ix, iy)[0]++;
              }
            }
          }
        }
      }
      if(turn % viewEvery == 0 && ix < c_height) {
        putchar('\n');
      }
    }

    // This returns everything to the top for another draw.
    if(turn % viewEvery == 0) {
      // This makes sure we don't peg the processor.
      FD_ZERO(&fd_r);
      FD_SET(0, &fd_r);

      // This is our sleep value - at least 300
      ts.tv_usec = 1000000 / fps;
      select(1, &fd_r, NULL, NULL, &ts);
      if(FD_ISSET(0, &fd_r)) { 
        return(0);
      }

      printf("\033[%dA", c_height);
    }
  }
  free(g_life);
  free(g_pixel);
  return 0;
}
