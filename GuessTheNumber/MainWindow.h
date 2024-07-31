#pragma once
#include <wx/wx.h>

class MainWindow : public wxFrame{
public:
    MainWindow(const wxString& Title);

    wxFont* HeadingFont = nullptr;
    wxFont* MainFont = nullptr;
    wxFont* NumberFont = nullptr;


    int m_value;
    int m_RandomNumber;
    int m_Chances;
    int m_Seconds;

    bool HasUserWon;

    wxPanel* Panel = nullptr;
    wxStaticText* Heading = nullptr;
    wxStaticText* TimeLeft = nullptr;
    wxStaticText* ChancesLeft = nullptr;
    wxStaticText* StartingRangeText = nullptr;
    wxStaticText* StartingRangeNumber = nullptr;
    wxStaticText* EndRangeText = nullptr;
    wxStaticText* EndRangeNumber = nullptr;
    wxStaticText* GuessedNumberText = nullptr;
    wxTextCtrl* GuessedNumber = nullptr;
    wxStaticText* StatusLog = nullptr;

    wxSizer* OuterSizer = nullptr;
    wxSizer* MainSizer = nullptr;
    wxSizer* ChanceAndTimeSizer = nullptr;
    wxSizer* NumbersHeadingSizer = nullptr;
    wxSizer* NumbersSizer = nullptr;

    void AddControls();
    void AddSizers();
    void inline BindEventHandlers();
    void inline UnBindEventHandlers();
    void SetUpGame();
    void StartGame();

    void CheckUserGuess();
    void OnEnterKeyPress(wxKeyEvent &evt); 
};