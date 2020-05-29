#include "PhotoOrganizerFrame.h"
#include <wx/wx.h>
#include <wx/dir.h>
#include <wx/string.h>
#include <wx/filefn.h> 
#include <string>
#include "FreeImage.h"

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

wxArrayString getAllDirectories(const wxDir& dir)
{
	wxArrayString directories;

	dir.GetAllFiles(dir.GetName(), &directories, wxEmptyString, wxDIR_DIRS);

	return directories;
}


PhotoOrganizerFrame::PhotoOrganizerFrame(wxWindow* parent)
	:
	Frame(parent)
{

}

void PhotoOrganizerFrame::m_loadFolderOnButtonClick(wxCommandEvent& event)
{
	wxString defaultPath = wxT("/");
	wxDirDialog dialog(this, wxT("Choose directory"), defaultPath, wxDD_NEW_DIR_BUTTON);
	if (dialog.ShowModal() == wxID_OK) {
		sourcePath = dialog.GetPath();
		//wxMessageBox(sourcePath);
		wxDir dir(sourcePath);
		if (dir.IsOpened()) {
			JPGfiles = getAllFilesInDirWithExtension(dir, "*.jpg");
			BMPfiles = getAllFilesInDirWithExtension(dir, "*.bmp");
			JPGcount = JPGfiles.GetCount();
			BMPcount = BMPfiles.GetCount();
			//wxMessageBox(std::to_string(JPGcount));
			//wxMessageBox(std::to_string(BMPcount));
		}

		for (wxString fileName : JPGfiles) {
			wxInitAllImageHandlers();
			wxImage image;
			wxString filePath = sourcePath + '\\' + fileName;
			//wxMessageBox(filePath);
			image.LoadFile(filePath);
			if (image.IsOk()) {
				wxMessageBox("Ok");
				break;
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
	compressionValue = m_compressiomLevel->GetValue();
}

void PhotoOrganizerFrame::m_folderNameOnText(wxCommandEvent& event)
{
	// TODO: Implement m_folderNameOnText
}

void PhotoOrganizerFrame::m_exportOnButtonClick(wxCommandEvent& event)
{
	FIBITMAP* bitmap;
	wxString defaultPath = wxT("/");
	wxDirDialog dialog(this, wxT("Choose directory"), defaultPath, wxDD_NEW_DIR_BUTTON);
	
	if (dialog.ShowModal() == wxID_OK) {
		targetPath = dialog.GetPath();
		wxDir dir(targetPath);
		wxDir source(sourcePath);

		if (dir.IsOpened()) {
			wxArrayString subdirectories;
			wxString dirname;
			bool cont = source.GetFirst(&dirname, wxEmptyString, wxDIR_DIRS);
			while (cont)
			{
				subdirectories.Add(dirname);
				wxMkdir(targetPath + '\\' + dirname);
				cont = dir.GetNext(&dirname);
			}

			for (int i = 0; i < JPGcount; i++)
			{
				wxString pathToFile = sourcePath + '\\' + JPGfiles.Item(i);
				wxString pathToTarget = targetPath + '\\' + JPGfiles.Item(i);

				bitmap = FreeImage_Load(FIF_JPEG, pathToFile, 0);
				if (bitmap)
				{
					FreeImage_Save(FIF_JPEG, bitmap, pathToTarget, compressionValue);
				}
			}
			wxMessageBox("Success!");
		}
	}
}
