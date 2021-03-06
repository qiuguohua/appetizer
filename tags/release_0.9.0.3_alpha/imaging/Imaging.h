#ifndef __Imaging_H
#define __Imaging_H

#include <wx/wx.h>
#include <wx/bitmap.h>

class Imaging {

public:
  
  static void StretchBlit(wxDC* destDC, wxDC* sourceDC, wxCoord destX, wxCoord destY, wxCoord destWidth, wxCoord destHeight, wxCoord srcX, wxCoord srcY, wxCoord srcWidth, wxCoord srcHeight);
  static void DrawIconWithTransparency(wxDC* destination, const wxIcon& icon, int destX, int destY);
  static wxBitmap* IconToBitmapWithAlpha(const wxIcon& icon);

};

#endif