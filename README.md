## 1. Project title and authors ##
   ####    Title: #### 
   Photo Organizer

---
   ####    Authors: #### 
   Krystian Śledź

   Michał Kuś

   Mikołaj Marchewa

---
## 2. Project description ##
   In the digital photography era, large numbers of photos are being stored on CDs/DVDs. Reorganization of such an archive, especially one consisting of a sizable number of disks, can be challenging. The aim of the project is to provide an application which will recreate directory structure based on the contents of a CD or DVD, and complete it with thumbnails of the original photos. Aggregation of all the thumbnails in one place will help the user manage their collection.

---
## 3. Preliminary assumptions for project realisation ##
   #### Basic functionality: #### 
   * Program allows for picking a source CD/DVD or any existing source directory and generates the thumbnails to the target directory.
   * Thumbnails' properties such as: maximum height, maximum width or both these parameters, can be set by the user, while keeping the aspect ratio of the original photo.   
   * Thumbnails are saved in JPG format with a degree of compression set by the user.
   * Application accepts images in JPG and BMP formats.
   * Application gives real-time information as to the progress of the process using the total number of photos to be copied, counted beforehand.
   * Empty directories or ones containing files in unsupported formats are ignored while recreating directory structure.
   #### Additional features that may be included: #### 
   * Support for other image types such as PNG, TIFF, RAW etc.
   * Semi-automated workflow, with user being able to alter the photo before it's saved as a thumbnail
   * Creating a contact sheet/s in each directory, consisting of 40 images (5x8) representing all the photos in a directory
---
## 4. Project analysis ##

   #### Input data description: #### 
   The application copies images in the following formats: ( PNG, JPG, RAW, TIFF, BMP ), from a chosen directory, along with every subdirectory (that is not empty or containing only files in not supported formats)

   #### Output data description: #### 
   The result is a cloned directory tree, with images in JPG format, compressed using the value set by the slider, with dimensions set by the user, and contact sheets named Contact_Sheet_X, where X is the index of a contact sheet in each of the cloned directories.

   #### Data structures: #### 
   Application does not use any custom data structures. The only standard structures that are used are "std::vector" and "wxArrayString".

   #### UI specification: ####

   ##### Load Folder button #####
   On click it opens a modal for the user to pick the source directory, from which the images and subdirectories are going to be copied.
   ##### Max Width checkbox and input field #####
   Max width constraint is enabled upon checking the checkbox, and can be set using the text field or it's up/down arrows.
   ##### Max Height checkbox and input field  #####
   Max height constraint is enabled upon checking the checkbox, and can be set using the text field or it's up/down arrows.
   ##### Compression level slider #####
   Compression level is set by moving the thumb of the slider, where left is minimal compression and right is maximal compression. (Note: This does not take into account the compression level with which the image was initially saved, so it is possible to save it with increased file size even though the quality will stay the same.)
   ##### Semi Automatic Mode checkbox #####
   On checking the checkbox the semi-automatic mode is enabled, which after starting the export process and clicking on the display panel allows for rotation using Left/Right Arrow Keys before saving each file manually by pressing Enter.
   ##### Export button #####
   On click it opens a modal for the user to pick the target directory, to which the images and subdirectories are going to be copied, then starts the copying process. 
   ##### Progress Bar #####
   It fills up gradually as the export process progresses.
   ##### Display Panel #####
   In semi-automatic mode it displays each image before the user saves it.

   #### Defining tasks: ####
   To deliver a working app we defined key elements that had to be present:
   * user interface
   * functions loading and saving an image
   * functions searching through existing directory and file structure, finding and cloning the proper directories
   * functions converting the format and size of an image

   #### Choice of programming tools: #### 
   We used wxWidgets library to create the application's interface, because we already had experience with the library and wxFormBuilder.<br>
   For image manipulation and conversion we used FreeImage library, since it provided necessary functionality and developer friendly, extensive documentation.

---
## 5. Task delegation and project roadmap ##
   ### Phase 1 ### 
   #### Basic UI and file manipulation ####
   User Interface using wxFormBuilder - Mikołaj<br>
   Loading files - Krystian<br>
   Exporting files - Michał<br>
   Initial documentation - Michał

   ### Phase 2 ###
   #### Core functionality ####
   Cloning directory tree  - Michał<br>
   Resizing image according to input fields - Michał<br>
   Counting all images, and counting images saved in runtime - Mikołaj<br>
   Checking directories for files with correct extensions and for abnormalities in paths - Mikołaj

   ### Phase 3 ###
   #### Additional features and code refactoring ####
   Creating contact sheets - Michał<br>
   Code refactoring - Mikołaj, Krystian, Michał<br>
   Semi-automatic mode - Mikołaj, Krystian<br>
   Improvement on choosing the size of thumbnail - Mikołaj, Krystian<br>
   Getting paths to individual images - Mikołaj<br>
   Final draft of documentation - Michał

---
## 6. Invention and description of necessary algorithms ##
Since a directory structure is basically a tree, we needed a tree traversal algorithm to navigate through it. The main functions traversing through the structures are <b>m_CloneDir( )</b> and <b>m_IsImageToCopyInsideFolder( )</b>, both use breadth-first type search which is the most natural way of manipulating directories.<br> <b>m_CloneDir( )</b> copies subdirectories inside the source, to the target, later to be filled with copied images, then it's called again on each of the cloned subdirectories. It checks using <b>m_IsImageToCopyInsideFolder( )</b> before it's called.<br> <b>m_IsImageToCopyInsideFolder( )</b> checks whether there are any concerning us files inside a directory, if not it calls itself for every subdirectory in that directory, to find if there are any images at all in the sub-tree.

---
## 7. Coding ##

##### void e_LoadFolderOnButtonClick(wxCommandEvent& event)
Function calls m_GetFilesPaths(), and saves path to source directory, pops up a modal if it encounters an error.<br>
##### void e_ExportOnButtonClick(wxCommandEvent& event)
Opens target directory and saves it's path, calls m_CloneDir() to replicate the source directory tree and checks what mode is the app running in, if the mode is automatic it also calls m_SaveOneImage() for every image. If the mode is semi-automatic it calls m_GoToNextFrame().<br>
##### void e_UpdateUI(wxUpdateUIEvent& event)
Updates the interface, calls Repaint().<br>
##### void e_OnKeyDown(wxKeyEvent& event)
Gets user input in semi-automatic mode. Left Arrow - rotate left, Right Arrow - rotate right, Enter - save image.<br>
##### void e_WidthOnCheck(wxCommandEvent& event)
Checks if width constraint is on, if not the text field is blocked.
##### void e_HeightOnCheck(wxCommandEvent& event)
Checks if height constraint is on, if not the text field is blocked.
##### void e_SemiAutomaticModeOnCheck(wxCommandEvent& event)
Checks if semi-automatic mode is on, if not the checkbox is blocked.
### Functions controlling size constraints for thumbnails:
##### void e_MaxHeightControlOnSpinCtrl(wxSpinEvent& event); 
##### void e_MaxHeightControlOnSpinCtrlText(wxCommandEvent& event);
##### void e_MaxHeightControlOnTextEnter(wxCommandEvent& event);
##### void e_MaxWidthControlOnSpinCtrl(wxSpinEvent& event);
##### void e_MaxWidthControlOnSpinCtrlText(wxCommandEvent& event);
##### void e_MaxWidthControlOnTextEnter(wxCommandEvent& event);
<br>

##### void e_CompressionLevelOnSlider(wxCommandEvent& event) 
Sets compression level with value of the slider.
##### void m_Repaint()
Repaints the image updating wxPanel.
##### void m_GetFilesPaths(const wxDir& dir)
Saves image paths from directory to m_imagePathsArray and corresponding file extensions to m_loadedImagesFormats.
##### const wxArrayString m_GetAllFilesInDirWithExtension(const wxDir& dir, const wxString extension) const
Returns all the files within a directory with the extension passed as an argument.
##### void m_GoToNextFrame()
Loads an image to the display and calls m_Repaint(), if there are no more images clears the display and sets a flag to false for the program to end.
##### void m_SaveOneImage(int index)
Saves a single rescaled image to target directory. 
##### void m_AddImageToContactSheet(FIBITMAP* contactSheet, FIBITMAP* bitmap, int& widthIndex, int& heightIndex)
Adds an image to the contact sheet.
##### void m_CloneDir(wxString& source, wxString& target)
Creates contact sheets in each directory.<br>
Traverses through source directory tree, copying the subdirectories to target tree using both iteration and recursion.
##### bool m_IsImageToCopyInsideFolder(wxString& currPath) const
Checks for presence of images in supported formats in a directory using both iteration and recursion.

---
## 8. Testing ##
Testing was being done during the development process, on different PCs, using various source directories and images, at every phase of development. It helped identify and eliminate several bugs in the application, such as an issue causing the exporting process to go endlessly when exporting directly to source directory. This approach prevented the bugs from being an underlying cause for failure of future modules.  

---
## 9. Deployment, report and conclusions ##
Deployment was successful. We accomplished all basic targets set in the beginning and implemented all the additional features.<br>
Some issues stemming from the libraries used are still present, e. g. PNG files containing transparent elements are not converted to JPG, and could be fixed in the future.<br>
Further code refactoring could help streamline implemented recursion.

---
