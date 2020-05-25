#include "PhotoOrganizerFrame.h"
#include <wx/wx.h>
#include <wx/dir.h>
#include <wx/string.h>
#include <string>

wxArrayString getAllFilesInDirWithExtension(const wxDir &dir, std::string extension) {
	wxString fileName;
	wxArrayString files;
	bool cont = dir.GetFirst(&fileName, extension, wxDIR_DEFAULT);
	while (cont) {
		//wxMessageBox(fileName.c_str());
		files.Add(fileName);
		cont = dir.GetNext(&fileName);
	}
	return files;
}


PhotoOrganizerFrame::PhotoOrganizerFrame(wxWindow* parent)
	:
	Frame(parent)
{

}

void PhotoOrganizerFrame::m_loadFolderOnButtonClick(wxCommandEvent& event)
{
	wxString defaultPath = wxT("/");
	wxDirDialog dialog(this, wxT("Dir picker"), defaultPath, wxDD_NEW_DIR_BUTTON);
	if (dialog.ShowModal() == wxID_OK) {
		wxString path = dialog.GetPath();
		//wxMessageBox(path);
		wxDir dir(path);
		wxArrayString JPGfiles;
		wxArrayString BMPfiles;
		if (dir.IsOpened()) {
			JPGfiles = getAllFilesInDirWithExtension(dir, "*.jpg");
			BMPfiles = getAllFilesInDirWithExtension(dir, "*.bmp");
			int JPGcount = JPGfiles.GetCount();
			int BMPcount = BMPfiles.GetCount();
			//wxMessageBox(std::to_string(JPGcount));
			//wxMessageBox(std::to_string(BMPcount));
		}

		for (wxString fileName : JPGfiles) {
			wxInitAllImageHandlers();
			wxImage image;
			wxString filePath = path + '\\' + fileName;
			//wxMessageBox(filePath);
			image.LoadFile(filePath);
			if (image.IsOk()) {
				wxMessageBox("Ok");
			}
		}
	}

}

void PhotoOrganizerFrame::m_maxHeightControlOnSpinCtrl(wxSpinEvent& event)
{
	// TODO: Implement m_maxHeightControlOnSpinCtrl
}

void PhotoOrganizerFrame::m_maxHeightControlOnSpinCtrlText(wxCommandEvent& event)
{
	// TODO: Implement m_maxHeightControlOnSpinCtrlText
}

void PhotoOrganizerFrame::m_maxHeightControlOnTextEnter(wxCommandEvent& event)
{
	// TODO: Implement m_maxHeightControlOnTextEnter
}

void PhotoOrganizerFrame::m_maxWidthControlOnSpinCtrl(wxSpinEvent& event)
{
	// TODO: Implement m_maxWidthControlOnSpinCtrl
}

void PhotoOrganizerFrame::m_maxWidthControlOnSpinCtrlText(wxCommandEvent& event)
{
	// TODO: Implement m_maxWidthControlOnSpinCtrlText
}

void PhotoOrganizerFrame::m_maxWidthControlOnTextEnter(wxCommandEvent& event)
{
	// TODO: Implement m_maxWidthControlOnTextEnter
}

void PhotoOrganizerFrame::m_compressiomLevelOnSlider(wxCommandEvent& event)
{
	// TODO: Implement m_compressiomLevelOnSlider
}

void PhotoOrganizerFrame::m_folderNameOnText(wxCommandEvent& event)
{
	// TODO: Implement m_folderNameOnText
}

void PhotoOrganizerFrame::m_exportOnButtonClick(wxCommandEvent& event)
{
	wxString defaultPath = wxT("/");
	wxDirDialog dialog(this, wxT("Dir picker"), defaultPath, wxDD_NEW_DIR_BUTTON);
	if (dialog.ShowModal() == wxID_OK) {
		wxString path = dialog.GetPath();
		//wxMessageBox(path);
		wxDir dir(path);
		if (dir.IsOpened()) {
			wxMessageBox("Opened");
		}
	}
}
