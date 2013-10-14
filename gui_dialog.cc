#include "gui_dialog.h"
#include "gui_manage.h"

GUIDialog::GUIDialog( const char* filename, const Coordinate position) :
    GUI( filename, position, g_gui_manage->GetRoot()) {
  
}

GUIDialog::~GUIDialog() {

}