#include "gui_dialog.h"
#include "gui_manage.h"

GUIDialog::GUIDialog( const char* filename, const Coordinate position) :
    GUI( filename, position, g_gui_manage->GetRoot()) {
  type_ = GUI_DIALOG;
}

GUIDialog::~GUIDialog() {

}

void GUIDialog::EventMouseLeftHolding() {
  Coordinate mouse_position = g_gui_manage->GetMousePosition();
  Coordinate mouse_position_previous = g_gui_manage->GetMousePositionPrevious();
  coordinate_.Plus( mouse_position - mouse_position_previous);
}