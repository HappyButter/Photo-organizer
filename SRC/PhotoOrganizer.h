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
	wxSlider* m_compressionLevel;
	wxStaticText* m_staticText8;
	wxButton* m_export;
	wxGauge* m_progressBar;
	wxPanel* m_panel;
	wxCheckBox* m_checkBoxWidth;
	wxCheckBox* m_checkBoxHeight;
	wxCheckBox* m_checkBoxSemiAutomaticMode;
	wxKeyEvent* m_key;

	virtual void e_LoadFolderOnButtonClick(wxCommandEvent& event) { event.Skip(); }
	virtual void e_ExportOnButtonClick(wxCommandEvent& event) { event.Skip(); }
	virtual void e_UpdateUI(wxUpdateUIEvent& event) { event.Skip(); }
	virtual void e_WidthOnCheck(wxCommandEvent& event) { event.Skip(); }
	virtual void e_HeightOnCheck(wxCommandEvent& event) { event.Skip(); }
	virtual void e_SemiAutomaticModeOnCheck(wxCommandEvent& event) { event.Skip(); }
	virtual void e_MaxHeightControlOnSpinCtrl(wxSpinEvent& event) { event.Skip(); }
	virtual void e_MaxHeightControlOnSpinCtrlText(wxCommandEvent& event) { event.Skip(); }
	virtual void e_MaxHeightControlOnTextEnter(wxCommandEvent& event) { event.Skip(); }
	virtual void e_MaxWidthControlOnSpinCtrl(wxSpinEvent& event) { event.Skip(); }
	virtual void e_MaxWidthControlOnSpinCtrlText(wxCommandEvent& event) { event.Skip(); }
	virtual void e_MaxWidthControlOnTextEnter(wxCommandEvent& event) { event.Skip(); }
	virtual void e_CompressionLevelOnSlider(wxCommandEvent& event) { event.Skip(); }
};