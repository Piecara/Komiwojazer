#include "stdafx.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h> 
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <ctime>
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5\allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h> 
#include <allegro5/allegro_ttf.h>
#include <math.h>
#include <cmath>
using namespace std;


const float FPS = 15.0;
int windowHeight = 720;
int windowWidth = 1280;

const int amount = 100;

int dotsAmount = 10;
int dotSize = 12;
int lineSize = 2;

int wybor = -1;

bool running = true;
bool menu = true;
bool process = false;
bool calculates = true;

bool distanceOnLines = true;
bool numbersOnDots = true;
bool drawLine = true;

void gwazdki(int ilosc) {
	int i;
	srand(time(0));

	for (i = 0; i < ilosc; i++) {
		int x = rand() % windowWidth;
		int y = rand() % windowHeight;
		al_draw_filled_circle(x, y, 2, al_map_rgba(200, 200, 200, 60));
	}
}

class dots {
public:
	int dotId;
	int x;
	int y;
	int r;
	bool visited;
	void draw() {
		al_draw_filled_circle(this->x, this->y, this->r, al_map_rgb(255, 255, 255));
	}
};

void createDots(dots dotArr[amount], int amount) {
	for (int i = 0; i < amount; i++) {
		dotArr[i].x = dotSize + rand() % (windowWidth - 2 * dotSize);
		dotArr[i].y = dotSize + rand() % (windowHeight - 2 * dotSize);
		dotArr[i].r = dotSize;
		dotArr[i].visited = false;
		dotArr[i].dotId = i;
		cout << dotArr[i].x << " x  ";
		cout << dotArr[i].y << " y   #" << i << endl;
	}
}
float calculateDistance(dots dotArr[amount],int start, int amount) {
	float n = 0;
	for (int i = start; i < amount-1; i++)
	n += sqrt((dotArr[i].x - dotArr[i+1].x) *(dotArr[i].x - dotArr[i+1].x) + (dotArr[i].y - dotArr[i + 1].y) *(dotArr[i].y - dotArr[i + 1].y));
	return n;
}
void najblizszegoSasiada(dots dotArr[],dots dotArrNS[], int amount) {
	dotArrNS[0] = dotArr[0];
	dotArr[0].visited = true;
	float odleglosc;
	float odlegloscSave = 15000;
	int counter = 0;


	while (counter < amount) {
		odlegloscSave = 15000;
		for (int i = 0; i < amount; i++) {
			if (dotArr[i].visited == true)
				continue;
			else {
				odleglosc = sqrt((dotArrNS[counter].x - dotArr[i].x) *(dotArrNS[counter].x - dotArr[i].x) + (dotArrNS[counter].y - dotArr[i].y) *(dotArrNS[counter].y - dotArr[i].y));
				if (odleglosc < odlegloscSave) {
					dotArrNS[counter+1] = dotArr[i];
					odlegloscSave = odleglosc;
				}
			}

		}
		for (int i=0; i < amount; i++) {
			if (dotArrNS[counter+1].dotId == dotArr[i].dotId) {
				dotArr[i].visited = true;
			}
		}
		counter++;
	}
}

void drawDots(dots dotArr[amount], int dotsAmount, ALLEGRO_FONT *font24, bool numbers) {
	for (int i = 0; i < dotsAmount; i++) {
		
		dotArr[i].draw();
		al_draw_filled_circle(dotArr[0].x, dotArr[0].y, dotArr[0].r, al_map_rgb(0, 255, 0));
		al_draw_filled_circle(dotArr[dotsAmount-1].x, dotArr[dotsAmount-1].y, dotArr[dotsAmount-1].r, al_map_rgb(255, 0, 0));

		if (numbers) 
			al_draw_textf(font24, al_map_rgb(0, 0, 0), dotArr[i].x-dotSize+1, dotArr[i].y-dotSize+1, 0, "%d", i+1);
		
	}
	if(numbers)
		al_draw_textf(font24, al_map_rgb(0, 0, 0), dotArr[0].x - dotSize + 1, dotArr[0].y - dotSize + 1, 0, "%d", 1);
}

void drawLines(dots dotArr[amount], int amount, ALLEGRO_FONT *font24, bool drawDistance) {
	for (int i = 0; i < amount; i++)
	{
		if (i == (amount - 1)) continue;
		al_draw_line(dotArr[i].x, dotArr[i].y, dotArr[i + 1].x, dotArr[i + 1].y, al_map_rgba(0, 255, 0, 128), lineSize);
	if(drawDistance)
		al_draw_textf(font24, al_map_rgb(255, 255, 255), -20+(dotArr[i].x + dotArr[i+1].x) / 2, -20 + (dotArr[i].y + dotArr[i + 1].y) / 2, 0, "%.0f", calculateDistance(dotArr,i,i+2));

	}
	al_draw_line(dotArr[0].x, dotArr[0].y, dotArr[amount - 1].x, dotArr[amount - 1].y, al_map_rgba(0, 255, 0, 128), lineSize);
}

void menuDrawing(ALLEGRO_KEYBOARD_STATE keyboard, ALLEGRO_FONT *font24, ALLEGRO_FONT *font48) {
	bool incrementDots = false;
	bool decrementDots = false;
	ALLEGRO_COLOR colorGold = al_map_rgb(235, 195, 0);
	ALLEGRO_COLOR colorGoldPressed = al_map_rgb(255, 255, 255);
	ALLEGRO_COLOR colorButton = al_map_rgb(51, 75, 102);
	ALLEGRO_COLOR colorFocus = al_map_rgb(85, 125, 170);


	gwazdki(100);


	if (al_key_down(&keyboard, ALLEGRO_KEY_1)) {
		wybor = 0;
		decrementDots = true;

	};

	if (al_key_down(&keyboard, ALLEGRO_KEY_2)) {
		wybor = 1;
	};

	if (al_key_down(&keyboard, ALLEGRO_KEY_3)) {
		wybor = 2;
		incrementDots = true;
	};


	if (al_key_down(&keyboard, ALLEGRO_KEY_4)) {
		numbersOnDots = true;
	};

	if (al_key_down(&keyboard, ALLEGRO_KEY_5)) {
		distanceOnLines = true;
	};

	if (al_key_down(&keyboard, ALLEGRO_KEY_6)) {
		drawLine = true;
	};




	al_draw_filled_rectangle(windowWidth / 2 - 550, 300, windowWidth / 2 - 250, 600, colorButton);
	al_draw_filled_rectangle(windowWidth / 2 - 90, 395, windowWidth / 2, 425, colorButton);
	al_draw_filled_rectangle(windowWidth / 2 + 250, 300, windowWidth / 2 + 550, 600, colorButton);



	if (decrementDots == true) {
		al_draw_filled_rectangle(windowWidth / 2 - 550, 300, windowWidth / 2 - 250, 600, colorFocus);
		dotsAmount -= 1;
		if (dotsAmount <= 1) dotsAmount = 1;
		incrementDots = false;


	};
	if (wybor == 1) {
		al_draw_filled_rectangle(windowWidth / 2 - 150, 300, windowWidth / 2 + 150, 600, colorFocus);
		process = true;
		menu = false;
		wybor = NULL;
	}


	if (incrementDots == true) {
		al_draw_filled_rectangle(windowWidth / 2 + 250, 300, windowWidth / 2 + 550, 600, colorFocus);
		dotsAmount += 1;
		if (dotsAmount >= amount) dotsAmount = amount;
		decrementDots = false;
	}
	al_draw_textf(font48, colorGold, 560, windowHeight - 320, 0, "dots : %d", dotsAmount);


	al_draw_text(font24, colorGold, 30, 510, 0, "1 - zmniejsz");
	al_draw_text(font24, colorGold, 30, 540, 0, "2 - uruchom");
	al_draw_text(font24, colorGold, 30, 570, 0, "3 - zwieksz");
	al_draw_text(font24, colorGold, 30, 600, 0, "4 - wlacz numeracje");
	al_draw_text(font24, colorGold, 30, 630, 0, "5 - wlacz dystans miedzy punktami");
	al_draw_text(font24, colorGold, 30, 660, 0, "6 - wlacz rysowanie linii + dlugosc trasy (powiazane z 5)");

}



int main() {

	srand(time(NULL));

	ALLEGRO_EVENT event;
	ALLEGRO_DISPLAY *display;
	ALLEGRO_EVENT_QUEUE *queue;
	ALLEGRO_TIMER *timer;
	ALLEGRO_KEYBOARD_STATE keyboard;


	al_init();
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();


	display = al_create_display(windowWidth, windowHeight);
	queue = al_create_event_queue();
	timer = al_create_timer(1.0 / FPS);


	al_clear_to_color(al_map_rgb(0, 0, 0));


	al_install_keyboard();

	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_timer_event_source(timer));


	ALLEGRO_FONT *font24 = al_load_font("arial.ttf", 18, 0);
	ALLEGRO_FONT *font48 = al_load_font("arial.ttf", 48, 0);
	if (font24 == NULL) puts("nie wczytano czcionki");

	al_start_timer(timer);


	//OPENERY
	dots dotArr[amount];
	dots dotArrNS[amount];
	float distance;
	bool voyage = false;


	while (running) {

		al_wait_for_event(queue, &event);

		double czas = al_get_time();

		if (event.type == ALLEGRO_EVENT_TIMER) {

			if (menu) {
				al_set_target_bitmap(al_get_backbuffer(display));
				al_clear_to_color(al_map_rgb(27, 39, 53));					// tlo okna

				al_get_keyboard_state(&keyboard);

				menuDrawing(keyboard, font24,font48);

			}
			if (process) {
				al_set_target_bitmap(al_get_backbuffer(display));
				al_clear_to_color(al_map_rgb(27, 39, 53));					// tlo okna

				al_get_keyboard_state(&keyboard);
				if (al_key_down(&keyboard, ALLEGRO_KEY_SPACE)) {
					voyage = true;
				}
				if (drawLine) {	
					if (voyage) {
						drawLines(dotArrNS, dotsAmount, font24, distanceOnLines);
						al_draw_textf(font24, al_map_rgb(255, 255, 255), 50, windowHeight - 50, 0, "dlugosc trasy : %.0f", distance);
					}
					else {
						drawLines(dotArr, dotsAmount, font24, distanceOnLines);
						al_draw_textf(font24, al_map_rgb(255, 255, 255), 50, windowHeight - 50, 0, "dlugosc trasy : %.0f", distance);
					}
					
				}

				
				if (calculates) {
					createDots(dotArr, dotsAmount);
					najblizszegoSasiada(dotArr, dotArrNS, dotsAmount);
					distance = calculateDistance(dotArr, 0,dotsAmount);
					calculates = false;
				}
				if (voyage) {
					drawDots(dotArrNS, dotsAmount, font24, numbersOnDots);
					distance = calculateDistance(dotArrNS, 0, dotsAmount);
				}
				else {
					drawDots(dotArr, dotsAmount, font24, numbersOnDots);
				}

				al_wait_for_event(queue, &event);

				if (al_key_down(&keyboard, ALLEGRO_KEY_ESCAPE)) {
					menu = true;
					process = false;
					calculates = true;
					numbersOnDots = false;
					distanceOnLines = false;
					drawLine = false;
					system("cls");
				};


			}

			if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
				running = false;

			al_flip_display();
		}
	}
	al_destroy_event_queue(queue);
	al_destroy_font(font24);
	al_destroy_display(display);
}
