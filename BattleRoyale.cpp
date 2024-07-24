#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <iostream>
#include <string>
#include <fcntl.h>
#include <sys/stat.h>
#include <fstream>
#include <pthread.h>
using namespace std;

pthread_mutex_t lock;
int playing = 4;

class player{
public:
	int id;
	char sym;
	int att;
	int health;
	int position[2];
	
	player() {
		id = 0;
		sym = 0;
		att = 0;
		health = 0;
	}


};

class battlefield{
public:
	int num;
	player* plays;
	int p;
	int field[10][10];
	
	battlefield() {
		num = 4;
		plays = NULL;
		p = 0;
	}
	
	void initialize() {
		int x;
		cout << "\nEnter attack strength of all players: ";
		cin >> x;
		int y;
		cout << "\nEnter health of all players: ";
		cin >> y;
		plays = new player[num];
		for (int i=0; i<num; i++)
		{
			cout << "\nPlayer " << i+1 << " select your symbol: ";
			cin >> plays[i].sym;
			plays[i].id = i+1;
			plays[i].att = x;			
			plays[i].health = y;
		}
		cout << endl << endl;
		for (int i=0; i<10; i++)
		{
			for (int j=0; j<10; j++)
			{
				field[i][j] = 0;
			}
		}
		field[0][0] = 1;
		plays[0].position[0] = 0;
		plays[0].position[1] = 0;
		
		field[0][9] = 2;
		plays[1].position[0] = 0;
		plays[1].position[1] = 9;
		
		field[9][0] = 3;
		plays[2].position[0] = 9;
		plays[2].position[1] = 0;
		
		field[9][9] = 4;
		plays[3].position[0] = 9;
		plays[3].position[1] = 9;
	}
	
	void print() {
		cout << endl << endl;
		for (int i=0; i<10; i++)
		{
			for (int j=0; j<10; j++)
			{
				if (field[i][j] == 0)
					cout << " * ";
				else if (field[i][j] == 1)
					cout << " " << plays[0].sym << " ";
				else if (field[i][j] == 2)
					cout << " " << plays[1].sym << " ";
				else if (field[i][j] == 3)
					cout << " " << plays[2].sym << " ";
				else if (field[i][j] == 4)
					cout << " " << plays[3].sym << " ";
			}
			cout << endl;
		}
		cout << endl << endl;
	}
	
	bool movement(char in, int id)
	{
		if (in == 'W' || in == 'w')
		{
			if (plays[id-1].position[0] > 0)
			{
				int row = plays[id-1].position[0], col = plays[id-1].position[1];
				field[row][col] = 0;
				row--;
				field[row][col] = id;
				plays[id-1].position[0] = row;
				plays[id-1].position[1] = col;
				return true;
			}
			else
				return false;
		}
		
		else if (in == 'S' || in == 's')
		{
			if (plays[id-1].position[0] < 9)
			{
				int row = plays[id-1].position[0], col = plays[id-1].position[1];
				field[row][col] = 0;
				row++;
				field[row][col] = id;
				plays[id-1].position[0] = row;
				plays[id-1].position[1] = col;
				return true;
			}
			else
				return false;
		}
		
		else if (in == 'A' || in == 'a')
		{
			if (plays[id-1].position[1] > 0)
			{
				int row = plays[id-1].position[0], col = plays[id-1].position[1];
				field[row][col] = 0;
				col--;
				field[row][col] = id;
				plays[id-1].position[0] = row;
				plays[id-1].position[1] = col;
				return true;
			}
			else
				return false;
		}
		
		else if (in == 'D' || in == 'd')
		{
			if (plays[id-1].position[1] < 9)
			{
				int row = plays[id-1].position[0], col = plays[id-1].position[1];
				field[row][col] = 0;
				col++;
				field[row][col] = id;
				plays[id-1].position[0] = row;
				plays[id-1].position[1] = col;
				return true;
			}
			else
				return false;
		}
		
		else
			return false;
	}
	
	void attack(int id)
	{
		int row = plays[id-1].position[0], col = plays[id-1].position[1];
		int opp;
		
		if (row > 0 && col > 0 && field[row-1][col-1] != 0)
		{
			opp = field[row-1][col-1];
			plays[opp-1].health -= plays[id-1].att;
			if (plays[opp-1].health <= 0)
			{	
				cout << endl << plays[opp-1].id << " Terminated\n\n";
				playing--;
				field[row-1][col-1] = 0;
			}
		}
		if (row > 0 && field[row-1][col] != 0)
		{
			opp = field[row-1][col];
			plays[opp-1].health -= plays[id-1].att;
			if (plays[opp-1].health <= 0)
			{	
				cout << endl << plays[opp-1].id << " Terminated\n\n";
				playing--;
				field[row-1][col] = 0;
			}
		}
		if (row > 0 && col < 9 && field[row-1][col+1] != 0)
		{
			opp = field[row-1][col+1];
			plays[opp-1].health -= plays[id-1].att;
			if (plays[opp-1].health <= 0)
			{	
				cout << endl << plays[opp-1].id << " Terminated\n\n";
				playing--;
				field[row-1][col+1] = 0;
			}
		}
		
		
		if (col > 0 && field[row][col-1] != 0)
		{
			opp = field[row][col-1];
			plays[opp-1].health -= plays[id-1].att;
			if (plays[opp-1].health <= 0)
			{	
				cout << endl << plays[opp-1].id << " Terminated\n\n";
				playing--;
				field[row-1][col-1] = 0;
			}
		}
		if (col < 9 && field[row][col+1] != 0)
		{
			opp = field[row][col+1];
			plays[opp-1].health -= plays[id-1].att;
			if (plays[opp-1].health <= 0)
			{	
				cout << endl << plays[opp-1].id << " Terminated\n\n";
				playing--;
				field[row][col+1] = 0;
			}
		}
		
		
		if (row < 9 && col > 0 && field[row+1][col-1] != 0)
		{
			opp = field[row+1][col-1];
			plays[opp-1].health -= plays[id-1].att;
			if (plays[opp-1].health <= 0)
			{	
				cout << endl << plays[opp-1].id << " Terminated\n\n";
				playing--;
				field[row+1][col-1] = 0;
			}
		}
		if (row < 9 && field[row+1][col] != 0)
		{
			opp = field[row+1][col];
			plays[opp-1].health -= plays[id-1].att;
			if (plays[opp-1].health <= 0)
			{	
				cout << endl << plays[opp-1].id << " Terminated\n\n";
				playing--;
				field[row+1][col] = 0;
			}
		}
		if (row < 9 && col < 9 && field[row+1][col+1] != 0)
		{
			opp = field[row+1][col+1];
			plays[opp-1].health -= plays[id-1].att;
			if (plays[opp-1].health <= 0)
			{	
				cout << endl << plays[opp-1].id << " Terminated\n\n";
				playing--;
				field[row+1][col+1] = 0;
			}
		}
	}

};

battlefield b;
int np = 0;

void* func(void* x)
{
	pthread_mutex_lock(&lock);
	if (b.plays[np].health <= 0)
	{
		np++;
		if (np > 3)
			np = 0;
		pthread_mutex_unlock(&lock);
		pthread_exit(NULL);	
	}	
	
	cout << "Player " << np+1 << " turn\n";
	cout << "Health: " << b.plays[np].health << endl;
	b.print();
	char in;
	cout << "use WASD to move or press P to attack: ";
	cin >> in;
	if (in == 'p' || in == 'P')
	{
		cout << "Attacking...\n";
		b.attack(np+1);
		//b.print();
		cout << "use WASD to move: ";
		cin >> in;
		bool is = b.movement(in, np+1);
		while (!is)
		{
			cout << "use WASD to move: ";
			cin >> in;
			is = b.movement(in, np+1);
		}
	}
	else
	{
		bool is = b.movement(in, np+1);
		while (!is)
		{
			cout << "use WASD to move: ";
			cin >> in;
			is = b.movement(in, np+1);
		}
		b.print();
		cout << "Attacking...\n";
		b.attack(np+1);
	}
	np++;
	if (np > 3)
		np = 0;
	
	pthread_mutex_unlock(&lock);
	pthread_exit(NULL);
}


int main()
{
	pthread_mutex_init(&lock, NULL);
	b.initialize();
	b.print();		
	bool win = false;
	pthread_t tid1, tid2, tid3, tid4;
	while (!win)
	{
	
		pthread_create(&tid1, NULL, func, NULL);
		pthread_create(&tid2, NULL, func, NULL);
		pthread_create(&tid3, NULL, func, NULL);
		pthread_create(&tid4, NULL, func, NULL);
		
		if (playing == 1)
		{
			for (int i=0; i<4; i++)
			{
				if (b.plays[i].health > 0)
				{
					cout << "\nPlayer " << i+1 << " wins!\n\n\n";
					win = true;
					break;
				}
			}
		}
	}
}
