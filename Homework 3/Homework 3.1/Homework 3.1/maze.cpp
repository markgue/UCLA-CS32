#include <string>
using namespace std;

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
	// Check if the start equals the end
	if (sr == er && sc == ec)
		return true;
	// Otherwise clear that spot and begin checking each direction if it is empty
	maze[sr][sc] = 'o';
	// South
	if (maze[sr + 1][sc] == '.' && pathExists(maze, nRows, nCols, sr + 1, sc, er, ec))
		return true;
	// North
	if (maze[sr - 1][sc] == '.' && pathExists(maze, nRows, nCols, sr - 1, sc, er, ec))
		return true;
	// East
	if (maze[sr][sc + 1] == '.' && pathExists(maze, nRows, nCols, sr, sc + 1, er, ec))
		return true;
	// West
	if (maze[sr][sc - 1] == '.' && pathExists(maze, nRows, nCols, sr, sc - 1, er, ec))
		return true;
	// The function has not found a path, so return false
	return false;
}