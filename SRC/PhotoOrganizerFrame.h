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
	// Handlers for Frame events.
	void m_loadFolderOnButtonClick(wxCommandEvent& event);
	void m_maxHeightControlOnSpinCtrl(wxSpinEvent& event);
	void m_maxHeightControlOnSpinCtrlText(wxCommandEvent& event);
	void m_maxHeightControlOnTextEnter(wxCommandEvent& event);
	void m_maxWidthControlOnSpinCtrl(wxSpinEvent& event);
	void m_maxWidthControlOnSpinCtrlText(wxCommandEvent& event);
	void m_maxWidthControlOnTextEnter(wxCommandEvent& event);
	void m_compressiomLevelOnSlider(wxCommandEvent& event);
	void m_exportOnButtonClick(wxCommandEvent& event);
	void m_isSemiAutomaticModeOn(wxCommandEvent& event);
	void m_isWidthBoxChecked(wxCommandEvent& event);
	void m_isHeightBoxChecked(wxCommandEvent& event);
	void UpdateUI(wxUpdateUIEvent& event);
	void Repaint();

	const wxArrayString getAllFilesInDirWithExtension(const wxDir& dir, const wxString extension) const;

	
	void addImageToContactSheet(FIBITMAP* contactSheet, FIBITMAP* bitmap, int& widthIndex, int& heightIndex);

	
	/*	NEW BEGINNING  */
	void copyAllImages(wxString& currPath, wxString& targetPath); // zostawiam tylko dla Micha³a; usunac po zrobieniu stykowek

	void getFilesPaths(const wxDir& dir);
	void Go_To_Next_Frame();
	void saveOneImage(int index);
	void cloneDir(wxString& source, wxString& target);
	void OnKeyDown(wxKeyEvent& event);
	bool isImageToCopyInsideFolder(wxString& currPath) const;
private:
	/*	NEW BEGINNING  */

	wxArrayString& m_imagesPathArray = wxArrayString();
	std::vector<FREE_IMAGE_FORMAT> m_imagesFromats = {};
	std::vector<std::string> m_extensions;
	std::vector<FREE_IMAGE_FORMAT> m_formats;
	wxString sourcePath = "";
	wxString directionPath = "";
	wxImage* m_image;


	int m_imagesCounter = 0;
	int m_imagesLoaded = 0;
	int compressionValue = 50;
	int maxWidth = m_maxWidthControl->GetValue();
	int maxHeight = m_maxHeightControl->GetValue();
	int angle = 0;
	int contactSheetCount = 1;
	
	double ratio = 4. / 3.;

	bool isCustomWidth = m_checkBoxWidth->GetValue();
	bool isCustomHeight = m_checkBoxHeight->GetValue();
	bool isSemiAutomaticModeOn = m_checkBoxSemiAutomaticMode->GetValue();
	bool m_isLoadingImages = false;

	/*  OLD STUFF*/
};