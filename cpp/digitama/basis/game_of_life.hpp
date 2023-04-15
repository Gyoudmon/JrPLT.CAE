#pragma once // 确保只被 include 一次

#include "../big_bang/bang.hpp"

namespace WarGrey::STEM {
    enum class GameState { Run, Stop, Edit };

/*************************************************************************************************/
    /** 声明游戏物体 **/
    class GameOfLifelet : public WarGrey::STEM::IGraphlet {
    public:
        GameOfLifelet(int row, int col, float gridsize);
        virtual ~GameOfLifelet();

        void construct(SDL_Renderer* renderer) override;

    public:
        void feed_extent(float x, float y, float* width = nullptr, float* height = nullptr) override;
        void draw(SDL_Renderer* renderer, float x, float y, float Width, float Height) override;

    public:
        int preferred_local_fps() override { return 8; }

    protected: // 演化策略, 默认留给子类实现
        virtual void evolve(int** world, int* shadow, int row, int col);

    public:
        void show_grid(bool yes);
        void construct_random_world();
        bool pace_forward(int repeats);
        int current_generation() { return this->generation; }
        void reset();

    private:
        int row;
        int col;
        int generation;
        int** world;
        int* shadow;

    private:
        bool hide_grid;

    private:
        float gridsize;
    };

/*************************************************************************************************/
    /** 声明游戏宇宙 **/
    class GameOfLifeWorld : public WarGrey::STEM::TheBigBang {
        public:
            GameOfLifeWorld(float gridsize = 16.0F);
            GameOfLifeWorld(const char* title, float gridsize = 16.0F);
            virtual ~GameOfLifeWorld();

        public:    // 覆盖游戏基本方法
            void load(float width, float height) override;
            void reflow(float width, float height) override;
            void update(uint64_t count, uint32_t interval, uint64_t uptime) override;
            // void on_mission_start(float width, float height) override;

        protected: // 覆盖输入事件处理方法
            void on_char(char key, uint16_t modifiers, uint8_t repeats, bool pressed) override;  // 处理键盘事件

        private:
            void switch_game_state(WarGrey::STEM::GameState new_state);
            void construct_random_game_world();
            bool forward_game_world(int repeats, bool force);

        private: // 游戏物体
            GameOfLifelet* gameboard;

        private: // 游戏状态
            WarGrey::STEM::GameState state;

        private:
            float gridsize;
    };

/*************************************************************************************************/
/*    class ConwayLifeWorld : public WarGrey::STEM::GameOfLifeWorld {
        public:
            ConwayLifeWorld();

        protected:
            void evolve(int** world, int* shadow, int stage_width, int stage_height) override;
    };

    class HighLifeWorld : public WarGrey::STEM::GameOfLifeWorld {
        public:
            HighLifeWorld();

        protected:
            void evolve(int** world, int* shadow, int stage_width, int stage_height) override;
    };
*/
}
