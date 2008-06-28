//
// C++ Implementation: pixmap_data
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "pixmap_data.h"
#include <png.h>
#include "error_macros.h"
namespace ReShaked {

Painter *PixmapData::painter=NULL;

struct PNGReadStatus {

	int offset;
	const unsigned char *image;
};

void user_read_data(png_structp png_ptr,png_bytep data, unsigned int p_length) {

	PNGReadStatus *rstatus;
	rstatus=(PNGReadStatus*)png_ptr->io_ptr;

	memcpy(data,&rstatus->image[rstatus->offset],p_length);
	rstatus->offset+=p_length;
}

BitmapID PixmapData::create_bitmap_from_buffer(const unsigned char *p_png_buffer) {

	png_structp png = NULL;
	png_infop info = NULL;
	png_uint_32 width, height;
	int pixel_depth, depth, color, size;
	PNGReadStatus rstatus;
	png_bytep data;
	png_bytep *row_p;


	struct Component {
		unsigned char r,g,b,a;} pixel_c;

	png = png_create_read_struct(PNG_LIBPNG_VER_STRING, (png_voidp)NULL, NULL, NULL);
	ERR_FAIL_COND_V(!png,-1);

	info = png_create_info_struct(png);
	
	if (!info) {
		
		png_destroy_read_struct(&png,NULL,NULL);
		ERR_FAIL_COND_V(!info,-1);
		
	}

	rstatus.image = p_png_buffer;
	rstatus.offset = 0;
	png_set_read_fn(png,(void*)&rstatus,&user_read_data); // Set user read function

	// Read png header into struct
	png_read_info(png, info);
	png_get_IHDR(png, info, &width, &height, &depth, &color, NULL, NULL, NULL);

	data = (png_bytep) malloc(png_get_rowbytes(png, info)*height);	
	row_p = (png_bytep *) malloc(sizeof(png_bytep)*height);

	for (int i = 0; i < (signed)height; i++) {
		row_p[i] = &data[png_get_rowbytes(png, info) * i];
	}

	png_read_image(png, row_p);

	BitmapID bmp=painter->create_bitmap(Size(width,height),MODE_PIXMAP,true);
	
	switch(color) {

		case 0 : { // Each pixel is a grayscale sample.
			switch(depth) {
				case  1 : break;
				case  2 : break;
				case  4 : break;
				case  8 : break;
				case 16 : break;
				default : break;
			}
			
			printf("depth is %i\n",depth);
			ERR_PRINT("Grayscale PNG not supproted!");
			return -1;
		break;
		}

		case 2:  // Each pixel is a R,G,B triple
			switch(depth) {
				case 16 : // 48 bits (RGB)
				break;
					
				default:  // 24 bits (RGB) case 8:
					for (int x=0;x<width;x++) {
						for (int y=0;y<height;y++) {
							
							unsigned char * pixel=&data[png_get_rowbytes(png, info)*y+x*3];
							painter->set_bitmap_pixel(bmp,Point(x,y),Color(pixel[0],pixel[1],pixel[2]));
						}
					}
				break;
			}
		break;
		
	
		case 6:  // Each pixel is an R,G,B,A.
			switch(depth) {
				case 16 : // 64 bits (RGBA)
				break;
					
				default: // 32 bits (RGBA) case 8:
					
					for (int x=0;x<width;x++) {
						for (int y=0;y<height;y++) {
							
							unsigned char * pixel=&data[png_get_rowbytes(png, info)*y+x*4];
							painter->set_bitmap_pixel(bmp,Point(x,y),Color(pixel[0],pixel[1],pixel[2]),pixel[3]);
						}
					}
					
				break;
			}
		break;
		
		default:
			
			for (int x=0;x<width;x++) {
				for (int y=0;y<height;y++) {
					
					unsigned char * pixel=&data[png_get_rowbytes(png, info)*y+x*4];
					painter->set_bitmap_pixel(bmp,Point(x,y),Color(pixel[0],pixel[1],pixel[2]),pixel[3]);
				}
			}
		break;
	}
		
	//************* TRANSFORMATION **************//
		
	free(row_p);
	
	png_destroy_read_struct(&png, &info, NULL);
	return bmp;
}

BitmapID PixmapData::pixmap_id_list[PIXMAP_MAX];

BitmapID PixmapData::get_pixmap(PixmapDataList p_pixmap) {
	
	ERR_FAIL_INDEX_V(p_pixmap,PIXMAP_MAX,-1);
	
	return pixmap_id_list[p_pixmap];
	
}


void PixmapData::set_painter(Painter *p_painter) {
	
	painter=p_painter;
	for (int i=0;i<PIXMAP_MAX;i++) {
			
		pixmap_id_list[i]=PixmapData::create_bitmap_from_buffer(pixmap_list[i]);
	}
	

}
	

}
