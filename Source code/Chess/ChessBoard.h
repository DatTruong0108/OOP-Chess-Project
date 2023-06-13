#pragma once

#include <SFML/Graphics.hpp>
#include "GameAudioPlayer.h"
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>
#include <math.h>
#include <stack>
#include "Moved.h"
#include "King.h"
#include "Queen.h"
#include "Bishop.h"
#include "Knight.h"
#include "Rook.h"
#include "Pawn.h"
#include "GameUser.h"
#include "GameAudioPlayer.h"
#include "ImageItem.h"
#include "TextureItem.h"

using namespace sf;
using namespace std;

class ChessBoard {
private:
    float _cellSize; //size of cell

    vector<pair<Vector2i, int>> Move; //save moves for replay later, Vector2i is the position, int is the time since last move

    bool _isPlayWithComputer = false; // use for bot
     
    bool _hasEndGame = false; // use to check is game vs bot end

    ImageItem _img; // get images

    TextureItem _texture; // load images to texture

    RenderWindow& _window; //game window


    vector<vector<Cell>> _cells; //the 2D vector of cells

    Vector2i _selectedPosition; //the position of the selected cell

    vector<ChessMove> _possibleMoves; //vector of possible moves

    GameUser _whiteUser; //white player

    GameUser _blackUser; //black player

    shared_ptr<GameUser> _currentUser; //current player

    ifstream _dataFileIn; //read moves

    ofstream _dataFileOut; //save moves
    
    stack<Moved> _moved;
    
    stack<Moved> _undone; //moves have been undone
    //the following methods are marked as private to ensure encapsulation
    //the others class will only know the run() and reset() methods
    //they will not know how they process
    void initializeBoard(); //initialize the board

    void populateBoard(); //populate the board

    void drawBoard(); //draw the board

    void showPossibleMoves(const vector<ChessMove>& positions); //show possible positions that can be moved to

    void movePiece(Cell& originCell, Cell& destinationCell); //move the piece from origin cell to destination cell

    void capturePiece(Cell& cell); //capture the piece in the cell

    void promotePiece(Cell& cell); //promote the Pawn

    void clearHighlightedCells(); //clear the highlighted cells

    void onMouseClicked(const Vector2i& position); //process the mouse click event

    void onOccupiedCellClicked(const Vector2i& position); //process the mouse click event on occupied cell

    void onHighlightedCellClicked(const Vector2i& position); //process the mouse click event on highlighted cell

    void onUndoCellClicked(); //process the mouse click event on undo button

    void onRedoCellClicked(); //process the mouse click event on redo button
protected:

public:
    enum {
        BOARD_WIDTH = 8,
        BOARD_HEIGHT = 8
    };

    ChessBoard(RenderWindow& window); //partly-parameterized constructor

    ChessBoard(const string& name, RenderWindow& window, bool pWC);

    // use for replay
    ChessBoard(const string& name, RenderWindow& window, const ImageItem& img, const TextureItem& text, const bool& pWC);

    ~ChessBoard(); //destructor

    void run(); //run the new chessboard

    void runResume(); //run the chessboard from the saved file

    void prepareForReplay();    //get ready to replay the game

    void replay(); //replay the game

    void reset(); //reset the chessboard
};