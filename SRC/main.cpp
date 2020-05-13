#include <wx/wx.h>
#include <memory>

#include "PhotoOrganizerFrame.h"

class MyApp : public wxApp {

public:

	virtual bool OnInit();
	virtual int OnExit() { return 0; }

};

IMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
	auto* mainFrame1 = new PhotoOrganizerFrame(NULL);
	mainFrame1->Show(true);
	SetTopWindow(mainFrame1);

	return true;
}