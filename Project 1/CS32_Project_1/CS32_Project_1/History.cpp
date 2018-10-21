#include "History.h"

using namespace std;

History::History(int nRows, int nCols) 
{
	m_rows = nRows;
	m_cols = nCols;
	for (int i = 0; i < m_rows; i++)
		for (int j = 0; j < m_cols; j++)
			m_grid[i][j] = '.';

}

bool History::record(int r, int c)
{
	// Check if the row and col are valid
	if (r > m_rows || r < 1 || c > m_cols || c < 1)
		return false;

	if (m_grid[r - 1][c - 1] == '.')
		m_grid[r - 1][c - 1] = 'A';
	else if (m_grid[r - 1][c - 1] >= 'A' && m_grid[r - 1][c - 1] < 'Z')
		m_grid[r - 1][c - 1]++;
	
	return true;
}

void History::display() const
{
	clearScreen();
	for (int i = 0; i < m_rows; i++)
	{
		for (int j = 0; j < m_cols; j++)
			cout << m_grid[i][j];
		cout << endl;
	}
	cout << endl;
}