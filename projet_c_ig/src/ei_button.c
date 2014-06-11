
#include "ei_button.h"
#include "ei_draw.h"
#include "ei_common.h"
#include "ei_shape.h"
#include "ei_utilities.h"

/* Frame draw */
void ei_frame_draw(ei_surface_t window, ei_rect_t rectangle, ei_frame_t * frame,
		ei_rect_t *clipper)
{
	//printf("debut dessin frame \n");
	ei_button_draw_loc(window, rectangle, frame->bg_color, frame->relief, 0, frame->border_width, clipper);
	
	ei_rect_t rectangle_red = reduction(rectangle, frame->border_width);
	//printf("fin dessin, debut texte\n");
	if (frame->text && frame->text_font && frame->text_anchor) {
		//printf("frame->text non null\n");
		ei_insert_text(window, rectangle_red, frame->text,
				frame->text_font, frame->text_color,
				frame->text_anchor,clipper);
	}
	else {
		if (frame->img) {
			if (frame->img_rect) aff_img(window,rectangle_red,frame->img,frame->img_rect,frame->img_anchor);
			//else printf("frame->img_rect=NULL\n");
		}
		//else printf("frame->img=NULL\n");
	}
}

void ei_button_draw(ei_surface_t window, ei_rect_t rectangle,
		ei_button_t * button, ei_rect_t *clipper)
{
	//printf("debut dessin \n");
	ei_button_draw_loc(window, rectangle, *button->color, button->relief,
			button->corner_radius, button->border_width, clipper);

	int marge = button->border_width + 2;   //2 pixels en plus pour la visibilité
	//peut-être mettre en fonction du rayon aussi pour que cela soit plus joli dans certain cas
	ei_rect_t rectangle_reduit=reduction(rectangle,marge);

	if (button->text) {
		ei_insert_text(window, rectangle_reduit, button->text,
				button->text_font, button->text_color,
				button->text_anchor,clipper);
	}
	else {
		if (button->img) {
			if (button->img_rect)
				aff_img(window, rectangle_reduit, button->img,
						button->img_rect, button->img_anchor);
			//else printf("button->img_rect=NULL\n");
		}
		//else printf("button->img=NULL\n");
	}
	//printf("fin dessin bouton\n");
}

void ei_toplevel_draw(ei_surface_t surface, ei_toplevel_t *toplevel, ei_rect_t *clipper) {
//présuposés: screenlocation.size={2*bw+content_rect largeur, bar + 2*bw +content_rect hauteur}
	ei_rect_t rec=toplevel->widget.screen_location;
	ei_linked_point_t lp;
//Dessin de la bordure
	ei_rect_t bord=rec;
	bord.top_left=plus(rec.top_left,0,toplevel->bar_height);
	bord.size.height=bord.size.height-toplevel->bar_height;
	lp=ei_rect_to_points(bord);

	ei_color_t bord_color=obscurcir(toplevel->color,0.3);
	ei_draw_polygon(surface,&lp,bord_color,clipper);
//Dessin du content_rect
   lp =ei_rect_to_points(*toplevel->widget.content_rect);
   ei_draw_polygon(surface, &lp, toplevel->color, clipper);

//Dessin de la barre	
	//printf("début ei_bar_draw\n");
	ei_rect_t bar=rec;
	bar.size.height=toplevel->bar_height;
	lp=ei_rect_to_points(bar);
	ei_draw_polygon(surface,&lp,toplevel->bar_color,clipper);
	int marge;
	ei_rect_t btn_c;
	if (toplevel->closable) {
	//Dessin du bouton close
		ei_color_t btn_c_color={0,0,0,255};
		marge=toplevel->bar_height*0.25;
		btn_c.top_left=plus(rec.top_left,marge,marge);
		btn_c.size.width=toplevel->bar_height-2*marge;
		btn_c.size.height=toplevel->bar_height-2*marge;
		marge=0.2*btn_c.size.height;
		ei_button_draw_loc(surface,btn_c,btn_c_color,toplevel->rel_btn_close,0,marge,clipper);
	} else {
		marge=0;
		btn_c.size.width=0;
		btn_c.size.height=0;
	}
	//printf("bouton déssiné\n");
	if (toplevel->resizable) {
	//Dessin du bouton resize
		ei_color_t btn_r_color=eclaircir(toplevel->color,0.2);
		ei_rect_t btn_r;
		ei_size_t btn_r_s={toplevel->resize_size,toplevel->resize_size};
		btn_r.size=btn_r_s;
		//int width=rec.top_left.x+toplevel->bar_height+2*toplevel->border_width;
		//int height=rec.top_left.y+toplevel->bar_height+2*toplevel->border_width;
		int width=rec.size.width;
		int height=rec.size.height;
		btn_r.top_left=plus(rec.top_left,width-toplevel->resize_size-toplevel->border_width,height-toplevel->resize_size-toplevel->border_width);
		ei_button_draw_loc(surface,btn_r,btn_r_color,ei_relief_none,0,0,clipper);
	}
	if (toplevel->title) {
	//Affichage du titre
		ei_rect_t rec_txt;
		rec_txt.top_left=plus(rec.top_left,2*marge+btn_c.size.width,0);
		rec_txt.size.height=toplevel->bar_height;
		rec_txt.size.width=toplevel->widget.screen_location.size.width-2*(2*marge+btn_c.size.width);
		ei_insert_text(surface,rec_txt,toplevel->title,toplevel->title_font,toplevel->title_color,0,clipper);
	}
}

void ei_button_draw_loc(ei_surface_t window, ei_rect_t rectangle,
		ei_color_t couleur, ei_relief_t relief, int rayon,
		int marge, ei_rect_t *clipper)
{
	ei_linked_point_t *Liste = NULL;

	if (relief == ei_relief_none) {
		Liste = ei_button_rounded_frame(rectangle, rayon, 0);
		ei_draw_polygon(window, Liste, couleur, clipper);
		free_lp(Liste);
	} else {
		ei_color_t couleur_eclairee, couleur_assombrie, couleur_haute,
					  couleur_basse;
		float coeff_couleur = 0.2;
		couleur_eclairee=eclaircir(couleur,coeff_couleur);
		couleur_assombrie=obscurcir(couleur,coeff_couleur);

		if (relief == ei_relief_raised) {
			couleur_haute = couleur_eclairee;
			couleur_basse = couleur_assombrie;
		} else {
			couleur_haute = couleur_assombrie;
			couleur_basse = couleur_eclairee;
		}

		ei_linked_point_t *partie_haute =
			ei_button_rounded_frame(rectangle, rayon, 1);
		ei_draw_polygon(window, partie_haute, couleur_haute, clipper);
		free_lp(partie_haute);

		ei_linked_point_t *partie_basse = ei_button_rounded_frame(rectangle, rayon, -1);
		ei_draw_polygon(window, partie_basse, couleur_basse, clipper);
		free_lp(partie_basse);

		ei_rect_t rectangle_interieur=reduction(rectangle,marge);
		if (rayon!=0) Liste=ei_button_rounded_frame(rectangle_interieur, rayon -marge, 0);
		else Liste=ei_button_rounded_frame(rectangle_interieur, 0, 0);
		ei_draw_polygon(window, Liste, couleur, clipper);
		free_lp(Liste);

	}
}

void ei_insert_text(ei_surface_t window, ei_rect_t rectangle, char *text,
		ei_font_t font, ei_color_t color, ei_anchor_t anchor, ei_rect_t *clipper)
{
	//printf("j'affiche le texte \n");
	int width,height;
	hw_text_compute_size(text, font, &width, &height);
	ei_point_t ancre;
	ancre=find_anchor(rectangle,width,height,anchor);
	ei_draw_text(window, &ancre, text, font, &color, clipper);
}

void aff_img(ei_surface_t window, ei_rect_t rectangle, ei_surface_t img,
		ei_rect_t * img_rect, ei_anchor_t img_anchor)
{
	int result;

	//printf("jaffiche l'image");
	//assert(img_rect);
	ei_rect_t img_rec1=hw_surface_get_rect(img);
	
	//printf("img size {%i,%i}\n",test.size.width,test.size.height);
	ei_rect_t nv_img_rect;
	if (img_rect) {
		//printf("img_rect size{%i,%i}\n",img_rect->size.width,img_rect->size.height);
		nv_img_rect=*img_rect;
		if (nv_img_rect.top_left.x>img_rec1.size.width||nv_img_rect.top_left.y>img_rec1.size.height) {
			printf("Attention, the left_corner of your img_rec selection is out of the image, you favorite library relocate it to (0,0)\n");
		  nv_img_rect.top_left=ei_point(0,0);	
		}
		if (nv_img_rect.top_left.x+nv_img_rect.size.width>img_rec1.size.width) nv_img_rect.size.width=img_rec1.size.width-nv_img_rect.top_left.x;
		if (nv_img_rect.top_left.y+nv_img_rect.size.height>img_rec1.size.height) nv_img_rect.size.height=img_rec1.size.height-nv_img_rect.top_left.y;
	} else {
		nv_img_rect=img_rec1;
	}

	int w_dst=rectangle.size.width;
	int h_dst=rectangle.size.height;
	int w_ori=nv_img_rect.size.width;
	int h_ori=nv_img_rect.size.height;
	int cstraint_w=MIN(w_dst,w_ori);
	int cstraint_h=MIN(h_dst,h_ori);
	ei_size_t s={cstraint_w,cstraint_h};

	ei_rect_t img_part;
	if (w_ori>w_dst||h_ori>h_dst) {
		ei_point_t ancre;
		ancre=find_anchor(nv_img_rect,cstraint_w,cstraint_h,img_anchor);
		ei_size_t s={cstraint_w,cstraint_h};
		img_part.top_left=ancre;
		img_part.size=s;
	} else {
		img_part=nv_img_rect;
	}

	ei_rect_t rec_dst;
	if	(s.width==rectangle.size.width&&s.height==rectangle.size.height) {
		rec_dst=rectangle;
	} else {
		ei_point_t ancre_dst;
		ancre_dst=find_anchor(rectangle,cstraint_w,cstraint_h,img_anchor);
	 	rec_dst.top_left=ancre_dst;
		rec_dst.size=s;
	}	  
	
	//printf("img_part size{%i,%i}\n",img_part.size.width,img_part.size.height);
	//printf("rectangle size{%i,%i}\n",rectangle.size.width,rectangle.size.height);
	hw_surface_lock(img);
	result = ei_copy_surface(window, &rec_dst, img, &img_part, 1);

	hw_surface_unlock(img);

	assert(!result);
	//printf("result %x\n", result);
}

