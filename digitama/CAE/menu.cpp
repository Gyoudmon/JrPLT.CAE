#include "menu.hpp"

using namespace Plteen;
using namespace WarGrey::CAE;

/*************************************************************************************************/
static const char MENU_RETURN_KEY = '0';

/*************************************************************************************************/
void WarGrey::CAE::IMenu::load(float width, float height) {
    auto font = GameFont::monospace(FontSize::large);
    this->menuitems = this->prepare_menu_items();

    for (auto menuitem : this->menuitems) {
        const char* caption = menuitem.second.c_str();
        char key = menuitem.first;

        this->menulabels[key] = this->insert(new Labellet(font, BLACK, "%c. %s", key, caption));
    }
}

void WarGrey::CAE::IMenu::reflow(float width, float height) {
    for (size_t idx = 1; idx < this->menuitems.size(); idx ++) {
        Labellet* prev_label = this->menulabels[this->menuitems[idx - 1].first];
        Labellet* self_label = this->menulabels[this->menuitems[idx].first];
        
        this->move_to(self_label, { prev_label, MatterPort::LB }, MatterPort::LT, { 0.0F, 2.0F });
    }
}

void WarGrey::CAE::IMenu::on_menu_char(IMenuEventListener* master, MenuType self, char key) {
    this->select_menu(key);
    this->on_menu_key(master, self, key);
}

void WarGrey::CAE::IMenu::select_menu(char key) {
    if (this->menulabels.find(this->current_menuitem) != this->menulabels.end()) {
        this->menulabels[this->current_menuitem]->set_text_color(BLACK);
    }

    if (this->menulabels.find(key) != this->menulabels.end()) {
        this->menulabels[key]->set_text_color(ROYALBLUE);
    }
    
    this->current_menuitem = key;    
}

/*************************************************************************************************/
static const char TLM_CLSS_KEY = '1';
static const char TLM_CRSE_KEY = '2';
static const char TLM_STDT_KEY = '3';
static const char TLM_CLER_KEY = '4';

std::vector<std::pair<char, std::string>> WarGrey::CAE::TopLevelMenu::prepare_menu_items() {
    return {
        { MENU_RETURN_KEY, "退出系统"},
        { TLM_CLSS_KEY, "开设班级"},
        { TLM_CRSE_KEY, "开设课程"},
        { TLM_STDT_KEY, "新生注册"},
        { TLM_CLER_KEY, "清理信息"}
    };
}

void WarGrey::CAE::TopLevelMenu::on_menu_key(IMenuEventListener* master, MenuType self, char key) {
    switch (key) {
    case TLM_CLSS_KEY: master->on_menu_task(self, MenuTask::CreateClass); break;
    case TLM_CRSE_KEY: master->on_menu_task(self, MenuTask::CreateDiscipline); break;
    case TLM_STDT_KEY: master->on_menu_task(self, MenuTask::CreateStudent); break;
    case TLM_CLER_KEY: master->on_menu_switch(self, MenuType::Clear); break;
    case MENU_RETURN_KEY: master->on_menu_task(self, MenuTask::Exit); break;
    default: /* do nothing */;
    }
}

/*************************************************************************************************/
static const char CLASS_DELETE_KEY = '1';

std::vector<std::pair<char, std::string>> WarGrey::CAE::ClassMenu::prepare_menu_items() {
    return {
        { CLASS_DELETE_KEY, "删除班级" }
    };
}

void WarGrey::CAE::ClassMenu::on_menu_key(IMenuEventListener* master, MenuType self, char key) {
    switch (key) {
    case CLASS_DELETE_KEY: master->on_menu_task(self, MenuTask::DeleteClass); break;
    default: /* do nothing */;
    }
}

/*************************************************************************************************/
static const char DSCPLN_DELETE_KEY = '1';

std::vector<std::pair<char, std::string>> WarGrey::CAE::DisciplineMenu::prepare_menu_items() {
    return {
        { DSCPLN_DELETE_KEY, "删除课程"}
    };
}

void WarGrey::CAE::DisciplineMenu::on_menu_key(IMenuEventListener* master, MenuType self, char key) {
    switch (key) {
    case DSCPLN_DELETE_KEY: master->on_menu_task(self, MenuTask::DeleteDiscipline); break;
    default: /* do nothing */;
    }
}

/*************************************************************************************************/
static const char STDT_UPDATE_KEY = '1';
static const char STDT_DELETE_KEY = '2';
static const char STDT_CLASS_KEY = '3';
static const char STDT_AVATAR_KEY = '4';

std::vector<std::pair<char, std::string>> WarGrey::CAE::StudentMenu::prepare_menu_items() {
    return {
        { STDT_UPDATE_KEY, "更新学生信息"},
        { STDT_DELETE_KEY, "删除学生信息"},
        { STDT_CLASS_KEY,  "分班"},
        { STDT_AVATAR_KEY, "更改形象和性别"}
    };
}

void WarGrey::CAE::StudentMenu::on_menu_key(IMenuEventListener* master, MenuType self, char key) {
    switch (key) {
    case STDT_UPDATE_KEY: master->on_menu_task(self, MenuTask::UpdateStudent); break;
    case STDT_DELETE_KEY: master->on_menu_task(self, MenuTask::DeleteStudent); break;
    case STDT_CLASS_KEY: master->on_menu_task(self, MenuTask::BindClass); break;
    case STDT_AVATAR_KEY: master->on_menu_task(self, MenuTask::UpdateAvatar); break;
    default: /* do nothing */;
    }
}

/*************************************************************************************************/
static const char GRADE_CREATE_KEY = '1';
static const char GRADE_UPDATE_KEY = '2';
static const char GRADE_DELETE_KEY = '3';

std::vector<std::pair<char, std::string>> WarGrey::CAE::GradeMenu::prepare_menu_items() {
    return {
        { GRADE_CREATE_KEY, "添加成绩信息"},
        { GRADE_UPDATE_KEY, "更新成绩信息"},
        { GRADE_DELETE_KEY, "删除成绩信息"}
    };
}

void WarGrey::CAE::GradeMenu::on_menu_key(IMenuEventListener* master, MenuType self, char key) {
    switch (key) {
    case GRADE_CREATE_KEY: master->on_menu_task(self, MenuTask::CreateGrade); break;
    case GRADE_UPDATE_KEY: master->on_menu_task(self, MenuTask::UpdateGrade); break;
    case GRADE_DELETE_KEY: master->on_menu_task(self, MenuTask::DeleteGrade); break;
    default: /* do nothing */;
    }
}

/*************************************************************************************************/
static const char CLEAR_STUDT_KEY = '1';
static const char CLEAR_GRADE_KEY = '2';

std::vector<std::pair<char, std::string>> WarGrey::CAE::ClearMenu::prepare_menu_items() {
    return {
        { MENU_RETURN_KEY, "返回上级菜单"},
        { CLEAR_STUDT_KEY, "清除游离学生信息"},
        { CLEAR_GRADE_KEY, "清除游离成绩信息"}
    };
}

void WarGrey::CAE::ClearMenu::on_menu_key(IMenuEventListener* master, MenuType self, char key) {
    switch (key) {
    case CLEAR_STUDT_KEY: master->on_menu_task(self, MenuTask::ClearStudent); break;
    case CLEAR_GRADE_KEY: master->on_menu_task(self, MenuTask::ClearGrade); break;
    case MENU_RETURN_KEY: master->on_menu_switch(self, MenuType::TopLevel); break;
    default: /* do nothing */;
    }
}
