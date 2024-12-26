#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
//adcionando os prototipos
#include "objeto.h"

//globais
#define ALTURA  640
#define LARGURA  460
#define QUANT_INIMIGOS 10
#define FPS 60
 
enum teclas{cima, baixo, esquerda, direita};
bool teclas[4] = {false, false, false, false};

//prototipos das funcoes aqui em cima para o codico ficar mais legivel

int main(){
    bool fechar = false;
    bool redraw = true;
    int pos_x = LARGURA / 2;
	int pos_y = ALTURA / 2;
    int imag_x = LARGURA/2;
	int imag_y = ALTURA/2;
	float grau = 0;

    //cria janela
    ALLEGRO_DISPLAY *Janela = NULL;
    //cria um evento para chamar uma entrada do computador
    ALLEGRO_EVENT_QUEUE *Evento = NULL;
    //clemilson é um nome engraçado
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_BITMAP *seta = NULL;

    //principal funcao para iniciar tudo
    if(!al_init()){
        printf("Erro ao iniciar\n");
        return EXIT_FAILURE;
    }

    //atribui as dimencoes a janela
    Janela = al_create_display(ALTURA, LARGURA);
    if(!Janela){
        printf("Erro ao criar janela\n");
        return EXIT_FAILURE;
    }

    //eu ainda tenho que definir esta
    al_init_primitives_addon();
    al_install_keyboard();
    al_init_image_addon();
    //cria o evento :0
    Evento = al_create_event_queue();
    timer = al_create_timer(1.0/FPS);
    seta = al_load_bitmap("seta.png");
    imag_x = al_get_bitmap_width(seta);
	imag_y = al_get_bitmap_height(seta);

    al_register_event_source(Evento, al_get_keyboard_event_source());
    al_register_event_source(Evento, al_get_timer_event_source(timer));
    al_register_event_source(Evento, al_get_display_event_source(Janela)); 

    srand(time(NULL));

    al_start_timer(timer);
    //looping para manter a janela aberta
    while(!fechar){
        ALLEGRO_EVENT ev;
        al_wait_for_event(Evento, &ev);
        if(ev.type == ALLEGRO_EVENT_TIMER){
            redraw = true;
        }
		else if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch(ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				fechar = true;
				break;
            case ALLEGRO_KEY_UP:
                grau = 0;
                break;
			case ALLEGRO_KEY_LEFT:
				grau = -90;
				break;
            case ALLEGRO_KEY_DOWN:
                grau = 180;
                break;
			case ALLEGRO_KEY_RIGHT:
				grau = 90;
				break;
			}
		}
        else if(ev.type == ALLEGRO_EVENT_KEY_UP){
            switch(ev.keyboard.keycode){
                case ALLEGRO_KEY_UP:
                    teclas[cima] = false;
                    break;
                case ALLEGRO_KEY_DOWN:
                    teclas[baixo] = false;
                    break;
                case ALLEGRO_KEY_LEFT:
                    teclas[esquerda] = false;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    teclas[direita] = false;
                    break;
            }
        }
        else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            fechar = true;
        }
        else if(ev.type == ALLEGRO_EVENT_TIMER)
		{
			pos_y -= teclas[cima] * 10; 
			pos_y += teclas[baixo] * 10;
			pos_x -= teclas[esquerda] * 10;
			pos_x += teclas[direita] * 10;
			redraw = true;
		}
        if(redraw && al_is_event_queue_empty(Evento)){
            redraw = false;
            al_flip_display();
            al_clear_to_color(al_map_rgb(192, 192, 192));
            al_draw_filled_rectangle(0, 270, ALTURA, 190, al_map_rgb(128, 128, 128));
            al_draw_rotated_bitmap(seta, imag_x / 2, imag_y / 2, ALTURA / 2, LARGURA / 2, grau * 3.14 / 180, 0);
        }
    }
    al_destroy_bitmap(seta);
    al_destroy_event_queue(Evento);
    al_destroy_display(Janela);
}