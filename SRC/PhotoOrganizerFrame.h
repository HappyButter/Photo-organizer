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

	//Function calls m_GetFilesPaths(), and saves path to source directory,
	// pops up a modal if it encounters an error.
	void e_LoadFolderOnButtonClick(wxCommandEvent& event);
	//Opens target directory and saves it's path,
	// calls m_CloneDir() to replicate source directory tree
	// and checks what mode is the app running in, if the mode is automatic it also calls m_SaveOneImage() for every image.
	// If the mode is semi-automatic it calls m_GoToNextFrame().
	void e_ExportOnButtonClick(wxCommandEvent& event);
	// Updates the interface, calls Repaint().
	void e_UpdateUI(wxUpdateUIEvent& event);
	// Gets user input in semi-automatic mode.
	// Left Arrow - rotate left, Right Arrow - rotate right, Enter - save image.
	void e_OnKeyDown(wxKeyEvent& event);
	// Checks if width constraint is on, if not the text field is blocked.
	void e_WidthOnCheck(wxCommandEvent& event);
	// Checks if height constraint is on, if not the text field is blocked.
	void e_HeightOnCheck(wxCommandEvent& event);
	// Checks if semi-automatic mode is on, if not the checkbox is blocked.
	void e_SemiAutomaticModeOnCheck(wxCommandEvent& event);
	
	// Functions controlling size constraints for thumbnails:
	void e_MaxHeightControlOnSpinCtrl(wxSpinEvent& event);
	void e_MaxHeightControlOnSpinCtrlText(wxCommandEvent& event);
	void e_MaxHeightControlOnTextEnter(wxCommandEvent& event);
	void e_MaxWidthControlOnSpinCtrl(wxSpinEvent& event);
	void e_MaxWidthControlOnSpinCtrlText(wxCommandEvent& event);
	void e_MaxWidthControlOnTextEnter(wxCommandEvent& event);
	// Sets compression level with value of the slider.
	void e_CompressionLevelOnSlider(wxCommandEvent& event);

	// Repaints the image updating wxPanel.
	void m_Repaint();
	// Saves image paths from directory to m_imagePathsArray
	//  and corresponding file extensions to m_loadedImagesFormats.
	void m_GetFilesPaths(const wxDir& dir);
	// Returns all the files within a directory with the extension passed as an argument.
	const wxArrayString m_GetAllFilesInDirWithExtension(const wxDir& dir, const wxString extension) const;
	// Loads an image to the display and calls m_Repaint(), if there are no more images clears the display
	// and sets a flag to false for the program to end.
	void m_GoToNextFrame();
	// Saves a single rescaled image to target directory.
	void m_SaveOneImage(int index);
	// Adds an image to contact sheet.
	void m_AddImageToContactSheet(FIBITMAP* contactSheet, FIBITMAP* bitmap, int& widthIndex, int& heightIndex);
	// Creates contact sheets in each directory.
	// Traverses through source directory tree,
	// copying the subdirectories to target tree using both iteration and recursion.
	void m_CloneDir(wxString& source, wxString& target);
	// Checks for presence of images in supported formats in a directory
	// using both iteration and recursion.
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