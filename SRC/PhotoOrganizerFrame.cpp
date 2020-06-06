#include "PhotoOrganizerFrame.h"

PhotoOrganizerFrame::PhotoOrganizerFrame(wxWindow* parent)
	: PhotoOrganizer(parent),
	m_extensions{ "*.png", "*.jpg", "*.raw", "*.bmp", "*.tiff" },
	m_formats{ FIF_PNG, FIF_JPEG, FIF_RAW, FIF_BMP, FIF_TIFF },
	m_image(new wxImage())
{
	m_panel->Bind(wxEVT_CHAR_HOOK, &PhotoOrganizerFrame::OnKeyDown, this);
	wxInitAllImageHandlers();
}

PhotoOrganizerFrame::~PhotoOrganizerFrame()
{
	delete m_image;
}

void PhotoOrganizerFrame::UpdateUI(wxUpdateUIEvent& event)
{ 
	Repaint();
}

void PhotoOrganizerFrame::OnKeyDown(wxKeyEvent& event)
{
	wxMessageBox("Out Key '%c'", event.GetUnicodeKey());
	if(isSemiAutomaticModeOn)
	{
		wxMessageBox("Key '%c'", event.GetUnicodeKey());
		switch ((int)event.GetKeyCode()){
		case 316:
			angle += 1;
			angle = angle % 4;
			Repaint();
			break;
		case 314:
			angle -= 1;
			angle = angle % 4;
			Repaint();
			break;
		case 13:
			isEnterPressed = true;
			break;
		default:
			break;
		}
	}
	event.Skip();
}

void PhotoOrganizerFrame::Repaint()
{
	if (m_image->Ok())
	{
		wxClientDC DC(m_panel);
		wxBufferedDC bufferedDC(&DC);

		wxImage tmp(*m_image);
		for (int i = 1; i <= angle; ++i)
			tmp = tmp.Rotate90();
		
		int drawImageWidth = maxWidth;
		int drawImageHeight= maxHeight;
		if (angle % 2 == 1)
		{
			drawImageWidth = maxHeight;
			drawImageHeight = maxWidth;
		}

		int boxWitdth = DC.GetSize().x;
		int boxHeight = DC.GetSize().y;

		double customRatio = (double)boxWitdth / (double)boxHeight;

		if (customRatio > ratio)
		{
			drawImageHeight = boxHeight;
			drawImageWidth = ratio * drawImageHeight;
		}
		else if (customRatio < ratio)
		{
			drawImageWidth = boxWitdth;
			drawImageHeight = drawImageWidth / ratio;
		}
		else
		{
			drawImageWidth = boxWitdth;
			drawImageHeight = boxHeight;
		}

		tmp.Rescale(drawImageWidth, drawImageHeight);
		
		wxBitmap imgBitmap(tmp);
		bufferedDC.Clear();
		bufferedDC.DrawBitmap(imgBitmap, 0, 0);
	}
}


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
	double customRatio;
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
				ratio = (double)FreeImage_GetWidth(bitmap) / (double)(FreeImage_GetHeight(bitmap));

				if (isSemiAutomaticModeOn)
				{
					m_image->LoadFile(pathToFile, wxBITMAP_TYPE_ANY);
					Repaint();
				}
				if (isCustomHeight && isCustomWidth)
				{
					double customRatio = maxWidth / maxHeight;
					if (customRatio > ratio)
					{
						setHeight = maxHeight;
						setWidth = ratio * setHeight;
					}
					else if (customRatio < ratio)
					{
						setWidth = maxWidth;
						setHeight = setWidth / ratio;
					}
					else
					{
						setWidth = maxWidth;
						setHeight = maxHeight;
					}
				}
				else if (isCustomHeight && !isCustomWidth)
				{
					setHeight = maxHeight;
					setWidth = ratio * setHeight;
				}
				else if (isCustomWidth && !isCustomHeight)
				{
					setWidth = maxWidth;
					setHeight = setWidth / ratio;
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
	m_filesCompressionCounter = 0;
	m_progressBar->SetValue(0);
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
			counter += filesCounter(currPath + '\\' + sub);
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
	if (dialog.ShowModal() == wxID_OK) 
	{
		sourcePath = dialog.GetPath();
		wxDir curr(sourcePath);
		
		m_filesCount = filesCounter(sourcePath);

		if (m_filesCount == 0)
		{
			wxMessageBox("No images to be converted. Pick Folder with images!");
			sourcePath = "";
		}
		else
		{
			m_filesCompressionCounter = 0;
			m_progressBar->SetValue((int)((m_filesCompressionCounter * 100)/m_filesCount));
		}
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
		bool wasOpened = target.IsOpened();
		while (target.IsOpened() && directionPath != sourcePath )
		{
			cloneDir(sourcePath, targetPath);
			target.Close();
		}
		if (wasOpened)
			wxMessageBox("Images conversion completed!");
	}
}

void PhotoOrganizerFrame::m_maxHeightControlOnSpinCtrl(wxSpinEvent& event)
{
	maxHeight = m_maxHeightControl->GetValue();
}

void PhotoOrganizerFrame::m_maxHeightControlOnSpinCtrlText(wxCommandEvent& event)
{
	maxHeight = m_maxHeightControl->GetValue();
}

void PhotoOrganizerFrame::m_maxHeightControlOnTextEnter(wxCommandEvent& event)
{
	maxHeight = m_maxHeightControl->GetValue();
}

void PhotoOrganizerFrame::m_maxWidthControlOnSpinCtrl(wxSpinEvent& event)
{
	maxWidth = m_maxWidthControl->GetValue();
}

void PhotoOrganizerFrame::m_maxWidthControlOnSpinCtrlText(wxCommandEvent& event)
{
	maxWidth = m_maxWidthControl->GetValue();
}

void PhotoOrganizerFrame::m_maxWidthControlOnTextEnter(wxCommandEvent& event)
{
	maxWidth = m_maxWidthControl->GetValue();
}

void PhotoOrganizerFrame::m_compressiomLevelOnSlider(wxCommandEvent& event)
{
	compressionValue = 100 - m_compressiomLevel->GetValue();
}

void PhotoOrganizerFrame::m_isHeightBoxChecked(wxCommandEvent& event)
{
	isCustomHeight = m_checkBoxHeight->GetValue();
}

void PhotoOrganizerFrame::m_isWidthBoxChecked(wxCommandEvent& event)
{
	isCustomWidth = m_checkBoxWidth->GetValue();
}

void PhotoOrganizerFrame::m_isSemiAutomaticModeOn(wxCommandEvent& event)
{
	isSemiAutomaticModeOn = m_checkBoxSemiAutomaticMode->GetValue();
}