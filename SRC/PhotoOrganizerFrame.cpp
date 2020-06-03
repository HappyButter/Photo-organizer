#include "PhotoOrganizerFrame.h"

const wxArrayString PhotoOrganizerFrame::getAllFilesInDirWithExtension(const wxDir& dir, const wxString extension) const
{
	wxString fileName;
	wxArrayString files;
	bool cont = dir.GetFirst(&fileName, extension, wxDIR_DEFAULT);
	while (cont) {
		files.Add(fileName);
		cont = dir.GetNext(&fileName);
	}
	return files;
}


void PhotoOrganizerFrame::copyAllImages(wxString& currPath, wxString& targetPath)
{
	int index = 0;
	int setWidth = maxWidth;
	int setHeight = maxHeight;
	double ratio = 4. / 3.;

	wxArrayString& files = wxArrayString();
	FIBITMAP* bitmap;
	FIBITMAP* bitmapRescaled;
	wxDir curr(currPath);

	for (const auto& format : m_extensions)
	{
		files = getAllFilesInDirWithExtension(curr, format);
		for (wxString name : files)
		{
			wxString pathToFile = currPath + '\\' + name;
			wxString pathToTarget = targetPath + '\\' + name;

			bitmap = FreeImage_Load(m_formats[index], pathToFile, 0);
			if (bitmap)
			{
				ratio = FreeImage_GetWidth(bitmap) / double(FreeImage_GetHeight(bitmap));
				if (FreeImage_GetWidth(bitmap) > maxWidth)
				{
					setWidth = maxWidth;
					setHeight = setWidth / ratio;
				}
				else if (FreeImage_GetHeight(bitmap) > maxHeight)
				{
					setHeight = maxHeight;
					setWidth = ratio * setHeight;
				}
				else
				{
					setHeight = FreeImage_GetHeight(bitmap);
					setWidth = FreeImage_GetWidth(bitmap);
				}

				bitmapRescaled = FreeImage_Rescale(bitmap, setWidth, setHeight);
				FreeImage_Save(FIF_JPEG, bitmapRescaled, pathToTarget, compressionValue);
				FreeImage_Unload(bitmap);
				FreeImage_Unload(bitmapRescaled);

				m_filesCompressionCounter++;
				m_progressBar->SetValue((int)((m_filesCompressionCounter * 100) / m_filesCount));
			}	
		}
		index++;
	}
}

bool PhotoOrganizerFrame::isImageToCopyInsideFolder(wxString& currPath) const
{
	bool isInsideImage = false;
	wxDir dir(currPath);
	wxString dirName = "";
	wxArrayString& files = wxArrayString();
	wxArrayString subdirectories;

	for (const auto& extension : m_extensions)
	{
		files = getAllFilesInDirWithExtension(dir, extension);
		
		if (files.GetCount() > 0)
			return true;
	}

	if (dir.HasSubDirs())
	{
		bool cont = dir.GetFirst(&dirName, wxEmptyString, wxDIR_DIRS);
		while (cont)
		{
			if (directionPath != currPath + '\\' + dirName)
				subdirectories.Add(dirName);

			cont = dir.GetNext(&dirName);
		}

		for (wxString sub : subdirectories)
		{
			if (isImageToCopyInsideFolder(currPath + '\\' + sub))
				return true;
		}
	}
	return false;
}


void PhotoOrganizerFrame::cloneDir(wxString& currPath, wxString& targetPath)
{
	wxArrayString subdirectories;
	wxString dirName;
	wxDir source(currPath);
	wxDir target(targetPath);

	copyAllImages(currPath, targetPath);

	if (source.HasSubDirs())
	{
		bool cont = source.GetFirst(&dirName, wxEmptyString, wxDIR_DIRS);
		while (cont)
		{
			if (directionPath != currPath + '\\' + dirName && isImageToCopyInsideFolder(currPath + '\\' + dirName))
			{
				subdirectories.Add(dirName);
				wxMkdir(targetPath + '\\' + dirName);
			}
			cont = source.GetNext(&dirName);
		}

		for (wxString sub : subdirectories)
			cloneDir(currPath + '\\' + sub, targetPath + '\\' + sub);
	}
}


PhotoOrganizerFrame::PhotoOrganizerFrame(wxWindow* parent)
	: PhotoOrganizer(parent),
	m_extensions{ "*.png", "*.jpg", "*.raw", "*.bmp", "*.tiff"},
	m_formats{FIF_PNG, FIF_JPEG, FIF_RAW, FIF_BMP, FIF_TIFF}
{

}


int PhotoOrganizerFrame::filesCounter(wxString& currPath) const
{
	int counter = 0;
	wxArrayString subdirectories;
	wxString dirName;
	wxDir source(currPath);

	// Recursive search images in subfolders
	if (source.HasSubDirs())
	{
		bool cont = source.GetFirst(&dirName, wxEmptyString, wxDIR_DIRS);
		while (cont)
		{
			subdirectories.Add(dirName);
			cont = source.GetNext(&dirName);
		}

		for (wxString sub : subdirectories)
		{
			counter += filesCounter(currPath + '\\' + sub);
		}
	}

	// counting images
	wxArrayString& currentExtensionFiles = wxArrayString();
	for (const auto& extension : m_extensions)
	{
		currentExtensionFiles = getAllFilesInDirWithExtension(currPath, extension);
		counter += currentExtensionFiles.GetCount();
	}

	return counter;
}


void PhotoOrganizerFrame::m_loadFolderOnButtonClick(wxCommandEvent& event)
{
	wxString defaultPath = wxT("/");
	wxDirDialog dialog(this, wxT("Choose directory"), defaultPath, wxDD_NEW_DIR_BUTTON);
	if (dialog.ShowModal() == wxID_OK) {
		sourcePath = dialog.GetPath();
		wxDir curr(sourcePath);

		m_filesCompressionCounter = 0;
		m_filesCount = filesCounter(sourcePath);
		wxMessageBox(std::to_string(m_filesCount));
		m_progressBar->SetValue((int)((m_filesCompressionCounter * 100)/m_filesCount));
	}
}


void PhotoOrganizerFrame::m_exportOnButtonClick(wxCommandEvent& event)
{
	wxString defaultPath = wxT("/");
	wxDirDialog dialog(this, wxT("Choose directory"), defaultPath, wxDD_NEW_DIR_BUTTON);

	if (dialog.ShowModal() == wxID_OK)
	{
		wxString targetPath = dialog.GetPath();
		wxDir target(targetPath);
		directionPath = targetPath;

		while (target.IsOpened() && directionPath != sourcePath )
		{
			cloneDir(sourcePath, targetPath);
			target.Close();
		}
	}
	wxMessageBox("Compression completed");
}

void PhotoOrganizerFrame::m_maxHeightControlOnSpinCtrl(wxSpinEvent& event)
{
	maxHeight = m_maxHeightControl->GetValue();
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
	maxWidth = m_maxWidthControl->GetValue();
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
	compressionValue = 100 - m_compressiomLevel->GetValue();
}


void PhotoOrganizerFrame::m_isHeightBoxChecked(wxCommandEvent& event)
{
	// TODO: Implement m_isHeightBoxChecked
}


void PhotoOrganizerFrame::m_isWidthBoxChecked(wxCommandEvent& event)
{
	// TODO: Implement m_isWidthBoxChecked
}


void PhotoOrganizerFrame::m_isSemiAutomaticModeOn(wxCommandEvent& event)
{
	// TODO: Implement m_isSemiAutomaticModeOn
}