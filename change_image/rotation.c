#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <math.h>

const float PI=3.1415;


Uint32 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat* format);
SDL_Surface* load_image(const char* path);

Uint32 pixel_to_black(Uint32 pixel_color, SDL_PixelFormat* format) 
	// convert a pixel on a black pixel
{
    Uint8 r, g, b;
	SDL_GetRGB(pixel_color, format, &r, &g, &b);
	int average= 0.0*r + 0.0*g + 0.0*b;
	return SDL_MapRGB(format, average,average ,average );
}

Uint32 copy_pixel(Uint32 pixel,SDL_PixelFormat* format)
	//deep copy of pixel
{

    	Uint8 r, g, b;
	SDL_GetRGB(pixel, format, &r, &g, &b);
	return SDL_MapRGB(format, r,g ,b );
}

long to_int(float fl)
	//convert float to int
{
	if((long)(fl+0.5)==(long)fl+1)
		return (long)fl+1;
	else 
		return (long)fl;
}

SDL_Surface* sub_pixel(SDL_Surface* surface,int sub)
	//subdivide list pixel sub² sub is a number (2 is good)
	//return SDL_Surface *
{
	// list of pixel
    Uint32* pixels = surface->pixels;
    unsigned long int W = (unsigned long int)surface->w ;
    unsigned long int H=(unsigned long int)surface->h;
    SDL_Surface* copy= SDL_CreateRGBSurface(0,W/sub,H/sub,32,0,0,0,0);
    SDL_PixelFormat* format = surface->format;
    SDL_LockSurface(surface);
    Uint32* pixelcopy = copy->pixels;

    //this is for average
    int den=0;
    int r=0;
    int g=0;
    int b=0;
    for(unsigned long int i =0;i<(W*H)/(sub*sub);i++)
    {
	    for(int u=0;u<sub;u++)
	    {
		    for(int j=0;j<sub;j++)
		    {
			    if((i%W*sub)+((i/W)*W*sub*sub)+j+(u*sub*W))
			    {
    				Uint8 lr, lg, lb;
				SDL_GetRGB(
					pixels[(i%W*sub)+((i/W)*W*sub*sub)
					+j+(u*sub*W)], format, &lr, &lg, &lb);
				if(lr+lg+lb)
					// if this is a black pixel I don't 
					// want to take this in average
				{
					r+=lr;
					b+=lb;
					g+=lg;
					den+=1;
				}
			    }
		    }
	    }
	if(den)//security of divition by zero
	{
		pixelcopy[i]=SDL_MapRGB(format,r/den,g/den,b/den);
	}
	else
	{
		// if r,g,b is zero
		pixelcopy[i]=SDL_MapRGB(format,r,g,b);
	}
	r=0;
	b=0;
	g=0;
	den=0;
    }
    free(surface);
    return copy;
}

SDL_Surface* mul_pixel(SDL_Surface* surface,int mul)
	//multiply list pixel mul² mul is a number (2 is good)
	//return SDL_Surface *
{
    Uint32* pixels = surface->pixels;
    unsigned long int W = (unsigned long int)surface->w ;
    unsigned long int H=(unsigned long int)surface->h;

    // -create new surface
    SDL_Surface* copy= SDL_CreateRGBSurface(0,W*mul,H*mul,32,0,0,0,0);
    SDL_PixelFormat* format = surface->format;
    SDL_LockSurface(surface);
    Uint32* pixelcopy = copy->pixels;
    for(unsigned long int i =0;i<W*H;i++)
    {
	    for(int u=0;u<mul;u++)
	    {
		    for(int j=0;j<mul;j++)
		    {
			    if((i%W*mul)+((i/W)*W*mul*mul)+j+(u*mul*W))
			    {
				    //change pixel in pixelcopy
				pixelcopy[(i%W*mul)+((i/W)*W*mul*mul)
					+j+(u*mul*W)]=
					copy_pixel(pixels[i],format);
			    }
		    }
	    }
    }
    free(surface);
    return copy;
}

void surface_to_turn(SDL_Surface* surface, float val_angle)
	// rotate a surface 
{
    Uint32* pixels = surface->pixels; // take list of pixel
    long W = surface->w ; //height and weight
    long H= surface->h;

    // -create copy surface
    SDL_Surface* copy= SDL_CreateRGBSurface(0,W,H,32,0,0,0,0);
    SDL_PixelFormat* format = surface->format;
    SDL_LockSurface(surface);
    Uint32* pixelcopie = copy->pixels;
    for (long i=0;i<W*H;i++)
    {
	    pixelcopie[i]=copy_pixel(pixels[i],format);
	    //copy of pixel list
	    pixels[i]=pixel_to_black(pixels[i],format);
	    //change all pixel in black pixel
	    
    }
    long X; 	//init of W in double for
    long Y; 	//init of H in double for
    float Xcentre=H/2.0-0.5; //val of X center
    float Ycentre=W/2.0; //val of y center
    //printf("%f,%f,%i,%i,%f\n",Xcentre,Ycentre,W,H,Xcentre*H+Ycentre);
    //int centre=Xcentre*H+Ycentre; // val of center
    float angle=val_angle/180.0*PI; //val of angle
    for(long i=0;i<H;i++)
    {
	    for (long j=0;j<W;j++)
	    {
		X=(W*i); // val of X this is for list 
	       	Y=j;     // val of Y this is for list  so with X+Y
		long placeX =  // determinate new place of pixel
			W*(to_int(Xcentre+(((cosf(angle))) * (i-Xcentre))
					-(( sinf(angle) * (j-Ycentre)))));
		
       		//this is for x, it's like var X
		long placeY=(Ycentre+(((( sinf(angle))) * (i-Xcentre))
					+( cosf(angle)*(j-Ycentre))));
				//this is for y, it's like var Y
		//printf("place= %i, centre=%i\n",place,centre);
		if (placeX+placeY>=0 
			&& placeY>=0 && placeY<=W	
			&& placeX+placeY<=H*W)
		{
			pixels[to_int(placeX+placeY)]=pixelcopie[X+Y]; 
			//fixe position of pixel
		}
		//pixels[X+Y]=pixelcopie[ //useless like me ;-;
		//	(H*( cos(angle)) * (i-Xcentre))-
		//	( sin(angle) * (j-Ycentre))+
		//	(H*( sin(angle)) * (i-Xcentre))-
		//	( cos(angle)*(j-Ycentre))];
		//pixels[X+Y]=pixel_to_grayscale(pixels[X+Y],format);
		//pixels[(i*H)+j]=pixelcopie[((H*i)+(H-j-1))];
		//pixels[(i*H)+j]=pixelcopie[(W-i)*H+j];	   
	    }
    }
    SDL_UnlockSurface(surface);
}

void surface_to_turn_an(SDL_Surface* surface, float angle)
{
	// rotate a surface same as surface_to_turn but he need a angle in 
	// -2pi to 2pi
    Uint32* pixels = surface->pixels; // take list of pixel
    long W = surface->w ; //height and weight
    long H= surface->h;

    // -create copy surface
    SDL_Surface* copy= SDL_CreateRGBSurface(0,W,H,32,0,0,0,0);
    SDL_PixelFormat* format = surface->format;
    SDL_LockSurface(surface);
    Uint32* pixelcopie = copy->pixels;
    for (long i=0;i<W*H;i++)
    {
	    pixelcopie[i]=copy_pixel(pixels[i],format);
	    //copy of pixel list
	    pixels[i]=pixel_to_black(pixels[i],format);
	    //change all pixel in black pixel
	    
    }
    long X; 	//init of W in double for
    long Y; 	//init of H in double for
    float Xcentre=H/2.0-0.5; //val of X center
    float Ycentre=W/2.0; //val of y center
    //printf("%f,%f,%i,%i,%f\n",Xcentre,Ycentre,W,H,Xcentre*H+Ycentre);
    //int centre=Xcentre*H+Ycentre; // val of center
    for(long i=0;i<H;i++)
    {
	    for (long j=0;j<W;j++)
	    {
		X=(W*i); // val of X this is for list 
	       	Y=j;     // val of Y this is for list  so with X+Y
		long placeX =  // determinate new place of pixel
			W*(to_int(Xcentre+(((cosf(angle))) * (i-Xcentre))
					-(( sinf(angle) * (j-Ycentre)))));
		
       		//this is for x, it's like var X
		long placeY=(Ycentre+(((( sinf(angle))) * (i-Xcentre))
					+( cosf(angle)*(j-Ycentre))));
				//this is for y, it's like var Y
		//printf("place= %i, centre=%i\n",place,centre);
		if (placeX+placeY>=0 
			&& placeY>=0 && placeY<=W	
			&& placeX+placeY<=H*W)
		{
			pixels[to_int(placeX+placeY)]=pixelcopie[X+Y]; 
			//fixe position of pixel
		}
		//pixels[X+Y]=pixelcopie[ //useless like me ;-;
		//	(H*( cos(angle)) * (i-Xcentre))-
		//	( sin(angle) * (j-Ycentre))+
		//	(H*( sin(angle)) * (i-Xcentre))-
		//	( cos(angle)*(j-Ycentre))];
		//pixels[X+Y]=pixel_to_grayscale(pixels[X+Y],format);
		//pixels[(i*H)+j]=pixelcopie[((H*i)+(H-j-1))];
		//pixels[(i*H)+j]=pixelcopie[(W-i)*H+j];	   
	    }
    }
    SDL_UnlockSurface(surface);
}
