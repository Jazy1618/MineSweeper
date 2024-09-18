/*
Jasmine Amani Murphy
Mine Sweeper
Using classes, recreate the game Mine
Sweeper. The player enters a coordinate
value, and if the coordinate is a bomb
on the field, they lose. Otherwise, the
tile shows how many bombs are around it,
or if it has no bomb neighbors, all the
tiles around it become visible and show
how many bombs are around them.
*/
#include <ctime>
#include <string>
#include <vector>
#include <cstdlib>
#include <iostream>
using namespace std;

struct Tile {
	bool bomb;
	bool visible;
	int bombCount = 0;
};

class Minesweeper {
public:
	Minesweeper(const int R, const int C) : ROWS(R), COLS(C) {
		const int BOMB_PROBABILITY = 15;
		srand(time(NULL));
		vector<Tile> nxtRow;
		for (size_t i = 0; i < ROWS; ++i) {
			for (size_t j = 0; j < COLS; ++j) {
				if ((rand() % 100) < BOMB_PROBABILITY) {
					Tile nxtTile;
					nxtTile.bomb = true;
					nxtTile.visible = false;
					nxtRow.push_back(nxtTile);
				}
				else {
					Tile nxtTile;
					nxtTile.bomb = false;
					nxtTile.visible = false;
					nxtRow.push_back(nxtTile);
				}
			}
			board.push_back(nxtRow);
			nxtRow.clear();
		}

		int count;
		for (size_t i = 0; i < ROWS; ++i) {
			for (size_t j = 0; j < COLS; ++j) {
				count = 0;
				for (int x = -1; x < 2; ++x) {
					for (int y = -1; y < 2; ++y) {
						if (validRow(i + x) && validCol(j + y)) {
							if (board[i + x][j + y].bomb == true) {
								count++;
							}
						}
					}
				}
				board[i][j].bombCount = count;
			}
		}
	}

	void display(bool bomb) {
		if (bomb == true) {
			for (size_t i = 0; i < board.size(); ++i) {
				for (size_t j = 0; j < board[i].size(); ++j) {
					if (board[i][j].bomb == true) {
						cout << "O ";
					}
					else if (board[i][j].bombCount == 0) {
						cout << "- ";
					}
					else {
						cout << board[i][j].bombCount << " ";
					}
				}
				cout << endl;
			}
		}
		else {
			for (size_t i = 0; i < board.size(); ++i) {
				for (size_t j = 0; j < board[i].size(); ++j) {
					if (board[i][j].visible == false) {
						cout << "X ";
					}
					else {
						if (board[i][j].bomb == true) {
							cout << "O ";
						}
						else {
							cout << board[i][j].bombCount << " ";
						}
					}
				}
				cout << endl;
			}
		}
		cout << endl;
	}

	bool done() {
		for (size_t i = 0; i < board.size(); ++i) {
			for (size_t j = 0; j < board[i].size(); ++j) {
				if ((board[i][j].bomb == false) && (board[i][j].visible == false)) {
					return false;
				}
			}
		}
		return true;
	}

	bool validCol(int col) {
		return (col >= 0) && (col < board.size());
	}

	bool validRow(int row) {
		return (row >= 0) && (row < board.size());
	}

	bool isVisible(int row, int col) {
		return board[row-1][col-1].visible;
	}

	bool play(int row, int col) {
		//int trueRow = row - 1;
		//int trueCol = col - 1;
		if (board[row][col].bomb == true) {
			board[row][col].visible = true;
			return false;
		}
		else if (board[row][col].bombCount > 0) {
			board[row][col].visible = true;
			return true;
		}
		else if (board[row][col].bombCount == 0) {
			board[row][col].visible = true;
			for (int i = -1; i < 2; ++i) {
				for (int j = -1; j < 2; ++j) {
					if (validRow(row + i) && validCol(col + j)) {
						if (board[row + i][col + j].visible == false) {
							play(row + i, col + j);
						}
					}
				}
			}

			return true;
		}
	}

private:
	const int ROWS;
	const int COLS;
	vector<vector<Tile>> board;
};


int main() {
	Minesweeper sweeper(10, 10);

	// Continue until only invisible cells are bombs
	while (!sweeper.done()) {
		sweeper.display(false); // display board without bombs
		int row_sel = -1, col_sel = -1;
		while (row_sel == -1) {
			// Get a valid move
			int r, c;
			cout << "row? ";
			cin >> r;
			if (!sweeper.validRow(r-1)) {
				cout << "Row out of bounds\n";
				continue;
			}
			cout << "col? ";
			cin >> c;
			if (!sweeper.validCol(c-1)) {
				cout << "Column out of bounds\n";
				continue;
			}
			if (sweeper.isVisible(r, c)) {
				cout << "Square already visible\n";
				continue;
			}
			row_sel = r-1;
			col_sel = c-1;
		}
		// Set selected square to be visible. May effect other cells.
		if (!sweeper.play(row_sel, col_sel)) {
			cout << "Sorry, you died..\n";
			sweeper.display(true); // Final board with bombs shown
			exit(0);
		}
	}
	// Ah! All invisible cells are bombs, so you won!
	cout << "You won!!!!\n";
	sweeper.display(true); // Final board with bombs shown
}