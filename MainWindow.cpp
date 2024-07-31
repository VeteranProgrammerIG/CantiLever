#pragma once
#include "MainWindow.h"
#include <wx/valnum.h>
#include <cstdlib>
#include <time.h>
#include <format>
#include <chrono>
#include <thread>
#include <string>


MainWindow::MainWindow(const wxString &Title) : wxFrame(nullptr, wxID_ANY, Title, 
            wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)){
    // Now the user won't be able to resize the window. Muhahahaha!!!!
    
    AddControls();
    AddSizers();
    BindEventHandlers();
    SetUpGame();
}


void MainWindow::AddControls(){
    // 0 means choose a suitable width.
	// 36 means the font is 36 pixels tall.
    HeadingFont = new wxFont(wxFontInfo(wxSize(0, 36)).Bold());
    MainFont = new wxFont(wxFontInfo(wxSize(0, 24)));
    NumberFont = new wxFont(wxFontInfo(wxSize(0, 30)));

    // Allow positive integers and display them with thousands separators.
    // Only allows integers as the input from user.
    wxIntegerValidator<int> val(&m_value, wxNUM_VAL_THOUSANDS_SEPARATOR);
    

    Panel = new wxPanel(this);
    Panel -> SetFont(*MainFont);

    Heading = new wxStaticText(Panel, wxID_ANY, "Guess The Number", wxDefaultPosition, 
                wxDefaultSize, wxTE_CENTRE);
    Heading -> SetFont(*HeadingFont);

    //TimeLeft = new wxStaticText(Panel, wxID_ANY, "Time Left :- ");

    ChancesLeft = new wxStaticText(Panel, wxID_ANY, "Chances Left :- ");

    StartingRangeText = new wxStaticText(Panel, wxID_ANY, "Starting Range");
    StartingRangeNumber = new wxStaticText(Panel, wxID_ANY, "0");
    StartingRangeNumber -> SetFont(*NumberFont);

    EndRangeText = new wxStaticText(Panel, wxID_ANY, "End Range");
    EndRangeNumber = new wxStaticText(Panel, wxID_ANY, "50");
    EndRangeNumber -> SetFont(*NumberFont);

    GuessedNumberText = new wxStaticText(Panel, wxID_ANY, "Guess Your Number");
    GuessedNumber = new wxTextCtrl(Panel, wxID_ANY, "", wxDefaultPosition, 
                wxDefaultSize, wxTE_CENTRE | wxTE_PROCESS_ENTER | wxBORDER_NONE, val);
    GuessedNumber -> SetFont(*NumberFont);

    StatusLog = new wxStaticText(Panel, wxID_ANY, "");
}


void MainWindow::AddSizers(){
    // Although the user won't be able to resize window, I will keep SizerFlags and the
    // sizers themselves intact because removing them will take time.
    // Plus I find them much easier than absolute positioning.

    MainSizer = new wxBoxSizer(wxVERTICAL);

    MainSizer -> Add(Heading, wxSizerFlags().Expand());
    MainSizer -> AddSpacer(25);

    ChanceAndTimeSizer = new wxBoxSizer(wxHORIZONTAL);
    //ChanceAndTimeSizer -> Add(TimeLeft);
    //ChanceAndTimeSizer -> AddStretchSpacer(1);
    ChanceAndTimeSizer -> Add(ChancesLeft);

    MainSizer -> Add(ChanceAndTimeSizer, wxSizerFlags().Expand());
    MainSizer -> AddSpacer(35);

    NumbersHeadingSizer = new wxBoxSizer(wxHORIZONTAL);
    NumbersHeadingSizer -> AddSpacer(50);
    NumbersHeadingSizer -> Add(StartingRangeText);
    NumbersHeadingSizer -> AddSpacer(30);
    NumbersHeadingSizer -> Add(EndRangeText);
    NumbersHeadingSizer -> AddSpacer(70);
    NumbersHeadingSizer -> Add(GuessedNumberText);

    MainSizer -> Add(NumbersHeadingSizer, wxSizerFlags().Expand());

    NumbersSizer = new wxBoxSizer(wxHORIZONTAL);
    NumbersSizer -> AddSpacer(120);
    NumbersSizer -> Add(StartingRangeNumber);
    NumbersSizer -> AddSpacer(140);
    NumbersSizer -> Add(EndRangeNumber);
    NumbersSizer -> AddSpacer(150);
    NumbersSizer -> Add(GuessedNumber);

    MainSizer -> Add(NumbersSizer, wxSizerFlags().Expand());
    MainSizer -> AddSpacer(50);

    MainSizer -> Add(StatusLog, wxSizerFlags().Expand());

    OuterSizer = new wxBoxSizer(wxVERTICAL);
    OuterSizer -> Add(MainSizer, wxSizerFlags().Border(wxALL, 25));

    Panel -> SetSizer(OuterSizer);
    OuterSizer -> SetSizeHints(this); // The frame now respects the min size of OuterSizer.
}


void inline MainWindow::BindEventHandlers(){
    GuessedNumber -> Bind(wxEVT_KEY_DOWN, &MainWindow::OnEnterKeyPress, this);
}


void inline MainWindow::UnBindEventHandlers(){
    GuessedNumber -> Unbind(wxEVT_KEY_DOWN, &MainWindow::OnEnterKeyPress, this);
}



void MainWindow::SetUpGame(){
    // Generating a random number between 0 and 50 (Both inclusive).
    
    // Use current time as seed for random generator.
    // This makes sure that the RNG generates random numbers every time the program runs 
    // by using a different seed every time.
    srand(time(0));

    m_RandomNumber = rand() % 51;

    m_Chances = 5;
    ChancesLeft -> SetLabel(std::format("Chances Left :- {}", m_Chances));

    //m_Seconds = 10;
    //TimeLeft -> SetLabel(std::format("Time Left :- {}", m_Seconds));

    //HasUserWon = false;
}


/* void MainWindow::StartGame(){
    auto t1 = std::chrono::system_clock::now();
    
    while(m_Seconds > 0 && m_Chances > 0){

        if ((std::chrono::system_clock::now() - t1) >= std::chrono::seconds(1)){
            TimeLeft -> SetLabel(std::format("Time Left :- {}", --m_Seconds));
            t1 = std::chrono::system_clock::now();
        }

        if (HasUserWon){
            StatusLog -> SetLabel("Congratulations!! You win !!!");
            break;
        }

    }

    if (!HasUserWon){
        StatusLog -> SetLabel("You Lose :'(  Better Luck next time.");
    }
} */


void MainWindow::CheckUserGuess(){
    // stoi stands for String to Integer.
    int UserGuess = std::stoi((std::string)GuessedNumber -> GetValue());

    if (UserGuess == m_RandomNumber){
        StatusLog -> SetLabel("Congratulations!! You won !!!");
        //HasUserWon = true;
        UnBindEventHandlers();

        return;

    }else{
        ChancesLeft -> SetLabel(std::format("Chances Left :- {}", --m_Chances));
        GuessedNumber -> Clear();
        
        if (UserGuess > m_RandomNumber){

            StatusLog -> SetLabel("The guessed number is greater than Random number.");
        }
        
        else{
            StatusLog -> SetLabel("The guessed number is smaller than Random number.");
        }
    }


    if (m_Chances <= 0){
        StatusLog -> SetLabel("You Lose :'(  Better Luck next time.");
        UnBindEventHandlers();
    }
}


void MainWindow::OnEnterKeyPress(wxKeyEvent &evt)
{
    if (evt.GetKeyCode() == WXK_RETURN || evt.GetKeyCode() == WXK_NUMPAD_ENTER 
                    && GuessedNumber -> HasFocus()){
        CheckUserGuess();
    }

    evt.Skip();
}