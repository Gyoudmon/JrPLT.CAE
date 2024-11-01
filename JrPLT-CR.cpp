#include <plteen/game.hpp>
#include <plteen/datum/string.hpp>
#include <plteen/datum/vector.hpp>

#include "digitama/CAE/menu.hpp"
#include "digitama/CAE/avatar.hpp"
#include "digitama/CAE/model.hpp"

#include "digitama/CAE/view/doorlet.hpp"
#include "digitama/CAE/view/disciplinelet.hpp"
#include "digitama/CAE/view/studentlet.hpp"
#include "digitama/CAE/view/desklet.hpp"
#include "digitama/CAE/view/gradelet.hpp"

#include <sstream>
#include <vector>
#include <map>

using namespace Plteen;
using namespace WarGrey::CAE;

/*************************************************************************************************/
namespace {
    static const size_t DESK_COUNT = 7;
    static const float platform_width = 512.0F;
    static const float platform_height = 80.0F;
    static const double gliding_duration = 0.4;

    static const char* default_class_report_title = "[班级成绩单]";
    static const char* default_student_report_title = "[学生成绩单]";

    static const std::vector<DisciplineType> report_disciplines = {
        DisciplineType::Mathematics, DisciplineType::Programming,
        DisciplineType::STEM, DisciplineType::Printing3D
    };
    
    enum class GradeTask { CheckCondition, DoTask, _ };

    /*********************************************************************************************/
    class STEMClassroomPlane : public Plane, public IMenuEventListener, public IModelListener {
    public:
        STEMClassroomPlane(std::string gmsin, std::string gmsout) : Plane("JrPLT CAE") {
            this->model = new GMSModel(this);
            this->gmsin = gmsin;
            this->gmsout = (gmsout.empty()) ? this->gmsin : gmsout;
        }

        virtual ~STEMClassroomPlane() {
            delete this->model;
        }

    public:
        void load(float width, float height) override {
            this->load_gui_elements(width, height);
            this->load_menus(width, height);
            this->load_classroom(width, height);
            this->load_avatars(width, height);

            try {
                this->model->import_from_file(this->gmsin);
                this->reflow_model_sprites(gliding_duration);
            } catch (const std::exception& e) {}

            this->load_grade_reports(width, height);
        }

        void reflow(float width, float height) override {
            this->reflow_gui_elements(width, height);
            this->reflow_classroom(width, height);
            this->reflow_class_logos();
            this->reflow_discipline_logos();
        }

    public:
        bool can_select(IMatter* m) override {
            return true;
        }
        
        bool can_select_multiple() override {
            return true;
        }

        void after_select(IMatter* m, bool yes) override {
            auto cls = dynamic_cast<DoorSprite*>(m);
            auto dis = dynamic_cast<DisciplineSprite*>(m);
            auto stu = dynamic_cast<StudentSprite*>(m);
            
            if (yes) {
                if (m == this->platform) {
                    if (this->the_disCode > 0U) {
                        this->remove_selected(this->disciplines[this->the_disCode]);
                        this->the_disCode = 0U;
                    }

                    this->reflow_discipline_logos();
                    this->update_students_score_bars(this->the_disCode);
                } else if (stu != nullptr) {
                    this->on_student_changed(stu->primary_key());
                } else if (cls != nullptr) {
                    if ((this->the_task == MenuTask::BindClass) && (this->the_sNo > 0U)) {
                        this->glide_to(gliding_duration, this->students[this->the_sNo], { cls, MatterPort::RC }, MatterPort::LC);
                        this->model->bind_student_to_class(this->the_sNo, cls->primary_key());
                    } else {
                        this->on_class_changed(cls->primary_key(), false);
                    }
                } else if (dis != nullptr) {
                    this->on_discipline_changed(this->model->get_discipline_code(dis->get_type()), false);
                }
            } else {
                if (stu != nullptr) {
                    this->on_student_changed(0U);
                } else if (dis != nullptr) {
                    this->on_discipline_changed(0U, false);
                } else if (cls != nullptr) {
                    // do nothing
                }
            }

            this->switch_menu();
        }

        void on_tap(IMatter* m, float local_x, float local_y) override {
            auto dsk = dynamic_cast<HexagonalDesklet*>(m);

            if (dsk != nullptr) {
                this->try_exchange_seat(dsk, local_x, local_y);
            } else {
                Plane::on_tap(m, local_x, local_y);
            }
        }

        bool update_tooltip(IMatter* m, float lx, float ly, float gx, float gy) override {
            bool updated = false;
            
            if (m != this->agent) {
                auto student = dynamic_cast<StudentSprite*>(m);
                auto desk = dynamic_cast<IDesk*>(m);

                if (student != nullptr) {
                    this->tooltip->set_text(" %s[%u] ", student->name(), student->primary_key());
                    updated = true;
                } else if (desk != nullptr) {
                    this->tooltip->set_text(" %d ", desk->get_seat_by(lx, ly));
                    updated = true;
                }
            }

            return updated;
        }

    public:
        void on_char(char key, uint16_t modifiers, uint8_t repeats, bool pressed) override {
            if (!pressed) {
                if (isdigit(key)) {
                    int idx = key - '0';
                    IMenu* self = this->menus[this->the_menu_type]->unsafe_plane<IMenu>();

                    if ((idx >= 0) && (idx < self->count() + self->menu_key_base())) {
                        self->on_menu_char(this, this->the_menu_type, key);
                    }
                } else if (key == 'w') {
                    if (!this->gmsout.empty()) {
                        try {
                            this->model->export_to_file(this->gmsout);
                            this->log_message(Log::Info, make_nstring("done exporting to %s.", this->gmsout.c_str()));
                        } catch (const std::exception& e) {
                            this->log_message(Log::Fatal, e.what());
                        }
                    }
                } else if (int(key) == 27) { // ESC
                    this->the_task = MenuTask::_;
                    this->menus[this->the_menu_type]->unsafe_plane<IMenu>()->select_menu('\0');
                }
            }
        }

        void on_text(const char* text, size_t size, bool entire) override {
            if (entire) {
                if (size > 0) {
                    try {
                        switch (this->the_task) {
                        case MenuTask::CreateClass: this->model->create_class_from_user_input(text, size); break;
                        case MenuTask::DeleteClass: this->on_model_deletion(this->the_task, text, size); break;
                        case MenuTask::CreateDiscipline: this->model->create_discipline_from_user_input(text, size); break;
                        case MenuTask::DeleteDiscipline: this->on_model_deletion(this->the_task, text, size); break;
                        case MenuTask::CreateStudent: this->model->create_student_from_user_input(text, size); break;
                        case MenuTask::UpdateStudent: this->model->update_student_from_user_input(this->the_sNo, text, size); break;
                        case MenuTask::UpdateAvatar: this->model->update_student_avatar_from_user_input(this->the_sNo, text, size); break;
                        case MenuTask::DeleteStudent: this->on_model_deletion(this->the_task, text, size); break;
                        case MenuTask::CreateGrade: this->on_grade_text(text, size); break;
                        case MenuTask::UpdateGrade: this->on_grade_text(text, size); break;
                        case MenuTask::DeleteGrade: this->on_grade_text(text, size); break;
                        case MenuTask::ClearStudent: this->on_model_deletion(this->the_task, text, size); break;
                        case MenuTask::ClearGrade: this->on_model_deletion(this->the_task, text, size); break;
                        default: /* do nothing */;
                        }
                    } catch (const std::exception& e) {
                        this->log_message(Log::Fatal, e.what());
                    }
                } else if (this->the_grade_subtask != GradeTask::_) {
                    this->on_grade_text(text, size);
                }
            }
        }

        void on_grade_text(const char* text, size_t size) {
            switch (this->the_grade_subtask) {
            case GradeTask::CheckCondition: {
                size_t pos = 0U;
                uint64_t ts = scan_natural(text, &pos, size);
                    
                if (ts > 0U) {
                    this->the_timestamp = ts;
                }

                this->on_grade_task(this->the_timestamp > 0U);
            }; break;
            case GradeTask::DoTask: {
                if (size > 0U) {
                    switch (this->the_task) {
                    case MenuTask::CreateGrade: {
                        this->model->register_student_scores_from_user_input(this->the_sNo,
                            this->the_disCode, this->the_timestamp,
                            text, size);
                    }; break;
                    case MenuTask::UpdateGrade: {
                        this->model->update_student_scores_from_user_input(this->the_sNo,
                            this->the_disCode, this->the_timestamp,
                            text, size);
                    }; break;
                    default: /* do nothing */;
                    }

                    this->update_class_report(this->the_clsId);
                    this->update_student_report(this->the_sNo);
                    this->update_students_score_bars(this->the_disCode, this->the_sNo);
                    this->log_message(Log::Info, "Done.");
                }

                this->the_grade_subtask = GradeTask::_;
            }; break;
            default: /* do nothing */;
            }
        }

        void on_model_deletion(MenuTask task, const char* text, size_t size) {
            std::string answer;
            size_t pos = 0;
            
            scan_skip_space(text, &pos, size);
            answer = scan_string(text, &pos, size);

            if (string_ci_equal(answer, "y") || string_ci_equal(answer, "yes")) {
                try {
                    switch (task) {
                    case MenuTask::DeleteClass: this->model->delete_class_as_user_request(this->the_clsId); break;
                    case MenuTask::DeleteDiscipline: this->model->delete_discipline_as_user_request(this->the_disCode); break;
                    case MenuTask::DeleteStudent: this->model->delete_student_as_user_request(this->the_sNo); break;
                    case MenuTask::ClearStudent: this->model->clear_detached_students(); break;
                    case MenuTask::ClearGrade: this->model->clear_detached_grades(); break;
                    default: /* do nothing */;
                    }

                    this->log_message(Log::Info, "Done.");
                    this->switch_menu();
                } catch (const std::exception& e) {
                    this->log_message(Log::Fatal, e.what());
                }
            }
        }

    public:
        void on_menu_switch(MenuType self, MenuType to) override {
            if (self != to) {
                if (this->menus.find(to) != this->menus.end()) {
                    this->menus[self]->unsafe_plane<IMenu>()->select_menu('\0');
                    this->menus[self]->show(false);
                    this->menus[to]->show(true);
                    this->the_menu_type = to;
                }

                this->the_task = MenuTask::_;
                this->the_grade_subtask = GradeTask::_;

                this->avatar->show(this->the_menu_type == MenuType::Student);
                this->stuReport->show(!this->avatar->visible());
                this->clsReport->show(!this->avatar->visible());
            }
        }
        
        void on_menu_task(MenuType self, MenuTask task) override {
            this->the_task = task;

            try {
                switch (task) {
                case MenuTask::Exit: this->mission_complete(); break;
                case MenuTask::CreateClass: this->start_input_text("请按格式输入待创建班级的信息(%s): ", ClassEntity::prompt()); break;
                case MenuTask::DeleteClass: this->start_input_text("是否确定删除班级[%llu]的信息(Y/N): ", this->the_clsId); break;
                case MenuTask::CreateDiscipline: this->start_input_text("请按格式输入待创建课程的信息(%s): ", DisciplineEntity::prompt()); break;
                case MenuTask::DeleteDiscipline: this->start_input_text("是否确定删除课程[%s]的信息(Y/N): ", this->disciplines[this->the_disCode]->name()); break;
                case MenuTask::CreateStudent: this->start_input_text("请按格式输入新生信息(%s): ", StudentEntity::prompt()); break;
                case MenuTask::UpdateStudent: this->start_input_text("请按格式输入学生[%s]的信息(%s): ", this->students[this->the_sNo]->name(), StudentEntity::update_prompt()); break;
                case MenuTask::UpdateAvatar: this->start_input_text("请按格式输入学生[%s]的信息(%s): ", this->students[this->the_sNo]->name(), StudentEntity::update_gender_prompt()); break;
                case MenuTask::DeleteStudent: this->start_input_text("是否确定删除学生[%s]的信息(Y/N): ", this->students[this->the_sNo]->name()); break;
                case MenuTask::CreateGrade: this->on_grade_task(false); break;
                case MenuTask::UpdateGrade: this->on_grade_task(false); break;
                case MenuTask::DeleteGrade: this->on_grade_task(false); break;
                case MenuTask::ClearStudent: this->start_input_text("是否确定清理游离学生的信息(Y/N): "); break;
                case MenuTask::ClearGrade: this->start_input_text("是否确定清理游离的成绩信息(Y/N): "); break;
                default: /* do nothing */;
                }
            } catch (const std::exception& e) {
                this->log_message(Log::Fatal, e.what());
            }
        }

        void on_grade_task(bool timestamp_okay) {
            this->the_grade_subtask = GradeTask::CheckCondition;

            if (timestamp_okay || this->check_timestamp_for_grade_task()) {
                this->the_grade_subtask = GradeTask::DoTask;

                switch (this->the_task) {
                case MenuTask::CreateGrade: {
                    this->start_input_text("请按顺序输入课程(%s@%llu)成绩的各项组成部分，以空格分隔",
                        this->disciplines[this->the_disCode]->name(),
                        this->the_timestamp);
                }; break;
                case MenuTask::UpdateGrade: {
                    this->start_input_text("请按顺序输入课程(%s@%llu)成绩的各项组成部分，以空格分隔",
                        this->disciplines[this->the_disCode]->name(),
                        this->the_timestamp);
                }; break;
                case MenuTask::DeleteGrade: {
                    this->model->delete_student_scores_as_user_request(this->the_sNo,
                        this->the_disCode, this->the_timestamp);
                    this->update_class_report(this->the_clsId);
                    this->update_student_report(this->the_sNo);
                    this->update_students_score_bars(this->the_sNo, this->the_disCode);
                    this->the_grade_subtask = GradeTask::_;
                 }; break;
                default: /* do nothing */; break;
                }
            }
        }

        bool check_timestamp_for_grade_task() {
            this->the_grade_subtask = GradeTask::CheckCondition;

            if (this->the_timestamp == 0U) {
                this->start_input_text("请按格式输入成绩时间戳(YYYYMMDD): ");
            } else {
                this->start_input_text("当前成绩时间戳 %llu, 直接修改或 ENTER 确认. ", this->the_timestamp);
            }

            return false;
        }

    public:
        void on_class_created(uint64_t pk, shared_class_t entity, bool in_batching) override {
            this->doors[pk] = this->insert(new DoorSprite(pk));
            this->doors[pk]->resize_by_height(platform_height * 0.72F);

            if (!in_batching) {
                this->reflow_class_logos(gliding_duration);
            }
        }

        void on_class_deleted(uint64_t pk, shared_class_t entity, bool in_batching) override {
            this->remove(this->doors[pk]);
            this->doors.erase(pk);

            if (this->the_clsId == pk) {
                this->on_class_changed(0U, in_batching);
            }

            if (!in_batching) {
                this->reflow_class_logos(gliding_duration);
            }
        }

        void on_class_changed(uint64_t clsId, bool in_batching) {
            if (this->doors.find(this->the_clsId) != this->doors.end()) {
                this->remove_selected(this->doors[this->the_clsId]);
                this->doors[this->the_clsId]->close();
            }

            this->the_clsId = (this->the_clsId != clsId) ? clsId : 0U;
            this->stuLabel->set_text(MatterPort::RB, " ");

            if (this->the_clsId > 0U) {
                this->doors[this->the_clsId]->open();
                this->clsLabel->set_text(MatterPort::RT, "%u班(%zu人)", this->the_clsId, this->model->get_class_population(this->the_clsId));
            } else {
                this->clsLabel->set_text(MatterPort::RB, " ");    
            }
            
            if (!in_batching) {
                this->update_class_report(this->the_clsId);
                this->reflow_students(gliding_duration);
                this->stuReport->clear();
            }
        }

        void on_discipline_created(uint64_t pk, shared_discipline_t entity, bool in_batching) override {
            this->disciplines[pk] = this->insert(new DisciplineSprite(entity->cannonical_type()));
            this->disciplines[pk]->resize_by_height(platform_height * 0.90F);

            if (!in_batching) {
                this->update_report_discipline(this->clsReport);
                this->update_report_discipline(this->stuReport);
                this->reflow_discipline_logos(gliding_duration);
            }
        }

        void on_discipline_deleted(uint64_t pk, shared_discipline_t entity, bool in_batching) override {
            this->remove(this->disciplines[pk]);
            this->disciplines.erase(pk);

            if (this->the_disCode == pk) {
                this->on_discipline_changed(0U, in_batching);
            }

            if (!in_batching) {
                this->update_report_discipline(this->clsReport);
                this->update_report_discipline(this->stuReport);
                this->reflow_discipline_logos(gliding_duration);
            }
        }

        void on_discipline_changed(uint64_t pk, bool in_batching) {
            if (this->disciplines.find(this->the_disCode) != this->disciplines.end()) {
                this->remove_selected(this->disciplines[this->the_disCode]);
            }
            
            this->the_disCode = pk;

            if (!in_batching) {
                this->update_students_score_bars(this->the_disCode);
                this->reflow_discipline_logos(gliding_duration);
            }
        }

        void on_student_created(uint64_t pk, shared_student_t entity, bool in_batching) override {
            this->students[pk] = this->insert(new StudentSprite(pk, entity->get_nickname(), entity->get_avatar()));

            if (!in_batching) {
                this->reflow_students(gliding_duration);
            }
        }

        void on_student_updated(uint64_t pk, shared_student_t entity) override {
            this->students[pk]->set_nickname(entity->get_nickname());
            this->on_student_changed(pk);
            this->log_message(Log::Info, make_nstring("学生(%s)信息已修改.", this->students[pk]->name()));
        }

        void on_student_avatar_updated(uint64_t pk, shared_student_t entity) override {
            this->on_student_deleted(pk, entity, true);
            this->on_student_created(pk, entity, false);
        }

        void on_student_deleted(uint64_t pk, shared_student_t entity, bool in_batching) override {
            this->remove(this->students[pk]);
            this->students.erase(pk);

            if (this->the_sNo == pk) {
                this->on_student_changed(0U);
            }

            if (!in_batching) {
                this->reflow_students(gliding_duration);
            }
        }

        void on_student_changed(uint64_t sNo) {
            if (this->students.find(this->the_sNo) != this->students.end()) {
                this->remove_selected(this->students[this->the_sNo]);
            }
            
            this->the_sNo = sNo;

            if (this->the_sNo > 0U) {
                this->stuLabel->set_text(MatterPort::RB, "%s", this->students[this->the_sNo]->name());
            } else {
                this->stuLabel->set_text(MatterPort::RB, "-");
            }

            this->update_student_report(this->the_sNo);
        }

    protected:
        void on_motion_complete(IMatter* m, float x, float y, double xspd, double yspd) override {
            auto dis = dynamic_cast<DisciplineSprite*>(m);
            auto stu = dynamic_cast<StudentSprite*>(m);

            if (dis != nullptr) {
                if (this->the_disCode > 0U) {
                    if (this->model->get_discipline_code(dis->get_type()) != this->the_disCode) {
                        dis->show(false);
                    }
                }
            } else if (stu != nullptr) {
                if (this->the_task == MenuTask::BindClass) {
                    stu->show(this->the_clsId == this->model->get_student_class(stu->primary_key()));

                    if (!stu->visible()) {
                        this->on_student_changed(0U);
                    }
                }
            }
        }

    private:
        void load_gui_elements(float width, float height) {
            this->agent = this->insert(new Linkmon());
            this->title = this->insert(new Labellet(GameFont::Title(), BLACK, "%s", this->name()));
            this->side_border = this->insert(new VLinelet(height, GRAY));
            this->tooltip = this->insert(make_label_for_tooltip(GameFont::fangsong()));
            
            this->set_sentry_sprite(this->agent);
            this->set_tooltip_matter(this->tooltip);
            this->agent->scale(-1.0F, 1.0F);
            this->tooltip->set_background_color(GHOSTWHITE);
        }

        void reflow_gui_elements(float width, float height) {
            float sidebar_width = this->calculate_sidebar_width();
            float gap = 3.0F;

            this->move_to(this->title, { this->agent, MatterPort::RB }, MatterPort::LB);
            this->move_to(this->side_border, { sidebar_width, height }, MatterPort::CB);
            
            for (auto menu : this->menus) {
                this->move_to(menu.second, { this->agent, MatterPort::LB }, MatterPort::LT, { 4.0F, 4.0F });
            }

            this->move_to(this->avatar, { 0.0F, height }, MatterPort::LB, { gap, -gap });
            this->move_to(this->stuReport, { 0.0F, height }, MatterPort::LB, { gap, -gap });
            this->move_to(this->clsReport, { this->stuReport, MatterPort::CT }, MatterPort::CB, { 0.0F, -gap });
        }

    private:
        void load_classroom(float width, float height) {
            this->platform = this->insert(new Rectanglet(platform_width, platform_height, STEELBLUE));
            this->clsLabel = this->insert(new Labellet(GameFont::monospace(FontSize::x_large), GHOSTWHITE, "[Class]"));
            this->stuLabel = this->insert(new Labellet(GameFont::serif(), GHOSTWHITE, "%s", "[Student]"));
            
            for (size_t idx = 1; idx <= DESK_COUNT; idx ++) {
                this->desks.push_back(this->insert(new HexagonalDesklet(idx, 90.0F, DARKORANGE)));
            }
        }

        void reflow_classroom(float width, float height) {
            float sidebar_pos = this->calculate_sidebar_width();
            float croom_width = width - sidebar_pos;

            this->move_to(this->platform, { croom_width * 0.50F + sidebar_pos, height * 0.95F }, MatterPort::CC);
            this->move_to(this->clsLabel, { this->platform, MatterPort::RT }, MatterPort::RT, { -4.0F, +4.0F });
            this->move_to(this->stuLabel, { this->platform, MatterPort::RB }, MatterPort::RB, { -4.0F, -4.0F });

            this->move_to(this->desks[0], { croom_width * 0.25F + sidebar_pos, height * 0.64F }, MatterPort::CC);
            this->move_to(this->desks[1], { croom_width * 0.50F + sidebar_pos, height * 0.75F }, MatterPort::CC);
            this->move_to(this->desks[2], { croom_width * 0.75F + sidebar_pos, height * 0.64F }, MatterPort::CC);
            this->move_to(this->desks[3], { croom_width * 0.25F + sidebar_pos, height * 0.32F }, MatterPort::CC);
            this->move_to(this->desks[4], { croom_width * 0.50F + sidebar_pos, height * 0.45F }, MatterPort::CC);
            this->move_to(this->desks[5], { croom_width * 0.75F + sidebar_pos, height * 0.32F }, MatterPort::CC);
            this->move_to(this->desks[6], { croom_width * 0.50F + sidebar_pos, height * 0.15F }, MatterPort::CC);

            this->reflow_model_sprites(0.0);
        }

        void reflow_model_sprites(double duration = gliding_duration) {
            this->reflow_class_logos(duration);
            this->reflow_discipline_logos(duration);
            this->reflow_students(duration);
        }

        void reflow_class_logos(double duration = gliding_duration) {
            if (!this->doors.empty()) {
                Dot spot = this->get_matter_location(this->side_border, MatterPort::CB);
                Box grid = this->doors.rbegin()->second->get_bounding_box() * 1.2F;

                for (auto cls = this->doors.rbegin(); cls != this->doors.rend(); cls ++) {
                    this->glide_to(duration, cls->second, spot, MatterPort::CB, { 0.0F, -1.0F });
                    spot.y -= grid.height();
                }
            }
        }

        void reflow_discipline_logos(double duration = gliding_duration) {
            if (!this->disciplines.empty()) {
                Dot gap(4.0F, 0.0F);
                Dot spot = this->get_matter_location(this->platform, MatterPort::LC) + gap;
                float grid_width = this->disciplines.begin()->second->get_bounding_box().width() + gap.x;
                float dis_x0 = spot.x;

                for (auto dis : this->disciplines) {
                    uint64_t disCode = this->model->get_discipline_code(dis.second->get_type());

                    if ((disCode == this->the_disCode) || (this->the_disCode == 0U)) {
                        dis.second->show(true);
                        this->glide_to(duration, dis.second, spot, MatterPort::LC);
                        spot.x += grid_width;
                    } else {
                        this->glide_to(duration, dis.second, { dis_x0, spot.y }, MatterPort::LC);
                    }
                }
            }
        }

        void reflow_students(double duration = gliding_duration) {
            if (!this->students.empty()) {
                Dot nocls_stu = this->get_matter_location(this->side_border, MatterPort::LB);
                Box grid = this->students.begin()->second->get_bounding_box();
                uint64_t desk_idx, seat_idx;
                float gap = 4.0F;
         
                nocls_stu.x *= 0.90F;
                grid.rbdot += { gap, gap };
                grid.ltdot.y = grid.height() * 3.0F;

                for (auto stu : this->students) {
                    uint64_t stuClsId = this->model->get_student_class(stu.second->primary_key());

                    stu.second->show((stuClsId == this->the_clsId) || (stuClsId == 0U));

                    if (stuClsId == 0U) {
                        this->glide_to(duration, stu.second, nocls_stu, MatterPort::RB);

                        if (nocls_stu.y > grid_y) {
                            nocls_stu.y -= grid.height();
                        } else {
                            nocls_stu.x -= grid.width();
                            nocls_stu.y = this->get_matter_location(this->side_border, MatterPort::LB).y;
                        }
                    } else {
                        if (stu.second->visible()) {
                            this->model->feed_student_seat(stu.first, &desk_idx, &seat_idx);

                            if ((desk_idx > 0U) && (seat_idx > 0U)) {
                                this->desks[desk_idx - 1]->sit(stu.second, seat_idx, duration);
                            } else {
                                this->glide_to(duration, stu.second, { this->doors[stuClsId], MatterPort::RC }, MatterPort::LC);
                            }
                        } else {
                            this->move_to(stu.second, { this->doors[stuClsId], MatterPort::RC }, MatterPort::LC);
                        }
                    }
                }
            }
        }

    private:
        void try_exchange_seat(HexagonalDesklet* dsk, float local_x, float local_y) {
            if (this->the_sNo > 0U) {
                if (this->the_clsId > 0U) {
                    int idx = dsk->get_seat_by(local_x, local_y);

                    if (idx > 0) {
                        uint64_t stuTarget = this->model->get_student_at_seat(this->the_clsId, dsk->get_index(), idx);
                            
                        dsk->sit(this->students[this->the_sNo], idx, gliding_duration);

                        if (stuTarget > 0U) { // 交换座位
                            uint64_t the_dsk, the_st;

                            this->model->feed_student_seat(this->the_sNo, &the_dsk, &the_st);

                            if (the_dsk > 0U) {
                                this->desks[the_dsk - 1]->sit(this->students[stuTarget], the_st, gliding_duration);
                                this->model->bind_student_to_seat(stuTarget, the_dsk, the_st);
                            } else {
                                this->model->bind_student_to_seat(stuTarget, 0, the_st);
                                this->glide_to(gliding_duration, this->students[stuTarget],
                                    { this->doors[this->the_clsId], MatterPort::RC },
                                    MatterPort::LC);
                            }
                        }

                        if (this->model->get_student_class(this->the_sNo) == 0U) {
                            this->model->bind_student_to_class(this->the_sNo, this->the_clsId);
                        }

                        this->model->bind_student_to_seat(this->the_sNo, dsk->get_index(), idx);
                    }
                } else {
                    this->log_message(Log::Error, "Hasn't Bound to a Class");
                }
            }
        }

    private:
        void switch_menu() {
            if ((this->the_disCode > 0U) || (this->the_sNo > 0U)) {
                if ((this->the_disCode > 0U) && (this->the_sNo > 0U)) {
                    this->on_menu_switch(the_menu_type, MenuType::Grade);
                } else if (this->the_sNo > 0U) {
                    this->on_menu_switch(the_menu_type, MenuType::Student);
                } else {
                    this->on_menu_switch(the_menu_type, MenuType::Discipline);
                }
            } else if ((this->the_clsId > 0U) && (this->is_selected(this->doors[this->the_clsId]))) {
                this->on_menu_switch(the_menu_type, MenuType::Class);
            } else {
                this->on_menu_switch(the_menu_type, MenuType::TopLevel);
            }
        }
        
        void load_menus(float width, float height) {
            this->menus[MenuType::TopLevel] = this->insert(new Continent(new TopLevelMenu()));
            this->menus[MenuType::Class] = this->insert(new Continent(new ClassMenu()));
            this->menus[MenuType::Discipline] = this->insert(new Continent(new DisciplineMenu()));
            this->menus[MenuType::Student] = this->insert(new Continent(new StudentMenu()));
            this->menus[MenuType::Grade] = this->insert(new Continent(new GradeMenu()));
            this->menus[MenuType::Clear] = this->insert(new Continent(new ClearMenu()));

            for (auto menu : this->menus) {
                if (this->the_menu_type != menu.first) {
                    menu.second->show(false);
                }

                menu.second->camouflage(true);
            }
        }

        void load_avatars(float width, float height) {
            this->avatar = this->insert(new Continent(new AvatarPlane("Student's Figure")));

            this->avatar->set_border_color(ROYALBLUE);
            this->avatar->set_background_color(RGBA(DIMGRAY, 0.64));

            this->avatar->show(false);
            this->avatar->camouflage(true);
        }

        void load_grade_reports(float width, float height) {
            this->clsReport = this->load_grade_report(width, height, default_class_report_title);
            this->stuReport = this->load_grade_report(width, height, default_student_report_title);
        }

        Gradelet* load_grade_report(float width, float height, const char* title) {
            auto report = this->insert(new Gradelet(title, this->calculate_sidebar_width() - 64.0F));

            this->update_report_discipline(report);
            
            return report;
        }

        void update_report_discipline(Gradelet* report) {
            std::vector<DisciplineType> actual_types;

            for (auto disType : report_disciplines) {
                uint64_t disCode = this->model->get_discipline_code(disType);

                if (disCode > 0U) {
                    actual_types.push_back(disType);
                }
            }

            report->set_disciplines(actual_types, MatterPort::LB);
        }

        void update_class_report(uint64_t clsId) {
            std::vector<double> avg_scores;
            uint64_t ts = this->model->get_class_latest_timestamp(this->the_clsId);

            if (ts > 0U) {
                for (auto disType : report_disciplines) {
                    uint64_t disCode = this->model->get_discipline_code(disType);

                    if (disCode > 0U) {
                        avg_scores.push_back(this->model->get_class_average_score(this->the_clsId, disCode, ts));
                    }
                }
            }

            if (clsId > 0U) {
                this->clsReport->set_title("%llu班 平均成绩", clsId);
            } else {
                this->clsReport->set_title(default_class_report_title, clsId);
            }

            this->clsReport->set_scores(ts, avg_scores);
        }

        void update_student_report(uint64_t sNo) {
            if (sNo == 0U) {
                this->stuReport->clear();
                this->stuReport->set_title("%s", default_student_report_title);
            } else {
                uint64_t lts = this->model->get_student_latest_timestamp(this->the_sNo);
                uint64_t pts = this->model->get_student_latest_timestamp(this->the_sNo, 1);

                if (lts > 0U) {
                    std::vector<std::vector<double>> s_pts;
                    std::vector<double> diffs;
    
                    for (auto disType : report_disciplines) {
                        uint64_t disCode = this->model->get_discipline_code(disType);
                        std::vector<double> lpts, ppts;

                        if (disCode > 0U) {
                            this->model->feed_student_score_points(sNo, disCode, lts, lpts);
                            this->model->feed_student_score_points(sNo, disCode, pts, ppts);
                        }

                        if (ppts.size() > 0) {
                            diffs.push_back(vector_sum(lpts) - vector_sum(ppts));
                        } else {
                            diffs.push_back(flnan);
                        }
                        
                        s_pts.push_back(lpts);
                    }

                    this->stuReport->set_score_diffs(diffs);
                    this->stuReport->set_score_via_points(lts, s_pts);
                }

                this->stuReport->set_title("%s[%u]", this->students[sNo]->name(), sNo);
            }
        }

        void update_students_score_bars(uint64_t disCode, uint64_t sNo = 0U) {
            if (sNo == 0U) {
                for (auto& stu : this->students) {
                    this->update_students_score_bars(disCode, stu.first);
                }
            } else if (this->model->get_student_class(sNo) == this->the_clsId) {
                if (disCode == 0U) {
                    this->students[sNo]->set_score_percentage(0.0);
                } else {
                    uint64_t ts = this->model->get_student_latest_timestamp(sNo);
                    uint32_t color = CRIMSON;
                    double score = 0.0;
                    
                    if (ts > 0U) {
                        score = this->model->get_student_score(sNo, disCode, ts);
                    }

                    if (score >= 80.0) {
                        color = GREEN;
                    } else if (score >= 60.0) {
                        color = ORANGE;
                    }

                    this->students[sNo]->set_score_percentage(score / 100.0, color);
                }
            }
        }

    private:
        float calculate_sidebar_width() {
            Box abox = this->agent->get_bounding_box();
            Box tbox = this->title->get_bounding_box();

            return tbox.width() + abox.width() * 1.618F;
        }

    private:
        Rectanglet* platform;
        std::vector<HexagonalDesklet*> desks;
    
    private:
        Labellet* title;
        Labellet* tooltip;
        Labellet* clsLabel;
        Labellet* stuLabel;
        Linelet* side_border;
        std::map<uint64_t, DoorSprite*> doors;
        std::map<uint64_t, DisciplineSprite*> disciplines;
        std::map<uint64_t, StudentSprite*> students;
        std::map<MenuType, Continent*> menus;
        Continent* avatar;
        Gradelet* stuReport;
        Gradelet* clsReport;
        Linkmon* agent;

    private:
        MenuType the_menu_type = MenuType::TopLevel;
        MenuTask the_task = MenuTask::_;
        uint64_t the_clsId = 0U;
        uint64_t the_disCode = 0U;
        uint64_t the_sNo = 0U;
        GMSModel* model;

    private:
        GradeTask the_grade_subtask = GradeTask::_;
        uint64_t the_timestamp = 0U;

    private:
        std::string gmsin;
        std::string gmsout;
    };

    /*********************************************************************************************/
    enum GMSCmdOpt { GMSIn, GMSOut, _ };

    class STEMClassroom : public Cosmos {
    public:
        STEMClassroom() : Cosmos(60) {}

    public:
        void construct(int argc, char* argv[]) override {
            enter_digimon_zone(argv[0]);
            imgdb_setup(digimon_subdir("stone"));

#ifdef __windows__
            digimon_appdata_setup("C:\\opt\\JrPLT\\");
            digimon_mascot_setup("C:\\opt\\JrPLT\\stone\\mascot");
#else
            digimon_appdata_setup("/opt/JrPLT/");
            digimon_mascot_setup("/opt/JrPLT/stone/mascot");
#endif

            this->parse_commandline_argument(argc, argv);
            this->set_snapshot_folder("/Users/wargrey/Desktop");
            this->set_cmdwin_height(24);

            GameFont::fontsize(20);

            this->push_plane(new STEMClassroomPlane(this->gmsin, this->gmsout));
        }

    private:
        void parse_commandline_argument(int argc, char* argv[]) {
            GMSCmdOpt opt = GMSCmdOpt::_;

            for (int idx = 1; idx < argc; idx ++) {
                switch (opt) {
                case GMSCmdOpt::GMSIn: {
                    this->gmsin = argv[idx];
                    opt = GMSCmdOpt::_;
                 }; break;
                case GMSCmdOpt::GMSOut: {
                    this->gmsout = argv[idx];
                    opt = GMSCmdOpt::_;
                }; break;
                default: {
                    if ((strcmp(argv[idx], "-i") == 0) || (strcmp(argv[idx], "--in") == 0)) {
                        opt = GMSCmdOpt::GMSIn;
                    } else if ((strcmp(argv[idx], "-o") == 0) || (strcmp(argv[idx], "--out") == 0)) {
                        opt = GMSCmdOpt::GMSOut;
                    }
                }; break;
                }
            }
        }

    private:
        std::string gmsin;
        std::string gmsout;
    };
}

/*************************************************************************************************/
int main(int argc, char* args[]) {
    STEMClassroom universe;

    universe.construct(argc, args);
    universe.big_bang();

    return 0;
}
