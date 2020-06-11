#pragma once
#include "PhotoOrganizer.h"

//// end generated include
#include <wx/wx.h>
#include <wx/dir.h>
#include <wx/string.h>
#include <wx/filefn.h> 
#include <wx/image.h>
#include <wx/bitmap.h>
#include <wx/dcbuffer.h>
#include <wx/event.h>
#include <wx/utils.h> 
#include <iostream>
#include <string>
#include <vector>
#include "FreeImage.h"

/** Implementing Frame */
class PhotoOrganizerFrame : public PhotoOrganizer
{
public:
	PhotoOrganizerFrame(wxWindow* parent);
	~PhotoOrganizerFrame();

protected:
	
	// Event handlers
	void e_LoadFolderOnButtonClick(wxCommandEvent& event);
	void e_ExportOnButtonClick(wxCommandEvent& event);
	void e_UpdateUI(wxUpdateUIEvent& event);
	void e_OnKeyDown(wxKeyEvent& event);
	void e_WidthOnCheck(wxCommandEvent& event);
	void e_HeightOnCheck(wxCommandEvent& event);
	void e_SemiAutomaticModeOnCheck(wxCommandEvent& event);
	void e_MaxHeightControlOnSpinCtrl(wxSpinEvent& event);
	void e_MaxHeightControlOnSpinCtrlText(wxCommandEvent& event);
	void e_MaxHeightControlOnTextEnter(wxCommandEvent& event);
	void e_MaxWidthControlOnSpinCtrl(wxSpinEvent& event);
	void e_MaxWidthControlOnSpinCtrlText(wxCommandEvent& event);
	void e_MaxWidthControlOnTextEnter(wxCommandEvent& event);
	void e_CompressionLevelOnSlider(wxCommandEvent& event);

	void m_Repaint();
	void m_GetFilesPaths(const wxDir& dir);
	const wxArrayString m_GetAllFilesInDirWithExtension(const wxDir& dir, const wxString extension) const;
	void m_GoToNextFrame();
	void m_SaveOneImage(int index);
	void m_AddImageToContactSheet(FIBITMAP* contactSheet, FIBITMAP* bitmap, int& widthIndex, int& heightIndex);
	void m_CloneDir(wxString& source, wxString& target);
	bool m_IsImageToCopyInsideFolder(wxString& currPath) const;

private:	
	const std::vector<std::string> c_extensions = { "*.png", "*.jpg", "*.raw", "*.bmp", "*.tiff" };
	const std::vector<FREE_IMAGE_FORMAT> c_formats = { FIF_PNG, FIF_JPEG, FIF_RAW, FIF_BMP, FIF_TIFF };

	std::vector<FREE_IMAGE_FORMAT> m_loadedImagesFormats = {};
	wxArrayString& m_imagesPathArray = wxArrayString();
	
	wxString m_sourcePath = "";
	wxString m_destinationPath = "";
	wxImage* m_image;

	int m_imagesCounter = 0;
	int m_imagesSaved = 0;
	int m_compressionValue = m_compressionLevel->GetValue();
	int m_maxWidth = m_maxWidthControl->GetValue();
	int m_maxHeight = m_maxHeightControl->GetValue();
	int m_angle = 0;
	int m_contactSheetCount = 1;

	double m_ratio = 4. / 3.;

	bool m_isCustomWidth = m_checkBoxWidth->GetValue();
	bool m_isCustomHeight = m_checkBoxHeight->GetValue();
	bool m_isSemiAutomaticModeOn = m_checkBoxSemiAutomaticMode->GetValue();
	bool m_isLoadingImages = false;
};