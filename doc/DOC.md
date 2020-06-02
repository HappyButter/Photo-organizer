## 1. Project title and authors
####    Title:
Photo Organizer


####    Authors:
Krystian Śledź

Michał Kuś

Mikołaj Marchewa

## 2. Project description
In the digital photography era, large numbers of photos are being stored on CDs/DVDs. Reorganization of such an archive, especially one consisting of a sizeable number of disks, can be challenging. The aim of the project is to provide an application which will recreate directory structure based on the contents of a CD or DVD, and complete it with thumbnails of the original photos. Aggregation of all the thumbnails in one place will help the user manage their collection.

## 3. Preliminary assumptions for project realisation
#### Basic functionality:
* Program allows for picking a source CD/DVD or any existing source directory and generates the thumbnails to the target directory.
* Thumbnails' properties such as: maximum height, maximum width or both these parameters, can be set by the user, while keeping the aspect ratio of the original photo.   
* Thumbnails are saved in JPG format with degree of compression set by the user.
* Application accepts images in JPG and BMP formats.
* Application gives real-time information as to the progress of the process using the total number of photos to be copied,0 counted beforehand.
* Empty directories or ones containing files in unsupported formats are ignored while recreating directory structure.
#### Additional features that may be included:
* Support for other image types such as PNG, TIFF, RAW etc.
* Half-automated workflow, with user being able to alter the photo before it's saved as a thumbnail
* Creating a contact sheet/s in each directory, consisting of 40 images (5x8) representing all the photos in a directory

## 4. Project analysis

## 5. Task delegation and project roadmap
mikołaj - wx, wyświetlanie,
krystian - wczytywanie, eksport, ratio
michał - dokumentacja, konwersja, zapis

## 6. Invention and description of necessary algorithms

## 7. Coding

## 8. Testing

## 9. Deployment, report and conclusions