#ifndef _GAME_OF_LIFE_H // 确保只被 include 一次
#define _GAME_OF_LIFE_H

#include "game.hpp"

namespace WarGrey::STEM {

    /** 声明游戏宇宙 **/
    class GameOfLife : public WarGrey::STEM::Universe {
        public:
            GameOfLife(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* texture);
            virtual ~GameOfLife();

        public:  // 覆盖游戏方法
            void construct(int argc, char* argv[], int width, int height);
            void update(WarGrey::STEM::timer_frame_t* frame, SDL_Renderer* renderer);

        private:
            void evolve(WarGrey::STEM::timer_frame_t* frame, SDL_Renderer* renderer);

        private: // 世界(网格)参数
            int** world;
            int* shadow;
            int generation;
            int world_width;
            int world_height;
            int draw_x;
            int draw_y;

        private:
            int lineheight;
    };
}

#endif

