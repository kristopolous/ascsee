#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <wand/MagickWand.h>

#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#define MIN(a,b) (((a) < (b)) ? (a) : (b))
/* (death) larva stage -> pupa -> maturity */
#define MAP " .',:;=*i!|lX@#"
unsigned char *life;
unsigned char*pixel;
int width, height;

int Life(int x, int y) {
  return life[x * width + y];
}
int Pixel(int x, int y) {
  return pixel[x * width + y];
}

int main(int argc, char*argv[]) {
	fd_set fd_r;
	srand(time(0));
	struct timeval ts;

	const int cutoff = 225, 
	      reproduce = rand() % 40 + 10, 
        contrastRounds = rand() % 5,
	      dieoff = rand() % 5 + 1,
	      move = rand() % 20 + 1, 
	      moveCost = rand() % 50 + 1, 
	      tps = 300 * (rand() % 30 + 1);

	int	ix = 0,
		iy,
		i,
		j,
		turn = 0,
		best, 
		bstIx, 
		bstIy, 
		limI, 
		limJ;

  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  height = w.ws_row - 1;
  width = w.ws_col;
  printf("%d %d\n", height, width);

	ts.tv_sec = 0;
  MagickWandGenesis();

  MagickWand *wand = NewMagickWand();
  life = (unsigned char*)malloc(sizeof(unsigned char) * width * height);
  memset(life, width * height, 0);
  pixel = malloc(width * height * 2);
  int fd = open(argv[1], O_RDONLY);
  FILE *fdesc = fdopen(fd, "rb");
  MagickBooleanType stat;
  stat = MagickReadImageFile(wand, fdesc);

  if (stat == MagickFalse) {
    return 0;
  }

  MagickResetIterator(wand);

  while (MagickNextImage(wand) != MagickFalse);

  for(ix = 0; ix < contrastRounds; ix++) {
    MagickContrastImage( wand, MagickTrue);
  }

  //MagickWriteImage(wand, "/tmp/file.png");

  MagickResizeImage(
    wand,
    width, height,
    LanczosFilter,
    1.0
  );
  MagickExportImagePixels(wand,
    0, 0,
    width, height,
    "I",
    CharPixel,
    pixel
  );

	printf("(%d tps)\n\n", tps);
  for(ix = 0; ix < 10; ix++) {
  	life[rand() % (height * width)] = 32;
  }

/*
		for(ix = 0; ix < height; ix++) {
			for(iy = 0; iy < width; iy++) {
        putchar(MAP[Pixel(ix,iy) >> 4]);
      }
      printf("\n");
    }
*/
	for(;;turn++) {
		for(ix = 0; ix < height; ix++) {
			for(iy = 0; iy < width; iy++) {
				if( (turn&0xf) == 0) { 
					putchar(MAP[Life(ix,iy) >> 4]);
				}
				if(Life(ix,iy) > 0) {
          // dies
					if(Life(ix,iy) > Pixel(ix,iy) && (rand() % (Pixel(ix,iy) + 1)) < dieoff ) { 
						life[width * ix + iy] = 0;
            // reproduces 
					} else if(rand() % reproduce == 0) { 
							limI = MIN(ix + 2, height);
							limJ = MIN(iy + 2, width);
							for(i = MAX(ix - 1, 0); i < limI; i++){
								for(j = MAX(iy - 1, 0); j < limJ; j++){
								if(Life(i,j) == 0 && (rand() % 16 == 0)) {
									life[i * width + j] = 1;
								}
							}
						}
            // move to a better spot 
					} else if(rand() % move == 0) { 
              // current position 
							best = Pixel(ix,iy); 
							limI = MIN(ix + 1, height);
							limJ = MIN(iy + 1, width);
							for(i = MAX(ix - 1, 0); i < limI; i++){
								for(j = MAX(iy - 1, 0); j < limJ; j++){
									if(Life(i,j) == 0) {
										if(Pixel(i,j) > best) {
											bstIx = i;
											bstIy = j;
											best = Pixel(i,j);
										}
									}
								}
							}
                // found a new spot 
							if(best != Pixel(ix,iy)) {	
								life[bstIx * width + bstIy] = MIN(Life(ix,iy) + 1, cutoff);
								life[ix * width + iy] = 0;
							}
            // moving is more expensive then staying 
					} else if(rand() % moveCost == 0) { 
						life[ix * width + iy]++; 
						if(Life(ix,iy) > cutoff) {
							life[ix * width + iy] = cutoff;
						}
					}
				}
			}
			if((turn & 0xf) == 0) {
				putchar('\n');
			}
		}
		FD_ZERO(&fd_r);
		FD_SET(0, &fd_r);
		ts.tv_usec = 1000000 / tps;
		select(1, &fd_r, NULL, NULL, &ts);
		if(FD_ISSET(0, &fd_r)) { 
			return(0);
		}
		if((turn & 0xf) == 0) {
			printf("\033[%dA", height);
		}
	}
	return 0;
}
