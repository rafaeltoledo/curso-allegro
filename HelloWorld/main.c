#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <time.h>
#include <string.h>

const int FPS = 60;
const int INTERVALO_CANOS = 240;

void mostra_texto() {
    ALLEGRO_EVENT_QUEUE *fila_eventos =
        al_create_event_queue();
    ALLEGRO_FONT *fonte = 
        al_load_font("Recursos/flappy_font.ttf", 48, 0);
    ALLEGRO_BITMAP *fundo =
        al_load_bitmap("Recursos/scroll.jpg");
    ALLEGRO_BITMAP *sprites[3];
    sprites[0] = al_load_bitmap("Recursos/frame01.png");
    al_convert_mask_to_alpha(sprites[0], al_map_rgb(0xFF, 0, 0xFF));
    sprites[1] = al_load_bitmap("Recursos/frame02.png");
    al_convert_mask_to_alpha(sprites[1], al_map_rgb(0xFF, 0, 0xFF));
    sprites[2] = al_load_bitmap("Recursos/frame03.png");
    al_convert_mask_to_alpha(sprites[2], al_map_rgb(0xFF, 0, 0xFF));

    al_register_event_source(fila_eventos,
        al_get_keyboard_event_source());

    int y1 = 0;
    int y2 = al_get_bitmap_height(fundo);

    int tempo_inicial, tempo_final;

    char nome[20];
    strcpy(nome, ""); // Inicializando a string
    bool sair = false, andando = false;

    int indice_bitmap = 0;
    int sentido = 1, x = 100;
    int atualiza_frame = 0;
    while (sair == false) {
        tempo_inicial = al_get_time();

        //al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_bitmap(fundo, 0, y1--, 0);
        al_draw_bitmap(fundo, 0, y2--, 0);

        if (andando) {
            x += 2 * sentido;

            if (x < 0) {
                x = 0;
            } else if (x > 640 - al_get_bitmap_width(sprites[indice_bitmap])) {
                x = 640 - al_get_bitmap_width(sprites[indice_bitmap]);
            }

            atualiza_frame++;
            if (atualiza_frame % 7 == 0) {
                atualiza_frame = 0;
                indice_bitmap++;
                if (indice_bitmap == 3) indice_bitmap = 0;
            }
        }

        if (sentido > 0) {
            al_draw_bitmap(sprites[indice_bitmap], x, 150, 0);
        } else {
            al_draw_bitmap(sprites[indice_bitmap], x, 150, ALLEGRO_FLIP_HORIZONTAL);
        }
        
        al_draw_textf(fonte, al_map_rgb(0xFF, 0xFF, 0xFF),
            10, 10, ALLEGRO_ALIGN_LEFT, "Nome: %s", nome);
        al_flip_display();

        while (!al_is_event_queue_empty(fila_eventos)) {
            ALLEGRO_EVENT evento;
            al_wait_for_event(fila_eventos, &evento);

            if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
                if (evento.keyboard.keycode ==
                        ALLEGRO_KEY_ESCAPE) {
                    sair = true;
                }
                else if (evento.keyboard.keycode ==
                        ALLEGRO_KEY_BACKSPACE) {
                    if (strlen(nome) > 0) {
                        nome[strlen(nome) - 1] = '\0';
                    }
                }
                else if (evento.keyboard.keycode ==
                        ALLEGRO_KEY_LEFT) {
                    sentido = -1;
                    andando = true;
                }
                else if (evento.keyboard.keycode ==
                        ALLEGRO_KEY_RIGHT) {
                    sentido = 1;
                    andando = true;
                }
            }
            else if (evento.type == ALLEGRO_EVENT_KEY_UP) {
                if (evento.keyboard.keycode == ALLEGRO_KEY_LEFT ||
                    evento.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
                    andando = false;
                    indice_bitmap = 0;
                }
            }
            else if (evento.type == ALLEGRO_EVENT_KEY_CHAR) {
                if (strlen(nome) < 19) {
                    char temp[] = {evento.keyboard.unichar, '\0'};
                    if ( evento.keyboard.unichar == ' ' ||
                        (evento.keyboard.unichar >= '0' && evento.keyboard.unichar <= '9') ||
                        (evento.keyboard.unichar >= 'a' && evento.keyboard.unichar <= 'z') ||
                        (evento.keyboard.unichar >= 'A' && evento.keyboard.unichar <= 'Z')) {
                        strcat(nome, temp); // Concatena temp em nome
                    }
                }
            }
        }

        if (y1 <= -al_get_bitmap_height(fundo)) {
            y1 = al_get_bitmap_height(fundo);
        } 

        if (y2 <= -al_get_bitmap_height(fundo)) {
            y2 = al_get_bitmap_height(fundo);
        }

        tempo_final = al_get_time() - tempo_inicial;
        if (tempo_final < 1.0 / FPS) {
            al_rest(1.0 / FPS - tempo_final);
        }
    }

    int i;
    for (i = 0; i < 3; i++) {
        al_destroy_bitmap(sprites[i]);
    }
    al_destroy_bitmap(fundo);
    al_destroy_font(fonte);
    al_destroy_event_queue(fila_eventos);
}

struct cano {
    int x;
    int y;
};

int atualizar_posicao(int aceleracao, int posicao) {
    return posicao - aceleracao;
}

void desenha_cano(struct cano c)
{
    al_draw_filled_rectangle(c.x, 0, c.x + 80, c.y,
        al_map_rgb(0, 0xFF, 0));

    al_draw_filled_rectangle(c.x, c.y + 120, c.x + 80, 480,
        al_map_rgb(0, 0xFF, 0));
}

int gera_altura_cano() {
    return rand() % 350 + 10;
}

struct cano atualizar_cano(struct cano c) {
    c.x = c.x - 2;

    if (c.x < -80) {
        c.x = 640 + INTERVALO_CANOS;
        c.y = gera_altura_cano();
    }

    return c;
}

bool verifica_colisao(int x, int y, struct cano c) {

    int raio = 20;
    int distancia_canos = 120;
    int largura_cano = 80;

    // 1.
    if (x - raio > c.x + largura_cano) return false;
    // 2.
    if (x + raio < c.x) return false;
    // 3.
    if (y - raio > c.y &&
        y + raio < c.y + distancia_canos) return false;

    return true;
}

void mostra_jogo()
{
    ALLEGRO_BITMAP *fundo = 
                al_load_bitmap("Recursos/flappy_bg.jpg");
    ALLEGRO_SAMPLE *som_pulo =
        al_load_sample("Recursos/sfx_wing.ogg");
    ALLEGRO_SAMPLE *batida =
        al_load_sample("Recursos/sfx_hit.ogg");
    ALLEGRO_SAMPLE *ponto =
        al_load_sample("Recursos/sfx_point.ogg");
    ALLEGRO_FONT *fonte =
        al_load_font("Recursos/flappy_font.ttf", 48, 0);
    ALLEGRO_FONT *fonte_game_over =
        al_load_font("Recursos/flappy_font.ttf", 200, 0);
    ALLEGRO_TIMER *timer = al_create_timer(1.0);

    ALLEGRO_EVENT_QUEUE *fila_eventos =
        al_create_event_queue();
    al_register_event_source(fila_eventos,
        al_get_keyboard_event_source());
    al_register_event_source(fila_eventos,
        al_get_mouse_event_source());
    al_register_event_source(fila_eventos,
        al_get_timer_event_source(timer));

    srand(time(NULL));

    int y = 240;
    float aceleracao = 0;
    double tempo_inicial, tempo_final;
    bool sair = false;
    int raio = 20;
    bool game_over = false;

    struct cano canos[4];

    // inicialização dos canos
    canos[0].x = 750;
    canos[0].y = gera_altura_cano();

    canos[1].x = 750 + (INTERVALO_CANOS * 1);
    canos[1].y = gera_altura_cano();

    canos[2].x = 750 + (INTERVALO_CANOS * 2);
    canos[2].y = gera_altura_cano();

    canos[3].x = 750 + (INTERVALO_CANOS * 3);
    canos[3].y = gera_altura_cano();

    int pontuacao = 0;
    al_start_timer(timer);
    while (sair == false) {
        tempo_inicial = al_get_time();

        //al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_bitmap(fundo, 0, 0, 0);

        int i = 0;
        for (i = 0; i < 4; i++) {
            desenha_cano(canos[i]);
        }

        al_draw_textf(fonte, al_map_rgb(0xFF, 0xFF, 0xFF),
            10, 10, ALLEGRO_ALIGN_LEFT, "%i", pontuacao);

        al_draw_filled_circle(70, y, raio,
            al_map_rgb(0xFF, 0, 0));

        if (game_over) {
            al_draw_text(fonte, al_map_rgb(0xFF, 0xFF, 0),
                320, 240, ALLEGRO_ALIGN_CENTER, "Game Over");
        }

        al_flip_display();

        y = atualizar_posicao(aceleracao, y);
        aceleracao -= 0.25;

        if (y > 480 - raio) {
            y = 480 - raio;
            aceleracao = 0;
            if (game_over == false) {
                game_over = true;
                al_play_sample(batida, 1.0, 0.0, 1.0,
                    ALLEGRO_PLAYMODE_ONCE, NULL);
            }
        }

        if (y < raio) {
            y = raio;
            aceleracao = 0;
        }

        while (!al_is_event_queue_empty(fila_eventos)) {
            ALLEGRO_EVENT evento;
            al_wait_for_event(fila_eventos, &evento);

            if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
                if (evento.keyboard.keycode ==
                                    ALLEGRO_KEY_ESCAPE)
                {
                    sair = true;
                }
            }
            else if (evento.type ==
                    ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                if (game_over == false) {
                    aceleracao = 5;
                    al_play_sample(som_pulo, 1.0, 0.0, 1.0,
                        ALLEGRO_PLAYMODE_ONCE, NULL);
                }
            }
            else if (evento.type ==
                    ALLEGRO_EVENT_TIMER) {
                if (game_over == false) {
                    al_play_sample(ponto, 1.0, 0.0, 1.0,
                        ALLEGRO_PLAYMODE_ONCE, NULL);
                    pontuacao++;
                }
            }
        }

        if (game_over == false) {
            for (i = 0; i < 4; i++) {
                game_over = verifica_colisao(70, y, canos[i]);

                if (game_over) {
                    al_play_sample(batida, 1.0, 0.0, 1.0,
                        ALLEGRO_PLAYMODE_ONCE, NULL);
                    break;
                }
            }

            for (i = 0; i < 4; i++) {
                canos[i] = atualizar_cano(canos[i]);
            }
        }

        tempo_final = al_get_time() - tempo_inicial;
        if (tempo_final < 1.0 / FPS) {
            al_rest(1.0 / FPS - tempo_final);
        }
    }

    al_destroy_timer(timer);
    al_destroy_font(fonte);
    al_destroy_font(fonte_game_over);
    al_destroy_sample(ponto);
    al_destroy_sample(batida);
    al_destroy_sample(som_pulo);
    al_destroy_bitmap(fundo);
    al_destroy_event_queue(fila_eventos);
}

void monta_menu(int opcao_selecionada)
{
    ALLEGRO_BITMAP *config =
        al_load_bitmap("Recursos/config.png");
    ALLEGRO_BITMAP *config_pressed =
        al_load_bitmap("Recursos/config_pressed.png");
    ALLEGRO_BITMAP *iniciar =
        al_load_bitmap("Recursos/iniciar.png");
    ALLEGRO_BITMAP *iniciar_pressed =
        al_load_bitmap("Recursos/iniciar_pressed.png");
    ALLEGRO_BITMAP *sair =
        al_load_bitmap("Recursos/sair.png");
    ALLEGRO_BITMAP *sair_pressed =
        al_load_bitmap("Recursos/sair_pressed.png");

    // Primeira opção do menu (iniciar)
    if (opcao_selecionada == 1) {
        al_draw_bitmap(iniciar_pressed, 320 -
            al_get_bitmap_width(iniciar_pressed) / 2,
            240 - 10 -
            al_get_bitmap_height(iniciar_pressed) -
            al_get_bitmap_height(config) / 2,
            0);
    } else {
        al_draw_bitmap(iniciar, 320 -
            al_get_bitmap_width(iniciar) / 2,
            240 - 10 -
            al_get_bitmap_height(iniciar) -
            al_get_bitmap_height(config) / 2,
            0);
    }

    // Segundo botão do menu (config)
    if (opcao_selecionada == 2) {
        al_draw_bitmap(config_pressed, 320 -
            al_get_bitmap_width(config_pressed) / 2,
            240 - al_get_bitmap_height(config_pressed) / 2,
            0);
    } else {
        al_draw_bitmap(config, 320 -
            al_get_bitmap_width(config) / 2,
            240 - al_get_bitmap_height(config) / 2,
            0);
    }

    // Terceiro botão do menu (sair)
    if (opcao_selecionada == 3) {
        al_draw_bitmap(sair_pressed, 320 -
            al_get_bitmap_width(sair_pressed) / 2,
            240 + 10 + al_get_bitmap_height(config) / 2,
            0);
    } else {
        al_draw_bitmap(sair, 320 -
            al_get_bitmap_width(sair_pressed) / 2,
            240 + 10 + al_get_bitmap_height(config) / 2,
            0);
    }

    al_destroy_bitmap(config);
    al_destroy_bitmap(config_pressed);
    al_destroy_bitmap(iniciar);
    al_destroy_bitmap(iniciar_pressed);
    al_destroy_bitmap(sair);
    al_destroy_bitmap(sair_pressed);
}

int main()
{
    // Declara a janela do aplicativo
    ALLEGRO_DISPLAY *janela;
    // Variável para controlar os eventos
    ALLEGRO_EVENT_QUEUE *fila_eventos;
    // Declara a imagem de fundo
    ALLEGRO_BITMAP *fundo;
    // Variável que representa a música de fundo
    ALLEGRO_AUDIO_STREAM *musica;

    // Inicializa a biblioteca
    al_init();
    // Inicializa o módulo de imagens
    al_init_image_addon();
    // Inicializando o teclado
    al_install_keyboard();
    // Inicializando o mouse
    al_install_mouse();
    // Inicializar o áudio
    al_install_audio();
    // Inicializar a parte de codecs de áudio
    al_init_acodec_addon();
    // Reservar samples
    al_reserve_samples(4);
    // Inicializar o módulo de primitivas gráficas
    al_init_primitives_addon();
    // Inicializar as bibliotecas de fonte
    al_init_font_addon();
    al_init_ttf_addon();

    // Cria a fila de eventos
    fila_eventos = al_create_event_queue();
    // Carregar a imagem de fundo
    fundo = al_load_bitmap("Recursos/bg.jpg");

    // Cria uma janela de 640x480
    janela = al_create_display(640, 480);
    al_set_window_title(janela, "Super Projeto");
    // Pinta a tela de vermelho
    //al_clear_to_color(al_map_rgb(0xFF, 0x00, 0x00));

    // Desenhar a imagem na tela
    al_draw_bitmap(fundo, 0, 0, 0);

    // Aplica as alterações na tela
    al_flip_display();

    // Carregar a música
    musica = al_load_audio_stream("Recursos/music.ogg",
        4, 1024);
    al_attach_audio_stream_to_mixer(musica,
        al_get_default_mixer());

    // A fila de eventos receberá os eventos da janela
    al_register_event_source(fila_eventos,
        al_get_display_event_source(janela));
    al_register_event_source(fila_eventos,
        al_get_keyboard_event_source());

    // Música, comece a tocar!
    al_set_audio_stream_playing(musica, true);

    // Espera 5 segundos
    //al_rest(5.0);
    int opcao_menu = 1;
    bool sair = false;
    while (sair == false)
    {
        al_draw_bitmap(fundo, 0, 0, 0);
        monta_menu(opcao_menu);
        al_flip_display();

        while (!al_is_event_queue_empty(fila_eventos))
        {
            ALLEGRO_EVENT evento;
            al_wait_for_event(fila_eventos, &evento);

            if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                sair = true;
            }
            else if (evento.type == ALLEGRO_EVENT_KEY_DOWN)
            {
                if (evento.keyboard.keycode ==
                                    ALLEGRO_KEY_ESCAPE)
                {
                    sair = true;
                }
                else if (evento.keyboard.keycode ==
                                    ALLEGRO_KEY_DOWN)
                {
                    opcao_menu++;
                    if (opcao_menu > 3) {
                        opcao_menu = 1;
                    }
                }
                else if (evento.keyboard.keycode ==
                                    ALLEGRO_KEY_UP)
                {
                    opcao_menu--;
                    if (opcao_menu < 1) {
                        opcao_menu = 3;
                    }
                }
                else if (evento.keyboard.keycode ==
                                    ALLEGRO_KEY_ENTER)
                {
                    if (opcao_menu == 3) {
                        sair = true;
                    }
                    else {
                        al_set_audio_stream_playing(musica, false);
                        al_unregister_event_source(
                            fila_eventos,
                            al_get_keyboard_event_source());

                        if (opcao_menu == 1) {
                            mostra_jogo();    
                        } else if (opcao_menu == 2) {
                            mostra_texto();
                        }
                        
                        al_register_event_source(
                            fila_eventos,
                            al_get_keyboard_event_source());
                        al_rewind_audio_stream(musica);
                        al_set_audio_stream_playing(musica, true);
                    }
                }
            }
        }
    }

    // Música, pode parar!
    al_set_audio_stream_playing(musica, false);

    // Libera a memória usada pela música
    al_destroy_audio_stream(musica);
    // Libera a memória usada pela imagem de fundo
    al_destroy_bitmap(fundo);
    // Libera a memória usada pela janela
    al_destroy_display(janela);

    return 0;
}
