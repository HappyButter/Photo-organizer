#include "PhotoOrganizer.h"

///////////////////////////////////////////////////////////////////////////

PhotoOrganizer::PhotoOrganizer(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxFrame(parent, id, title, pos, size, style)
{
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer(wxVERTICAL);

	m_loadFolder = new wxButton(this, wxID_ANY, wxT("Load Folder"), wxDefaultPosition, wxSize(120, 30), 0);
	m_loadFolder->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BACKGROUND));
	m_loadFolder->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));

	bSizer2->Add(m_loadFolder, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer(wxHORIZONTAL);

	m_checkBoxWidth = new wxCheckBox(this, wxID_ANY, wxT("Max Width"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer5->Add(m_checkBoxWidth, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	m_maxWidthControl = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), wxSP_ARROW_KEYS, 20, 2000, 400);
	bSizer5->Add(m_maxWidthControl, 0, wxALL, 5);

	bSizer4->Add(bSizer5, 1, wxALL, 5);

	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer(wxHORIZONTAL);

	m_checkBoxHeight = new wxCheckBox(this, wxID_ANY, wxT("Max Height"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer6->Add(m_checkBoxHeight, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	m_maxHeightControl = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), wxSP_ARROW_KEYS, 20, 3000, 300);
	bSizer6->Add(m_maxHeightControl, 0, wxALL, 5);

	bSizer4->Add(bSizer6, 0, wxALL, 5);
	bSizer2->Add(bSizer4, 0, wxSHAPED, 1);

	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer(wxVERTICAL);

	m_staticText4 = new wxStaticText(this, wxID_ANY, wxT("Compression level"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText4->Wrap(-1);
	bSizer7->Add(m_staticText4, 0, wxALIGN_CENTER_HORIZONTAL | wxALL | wxBOTTOM, 5);

	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer(wxHORIZONTAL);

	m_staticText5 = new wxStaticText(this, wxID_ANY, wxT("minimal"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText5->Wrap(-1);
	bSizer8->Add(m_staticText5, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	m_compressionLevel = new wxSlider(this, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxSize(120, -1), wxSL_HORIZONTAL);
	bSizer8->Add(m_compressionLevel, 0, wxALL, 10);

	m_staticText8 = new wxStaticText(this, wxID_ANY, wxT("maximal"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText8->Wrap(-1);
	bSizer8->Add(m_staticText8, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);


	bSizer7->Add(bSizer8, 0, wxALIGN_CENTER_HORIZONTAL | wxSHAPED, 5);

	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer(wxVERTICAL);


	m_checkBoxSemiAutomaticMode = new wxCheckBox(this, wxID_ANY, wxT("Semi Automatic Mode"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer11->Add(m_checkBoxSemiAutomaticMode, 0, wxALIGN_CENTER | wxALL, 5);

	m_export = new wxButton(this, wxID_ANY, wxT("Export"), wxDefaultPosition, wxSize(120, 40), 0);
	m_export->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BACKGROUND));
	m_export->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));

	bSizer11->Add(m_export, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);


	bSizer7->Add(bSizer11, 0, wxALIGN_CENTER_HORIZONTAL, 5);

	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer(wxVERTICAL);

	m_progressBar = new wxGauge(this, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL);
	m_progressBar->SetValue(0);
	bSizer12->Add(m_progressBar, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 20);


	bSizer7->Add(bSizer12, 1, wxEXPAND, 5);


	bSizer2->Add(bSizer7, 0, wxALIGN_CENTER_HORIZONTAL | wxBOTTOM | wxEXPAND, 5);


	bSizer1->Add(bSizer2, 0, wxEXPAND, 5);

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer(wxVERTICAL);

	m_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(600, 600), wxTAB_TRAVERSAL);
	m_panel->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWFRAME));


	bSizer3->Add(m_panel, 1, wxEXPAND | wxALL, 5);


	bSizer1->Add(bSizer3, 1, wxEXPAND, 5);


	this->SetSizer(bSizer1);
	this->Layout();

	this->Centre(wxBOTH);

	// Connect Events
	m_checkBoxWidth->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(PhotoOrganizer::e_WidthOnCheck), NULL, this);
	m_checkBoxHeight->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(PhotoOrganizer::e_HeightOnCheck), NULL, this);
	m_checkBoxSemiAutomaticMode->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(PhotoOrganizer::e_SemiAutomaticModeOnCheck), NULL, this);
	m_loadFolder->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(PhotoOrganizer::e_LoadFolderOnButtonClick), NULL, this);
	m_export->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(PhotoOrganizer::e_ExportOnButtonClick), NULL, this);
	Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(PhotoOrganizer::e_UpdateUI));
	m_maxHeightControl->Connect(wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(PhotoOrganizer::e_MaxHeightControlOnSpinCtrl), NULL, this);
	m_maxHeightControl->Connect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(PhotoOrganizer::e_MaxHeightControlOnSpinCtrlText), NULL, this);
	m_maxHeightControl->Connect(wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(PhotoOrganizer::e_MaxHeightControlOnTextEnter), NULL, this);
	m_maxWidthControl->Connect(wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(PhotoOrganizer::e_MaxWidthControlOnSpinCtrl), NULL, this);
	m_maxWidthControl->Connect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(PhotoOrganizer::e_MaxWidthControlOnSpinCtrlText), NULL, this);
	m_maxWidthControl->Connect(wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(PhotoOrganizer::e_MaxWidthControlOnTextEnter), NULL, this);
	m_compressionLevel->Connect(wxEVT_SLIDER, wxCommandEventHandler(PhotoOrganizer::e_CompressionLevelOnSlider), NULL, this);
}

PhotoOrganizer::~PhotoOrganizer()
{
	// Disconnect Events
	m_checkBoxWidth->Disconnect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(PhotoOrganizer::e_WidthOnCheck), NULL, this);
	m_checkBoxHeight->Disconnect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(PhotoOrganizer::e_HeightOnCheck), NULL, this);
	m_checkBoxSemiAutomaticMode->Disconnect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(PhotoOrganizer::e_SemiAutomaticModeOnCheck), NULL, this);
	m_loadFolder->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(PhotoOrganizer::e_LoadFolderOnButtonClick), NULL, this);
	m_export->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(PhotoOrganizer::e_ExportOnButtonClick), NULL, this);
	Disconnect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(PhotoOrganizer::e_UpdateUI));
	m_maxHeightControl->Disconnect(wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(PhotoOrganizer::e_MaxHeightControlOnSpinCtrl), NULL, this);
	m_maxHeightControl->Disconnect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(PhotoOrganizer::e_MaxHeightControlOnSpinCtrlText), NULL, this);
	m_maxHeightControl->Disconnect(wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(PhotoOrganizer::e_MaxHeightControlOnTextEnter), NULL, this);
	m_maxWidthControl->Disconnect(wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(PhotoOrganizer::e_MaxWidthControlOnSpinCtrl), NULL, this);
	m_maxWidthControl->Disconnect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(PhotoOrganizer::e_MaxWidthControlOnSpinCtrlText), NULL, this);
	m_maxWidthControl->Disconnect(wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(PhotoOrganizer::e_MaxWidthControlOnTextEnter), NULL, this);
	m_compressionLevel->Disconnect(wxEVT_SLIDER, wxCommandEventHandler(PhotoOrganizer::e_CompressionLevelOnSlider), NULL, this);

}