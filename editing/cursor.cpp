#include "cursor.h"



namespace ReShaked {

Tick Cursor::get_tick() {

    return cursor_tick;

}
void Cursor::set_tick(Tick p_tick) {

    cursor_tick=p_tick;

}
int Cursor::get_track() {

    return current_track;
}
void Cursor::set_track(int p_track) {


    current_track=p_track;
}
int Cursor::get_track_automation() {

    return current_automation;

}
void Cursor::set_edited_track_automation(int p_track_automation) {

    current_automation=p_track_automation;
}
void Cursor::set_snap(Tick p_snap) {

    snap=p_snap;
}
Tick Cursor::get_snap() {

    return snap;

}


Cursor::Cursor() {

    cursor_tick=0;
    current_track=0;
    current_automation=0;
    edit_target=EDIT_TRACK;
    snap=0;
}

};
