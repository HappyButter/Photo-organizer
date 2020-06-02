#include "PhotoOrganizerFrame.h"

const wxArrayString PhotoOrganizerFrame::getAllFilesInDirWithExtension(const wxDir &dir, const wxString extension) const
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

void PhotoOrganizerFrame::copyAll_JPG(wxString& currPath, wxString& targetPath) const
{
	FIBITMAP* bitmap;
	wxDir curr(currPath);
	JPGfiles = getAllFilesInDirWithExtension(curr, "*.jpg");
	int setWidth = maxWidth;
	int setHeight = maxHeight;
	double ratio = 4. / 3.;
	
	for (wxString jpgName : JPGfiles)
	{
		wxString pathToFile = currPath + '\\' + jpgName;
		wxString pathToTarget = targetPath + '\\' + jpgName;

		bitmap = FreeImage_Load(FIF_JPEG, pathToFile, 0);

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

			bitmap = FreeImage_Rescale(bitmap,  setWidth, setHeight);
			FreeImage_Save(FIF_JPEG, bitmap, pathToTarget, compressionValue);
			FreeImage_Unload(bitmap);
		}
	}
	wxMessageBox("Success JPG!");
}

void PhotoOrganizerFrame::copyAll_PNG(wxString& currPath, wxString& targetPath) const
{
	FIBITMAP* bitmap;
	wxDir curr(currPath);
	PNGfiles = getAllFilesInDirWithExtension(curr, "*.png"); // !
	int setWidth = maxWidth;
	int setHeight = maxHeight;
	double ratio = 4. / 3.;

	for (wxString pngName : PNGfiles)
	{
		wxString pathToFile = currPath + '\\' + pngName;
		wxString pathToTarget = targetPath + '\\' + pngName;

		bitmap = FreeImage_Load(FIF_PNG, pathToFile, 0); // !
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

			bitmap = FreeImage_Rescale(bitmap, setWidth, setHeight);
			FreeImage_Save(FIF_JPEG, bitmap, pathToTarget, compressionValue);
			FreeImage_Unload(bitmap);
		}
	}
	wxMessageBox("Success PNG!"); // ?
}

void PhotoOrganizerFrame::cloneDir(wxString& currPath, wxString& targetPath) const
{
	wxArrayString subdirectories;
	wxString dirName;
	wxDir source(currPath);
	wxDir target(targetPath);
	
	copyAll_JPG(currPath, targetPath);
	copyAll_PNG(currPath, targetPath);
	//copyAll_BMP(currPath, targetPath);
	//copyAll_TIFF(currPath, targetPath);
	//copyAll_RAW(currPath, targetPath);

	if (source.HasSubDirs())
	{
		bool cont = source.GetFirst(&dirName, wxEmptyString, wxDIR_DIRS);
		while (cont)
		{
			subdirectories.Add(dirName);
			wxMkdir(targetPath + '\\' + dirName);
			cont = source.GetNext(&dirName);
		}
		for (wxString sub : subdirectories)
		{
			cloneDir(currPath + '\\' + sub, targetPath + '\\' + sub);
		}
	}
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
		/*
		wxDir dir(sourcePath);
		if (dir.IsOpened()) {
			JPGfiles = getAllFilesInDirWithExtension(dir, "*.jpg");
			BMPfiles = getAllFilesInDirWithExtension(dir, "*.bmp");
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
		*/
	}

}

void PhotoOrganizerFrame::m_maxHeightControlOnSpinCtrl(wxSpinEvent& event)
{
	maxHeight = m_maxHeightControl->GetValue();
}

void PhotoOrganizerFrame::m_maxHeightControlOnSpinCtrlText(wxCommandEvent& event)
{
	// what does it do ??
}

void PhotoOrganizerFrame::m_maxHeightControlOnTextEnter(wxCommandEvent& event)
{
	// what does it do ??
}

void PhotoOrganizerFrame::m_maxWidthControlOnSpinCtrl(wxSpinEvent& event)
{
	maxWidth = m_maxWidthControl->GetValue();
}

void PhotoOrganizerFrame::m_maxWidthControlOnSpinCtrlText(wxCommandEvent& event)
{
	// what does it do ??
}

void PhotoOrganizerFrame::m_maxWidthControlOnTextEnter(wxCommandEvent& event)
{
	// what does it do ??
}

void PhotoOrganizerFrame::m_compressiomLevelOnSlider(wxCommandEvent& event)
{
	compressionValue = 100 - m_compressiomLevel->GetValue();
}

void PhotoOrganizerFrame::m_folderNameOnText(wxCommandEvent& event)
{
	// TODO: Implement m_folderNameOnText
}

void PhotoOrganizerFrame::m_exportOnButtonClick(wxCommandEvent& event)
{
	wxString defaultPath = wxT("/");
	wxDirDialog dialog(this, wxT("Choose directory"), defaultPath, wxDD_NEW_DIR_BUTTON);
	
	if (dialog.ShowModal() == wxID_OK)
	{
		wxString targetPath = dialog.GetPath();
		wxDir target(targetPath);

		while (target.IsOpened())
		{
			cloneDir(sourcePath, targetPath);

			target.Close();
		}
	}
}
