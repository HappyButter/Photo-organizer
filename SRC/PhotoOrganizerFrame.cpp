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
	if (isSemiAutomaticModeOn && m_isLoadingImages)
	{
		switch ((int)event.GetKeyCode()) {
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
			Go_To_Next_Frame();
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
		double currentRatio = (double)tmp.GetWidth() / tmp.GetHeight();

		int drawImageWidth = maxWidth;
		int drawImageHeight = maxHeight;

		if (angle % 2 == 1)
		{
			drawImageWidth = maxHeight;
			drawImageHeight = maxWidth;
		}

		int boxWitdth = DC.GetSize().x;
		int boxHeight = DC.GetSize().y;

		double customRatio = (double)boxWitdth / (double)boxHeight;

		if (customRatio > currentRatio)
		{
			drawImageHeight = boxHeight;
			drawImageWidth = currentRatio * drawImageHeight;
		}
		else if (customRatio < currentRatio)
		{
			drawImageWidth = boxWitdth;
			drawImageHeight = drawImageWidth / currentRatio;
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


void PhotoOrganizerFrame::getFilesPaths(const wxDir& source)
{
	wxArrayString subdirectories;
	wxString dirName;

	// Recursive search for subfolders
	if (source.HasSubDirs())
	{
		bool cont = source.GetFirst(&dirName, wxEmptyString, wxDIR_DIRS);
		while (cont)
		{
			if(isImageToCopyInsideFolder(source.GetName() + '\\' + dirName))
				subdirectories.Add(dirName);

			cont = source.GetNext(&dirName);
		}

		for (wxString sub : subdirectories)
			getFilesPaths(source.GetName() + '\\' + sub);
	}

	// sace images paths
	wxString fileName;
	wxString fileNameWithSubDirs;
	wxArrayString fileNamesList;
	int index = 0;

	for (const auto& extension : m_extensions)
	{
		bool cont = source.GetFirst(&fileName, extension, wxDIR_DEFAULT);
		while (cont) {
			fileNameWithSubDirs = source.GetName() + '\\' + fileName;
			fileNameWithSubDirs.Replace(sourcePath, "");
			m_imagesPathArray.Add(fileNameWithSubDirs);
			m_imagesFromats.push_back(m_formats[index]);

			m_imagesCounter++;
			cont = source.GetNext(&fileName);
		}
		index++;
	}
}

void PhotoOrganizerFrame::Go_To_Next_Frame()
{
	if (m_imagesLoaded < m_imagesCounter)
	{
		angle = 0;
		m_image->LoadFile(sourcePath + m_imagesPathArray[m_imagesLoaded], wxBITMAP_TYPE_ANY);
		saveOneImage(m_imagesLoaded);
		Repaint();
	}
	else
	{
		m_image->Clear();
		wxMessageBox("Images conversion completed!");
		m_imagesLoaded = 0;
		m_isLoadingImages = false;
		m_progressBar->SetValue(0);
	}
}


void PhotoOrganizerFrame::saveOneImage(int index)
{
	int setHeight, setWidth;

	FIBITMAP* bitmap;
	FIBITMAP* bitmapRescaled;

	wxString pathToSource = sourcePath + '\\' + m_imagesPathArray[index];
	wxString pathToTarget = directionPath + '\\' + m_imagesPathArray[index];

	bitmap = FreeImage_Load(m_imagesFromats[index], pathToSource, 0);
	if (bitmap)
	{
		ratio = (double)FreeImage_GetWidth(bitmap) / (double)(FreeImage_GetHeight(bitmap));
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

		m_imagesLoaded++;
		m_progressBar->SetValue((int)((m_imagesLoaded * 100) / m_imagesCounter));
	}
}



void PhotoOrganizerFrame::m_exportOnButtonClick(wxCommandEvent& event)
{
	wxString defaultPath = wxT("/");
	wxDirDialog dialog(this, wxT("Choose directory"), defaultPath, wxDD_NEW_DIR_BUTTON);

	if (dialog.ShowModal() == wxID_OK)
	{
		directionPath = dialog.GetPath();
		wxDir target(directionPath);

		bool wasOpened = target.IsOpened();

		if (wasOpened && directionPath != sourcePath && isSemiAutomaticModeOn)
		{
			cloneDir(sourcePath, directionPath);
			m_isLoadingImages = true;
			m_imagesLoaded = 0;
			Go_To_Next_Frame();
		}
		else if (wasOpened && directionPath != sourcePath && !isSemiAutomaticModeOn)
		{
			cloneDir(sourcePath, directionPath);
			for (int i = 0; i < m_imagesCounter; ++i)
				saveOneImage(i);
			m_imagesLoaded = 0;
			m_progressBar->SetValue(0);
			wxMessageBox("Images conversion completed!");
		}
	}
}


void PhotoOrganizerFrame::copyAllImages(wxString& currPath, wxString& targetPath)
{
	//int index = 0;
	//int setWidth = maxWidth;
	//int setHeight = maxHeight;
	//int filesInDir = 0;
	//int widthIndex = 0;
	//int heightIndex = 0;
	//double ratio = 4. / 3.;

	//wxArrayString& files = wxArrayString();
	//FIBITMAP* contactSheet = FreeImage_Allocate(720, 1024, 24);
	//FIBITMAP* bitmap;
	//FIBITMAP* bitmapRescaled;
	//wxDir curr(currPath);
	//wxString pathToFile = currPath;
	//wxString pathToTarget = targetPath;

	//for (const auto& format : m_extensions)
	//{
	//	files = getAllFilesInDirWithExtension(curr, format);

	//	for (wxString name : files)
	//	{
	//		pathToFile = currPath + '\\' + name;
	//		pathToTarget = targetPath + '\\' + name;
	//		bitmap = FreeImage_Load(m_formats[index], pathToFile, 0);
	//		if (bitmap)
	//		{
	//			ratio = (double)FreeImage_GetWidth(bitmap) / (double)(FreeImage_GetHeight(bitmap));

	//			if (filesInDir == 40)
	//				filesInDir = 0;
	//			widthIndex = filesInDir % 5;
	//			heightIndex = filesInDir / 5;
	//			addImageToContactSheet(contactSheet, bitmap, widthIndex, heightIndex);

	//			if (isSemiAutomaticModeOn)
	//			{
	//				m_image->LoadFile(pathToFile, wxBITMAP_TYPE_ANY);
	//				Repaint();
	//			}
	//			if (isCustomHeight && isCustomWidth)
	//			{
	//				double customRatio = maxWidth / maxHeight;
	//				if (customRatio > ratio)
	//				{
	//					setHeight = maxHeight;
	//					setWidth = ratio * setHeight;
	//				}
	//				else if (customRatio < ratio)
	//				{
	//					setWidth = maxWidth;
	//					setHeight = setWidth / ratio;
	//				}
	//				else
	//				{
	//					setWidth = maxWidth;
	//					setHeight = maxHeight;
	//				}
	//			}
	//			else if (isCustomHeight && !isCustomWidth)
	//			{
	//				setHeight = maxHeight;
	//				setWidth = ratio * setHeight;
	//			}
	//			else if (isCustomWidth && !isCustomHeight)
	//			{
	//				setWidth = maxWidth;
	//				setHeight = setWidth / ratio;
	//			}
	//			else
	//			{
	//				setHeight = FreeImage_GetHeight(bitmap);
	//				setWidth = FreeImage_GetWidth(bitmap);
	//			}

	//			bitmapRescaled = FreeImage_Rescale(bitmap, setWidth, setHeight);
	//			FreeImage_Save(FIF_JPEG, bitmapRescaled, pathToTarget, compressionValue);
	//			FreeImage_Unload(bitmap);
	//			FreeImage_Unload(bitmapRescaled);


	//		}
	//	}
	//	index++;
	//}
	//if (filesInDir != 0)
	//	FreeImage_Save(FIF_JPEG, contactSheet, targetPath + '\\' + "ContactSheet_0.jpg", compressionValue);
	//FreeImage_Unload(contactSheet);
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


void PhotoOrganizerFrame::addImageToContactSheet(FIBITMAP* contactSheet, FIBITMAP* bitmap, int& widthIndex, int& heightIndex)
{
	FIBITMAP* bitmapNew;
	FIBITMAP* bitmapRescaled;

	int setWidth = 144;
	int setHeight = 128;
	std::vector<int> leftPositions = { 0, 144, 288, 432, 576 };
	std::vector<int> topPositions = { 0, 128, 256, 384, 512, 640, 768, 896 };
	double ratio = 144. / 128.;

	bitmapNew = FreeImage_Allocate(144, 128, 24);

	ratio = FreeImage_GetWidth(bitmap) / double(FreeImage_GetHeight(bitmap));
	if (FreeImage_GetWidth(bitmap) > setWidth || FreeImage_GetHeight(bitmap) > setHeight)
	{
		if (FreeImage_GetWidth(bitmap) > FreeImage_GetHeight(bitmap))
			setHeight = setWidth / ratio;
		else
			setWidth = setHeight * ratio;
	}
	else
	{
		setHeight = FreeImage_GetHeight(bitmap);
		setWidth = FreeImage_GetWidth(bitmap);
	}

	bitmapRescaled = FreeImage_Rescale(bitmap, setWidth, setHeight);
	if (bitmapRescaled)
	{
		FreeImage_Paste(bitmapNew, bitmapRescaled, (144 - setWidth) / 2, (128 - setHeight) / 2, 256);
		if (bitmapNew)
			FreeImage_Paste(contactSheet, bitmapNew, leftPositions[widthIndex], topPositions[heightIndex], 256);
		else
		{
			wxMessageBox("not new");
		}
	}
	else
	{
		wxMessageBox("not rescaled");
	}
	if (widthIndex == 4 && heightIndex == 7)
	{
		wxString filename;
		wxString sheetCountString = wxString::Format(wxT("%i"), contactSheetCount);
		filename = "ContactSheet_" + sheetCountString + ".jpg";
		contactSheetCount++;
		wxMessageBox(filename);
		FreeImage_Save(FIF_JPEG, contactSheet, directionPath + '\\' + filename, compressionValue);
		contactSheet = FreeImage_Allocate(720, 1024, 24);
	}

	FreeImage_Unload(bitmapNew);
	FreeImage_Unload(bitmapRescaled);
}

void PhotoOrganizerFrame::m_loadFolderOnButtonClick(wxCommandEvent& event)
{
	m_imagesPathArray.Empty();
	wxString defaultPath = wxT("/");
	wxDirDialog dialog(this, wxT("Choose directory"), defaultPath, wxDD_NEW_DIR_BUTTON);
	if (dialog.ShowModal() == wxID_OK)
	{
		sourcePath = dialog.GetPath();
		wxDir curr(sourcePath);
		getFilesPaths(curr);

		if (m_imagesCounter == 0)
		{
			wxMessageBox("No images to be converted. Pick Folder with images!");
			sourcePath = "";
		}
		else
		{
			m_imagesLoaded = 0;
			m_progressBar->SetValue(0);
			wxMessageBox("Images loaded: " + std::to_string(m_imagesCounter));
		}
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