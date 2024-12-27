#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

//GLOBAIS
#define ALTURA  640
#define LARGURA  460
#define FPS 60

int main(){
    //VARIAVEIS DO JOGO
    enum teclas{cima, baixo, esquerda, direita};
    bool teclas[4] = {false, false, false, false};
    
    bool fechar = false;
    bool redraw = true;

    //INICIA OBJETOS
    enum {JOGADOR, INIMIGOS};

    //CRIA JOGADOR
    int jogador_ID = JOGADOR;
    int jogador_x = LARGURA / 2;
	int jogador_y = ALTURA / 2;
    int jogador_borda_x = LARGURA/2;
	int jogador_borda_y = ALTURA/2;
    int jogador_pontos = 0;
	float jogador_grau = 0;

    //CRIA INIMIGO
    int inim_ID = INIMIGOS;
    int inim_x = LARGURA/2;
    int inim_y = ALTURA/2;
    int inim_borda_x = LARGURA/2;
    int inim_borda_y = ALTURA/2;
    int inim_velocidade = 7;
    int inim_grau = 90;
    bool acertou = false;

    //INICIA OS OBJETOS DA BIBLIOTECA
    ALLEGRO_DISPLAY *janela = NULL;
    ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_BITMAP *seta = NULL;
    ALLEGRO_BITMAP *inim = NULL;

    //INICIALIZACAO DO JOGO
    if(!al_init()){
        printf("Erro ao iniciar\n");
        return EXIT_FAILURE;
    }

    //INICIALIZACAO DA JANELA
    janela = al_create_display(ALTURA, LARGURA);
    if(!janela){
        printf("Erro ao criar janela\n");
        return EXIT_FAILURE;
    }

    //INICIALIZACAO DE ADDON E INSTALACOES
    al_init_primitives_addon();
    al_install_keyboard();
    al_init_image_addon();

    //DESENHO JOGADOR
    seta = al_load_bitmap("Assets/seta.png");
    jogador_borda_x = al_get_bitmap_width(seta);
    jogador_borda_y = al_get_bitmap_height(seta);

    //DESENHO INIMIGO
    inim = al_load_bitmap("Assets/seta.png");
    inim_borda_x = al_get_bitmap_width(inim);
    inim_borda_y = al_get_bitmap_height(inim);

    //CRIACAO DA FILA E DEMAIS DISPOSITIVOS
    fila_eventos = al_create_event_queue();
    timer = al_create_timer(1.0/FPS);
    
    //REGISTROS
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    al_register_event_source(fila_eventos, al_get_display_event_source(janela)); 

    //FUNCOES INICIAIS
    srand(time(NULL));
    al_start_timer(timer);  

    //LOOPING PRINCIPAL
    while(!fechar){
        ALLEGRO_EVENT ev;
        al_wait_for_event(fila_eventos, &ev);

        //TECLADO
        if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            fechar = true;
        }
		else if(ev.type == ALLEGRO_EVENT_KEY_DOWN){
			switch(ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				fechar = true;
				break;
            case ALLEGRO_KEY_P:
                fechar = true;
                break;
            case ALLEGRO_KEY_UP:
                teclas[cima] = true;
                break;
            case ALLEGRO_KEY_DOWN:
                teclas[baixo] = true;
                break;
            case ALLEGRO_KEY_LEFT:
                teclas[esquerda] = true;
                break;
            case ALLEGRO_KEY_RIGHT:
                teclas[direita] = true;
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
        //EVENTOS E LOGICA DO JOGO
        else if(ev.type == ALLEGRO_EVENT_TIMER){
            redraw = true;
            if(teclas[cima])
                jogador_grau = 0;
            if(teclas[baixo])
                jogador_grau = 180;
            if(teclas[esquerda])
                jogador_grau = -90;
            if(teclas[direita])
                jogador_grau = 90;
            inim_y -= inim_velocidade;
        }
        //JOGO
        if(redraw && al_is_event_queue_empty(fila_eventos)){
            al_draw_rotated_bitmap(seta, jogador_borda_x / 2, jogador_borda_y / 2, ALTURA / 2, LARGURA / 2, jogador_grau * 3.14 / 180, 0);
            al_draw_rotated_bitmap(inim, inim_x / 2, inim_y / 2, ALTURA, 308, inim_grau * 3.14 / 180, 0);
            al_flip_display();
            al_clear_to_color(al_map_rgb(192, 192, 192));
            al_draw_filled_rectangle(0, 270, ALTURA, 190, al_map_rgb(128, 128, 128));
        }
    }
    //FINALIZACAO DO PROGRAMA
    al_destroy_display(janela);
    al_destroy_event_queue(fila_eventos);
    al_destroy_bitmap(seta);
    al_destroy_timer(timer);
}