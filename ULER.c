#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <sys/timeb.h>

struct segment
{
	int x,y;
	struct segment *next, *prev;
}*head,*tail,*current,*temp;

void header()
{
	printf("++===============================================================++\n");
	printf("||                         FINAL PROJECT                         ||\n");
	printf("||                     PROGRAM STRUKTUR DATA                     ||\n");
	printf("||                                                               ||\n");
	printf("||                 PERMAINAN SNAKE LINKED-LIST                   ||\n");
	printf("||---------------------------------------------------------------||\n");
	printf("||                                                               ||\n");
	printf("||                            OLEH :                             ||\n");
	printf("||                  FIRYANUL RIZKY          1708561004           ||\n");
	printf("||                  BAYU WIRA               1708561018           ||\n");
	printf("||                  LANANG ARY K            1708561031           ||\n");
	printf("||                                                               ||\n");
	printf("||---------------------------------------------------------------||\n");
	printf("||                  JURUSAN TEKNIK INFORMATIKA                   ||\n");
	printf("||                     UNIVERSITAS UDAYANA                       ||\n");
	printf("++===============================================================++\n");
	printf("TEKAN SEMBARANG TOMBOL UNTUK LANJUT\n");
	system("PAUSE");
	system("cls");
}

int skor_read()
{
	FILE *rd;
	int high_score;
	rd = fopen("Highscore.txt","r");
	if(!rd){
		return 0;
	}
	else{
		while(!feof(rd)){
			fscanf(rd,"%d",&high_score); fflush(stdin);
		}
		fclose(rd);
	}
	return high_score;
}

int skor_write(int score)
{
	FILE *wr;
	int skor;
	skor = score;
	wr = fopen("Highscore.txt","w");
	if(!wr){
		return 0;
	}
	else{
		fprintf(wr,"%d",skor);
	}
	fclose(wr);
}

/**
	Push segment ke snake (pada bagian head).
*/
void push(int x, int y)
{
	current = (struct segment*)malloc(sizeof(struct segment));
	current->next = current->prev = NULL;
	current->x = x;
	current->y = y;
	if(head == NULL){
		head = tail = current;
	}
	else{
		current->next = head;
		head->prev = current;
		head = current;
	}
}

/**
	Pop bagian ekor snake.
*/
void pop()
{
	if(head == tail){
		free(head);
		head = tail = NULL;
	}
	else{
		tail = tail->prev;
		free(tail->next);
		tail->next = NULL;
	}
}

void pop_all()
{
	while(head!=NULL){
		pop();
	}
}

/**
	Pindahkan posisi kursor di layar
	Fungsi ini spesifik untuk OS windows.
*/
void gotoxy(int x, int y)
{
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);

}

/**
	Gambar snake di layar
*/
void display()
{
	temp = head;
	while(temp!=NULL){
		gotoxy(temp->x,temp->y);
		printf("o");
		temp = temp->next;
	}
}

// Variabel untuk menyimpan panjang ular
int snake_size = 3;
int snake_speed = 200;
int dir = VK_RIGHT;
int game_width = 56;
int game_height = 25;
int food_x, food_y;

/**
	Memeriksa apakah terdapat salah satu segment
	di koordinat x,y.
	Return 0 artinya tidak bertumpuk, 1 artinya bertumpuk.
*/
int check(int x, int y)
{
	temp = head;
	while(temp!=NULL){
		if(temp->x == x && temp->y == y){
			return 1;
		}
		temp = temp->next;
	}
	return 0;

}
/**
	Taruh makanan secara acak, namun memastikan
	makanan tidak bertumpuk dengan salah satu segment ular
*/
void place_food()
{
	int x,y;
	do{
		food_x = rand()%(game_width-1) + 1;
		food_y = rand()%(game_height-1) + 1;
	}while(check(food_x,food_y) == 1);
}


void load(){
	int i;
	system("cls");
	printf("\n\n\t\tSilahkan tunggu");
	for(i=0;i<6;i++){
		Sleep(600);
		printf(".");
	}
	system("cls");
	for(i=0;i<5;i++){
		printf("\n\n\t\t%d",5-i);
		Sleep(1000);
		system("cls");
	}
	printf("\n\n\t\tPERMAINAN DIMULAI");
	Sleep(2000);
	system("cls");
}

int in_game(){
	int level = 1;
	int a,b;
	int i;
	int x,y;

	// Randomize
	srand(time(NULL));

	//set nilai high score dan score
	int score = 0;
	int high_score;
	high_score = skor_read();
	load();
	// Untuk menyimpan penanda waktu saat snake bergerak
	struct timeb last_timestamp;
	ftime(&last_timestamp);// Set nilai awal

	// Pertama-tama, push segment (node) ke kanan
	// sebanyak 3 segment (sesuai nilai variable snake_size)
	//tepat di koordinat x=0, dan y=24
	for(i = 1; i<=snake_size; i++)
	{
		push(i,15);
	}

	// Tempatkan makanan secara acak
	place_food();

	// Game loop. Bagian di dalam while akan dieksekusi terus menerus
	while (1)
	{
		// Ambil penanda waktu saat ini
		struct timeb current_timestamp;
		ftime(&current_timestamp);

		// Selisih waktu terakhir dengan waktu sekarang dalam ms
		int interval = 1000 * (current_timestamp.time - last_timestamp.time) + (current_timestamp.millitm - last_timestamp.millitm);

		// Snake bergerak setiap n ms (sesuai nilai variable snake_speed)
		// Dihitung dengan membandingkan selisih waktu sekarang dengan waktu
		// terakhir kali snake bergerak.
		if(interval > snake_speed)
		{

			// mentukan posisi x,y ke mana snake akan bergerak.
			// Posisi dilihat dari koordinat segment kepala (head)
			// dan arah (variable dir)
			switch(dir)
			{
				case VK_LEFT :
					x = head->x - 1;
					y = head->y;
				break;
				case VK_RIGHT :
					x = head->x + 1;
					y = head->y;
				break;
				case VK_UP :
					x = head->x;
					y = head->y - 1;
				break;
				case VK_DOWN :
					x = head->x;
					y = head->y + 1;
			}

			// Jika posisi kepala (head) menabrak tembok pembatas,
			// maka permainan berakhir (keluar dari game loop)
			if(x<1||x>=game_width||y<1||y>=game_height)
			{
				break;
			}

			// Jika posisi kepala (head) menabrak dirinya sendiri
			// (posisi sama dengan salah satu segment), maka permainan
			// berakhir (keluar dari game loop)
			if(check(x,y) == 1)
			{
				break;
			}

			// Jika tidak terjadi tabrakan (collision) dalam fungsi check(), maka snake
			// boleh bergerak maju..

			// Pop ekor, lalu push segment ke depan head sehingga
			// snake tampak bergerak maju.
            // Namun jika posisi x,y ke mana kepala (head) snake akan
            // bergerak berada di posisi makanan, tidak perlu pop
            // sehingga segment bertambah panjang.
			if(x==food_x && y == food_y)
			{
				score =  score+100;
				place_food();

				//jika score sudah mencapai kelipatan 300 maka kecepatan ular akan bertambah
				if(score%300 == 0&&score!=0)
				{
					snake_speed = snake_speed-10;
					level = level+1;
				}
			}
			else
			{
				pop();
			}
			push(x,y);

			// Tampilkan kondisi permainan saat ini di layar...

			// Bersihkan layar dan tampilkan arena permainan
			system("cls");
			printf("#########################################################\t\tGAME SNAKE\n");
			printf("#                                                       #\t\tLEVEL : %d\n",level);
			printf("#                                                       #\t\tSCORE : %d\n",score);
			printf("#                                                       #\t\tHIGH SCORE : %d\n",high_score);
			printf("#                                                       #\n");
			printf("#                                                       #\n");
			printf("#                                                       #\n");
			printf("#                                                       #\n");
			printf("#                                                       #\n");
			printf("#                                                       #\n");
			printf("#                                                       #\n");
			printf("#                                                       #\n");
			printf("#                                                       #\n");
			printf("#                                                       #\n");
			printf("#                                                       #\n");
			printf("#                                                       #\n");
			printf("#                                                       #\n");
			printf("#                                                       #\n");
			printf("#                                                       #\n");
			printf("#                                                       #\n");
			printf("#                                                       #\n");
			printf("#                                                       #\n");
			printf("#                                                       #\n");
			printf("#                                                       #\n");
			printf("#                                                       #\n");
			printf("#########################################################\n");

			// Cetak (render) snake di layar
			display();

			// Cetak (render) makanan di layar
			gotoxy(food_x,food_y);
			printf("@");

			// Perbarui penanda waktu
			last_timestamp = current_timestamp;
		}

		// Ubah arah jika tombol panah ditekan
		if(GetKeyState(VK_LEFT) < 0)
		{
			if(dir!=VK_RIGHT){
				dir = VK_LEFT;
			}
		}
		if(GetKeyState(VK_RIGHT) < 0)
		{
			if(dir!=VK_LEFT){
				dir = VK_RIGHT;
			}

		}
		if(GetKeyState(VK_UP) < 0)
		{
			if(dir!=VK_DOWN){
				dir = VK_UP;
			}
		}
		if(GetKeyState(VK_DOWN) < 0)
		{
			if(dir!=VK_UP){
				dir = VK_DOWN;
			}
		}
		if(GetKeyState(VK_ESCAPE) < 0)
		{
			return 0;
		}
	}
	system("cls");
	printf("GAME OVER!\n");
	printf("your score 	: %d\n",score);
	printf("HIGH SCORE	: %d\n",high_score);
	if(score>high_score)
	{
		printf("You Beat the high score!");
		skor_write(score);
	}
	else
	{
		printf("MAAF SKOR ANDA BELUM BISA MENGALAHKAN HIGHSCORE SAAT INI, COBA LAIN WAKTU");
	}
	pop_all();
	fflush(stdin);
	getch();

}
int main()
{
	int menu;
	header();
	in_game();
	system("pause");
return 0;
}
