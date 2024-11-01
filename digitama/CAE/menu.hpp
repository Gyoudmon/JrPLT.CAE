#pragma once

#include <plteen/game.hpp>

#include <vector>
#include <map>

namespace WarGrey::CAE {
    enum class MenuType { TopLevel, Class, Discipline, Student, Grade, Clear };
    enum class MenuTask { Exit,
        CreateClass, DeleteClass,
        CreateDiscipline, DeleteDiscipline,
        CreateStudent, UpdateStudent, DeleteStudent, BindClass, UpdateAvatar, ClearStudent,
        CreateGrade, UpdateGrade, DeleteGrade, ClearGrade,
        _
    };

    class IMenuEventListener {
    public:
        virtual void on_menu_switch(MenuType self, MenuType to) = 0;
        virtual void on_menu_task(MenuType self, MenuTask task) = 0;
    };

    /*********************************************************************************************/
    class IMenu : public Plteen::Plane {
    using Plane::Plane;

    public:
        void load(float width, float height) override;
        void reflow(float width, float height) override;

    public:
        void on_menu_char(IMenuEventListener* master, MenuType self, char key);

    public:
        size_t count() { return this->menuitems.size(); }
        void select_menu(char key);

    public:
        virtual uint8_t menu_key_base() { return 0; }

    protected:
        virtual void on_menu_key(IMenuEventListener* master, MenuType self, char key) = 0;
        virtual std::vector<std::pair<char, std::string>> prepare_menu_items() = 0;
        
    private:
        std::vector<std::pair<char, std::string>> menuitems;
        std::map<char, Plteen::Labellet*> menulabels;

    private:
        char current_menuitem = '\0';
    };

    /*********************************************************************************************/
    class TopLevelMenu : public WarGrey::CAE::IMenu {
    public:
        TopLevelMenu() : IMenu("默认菜单") {}
        virtual ~TopLevelMenu() {}

    public:
        uint8_t menu_key_base() override { return 0; }

    protected:
        void on_menu_key(IMenuEventListener* master, MenuType self, char key) override;
        std::vector<std::pair<char, std::string>> prepare_menu_items() override;
    };

    class ClassMenu : public WarGrey::CAE::IMenu {
    public:
        ClassMenu() : IMenu("班级信息菜单") {}
        virtual ~ClassMenu() {}

    public:
        uint8_t menu_key_base() override { return 1; }

    protected:
        void on_menu_key(IMenuEventListener* master, MenuType self, char key) override;
        std::vector<std::pair<char, std::string>> prepare_menu_items() override;
    };

    class DisciplineMenu : public WarGrey::CAE::IMenu {
    public:
        DisciplineMenu() : IMenu("课程信息菜单") {}
        virtual ~DisciplineMenu() {}

    public:
        uint8_t menu_key_base() override { return 1; }
    
    protected:
        void on_menu_key(IMenuEventListener* master, MenuType self, char key) override;
        std::vector<std::pair<char, std::string>> prepare_menu_items() override;
    };

    class StudentMenu : public WarGrey::CAE::IMenu {
    public:
        StudentMenu() : IMenu("学生信息菜单") {}
        virtual ~StudentMenu() {}

    public:
        uint8_t menu_key_base() override { return 1; }

    protected:
        void on_menu_key(IMenuEventListener* master, MenuType self, char key) override;
        std::vector<std::pair<char, std::string>> prepare_menu_items() override;
    };

    class GradeMenu : public WarGrey::CAE::IMenu {
    public:
        GradeMenu() : IMenu("成绩管理菜单") {}
        virtual ~GradeMenu() {}

    public:
        uint8_t menu_key_base() override { return 1; }

    protected:
        void on_menu_key(IMenuEventListener* master, MenuType self, char key) override;
        std::vector<std::pair<char, std::string>> prepare_menu_items() override;
    };

    class ClearMenu : public WarGrey::CAE::IMenu {
    public:
        ClearMenu() : IMenu("清理菜单") {}
        virtual ~ClearMenu() {}

    public:
        uint8_t menu_key_base() override { return 0; }

    protected:
        void on_menu_key(IMenuEventListener* master, MenuType self, char key) override;
        std::vector<std::pair<char, std::string>> prepare_menu_items() override;
    };
}
