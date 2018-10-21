#include <string>
#include <iostream>
#include <stack>
using namespace std;

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec);
// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise

int main()
{
	string maze[10] = {
		"XXXXXXXXXX",
		"X...X..X.X",
		"X..XX....X",
		"X.X.XXXX.X",
		"XXX......X",
		"X...X.XX.X",
		"X.X.X..X.X",
		"X.XXXX.X.X",
		"X..X...X.X",
		"XXXXXXXXXX"
	};

	if (pathExists(maze, 10, 10, 4, 3, 1, 8))
		cout << "Solvable!" << endl;
	else
		cout << "Out of luck!" << endl;
}

class Coord
{
public:
	Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
	int r() const { return m_r; }
	int c() const { return m_c; }
private:
	int m_r;
	int m_c;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
	//  Push the starting coordinate(sr, sc) onto the coordinate stack and
	//	update maze[sr][sc] to indicate that the algorithm has encountered
	//	it(i.e., set maze[sr][sc] to have a value other than '.').
	stack<Coord> coordStack;

	coordStack.push(Coord(sr, sc));
	maze[sr][sc] = 'S';
	
	//	While the stack is not empty,
	while (!coordStack.empty())
	{
		//	{ Pop the top coordinate off the stack.This gives you the current
		//	(r,c) location that your algorithm is exploring.
		//	If the current(r,c) coordinate is equal to the ending coordinate,
		//	then we've solved the maze so return true! 
		Coord currentCoord = coordStack.top();
		coordStack.pop();
		int r = currentCoord.r();
		int c = currentCoord.c();
		cout << "(" << r << "," << c << ")" << endl;
		if (r == er && c == ec)
			return true;

		//	Check each place you can move from the current cell as follows :
		//  If you can move EAST and haven't encountered that cell yet,
		//	then push the coordinate(r,c + 1) onto the stack and update
		//	maze[r][c + 1] to indicate the algorithm has encountered it.
		if (maze[r][c + 1] == '.')
		{
			coordStack.push(Coord(r, c + 1));
			maze[r][c + 1] = 'o';
		}

		//	If you can move SOUTH and haven't encountered that cell yet,
		//	then push the coordinate(r + 1,c) onto the stack and update
		//	maze[r + 1][c] to indicate the algorithm has encountered it.
		if (maze[r + 1][c] == '.')
		{
			coordStack.push(Coord(r + 1, c));
			maze[r + 1][c] = 'o';
		}

		//	If you can move WEST and haven't encountered that cell yet,
		//	then push the coordinate(r,c - 1) onto the stack and update
		//	maze[r][c - 1] to indicate the algorithm has encountered it.
		if (maze[r][c - 1] == '.')
		{
			coordStack.push(Coord(r, c - 1));
			maze[r][c - 1] = 'o';
		}
		//	If you can move NORTH and haven't encountered that cell yet,
		//	then push the coordinate(r - 1,c) onto the stack and update
		//	maze[r - 1][c] to indicate the algorithm has encountered it.
		if (maze[r - 1][c] == '.')
		{
			coordStack.push(Coord(r - 1, c));
			maze[r - 1][c] = 'o';
		}
	}
	//	There was no solution, so return false
	return false;
}