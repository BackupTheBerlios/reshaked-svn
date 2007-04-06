


#include "painter.h"

#include "defs.h"
#include <stdio.h>
namespace GUI {

struct PainterPrivate {

	
	enum {
		
		MAX_FONTS=32
	};
	
	struct Font {
		
		enum {
			HASHTABLE_BITS=8,
			HASHTABLE_SIZE=(1<<HASHTABLE_BITS),
			HASHTABLE_MASK=HASHTABLE_SIZE-1
		};
		
		struct Character {
			
			unsigned int valign;
			BitmapID bitmap;
			unsigned int unicode;
			Rect rect; //rect from the bitmap
			Character *next;
			Character() { next=0; unicode=0; bitmap=0; valign=0; }
		};
		
		Character *characters[HASHTABLE_SIZE];
		
		int height;
		int ascent;
		bool in_use;
		
		void cleanup() { 
			for (int i=0;i<HASHTABLE_SIZE;i++) {
				
				while(characters[i]) {
					Character *c=characters[i];
					characters[i]=c->next;
					delete c;
				}
			}
			height=0;
			ascent=0;
			in_use=false;
		}
		
		void add_char(unsigned int p_char,BitmapID p_bitmap,const Rect& p_rect,unsigned int p_valign=0) {
			
			
			if ( find_char( p_char) ) {
				PRINT_ERROR("Char already exists.");
				return;
			}
			
			
				
			int bucket=p_char&HASHTABLE_MASK;
			
			Character *nc = new Character;
			nc->bitmap=p_bitmap;
			nc->unicode=p_char;
			nc->rect=p_rect;
			nc->next=characters[bucket]; //should be added at the end for speed...
			nc->valign=p_valign;
			characters[bucket]=nc;			
		}
		
		Font() { in_use=false; ascent=0; height=0; for (int i=0;i<HASHTABLE_SIZE;i++) characters[i]=0; }
		
		
		inline Character * find_char(unsigned int p_unicode) {
			
						
			unsigned int bucket=p_unicode&HASHTABLE_MASK;
			
			if (bucket>HASHTABLE_SIZE) {
				
				printf("MUUH\n");
			}
			Character *c=characters[bucket];
			
			while (c) {
				
				if (c->unicode==p_unicode)
					return c;
				c=c->next;				
			}
			
			return 0; // should change this to some default
		}
	};
	
	Font fonts[MAX_FONTS];
};


int Painter::get_font_string_width(FontID p_font,const String& p_string) {
	
	if (!is_font_valid( p_font )) {
		
		PRINT_ERROR("Invalid Font");
		return 0;
	}	
	
	int width=0;
	
	for (int i=0;i<p_string.length();i++) {
		
		PainterPrivate::Font::Character * c = p->fonts[p_font].find_char(p_string[i]);
		
		if (!c)
			continue;
		
		width+=c->rect.size.width;
	}	

	return width;
}

void Painter::draw_text(FontID p_font,const Point & p_pos,const String &p_string,const Color&p_color,int p_clip_w) {
	
	if (!is_font_valid( p_font )) {
		
		PRINT_ERROR("Invalid Font");
		return ;
	}	
	Point pos=p_pos;
	
	for (int i=0;i<p_string.length();i++) {
		
		PainterPrivate::Font::Character * c = p->fonts[p_font].find_char(p_string[i]);
		
		if (!c)
			continue;

		if (p_clip_w>=0 && (pos.x+c->rect.size.width)>(p_pos.x+p_clip_w))
			break; //width exceeded
		
		Point cpos=pos;
		cpos.y-=p->fonts[p_font].ascent;
		cpos.y+=c->valign;
		draw_bitmap( c->bitmap, cpos, c->rect, p_color );
		pos.x+=c->rect.size.width;
		
	}
}

int Painter::get_font_char_width(FontID p_font,unsigned int p_char) {
	
	if (!is_font_valid(p_font))
		return -1;
	PainterPrivate::Font::Character * c = p->fonts[p_font].find_char(p_char);
	if (!c)
		return 0;
	return c->rect.size.width;
	
}
int Painter::get_font_height(FontID p_font) {
	
	if (!is_font_valid( p_font )) {
		
		PRINT_ERROR("Invalid Font");
		return -1;
	}
	
	return p->fonts[p_font].height;
	
}
int Painter::get_font_ascent(FontID p_font) {
	
	if (!is_font_valid( p_font )) {
		
		PRINT_ERROR("Invalid Font");
		return -1;
	}
	
	return p->fonts[p_font].ascent;
	
}
int Painter::get_font_descent(FontID p_font) {
	
	if (!is_font_valid( p_font )) {
		
		PRINT_ERROR("Invalid Font");
		return -1;
	}
	
	return p->fonts[p_font].height-p->fonts[p_font].ascent;
}

FontID Painter::create_font(int p_height,int p_ascent)  {
	
	for (int i=0;i<PainterPrivate::MAX_FONTS;i++) {
		
		if (p->fonts[i].in_use)
			continue;
		
		p->fonts[i].cleanup();
		p->fonts[i].in_use=true;
		p->fonts[i].height=p_height;
		p->fonts[i].ascent=p_ascent;
		
		return i;
	}
	
	return -1;
}

bool Painter::is_font_valid(FontID p_font) {
	
	return (p_font>=0 && p_font <PainterPrivate::MAX_FONTS && p->fonts[p_font].in_use );
}

void Painter::erase_font_and_bitmaps(FontID p_font) {

	if (!is_font_valid( p_font ))
		return;
	for (int i=0;i<PainterPrivate::Font::HASHTABLE_SIZE;i++) {
	
		PainterPrivate::Font::Character *c=p->fonts[p_font].characters[i];
		
		while (c) {
		
			if (is_bitmap_valid(c->bitmap))
				remove_bitmap(  c->bitmap );
			
			c=c->next;
		}
	}
	
	p->fonts[p_font].cleanup();

}

void Painter::erase_font(FontID p_font)  {
	
	if (!is_font_valid( p_font ))
		return;
	p->fonts[p_font].cleanup();
	
	
}
void Painter::font_add_char(FontID p_font,unsigned int p_char,BitmapID p_bitmap,const Rect& p_rect,unsigned int p_valign) {
	
	if (!is_font_valid( p_font )) {
		PRINT_ERROR("Invalid font");
		return;
	}
	if (!is_bitmap_valid( p_bitmap )) {
		PRINT_ERROR("Invalid bitmap");
		return;
	}
	
	p->fonts[p_font].add_char(p_char,p_bitmap,p_rect,p_valign);
//	printf("adding ucode: %i\n",p_char);	
}

void Painter::draw_fill_rect(const Point & p_from,const Size & p_size,const Color& p_color,const Rect& p_clip) {
	
	Rect final = p_clip.clip( Rect( p_from, p_size ) );
	if (final.size.width<=0 ||final.size.height<=0 )
		return;
	draw_fill_rect( final.pos, final.size, p_color );
}


void Painter::draw_tiled_bitmap(BitmapID p_bitmap,const Rect& p_rect,const Color&p_color) {
	
	draw_tiled_bitmap( p_bitmap,p_rect,p_rect.pos,p_color );
	
}


void Painter::draw_style_box(const StyleBox& p_stylebox,const Point& p_pos, const Size& p_size,bool p_draw_center) {
	
	draw_style_box( p_stylebox, p_pos, p_size, Rect( p_pos, p_size ), p_draw_center );
}
void Painter::draw_style_box(const StyleBox& p_stylebox,const Point& p_pos, const Size& p_size,const Rect &p_clip,bool p_draw_center) {
	
	if (!p_clip.intersects_with( Rect( p_pos, p_size ) ))
		    return;
	
	
	switch(p_stylebox.mode) {
		
		case StyleBox::MODE_FLAT: {
			
			Rect r( p_pos, p_size );
			
			for (int i=0;i<p_stylebox.flat.margin;i++) {
				

				
				Color color_upleft=p_stylebox.flat.border_upleft;
				Color color_downright=p_stylebox.flat.border_downright;

				if (p_stylebox.flat.blend) {

					color_upleft.r=(p_stylebox.flat.margin-i)*(int)color_upleft.r/p_stylebox.flat.margin + i*(int)p_stylebox.flat.center.r/p_stylebox.flat.margin;
					color_upleft.g=(p_stylebox.flat.margin-i)*(int)color_upleft.g/p_stylebox.flat.margin + i*(int)p_stylebox.flat.center.g/p_stylebox.flat.margin;
					color_upleft.b=(p_stylebox.flat.margin-i)*(int)color_upleft.b/p_stylebox.flat.margin + i*(int)p_stylebox.flat.center.b/p_stylebox.flat.margin;

					color_downright.r=(p_stylebox.flat.margin-i)*(int)color_downright.r/p_stylebox.flat.margin + i*(int)p_stylebox.flat.center.r/p_stylebox.flat.margin;
					color_downright.g=(p_stylebox.flat.margin-i)*(int)color_downright.g/p_stylebox.flat.margin + i*(int)p_stylebox.flat.center.g/p_stylebox.flat.margin;
					color_downright.b=(p_stylebox.flat.margin-i)*(int)color_downright.b/p_stylebox.flat.margin + i*(int)p_stylebox.flat.center.b/p_stylebox.flat.margin;
					
				}
				
				draw_fill_rect( Point( r.pos.x,r.pos.y+r.size.y-1), Size(r.size.x ,1 ), color_downright,p_clip);
				draw_fill_rect( Point( r.pos.x+r.size.x-1,r.pos.y ), Size( 1 ,r.size.y ), color_downright,p_clip);
				
				draw_fill_rect( r.pos, Size(r.size.x ,1 ), color_upleft,p_clip);
				draw_fill_rect( r.pos, Size( 1 ,r.size.y ), color_upleft,p_clip);
				
				
				r.pos.x++;
				r.pos.y++;
				r.size.x-=2;
				r.size.y-=2;
			}
			
			if (p_draw_center && p_stylebox.draw_center)
				draw_fill_rect( r.pos, r.size , p_stylebox.flat.center, p_clip );
			
		} break;
		case StyleBox::MODE_FLAT_BITMAP:
		case StyleBox::MODE_BITMAP: {
			
			//fixed
			if (is_bitmap_valid( p_stylebox.bitmaps[StyleBox::POS_TOPLEFT] )) {
			
				draw_bitmap( p_stylebox.bitmaps[StyleBox::POS_TOPLEFT] , p_pos );
			}
			
			if (is_bitmap_valid( p_stylebox.bitmaps[StyleBox::POS_TOPRIGHT] )) {
			
				Point pos=p_pos;
				pos.x+=p_size.x-get_bitmap_size( p_stylebox.bitmaps[StyleBox::POS_TOPRIGHT] ).width;
			
				draw_bitmap( p_stylebox.bitmaps[StyleBox::POS_TOPRIGHT] , pos );
			}
			if (is_bitmap_valid( p_stylebox.bitmaps[StyleBox::POS_BOTTOMLEFT] )) {
			
				Point pos=p_pos;
				pos.y+=p_size.y-get_bitmap_size( p_stylebox.bitmaps[StyleBox::POS_BOTTOMLEFT] ).height;
				draw_bitmap( p_stylebox.bitmaps[StyleBox::POS_BOTTOMLEFT] , pos );
			}
			if (is_bitmap_valid( p_stylebox.bitmaps[StyleBox::POS_BOTTOMRIGHT] )) {
			
				Point pos=p_pos+p_size-get_bitmap_size( p_stylebox.bitmaps[StyleBox::POS_BOTTOMRIGHT] );
				draw_bitmap( p_stylebox.bitmaps[StyleBox::POS_BOTTOMRIGHT] , pos );
			}
			
			//expandable
			if (p_draw_center && p_stylebox.draw_center && p_stylebox.mode==StyleBox::MODE_BITMAP && is_bitmap_valid( p_stylebox.bitmaps[StyleBox::POS_CENTER] )) {
			
				
				Point pos=p_pos+get_bitmap_size( p_stylebox.bitmaps[StyleBox::POS_TOPLEFT] );
				Size size=p_size;
				size-=get_bitmap_size( p_stylebox.bitmaps[StyleBox::POS_TOPLEFT] )+get_bitmap_size( p_stylebox.bitmaps[StyleBox::POS_BOTTOMRIGHT] );
				
				Rect dest=Rect(pos,size).clip(p_clip);
				draw_tiled_bitmap( p_stylebox.bitmaps[StyleBox::POS_CENTER] , dest,pos );
				
			}			
			
			if (p_draw_center && p_stylebox.draw_center && p_stylebox.mode==StyleBox::MODE_FLAT_BITMAP) {
			
				
				Point pos=p_pos+get_bitmap_size( p_stylebox.bitmaps[StyleBox::POS_TOPLEFT] );
				Size size=p_size;
				size-=get_bitmap_size( p_stylebox.bitmaps[StyleBox::POS_TOPLEFT] )+get_bitmap_size( p_stylebox.bitmaps[StyleBox::POS_BOTTOMRIGHT] );
				
				
				draw_fill_rect( pos, size, p_stylebox.flat.center, p_clip );
				
			}			
			
			if (is_bitmap_valid( p_stylebox.bitmaps[StyleBox::POS_LEFT] )) {
			
				
				Point pos=p_pos;
				pos.y+=get_bitmap_size( p_stylebox.bitmaps[StyleBox::POS_TOPLEFT] ).height;
				
				Size size=p_size;
				
				size.height-=get_bitmap_size( p_stylebox.bitmaps[StyleBox::POS_TOPLEFT] ).height+get_bitmap_size( p_stylebox.bitmaps[StyleBox::POS_BOTTOMRIGHT] ).height;
				size.width=get_bitmap_size( p_stylebox.bitmaps[StyleBox::POS_LEFT] ).width;
				
				Rect dest=Rect(pos,size).clip(p_clip);
				draw_tiled_bitmap( p_stylebox.bitmaps[StyleBox::POS_LEFT] ,dest,pos );
				
			
			}			
			
			if (is_bitmap_valid( p_stylebox.bitmaps[StyleBox::POS_TOP] )) {
			
				
				Point pos=p_pos;
				pos.x+=get_bitmap_size( p_stylebox.bitmaps[StyleBox::POS_TOPLEFT] ).width;
				
				Size size=p_size;
				
				size.width-=get_bitmap_size( p_stylebox.bitmaps[StyleBox::POS_TOPLEFT] ).width+get_bitmap_size( p_stylebox.bitmaps[StyleBox::POS_BOTTOMRIGHT] ).width;
				size.height=get_bitmap_size( p_stylebox.bitmaps[StyleBox::POS_TOP] ).height;
				
				Rect dest=Rect(pos,size).clip(p_clip);
				draw_tiled_bitmap( p_stylebox.bitmaps[StyleBox::POS_TOP] ,dest,pos );
				
			}						
			if (is_bitmap_valid( p_stylebox.bitmaps[StyleBox::POS_RIGHT] )) {
			
				
				Point pos=p_pos;
				pos.x+=p_size.x-get_bitmap_size( p_stylebox.bitmaps[StyleBox::POS_RIGHT] ).width;
				pos.y+=get_bitmap_size( p_stylebox.bitmaps[StyleBox::POS_TOPRIGHT] ).height;
				Size size=p_size;
				
				size.height-=get_bitmap_size( p_stylebox.bitmaps[StyleBox::POS_TOPRIGHT] ).height+get_bitmap_size( p_stylebox.bitmaps[StyleBox::POS_BOTTOMRIGHT] ).height;
				size.width=get_bitmap_size( p_stylebox.bitmaps[StyleBox::POS_RIGHT] ).width;
				
				Rect dest=Rect(pos,size).clip(p_clip);
				draw_tiled_bitmap( p_stylebox.bitmaps[StyleBox::POS_RIGHT] ,dest,pos );
				
			}						
			if (is_bitmap_valid( p_stylebox.bitmaps[StyleBox::POS_BOTTOM] )) {
			
				
				Point pos=p_pos;
				pos.y+=p_size.y-get_bitmap_size( p_stylebox.bitmaps[StyleBox::POS_BOTTOM] ).height;
				pos.x+=get_bitmap_size( p_stylebox.bitmaps[StyleBox::POS_BOTTOMLEFT] ).width;
				Size size=p_size;
				
				size.width-=get_bitmap_size( p_stylebox.bitmaps[StyleBox::POS_BOTTOMRIGHT] ).width+get_bitmap_size( p_stylebox.bitmaps[StyleBox::POS_BOTTOMLEFT] ).width;
				size.height=get_bitmap_size( p_stylebox.bitmaps[StyleBox::POS_BOTTOM] ).height;
				
				Rect dest=Rect(pos,size).clip(p_clip);
				draw_tiled_bitmap( p_stylebox.bitmaps[StyleBox::POS_BOTTOM] ,dest,pos );
				
			}						
		} break;
		
		default: {}
	}
}

int Painter::get_style_box_margin(const StyleBox& p_stylebox,const Margin& p_margin) {
	
	
	
	if (p_stylebox.margins[p_margin]>=0)
		return p_stylebox.margins[p_margin];
	else {
		
		int res=0;
		
		switch(p_stylebox.mode) {
			
			case StyleBox::MODE_FLAT: {
				
				res=p_stylebox.flat.margin;
			} break;
			case StyleBox::MODE_FLAT_BITMAP:
			case StyleBox::MODE_BITMAP: {
				
				switch(p_margin) {
					
					case MARGIN_TOP: res=get_bitmap_size( p_stylebox.bitmaps[ StyleBox::POS_TOP ] ).height; break;
					case MARGIN_BOTTOM: res=get_bitmap_size( p_stylebox.bitmaps[ StyleBox::POS_BOTTOM ] ).height; break;
					case MARGIN_LEFT: res=get_bitmap_size( p_stylebox.bitmaps[ StyleBox::POS_LEFT ] ).width; break;
					case MARGIN_RIGHT: res=get_bitmap_size( p_stylebox.bitmaps[ StyleBox::POS_RIGHT ] ).width; break;
					default: res=0; 
				};				
			} break;
			default: res=0;
	
		}
	
		if (res<0)
			res=0;
		
		return res;
	}

}

Size Painter::get_style_box_min_size(const StyleBox& p_stylebox,bool p_with_center) {
	
	Size min=Size( 	get_style_box_margin( p_stylebox, MARGIN_LEFT) + 
			get_style_box_margin( p_stylebox, MARGIN_RIGHT), 
			get_style_box_margin( p_stylebox, MARGIN_TOP) + 
			get_style_box_margin( p_stylebox, MARGIN_BOTTOM ) );
	
	if (p_stylebox.mode==StyleBox::MODE_BITMAP && p_with_center) {
		
		min+=get_bitmap_size( p_stylebox.bitmaps[ StyleBox::POS_CENTER ] );
	}
		     
		     
	return min;
}

void Painter::draw_arrow( const Point& p_pos, const Size& p_size, Direction p_dir, const Color& p_color,bool p_trianglify) {
	
	if (p_size.width<=0 || p_size.height<=0)
		return;
	
	Point pos=p_pos;
	Size size=p_size;
	
	if (p_trianglify) {
		
		if (p_dir==UP || p_dir==DOWN ) {
			
			int desired_height=(p_size.width+1)/2;
			
			
			if (desired_height < p_size.height ) {
				//good, let's do it
				
				pos.y+=(p_size.height-desired_height)/2;
				size.height=desired_height;
				
			}
		}
		
		if (p_dir==LEFT || p_dir==RIGHT ) {
			
			int desired_width=(p_size.height+1)/2;
			
			
			if (desired_width < p_size.width ) {
				//good, let's do it
				
				pos.x+=(p_size.width-desired_width)/2;
				size.width=desired_width;
				
			}
		}
	}
	size.height &= ~1;
	size.width &= ~1;
	for (int i=0;i<size.height;i++) {
		
		int begin,end;
		switch (p_dir) {
					
			case UP:
			case DOWN: {
				
				int idx=i;
				
				if (p_dir==UP) {
					
					idx=size.height-i-1;
				}
				
				int ofsm=idx*(size.width/2)/size.height;
				begin=ofsm;
				end=size.width-ofsm-1;
				
				
			} break;
			case RIGHT:
			case LEFT: {
				
				int ofs=(i%(size.height/2))*(size.width)/(size.height/2);
				end=size.width-1;
				begin=(i>=(size.height/2))?ofs:(size.width-ofs);
				
				if (p_dir==RIGHT) {
					
					int auxbeg=size.width-end;
					end=size.width-begin;
					begin=auxbeg;
				}
				
			} break;
			
		};
		
		Point from( pos.x + begin , pos.y + i );
		Size rsize( end-begin , 1 );
		draw_fill_rect( from, rsize, p_color );
	}
}

Painter::Painter() {
	
	p = new PainterPrivate;
}
Painter::~Painter() {
	
	delete p;
}

};