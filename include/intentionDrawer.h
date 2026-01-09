#ifndef _INTENTIONDRAWER_H_
#define _INTENTIONDRAWER_H_

#include "ocpn_plugin.h" // API 20 unified header
#include <wx/string.h>

class AIS_Target_Data;

// =========================================================
// IntentionDrawer Plugin Class (API 20)
// =========================================================

class IntentionDrawer : public opencpn_plugin_120 {
public:
    IntentionDrawer(void* mgr);
    virtual ~IntentionDrawer() {}

    //standard required functions
    int Init() override;
    bool DeInit() override;
    int GetAPIVersionMajor() override;
    int GetAPIVersionMinor() override;
    int GetPlugInVersionMajor() override;
    int GetPlugInVersionMinor() override;
    wxBitmap* GetPlugInBitmap() override;   //this ones for the plugin icon so maybe redundant

    //functions for the plugin manager thing so technically we dont need to do these
    wxString GetCommonName() override;
    wxString GetShortDescription() override;
    wxString GetLongDescription() override;

    //optionele functie
    void SetAISSentence(wxString& sentence) override;
    bool RenderOverlayMultiCanvas(wxDC& dc, PlugIn_ViewPort* vp, int canvasIndex) override;

private:
    wxString m_lastAISSentence;

    // example: store last MMSI received
    // int m_lastMMSI = 0;
};

#endif