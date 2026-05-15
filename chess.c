#include <stdio.h>
#include <ctype.h>

enum chessPieceType {
    PAWN,
    KING,
    QUEEN,
    KNIGHT,
    BISHOP,
    ROOK,

    EMPTY
};

enum chessPieceColor {
    BLACK,
    WHITE,

    NO_COLOR
};

typedef struct {
    enum chessPieceType type;
    enum chessPieceColor color;
} chessPiece;

typedef struct {
    chessPiece piece;
    // There is no reason to combine these by default, i. e 'a1'
    char row;  // i. e '1'
    char file; // i. e 'a'
} chessSquare;

chessSquare chess_board[8][8]; // [8] <- row + [8] <- file

int is_good_square(chessSquare square) {
    if (square.row > '8' || square.row < '1') return 0;
    if (square.file > 'h' || square.file < 'a') return 0;

    return 1;
}

int set_square(int rank, int file, enum chessPieceType type, enum chessPieceColor color) {
    chess_board[rank][file].piece.type = type;
    chess_board[rank][file].piece.color = color;
    chess_board[rank][file].row = '1' + rank;
    chess_board[rank][file].file = 'a' + file;
}

int make_board() {
   for (int r = 0; r < 8; r++) {
        for (int f = 0; f < 8; f++) {
            set_square(r, f, EMPTY, NO_COLOR);
        }
    }

    set_square(0, 0, ROOK, WHITE);
    set_square(0, 1, KNIGHT, WHITE);
    set_square(0, 2, BISHOP, WHITE);
    set_square(0, 3, QUEEN, WHITE);
    set_square(0, 4, KING, WHITE);
    set_square(0, 5, BISHOP, WHITE);
    set_square(0, 6, KNIGHT, WHITE);
    set_square(0, 7, ROOK, WHITE);
    for (int f = 0; f < 8; f++) set_square(1, f, PAWN, WHITE);

    set_square(7, 0, ROOK, BLACK);
    set_square(7, 1, KNIGHT, BLACK);
    set_square(7, 2, BISHOP, BLACK);
    set_square(7, 3, QUEEN, BLACK);
    set_square(7, 4, KING, BLACK);
    set_square(7, 5, BISHOP, BLACK);
    set_square(7, 6, KNIGHT, BLACK);
    set_square(7, 7, ROOK, BLACK);
    for (int f = 0; f < 8; f++) set_square(6, f, PAWN, BLACK);

    return 0;
}

int print_square(chessPiece piece) {

    char to_print = '.';


    if (piece.color == NO_COLOR) goto print;

    switch (piece.type) {
        case PAWN:   to_print = 'P'; break;
        case KING:   to_print = 'K'; break;
        case QUEEN:  to_print = 'Q'; break;
        case KNIGHT: to_print = 'N'; break;
        case BISHOP: to_print = 'B'; break;
        case ROOK:   to_print = 'R'; break;
        case EMPTY:
        default:     to_print = '.';
    }

    if (piece.color == BLACK) to_print = tolower((unsigned char)to_print);

print:
    
    putchar(to_print);
    
    return 0;
}

int print_board() {
    for (int i = 7; i >= 0; i--) {
        for (int j = 0; j < 8; j++) {
            if (j == 0) printf("%c ", chess_board[i][j].row);
            print_square(chess_board[i][j].piece);
        }
        printf("\n");
    }
    printf("  ABCDEFGH\n");

    return 0;
}

chessSquare *parse_coordinates(const char *coords) {
    int file = coords[0] - 'a';
    int row  = coords[1] - '1';
    return &chess_board[row][file];
}

int move_piece(chessSquare *piece, chessSquare *dest) {
    if (piece->piece.type == EMPTY || piece->piece.color == NO_COLOR) return -1;
    if (!is_good_square(*piece) || !is_good_square(*dest)) return -1;

    dest->piece.type = piece->piece.type;
    dest->piece.color = piece->piece.color;
    piece->piece.type = EMPTY;
    piece->piece.color = NO_COLOR;
    return 0;
}

int main() {
    make_board();

    while (1) {
        print_board();

        char from[3];
        printf("from: ");
        scanf("%2s", from);

        char to[3];
        printf("to: ");
        scanf("%2s", to);

        printf("(");
        print_square(parse_coordinates(from)->piece);
        printf(")\n");

        printf("(");
        print_square(parse_coordinates(to)->piece);
        printf(")\n");

        move_piece(parse_coordinates(from), parse_coordinates(to));
    }
}
