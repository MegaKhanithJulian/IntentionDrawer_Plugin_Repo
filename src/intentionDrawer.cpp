#include "intentionDrawer.h"
#include <wx/wx.h>
#include <vector>
#include <cmath>
#include <string>
#include <sstream>
#include <iostream>

extern "C" DECL_EXP opencpn_plugin* create_pi(void* ppimgr)
{
    return static_cast<opencpn_plugin*>(
        new IntentionDrawer(ppimgr));   
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin* p) { delete p; }

// Constructor
IntentionDrawer::IntentionDrawer(void* mgr) : opencpn_plugin_120(mgr)
{
    m_lastAISSentence = "";
}

//WARNING: the plugin does NOT WORK with OpenGL rendering. we only use DC renderer.
//or does it? i dont know.

#define PI 3.14159265358979323846

//didnt test, dont know how to test, so itll be fine. (i hope?)
#define GOOD_PLUGIN_ICON "images/veryGoodIcon.XPM"

// Waypoint struct
struct WP {
    WP(double longitude, double latitude, int headng, int h, int m, int s) {
        lon = longitude;
        lat = latitude;
        heading = headng;
        hour = h;
        minute = m;
        second = s;

        eta = wxString::Format("%02d:%02d:%02d", h, m, s);
    }

    double lon;
    double lat;
    int heading;
    int hour;
    int minute;
    int second;
    wxString eta;
};

struct Vessel {
    std::string mmsi; // unique vessel identifier
    std::vector<WP> route; // vessel-specific waypoints
    wxColour color; // optional: each vessel can have a distinct color
};

// Draws a giant fat line across the visible map, independent of vessels
void DrawGiantTestLine(wxDC& dc, PlugIn_ViewPort* vp)
{
    // Set a thick, bright pen
    dc.SetPen(wxPen(*wxRED, 150)); // 150 pixels thick, bright red

    // Draw from top-left to bottom-right of the viewport
    wxPoint start(0, 0);
    wxPoint end(vp->pix_width, vp->pix_height);

    dc.DrawLine(start, end);
}

Vessel createSampleVessel()
{
    Vessel vessel;
    vessel.mmsi = "123456789"; // unique vessel ID
    vessel.color = wxColour(255, 0, 0); // red vessel

    // Base point
    double baseLat = 51.91185489189016;
    double baseLon = 4.486495169805941;

    // Add waypoints around the base point
    vessel.route.push_back(WP(baseLon + 0.001, baseLat + 0.001, 90, 10, 30, 0));
    vessel.route.push_back(WP(baseLon + 0.002, baseLat + 0.002, 95, 10, 40, 0));
    vessel.route.push_back(
        WP(baseLon + 0.003, baseLat + 0.0015, 100, 10, 50, 0));
    vessel.route.push_back(
        WP(baseLon + 0.004, baseLat + 0.0025, 110, 11, 0, 0));
    vessel.route.push_back(
        WP(baseLon + 0.005, baseLat + 0.003, 120, 11, 10, 0));

    return vessel;
}

std::vector<Vessel> vessels; // all vessels currently tracked

int IntentionDrawer::Init(void) {
	//i dont even fucking know what to initialise
    vessels.push_back(createSampleVessel());

	return WANTS_NMEA_SENTENCES | WANTS_OVERLAY_CALLBACK;
}
bool IntentionDrawer::DeInit(void) {
	//i dont even fucking know what to uninitialise

	return true;
}

int IntentionDrawer::GetAPIVersionMajor() { return API_VERSION_MAJOR; }
int IntentionDrawer::GetAPIVersionMinor() { return API_VERSION_MINOR; }
//version x.x (example: 0.1)
int IntentionDrawer::GetPlugInVersionMajor() { return 0; } // Your plugin's major version
int IntentionDrawer::GetPlugInVersionMinor() { return 1; } // minor version

//necessary for display of the plugin in the plugin manager that we use to plug in the plugin in a way thats managed by the OpenCPN plugin manager
wxString IntentionDrawer::GetCommonName() { return _("IntentionDrawer"); }

wxString IntentionDrawer::GetShortDescription() {
    return _("Charts the intended route of vessels via unique type 8 NMEA & webserver data");
}

wxString IntentionDrawer::GetLongDescription()
{
    return _("The functionalities are as follow:\n\n"

             "* Toggle for local AIS intentions to be received and displayed (NOT DONE)\n"
             "* Toggle for custom online webserver intentions to be received and displayed (NOT DONE)\n"
             "* Draws a really big line somewhere (?)\n\n"

             "Version: 0.1.003\n"
             "Author: the lion does not concern himself with names\n"
             "Copyright (c) 2025");
}

//The bitmap is typically a 32x32 pixel XPM image defined in your plugin's resources.
wxBitmap* IntentionDrawer::GetPlugInBitmap() {
    return new wxBitmap(GOOD_PLUGIN_ICON);
}

//here ye,
//here ye,
//for the actual code,
//this is the place to be

std::vector<std::vector<std::string>> incompleteSentences;

void translateAIS(std::string msg)
{
    // Example message: "!AIVDM,1,1,,A,83aDp0@j2d<dtdNNMQO0jgaE:tt0,0*3C"
    std::vector<std::string> parts;
    std::stringstream ss(msg);
    std::string field;

    // Split the message by commas
    while (std::getline(ss, field, ',')) {
        parts.push_back(field);
    }

    // Print parts for testing
    // part 0: !AIVDM
    // part 1: 1
    // Part 2: 1
    // Part 3:
    // Part 4: A
    // Part 5: 83aDp0@j2d<dtdNNMQO0jgaE:tt0
    // Part 6: 0*3C
    for (size_t i = 0; i < parts.size(); ++i) {
        std::cout << "Part " << i << ": " << parts[i] << std::endl;
    }
    // --- Check checksum ---
    if (!parts.empty()) {
        // Find '*' in last part
        size_t asteriskPos = parts.back().find('*');
        if (asteriskPos == std::string::npos) {
            std::cout << "No checksum found." << std::endl;
            return; // invalid message
        }

        // Extract transmitted checksum
        std::string checksumStr = parts.back().substr(asteriskPos + 1);
        if (checksumStr.size() != 2)
            return; // invalid checksum length

        unsigned int transmittedChecksum = 0;
        std::stringstream ssChecksum;
        ssChecksum << std::hex << checksumStr;
        ssChecksum >> transmittedChecksum;

        // Calculate checksum: XOR of all characters between '!' and '*'
        unsigned int calcChecksum = 0;
        // reconstruct full string from first character after '!' to '*'
        // (exclusive)
        std::string raw = msg.substr(1, msg.find('*') - 1);
        for (char c : raw) {
            calcChecksum ^= static_cast<unsigned char>(c);
        }

        if (calcChecksum != transmittedChecksum) {
            std::cout << "Checksum failed: " << std::hex << calcChecksum
                      << " != " << transmittedChecksum << std::endl;
            return; // invalid message
        }
    }

    std::string payload;

    // Check for segmented message
    if (parts[1][0] != '1') { // total messages > 1
        if (parts[2][0] < parts[1][0]) {
            // Not the final message: store it
            incompleteSentences.push_back(parts);
            return;
        }

        // Final message: combine previous parts
        for (auto it = incompleteSentences.begin();
            it != incompleteSentences.end();) {
            if (!it->empty() && it->at(3) == parts[3]) { // sequence ID match
                payload += it->at(5); // append payload
                it = incompleteSentences.erase(it); // remove after processing
            } else {
                ++it;
            }
        }
    }

    // Append current part's payload
    payload += parts[5];

    // --- Only now, check AIS type ---
    if (payload.empty() || payload[0] != '8') {
        return;
    }

    // std::cout << payload << std::endl;
    std::vector<bool> bits;

    // Convert payload to bits
    for (char c : payload) {
        int val;

        // Manual AIS 6-bit mapping
        if (c >= '0' && c <= '9')
            val = c - '0';
        else if (c >= ':' && c <= '@')
            val = 10 + (c - ':');
        else if (c >= 'A' && c <= 'W')
            val = 17 + (c - 'A');
        else if (c >= '`' && c <= 'w')
            val = 40 + (c - '`');
        else
            continue; // skip invalid chars

        // Push 6 bits (MSB first)
        for (int i = 5; i >= 0; --i) {
            bits.push_back((val >> i) & 1);
        }
    }

    // adaptable function: i-(i+bitsForParam)where i is the amount of bits
    // previous params used up
    int FI = 0;
    for (size_t i = 50; i < 56 && i < bits.size(); ++i) {
        FI = (FI << 1) | bits[i];
    }
    if (FI != 12) {
        return;
    }

    int mmsi = 0;
    for (size_t i = 8; i < 38 && i < bits.size(); ++i) {
        mmsi = (mmsi << 1) | bits[i];
    }
    std::cout << "MMSI: " << mmsi << std::endl;

    int waypointAmt = 0;
    for (size_t i = 78; i < 81 && i < bits.size(); ++i) {
        waypointAmt = (waypointAmt << 1) | bits[i];
    }
    std::vector<WP> waypoints;
    // read the first waypoint (separate from the rest, because it's not a Delta
    // value) read the Longitude
    int raw_lon = 0;
    for (size_t i = 81; i < 108 && i < bits.size(); ++i) {
        raw_lon = (raw_lon << 1) | bits[i];
    }
    // 2's complement (27 bits)
    if (raw_lon & (1 << 26)) { // check MSB
        raw_lon -= (1 << 27);
    }
    // Convert to degrees
    double lon1 = raw_lon / 360000.0; // 1/6000 min -> deg
    std::cout << lon1 << std::endl;

    int raw_lat = 0;
    for (size_t i = 108; i < 134 && i < bits.size(); ++i) {
        raw_lat = (raw_lat << 1) | bits[i];
    }
    // 2's complement (26 bits)
    if (raw_lat & (1 << 25)) {
        raw_lat -= (1 << 26);
    }
    // Convert to degrees
    double lat1 = raw_lat / 360000.0;
    std::cout << lat1 << std::endl;

    // read heading in degrees 0-360 (0 = north)
    int heading = 0;
    for (size_t i = 134; i < 143 && i < bits.size(); ++i) {
        heading = (heading << 1) | bits[i];
    }
    std::cout << heading << std::endl;

    // read ETA in seconds 0-4095
    int eta_delta = 0;
    for (size_t i = 143; i < 153 && i < bits.size(); ++i) {
        eta_delta = (eta_delta << 1) | bits[i];
    }
    std::cout << eta_delta << std::endl;
    int hours = eta_delta / 3600;
    int minutes = (eta_delta % 3600) / 60;
    int seconds = eta_delta % 60;
    // TODO: make a setting whether to display remaining time or arrival time
    // right now its remaining time

    waypoints.emplace_back(lon1, lat1, heading, hours, minutes, seconds);

    for (int j = 1; j < waypointAmt; j++) {
        // calculate the bit range offset for consecutive waypoints
        size_t offset = (j - 1) * 43;

        // read the Longitude delta (12 bits)
        int longDelta = 0;
        for (size_t i = 153 + offset; i < 165 + offset && i < bits.size();
            ++i) {
            longDelta = (longDelta << 1) | bits[i];
        }
        if (longDelta & (1 << 11)) {
            longDelta -= (1 << 12);
        }
        double lon = longDelta / 1200000.0;
        lon += waypoints.back().lon;

        // read the Latitude delta (12 bits)
        int latDelta = 0;
        for (size_t i = 165 + offset; i < 177 + offset && i < bits.size();
            ++i) {
            latDelta = (latDelta << 1) | bits[i];
        }
        if (latDelta & (1 << 11)) {
            latDelta -= (1 << 12);
        }
        double lat = latDelta / 1200000.0;
        lat += waypoints.back().lat;

        // read heading (9 bits)
        int hdng = 0;
        for (size_t i = 177 + offset; i < 186 + offset && i < bits.size();
            ++i) {
            hdng = (hdng << 1) | bits[i];
        }

        // read ETA (10 bits)
        int etaDelta = 0;
        for (size_t i = 186 + offset; i < 196 + offset && i < bits.size();
            ++i) {
            etaDelta = (etaDelta << 1) | bits[i];
        }

        int hrs = etaDelta / 3600;
        int mins = (etaDelta % 3600) / 60;
        int secs = etaDelta % 60;
        hrs += waypoints.back().hour;
        mins += waypoints.back().minute;
        seconds += waypoints.back().second;
        std::cout << hrs << ":" << mins << ":" << secs << std::endl;

        waypoints.emplace_back(lon, lat, hdng, hrs, mins, secs);
    }
}

//turn degrees into radians
inline double Deg2Rad(int deg) { return deg * PI / 180.0; }

// Draw a single waypoint with heading arrow and ETA text
//meaning, you probably dont want to use this directly. have a gander at DrawVesselDC()
void DrawWaypointDC(wxDC& dc, PlugIn_ViewPort* vp, const WP& wp)
{
    wxPoint p;
    GetCanvasPixLL(vp, &p, wp.lat, wp.lon);

    // Waypoint circle
    dc.SetPen(wxPen(*wxBLACK, 1));
    dc.SetBrush(*wxBLUE_BRUSH);
    dc.DrawCircle(p, 4);

    // Heading arrow
    double rad = Deg2Rad(wp.heading);
    double len = 20.0; // pixels

    wxPoint tip(p.x + len * sin(rad), p.y - len * cos(rad));

    dc.SetPen(wxPen(*wxRED, 2));
    dc.DrawLine(p, tip);

    // Arrowhead
    double arrowAngle = PI / 8; // 22.5 degrees
    double arrowLen = 6.0;
    wxPoint left(tip.x - arrowLen * sin(rad - arrowAngle),
        tip.y + arrowLen * cos(rad - arrowAngle));
    wxPoint right(tip.x - arrowLen * sin(rad + arrowAngle),
        tip.y + arrowLen * cos(rad + arrowAngle));
    dc.DrawLine(tip, left);
    dc.DrawLine(tip, right);

    // ETA text
    dc.SetTextForeground(*wxWHITE);
    dc.DrawText(wxString(wp.eta), p.x + 5, p.y - 15);
}

// Draw the route and waypoints for a single vessel
void DrawVesselDC(wxDC& dc, PlugIn_ViewPort* vp, const Vessel& v)
{
    if (v.route.size() < 2)
        return;

    // Route lines
    dc.SetPen(wxPen(v.color, 2));
    for (size_t i = 1; i < v.route.size(); i++) {
        wxPoint p1, p2;
        GetCanvasPixLL(vp, &p1, v.route[i - 1].lat, v.route[i - 1].lon);
        GetCanvasPixLL(vp, &p2, v.route[i].lat, v.route[i].lon);
        dc.DrawLine(p1, p2);
    }

    // Draw waypoints
    for (auto& wp : v.route) {
        DrawWaypointDC(dc, vp, wp);
    }
}

//do not call. automatically called by opencpn every frame
bool IntentionDrawer::RenderOverlayMultiCanvas(wxDC& dc, PlugIn_ViewPort* vp, int canvasIndex) {
    bool drewSomething = false;
    DrawGiantTestLine(dc, vp); // purely visual test

    for (auto& v : vessels) {
        if (v.route.size() >= 2) {
            //DrawVesselDC(dc, vp, v); //make it draw the B..oats
            drewSomething = true;
        }
    }

    return drewSomething;
}

//automatically overwrite vessel with recognised mmsi, or create new vessel at the end of array
//expects mmsi string, vector of waypoints, wxColour colour
void AddOrUpdateVessel(const std::string& mmsi, const std::vector<WP>& route, wxColour color) {
    for (auto& v : vessels) {
        if (v.mmsi == mmsi) {
            v.route = route;
            v.color = color;
            RequestRefresh(GetOCPNCanvasWindow());
            return;
        }
    }

    vessels.push_back({ mmsi, route, color });
    RequestRefresh(GetOCPNCanvasWindow());
}

//this is the function that receives every raw NMEA message that opencpn does
void IntentionDrawer::SetAISSentence(wxString& sentence)
{
    translateAIS(sentence.ToStdString());
}
