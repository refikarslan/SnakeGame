#include<allegro.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<conio2.h>

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define BLACK makecol(0,0,0)
#define WHITE makecol(255,255,255)

#define TIMES_NEW (FONT *)times[0].dat
#define Until_you_die while(1)

short int x, y, x_food, y_food, j=0, p=0, tam=1, tam_2=0, snake_dir, points=-10, snake[1000][2]={0};
short int mode, size, aspect;
char txt[50];


BITMAP *gameover = NULL;
BITMAP *head_up = NULL; 
BITMAP *head_down = NULL;
BITMAP *head_left = NULL;
BITMAP *head_right = NULL;
BITMAP *body = NULL;
BITMAP *food = NULL;
BITMAP *borders = NULL;
BITMAP *user_screen = NULL;

void place_food()  //yemek fonksiyonu
{
    short int i;

    points += 10;

    if(j == 1) tam++;
    else j = 1;

    srand(time(NULL));

    for(;;)  //iki tane dizi oldugu icin (;;) oldu
    {
        x_food = (rand() % SCREEN_W-20) + 20;
        y_food = (rand() % SCREEN_H-20) + 20;

        if(x_food<20 || y_food<20 || x_food>SCREEN_W-40 || y_food>SCREEN_H-40) continue;

        for(i=0; i<=tam; i++)
            if(x == snake[i][0] && y == snake[i][1]) continue;

        if(x_food % 20 == 0 && y_food % 20 == 0 && x!=x_food && y!=y_food)
        {
            draw_sprite(user_screen, food, x_food, y_food);
            break;
        }
    }
}

void replace_food() //yemek degistirme fonksiyonu
{
    draw_sprite(user_screen, food, x_food, y_food);
}

void place_borders()   // yer sinirlama fonksiyonu
{
    register short int alt, comp;

    for(alt=9; alt<SCREEN_H-19; alt+=10)
    {
        blit(borders, user_screen, 0, 0, 9, alt, 10, 10);
        blit(borders, user_screen, 0, 0, SCREEN_W-19, alt, 10, 10);
    }

    for(comp=9; comp<SCREEN_W-19; comp+=10)
    {
        blit(borders, user_screen, 0, 0, comp, 9, 10, 10);
        blit(borders, user_screen, 0, 0, comp, SCREEN_H-19, 10, 10);
    }
}

void verification_body() // vucut dogrulama fonksiyonu
{
    short int i;

    for(i=0; i<tam-1; i++)
        if(x == snake[i][0] && y == snake[i][1]) x = 0;
}

void clean()
{
    short int i, cauda_x, cauda_y;

    cauda_x = snake[0][0];
    cauda_y = snake[0][1];
    rectfill(user_screen, cauda_x, cauda_y, cauda_x+20, cauda_y+20, BLACK); 

    place_borders();

    for(i=0; i<tam; i++)
    {
        snake[i][0] = snake[i+1][0];
        snake[i][1] = snake[i+1][1];
    }
    tam_2--;
}

void look_body()
{
    snake[tam_2+1][0] = x;
    snake[tam_2+1][1] = y;
    tam_2++;
}

void allegro()
{
    allegro_init();
    install_keyboard();
    install_timer();
}

void windowed() //Pencere ayarlamalari yaptik
{
    int weight, height;
    set_color_depth(32);
    if(size == 1)
    {
        weight = 800;
        height = 600;
    }

    else if (size == 2)
    {
        weight = 640;
        height = 480;
    }
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, weight, height, 0, 0); //GFX_AUTODETECT_WINDOWED pencere ekrani icin kullanilir
    set_window_title("Snake Game"); //pencere basligi ayarlandi
}

void fullscreen() //Tam ekran ayarlamalari yaptik
{
    int weight, height;
    if(size == 1)
    {
        weight = 800;
        height = 600;
    }

    else if(size == 2)
    {
        weight = 640;
        height = 480;
    }

    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, weight, height, 0, 0); //GFX_AUTODETECT_FULLSCREEN tam ekran icin kullanilir
    set_window_title("Snake Game");
}

int main() //ana fonksiyon
{
    double dif;
    short int user_key, aux_key, length, delay, velocity, alt;
    time_t start, end;
    DATAFILE *times = NULL;

    textcolor(12);
    gotoxy(32,2);
    printf("SNAKE GAME\n\n");
    gotoxy(10,4);
    textcolor(10);
    printf("During the game press ESC to quit or any key to pause");
    gotoxy(9,6);
    printf("If you have a widescreen display use the windowed mode");
    gotoxy(0,17);
    textcolor(14);

    while(mode != 1 && mode != 2)
    {
        printf("\n\nWindowed or fullscreen?\n\n");
        printf("1. Windowed\n");
        printf("2. Fullscreen\n\n> ");
        scanf("%d", &mode);
    }

    while(size != 1 && size !=2)
    {
        printf("\nWhich size?\n\n");
        printf("1. 800X600 (normal)\n");
        printf("2. 640X480 (small)\n\n> ");
        scanf("%d", &size);
    }


    while(velocidade < 1 || velocidade > 5)
    {
        printf("\nChoose a speed (1-5): ");
        scanf("%d", &velocidade);
    }

    allegro();

    if(mode == 1) windowed();
    else if(mode == 2) fullscreen();

    switch (velocidade)
    {
        case 1:
            delay = 220;
            break;
        case 2:
            delay = 140;
            break;
        case 3:
            delay = 90;
            break;
        case 4:
            delay = 40;
            break;
        case 5:
            delay = 30;
            break;
    }

    x=SCREEN_W/2;
    y=SCREEN_H/2;

    user_screen = create_bitmap(SCREEN_W,SCREEN_H);
    times = load_datafile("times.dat");
    gameover = load_bitmap("game_over.bmp", NULL);
    borders = load_bitmap("borders.bmp", NULL);
    head_up = load_bitmap("head_up.bmp", NULL);
    head_down = load_bitmap("head_down.bmp", NULL);
    head_left = load_bitmap("head_left.bmp", NULL);
    head_right = load_bitmap("head_right.bmp", NULL);
    body = load_bitmap("body.bmp", NULL);
    food = load_bitmap("apple.bmp", NULL);

    place_borders();

    sprintf(txt,"Use the arrows to control the snake!");
    textout_ex(user_screen,TIMES_NEW,txt,SCREEN_W/2-170, SCREEN_H/2-15, WHITE, -1);

    draw_sprite(screen,user_screen,0,0);

    while(!keypressed()) ;//kullanicinin bekledigi tusa basiniz

    destroy_bitmap(user_screen);

    user_screen = create_bitmap(SCREEN_W,SCREEN_H);
    place_borders();
    place_food();

    draw_sprite(screen,user_screen,0,0);

    time(&start);

    Until_you_die //ana dongu
    {
        rest(delay);

        if(keypressed()) user_key=readkey();

        guarda_body();
        verifica_body();
        if(tam_2 == tam) clean();

        if(x<20 || y<20 || x>SCREEN_W-40 || y>SCREEN_H-40) break;
        else if(x == x_food && y == y_food) place_food();
        else replace_food();

        up:///snake movement loop
        if(user_key >> 8 == KEY_ESC) break;
        else if(user_key >> 8 == KEY_UP )
        {
            if (aux_key >> 8 == KEY_DOWN) { user_key=aux_key; goto up; }
            else { y-=20; snake_dir = UP;}
        }
        else if(user_key >> 8 == KEY_DOWN)
        {
            if (aux_key >> 8 == KEY_UP) { user_key=aux_key; goto up; }
            else { y+=20; snake_dir = DOWN;}
        }
        else if(user_key >> 8 == KEY_LEFT)
        {
            if (aux_key >> 8 == KEY_RIGHT) { user_key=aux_key; goto up; }
            else { x-=20; snake_dir = LEFT;}
        }
        else if(user_key >> 8 == KEY_RIGHT)
        {
            if (aux_key >> 8 == KEY_LEFT) { user_key=aux_key; goto up; }
            else { x+=20; snake_dir = RIGHT;}
        }
        else
        {
            for(;;) if(keypressed()) break;
            continue;
        }

        aux_key = user_key;

        //bitmap'i degistir
        masked_blit(body, user_screen, 0, 0, snake[tam_2][0], snake[tam_2][1], 20, 20);
        switch (snake_dir)
        {
            case UP:
                masked_blit(head_up, user_screen, 0, 0, x, y, 20, 20); //kareyi konumlandirir
                break;

            case DOWN:
                masked_blit(head_down, user_screen, 0, 0, x, y, 20, 20); //kareyi konumlandirir
                break;

            case RIGHT:
                masked_blit(head_right, user_screen, 0, 0, x, y, 20, 20); //kareyi konumlandirir
                break;

            case LEFT:
                masked_blit(head_left, user_screen, 0, 0, x, y, 20, 20); //kareyi konumlandirir
                break;

            default:
                masked_blit(head_left, user_screen, 0, 0, x, y, 20, 20); //kareyi konumlandirir
                break;
        }

        BITMAP *clean = NULL;

        length = text_length(TIMES_NEW,txt);
        alt = text_height(TIMES_NEW);

        clean = create_bitmap(length, alt); //temiz bir bitmap olustur

        draw_sprite(user_screen, clean, 9, SCREEN_H-alt+3);

        sprintf(txt,"Score:%5.d", points);
        textout_ex(user_screen,TIMES_NEW,txt,9,SCREEN_H-alt+3,WHITE,BLACK);

        draw_sprite(screen, user_screen, 0, 0); 

        destroy_bitmap(clean);

    }

    time(&end);

    dif = difftime(end, start);

    draw_sprite(screen, gameover, SCREEN_W/2-170, SCREEN_H/2-150);

    if(mode == 2 || mode == 3) //tam ekran
    {
        sprintf(txt, "Score: %5.d", points);
        textout_ex(screen,TIMES_NEW,txt,SCREEN_W/2-50,SCREEN_H/2+50,WHITE,BLACK);

        sprintf(txt, "Time: %4.lfs", dif);
        textout_ex(screen,TIMES_NEW,txt,SCREEN_W/2-50,SCREEN_H/2+100,WHITE,BLACK);

        sprintf(txt,"Press ESC to quit");
        textout_ex(screen,font,txt,SCREEN_W/2-60, SCREEN_H-100,WHITE,BLACK);
        while(!key[KEY_ESC]) ;
    }

    else
    {
        sprintf(txt,"Score:  %d\nZaman:  %.lf s\n",points,dif);
        allegro_message(txt);
    }

    allegro_exit();

    return 0;
}
END_OF_MAIN()
