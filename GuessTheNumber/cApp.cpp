#pragma once
#include "cApp.h"
#include "MainWindow.h"
#include <wx/wx.h>
#include <thread>

wxIMPLEMENT_APP(cApp);

bool cApp::OnInit(){
    window = new MainWindow("Guess The Number");
    window -> SetClientSize(750, 350);
    window -> Center();
    window -> Show();

    //window -> StartGame();

    return true;
}
