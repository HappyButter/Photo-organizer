#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/gdicmn.h>
#include <wx/button.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/checkbox.h>
#include <wx/stattext.h>
#include <wx/spinctrl.h>
#include <wx/sizer.h>
#include <wx/slider.h>
#include <wx/textctrl.h>
#include <wx/gauge.h>
#include <wx/panel.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class PhotoOrganizer
///////////////////////////////////////////////////////////////////////////////
class PhotoOrganizer : public wxFrame
{
public:
	PhotoOrganizer(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Photo Organizer"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(836, 499), long style = wxDEFAULT_FRAME_STYLE | wxTAB_TRAVERSAL);
	~PhotoOrganizer();

protected:
	wxButton* m_loadFolder;
	wxSpinCtrl* m_maxHeightControl;
	wxSpinCtrl* m_maxWidthControl;
	wxStaticText* m_staticText4;
	wxStaticText* m_staticText5;
	wxSlider* m_compressiomLevel;
	wxStaticText* m_staticText8;
	wxButton* m_export;
	wxGauge* m_progressBar;
	wxPanel* m_panel;
	wxCheckBox* m_checkBoxWidth;
	wxCheckBox* m_checkBoxHeight;
	wxCheckBox* m_checkBoxSemiAutomaticMode;
	wxKeyEvent* m_key;


	// Virtual event handlers, overide them in your derived class
	virtual void m_loadFolderOnButtonClick(wxCommandEvent& event) { event.Skip(); }
	virtual void m_maxHeightControlOnSpinCtrl(wxSpinEvent& event) { event.Skip(); }
	virtual void m_maxHeightControlOnSpinCtrlText(wxCommandEvent& event) { event.Skip(); }
	virtual void m_maxHeightControlOnTextEnter(wxCommandEvent& event) { event.Skip(); }
	virtual void m_maxWidthControlOnSpinCtrl(wxSpinEvent& event) { event.Skip(); }
	virtual void m_maxWidthControlOnSpinCtrlText(wxCommandEvent& event) { event.Skip(); }
	virtual void m_maxWidthControlOnTextEnter(wxCommandEvent& event) { event.Skip(); }
	virtual void m_compressiomLevelOnSlider(wxCommandEvent& event) { event.Skip(); }
	virtual void m_exportOnButtonClick(wxCommandEvent& event) { event.Skip(); }
	virtual void m_isWidthBoxChecked(wxCommandEvent& event) { event.Skip(); }
	virtual void m_isHeightBoxChecked(wxCommandEvent& event) { event.Skip(); }
	virtual void m_isSemiAutomaticModeOn(wxCommandEvent& event) { event.Skip(); }
	virtual void UpdateUI(wxUpdateUIEvent& event) { event.Skip(); }

};