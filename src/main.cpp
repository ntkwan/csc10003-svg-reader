#include <wx/graphics.h>
#include <wx/wx.h>

#include "Parser.hpp"
#include "graphics/Renderer.hpp"

class MyFrame : public wxFrame {
public:
    MyFrame(Group* root)
        : wxFrame(NULL, wxID_ANY, "Circle Drawing Example"), root(root) {
        SetSize(1600, 900);
        SetBackgroundColour(wxColour(255, 255, 255));
        zoomFactor = 1.0f;
    }

    void OnPaint(wxPaintEvent& event) {
        wxPaintDC dc(this);
        wxGraphicsContext* gc = wxGraphicsContext::Create(dc);
        if (gc) {
            Renderer renderer = Renderer(gc);
            root->render(renderer);
            delete gc;
        }

        // if (gc) {
        //     gc->SetAntialiasMode(wxANTIALIAS_DEFAULT);

        //     // Draw a circle
        //     wxGraphicsPath circlePath = gc->CreatePath();
        //     circlePath.AddCircle(100, 100, 100);
        //     gc->SetBrush(wxBrush(wxColour(255, 0, 0)));
        //     gc->FillPath(circlePath);

        //     // Draw an ellipse
        //     wxGraphicsPath ellipsePath = gc->CreatePath();
        //     ellipsePath.AddEllipse(200, 100, 200, 100);
        //     gc->SetBrush(wxBrush(wxColour(0, 255, 0)));
        //     gc->FillPath(ellipsePath);

        //     // Draw a rectangle
        //     wxGraphicsPath rectPath = gc->CreatePath();
        //     rectPath.AddRectangle(350, 100, 100, 50);
        //     gc->SetBrush(wxBrush(wxColour(0, 0, 255)));
        //     gc->FillPath(rectPath);

        //     // Draw a polygon
        //     wxGraphicsPath polygonPath = gc->CreatePath();
        //     polygonPath.MoveToPoint(500, 100);
        //     polygonPath.AddLineToPoint(550, 150);
        //     polygonPath.AddLineToPoint(500, 200);
        //     polygonPath.AddLineToPoint(450, 150);
        //     polygonPath.CloseSubpath();
        //     gc->SetBrush(wxBrush(wxColour(255, 255, 0)));
        //     gc->FillPath(polygonPath);

        //     // Draw a polyline
        //     wxGraphicsPath polylinePath = gc->CreatePath();
        //     polylinePath.MoveToPoint(650, 100);
        //     polylinePath.AddLineToPoint(700, 150);
        //     polylinePath.AddLineToPoint(650, 200);
        //     gc->SetPen(wxPen(wxColour(255, 0, 255), 10));
        //     gc->StrokePath(polylinePath);

        //     // Draw a line
        //     gc->SetPen(wxPen(wxColour(0, 255, 255), 10));
        //     gc->StrokeLine(800, 100, 850, 200);

        //     // Draw a path
        //     wxGraphicsPath customPath = gc->CreatePath();
        //     customPath.MoveToPoint(950, 100);
        //     customPath.AddLineToPoint(1000, 150);
        //     customPath.AddCurveToPoint(1050, 200, 1100, 150, 1150, 100);
        //     gc->SetPen(wxPen(wxColour(128, 128, 128), 10));
        //     gc->StrokePath(customPath);

        //     // Draw text
        //     gc->SetFont(gc->CreateFont(12, "Times New Roman",
        //                                wxFONTSTYLE_NORMAL,
        //                                wxFONTWEIGHT_NORMAL));
        //     gc->DrawText("Hello, World!", 100, 250);
        // }
    }

    void OnMouseWheel(wxMouseEvent& event) {
        int delta = event.GetWheelRotation();
        if (delta > 0) {
            zoomFactor += 0.1f;
        } else {
            zoomFactor -= 0.1f;
            if (zoomFactor < 0.1f) {
                zoomFactor = 0.1f;
            }
        }
        Refresh();
    }

    wxDECLARE_EVENT_TABLE();

private:
    float zoomFactor;
    Group* root;
};

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame) EVT_PAINT(MyFrame::OnPaint)
    EVT_MOUSEWHEEL(MyFrame::OnMouseWheel) wxEND_EVENT_TABLE()

        class MyApp : public wxApp {
public:
    bool OnInit() override {
        if (argc == 2) {
            Parser* parser = Parser::getInstance(argv[1].ToStdString());
            MyFrame* frame = new MyFrame(parser->getRoot());
            frame->Show();
            return true;
        }
        std::cout << "Usage: " << argv[0] << " <filename>" << std::endl;
        return false;
    }
};

wxIMPLEMENT_APP(MyApp);