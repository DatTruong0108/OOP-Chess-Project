#include "ChessBoard.h"
//call the fully-paramerized constructor
ChessBoard::ChessBoard(RenderWindow& window) : ChessBoard("ChessBoard", window, true) {

}

ChessBoard::ChessBoard(const string& name, RenderWindow& window, bool pWC) : _window(window), _whiteUser(PieceColor::WHITE), _blackUser(PieceColor::BLACK) {
    //set the window title, frame limit, background and button's images
    this->_window.setFramerateLimit(120);

    this->_window.setTitle(name);

    this->_isPlayWithComputer = pWC;

    _img.setBackgroundImg("../resources/images/interface/main.png");
    _texture.setBackgroundTexture(_img.getBackgroundImg());

    _img.setUndoImg("../resources/images/interface/undo.png");
    _texture.setUndoTexture(_img.getUndoImg());

    _img.setRedoImg("../resources/images/interface/redo.png");
    _texture.setRedoTexture(_img.getRedoImg());

    _img.setReplayImg("../resources/images/interface/replay.png");
    _texture.setReplayTexture(_img.getReplayImg());

    _img.setExitImg("../resources/images/interface/exit.png");
    _texture.setExitTexture(_img.getExitImg());
}

// use for replay
ChessBoard::ChessBoard(const string& name, RenderWindow& window, const ImageItem& img, const TextureItem& text, const bool& pWC) : _window(window), _whiteUser(PieceColor::WHITE), _blackUser(PieceColor::BLACK) {
    this->_window.setFramerateLimit(120);

    this->_window.setTitle(name);

    this->_isPlayWithComputer = pWC;

    _img = img;

    _texture = text;
}

ChessBoard::~ChessBoard() {
    if (_dataFileOut.is_open())
        _dataFileOut.close();
}

// use for replay
//int replayMove = 0;
//clock_t waitMove = 0, endMove = 0;

RectangleShape undoButton(Vector2f(377.2257, 105.5901));
RectangleShape redoButton(Vector2f(377.2257, 105.5901));
RectangleShape replayButton(Vector2f(377.2257, 105.5901));
RectangleShape exitButton(Vector2f(377.2257, 105.5901));
Texture background;
//background = _texture.getBackgroundTexture();
//background.setSmooth(true);

//main function, process the window events and draw the board
void ChessBoard::run() {
    //Texture background;
    background = _texture.getBackgroundTexture();
    background.setSmooth(true);
    Sprite spriteBackground(background);
    spriteBackground.setScale(1600 / spriteBackground.getLocalBounds().width, 900 / spriteBackground.getLocalBounds().height);

    // resume game
    _dataFileOut.open("data.txt", ios_base::trunc);
    
    //load the sound
    utilities::AudioPlayer::loadSound();

    //reset the board
    this->reset();

    // undo button
    //RectangleShape undoButton(Vector2f(377.2257, 105.5901));
    undoButton.setPosition(1106, 231.5); // 1106 - 231.5 : undo button
    Texture undoTT = _texture.getUndoTexture();
    undoTT.setSmooth(true);
    undoButton.setTexture(&undoTT);

    // redo button
    //RectangleShape redoButton(Vector2f(377.2257, 105.5901));
    redoButton.setPosition(1106, 373.43); // 1106 - 373.43 : redo button
    Texture redoTT = _texture.getRedoTexture();
    redoTT.setSmooth(true);
    redoButton.setTexture(&redoTT);

    // replay button
    //RectangleShape replayButton(Vector2f(377.2257, 105.5901));
    replayButton.setPosition(1106, 515.36); // 1106 - 515.36 : replay button
    Texture replayTT = _texture.getReplayTexture();
    replayTT.setSmooth(true);
    replayButton.setTexture(&replayTT);

    // exit button
    //RectangleShape exitButton(Vector2f(377.2257, 105.5901));
    exitButton.setPosition(1106, 657.29); // 1106 - 657.29 : exit button
    Texture exitTT = _texture.getExitTexture();
    exitTT.setSmooth(true);
    exitButton.setTexture(&exitTT);

    //display the window
    this->_window.display();

    char undo, redo;
    fstream f;
    while (this->_window.isOpen() == true) {
        Event event;

        while (this->_window.pollEvent(event) == true) {
            //dataFileOut.open("data.txt", ios_base::out | ios_base::app);
            if (event.type == Event::Closed) {
                this->_window.close();
            }
            //exit by press escape key
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
                this->_window.close();
            }

            //process the mouse click
            if (Mouse::isButtonPressed(Mouse::Left)) {
                Vector2i position = Mouse::getPosition(this->_window);
                //get the time finish the move
                //endMove = clock();
                //save the move for replaying later
                //this->Move.push_back({ position, endMove - waitMove });
                //waitMove = endMove;
                this->onMouseClicked(position);

                if (undoButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    // write the clicked undo button to file
                    if (this->_dataFileOut.is_open())
                        this->_dataFileOut << position.x << " " << position.y << endl;
                    this->onUndoCellClicked();
                    // if play vs bot, undo twice for bot's move and player's move
                    if (this->_isPlayWithComputer && this->_hasEndGame == false)
                    {
                        this->_dataFileOut << position.x << " " << position.y << endl;
                        this->onUndoCellClicked();
                    }
                    if (this->_hasEndGame == true) this->_hasEndGame = false;
                }
                else if (redoButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    // write the clicked redo button to file
                    if (this->_dataFileOut.is_open())
                        this->_dataFileOut << position.x << " " << position.y << endl;
                    this->onRedoCellClicked();
                    // if play vs bot, redo twice for bot's move and player's move
                    if (this->_isPlayWithComputer)
                    {
                        this->_dataFileOut << position.x << " " << position.y << endl;
                        this->onRedoCellClicked();
                    }
                }
                else if (replayButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    this->prepareForReplay();
                    cout << "Replay finish.\n";
                    //waitMove = clock();
                    //replayMove = 0;
                }
                else if (exitButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    this->_window.close();
                }

            }

        }

        this->_window.clear(Color(150, 150, 150));

        this->_window.draw(spriteBackground);

        this->drawBoard();

        this->_window.draw(undoButton);
        this->_window.draw(redoButton);
        this->_window.draw(replayButton);
        this->_window.draw(exitButton);

        this->_window.display();
    }
}

void ChessBoard::runResume() {
    // resume game
    _img.setBackgroundImg("../resources/images/interface/main.png");
    _texture.setBackgroundTexture(_img.getBackgroundImg());
    //Texture background;
    background = _texture.getBackgroundTexture();
    background.setSmooth(true);
    Sprite spriteBackground(background);
    spriteBackground.setScale(1600 / spriteBackground.getLocalBounds().width, 900 / spriteBackground.getLocalBounds().height);

    _dataFileIn.open("data.txt");

    //load the sound
    utilities::AudioPlayer::loadSound();

    //reset the board
    this->reset();

    // undo button
    //RectangleShape undoButton(Vector2f(377.2257, 105.5901));
    undoButton.setPosition(1106, 231.5); // 1106 - 231.5 : undo button
    _img.setUndoImg("../resources/images/interface/undo.png");
    _texture.setUndoTexture(_img.getUndoImg());
    Texture undoTT = _texture.getUndoTexture();
    undoTT.setSmooth(true);
    undoButton.setTexture(&undoTT);

    // redo button
    //RectangleShape redoButton(Vector2f(377.2257, 105.5901));
    redoButton.setPosition(1106, 373.43); // 1106 - 373.43 : redo button
    _img.setRedoImg("../resources/images/interface/redo.png");
    _texture.setRedoTexture(_img.getRedoImg());
    Texture redoTT = _texture.getRedoTexture();
    redoTT.setSmooth(true);
    redoButton.setTexture(&redoTT);

    // replay button
    //RectangleShape replayButton(Vector2f(377.2257, 105.5901));
    replayButton.setPosition(1106, 515.36); // 1106 - 515.36 : replay button
    _img.setReplayImg("../resources/images/interface/replay.png");
    _texture.setReplayTexture(_img.getReplayImg());
    Texture replayTT = _texture.getReplayTexture();
    replayTT.setSmooth(true);
    replayButton.setTexture(&replayTT);

    // exit button
    //RectangleShape exitButton(Vector2f(377.2257, 105.5901));
    exitButton.setPosition(1106, 657.29); // 1106 - 657.29 : exit button
    _img.setExitImg("../resources/images/interface/exit.png");
    _texture.setExitTexture(_img.getExitImg());
    Texture exitTT = _texture.getExitTexture();
    exitTT.setSmooth(true);
    exitButton.setTexture(&exitTT);

    //display the window
    this->_window.display();

    char undo, redo;
    fstream f;
    while (this->_window.isOpen() == true) {
        Event event;

        while (this->_window.pollEvent(event) == true) {
            if (event.type == Event::Closed) {
                this->_window.close();
            }
            //exit by press escape key
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
                this->_window.close();
            }

            if (Mouse::isButtonPressed(Mouse::Left)) {
                Vector2i position = Mouse::getPosition(this->_window);
                //get the time finish the move
                //endMove = clock();
                //save the move for replaying later
                //this->Move.push_back({ position, endMove - waitMove });
                //waitMove = endMove;
                this->onMouseClicked(position);

                if (undoButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    // write the clicked undo button
                    if (this->_dataFileOut.is_open())
                        this->_dataFileOut << position.x << " " << position.y << endl;
                    this->onUndoCellClicked();
                    // if play with bots, undo twice for bot's move and player's move
                    if (this->_isPlayWithComputer && this->_hasEndGame == false)
                    {
                        this->_dataFileOut << position.x << " " << position.y << endl;

                        this->onUndoCellClicked();
                    }
                    if (this->_hasEndGame == true) this->_hasEndGame = false;
                }
                else if (redoButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    // write the clicked redo button
                    if (this->_dataFileOut.is_open())
                        this->_dataFileOut << position.x << " " << position.y << endl;
                    this->onRedoCellClicked();
                    // if play with bots, redo twice for bot's move and player's move
                    if (this->_isPlayWithComputer)
                    {

                        this->_dataFileOut << position.x << " " << position.y << endl;
                        this->onRedoCellClicked();
                    }
                }
                else if (replayButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    this->prepareForReplay();
                    cout << "Replay finish.\n";
                    //waitMove = clock();
                    //replayMove = 0;
                }
                else if (exitButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    this->_window.close();
                }

            }

        }

        this->_window.clear(Color(150, 150, 150));

        this->_window.draw(spriteBackground);

        this->drawBoard();

        this->_window.draw(undoButton);
        this->_window.draw(redoButton);
        this->_window.draw(replayButton);
        this->_window.draw(exitButton);

        this->_window.display();

        int fx, fy;
        bool checkPrevious = this->_isPlayWithComputer;
        if (_isPlayWithComputer) {
            _isPlayWithComputer = false;
        }

        //read file containing moves 
        if (_dataFileIn.is_open()) {
            while (!_dataFileIn.eof())
            {
                //system("pause");
                _dataFileIn >> fx >> fy;
                if (fx < 1106)
                    onMouseClicked(Vector2i(fx, fy));
                else if (fy < 373.43)
                    this->onUndoCellClicked();
                else
                    this->onRedoCellClicked();

                if (_dataFileIn.eof())
                    break;
            }
            _dataFileIn.close();
            _dataFileOut.open("data.txt", ios_base::app);
        }
        _isPlayWithComputer = checkPrevious;
    }
}

void ChessBoard::prepareForReplay()
{
    //create a new chessboard for replaying
    try {
        string name = "Replay game";

        RenderWindow window(VideoMode(900, 900), name);

        window.setFramerateLimit(120);
        window.setTitle(name);
        Image icon;
        icon.loadFromFile("../resources/images/interface/icon.png");
        window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

        ChessBoard chessboard(name, window, _img, _texture, false);

        //replay game
        _dataFileOut.close();
        chessboard.replay();
        _dataFileOut.open("data.txt", ios_base::app);
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return;
    }
    catch (...) {
        std::cerr << "Unknown error" << std::endl;
        return;
    }
}

void ChessBoard::replay() {
    //load the sound
    utilities::AudioPlayer::loadSound();

    //reset the board
    this->reset();

    while (this->_window.isOpen() == true) {
        Event event;
        while (this->_window.pollEvent(event) == true) {
            if (event.type == Event::Closed) {
                this->_window.close();
            }

            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
                this->_window.close();
            }

            //replay moves 
            
            this->_dataFileIn.open("data.txt");
            int fx, fy;
            bool checkPrevious = this->_isPlayWithComputer;
            this->_isPlayWithComputer = false;
            //read moves in file to replay
            if (this->_dataFileIn.is_open()) {
                cout << "Open\n";
                while (!this->_dataFileIn.eof())
                {
                    //system("pause");
                    this->_dataFileIn >> fx >> fy;
                    //cout << fx << " " << fy << " read\n";
                    //time between replay's moves
                    sleep(milliseconds(600));

                    if (fx < 1106)
                        onMouseClicked(Vector2i(fx, fy));
                    else if (fy < 373.43)
                        this->onUndoCellClicked();
                    else
                        this->onRedoCellClicked();
                    if (this->_dataFileIn.eof())
                        break;

                    this->_window.clear(Color(150, 150, 150));
                    this->drawBoard();
                    this->_window.display();
                }
                this->_dataFileIn.close();
            }
            else cout << "Cannot open!\n";
            this->_isPlayWithComputer = checkPrevious;
            return;
        }

        this->_window.clear(Color(150, 150, 150));

        this->drawBoard();

        this->_window.display();
    }
}

void ChessBoard::reset() {
    //reset the score
    this->_blackUser.setScore(0);
    this->_whiteUser.setScore(0);

    //clear pieces
    this->_blackUser.clearPieces();
    this->_whiteUser.clearPieces();

    //initialize and populate the board
    this->initializeBoard();

    this->populateBoard();
}

void ChessBoard::initializeBoard() {
    bool white = true;

    //specify the size of a cell
    utilities::Settings::updateCellSize(this->_window.getSize());

    float cellSize = utilities::Settings::getCellSize();

    //clear and resize the cells
    this->_cells.clear();
    this->_cells.resize(BOARD_HEIGHT);

    //BOARD_HEIGHT == BOARD_WIDTH
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        this->_cells[i].resize(BOARD_WIDTH);

        for (int j = 0; j < BOARD_WIDTH; j++) {
            this->_cells[i][j]._rect.setSize(Vector2f(cellSize, cellSize));

            //determine the color of the cell
            Color colorToFill;
            if (white == true) {
                colorToFill = utilities::Settings::getWhiteSquareColor();
            }
            else {
                colorToFill = utilities::Settings::getBlackSquareColor();
            }

            //process with RectangleShape class of SFML
            this->_cells[i][j]._rect.setFillColor(colorToFill);

            this->_cells[i][j]._rect.setPosition(utilities::assisstants::convertToDrawablePosition(i, j));

            this->_cells[i][j]._checkedRect = this->_cells[i][j]._rect;

            this->_cells[i][j]._checkedRect.setFillColor(utilities::Settings::getCheckedColor());

            this->_cells[i][j]._status = CellStatus::EMPTY;

            this->_cells[i][j]._highlightedCircle.setFillColor(utilities::Settings::getHighlightedColor());

            this->_cells[i][j]._highlightedCircle.setOutlineColor(utilities::Settings::getHighlightedColor());

            this->_cells[i][j]._highlightedCircle.setRadius(cellSize / 2.00);

            this->_cells[i][j]._highlightedCircle.setOrigin(cellSize / 2.00, cellSize / 2.00);

            Vector2f pos = utilities::assisstants::convertToDrawablePosition(i, j);
            this->_cells[i][j]._highlightedCircle.setPosition(pos.x + cellSize / 2.00, pos.y + cellSize / 2.00);

            //switch the color of the cell
            white = !white;
        }

        //switch the color of the cell
        //at the end of each line, we need to switch the color again
        white = !white;
    }
}

//populate the board, aka place the pieces to their positions
void ChessBoard::populateBoard() {
    if (this->_cells.empty() == true) {
        throw std::runtime_error("Cannot populate chess board because it has not been initialized yet!\n");
    }

    Sprite sprite;

    float cellSize = utilities::Settings::getCellSize();
    vector<shared_ptr<Piece>> pieces = {
        make_shared<Rook>(Rook(PieceColor::BLACK)),
        make_shared<Knight>(Knight(PieceColor::BLACK)),
        make_shared<Bishop>(Bishop(PieceColor::BLACK)),
        make_shared<Queen>(Queen(PieceColor::BLACK)),
        make_shared<King>(King(PieceColor::BLACK)),
        make_shared<Bishop>(Bishop(PieceColor::BLACK)),
        make_shared<Knight>(Knight(PieceColor::BLACK)),
        make_shared<Rook>(Rook(PieceColor::BLACK)),

        make_shared<Rook>(Rook(PieceColor::WHITE)),
        make_shared<Knight>(Knight(PieceColor::WHITE)),
        make_shared<Bishop>(Bishop(PieceColor::WHITE)),
        make_shared<Queen>(Queen(PieceColor::WHITE)),
        make_shared<King>(King(PieceColor::WHITE)),
        make_shared<Bishop>(Bishop(PieceColor::WHITE)),
        make_shared<Knight>(Knight(PieceColor::WHITE)),
        make_shared<Rook>(Rook(PieceColor::WHITE))
    };
    for (int i = 0; i < pieces.size(); i++) {
        //variable row only has the value of 0 or 7
        int row = (i / 8) * 7;
        int col = i % 8;

        for (int j = 0; j <= 1; j++) {
            if (j == 0) {
                this->_cells[row][col]._piece = pieces[i];
            }
            else if (row == 0) {
                row++;
                this->_cells[row][col]._piece = make_shared<Pawn>(Pawn(PieceColor::BLACK));
            }
            else if (row != 0) {
                row--;
                this->_cells[row][col]._piece = make_shared<Pawn>(Pawn(PieceColor::WHITE));

            }

            this->_cells[row][col]._status = CellStatus::OCCUPIED;

            this->_cells[row][col]._piece->getPieceSprite().setPosition(this->_cells[row][col]._rect.getPosition());

            //add the pieces to the appropriate players
            if (this->_cells[row][col]._piece->getPieceColor() == PieceColor::WHITE) {
                this->_whiteUser.addPiece(this->_cells[row][col]._piece->getPieceType(), this->_cells[row][col]._piece);
            }
            else {
                this->_blackUser.addPiece(this->_cells[row][col]._piece->getPieceType(), this->_cells[row][col]._piece);
            }
        }
    }

    //set the current user to white user
    this->_currentUser = make_shared<GameUser>(this->_whiteUser);
}

void ChessBoard::drawBoard() {
    for (int i = 0; i < BOARD_WIDTH; i++) {
        for (int j = 0; j < BOARD_HEIGHT; j++) {
            //draw the cell rectangle (square actually)
            this->_window.draw(this->_cells[i][j]._rect);

            //check the king's cell if we detect a check
            if (this->_cells[i][j].getIsChecked() == true) {
                this->_window.draw(this->_cells[i][j]._checkedRect);
            }

            //draw the highlighted circle if the cell is highlighted
            if (this->_cells[i][j]._status == CellStatus::HIGHLIGHTED) {
                this->_window.draw(this->_cells[i][j]._highlightedCircle);
            }

            //draw the piece if there is one
            if (this->_cells[i][j]._piece.get() != nullptr) {
                this->_cells[i][j]._piece->draw(this->_window);
            }
        }
    }
}

void ChessBoard::showPossibleMoves(const vector<ChessMove>& positions) {
    for (int i = 0; i < positions.size(); i++) {
        this->_cells[positions[i]._position.x][positions[i]._position.y].highlight(true);
    }
}

void ChessBoard::movePiece(Cell& originCell, Cell& destinationCell) {
    //check if the board is empty or the origin cell is empty
    if (this->_cells.empty() == true) {
        throw std::runtime_error("Cannot move pieces because the board is empty");
    }
    else if (originCell._status == CellStatus::EMPTY) {
        throw std::runtime_error("Cannot move an unexisted piece");
    }
    //if player make a move, clear stack
    while (!this->_undone.empty())
        this->_undone.pop();


    Vector2i originIndex = utilities::assisstants::convertToBoardIndex(originCell._rect.getPosition());
    Vector2i destinationIndex = utilities::assisstants::convertToBoardIndex(destinationCell._rect.getPosition());

    //push moves to stack
    this->_moved.push(Moved());
    this->_moved.top().setOri(originIndex);
    this->_moved.top().setDes(destinationIndex);
    this->_moved.top().setPieceColor(originCell._piece->getPieceColor());
    this->_moved.top().setHasMoved(originCell._piece->getHasMoved());

    //check if the pawn moves 2 steps
    if (originCell._piece->getPieceType() == PieceType::PAWN) {
        if (abs(destinationIndex.x - originIndex.x) == 2 && destinationIndex.y == originIndex.y) {
            originCell._piece->setHasDoubleMoved(true);
        }
    }

    //if it is not a chess move to row 3 of black pawn and row 4 of white pawn, reset the double move flag
    if (!(originCell._piece->getPieceType() == PieceType::PAWN && originCell._piece->getPieceColor() == PieceColor::BLACK && destinationIndex.x == 3)
        && !(originCell._piece->getPieceType() == PieceType::PAWN && originCell._piece->getPieceColor() == PieceColor::WHITE && destinationIndex.x == 4)) {
        for (int i = 0; i < BOARD_WIDTH; i++) {
            for (int j = 0; j < BOARD_HEIGHT; j++) {
                if (this->_cells[i][j]._piece.get() != nullptr && this->_cells[i][j]._piece->getPieceType() == PieceType::PAWN) {
                    this->_cells[i][j]._piece->setHasDoubleMoved(false);
                }
            }
        }
    }
    //check that the destination is valid,
    //aka it exists in the possible moves vector
    ChessMove requiredMove;
    requiredMove._position.x = -1;
    requiredMove._position.y = -1;

    for (const auto& move : this->_possibleMoves) {
        if (move._position == destinationIndex) {
            requiredMove = move;
            break;
        }
    }

    if (requiredMove._position.x == -1 || requiredMove._position.y == -1) {
        throw std::runtime_error("Cannot move to an invalid position");
    }


    //capture the piece at destination if possible
    if (destinationCell._piece.get() != nullptr) {
        this->_moved.top().setIsCapture(true);
        this->_moved.top().setCapturedPiece(destinationCell._piece->getPieceType());
        this->_moved.top().setPiece(destinationCell._piece);
        this->capturePiece(destinationCell);

        utilities::AudioPlayer::playSound(GameSound::CAPTURE);
    }
    else {
        this->_moved.top().setIsCapture(false);
        utilities::AudioPlayer::playSound(GameSound::MOVE);
    }


    //process the pawn promotion, short castiling and long castling
    int row;
    if (this->_currentUser->getColor() == PieceColor::WHITE) {
        row = 7;
    }
    else {
        row = 0;
    }
    this->_moved.top().setMoveType(requiredMove._type);

    if (requiredMove._type == MoveType::PROMOTION) {
        this->_moved.top().setPawn(originCell._piece);
        this->_currentUser->removePiece(originCell._piece->getPieceType(), originCell._piece);
        this->promotePiece(originCell);
        this->_moved.top().setSpecialPiece(originCell._piece->getPieceType());
        this->_currentUser->addPiece(originCell._piece->getPieceType(), originCell._piece);
    }
    else if (requiredMove._type == MoveType::LONG_CASTLING) {
        this->_cells[row][3]._piece = std::move(this->_cells[row][0]._piece);

        this->_cells[row][3]._piece->getPieceSprite().setPosition(this->_cells[row][3]._rect.getPosition());

        this->_cells[row][3]._piece->markAsMoved();

        this->_cells[row][3]._status == CellStatus::OCCUPIED;
        this->_cells[row][0]._status == CellStatus::EMPTY;
    }
    else if (requiredMove._type == MoveType::SHORT_CASTLING) {
        this->_cells[row][5]._piece = std::move(this->_cells[row][7]._piece);

        this->_cells[row][5]._piece->getPieceSprite().setPosition(this->_cells[row][5]._rect.getPosition());

        this->_cells[row][5]._piece->markAsMoved();
        this->_cells[row][5]._status == CellStatus::OCCUPIED;
        this->_cells[row][7]._status == CellStatus::EMPTY;
    }
    else if (requiredMove._type == MoveType::EN_PASSANT) {
        this->_moved.top().setSpecialPiece(this->_cells[originIndex.x][destinationIndex.y]._piece->getPieceType());
        this->_moved.top().setPawn(this->_cells[originIndex.x][destinationIndex.y]._piece);
        this->capturePiece(this->_cells[originIndex.x][destinationIndex.y]);
        this->_cells[originIndex.x][destinationIndex.y]._status = CellStatus::EMPTY;
    }
    else if (requiredMove._type == MoveType::NONE) {
        //do nothing;
    }

    //move the piece to the destination
    destinationCell._piece = std::move(originCell._piece);

    //update the position of piece
    destinationCell._piece->getPieceSprite().setPosition(destinationCell._rect.getPosition());

    //specify the moved piece
    destinationCell._piece->markAsMoved();

    //update the status of cells
    originCell._status = CellStatus::EMPTY;
    destinationCell._status = CellStatus::OCCUPIED;
}

void ChessBoard::capturePiece(Cell& cell) {
    if (this->_cells.empty() == true) {
        throw std::runtime_error("Cannot capture pieces because the board is empty");
    }
    else if (cell._status == CellStatus::EMPTY) {
        throw std::runtime_error("Cannot capture an unexisted piece");
    }

    this->_currentUser->setScore(this->_currentUser->getScore() + cell._piece->getPieceValue());

    GameUser user = this->_currentUser->getColor() == PieceColor::WHITE ? this->_blackUser : this->_whiteUser;
    user.removePiece(cell._piece->getPieceType(), cell._piece);
    //remove the captured piece;
    cell._piece = nullptr;
}

void ChessBoard::promotePiece(Cell& cell) {
    // load background
    float width = 3858 / 5;
    float height = 1316 / 5;
    RenderWindow window(VideoMode(width, height), "Promotion");
    window.setFramerateLimit(120);
    Image icon;
    icon.loadFromFile("../resources/images/interface/icon.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    Image background;
    background.loadFromFile("../resources/images/interface/promotion.png");
    Texture texture;
    texture.loadFromImage(background);
    texture.setSmooth(true);
    Sprite sprite(texture);
    sprite.setScale(width / texture.getSize().x, height / texture.getSize().y);
    
    vector<shared_ptr<Piece>> pieces = {
    make_shared<Queen>(Queen(this->_currentUser->getColor())),
    make_shared<Rook>(Rook(this->_currentUser->getColor())),
    make_shared<Bishop>(Bishop(this->_currentUser->getColor())),
    make_shared<Knight>(Knight(this->_currentUser->getColor()))
    };

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
                break;
            }
            // choose type of piece to promote
            if (event.type == Event::MouseButtonPressed) {
                if (event.mouseButton.button == Mouse::Left) {
                    Vector2i mousePos = Mouse::getPosition(window);
                    int selectedRow = mousePos.x / 192;
                    if (this->_dataFileOut.is_open())
                        this->_dataFileOut << mousePos.x << " " << mousePos.y << endl;
                    if (selectedRow >= 0 && selectedRow < pieces.size()) {
                        cell._piece = pieces[selectedRow];
                        window.close();
                    }
                }
            }
        }
        
        window.draw(sprite);
        for (int i = 0; i < pieces.size(); i++) {
            RectangleShape rectangle(Vector2f(192, 192));
            rectangle.setPosition(192 * i + 40, 100);
            
            pieces[i]->getPieceSprite().setPosition(rectangle.getPosition());
            
            pieces[i]->draw(window);
        }
        
        window.display();
        // if replaying or resuming, read the choice in file
        if (this->_dataFileIn.is_open())
        {
            int fx, fy;
            this->_dataFileIn >> fx >> fy;
            cell._piece = pieces[fx / 200];
            break;
        }
    }
}
void ChessBoard::clearHighlightedCells() {
    for (auto&& row : this->_cells) {
        for (auto&& cell : row) {
            cell.highlight(false);
        }
    }
}

void ChessBoard::onMouseClicked(const Vector2i& position) {
    //check if the position is on the window
    if (utilities::assisstants::isOnWindowPosition(position) == false) {
        return;
    }

    Vector2i cellPosition(position.y / utilities::Settings::getCellSize(), position.x / utilities::Settings::getCellSize());

    //check the status and call the appropriate function
    if (this->_cells[cellPosition.x][cellPosition.y]._status == CellStatus::HIGHLIGHTED) {
        if (_dataFileOut.is_open())
            _dataFileOut << position.x << " " << position.y << endl;
        this->onHighlightedCellClicked(cellPosition);
    }
    else if (this->_cells[cellPosition.x][cellPosition.y]._status == CellStatus::OCCUPIED) {
        if (_dataFileOut.is_open() && this->_currentUser->getColor() == this->_cells[cellPosition.x][cellPosition.y]._piece->getPieceColor())
            _dataFileOut << position.x << " " << position.y << endl;
        this->onOccupiedCellClicked(cellPosition);
    }
    else if (this->_cells[cellPosition.x][cellPosition.y]._status == CellStatus::EMPTY) {
        this->clearHighlightedCells();
    }
}

void ChessBoard::onOccupiedCellClicked(const Vector2i& position) {
    this->clearHighlightedCells();

    //if the player's color is not the same as the clicked piece, return
    if (this->_currentUser->getColor() != this->_cells[position.x][position.y]._piece->getPieceColor()) {
        return;
    }



    this->_selectedPosition = Vector2i(position.x, position.y);

    //filter the possible moves based on the checks
    vector<vector<Cell>> copyCells;
    this->_possibleMoves.clear();

    // get possible moves of the clicked piece
    vector<ChessMove> moves = this->_cells[position.x][position.y]._piece->computePossbibleMoves(this->_cells);

    int row;
    if (this->_currentUser->getColor() == PieceColor::WHITE) {
        row = 7;
    }
    else {
        row = 0;
    }

    bool isShortCastling = false;
    bool isLongCastling = false;

    for (int i = 0; i < moves.size(); i++) {
        if (moves[i]._type == MoveType::SHORT_CASTLING || moves[i]._type == MoveType::LONG_CASTLING) {
            if (this->_currentUser->isChecked(this->_cells) == true) {
                continue;
            }
        }

        copyCells = this->_cells;
        copyCells[moves[i]._position.x][moves[i]._position.y]._piece = nullptr;
        copyCells[moves[i]._position.x][moves[i]._position.y]._piece = std::move(copyCells[position.x][position.y]._piece);
        copyCells[position.x][position.y]._status = CellStatus::EMPTY;
        copyCells[moves[i]._position.x][moves[i]._position.y]._status = CellStatus::OCCUPIED;

        if (this->_currentUser->isChecked(copyCells) == false) {
            if (moves[i]._position == Vector2i(row, 5)) {
                isShortCastling = true;
            }
            else if (moves[i]._position == Vector2i(row, 3)) {
                isLongCastling = true;
            }
            else if (moves[i]._type == MoveType::SHORT_CASTLING && isShortCastling == false) {
                continue;
            }
            else if (moves[i]._type == MoveType::LONG_CASTLING && isLongCastling == false) {
                continue;
            }

            this->_possibleMoves.push_back(moves[i]);
        }
    }

    this->showPossibleMoves(this->_possibleMoves);
}

void ChessBoard::onHighlightedCellClicked(const Vector2i& position) {
    Sprite spriteBackground(background);
    spriteBackground.setScale(1600 / spriteBackground.getLocalBounds().width, 900 / spriteBackground.getLocalBounds().height);
    //remove any possible checks for the current user before ending turn
    this->_currentUser->checkKingCell(false, this->_cells);

    this->movePiece(this->_cells[this->_selectedPosition.x][this->_selectedPosition.y], this->_cells[position.x][position.y]);

    this->clearHighlightedCells();

    //change the current user
    GameUser user = this->_currentUser->getColor() == PieceColor::WHITE ? this->_blackUser : this->_whiteUser;
    this->_currentUser = make_shared<GameUser>(user);

    bool hasMoves = this->_currentUser->hasLegalMoves(this->_cells);

    //if current player is checked and have no possible moves
    //-> the left player wins
    if (this->_currentUser->isChecked(this->_cells) == true) {
        this->_currentUser->checkKingCell(true, this->_cells);
        utilities::AudioPlayer::playSound(GameSound::CHECK);

        if (hasMoves == false) {
            utilities::AudioPlayer::playSound(GameSound::CHECKMATE);
            if (this->_currentUser->getColor() == PieceColor::WHITE) {
                cout << "Checkmate! Black wins!\n";
                RenderWindow window(VideoMode(800, 450), "Checkmate!");
                Image icon;
                icon.loadFromFile("../resources/images/interface/icon.png");
                window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
                Image image;
                image.loadFromFile("../resources/images/interface/blackwin.png");
                Texture texture;
                texture.loadFromImage(image);
                texture.setSmooth(true);
                Sprite sprite(texture);
                sprite.setScale(800 / sprite.getLocalBounds().width, 450 / sprite.getLocalBounds().height);
                while (window.isOpen())
                {
                    Event event;
                    while (window.pollEvent(event))
                    {
                        if (event.type == Event::Closed)
                            window.close();
                    }

                    this->_window.clear(Color(150, 150, 150));

                    this->_window.draw(spriteBackground);

                    this->drawBoard();

                    this->_window.draw(undoButton);
                    this->_window.draw(redoButton);
                    this->_window.draw(replayButton);
                    this->_window.draw(exitButton);

                    this->_window.display();

                    window.clear();
                    window.draw(sprite);
                    window.display();
                }
                this->_hasEndGame = true;
                return;
            }
            else {
                cout << "Checkmate! White wins!\n";
                RenderWindow window(VideoMode(800, 450), "Checkmate!");
                Image icon;
                icon.loadFromFile("../resources/images/interface/icon.png");
                window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
                Image image;
                image.loadFromFile("../resources/images/interface/whitewin.png");
                Texture texture;
                texture.loadFromImage(image);
                texture.setSmooth(true);
                Sprite sprite(texture);
                sprite.setScale(800 / sprite.getLocalBounds().width, 450 / sprite.getLocalBounds().height);
                while (window.isOpen())
                {
                    Event event;
                    while (window.pollEvent(event))
                    {
                        if (event.type == Event::Closed)
                            window.close();
                    }

                    this->_window.clear(Color(150, 150, 150));

                    this->_window.draw(spriteBackground);

                    this->drawBoard();

                    this->_window.draw(undoButton);
                    this->_window.draw(redoButton);
                    this->_window.draw(replayButton);
                    this->_window.draw(exitButton);

                    this->_window.display();

                    window.clear();
                    window.draw(sprite);
                    window.display();
                }
                this->_hasEndGame = true;
                return;
            }
        }
    }
    // if current player is not checked and cannot move any steps
    // -> stalemate
    else {
        this->_currentUser->checkKingCell(false, this->_cells);

        if (hasMoves == false) {
            utilities::AudioPlayer::playSound(GameSound::STALEMATE);
            cout << "Stalemate!\n";
            RenderWindow window(VideoMode(800, 450), "Stalemate!");
            Image icon;
            icon.loadFromFile("../resources/images/interface/icon.png");
            window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
            Image image;
            image.loadFromFile("../resources/images/interface/stalemate.png");
            Texture texture;
            texture.loadFromImage(image);
            texture.setSmooth(true);
            Sprite sprite(texture);
            sprite.setScale(800 / sprite.getLocalBounds().width, 450 / sprite.getLocalBounds().height);
            while (window.isOpen())
            {
                Event event;
                while (window.pollEvent(event))
                {
                    if (event.type == Event::Closed)
                        window.close();
                }

                this->_window.clear(Color(150, 150, 150));

                this->_window.draw(spriteBackground);

                this->drawBoard();
                
                this->_window.draw(undoButton);
                this->_window.draw(redoButton);
                this->_window.draw(replayButton);
                this->_window.draw(exitButton);

                this->_window.display();

               

                window.clear();
                window.draw(sprite);
                window.display();
            }
            this->_hasEndGame = true;
        }
    }
    
    //if playing with bot and current player's color is black
    //-> bot's turn
    if (this->_isPlayWithComputer == true && this->_currentUser->getColor() == PieceColor::BLACK) {
        while (true) {
            //take a random piece of bot
            shared_ptr<Piece> randomPiece = this->_currentUser->getRandomPiece();
            Vector2i pos = randomPiece->getPosition();
            //get possible moves of the piece has been randomized before
            vector<ChessMove> posibleMove = randomPiece->computePossbibleMoves(this->_cells);
            //if there is not exists any piece's possible moves
            //-> reloop to get another piece
            if (posibleMove.size() == 0)
                continue;

            // random a move
            Vector2i posD = posibleMove[rand() % posibleMove.size()]._position;
            vector<vector<Cell>> copyCells = this->_cells;
            copyCells[posD.x][posD.y]._piece = nullptr;
            copyCells[posD.x][posD.y]._piece = std::move(copyCells[pos.x][pos.y]._piece);
            copyCells[pos.x][pos.y]._status = CellStatus::EMPTY;
            copyCells[posD.x][posD.y]._status = CellStatus::OCCUPIED;
            //if bot is checked after the random move
            //-> reloop to get another move
            if (this->_currentUser->isChecked(copyCells) == true)
            {
                continue;
            }
            this->onOccupiedCellClicked(pos);
            this->_dataFileOut << pos.y * utilities::Settings::getCellSize() << " " << pos.x * utilities::Settings::getCellSize() << endl;
            //cout << pos.x << " " << pos.y << endl;
            this->onHighlightedCellClicked(posD);
            this->_dataFileOut << posD.y * utilities::Settings::getCellSize() << " " << posD.x * utilities::Settings::getCellSize() << endl;
            //cout << posD.x << " " << posD.y << endl;
            break;
        }
    }
}

void ChessBoard::onUndoCellClicked()
{
    if (this->_moved.empty())
        return;
    Moved lastmove = this->_moved.top();
    this->_moved.pop();
    this->_undone.push(lastmove); // push undo's moves to stack

    Vector2i origin = lastmove.getOri();
    Vector2i destination = lastmove.getDes();
    GameUser* nextUser = lastmove.getColor() == PieceColor::WHITE ? &this->_blackUser : &this->_whiteUser;
    GameUser* curUser = lastmove.getColor() == PieceColor::BLACK ? &this->_blackUser : &this->_whiteUser;

    //move piece to initial position
    this->_cells[origin.x][origin.y]._piece = std::move(this->_cells[destination.x][destination.y]._piece);

    this->_cells[origin.x][origin.y]._piece->getPieceSprite().setPosition(this->_cells[origin.x][origin.y]._rect.getPosition());
    if (lastmove.getHasMoved() == true)
    {
        this->_cells[origin.x][origin.y]._piece->markAsMoved();
    }
    else
    {
        this->_cells[origin.x][origin.y]._piece->markHasntMoved();
    }

    this->_cells[origin.x][origin.y]._status = CellStatus::OCCUPIED;

    //if last move captured any pieces
    //-> recover them
    if (lastmove.getIsCapture() == true)
    {
        this->_cells[destination.x][destination.y]._piece = lastmove.getPiece();

        nextUser->addPiece(lastmove.getPiece()->getPieceType(), lastmove.getPiece());
        this->_cells[destination.x][destination.y]._piece->getPieceSprite().setPosition(this->_cells[destination.x][destination.y]._rect.getPosition());
        this->_cells[destination.x][destination.y]._status = CellStatus::OCCUPIED;

    }
    else
    {
        this->_cells[destination.x][destination.y]._status = CellStatus::EMPTY;
    }

    // check type of the move
    if (lastmove.getMoveType() == MoveType::PROMOTION)
    {

        curUser->addPiece(PieceType::PAWN, lastmove.getPawn());
        curUser->removePiece(this->_cells[origin.x][origin.y]._piece->getPieceType(), this->_cells[origin.x][origin.y]._piece);
        this->_cells[origin.x][origin.y]._piece = lastmove.getPawn();
        this->_cells[origin.x][origin.y]._piece->getPieceSprite().setPosition(this->_cells[origin.x][origin.y]._rect.getPosition());

    }
    if (lastmove.getMoveType() == MoveType::EN_PASSANT)
    {
        this->_cells[origin.x][destination.y]._piece = lastmove.getPawn();
        nextUser->addPiece(PieceType::PAWN, lastmove.getPawn());
        this->_cells[origin.x][destination.y]._status = CellStatus::OCCUPIED;
        this->_cells[origin.x][destination.y]._piece->setHasDoubleMoved(true);
    }
    if (lastmove.getMoveType() == MoveType::LONG_CASTLING)
    {
        this->_cells[origin.x][0]._piece = std::move(this->_cells[origin.x][3]._piece);
        this->_cells[origin.x][0]._piece->getPieceSprite().setPosition(this->_cells[origin.x][0]._rect.getPosition());
        this->_cells[origin.x][0]._piece->markHasntMoved();
        this->_cells[origin.x][0]._status = CellStatus::OCCUPIED;
        this->_cells[origin.x][3]._status = CellStatus::EMPTY;
    }
    if (lastmove.getMoveType() == MoveType::SHORT_CASTLING)
    {
        this->_cells[origin.x][7]._piece = std::move(this->_cells[origin.x][5]._piece);
        this->_cells[origin.x][7]._piece->getPieceSprite().setPosition(this->_cells[origin.x][7]._rect.getPosition());
        this->_cells[origin.x][7]._piece->markHasntMoved();
        this->_cells[origin.x][7]._status = CellStatus::OCCUPIED;
        this->_cells[origin.x][5]._status = CellStatus::EMPTY;
    }

    //re-check
    bool hasMoves = this->_currentUser->hasLegalMoves(this->_cells);
    if (this->_currentUser->isChecked(this->_cells) == true) {
        this->_currentUser->checkKingCell(true, this->_cells);
        utilities::AudioPlayer::playSound(GameSound::CHECK);

        if (hasMoves == false) {
            utilities::AudioPlayer::playSound(GameSound::CHECKMATE);
            if (this->_currentUser->getColor() == PieceColor::WHITE) {
                cout << "Checkmate! Black wins!\n";
                RenderWindow window(VideoMode(800, 450), "Checkmate!");
                Image icon;
                icon.loadFromFile("../resources/images/interface/icon.png");
                window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
                Image image;
                image.loadFromFile("../resources/images/interface/blackwin.png");
                Texture texture;
                texture.loadFromImage(image);
                texture.setSmooth(true);
                Sprite sprite(texture);
                sprite.setScale(800 / sprite.getLocalBounds().width, 450 / sprite.getLocalBounds().height);
                while (window.isOpen())
                {
                    Event event;
                    while (window.pollEvent(event))
                    {
                        if (event.type == Event::Closed)
                            window.close();
                    }
                    window.clear();
                    window.draw(sprite);
                    window.display();
                }
            }
            else {
                cout << "Checkmate! White wins!\n";
                RenderWindow window(VideoMode(800, 450), "Checkmate!");
                Image icon;
                icon.loadFromFile("../resources/images/interface/icon.png");
                window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
                Image image;
                image.loadFromFile("../resources/images/interface/whitewin.png");
                Texture texture;
                texture.loadFromImage(image);
                texture.setSmooth(true);
                Sprite sprite(texture);
                sprite.setScale(800 / sprite.getLocalBounds().width, 450 / sprite.getLocalBounds().height);
                while (window.isOpen())
                {
                    Event event;
                    while (window.pollEvent(event))
                    {
                        if (event.type == Event::Closed)
                            window.close();
                    }
                    window.clear();
                    window.draw(sprite);
                    window.display();
                }
                this->_hasEndGame = true;
            }
        }
    }
    else {
        this->_currentUser->checkKingCell(false, this->_cells);

        if (hasMoves == false) {
            utilities::AudioPlayer::playSound(GameSound::STALEMATE);
            cout << "Stalemate!\n";
            RenderWindow window(VideoMode(800, 450), "Stalemate!");
            Image icon;
            icon.loadFromFile("../resources/images/interface/icon.png");
            window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
            Image image;
            image.loadFromFile("../resources/images/interface/stalemate.png");
            Texture texture;
            texture.loadFromImage(image);
            texture.setSmooth(true);
            Sprite sprite(texture);
            sprite.setScale(800 / sprite.getLocalBounds().width, 450 / sprite.getLocalBounds().height);
            while (window.isOpen())
            {
                Event event;
                while (window.pollEvent(event))
                {
                    if (event.type == Event::Closed)
                        window.close();
                }
                window.clear();
                window.draw(sprite);
                window.display();
            }
            this->_hasEndGame = true;
        }
    }
    this->_currentUser = make_shared<GameUser>(*curUser);
    this->drawBoard();
}

void ChessBoard::onRedoCellClicked()
{
    if (this->_undone.empty())
        return;
    Moved lastUndo = this->_undone.top();
    this->_undone.pop();
    this->_moved.push(lastUndo); // push redo to stack
    this->_currentUser->checkKingCell(false, this->_cells);

    // same as undo button
    Vector2i origin = lastUndo.getOri();
    Vector2i destination = lastUndo.getDes();
    GameUser* userN = lastUndo.getColor() == PieceColor::WHITE ? &this->_blackUser : &this->_whiteUser;
    GameUser* userC = lastUndo.getColor() == PieceColor::BLACK ? &this->_blackUser : &this->_whiteUser;
    if (lastUndo.getIsCapture() == true)
    {
        this->capturePiece(this->_cells[destination.x][destination.y]);
    }



    if (lastUndo.getMoveType() == MoveType::PROMOTION) {
        userC->removePiece(this->_cells[origin.x][origin.y]._piece->getPieceType(), this->_cells[origin.x][origin.y]._piece);
        if (lastUndo.getSpecialPiece() == PieceType::QUEEN)
        {
            this->_cells[origin.x][origin.y]._piece = make_shared<Queen>(Queen(lastUndo.getColor()));
        }
        if (lastUndo.getSpecialPiece() == PieceType::ROOK)
        {
            this->_cells[origin.x][origin.y]._piece = make_shared<Rook>(Rook(lastUndo.getColor()));
        }
        if (lastUndo.getSpecialPiece() == PieceType::KNIGHT)
        {
            this->_cells[origin.x][origin.y]._piece = make_shared<Knight>(Knight(lastUndo.getColor()));
        }
        if (lastUndo.getSpecialPiece() == PieceType::BISHOP)
        {
            this->_cells[origin.x][origin.y]._piece = make_shared<Bishop>(Bishop(lastUndo.getColor()));
        }
        userC->addPiece(this->_cells[origin.x][origin.y]._piece->getPieceType(), this->_cells[origin.x][origin.y]._piece);
    }
    else if (lastUndo.getMoveType() == MoveType::LONG_CASTLING) {
        this->_cells[origin.x][3]._piece = std::move(this->_cells[origin.x][0]._piece);

        this->_cells[origin.x][3]._piece->getPieceSprite().setPosition(this->_cells[origin.x][3]._rect.getPosition());

        this->_cells[origin.x][3]._piece->markAsMoved();
    }
    else if (lastUndo.getMoveType() == MoveType::SHORT_CASTLING) {
        this->_cells[origin.x][5]._piece = std::move(this->_cells[origin.x][7]._piece);

        this->_cells[origin.x][5]._piece->getPieceSprite().setPosition(this->_cells[origin.x][5]._rect.getPosition());

        this->_cells[origin.x][5]._piece->markAsMoved();
    }
    else if (lastUndo.getMoveType() == MoveType::EN_PASSANT) {
        this->capturePiece(this->_cells[origin.x][destination.y]);
        this->_cells[origin.x][destination.y]._status = CellStatus::EMPTY;
    }
    else if (lastUndo.getMoveType() == MoveType::NONE) {
        //do nothing;
    }
    this->_cells[destination.x][destination.y]._piece = std::move(this->_cells[origin.x][origin.y]._piece);

    this->_cells[destination.x][destination.y]._piece->getPieceSprite().setPosition(this->_cells[destination.x][destination.y]._rect.getPosition());

    this->_cells[destination.x][destination.y]._piece->markAsMoved();
    //change the current user
    GameUser user = lastUndo.getColor() == PieceColor::WHITE ? this->_blackUser : this->_whiteUser;
    this->_currentUser = make_shared<GameUser>(user);

    this->_cells[origin.x][origin.y]._status = CellStatus::EMPTY;
    this->_cells[destination.x][destination.y]._status = CellStatus::OCCUPIED;

    bool hasMoves = this->_currentUser->hasLegalMoves(this->_cells);


    if (this->_currentUser->isChecked(this->_cells) == true) {
        this->_currentUser->checkKingCell(true, this->_cells);
        utilities::AudioPlayer::playSound(GameSound::CHECK);

        if (hasMoves == false) {
            utilities::AudioPlayer::playSound(GameSound::CHECKMATE);
            if (this->_currentUser->getColor() == PieceColor::WHITE) {
                cout << "Checkmate! Black wins!\n";
                RenderWindow window(VideoMode(800, 450), "Checkmate!");
                Image icon;
                icon.loadFromFile("../resources/images/interface/icon.png");
                window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
                Image image;
                image.loadFromFile("../resources/images/interface/blackwin.png");
                Texture texture;
                texture.loadFromImage(image);
                texture.setSmooth(true);
                Sprite sprite(texture);
                sprite.setScale(800 / sprite.getLocalBounds().width, 450 / sprite.getLocalBounds().height);
                while (window.isOpen())
                {
                    Event event;
                    while (window.pollEvent(event))
                    {
                        if (event.type == Event::Closed)
                            window.close();
                    }
                    window.clear();
                    window.draw(sprite);
                    window.display();
                }
            }
            else {
                cout << "Checkmate! White wins!\n";
                RenderWindow window(VideoMode(800, 450), "Checkmate!");
                Image icon;
                icon.loadFromFile("../resources/images/interface/icon.png");
                window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
                Image image;
                image.loadFromFile("../resources/images/interface/whitewin.png");
                Texture texture;
                texture.loadFromImage(image);
                texture.setSmooth(true);
                Sprite sprite(texture);
                sprite.setScale(800 / sprite.getLocalBounds().width, 450 / sprite.getLocalBounds().height);
                while (window.isOpen())
                {
                    Event event;
                    while (window.pollEvent(event))
                    {
                        if (event.type == Event::Closed)
                            window.close();
                    }
                    window.clear();
                    window.draw(sprite);
                    window.display();
                }
                this->_hasEndGame = true;
            }
        }
    }
    else {
        this->_currentUser->checkKingCell(false, this->_cells);

        if (hasMoves == false) {
            utilities::AudioPlayer::playSound(GameSound::STALEMATE);
            cout << "Stalemate!\n";
            RenderWindow window(VideoMode(800, 450), "Stalemate!");
            Image icon;
            icon.loadFromFile("../resources/images/interface/icon.png");
            window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
            Image image;
            image.loadFromFile("../resources/images/interface/stalemate.png");
            Texture texture;
            texture.loadFromImage(image);
            texture.setSmooth(true);
            Sprite sprite(texture);
            sprite.setScale(800 / sprite.getLocalBounds().width, 450 / sprite.getLocalBounds().height);
            while (window.isOpen())
            {
                Event event;
                while (window.pollEvent(event))
                {
                    if (event.type == Event::Closed)
                        window.close();
                }
                window.clear();
                window.draw(sprite);
                window.display();
            }
            this->_hasEndGame = true;
        }
    }
    this->drawBoard();
}