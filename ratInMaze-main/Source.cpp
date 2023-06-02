#include <iostream>
#include <vector>
#include <time.h>
#include <Windows.h>


#ifndef _WIN32_WINNT
#define _WIN32_WINNT 1537
#endif

#include <windows.h>
#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#define _WIN32_WINNT 1537
#endif
#include <WinCon.h>
#include <iostream>
//#include <fstream>
//#include <stdio.h>
//#include <stdlib.h>
#include <conio.h> 
#include "mygraphics.h"
#include "myconsole.h"


using namespace std;
bool SetColorAtPoint(int x, int y, int color)
{
	COORD c, size;
	c.X = x;
	c.Y = y;
	size.X = 1;
	size.Y = 1;

	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD w = color;
	unsigned long written = 0;
	WriteConsoleOutputAttribute(h, &w, 1, c, &written);

	if (written)
		return true;

	return false;
}
// Color Codes Hexadecimal values for both Fore Ground and Back Ground //ForeGround|BackGround
int B_BLACK = 0x00;	int B_Royal_Blue = 0x10;	int B_Dark_Green = 0x20;
int B_Tale_Blue = 0x30;	int B_Dark_Red = 0x40;	int B_Purple = 0x50;
int B_Tale_Green = 0x60;	int B_Light_Grey = 0x70; int B_Dark_Gray = 0x80;
int B_Light_Blue = 0x90; int B_Light_Green = 0xA0;	int B_Sky_Blue = 0xB0;
int B_Red = 0xC0;	int B_Pink = 0xD0;	int B_Yellow = 0xE0;
int B_White = 0xF0;

int F_BLACK = 0x00;	int F_Royal_Blue = 0x01; int F_Dark_Green = 0x02;
int F_Tale_Blue = 0x03;	int F_Dark_Red = 0x04;	int F_Purple = 0x05;
int F_Tale_Green = 0x06;	int F_Light_Grey = 0x07; int F_Dark_Gray = 0x08;
int F_Light_Blue = 0x09; int F_Light_Green = 0x0A;	int F_Sky_Blue = 0x0B;
int F_Red = 0x0C;	int F_Pink = 0x0D;	int F_Yellow = 0x0E;
int F_White = 0x0F;

class Maze;
class Dimensions
{
	friend class Maze;
	int x;
	int y;
public:
	Dimensions(){}
	Dimensions(const int a, const int b) :x{ a }, y{ b }{}
	void setValues(int a, int b)
	{
		x = a;
		y = b;
	}
};

class Maze
{
public:
	class Stack
	{
	public:
		class Node
		{
			Node* next;
			int data;
			friend class Stack;
		public:
			Node()
			{
				next = nullptr;
			}
			Node(int data, Node* next):data{data}, next{next}{}

		};
	private:
		Node* top;
	public:
		Stack()
		{
			top = nullptr;
		}
		bool isFull()
		{
			return false;
		}
		bool isEmpty()
		{
			return top == nullptr;
		}
		void push(int element)
		{
			Node* newNode = new Node(element, nullptr);
			if (top == nullptr)
			{
				top = newNode;
			}
			else
			{
				newNode->next = top;
				top = newNode;
			}
		}
		int pop()
		{
			int toReturn;
			if (!isEmpty())
			{
				Node* temp = top;
				toReturn = top->data;
				top = top->next;
				delete temp;
			}
			return toReturn;
		}
		int peek()
		{
			if (top == nullptr) return -1;
			return top->data;
		}

	};
	class Queue
	{
	public:
		class Node
		{
			Node* next;
			int data;
			friend class Queue;
		public:
			Node()
			{
				next = nullptr;
			}
			Node(int data, Node* next) :data{ data }, next{ next }{}

		};
	private:
		Node* front;
		Node* rear;
	public:
		Queue()
		{
			front = rear = nullptr;
		}
		bool isEmpty()
		{
			return front == nullptr;
		}
		bool isFull() {
			return false;
		}
		//adds values at the rear of the queue
		void enqueue(int value)
		{
			Node* newNode = new Node(value,nullptr);
			if(rear == nullptr)
			{
				front = rear = newNode;
			}
			else
			{
				rear->next = newNode;
				rear = newNode;
			}
		}
		//removes value at the front of the queue
		void dequeue(int& value)
		{
			if (front == nullptr)
				return;

			Node* temp = front;
			value = front->data;
			front = front->next;
			delete temp;
			if (front == nullptr)
				rear = nullptr;
		}

	};


private:

	int** maze;
	int rows;
	int cols;
	Dimensions source;
	Dimensions destination;
	Dimensions mouse;
public:
	Maze(int a, int b) :rows{ a }, cols{ b }
	{
		maze = new int*[rows];
		for (int i = 0; i < rows; ++i)
		{
			maze[i] = new int[cols] {};
		}
		
		int sourceX = 0, sourceY = 0, destinationX = 0, destinationY = 0;
		//    ! yahan pe check laganay hen for the destinations and the source !
		cout << "Enter the dimensions of the source of the mouse : ";
		cin >> sourceX >> sourceY;
		cout << "Enter the dimensions of the destination: ";
		cin >> destinationX >> destinationY;

		source.setValues(sourceX, sourceY);
		destination.setValues(destinationX, destinationY);
		mouse.setValues(sourceX, sourceY);
		
		printMaze();
		system("cls");
		launchMouseStackAlgorithm();


		//launchMouseLeeAlgorithm();
	}

	void placeHurdles(int numberOfHurdlesToBePlaced,int hurdleValue)
	{
		vector<int> xPositions;
		vector<int> yPositions;
		srand(time(0));

		for (int i = 0; i < numberOfHurdlesToBePlaced; ++i)
		{
			int x = rand() % rows;
			int y = rand() % cols;
			bool flag = false;
			for (auto a : xPositions)
			{
				if (a == x)
					flag = true;
			}
			for (auto a : xPositions)
			{
				if (a == y)
					flag = true;
			}
			if ((x == destination.x && y == destination.y)|| (x == source.x || y == source.y )) flag = true;
			
			if (!flag)
			{
				maze[x][y] = hurdleValue;
			}
			else
				--i;
		}
	}
	void myRects(int x1, int y1, int x2, int y2, int R, int G, int B)
	{
		HWND console_handle = GetConsoleWindow();
		HDC device_context = GetDC(console_handle);

		//change the color by changing the values in RGB (from 0-255)
		HPEN pen = CreatePen(PS_SOLID, 2, RGB(R, G, B));
		SelectObject(device_context, pen);
		HBRUSH brush = ::CreateSolidBrush(RGB(R, G, B)); //Fill color is passed as parameter to the function!!!
		SelectObject(device_context, brush);

		Rectangle(device_context, x1, y1, x2, y2);
		ReleaseDC(console_handle, device_context);
		DeleteObject(pen);
		DeleteObject(brush);
	}
	void printMaze()
	{
		int x = 500, y = 100;
		for (int i = 0; i < rows; ++i)
		{
			for (int j = 0; j < cols; ++j)
			{
				if(maze[i][j] == 0)
					myRects(x, y, x+30, y+30, 250, 153, 0);
				else if(maze[i][j] == 1)
					myRects(x, y, x + 30, y + 30, 0, 0, 255);
				else if(maze[i][j] == 2)
					myRects(x, y, x + 30, y + 30, 255,0 , 50);
				else if(maze[i][j] == 3)
					myRects(x, y, x + 30, y + 30, 255, 255, 0);
				else if(maze[i][j] == 5)
					myRects(x, y, x + 30, y + 30, 160, 32, 240);
				else if (maze[i][j] == 6)
					myRects(x, y, x + 30, y + 30, 51, 205,51 );
				x += 35;
			}
			y += 35;
			x = 500;
		}
	}
	void printTechnically()
	{
		_getch();
		system("cls");
		for (int i = 0; i < rows; ++i)
		{
			for (int j = 0; j < cols; ++j)
			{
				if (maze[i][j] == -2)
					cout << "A"<<" ";
				else
				cout << maze[i][j]<<" ";
			}
			cout << endl;

		}
		cout << endl;
	}
	void printForLeeAlgo( vector<int> xPath, vector<int> yPath)
	{
		system("cls");
		int x = 500, y = 100;
		for (int i = 0; i < rows; ++i)
		{
			for (int j = 0; j < cols; ++j)
			{
				bool flag = false;
				//first we will check for the path
				for (int k = 0; k < xPath.size(); ++k)
				{
					if (xPath[k] == i && yPath[k] == j)
					{
						flag = true;
						break;
					}
				}
				if (flag)
				{
					myRects(x, y, x + 30, y + 30, 51, 255, 251);

				}
				else if (i == mouse.x && j == mouse.y)
				{
					myRects(x, y, x + 30, y + 30, 51, 205, 51); 

				}
				else if (i == destination.x && j == destination.y)
				{
					myRects(x, y, x + 30, y + 30, 160, 32, 240);
				}
				else if (maze[i][j] != -2)
					myRects(x, y, x + 30, y + 30, 250, 153, 0);
				else if (maze[i][j] == -2)
					myRects(x, y, x + 30, y + 30, 0, 0, 255);

				/*if (maze[i][j] != -2)
				{


					myDrawText(x+10, y, 40, new char(char(maze[i][j]+48)), RGB(255, 255, 255), RGB(0, 0, 0));

				}*/
				/*else if (maze[i][j] == 2)
					myRects(x, y, x + 30, y + 30, 255, 0, 50);
				else if (maze[i][j] == 3)
					myRects(x, y, x + 30, y + 30, 255, 255, 0);
				else if (maze[i][j] == 5)
					myRects(x, y, x + 30, y + 30, 160, 32, 240);
				else if (maze[i][j] == 6)
					myRects(x, y, x + 30, y + 30, 51, 205, 51);*/
				x += 35;
			}
			y += 35;
			x = 500;
		}
	}
	void launchMouseLeeAlgorithm()
	{
		maze[destination.x][destination.y] = -1;
		placeHurdles(rows * cols / 3, -2);
		Queue currentX;
		Queue currentY;
		currentX.enqueue(mouse.x);
		currentY.enqueue(mouse.y);
		maze[mouse.x][mouse.y] = 0;
		bool isTargetEvaluated = false;
		int destX, destY;
		while (currentX.isEmpty() == false)
		{
			//first we will take the cell out of the queue
			int examineX;
			int examineY;
			currentX.dequeue(examineX);
			currentY.dequeue(examineY);
			//second we will mark the squares around the examine cell
			
			//up
			if (examineX > 0) 
			{
				if (maze[examineX - 1][examineY] == -1)
				{
					destX = examineX - 1;
					destY = examineY;
					maze[examineX - 1][examineY] = maze[examineX][examineY] + 1;
					isTargetEvaluated = true;
					break;
				}

				if (maze[examineX - 1][examineY] == 0 && !(examineX - 1 == mouse.x && mouse.y == examineY))
				{
					maze[examineX - 1][examineY] = maze[examineX][examineY] + 1;
					currentX.enqueue(examineX - 1);
					currentY.enqueue(examineY);
				}
				
				
			}
			if (examineX < rows-1 )
			{
				if (maze[examineX + 1][examineY] == -1)
				{
					destX = examineX + 1;
					destY = examineY;
					maze[examineX + 1][examineY] = maze[examineX][examineY] + 1;
					isTargetEvaluated = true;
					break;
				}
				if (maze[examineX + 1][examineY] == 0 && !(examineX + 1 == mouse.x && mouse.y == examineY))
				{
					maze[examineX + 1][examineY] = maze[examineX][examineY] + 1;
					currentX.enqueue(examineX + 1);
					currentY.enqueue(examineY);

				}
			}

			if (examineY > 0) 
			{
				if (maze[examineX][examineY-1] == -1)
				{
					destX = examineX ;
					destY = examineY- 1;
					maze[examineX][examineY - 1] = maze[examineX][examineY] + 1;
					isTargetEvaluated = true;
					break;
				}
				if (maze[examineX][examineY - 1] == 0 && !(examineX == mouse.x && mouse.y == examineY - 1))
				{
					currentX.enqueue(examineX);
					currentY.enqueue(examineY - 1);
					maze[examineX][examineY - 1] = maze[examineX][examineY] + 1;

				}
				
			}

			if (examineY <cols-1)
			{
				if (maze[examineX][examineY+1] == -1)
				{
					destX = examineX ;
					destY = examineY+1;
					maze[examineX][examineY + 1] = maze[examineX][examineY] + 1;
					isTargetEvaluated = true;
					break;
				}
				if (maze[examineX][examineY + 1] == 0 && !(examineX == mouse.x && mouse.y == examineY + 1))
				{
					maze[examineX][examineY + 1] = maze[examineX][examineY] + 1;
					currentX.enqueue(examineX);
					currentY.enqueue(examineY + 1);
				}
				
			}
		}
		if (isTargetEvaluated)
		{
			vector<int> pathX;
			vector<int> pathY;
			//here we will back track
			cout << destX << " " << destY << endl;
			pathX.push_back(destX);
			pathY.push_back(destY);
			int value = maze[destX][destY];
			for (int i=0;i<value;++i)
			{
				//up
				if (destX > 0 && maze[destX-1][destY] == maze[destX][destY] - 1)
				{
					pathX.push_back(destX-1);
					pathY.push_back(destY);
					destX--;
				}
				//right
				else if (destY < cols-1 && maze[destX][destY+1] == maze[destX][destY] - 1)
				{
					pathX.push_back(destX);
					pathY.push_back(destY+1);
					destY++;
				}
				//down
				else if (destX >rows-1 && maze[destX + 1][destY] == maze[destX][destY] - 1)
				{
					pathX.push_back(destX+1);
					pathY.push_back(destY);
					destX++;
				}
				//left
				else if (destY>0  && maze[destX ][destY-1] == maze[destX][destY] - 1)
				{
					pathX.push_back(destX );
					pathY.push_back(destY-1);
					destY--;
				}
			}
			/*for (int i = 0; i < value; ++i)
			{
				cout << "( " << pathX.pop() << ", " << pathY.pop() << " )";
			}
			cout << endl;*/
			printForLeeAlgo(pathX,pathY);

		}
		else
		{
			printTechnically();
			cout << "No path that ends up in the destination";
		}
	}
	void launchMouseStackAlgorithm()
	{
		maze[source.x][source.y] = 5;
		mouse = source;
		maze[destination.x][destination.y] = 6;
		placeHurdles(rows * cols / 3, 1);
		Stack xPositions;
		Stack yPositions;

		bool targetEvaluated = false;
		while(targetEvaluated == false)
		{ 
			
			if (mouse.x == destination.x && mouse.y == destination.y)
			{
				targetEvaluated= true;
				break;
			}
			//Now we will check the movability of the mouse
			
			int direction = rand() % 4;
			bool flag = false;
			while ((mouse.x < rows - 1) && (maze[mouse.x + 1][mouse.y] == 0||maze[mouse.x + 1][mouse.y] == 6))
			{
				xPositions.push(mouse.x);
				yPositions.push(mouse.y);
				flag = true;
				maze[mouse.x][mouse.y] = 3;
				mouse.x++;
				maze[mouse.x][mouse.y] = 5;

				Sleep(250);
				//system("cls");
				printMaze();
			}
			if (mouse.x == destination.x && mouse.y == destination.y)
			{
				targetEvaluated = true;
				break;
			}
			while( (mouse.x>0) && (maze[mouse.x-1][mouse.y] == 0 || maze[mouse.x - 1][mouse.y] == 6))
			{
				xPositions.push(mouse.x);
				yPositions.push(mouse.y);
				flag = true;
				maze[mouse.x][mouse.y] = 3;
				mouse.x--;
				maze[mouse.x][mouse.y] = 5;

				Sleep(250);
				//system("cls");
				printMaze();
			}
			if (mouse.x == destination.x && mouse.y == destination.y)
			{
				targetEvaluated = true;
				break;
			}
			while ((mouse.y > 0) && (maze[mouse.x][mouse.y-1] == 0 || maze[mouse.x ][mouse.y-1] == 6))
			{
				xPositions.push(mouse.x);
				yPositions.push(mouse.y);
				flag = true;
				maze[mouse.x][mouse.y] = 3;

				mouse.y--;
				maze[mouse.x][mouse.y] = 5;

				Sleep(250);
				//system("cls");
				printMaze();
			}
			if (mouse.x == destination.x && mouse.y == destination.y)
			{
				targetEvaluated = true;
				break;
			}
			while ((mouse.y < cols - 1) && (maze[mouse.x][mouse.y + 1] == 0 || maze[mouse.x ][mouse.y+1] == 6))
			{
				xPositions.push(mouse.x);
				yPositions.push(mouse.y);
				flag = true;
				maze[mouse.x][mouse.y] = 3;

				mouse.y++;
				maze[mouse.x][mouse.y] = 5;

				Sleep(250);
				//system("cls");
				printMaze();
				
			}
			if (flag == false)
			{
				
				maze[mouse.x][mouse.y] = 2;
				if (xPositions.isEmpty() || yPositions.isEmpty())
				{
					Sleep(500);
					system("cls");
					cout << "No path found for the mouse to reach the destination " << endl;
					break;
				}

				mouse.x = xPositions.pop();
				mouse.y = yPositions.pop();
			}
			maze[mouse.x][mouse.y] = 5;
			Sleep(250);
			//////system("cls");
			printMaze();
		}

	}
};


int main()
{

	int rows=0, cols=0;
	cout << "Enter the rows and the columns of the maze: ";
	cin >> rows>> cols;


	Maze ratInMaze{rows, cols};
	_getch();
	
	return 0;

}