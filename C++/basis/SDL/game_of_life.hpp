#ifndef _GAME_OF_LIFE_H // 确保只被 include 一次
#define _GAME_OF_LIFE_H

#include "game.hpp"

namespace WarGrey::STEM {
    /** 声明游戏宇宙 **/
    class GameOfLife : public WarGrey::STEM::Universe {
        public:
            GameOfLife(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* texture, const char* title);
            virtual ~GameOfLife();

        public:  // 覆盖游戏方法
            void construct(int argc, char* argv[], int width, int height);
            void update(SDL_Renderer* renderer, uint32_t interval, uint32_t count, uint32_t uptime);

        protected:
            virtual void evolve(int** world, int* shadow, int world_width, int world_height) = 0;

        private:
            void timeline_forward(uint32_t interval, uint32_t count, uint32_t uptime);

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
            char last_key_typed;
    };

    class ConwayLife : public WarGrey::STEM::GameOfLife {
        public:
            ConwayLife(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* texture);

        protected:
            void evolve(int** world, int* shadow, int world_width, int world_height);
    };

    class HighLife : public WarGrey::STEM::GameOfLife {
        public:
            HighLife(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* texture);

        protected:
            void evolve(int** world, int* shadow, int world_width, int world_height);
    };
}

#endif

