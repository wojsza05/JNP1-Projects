#ifndef GAME_TYPE_H
#define GAME_TYPE_H

// Beware that you need to divide the weight of (int)match_type by 10 to read it.
// It is because you cannot do enum -> float.
enum game_type{
    friendly = 10,
    points = 25,
    final = 50
};


#endif //GAME_TYPE_H
