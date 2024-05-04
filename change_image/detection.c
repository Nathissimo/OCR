#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <math.h>
#include "rotation.h"
#include "detection.h"


// rec_rech_s return index of the end of figure, i must be the top of figure
// a figure is only red pixel r=200
//
// he need pixels array of picture
// 	format of pixels
// 	index of first pixels i,j
// 	size of array h,w
// 	pointer of index (he must be initialised to 0)
// 
// he return char(boolean)
char rec_rech_s(Uint32* pixels,SDL_PixelFormat* format,
		size_t i,size_t j,size_t h,size_t w,
		size_t *s,size_t *e,size_t *we,int col)
{
	if(j>=w||i>=h)
		return 0;
	// take color of pixel
	Uint8 r, g, b;
	SDL_GetRGB(pixels[(i*w)+j], format, &r, &g, &b);

	//if is not a red pixel return false
	if((r!=200 && col!=1)||(g!=200 && col==1))
	{
		return 0;
	}
	
	//change color of pixel
	if(col==1)
	{
		pixels[(i*w)+j]=SDL_MapRGB(format, 200,0,0);
	}
	else
		pixels[(i*w)+j]=SDL_MapRGB(format, 0,200,0);

	//lanch rec_rech_s in pixel next to him and under
	rec_rech_s(pixels,format,i,j-1,h,w,s,e,we,col);

	rec_rech_s(pixels,format,i+1,j-1,h,w,s,e,we,col);

	rec_rech_s(pixels,format,i+1,j,h,w,s,e,we,col);

	rec_rech_s(pixels,format,i+1,j+1,h,w,s,e,we,col);

	rec_rech_s(pixels,format,i,j+1,h,w,s,e,we,col);
	
	// change values of pointer if this is necessary
	if(i>*s)
	{
		*s=i;
	}

	if(j>*e)
	{
		*e=j;
	}

	if(j<*we)
	{
		*we=j;
	}

	//return true
	return 1;
}

// return pointer of size of place of bigger squarre, in this squarre the are
// the bigger figure in red r=200 (or green g=200 if col==1)
// pointer is n s e we 
// 	n is index of top
// 	s is index of buttom
// 	e is index of right
// 	we is index of left
//
// he need surface
size_t* rech_s(SDL_Surface* surface,char col)
{
	Uint32* pixels = surface->pixels; // take list of pixel
	size_t W = surface->w ; //height and weight
	size_t H= surface->h;
	
	//take format
	SDL_PixelFormat* format = surface->format;
	SDL_LockSurface(surface);

	//initialised nsew (this is n s e we)
	size_t* nsew=malloc(4*sizeof(size_t));

	// this is important for rec_rech_s, this will be a futur test value
	size_t s=0;
	size_t e=0;
	size_t we=0;

	// initialise value of nsew
	for(int i =0;i<4;i++)
		nsew[i]=0;
	
	// lanch browse of pixels
	for(size_t i=0;i<H;i++)
	{
		for(size_t j=0;j<W;j++)
		{
			// reset s e and w
			s=0;
			e=0;
			we=W;

			// lanch rec_rech_s
			if(rec_rech_s(pixels,format,i,j,H,W,&s,&e,&we,col))
			{
				//if this fugure are more surface he will 
				//	be register
				if((s-i)*(e-we)>
					(nsew[1]-nsew[0])*(nsew[2]-nsew[3]))
				{
					nsew[0]=i;
					nsew[1]=s;
					nsew[2]=e;
					nsew[3]=we;
				}
				//i=s;
				j=e;
			}
		}
	}

	return nsew;

}


char rec_red(Uint32* pixels,SDL_PixelFormat* format,size_t i,size_t j,
		size_t lon,char angle,size_t h,size_t w);

// he color the lisne in red if you need you can modifi tolerance for different
// 	result
// he need surface to picture
void surface_to_red(SDL_Surface* surface)
{
	Uint32* pixels = surface->pixels; // take list of pixel                     
	size_t W = surface->w ; //height and weight                                    
     	size_t H= surface->h;
	
	// take format of pixel
	SDL_PixelFormat* format = surface->format;
	SDL_LockSurface(surface);

	// this is min of size to register line
	size_t mid=H/20;
	size_t mid2=W/20;
	
	for(size_t i=0;i<H;i++)
	{
		for(size_t j=0;j<W;j++)
		{
			//1 is right
			rec_red(pixels,format,i,j,mid2,1,H,W);
			//2 is bottom
			rec_red(pixels,format,i,j,mid,2,H,W);
			// 3 is right-down
			rec_red(pixels,format,i,j,mid,3,H,W);
			// 4 is left-down
			rec_red(pixels,format,i,j,mid,4,H,W);
		}
	}
}


// he color with r<200 line in red line r=200
// he need array of pixels, format of picture, start index i and j
// 	len of tolerance, angle : 1 left | 2 down | 3 left-down 
// 	size of picture h and w
//
// return char if he have modify the pixel
char rec_red(Uint32* pixels,SDL_PixelFormat* format,size_t i,size_t j,
		size_t lon,char angle,size_t h,size_t w)
{
	// take color of pixels
	Uint8 r, g, b;
	SDL_GetRGB(pixels[(i*w)+j], format, &r, &g, &b);

	// lon-- but he can't be negatif so I use the if
	if(lon>0)
	{
		lon--;
	}
	// if this pixel is not red
	if(r<200)
	{
		return lon==0;
	}

	if (angle==2)
	{
		//if I can get in down
		if(h>i+1)
		{
			// lanch in down
			if(rec_red(pixels,format,i+1,j,lon,angle,h,w))
			{
				pixels[(i*w)+j]=SDL_MapRGB(format, 200,0,0);
				return 1;
			}
			else
			{
			// see if next pixel is no red
			SDL_GetRGB(pixels[((i+1)*w)+j], format, &r, &g, &b);
			if(r<200)
			{
				// look if he can color left-dow or right-down
				if(j>0 && 
				rec_red(pixels,format,i+1,j-1,lon,angle,h,w))
				{
					pixels[(i*w)+j]=
						SDL_MapRGB(format, 200,0,0);
					return 1;
				}
				else 
				if (w>j+1 && 
				rec_red(pixels,format,i+1,j+1,lon,angle,h,w))
				{
					pixels[(i*w)+j]=
						SDL_MapRGB(format, 200,0,0);
					return 1;
				}
				return lon==0;
			}
			return lon==0;
			}
		}
		else
		{
			return lon==0;
		}
	}
	if (angle==1)
	{
		if(w>j+1)
		{
		// this is like previously
		if(rec_red(pixels,format,i,j+1,lon,angle,h,w))
			{
				pixels[(i*w)+j]=SDL_MapRGB(format, 200,0,0);
				return 1;
			}
			else
			{
			SDL_GetRGB(pixels[((i)*w)+j+1], format, &r, &g, &b);
			if(r<200)
			{
				// top-right and bot-right
				if(i>0 && 
				rec_red(pixels,format,i-1,j+1,lon,angle,h,w))
				{
					pixels[(i*w)+j]=
						SDL_MapRGB(format, 200,0,0);
					return 1;
				}
				else 
				if (h>i+1 && 
				rec_red(pixels,format,i+1,j+1,lon,angle,h,w))
				{
					pixels[(i*w)+j]=
						SDL_MapRGB(format, 200,0,0);
					return 1;
				}
				return lon==0;
			}
			else
			{
				return lon==0;
			}
		}
		}
		else
		{
			return lon==0;
		}
	}
	if (angle==3)
	{
		if(w>j+1 && h>i+1)
		{
		if(rec_red(pixels,format,i+1,j+1,lon,angle,h,w))
		{
			pixels[(i*w)+j]=SDL_MapRGB(format, 200,0,0);
			return 1;
		}
		else
		{
			SDL_GetRGB(pixels[((i+1)*w)+j+1], format, &r, &g, &b);
			if(r<200)
			{
				// right and bot
				if(rec_red(pixels,format,i,j+1,lon,angle,h,w))
				{
					pixels[(i*w)+j]=
						SDL_MapRGB(format, 200,0,0);
					return 1;
				}
				else 
				if (rec_red(pixels,format,i+1,j,lon,angle,h,w))
				{
					pixels[(i*w)+j]=
						SDL_MapRGB(format, 200,0,0);
					return 1;
				}
				return lon==0;
			}
			return lon==0;
		}
		}
		else
		{
			return lon==0;
		}
	}	
	if (angle==4)
	{
		if(j>=1 && h>i+1)
		{
		if(rec_red(pixels,format,i+1,j-1,lon,angle,h,w))
		{
			pixels[(i*w)+j]=SDL_MapRGB(format, 200,0,0);
			return 1;
		}
		else
		{
			SDL_GetRGB(pixels[((i+1)*w)+j-1], format, &r, &g, &b);
			if(r<200)
			{
				// right and bot
				if(rec_red(pixels,format,i,j-1,lon,angle,h,w))
				{
					pixels[(i*w)+j]=
						SDL_MapRGB(format, 200,0,0);
					return 1;
				}
				else 
				if (rec_red(pixels,format,i+1,j,lon,angle,h,w))
				{
					pixels[(i*w)+j]=
						SDL_MapRGB(format, 200,0,0);
					return 1;
				}
				return lon==0;
			}
			return lon==0;
		}
		}
		else
		{
			return lon==0;
		}
	}
	return lon==0;
}

SDL_Surface* create_image(SDL_Surface* surface, size_t width1, 
        size_t height1, size_t width2, size_t height2)
{
    // pixels pointer of image
    Uint32* pixels = surface->pixels;

    // width of image
    size_t w = surface->w;

    // height of image
    size_t h = surface->h;

    // height of the cut image
    size_t height = height2 - height1;

    // width of the cut image
    size_t width = width2 - width1;

    SDL_Surface* image_surface= 
	    SDL_CreateRGBSurface(0,width,height,32,0,0,0,0);

    Uint32* pixelcopy = image_surface->pixels;
    SDL_PixelFormat* format = surface->format;
    SDL_LockSurface(surface);

    // position of the last pixel of cut image in image
    size_t end = w * height2 + width2;
    
    // check if in bound
    if (end < w * h)
    {
        // index for p
        size_t count = 0;

        // loop
        for(size_t i=0; i<height; i++)
        {
            for(size_t j=0; j<width; j++)
            {
                // pixel position in image
                size_t position = (height1 + i) * w + width1 + j;
                
                // add pixel we want to p
                pixelcopy[count] = copy_pixel(*(pixels+position),format);

                // add count
                count++;
            }
        }

        //return cut image
        return image_surface;
    }

    // if out of bound
    else
    {
        // return error
        errx(1,"incorrect value");
    }
}
//calcul angle for turn surface
double angle_to_turn(SDL_Surface* surface)
{
	Uint32* pixels = surface->pixels; // take list of pixel                     
	size_t W = surface->w ; //height and weight                                    
     	size_t H= surface->h;
	
	// take format of pixel
	SDL_PixelFormat* format = surface->format;
	SDL_LockSurface(surface);
	Uint8 r, g, b;
	
	size_t i=0;
	size_t j=0;
	char bouboule=1;
	/*size_t I=0;
	size_t J=0;
	for(size_t jj=10;jj<W-10;jj++)
	{	
		while(i<H && bouboule)
		{
			SDL_GetRGB(pixels[(i*W)+5], format, &r, &g, &b);
			bouboule=r!=200 &&g!=200;
			i++;
		}
		bouboule =1;
		I+=i;
		i=0;
	}
	for(size_t ii=10;ii<H-10;ii++)
	{
		while(j<W && bouboule)
		{
			SDL_GetRGB(pixels[(5*W)+j], format, &r, &g, &b);
			bouboule=r!=200&&g!=200;
			j++;
		}
		bouboule=1;
		J+=j;
		j=0;
	}
	double JJ=(double)J/(double)(H-20);
	double II=(double)I/(double)(W-20);
	if(JJ<=2||II<=2)
	{
		return 0;
	}
	double an=((double) II/ (double)JJ);
	return (atan(an)-0.785398)/9.98;*/

	// take distance top to first border in midle	
	while(i<H && bouboule)
	{
		SDL_GetRGB(pixels[(i*W)+(H/2)], format, &r, &g, &b);
		bouboule=r!=200 &&g!=200;
		i++;
	}
	bouboule =1;
	while(j<W && bouboule)
	{
		SDL_GetRGB(pixels[((W/2)*W)+j], format, &r, &g, &b);
		bouboule=r!=200&&g!=200;
		j++;
	}
	//check correct value
	if(i<=2|| j<=2)
	{
		return 0;
	}
	double an=((double) (i-1)/ (double)(j-1));
	return (atan(an));

}
//check if is line or not
char is_line(Uint32* pixels,size_t i,int black_tol,
		SDL_PixelFormat* format,size_t H_max,size_t H,size_t W)
{
	if(H_max==H/3)
	{
		return 1;
	}
	else
	{
		Uint8 r, g, b;
		SDL_GetRGB(pixels[(H_max*W)+i], format, &r, &g, &b);
		if(r+g+b)
		{
			return is_line(pixels,i,black_tol,format,H_max+1,H,W);
		}
		else
		{
			if(black_tol-1)
			{
				return is_line(pixels,i,black_tol-1,format,
						H_max+1,H,W);
			}
			else
			{
				return 0;
			}
		}
	}
}
//check if this is a sudoku
char verif_sud(SDL_Surface* surface)
{
	Uint32* pixels = surface->pixels; // take list of pixel                     
	size_t W = surface->w ; //height and weight                                    
     	size_t H= surface->h;
	
	// take format of pixel
	SDL_PixelFormat* format = surface->format;
	SDL_LockSurface(surface);
	
	int tol=10;
	int black_tol=5;
	char bouboule=0;
	for (size_t i=(W/3)-tol;i<(W/3)+tol;i++)
	{
		if(is_line(pixels,i,black_tol,format,0,H,W))
		{
			i=(W/3)+tol+10;
			bouboule=1;
		}
	}
	if(bouboule==0)
	{
		return 0;
	}
	bouboule=0;
	for(size_t i=(W*2/3)-tol;i<(W*2/3)+tol;i++)
	{
		if(is_line(pixels,i,black_tol,format,0,H,W))
		{
			i=(W*2/3)+tol+10;
			bouboule=1;
		}
	}
	if(bouboule==0)
		return 0;
	return 1;
}
//change pixel in black pixel and there friend
void redelet(Uint32* pixels,size_t W,size_t H,size_t i,size_t j
		,SDL_PixelFormat* format)
{
	if(j<W && i<H)
	{
		Uint8 r, g, b;
		SDL_GetRGB(pixels[(i*W)+j], format, &r, &g, &b);
		if(r+b+g==0)
		{
			return ;
		}
		pixels[(i*W)+j]=SDL_MapRGB(format, 0,0,0);
		if(j>0)
		{
			redelet(pixels,W,H,i,j-1,format);
			redelet(pixels,W,H,i+1,j-1,format);

		}

		redelet(pixels,W,H,i+1,j,format);
		
		redelet(pixels,W,H,i+1,j+1,format);
		
		redelet(pixels,W,H,i,j+1,format);

	}
}
//delet the form in i j 
void delet(SDL_Surface* surface,size_t i,size_t j)
{
	Uint32* pixels = surface->pixels;
	size_t W = surface->w ;
	size_t H= surface->h;
	SDL_PixelFormat* format = surface->format;
	SDL_LockSurface(surface);
	char bouboul =1;
	while(bouboul)
	{
		Uint8 r, g, b;
		SDL_GetRGB(pixels[(i*W)+j], format, &r, &g, &b);
		if(r+g+b!=0)
		{
			bouboul=0;
			redelet(pixels,W,H,i,j,format);
		}
		j++;
	}
}

size_t* get_sudoku(SDL_Surface* surface)
{
    surface_to_red(surface);
    // for the color
    int col=0;
    char bouboule=1;
    char p=0;
    size_t* nsewe=malloc(4*sizeof(size_t));
    while(bouboule && p<5)
    {
	// take the position of form
    	nsewe=rech_s(surface,col);
	// set col 1 or 0
	col=(col+1)%2;
	//creat a image in this position
    	SDL_Surface* surface2=create_image(surface, nsewe[3], nsewe[0], nsewe[2], nsewe[1]);
    	//surface_to_turn_an(surface2,angle_to_turn(surface));
	//ckeck if this is sudoku
    	if(verif_sud(surface2))
    	{
		// stop while
		bouboule=0;
		surface=surface2;
    	}
	else
	{
		Uint32* pixels = surface->pixels;
		size_t W = surface->w ;
		SDL_PixelFormat* format = surface->format;
		Uint8 r, g, b;                                                  
		SDL_GetRGB(pixels[W+1], format, &r, &g, &b);
		if(r+g+b!=0)
		{
			delet(surface,nsewe[0],nsewe[3]);
			//free(nsewe);
		}
		else
		{

			int stop=15;
			double val=0.1;
			while (verif_sud(surface2)==0 && stop!=0)
			{
				//turn this image
				surface2=mul_pixel(surface2,2);
				surface_to_turn_an(surface2,val);
				surface2=sub_pixel(surface2,2);
				nsewe=rech_s(surface2,col);
				col=(col+1)%2;
				stop--;
				if(stop==0)
				{
					stop=0;
					bouboule=0;
					surface=surface2;
				}
			}
			//turn this image
			//surface2=mul_pixel(surface2,2);
			//surface_to_turn_an(surface2,angle_to_turn(surface));
			//surface2=sub_pixel(surface2,2);

			if(verif_sud(surface2)==0)
			{
				//delet this form
				delet(surface,nsewe[0],nsewe[3]);
				//free(nsewe);
			}
			else
			{
				bouboule=0;
				surface=surface2;
			}
		}
	}
	//to stop while
	p++;
    }
    return nsewe;
}
