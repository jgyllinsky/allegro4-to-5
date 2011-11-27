#include <stdio.h>
#include <stdlib.h>
#include "allegro.h"

/* FIXME */
int AL_RAND(){
    return rand();
}

#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

int * allegro_errno;
int allegro_error;
volatile char key[KEY_MAX];
BITMAP * screen;
struct FONT * font;
int * palette_color;
ALLEGRO_DISPLAY * display;
int mouse_x;
int mouse_y;
GFX_DRIVER * gfx_driver;

PALETTE current_palette;

RGB_MAP * rgb_map;
COLOR_MAP * color_map;

int palette_color8[256];

/*
struct BITMAP{
    ALLEGRO_BITMAP * real;
    int w;
    int h;
};
*/

/* allegro4 uses 0 as ok values */
static int is_ok(int code){
    if (code){
        return 0;
    }
    return -1;
}

static ALLEGRO_COLOR a4color(int color){
    RGB * rgb = &current_palette[color];
    return al_map_rgb(rgb->r * 4, rgb->g * 4, rgb->b * 4);
}

void poll_mouse(){
}

int keypressed(){
    return 0;
}

void vsync(){
}

void show_mouse(){
}

void set_trans_blender(int r, int g, int b, int a){
}

void create_rgb_table(RGB_MAP *table, AL_CONST PALETTE pal, AL_METHOD(void, callback, (int pos))){
}

void create_light_table(COLOR_MAP *table, AL_CONST PALETTE pal, int r, int g, int b, AL_METHOD(void, callback, (int pos))){
}

static BITMAP * create_bitmap_from(ALLEGRO_BITMAP * real){
    BITMAP * bitmap = malloc(sizeof(BITMAP));
    bitmap->real = real;
    bitmap->w = al_get_bitmap_width(real);
    bitmap->h = al_get_bitmap_height(real);
    return bitmap;
}

BITMAP * load_bitmap(const char * path,struct RGB *pal){
    return create_bitmap_from(al_load_bitmap(path));
}

void destroy_bitmap(BITMAP* bitmap){
    al_destroy_bitmap((ALLEGRO_BITMAP*) bitmap);
}

int poll_keyboard(){
}

static void setup_default_driver(BITMAP * screen){
    gfx_driver = al_malloc(sizeof(GFX_DRIVER));
    memset(gfx_driver, 0, sizeof(GFX_DRIVER));
    gfx_driver->w = al_get_bitmap_width(screen->real);
    gfx_driver->h = al_get_bitmap_height(screen->real);
}

int set_gfx_mode(int card, int width, int height, int virtualwidth, int virtualheight){
    int i;
    display = al_create_display(width, height);
    screen = create_bitmap_from(al_get_backbuffer(display));
    palette_color = palette_color8;
    setup_default_driver(screen);
    for (i = 0; i < 256; i++){
        palette_color8[i] = i;
    }
    return is_ok(display != NULL);
}

/*
void set_projection_viewport(){
}
*/

void clear_bitmap(BITMAP * bitmap){
    al_set_target_bitmap(bitmap->real);
    al_clear_to_color(al_map_rgb(0, 0, 0));
}

void allegro_message(){
}

BITMAP * create_bitmap(int width, int height){
    // return (BITMAP*) al_create_bitmap(width, height);
    BITMAP * bitmap = malloc(sizeof(BITMAP));
    bitmap->w = width;
    bitmap->h = height;
    bitmap->real = al_create_bitmap(width, height);
    return bitmap;
}

void install_timer(){
}

void install_mouse(){
}

int install_keyboard(){
}

static int a4key(int a5key){
    switch (a5key){
        case ALLEGRO_KEY_LCTRL: return KEY_LCONTROL;
        case ALLEGRO_KEY_RCTRL: return KEY_RCONTROL;
        default: return a5key;
    }
}

static void * read_keys(ALLEGRO_THREAD * self, void * arg){
    ALLEGRO_EVENT_QUEUE * queue = al_create_event_queue();
    al_register_event_source(queue, al_get_keyboard_event_source());
    while (true){
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);

        if (event.type == ALLEGRO_EVENT_KEY_DOWN){
            key[a4key(event.keyboard.keycode)] = 1;
        } else if (event.type == ALLEGRO_EVENT_KEY_UP){
            key[a4key(event.keyboard.keycode)] = 0;
        }
    }
}

static void start_key_thread(){
    ALLEGRO_THREAD * thread = al_create_thread(read_keys, NULL);
    if (thread != NULL){
        al_start_thread(thread);
    } else {
        printf("Could not start key thread!\n");
    }
}

int allegro_init(){
    allegro_errno = &allegro_error;
    int ok = al_init();
    al_init_primitives_addon();
    al_init_image_addon();
    al_install_keyboard();
    al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
    start_key_thread();
    return is_ok(ok);
}

void rectfill(BITMAP * buffer, int x1, int y1, int x2, int y2, int color){
    al_set_target_bitmap(buffer->real);
    al_draw_filled_rectangle(x1, y1, x2, y2, a4color(color));
}

void triangle(BITMAP * buffer, int x1, int y1, int x2, int y2, int x3, int y3, int color){
    al_set_target_bitmap(buffer->real);
    al_draw_filled_triangle(x1, y1, x2, y2, x3, y3, a4color(color));
}

void putpixel(BITMAP * buffer, int x, int y, int color){
    ALLEGRO_BITMAP * al_buffer = buffer->real;
    al_set_target_bitmap(al_buffer);
    al_put_pixel(x, y, a4color(color));
}

void set_palette(const PALETTE palette){
    memcpy(current_palette, palette, sizeof(PALETTE));
}

void blit(BITMAP * from, BITMAP * to, int to_x, int to_y, int from_x, int from_y, int width, int height){
    ALLEGRO_BITMAP * al_from = from->real;
    ALLEGRO_BITMAP * al_to = to->real;
    al_set_target_bitmap(al_to);
    al_draw_bitmap(al_from, to_x, to_y, 0);
    if (to == screen){
        al_flip_display();
    }
}

void textprintf_ex(struct BITMAP *bmp, AL_CONST struct FONT *f, int x, int y, int color, int bg, AL_CONST char *format, ...){
}

void textout_centre_ex(struct BITMAP *bmp, AL_CONST struct FONT *f, AL_CONST char *str, int x, int y, int color, int bg){
    // printf("%s\n", str);
}

void textprintf_centre_ex(struct BITMAP *bmp, AL_CONST struct FONT *f, int x, int y, int color, int bg, AL_CONST char *format, ...){
}

void textout_ex(struct BITMAP *bmp, AL_CONST struct FONT *f, AL_CONST char *str, int x, int y, int color, int bg){
}

void draw_gouraud_sprite(struct BITMAP *bmp, struct BITMAP *sprite, int x, int y, int c1, int c2, int c3, int c4){
    al_set_target_bitmap(bmp->real);
    al_draw_bitmap(sprite->real, x, y, 0);
}

fixed fixmul(fixed x, fixed y){
    return ftofix(fixtof(x) * fixtof(y));
}

fixed itofix(int x){
   return x << 16;
}

int fixtoi(fixed x){
   return fixfloor(x) + ((x & 0x8000) >> 15);
}

fixed ftofix(double x){
   if (x > 32767.0) {
      *allegro_errno = ERANGE;
      return 0x7FFFFFFF;
   }

   if (x < -32767.0) {
      *allegro_errno = ERANGE;
      return -0x7FFFFFFF;
   }

   return (fixed)(x * 65536.0 + (x < 0 ? -0.5 : 0.5));
}

double fixtof(fixed x){
   return (double)x / 65536.0;
}

int fixfloor(fixed x){
   /* (x >> 16) is not portable */
   if (x >= 0)
      return (x >> 16);
   else
      return ~((~x) >> 16);
}

fixed fixcos(fixed x){
   return _cos_tbl[((x + 0x4000) >> 15) & 0x1FF];
}

fixed fixsin(fixed x){
   return _cos_tbl[((x - 0x400000 + 0x4000) >> 15) & 0x1FF];
}

fixed fixdiv(fixed x, fixed y){
   if (y == 0) {
      *allegro_errno = ERANGE;
      return (x < 0) ? -0x7FFFFFFF : 0x7FFFFFFF;
   }
   else
      return ftofix(fixtof(x) / fixtof(y));
}

#define CALC_ROW(n)     (fixmul(x, m->v[n][0]) +      \
                         fixmul(y, m->v[n][1]) +      \
                         fixmul(z, m->v[n][2]) +      \
                         m->t[n])

void apply_matrix(MATRIX *m, fixed x, fixed y, fixed z, fixed *xout, fixed *yout, fixed *zout){
   *xout = CALC_ROW(0);
   *yout = CALC_ROW(1);
   *zout = CALC_ROW(2);
}

#undef CALC_ROW

fixed dot_product(fixed x1, fixed y_1, fixed z1, fixed x2, fixed y2, fixed z2){
   return fixmul(x1, x2) + fixmul(y_1, y2) + fixmul(z1, z2);
}

float dot_product_f(float x1, float y_1, float z1, float x2, float y2, float z2){
   return (x1 * x2) + (y_1 * y2) + (z1 * z2);
}

void persp_project(fixed x, fixed y, fixed z, fixed *xout, fixed *yout){
   *xout = fixmul(fixdiv(x, z), _persp_xscale) + _persp_xoffset;
   *yout = fixmul(fixdiv(y, z), _persp_yscale) + _persp_yoffset;
}

void persp_project_f(float x, float y, float z, float *xout, float *yout){
   float z1 = 1.0f / z;
   *xout = ((x * z1) * _persp_xscale_f) + _persp_xoffset_f;
   *yout = ((y * z1) * _persp_yscale_f) + _persp_yoffset_f;
}

