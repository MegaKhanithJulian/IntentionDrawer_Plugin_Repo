#include "intentionDrawer.h"
#include <wx/wx.h>

#ifdef __WXMSW__
#include <windows.h>
#include <GL/gl.h>
#elif defined(__WXGTK__)
#include <GL/gl.h>
#elif defined(__WXOSX__)
#include <OpenGL/gl.h>
#endif


extern "C" DECL_EXP opencpn_plugin* create_pi(void* ppimgr)
{
    return static_cast<opencpn_plugin*>(
        new IntentionDrawer(ppimgr));
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin* p) { delete p; }

// Constructor
IntentionDrawer::IntentionDrawer(void* mgr) : opencpn_plugin_120(mgr)
{
    //m_lastAISSentence = "";
}

//WARNING: the plugin does NOT WORK with OpenGL rendering. we only use DC renderer.
//or does it? i dont know. i can't know, it doesnt work at all.

#define PI 3.14159265358979323846

//didnt test, dont know how to test, so itll be fine. (i hope?)
/* XPM */
static char* iconArray[] = {
    /* columns rows colors chars-per-pixel */
    "32 32 240 2 ",
    "   c #262628",
    ".  c #353435",
    "X  c #383738",
    "o  c #551C01",
    "O  c #5B1D03",
    "+  c #5D2205",
    "@  c #622304",
    "#  c #692604",
    "$  c #6D2A05",
    "%  c #632A0B",
    "&  c #6D2A08",
    "*  c #732D06",
    "=  c #782F07",
    "-  c #752E09",
    ";  c #753005",
    ":  c #7D3506",
    ">  c #75310A",
    ",  c #7A320A",
    "<  c #7E3B0D",
    "1  c #743210",
    "2  c #474648",
    "3  c #49494B",
    "4  c #575759",
    "5  c #615951",
    "6  c #5C5F63",
    "7  c #706E6D",
    "8  c #727275",
    "9  c #787575",
    "0  c #853805",
    "q  c #81360A",
    "w  c #843B0D",
    "e  c #8A3E0A",
    "r  c #833811",
    "t  c #83410F",
    "y  c #8C410C",
    "u  c #92440E",
    "i  c #994709",
    "p  c #954B0E",
    "a  c #9F550E",
    "s  c #8C4213",
    "d  c #914413",
    "f  c #954916",
    "g  c #9B4C15",
    "h  c #9C521A",
    "j  c #A94C0E",
    "k  c #A4550B",
    "l  c #A55B0C",
    "z  c #B2530A",
    "x  c #B85309",
    "c  c #A45514",
    "v  c #A35913",
    "b  c #AD5A13",
    "n  c #AA5816",
    "m  c #B35D14",
    "M  c #B85917",
    "N  c #BC5C1A",
    "B  c #AD600F",
    "V  c #B36111",
    "C  c #BC631E",
    "Z  c #BA7113",
    "A  c #BC6520",
    "S  c #B1702B",
    "D  c #C5550B",
    "F  c #C65C0D",
    "G  c #CB5B0A",
    "H  c #D45E0A",
    "J  c #C15B16",
    "K  c #CE610E",
    "L  c #D7680E",
    "P  c #DA700F",
    "I  c #C2641C",
    "U  c #C86319",
    "Y  c #C5691E",
    "T  c #CA6C1D",
    "R  c #CF7614",
    "E  c #CB741D",
    "W  c #D7751A",
    "Q  c #E37519",
    "!  c #C46B20",
    "~  c #C66A20",
    "^  c #D26F21",
    "/  c #CC7323",
    "(  c #CB7727",
    ")  c #D37522",
    "_  c #D37B23",
    "`  c #D87826",
    "'  c #DB7C24",
    "]  c #D57C2C",
    "[  c #C67B32",
    "{  c #D37E32",
    "}  c #E47D26",
    "|  c #DD8125",
    " . c #DA842B",
    ".. c #D18928",
    "X. c #DB8B29",
    "o. c #CF8B37",
    "O. c #DB8B32",
    "+. c #D88838",
    "@. c #DE9639",
    "#. c #E38425",
    "$. c #EA8527",
    "%. c #E68A26",
    "&. c #EC8D24",
    "*. c #E3812A",
    "=. c #E58B2C",
    "-. c #E98D29",
    ";. c #F08E27",
    ":. c #ED9329",
    ">. c #EC932C",
    ",. c #F1972F",
    "<. c #E48D33",
    "1. c #E18C3B",
    "2. c #E59134",
    "3. c #EB9533",
    "4. c #ED9935",
    "5. c #E4923C",
    "6. c #E5993A",
    "7. c #EC9B3B",
    "8. c #F29C33",
    "9. c #F39937",
    "0. c #EAA73C",
    "q. c #F4A133",
    "w. c #F4A23D",
    "e. c #8E8270",
    "r. c #DA8D42",
    "t. c #DC9144",
    "y. c #DB974A",
    "u. c #DD9C53",
    "i. c #E98F42",
    "p. c #E29343",
    "a. c #E59C45",
    "s. c #EA9B44",
    "d. c #E99749",
    "f. c #E49D4D",
    "g. c #E29D53",
    "h. c #E89A58",
    "j. c #EAA044",
    "k. c #EAA249",
    "l. c #E2AB4C",
    "z. c #F4A444",
    "x. c #F7A748",
    "c. c #E5A652",
    "v. c #E9A854",
    "b. c #E2A35A",
    "n. c #E6AC5C",
    "m. c #EBAA5B",
    "M. c #F2A15B",
    "N. c #EFB451",
    "B. c #EBB557",
    "V. c #E3A260",
    "C. c #E7A965",
    "Z. c #E4AD6A",
    "A. c #EBB361",
    "S. c #E9B56B",
    "D. c #E9B869",
    "F. c #F1B666",
    "G. c #EFAB70",
    "H. c #E8B776",
    "J. c #EEB474",
    "K. c #E6BC7D",
    "L. c #F2BD74",
    "P. c #F2BC7C",
    "I. c #EEC467",
    "U. c #EEC876",
    "Y. c #ECC37C",
    "T. c #F2C17A",
    "R. c #F4D87C",
    "E. c #908F91",
    "W. c #A7A5A9",
    "Q. c #B1AEB0",
    "!. c #B6B7BD",
    "~. c #EAB985",
    "^. c #F6B886",
    "/. c #EFBB94",
    "(. c #D4BAA3",
    "). c #C2BEBF",
    "_. c #EDC285",
    "`. c #E6C38D",
    "'. c #ECC48D",
    "]. c #F2C484",
    "[. c #F1C58C",
    "{. c #F4CA8D",
    "}. c #F2D588",
    "|. c #ECC692",
    " X c #EAC994",
    ".X c #EDCB9C",
    "XX c #F3C493",
    "oX c #F8C295",
    "OX c #F1CA93",
    "+X c #F1CE9B",
    "@X c #EFD19C",
    "#X c #F4D695",
    "$X c #F3D29C",
    "%X c #CAC2BD",
    "&X c #ECC8A3",
    "*X c #EDCDA4",
    "=X c #F5CDA5",
    "-X c #EFD1A1",
    ";X c #EED2AE",
    ":X c #F2D2A4",
    ">X c #F9D3A4",
    ",X c #F2D4AB",
    "<X c #EFD6B5",
    "1X c #EFD9B8",
    "2X c #F1D6B1",
    "3X c #F3D8B6",
    "4X c #F2DABC",
    "5X c #F9D9B9",
    "6X c #F6E1AE",
    "7X c #F8E3B4",
    "8X c #F7E4BD",
    "9X c #BEBFC5",
    "0X c #CBC7C8",
    "qX c #CECED3",
    "wX c #D1CED1",
    "eX c #D9D6D8",
    "rX c #F2DCC3",
    "tX c #F7DECB",
    "yX c #E2DEDF",
    "uX c #E5E0CE",
    "iX c #F8E7C5",
    "pX c #F4E1CA",
    "aX c #FAF1CF",
    "sX c #F9E8D7",
    "dX c #F9F3DE",
    "fX c #DDDCE1",
    "gX c #E6E5E7",
    "hX c #EBE9EE",
    "jX c #FBEDE2",
    "kX c #F6EEEC",
    "lX c #FAF3E2",
    "zX c #FAF5EC",
    "xX c #FBF8EF",
    "cX c #EFEEF3",
    "vX c #F1EFF3",
    "bX c #F6F4F5",
    "nX c #FBF6F4",
    "mX c #F6F5FA",
    "MX c #FAF7FB",
    "NX c #FBF9FD",
    /* pixels */
    "K.|.,X3X4XrX4X2X-X XY.S.n.c.c.c.c.c.n.H._..X,X3XrXrXpXpXrX1X*X~.",
    ".X*X,X2X1X1X3X2X,X:X+X[.].].].].].[.OX+X,X2X3X4XrXtXtXrX4X1X<X*X",
    " X.X*X*X-X,X,X,X,X,X,X:X+X$X:X:X:X:X:X,X3X3X3X4XrXrX4X1X<X;X*X&X",
    "`.'.'.'.'.|.XX|.OX'.'._._._._._._._._._.'.|. X.X.X*X*X*X*X*X&X&X",
    "K.H.H.S.Z.C.C.m.m.v.k.k.k.k.k.k.k.k.k.a.a.a.a.f.g.b.b.C.Z.H.~.~.",
    "Z.V.b.g.f.f.f.a.s.7.7.7.7.7.7.4.4.4.3.3.2.=.=.=.=.O.O.+.+.+.r.y.",
    "u.u.y.t.p.p.i.7.7.7.4.4.8.8.8.,.,.,.3.>.>.-.%.#.#.*.' ' ] ] ] { ",
    "t.t.r.@.1.<.2.3.4.4.4.8.8.q.q.q.8.8.8.,.:.>.-.%.%.*.' ` ` ) / / ",
    "r.t.p.1.} ` W W $.$.$.;.,.9.z.x.x.z.w.8.:.&.%.#.#.} ' ` ) ) / / ",
    "t.+./ U D D G H L Q } i.G.^.oXoXoXoX^.M.i.*.Q P L K F D M J J I ",
    "S.h.{ U G K L P } d.^.(.%XnXMXMXyX0XkXjXtX/.h.*.W K G D x z V [ ",
    "'.[.T.m.5.5.s.M.G.5XnXE.6 mXNXNXW.3 hXNXNXnXtX~.d.p.p.g.b.Z.~.OX",
    "K.K.L.L.T.T.{.>XsXnXnXgXgXNXNXNXhXfXMXNXNXNXMXjX3X>X>X:X:X-X-X.X",
    "b.A.A.B.F.L.].8XzXlXdXxXbXeXmXNXNXMXgXNXNXNXNXMXjX4X:XOXOX{.'._.",
    "C ( ]  .2.k.L.aXdXiX8XuX6 3 hXNXNXeX3 E.mXNXMXMXzXrX~.n.c.y.o.[ ",
    "M J Y ^ ' =.J.aX8X7X,Xe.3 qXMXvXhXbXQ.X 9XNXMXMXlXiXV.O.E T C M ",
    "M I T ) ' =.L.aX7X6X:X5 W.NXNX).8 NXNX7 E.NXNXMXzXsXC.+._ E Y N ",
    "N I T ) ' <.P.lXlXdXlX7 8 mXNXE.  !.wX2 !.NXNXMXnXpXC.O._ E ~ C ",
    "N I T W | 5.J.sXMXMXNXwX. 4 4 . 9 .   7 cXNXNXNXkX&Xg.O._ E ~ C ",
    "N Y T _ | <.f.~.rXjXnXMX0X9 7 !.MXwX0XkXnXzXdXrX/.V.@. ._ / Y C ",
    "I Y / ) ' #.=.s.M.J.[.=X>X5XzXlXrXtX5XXX[.P.G.v.s.2.X._ _ / ~ C ",
    "C Y ~ ) _ | %.-.>.4.7.7.z.$XdX@Xb.Z.#Xz.8.4.3.>.%.#.| _ / ~ ~ C ",
    "C Y ~ / ) ' | %.-.&.&.&.w.$X6X#XU.U.#Xw.;.&.&.%.=. .' _ / ~ C C ",
    "A A ~ /  .v.n.A.A.v.B.F.B.D.}.R.I.N.U.F.7.4.j.A.S.Y.I.B.l.( C C ",
    "C ~ /  .X.t.S n g h o.Y.D.@.0.6...X.0.B.}.}.U.6.+.I.v.( b j ~ A ",
    "C / ~ a u y : $ O u R Z l k l i i z z E l.I.N.X.o...j 0 $ , q s ",
    "m b y < w r r - r n B k l a p t i k p m R R E m p y w , % + + 1 ",
    "d d s s s d d d g c k v v v v c v k c b V V m m n c c g g f f d ",
    ", , , , , , , q e e y y y y y s d u y u p p p p u y y w w w w w ",
    "& & & & & & - - : q q : : q q q q w q w e e e e 0 : ; - ; > > > ",
    "@ @ @ @ @ @ # * - - ; - - , , - > , = : q q q : , , * & $ & $ & ",
    "o o O O O + @ @ # & & & * $ $ # # $ - - = , ; - * & $ # # # @ @ "
};


int IntentionDrawer::Init(void) {
    //i dont even fucking know what to initialise

    return WANTS_OVERLAY_CALLBACK | WANTS_OPENGL_OVERLAY_CALLBACK | WANTS_DYNAMIC_OPENGL_OVERLAY_CALLBACK;
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
    return new wxBitmap(iconArray);
}

//here ye,
//here ye,
//for the actual code,
//this is the place to be

//do not call. automatically called by opencpn every frame
bool IntentionDrawer::RenderOverlay(wxDC& dc, PlugIn_ViewPort* vp)
{
    if (!vp) return false;
    dc.SetPen(wxPen(*wxRED, 5));
    dc.DrawLine(0, 0, vp->pix_width, vp->pix_height);
    return true;
}
bool IntentionDrawer::RenderOverlayMultiCanvas(wxDC& dc, PlugIn_ViewPort* vp, int canvasIndex, int priority) {
    // Draw on the chart
    dc.SetPen(wxPen(*wxRED, 2));
    dc.DrawLine(0, 0, vp->pix_width, vp->pix_height);
    return true;
}
bool IntentionDrawer::RenderGLOverlayMultiCanvas(wxGLContext* pcontext, PlugIn_ViewPort* vp, int canvasIndex, int priority) {
    if (!vp || !pcontext)
        return false;

    // Only draw for main overlay priority (adjust if needed)
    if (priority != OVERLAY_OVER_EMBOSS)
        return false;

    glPushMatrix();
    glLoadIdentity();

    // Make the line very thick
    glLineWidth(10.0f);

    // Red color
    glColor3f(1.0f, 0.0f, 0.0f);

    glBegin(GL_LINES);
    glVertex2f(-1.0f, -1.0f); // bottom-left of viewport
    glVertex2f(1.0f, 1.0f);   // top-right of viewport
    glEnd();

    glPopMatrix();
    glFlush();

    return true; // tell OpenCPN we drew something
}