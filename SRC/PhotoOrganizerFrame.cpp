#include "PhotoOrganizerFrame.h"

PhotoOrganizerFrame::PhotoOrganizerFrame(wxWindow* parent)
	: PhotoOrganizer(parent),
	m_image(new wxImage())
{
	m_panel->Bind(wxEVT_CHAR_HOOK, &PhotoOrganizerFrame::e_OnKeyDown, this);
	wxInitAllImageHandlers();
	m_maxWidthControl->Disable();
	m_maxHeightControl->Disable();
}

PhotoOrganizerFrame::~PhotoOrganizerFrame()
{
	delete m_image;
}


void PhotoOrganizerFrame::e_LoadFolderOnButtonClick(wxCommandEvent& event)
{
	m_imagesPathArray.Empty();
	wxString defaultPath = wxT("/");
	wxDirDialog dialog(this, wxT("Choose directory"), defaultPath, wxDD_NEW_DIR_BUTTON);
	if (dialog.ShowModal() == wxID_OK)
	{
		m_sourcePath = dialog.GetPath();
		wxDir curr(m_sourcePath);
		m_GetFilesPaths(curr);

		if (m_imagesCounter == 0)
		{
			wxMessageBox("No images to be converted. Pick Folder with images!");
			m_sourcePath = "";
		}
		else
		{
			m_imagesSaved = 0;
			m_progressBar->SetValue(0);
			wxMessageBox("Images loaded: " + std::to_string(m_imagesCounter));
		}
	}
	else 
	{
		wxMessageBox("An error occured. Can't open a modal.");
	}
}

void PhotoOrganizerFrame::e_ExportOnButtonClick(wxCommandEvent& event)
{
	wxString defaultPath = wxT("/");
	wxDirDialog dialog(this, wxT("Choose directory"), defaultPath, wxDD_NEW_DIR_BUTTON);

	if (dialog.ShowModal() == wxID_OK)
	{
		m_destinationPath = dialog.GetPath();
		wxDir target(m_destinationPath);

		bool wasOpened = target.IsOpened();

		if (wasOpened) {
			if (m_destinationPath != m_sourcePath) {
				if (m_isSemiAutomaticModeOn)
				{
					m_CloneDir(m_sourcePath, m_destinationPath);
					m_isLoadingImages = true;
					m_imagesSaved = 0;
					m_GoToNextFrame();
				}
				else if (!m_isSemiAutomaticModeOn)
				{
					m_CloneDir(m_sourcePath, m_destinationPath);
					for (int i = 0; i < m_imagesCounter; ++i)
						m_SaveOneImage(i);
					m_imagesSaved = m_imagesCounter;
					m_progressBar->SetValue(0);
					wxMessageBox("Images conversion completed!");
				}
			}
			else 
			{
				wxMessageBox("An error occured. The destination path cannot be the same as the source path.");
			}
		}
		else
		{
			wxMessageBox("An error occured. Can't open a directory.");
		}

		
	}
}

void PhotoOrganizerFrame::e_UpdateUI(wxUpdateUIEvent& event)
{
	m_Repaint();
}

void PhotoOrganizerFrame::e_OnKeyDown(wxKeyEvent& event)
{
	if (m_isSemiAutomaticModeOn && m_isLoadingImages)
	{
		switch ((int)event.GetKeyCode()) {
		case 316:
			m_angle += 1;
			m_Repaint();
			break;
		case 314:
			m_angle -= 1;
			m_Repaint();
			break;
		case 13:
			m_SaveOneImage(m_imagesSaved);
			m_GoToNextFrame();
			break;
		default:
			break;
		}
	}
	event.Skip();
}

void PhotoOrganizerFrame::e_WidthOnCheck(wxCommandEvent& event)
{
	m_isCustomWidth = m_checkBoxWidth->GetValue();
	m_isCustomWidth ? m_maxWidthControl->Enable() : m_maxWidthControl->Disable();
}

void PhotoOrganizerFrame::e_HeightOnCheck(wxCommandEvent& event)
{
	m_isCustomHeight = m_checkBoxHeight->GetValue();
	m_isCustomHeight ? m_maxHeightControl->Enable() : m_maxHeightControl->Disable();
}

void PhotoOrganizerFrame::e_SemiAutomaticModeOnCheck(wxCommandEvent& event)
{
	m_isSemiAutomaticModeOn = m_checkBoxSemiAutomaticMode->GetValue();
}

void PhotoOrganizerFrame::e_MaxHeightControlOnSpinCtrl(wxSpinEvent& event)
{
	m_maxHeight = m_maxHeightControl->GetValue();
}

void PhotoOrganizerFrame::e_MaxHeightControlOnSpinCtrlText(wxCommandEvent& event)
{
	m_maxHeight = m_maxHeightControl->GetValue();
}

void PhotoOrganizerFrame::e_MaxHeightControlOnTextEnter(wxCommandEvent& event)
{
	m_maxHeight = m_maxHeightControl->GetValue();
}

void PhotoOrganizerFrame::e_MaxWidthControlOnSpinCtrl(wxSpinEvent& event)
{
	m_maxWidth = m_maxWidthControl->GetValue();
}

void PhotoOrganizerFrame::e_MaxWidthControlOnSpinCtrlText(wxCommandEvent& event)
{
	m_maxWidth = m_maxWidthControl->GetValue();
}

void PhotoOrganizerFrame::e_MaxWidthControlOnTextEnter(wxCommandEvent& event)
{
	m_maxWidth = m_maxWidthControl->GetValue();
}

void PhotoOrganizerFrame::e_CompressionLevelOnSlider(wxCommandEvent& event)
{
	m_compressionValue = 100 - m_compressionLevel->GetValue();
}


void PhotoOrganizerFrame::m_Repaint()
{
	if (m_image->Ok())
	{
		wxClientDC DC(m_panel);
		wxBufferedDC bufferedDC(&DC);

		wxImage tmp(*m_image);
		m_angle = (m_angle + 4) % 4;
		for (int i = 1; i <= m_angle; ++i)
			tmp = tmp.Rotate90();
		double currentRatio = (double)tmp.GetWidth() / tmp.GetHeight();

		int drawImageWidth = m_maxWidth;
		int drawImageHeight = m_maxHeight;

		if (m_angle % 2 == 1)
		{
			drawImageWidth = m_maxHeight;
			drawImageHeight = m_maxWidth;
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

void PhotoOrganizerFrame::m_GetFilesPaths(const wxDir& source)
{
	wxArrayString subDirs;
	wxString dirName;

	// Recursive search for subfolders
	if (source.HasSubDirs())
	{
		bool isSubDir = source.GetFirst(&dirName, wxEmptyString, wxDIR_DIRS);
		while (isSubDir)
		{
			if (m_IsImageToCopyInsideFolder(source.GetName() + '\\' + dirName))
				subDirs.Add(dirName);

			isSubDir = source.GetNext(&dirName);
		}

		for (wxString sub : subDirs)
			m_GetFilesPaths(source.GetName() + '\\' + sub);
	}

	// sace images paths
	wxString fileName;
	wxString fileNameWithSubDirs;
	wxArrayString fileNamesList;
	int index = 0;

	for (const auto& extension : c_extensions)
	{
		bool cont = source.GetFirst(&fileName, extension, wxDIR_DEFAULT);
		while (cont) {
			fileNameWithSubDirs = source.GetName() + '\\' + fileName;
			fileNameWithSubDirs.Replace(m_sourcePath, "");
			m_imagesPathArray.Add(fileNameWithSubDirs);
			m_loadedImagesFormats.push_back(c_formats[index]);
			m_imagesCounter++;
			cont = source.GetNext(&fileName);
		}
		index++;
	}
}

const wxArrayString PhotoOrganizerFrame::m_GetAllFilesInDirWithExtension(const wxDir& dir, const wxString extension) const
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

void PhotoOrganizerFrame::m_GoToNextFrame()
{
	if (m_imagesSaved < m_imagesCounter)
	{
		m_image->LoadFile(m_sourcePath + m_imagesPathArray[m_imagesSaved], wxBITMAP_TYPE_ANY);
		if (!m_image)
		{
			m_imagesSaved++;
		}
		m_Repaint();
	}
	else
	{
		m_image->Clear();
		wxMessageBox("Images conversion completed!");
		m_imagesSaved = 0;
		m_isLoadingImages = false;
		m_progressBar->SetValue(0);
	}
}

void PhotoOrganizerFrame::m_SaveOneImage(int index)
{
	int setHeight, setWidth;

	FIBITMAP* bitmap;
	FIBITMAP* bitmapRescaled, bitmapRotated;

	wxString pathToSource = m_sourcePath + '\\' + m_imagesPathArray[index];
	wxString pathToTarget = m_destinationPath + '\\' + m_imagesPathArray[index];

	bitmap = FreeImage_Load(m_loadedImagesFormats[index], pathToSource, 0);
	if (bitmap)
	{
		m_ratio = (double)FreeImage_GetWidth(bitmap) / (double)(FreeImage_GetHeight(bitmap));
		if (m_isCustomHeight && m_isCustomWidth)
		{
			double customRatio = m_maxWidth / m_maxHeight;
			if (customRatio > m_ratio)
			{
				setHeight = m_maxHeight;
				setWidth = m_ratio * setHeight;
			}
			else if (customRatio < m_ratio)
			{
				setWidth = m_maxWidth;
				setHeight = setWidth / m_ratio;
			}
			else
			{
				setWidth = m_maxWidth;
				setHeight = m_maxHeight;
			}
		}
		else if (m_isCustomHeight && !m_isCustomWidth)
		{
			setHeight = m_maxHeight;
			setWidth = m_ratio * setHeight;
		}
		else if (m_isCustomWidth && !m_isCustomHeight)
		{
			setWidth = m_maxWidth;
			setHeight = setWidth / m_ratio;
		}
		else
		{
			setHeight = FreeImage_GetHeight(bitmap);
			setWidth = FreeImage_GetWidth(bitmap);
		}

		bitmapRescaled = FreeImage_Rescale(bitmap, setWidth, setHeight);
		bitmapRotated = *FreeImage_Rotate(bitmapRescaled, 90*m_angle);
		FreeImage_Save(FIF_JPEG, &bitmapRotated, pathToTarget, m_compressionValue);
		FreeImage_Unload(bitmap);
		FreeImage_Unload(bitmapRescaled);
	}
	m_angle = 0;
	m_imagesSaved++;
	m_progressBar->SetValue((int)((m_imagesSaved * 100) / m_imagesCounter));
}

void PhotoOrganizerFrame::m_AddImageToContactSheet(FIBITMAP* contactSheet, FIBITMAP* bitmap, int& widthIndex, int& heightIndex)
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
		wxString sheetCountString = wxString::Format(wxT("%i"), m_contactSheetCount);
		filename = "ContactSheet_" + sheetCountString + ".jpg";
		m_contactSheetCount++;
		FreeImage_Save(FIF_JPEG, contactSheet, m_destinationPath + '\\' + filename, m_compressionValue);
	}

	FreeImage_Unload(bitmapNew);
	FreeImage_Unload(bitmapRescaled);
}

void PhotoOrganizerFrame::m_CloneDir(wxString& currPath, wxString& targetPath)
{
	wxArrayString subdirectories;
	wxString dirName;
	wxDir source(currPath);
	wxDir target(targetPath);
	int index = 0;
	int filesInDir = 0;
	int widthIndex = 0;
	int heightIndex = 0;
	wxArrayString& files = wxArrayString();
	FIBITMAP* contactSheet = FreeImage_Allocate(720, 1024, 24);
	FIBITMAP* bitmap;
	wxString pathToFile = currPath;
	wxString pathToTarget = targetPath;

	for (const auto& format : c_extensions)
	{
		files = m_GetAllFilesInDirWithExtension(source, format);

		for (wxString name : files)
		{
			pathToFile = currPath + '\\' + name;
			pathToTarget = targetPath + '\\' + name;
			bitmap = FreeImage_Load(c_formats[index], pathToFile, 0);
			if (bitmap)
			{
				if (filesInDir == 40)
				{
					filesInDir = 0;
					FIBITMAP* temp = FreeImage_Allocate(720, 1024, 24);
					FreeImage_Paste(contactSheet, temp, 0, 0, 256);
					FreeImage_Unload(temp);
				}

				widthIndex = filesInDir % 5;
				heightIndex = filesInDir / 5;
				if(contactSheet)
					m_AddImageToContactSheet(contactSheet, bitmap, widthIndex, heightIndex);

				FreeImage_Unload(bitmap);
				filesInDir++;
			}
		}
		index++;
	}
	if (filesInDir != 0)
		FreeImage_Save(FIF_JPEG, contactSheet, targetPath + '\\' + "ContactSheet_0.jpg", m_compressionValue);

	FreeImage_Unload(contactSheet);


	if (source.HasSubDirs())
	{
		bool cont = source.GetFirst(&dirName, wxEmptyString, wxDIR_DIRS);
		while (cont)
		{
			if (m_destinationPath != currPath + '\\' + dirName && m_IsImageToCopyInsideFolder(currPath + '\\' + dirName))
			{
				subdirectories.Add(dirName);
				wxMkdir(targetPath + '\\' + dirName);
			}
			cont = source.GetNext(&dirName);
		}

		for (wxString sub : subdirectories)
			m_CloneDir(currPath + '\\' + sub, targetPath + '\\' + sub);
	}
}

bool PhotoOrganizerFrame::m_IsImageToCopyInsideFolder(wxString& currPath) const
{
	bool isInsideImage = false;
	wxDir dir(currPath);
	wxString dirName = "";
	wxArrayString& files = wxArrayString();
	wxArrayString subdirectories;

	for (const auto& extension : c_extensions)
	{
		files = m_GetAllFilesInDirWithExtension(dir, extension);

		if (files.GetCount() > 0)
			return true;
	}

	if (dir.HasSubDirs())
	{
		bool cont = dir.GetFirst(&dirName, wxEmptyString, wxDIR_DIRS);
		while (cont)
		{
			if (m_destinationPath != currPath + '\\' + dirName)
				subdirectories.Add(dirName);

			cont = dir.GetNext(&dirName);
		}

		for (wxString sub : subdirectories)
		{
			if (m_IsImageToCopyInsideFolder(currPath + '\\' + sub))
				return true;
		}
	}
	return false;
}