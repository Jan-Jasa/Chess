#ifndef CHESSBOT_CHESS_H
#define CHESSBOT_CHESS_H

#define BOARD_SIZE 8
#define BOX_WIDTH 10
#define DOWN 3
#define RIGHT 10
#define TO_DOWN std::string(DOWN, '\n')
#define TO_RIGHT std::string(RIGHT, ' ')
#define CLEAR_LINE std::string(100, ' ')
#define MOVES_PER_LINE 5

#define AGAINST_BOT true			// if true then it's ? vs bot, if false then it's player vs player
#define TWO_BOTS true				// assuming AGAINST_BOT is true, if true then it's bot vs bot, if false then it's player vs bot
#define BOT_IS_WHITE false			// assuming AGAINST_BOT is true and TWO_BOTS is false, if true then the bot is team white, black otherwise
#define WHITE_BOT_RANDOM true		// if true the white bot returns random moves, if false the white bot returns smart moves generated by minimax algorithm
#define BLACK_BOT_RANDOM false		// if true the black bot returns random moves, if false the black bot returns smart moves generated by minimax algorithm
#define WHITE_BOT_DIFFICULTY 3		// the tree depth of decision tree of the bot, which is also correlated to the bot's difficulty (aka chess IQ)
#define BLACK_BOT_DIFFICULTY 3		// 1: easy, 2: medium, 3: hard

// types of chess pieces
typedef enum {
    B_KING = -6, B_QUEEN, B_BISHOP, B_KNIGHT, B_ROOK, B_PAWN, EMPTY,
    W_KING, W_QUEEN, W_BISHOP, W_KNIGHT, W_ROOK, W_PAWN
} ChessPieces;

// types of player moves
typedef enum {
    NORMAL, CASTLING, PROMOTION, EN_PASSANT
} Moves;

// possible options on how the game can end
typedef enum {
    CHECKMATE, FIFTY_MOVES, THREEFOLD_REP, QUIT
} Endgame;

// starting position of the board
const char STARTING_BOARD[BOARD_SIZE][BOARD_SIZE] = {{B_ROOK, B_KNIGHT, B_BISHOP, B_QUEEN, B_KING, B_BISHOP, B_KNIGHT, B_ROOK},
                                                     {B_PAWN, B_PAWN, B_PAWN, B_PAWN, B_PAWN, B_PAWN, B_PAWN, B_PAWN},
                                                     {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
                                                     {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
                                                     {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
                                                     {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
                                                     {W_PAWN, W_PAWN, W_PAWN, W_PAWN, W_PAWN, W_PAWN, W_PAWN, W_PAWN},
                                                     {W_ROOK, W_KNIGHT, W_BISHOP, W_QUEEN, W_KING, W_BISHOP, W_KNIGHT, W_ROOK}};

class Player;
class PathNode;
class Bot;
class Chess;

#include <string>
#include <forward_list>
#include <vector>
#include <map>

// player class declaration
class Player {
protected:
    std::string name;			// name of the player
    unsigned short score = 0;	// score of the player
    bool castling = true;		// if true the player can castle, if false the player cannot castle anymore during the game
public:
    Player(const std::string &name) noexcept;
    std::string GetName() const noexcept;
    unsigned short GetScore() const noexcept;
    bool GetCastling() const noexcept;
    void SetCastling(const bool &castling) noexcept;
    void IncreaseScore(const unsigned short &inc) noexcept;
    void Reset() noexcept;
    bool operator== (const Player &p) const noexcept;
};

// path node class declaration
class PathNode {
private:
    std::map<std::string, PathNode> child_node_list;	// a map where the key (string) corresponds to the player's move and value corresponds to another node, which makes a decision tree
    void CreateSubtree(Chess &c) noexcept;
    float AlphaBeta(Chess &c, unsigned short &depth, float alpha, float beta, const bool &maximizing_player, const bool &initial_turn) noexcept;
public:
    std::string AlphaBetaRoot(Chess &c, unsigned short &difficulty) noexcept;
};

// bot class declaration, which inherits from player class
class Bot : public Player {
private:
    PathNode root;					// the root node of the bot's decision tree
    unsigned short difficulty;		// the depth of the bot's decision tree
public:
    Bot(const std::string &name, const unsigned short &difficulty) noexcept;
    unsigned short GetDifficulty() const noexcept;
    std::string GetIdealMove(Chess &c) noexcept;
    std::string GetIdealMove(Chess &c, unsigned short difficulty) noexcept;
    bool operator== (const Bot &b) const noexcept;
};

// chess class declaration
class Chess {
private:
    char board[BOARD_SIZE][BOARD_SIZE];		// game board
    Bot white, black;						// teams
    std::vector<std::pair<Moves, std::string>> all_game_moves;		// a record which contains all the moves played throughout the game in chronological order
    bool whites_turn = true;				// if true then it's team white's turn, if false then it's team black's turn
    unsigned short moves_after_last_pawn_move_or_capture = 0;		// number of moves played after a pawn moved or a capture happened
    static bool WithinBounds(const short &coord) noexcept;
    static void ChangeToString(char &x1, char &y1, char &x2, char &y2) noexcept;
    static std::string ToString(const short &x1, const short &y1, const short &x2, const short &y2) noexcept;
    static std::string PieceNameToString(const char &piece) noexcept;
    static float EvaluatePiece(const char &piece) noexcept;
    static void ClearAllMoves(const unsigned short &n) noexcept;
    static void PrintSeparator(const char &ch) noexcept;
    static void CopyBoard(const char from[BOARD_SIZE][BOARD_SIZE], char to[BOARD_SIZE][BOARD_SIZE]) noexcept;
    static bool AreBoardsEqual(const char board1[BOARD_SIZE][BOARD_SIZE], const char board2[BOARD_SIZE][BOARD_SIZE]) noexcept;
    static bool CanMovePiece(const short &x1, const short &y1, const short &x2, const short &y2, const std::forward_list<std::string> &all_moves) noexcept;
    Bot& GetCurrentPlayer() noexcept;
    Bot GetCurrentPlayerConst() const noexcept;
    Bot& GetOtherPlayer() noexcept;
    Bot GetOtherPlayerConst() const noexcept;
    void ChangeTurn() noexcept;
    void AppendToAllGameMoves(const short &x1, const short &y1, const short &x2, const short &y2) noexcept;
    void Reset() noexcept;
    void CheckCoordinates(const short &x, const short &y, const std::string &func_name) const noexcept(false);
    bool EndGameText(const unsigned short &n, const Endgame &end_game) const noexcept;
    short GetEnPassant(const short &x, const short &y) const noexcept;
    template<class Iterator> short GetEnPassant(const char board[BOARD_SIZE][BOARD_SIZE], const Iterator &it) const noexcept;
    bool ThreefoldRepetition() const noexcept;
    bool IsCheck(const bool &turn) const noexcept;
    bool IsCheck(std::string &move) noexcept;
    std::forward_list<std::string> PawnMoves(const short &x, const short &y) const noexcept;
    std::forward_list<std::string> RookMoves(const short &x, const short &y) const noexcept;
    std::forward_list<std::string> KnightMoves(const short &x, const short &y) const noexcept;
    std::forward_list<std::string> BishopMoves(const short &x, const short &y) const noexcept;
    std::forward_list<std::string> QueenMoves(const short &x, const short &y) const noexcept;
    std::forward_list<std::string> KingMoves(const short &x, const short &y) const noexcept;
    std::string GetRandomMove() noexcept;
    void ManuallyPromotePawn(const short &x, const short &y) noexcept;
    void UpdateBoard(const short &x, const short &y) const noexcept;
    void UpdateScore(const Bot &p) const noexcept;
    float EvaluatePosition(const short &x, const short &y) const noexcept;
    void PrintAllMovesMadeInOrder() const noexcept;
    bool CheckEndgame(const unsigned short &n = 0) noexcept;
public:
    Chess(const std::string &player1, const unsigned short &difficulty1, const std::string &player2, const unsigned short &difficulty2) noexcept;
    static void ChangeToRealCoordinates(char &x1, char &y1, char &x2, char &y2) noexcept;
    char GetPiece(const short &x, const short &y) const noexcept;
    bool GetTurn() const noexcept;
    std::forward_list<std::string> AllMoves() noexcept;
    void MovePiece(const short &x1, const short &y1, const short &x2, const short &y2, const bool &manual_promotion, const bool &update_board) noexcept;
    void MovePieceBack(const short &x1, const short &y1, const short &x2, const short &y2) noexcept;
    float EvaluateBoard(const bool &turn) const noexcept;
    void PrintBoard() const noexcept;
    bool PlayersTurn() noexcept;
    bool BotsTurn() noexcept;
    bool GameOver() noexcept;
};

// other function declarations
void MoveCursorToXY(const short &x, const short &y) noexcept;
std::string ToLowerString(std::string s) noexcept;
template<class T> T GetRandomNumber(const T &min, const T &max) noexcept;

#endif //CHESSBOT_CHESS_H
