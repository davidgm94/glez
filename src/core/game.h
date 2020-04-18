typedef struct {
    bool up, down, left, right;
} game_keyboard;
game_keyboard main_keyboard;

//typedef struct {
//    vec3f pos;
//    f32 size;
//    f32 step_speed;
//} player;
//
//player main_player;
//
//void process_keyboard_input(game_keyboard keyboard)
//{
//    main_player.pos.x += keyboard.right *   main_player.step_speed;
//    main_player.pos.x -= keyboard.left *    main_player.step_speed;
//    main_player.pos.y += keyboard.down *    main_player.step_speed;
//    main_player.pos.y -= keyboard.up *      main_player.step_speed;
//    printf("[PLAYER] X: %f. Y: %f. Z: %f.\n", main_player.pos.x, main_player.pos.y, main_player.pos.z);
//}