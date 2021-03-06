/* CairoDrawingUtils.cpp : Implementation of the header utilities;
 * Author: Maxie D. Schmidt (maxieds@gmail.com)
 * Created: 2019.03.11
 */

#include <math.h>

#include "FL/Fl.H"

#include "CairoDrawingUtils.h"
#include "ConfigOptions.h"
#include "RNAStructure.h"
#include "ThemesConfig.h"
#include "TerminalPrinting.h"

#define TRUNCRGBA(comp)             ((comp < 0 ? 0 : \
                         (comp > CAIRO_COLOR_RGBA_MAXVAL ? CAIRO_COLOR_RGBA_MAXVAL : comp)))
#define INRANGEPCT(alpha)           (alpha >= 0.0 && alpha <= 1.0)
#define TRUNCPCT(alpha)             ((alpha < 0.0 ? 0.0 : (alpha > 1.0 ? 1.0 : alpha)))

CairoColorSpec_t CairoColor_t::ConvertFromFLColor(const Fl_Color &flColor) {
     switch(flColor) {
          case FL_BLACK:
           return CR_BLACK;
      case FL_RED:
           return CR_RED;
      case FL_GREEN:
           return CR_GREEN;
      case FL_BLUE:
           return CR_BLUE;
      case FL_YELLOW:
           return CR_YELLOW;
      case FL_MAGENTA:
           return CR_MAGENTA;
      case FL_CYAN:
           return CR_CYAN;
      case FL_WHITE:
           return CR_WHITE;
      case FL_LOCAL_DARK_GRAY:
           return CR_LIGHT_GRAY;
      case FL_LOCAL_MEDIUM_GREEN:
           return CR_MEDIUM_GREEN;
      case FL_LOCAL_DARK_RED:
           return CR_DARK_RED;
      case FL_LOCAL_BRIGHT_YELLOW:
           return CR_BRIGHT_YELLOW;
      case FL_LOCAL_LIGHT_PURPLE:
           return CR_LIGHT_PURPLE;
      case FL_LOCAL_DEEP_BLUE:
           return CR_DEEP_BLUE;
      default:
           return CR_UNDEFINED;
     }
} 

Fl_Color CairoColor_t::ConvertToFLColor(CairoColorSpec_t &namedCairoColor) {
     switch(namedCairoColor) {
          case CR_BLACK:
           return FL_BLACK;
      case CR_RED:
           return FL_RED;
      case CR_GREEN:
           return FL_GREEN;
      case CR_BLUE:
           return FL_BLUE;
      case CR_YELLOW:
           return FL_YELLOW;
      case CR_MAGENTA:
           return FL_MAGENTA;
      case CR_CYAN:
           return FL_CYAN;
      case CR_WHITE:
           return FL_WHITE;
      default:
           return FL_BLACK;
     }
}

CairoColor_t::CairoColor_t(CairoRGBA_t r, CairoRGBA_t b, CairoRGBA_t g, CairoRGBA_t a) : 
          R(r), G(g), B(b), A(a) {}

CairoColor_t & CairoColor_t::SetRGB(CairoRGBA_t r, CairoRGBA_t b, CairoRGBA_t g) {
     SetRed(r);
     SetGreen(g);
     SetBlue(b);
     return *this;
}

CairoColor_t & CairoColor_t::SetRGBA(CairoRGBA_t r, CairoRGBA_t b, CairoRGBA_t g, CairoRGBA_t a) {
     SetRed(r);
     SetGreen(g);
     SetBlue(b);
     SetAlpha(a);
     return *this;
}

CairoColor_t & CairoColor_t::SetRed(CairoRGBA_t red) { 
     R = TRUNCRGBA(red);
     return *this;
}

CairoColor_t & CairoColor_t::SetRedRatio(float redPct) { 
     R = CAIRO_COLOR_RGBA_MAXVAL * TRUNCPCT(redPct);
     return *this;
}


CairoColor_t & CairoColor_t::SetGreen(CairoRGBA_t green) { 
     G = TRUNCRGBA(green);
     return *this;
}

CairoColor_t & CairoColor_t::SetGreenRatio(float greenPct) { 
     G = CAIRO_COLOR_RGBA_MAXVAL * TRUNCPCT(greenPct);
     return *this;
}

CairoColor_t & CairoColor_t::SetBlue(CairoRGBA_t blue) { 
     B = TRUNCRGBA(blue);
     return *this;
}

CairoColor_t & CairoColor_t::SetBlueRatio(float bluePct) { 
     B = CAIRO_COLOR_RGBA_MAXVAL * TRUNCPCT(bluePct);
     return *this;
}

CairoColor_t & CairoColor_t::SetAlpha(CairoRGBA_t alpha) { 
     A = TRUNCRGBA(alpha);
     return *this;
}

CairoColor_t & CairoColor_t::SetAlphaRatio(float alphaPct) { 
     A = CAIRO_COLOR_RGBA_MAXVAL * TRUNCPCT(alphaPct);
     return *this;
}

CairoColor_t & CairoColor_t::ScaleAlpha(float ratio) { 
     A *= TRUNCPCT(ratio);
     return *this;
}

CairoRGBA_t CairoColor_t::Red() const {
     return R;
}

double CairoColor_t::GetRedRatio() const {
     return (double) R / CAIRO_COLOR_RGBA_MAXVAL;
}

CairoRGBA_t CairoColor_t::Green() const {
     return G;
}

double CairoColor_t::GetGreenRatio() const {
     return (double) G / CAIRO_COLOR_RGBA_MAXVAL;
}

CairoRGBA_t CairoColor_t::Blue() const {
     return B;
}

double CairoColor_t::GetBlueRatio() const {
     return (double) B / CAIRO_COLOR_RGBA_MAXVAL;
}

CairoRGBA_t CairoColor_t::Alpha() const {
     return A;
}

double CairoColor_t::GetAlphaRatio() const {
     return (double) A / CAIRO_COLOR_RGBA_MAXVAL;
}

Fl_Color CairoColor_t::ToFLColorType() const {
     return fl_rgb_color(R, G, B);
}

CairoColor_t CairoColor_t::FromFLColorType(Fl_Color flColor) {
     uchar r, g, b;
     Fl::get_color(flColor, r, g, b);     
     CairoColor_t crColor(r, g, b, CAIRO_COLOR_OPAQUE);
     return crColor;
}

unsigned int CairoColor_t::ToHexInteger() const {
     return ColorUtil::GetRGBColor(R, G, B);
}

CairoColor_t CairoColor_t::FromHexInteger(unsigned int hexColor) {
     CairoColor_t cc(0, 0, 0);
     cc.SetRed(ColorUtil::RGBGetRed(hexColor));
     cc.SetGreen(ColorUtil::RGBGetGreen(hexColor));
     cc.SetBlue(ColorUtil::RGBGetBlue(hexColor));
     cc.SetAlpha(CAIRO_COLOR_OPAQUE);
     return cc;
}

CairoColor_t CairoColor_t::FromNamedConstant(const CairoColorSpec_t &namedConstant) {
     CairoColor_t namedColor = GetCairoColor(namedConstant);
     CairoColor_t cc(0, 0, 0);
     cc.SetRed(namedColor.Red());
     cc.SetGreen(namedColor.Green());
     cc.SetBlue(namedColor.Blue());
     cc.SetAlpha(namedColor.Alpha());
     return cc;
}

CairoColor_t CairoColor_t::GetCairoColor(const CairoColorSpec_t &namedCairoColor) {
    switch (namedCairoColor) {
        case CR_BLACK:
            return GetCairoColor(46, 52, 54);
        case CR_RED:
        return GetCairoColor(164, 0, 0);
        case CR_GREEN:
        return GetCairoColor(115, 210, 22);
        case CR_BLUE:
        return GetCairoColor(32, 74, 135);
        case CR_YELLOW:
        return GetCairoColor(196, 160, 0);
        case CR_MAGENTA:
            return GetCairoColor(249, 0, 201);
    case CR_CYAN:
        return GetCairoColor(32, 195, 215);
        case CR_BRANCH1:
            return GetCairoColor(92, 160, 215);
        case CR_BRANCH2:
            return GetCairoColor(183, 127, 77);
        case CR_BRANCH3:
            return GetCairoColor(243, 153, 193);
        case CR_BRANCH4:
            return GetCairoColor(123, 204, 153);
    case CR_WHITE:
        return GetCairoColor(255, 255, 255);
    case CR_TRANSPARENT:
        return GetCairoColor(255, 255, 255, CAIRO_COLOR_TRANSPARENT);
        case CR_SOLID_BLACK:
        return GetCairoColor(0, 0, 0, 255);
        case CR_SOLID_WHITE:
        return GetCairoColor(255, 255, 255, 255);
    case CR_LIGHT_GRAY:
        return GetCairoColor(46, 52, 54);
    case CR_MEDIUM_GREEN:
        return GetCairoColor(115, 210, 22);
    case CR_DARK_RED:
        return GetCairoColor(147, 38, 38);
    case CR_BRIGHT_YELLOW:
        return GetCairoColor(252, 233, 79);
    case CR_LIGHT_PURPLE:
        return GetCairoColor(173, 127, 168);
    case CR_DEEP_BLUE:
        return GetCairoColor(2, 32, 75);
    default:
        TerminalText::PrintDebug("Unknown named Cairo color %d\n", namedCairoColor);
        return GetCairoColor(206, 92, 0, 128);
    }
}

CairoColor_t CairoColor_t::GetCairoColor(CairoRGBA_t red, CairoRGBA_t green, 
                                 CairoRGBA_t blue, CairoRGBA_t alpha) {
     return CairoColor_t(red, green, blue, alpha);
}

CairoColor_t CairoColor_t::GetCairoColorFromRatio(float redPct, float greenPct, float bluePct, 
                                          float alphaPct) {
     CairoRGBA_t red = CAIRO_COLOR_RGBA_MAXVAL * TRUNCPCT(redPct);
     CairoRGBA_t green = CAIRO_COLOR_RGBA_MAXVAL * TRUNCPCT(greenPct);
     CairoRGBA_t blue = CAIRO_COLOR_RGBA_MAXVAL * TRUNCPCT(bluePct);
     CairoRGBA_t alpha = CAIRO_COLOR_RGBA_MAXVAL * TRUNCPCT(alphaPct);
     return CairoColor_t(red, green, blue, alpha);
}

bool CairoColor_t::ApplyRGBAColor(cairo_t *crContext) const {
     if(crContext == NULL) {
          return false;
     }
     cairo_set_source_rgba(crContext, GetRedRatio(), GetGreenRatio(), 
                   GetBlueRatio(), GetAlphaRatio());
     return true;
}

bool CairoColor_t::ApplyRGBAColor(CairoContext_t &crContext) const {
     cairo_set_source_rgba(crContext.GetCairoContext(), 
                   GetRedRatio(), GetGreenRatio(), 
                   GetBlueRatio(), GetAlphaRatio());
     return true;
}

CairoColor_t & CairoColor_t::operator+=(const CairoColor_t &rhsColor) { 
     SetRed(R + rhsColor.Red());
     SetGreen(G + rhsColor.Green());
     SetBlue(B + rhsColor.Blue());
} 

CairoColor_t & CairoColor_t::operator-=(const CairoColor_t &rhsColor) { 
     SetRed(R - rhsColor.Red());
     SetGreen(G - rhsColor.Green());
     SetBlue(B - rhsColor.Blue());
}

bool CairoColor_t::operator==(const CairoColor_t &rhsColor) const {
     return (R == rhsColor.Red()) && (G == rhsColor.Green()) && 
        (B == rhsColor.Blue()) && (A == rhsColor.Alpha());
}

CairoColor_t CairoColor_t::Lighten(float pct) {
     fl_color(Red(), Green(), Blue());
     Fl_Color flColor = fl_color();
     return FromFLColorType(ColorUtil::Lighter(flColor, TRUNCPCT(pct)));
}

CairoColor_t CairoColor_t::Darken(float pct) {
     fl_color(Red(), Green(), Blue());
     Fl_Color flColor = fl_color();
     return FromFLColorType(ColorUtil::Darker(flColor, TRUNCPCT(pct)));
}

CairoColor_t CairoColor_t::ToGrayscale() {
     CairoColor_t nextColor;
     CairoRGBA_t grayComp = TRUNCRGBA(R * 0.299 + G * 0.587 + B * 0.114);
     nextColor.R = nextColor.G = nextColor.B = grayComp;
     nextColor.A = A;
     return nextColor;
}

CairoColor_t CairoColor_t::ToTransparent() {
     CairoColor_t nextColor;
     nextColor.R = this->R;
     nextColor.G = this->G;
     nextColor.B = this->B;
     nextColor.A = CAIRO_COLOR_TRANSPARENT;
     return nextColor;
}

CairoColor_t CairoColor_t::ToOpaque() {
     CairoColor_t nextColor; 
     nextColor.R = this->R;
     nextColor.G = this->G;
     nextColor.B = this->B;
     nextColor.A = CAIRO_COLOR_OPAQUE;
     return nextColor;
}

CairoColor_t CairoColor_t::Tint(const CairoColor_t &tintColor, float pct) {
     Fl_Color tintFlColor = tintColor.ToFLColorType();
     Fl_Color curFlColor = ToFLColorType();
     return FromFLColorType(fl_color_average(curFlColor, tintFlColor, TRUNCPCT(pct)));
}

uint32_t CairoContext_t::CairoColor_t::ColorUtil::RGBRedComponent(uint32_t rgbHexColor) { 
    return rgbHexColor & ColorUtil::RGBHEX_RED_MASK;
}

uint32_t CairoContext_t::CairoColor_t::ColorUtil::RGBGreenComponent(uint32_t rgbHexColor) { 
    return rgbHexColor & ColorUtil::RGBHEX_GREEN_MASK;
}

uint32_t CairoContext_t::CairoColor_t::ColorUtil::RGBBlueComponent(uint32_t rgbHexColor) { 
    return rgbHexColor & ColorUtil::RGBHEX_BLUE_MASK;
}

uint32_t CairoContext_t::CairoColor_t::ColorUtil::GetRGBColor(uint8_t r, uint8_t g, uint8_t b) {
     return (((r & 0xff) << 24) | ((g & 0xff) << 16) | ((b & 0xff) << 8));
}

uint32_t CairoContext_t::CairoColor_t::ColorUtil::Lighter(uint32_t rgbHexColor, float alpha) { 
     return fl_color_average(rgbHexColor, FL_WHITE, TRUNCPCT(alpha));
}

uint32_t CairoContext_t::CairoColor_t::ColorUtil::Darker(uint32_t rgbHexColor, float alpha) { 
     return fl_color_average(rgbHexColor, FL_BLACK, TRUNCPCT(alpha));
}    

Fl_Color CairoContext_t::CairoColor_t::ColorUtil::Inactive(Fl_Color flColor) { 
     return fl_inactive(flColor);
}

Fl_Color CairoContext_t::CairoColor_t::ColorUtil::Contrast(Fl_Color flColor) { 
     return fl_contrast(flColor, FL_WHITE);
}

uint32_t CairoContext_t::CairoColor_t::ColorUtil::RGBGetRed(uint32_t rgbHexColor) { 
     return((rgbHexColor & 0xff000000) >> ColorUtil::RGBHEX_RED_RSHIFT);
}

uint32_t CairoContext_t::CairoColor_t::ColorUtil::RGBGetGreen(uint32_t rgbHexColor) { 
     return((rgbHexColor & 0x00ff0000) >> ColorUtil::RGBHEX_GREEN_RSHIFT);
}

uint32_t CairoContext_t::CairoColor_t::ColorUtil::RGBGetBlue(uint32_t rgbHexColor) { 
     return((rgbHexColor & 0x0000ff00) >> ColorUtil::RGBHEX_BLUE_RSHIFT);
}

uint32_t ColorUtil::RGBHexTupleFromFLColor(Fl_Color flColor) {
     uchar R, G, B;
     Fl::get_color(flColor, R, G, B);
     return GetRGBColor(R, G, B);
}

void CairoContext_t::FreeCairoStructures(cairo_t *cr, cairo_surface_t *crs) {
     if(cr != NULL) {
          cairo_destroy(cr); 
     }
     if(crs != NULL) {
          cairo_surface_destroy(crs);
     }
}

bool CairoContext_t::InitCairoStructures(size_t w, size_t h, bool freeOldStructs) {
     cairo_surface_t *oldCairoSurface = cairoSurface;
     cairo_t *oldCairoContext = cairoContext;
     width = w;
     height = h;
     cairoSurface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
     cairoContext = cairo_create(cairoSurface);
     BlankFillCanvas(CairoColor_t::GetCairoColor(CairoColorSpec_t::CR_TRANSPARENT));
     if(freeOldStructs) {
          FreeCairoStructures(oldCairoContext, oldCairoSurface);
     }
     return Initialized();
}

bool CairoContext_t::InitCairoStructures(cairo_t *crContext, bool freeOldStructs) {
     cairo_surface_t *oldCairoSurface = cairoSurface;
     cairo_t *oldCairoContext = cairoContext;
     cairoSurface = cairo_get_target(crContext);
     cairoContext = crContext;
     width = cairo_image_surface_get_width(cairoSurface);
     height = cairo_image_surface_get_height(cairoSurface);
     BlankFillCanvas(CairoColor_t::GetCairoColor(CairoColorSpec_t::CR_TRANSPARENT));
     if(freeOldStructs) {
          FreeCairoStructures(oldCairoContext, oldCairoSurface);
     }
     return Initialized();
}

bool CairoContext_t::BlankFillCanvas(CairoColor_t cairoFillColor) {
     if(!Initialized()) {
          return false;
     }
     cairo_save(cairoContext);
     SetColor(cairoFillColor);
     cairo_rectangle(cairoContext, 0, 0, width, height);
     cairo_fill(cairoContext);
     cairo_restore(cairoContext);
     return true;
}

bool CairoContext_t::CopyContextData(const CairoContext_t &crContext) {
     InitCairoStructures(crContext.width, crContext.height, false);
     if(!Initialized()) {
          return false;
     }
     cairo_save(cairoContext);
     cairo_set_source_surface(cairoContext, crContext.cairoSurface, 0, 0);
     cairo_rectangle(cairoContext, 0, 0, width, height);
     cairo_set_operator(cairoContext, CAIRO_OPERATOR_SOURCE);
     cairo_fill(cairoContext);
     cairo_restore(cairoContext);
     return true;
}

cairo_font_slant_t CairoContext_t::ExtractFontSlantFromStyle(uint16_t fontStyle) {
     if(fontStyle & ITALIC) {
          return CAIRO_FONT_SLANT_ITALIC;
     }
     else if(fontStyle & NORMAL) {
          return CAIRO_FONT_SLANT_NORMAL;
     }
     else {
          return CAIRO_FONT_SLANT_OBLIQUE;
     }
}

cairo_font_weight_t CairoContext_t::ExtractFontWeightFromStyle(uint16_t fontStyle) {
     if(fontStyle & BOLD) {
          return CAIRO_FONT_WEIGHT_BOLD;
     }
     return CAIRO_FONT_WEIGHT_NORMAL;
}

CairoContext_t::CairoContext_t() : width(0), height(0), cairoSurface(NULL), cairoContext(NULL) {}

CairoContext_t::CairoContext_t(size_t w, size_t h) {
     InitCairoStructures(w, h, false);
}

CairoContext_t::CairoContext_t(const CairoContext_t &cctxt) : CairoContext_t(cctxt.width, cctxt.height) {
     CopyContextData(cctxt);
}

CairoContext_t & CairoContext_t::operator=(const CairoContext_t &rhsContext) {
     if(this != &rhsContext) {
          CopyContextData(rhsContext);
     }
     return *this;
}

CairoContext_t::~CairoContext_t() {
     if(cairoSurface != NULL) {
          cairo_surface_flush(cairoSurface);
     }
     FreeCairoStructures(cairoContext, cairoSurface);
}

bool CairoContext_t::operator==(const uintptr_t &ptrAddr) const {
     return (uintptr_t) cairoContext == ptrAddr;
}

cairo_t * CairoContext_t::operator->() const {
     return cairoContext;
}

size_t CairoContext_t::GetWidth() const {
     return width;
}

size_t CairoContext_t::GetHeight() const {
     return height;
}

bool CairoContext_t::Initialized() const {
     return (width > 0) && (height > 0) && (cairoContext != NULL);
}

cairo_t * CairoContext_t::GetCairoContext() const {
     return cairoContext;
}

cairo_surface_t * CairoContext_t::GetCairoSurface() const {
     return cairo_get_target(cairoContext);
     //return cairoSurface;
}

bool CairoContext_t::LoadFromImage(const char *imageFilePath) {
     if(imageFilePath == NULL) {
          return false;
     }
     cairo_surface_t *oldCairoSurface = cairoSurface;
     cairo_t *oldCairoContext = cairoContext;
     cairoSurface = cairo_image_surface_create_from_png(imageFilePath);
     cairoContext = cairo_create(cairoSurface);
     width = cairo_image_surface_get_width(cairoSurface);
     height = cairo_image_surface_get_height(cairoSurface);
     FreeCairoStructures(oldCairoContext, oldCairoSurface);
     return Initialized();
}

bool CairoContext_t::SaveToImage(const char *imageOutPath) {
     if(imageOutPath == NULL) {
          return false;
     }
     cairo_status_t opStatus = cairo_surface_write_to_png(cairo_get_target(cairoContext), 
                                                  imageOutPath);
     return opStatus == CAIRO_STATUS_SUCCESS;
}

CairoContext_t & CairoContext_t::Resize(size_t newWidth, size_t newHeight, bool scalePrevPixels) {
     if(!Initialized() || newWidth == 0 || newHeight == 0) {
          return *this;
     }
     cairo_surface_t *newCairoSurface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, newWidth, newHeight);
     cairo_t *newCairoContext = cairo_create(newCairoSurface);
     if(scalePrevPixels) {
          cairo_scale(newCairoContext, (double) newWidth / width, (double) newHeight / height);
     }
     cairo_set_source_surface(newCairoContext, cairo_get_target(cairoContext), 0, 0);
     cairo_set_operator(newCairoContext, CAIRO_OPERATOR_SOURCE);
     cairo_paint(newCairoContext);
     cairo_identity_matrix(newCairoContext);
     width = newWidth;
     height = newHeight;
     cairo_surface_t *oldCairoSurface = cairoSurface;
     cairo_t *oldCairoContext = cairoContext;
     cairoSurface = newCairoSurface;
     cairoContext = newCairoContext;
     FreeCairoStructures(oldCairoContext, oldCairoSurface);
     return *this;
}

void CairoContext_t::SaveSettings() {
     if(cairoContext != NULL) {
          cairo_save(cairoContext);
     }
}

void CairoContext_t::RestoreSettings() {
     if(cairoContext != NULL) {
          cairo_restore(cairoContext);
     }
}

bool CairoContext_t::SetFontFace(const char *fontFaceName, uint16_t fontStyle) { 
     cairo_select_font_face(cairoContext, fontFaceName, 
                    ExtractFontSlantFromStyle(fontStyle), 
                ExtractFontWeightFromStyle(fontStyle));
     return true;
}

bool CairoContext_t::SetFontFace(uint16_t fontStyle) {
     const char *fontName;
     if(fontStyle & SERIF) {
          fontName = "Serif";
     }
     else if(fontStyle & MONOSPACE) {
      fontName = "Sans Mono";
     }
     else if(fontStyle & FUNKY) {
      fontName = "Purissa";
     }
     else {
      fontName = "Sans";
     }
     return SetFontFace(fontName, fontStyle);
}

bool CairoContext_t::SetFontSize(int fontSize) {
     cairo_set_font_size(cairoContext, fontSize);
     return true;
}

bool CairoContext_t::SetStrokeSize(int strokeSize) {
     cairo_set_line_width(cairoContext, strokeSize);
     return true;
}

bool CairoContext_t::CairoToGrayscale() {
     return false;
}

bool CairoContext_t::SetColor(const CairoColor_t &cairoColor) {
     return cairoColor.ApplyRGBAColor(this->cairoContext);
}

bool CairoContext_t::Scale(double sxy) {
     return Scale(sxy, sxy);
}

bool CairoContext_t::Scale(double sx, double sy) {
     int nextWidth = (int) (sx * GetWidth());
     int nextHeight = (int) (sy * GetHeight());
     Resize(nextWidth, nextHeight, true);
     return true;
}

bool CairoContext_t::Translate(int xoffset, int yoffset) {
     cairo_translate(cairoContext, xoffset, yoffset);
     return true;
}

bool CairoContext_t::ResetToIdentityTransform() {
     cairo_identity_matrix(cairoContext);
     return true;
}

bool CairoContext_t::OverlayGraphics(const CairoContext_t &overlayContext, int startX, int startY) {
     cairo_t *crOverlayContext = overlayContext.GetCairoContext();
     cairo_surface_t *crOverlaySurface = cairo_get_target(crOverlayContext);
     int overlayWidth = cairo_image_surface_get_width(crOverlaySurface);
     int overlayHeight = cairo_image_surface_get_height(crOverlaySurface);
     SaveSettings();
     cairo_set_source_surface(cairoContext, crOverlaySurface, startX, startY);
     cairo_paint(cairoContext);
     cairo_identity_matrix(cairoContext);
     RestoreSettings();
     return true;
}

bool CairoContext_t::DrawBaseNode(int centerX, int centerY, 
                          const char *nodeLabel, 
                  size_t nodeSize, 
                  CairoColor_t cairoBaseColor, 
                  NodeStyle_t nodeStyle) {
     cairoBaseColor = cairoBaseColor.ToOpaque();
     CairoColor_t nodeBGColor = cairoBaseColor.Lighten(0.75).ToOpaque();
     CairoColor_t nodeBorderColor = cairoBaseColor.Darken(0.1).ToOpaque();
     CairoColor_t nodeTextColor = CairoColor_t::GetCairoColor(CairoColorSpec_t::CR_SOLID_BLACK);
     CairoColor_t transparentBaseColor = CairoColor_t::GetCairoColor(CairoColorSpec_t::CR_TRANSPARENT);
     int nodeRadius = (int) (nodeSize / M_SQRT2 / 2.0 - 3);
     CairoContext_t nodeOverlayContext(nodeSize, nodeSize);
     cairo_t *cairoContext2 = nodeOverlayContext.GetCairoContext();
     transparentBaseColor.ApplyRGBAColor(cairoContext2);
     cairo_rectangle(cairoContext2, 0, 0, nodeSize, nodeSize);
     cairo_fill(cairoContext2);
     if(nodeStyle == CIRCLE_NODE) { 
          nodeBGColor.ApplyRGBAColor(cairoContext2);
          cairo_arc(cairoContext2, nodeSize / 2, nodeSize / 2, nodeRadius, 
                0.0, 2.0 * M_PI);
      cairo_fill(cairoContext2);
      nodeBorderColor.ApplyRGBAColor(cairoContext2);
      cairo_arc(cairoContext2, nodeSize / 2, nodeSize / 2, nodeRadius, 
            0.0, 2.0 * M_PI);
          cairo_stroke(cairoContext2);
     }
     else if(nodeStyle == SQUARE_NODE) {
          nodeBGColor.ApplyRGBAColor(cairoContext2);
          cairo_rectangle(cairoContext2, 0, 0, 0.95 * nodeSize, 0.95 * nodeSize);
      cairo_fill(cairoContext2);
      nodeBorderColor.ApplyRGBAColor(cairoContext2);
          cairo_rectangle(cairoContext2, 0, 0, 0.95 * nodeSize, 0.95 * nodeSize);
          cairo_stroke(cairoContext2);
     }
     // now draw the text in the center framed in the center of the node:
     nodeTextColor.ApplyRGBAColor(cairoContext2);
     nodeOverlayContext.DrawText(nodeSize / 2, nodeSize / 2, nodeLabel, CENTER);
     transparentBaseColor.ApplyRGBAColor(cairoContext2);
     transparentBaseColor.ApplyRGBAColor(cairoContext);
     OverlayGraphics(nodeOverlayContext, centerX - nodeSize / 2, centerY - nodeSize / 2);
     return true;
}

bool CairoContext_t::DrawLine(size_t baseX, size_t baseY, size_t nextX, size_t nextY) {
     SaveSettings();
     cairo_move_to(cairoContext, baseX, baseY);
     cairo_line_to(cairoContext, nextX, nextY);
     cairo_stroke(cairoContext);
     RestoreSettings();
     return true;
}

bool CairoContext_t::DrawText(size_t baseX, size_t baseY, const char *text, 
                      CairoTextDrawParams_t drawAlign) {
     if(text == NULL) {
          return false;
     }
     else if(drawAlign == CENTER) {
      cairo_text_extents_t textStrDims;
      cairo_text_extents(cairoContext, text, &textStrDims);
          baseX -= textStrDims.width / 2;
      baseY += textStrDims.height / 2;
     }
     cairo_move_to(cairoContext, baseX, baseY);
     cairo_show_text(cairoContext, text);
     return true;
}
