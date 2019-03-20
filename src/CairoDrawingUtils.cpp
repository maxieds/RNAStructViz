/* CairoDrawingUtils.cpp : Implementation of the header utilities;
 * Author: Maxie D. Schmidt (maxieds@gmail.com)
 * Created: 2019.03.11
 */

#include "CairoDrawingUtils.h"

#define TRUNCRGBA(comp)             ((comp < 0 ? 0 : \
			             (comp > CAIRO_COLOR_RGBA_MAXVAL ? CAIRO_COLOR_RGBA_MAXVAL : comp)))
#define INRANGEPCT(alpha)           (alpha >= 0.0 && alpha <= 1.0)
#define TRUNCPCT(alpha)             ((alpha < 0.0 ? 0.0 : (alpha > 1.0 ? 1.0 : alpha)))

CairoColorSpec_t CairoColor_t::ConvertFromFLColor(const Fl_Color &flColor) const {
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
	  case CR_MAGENTA:
	       return CR_MAGENTA;
	  case FL_CYAN:
	       return CR_CYAN;
	  case FL_WHITE:
	       return CR_WHITE;
	  default:
	       return CR_UNDEFINED;
     }
} 

Fl_Color CairoColor_t::ConvertToFLColor(CairoColorSpec_t &namedCairoColor) const {
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
     return ColorUtil::GetRGBColor(R, G, B);
}

CairoColor_t CairoColor_t::FromFLColorType(Fl_Color flColor) {
     uint32_t flHexColor = ColorUtil::RGBHexTupleFromFLColor(flColor);
     CairoColor_t crColor(0, 0, 0);
     crColor.SetRGB(ColorUtil::RGBGetRed(flHexColor), 
	            ColorUtil::RGBGetGreen(flHexColor), 
	            ColorUtil::RGBGetBlue(flHexColor));
     return crColor;
}

unsigned int CairoColor_t::ToHexInteger() const {
     return ColorUtil::GetRGBColor(R, G, B);
}

CairoColor_t & CairoColor_t::FromHexInteger(unsigned int hexColor) {
     SetRed(ColorUtil::RGBGetRed(hexColor));
     SetGreen(ColorUtil::RGBGetGreen(hexColor));
     SetBlue(ColorUtil::RGBGetBlue(hexColor));
     return *this;
}

CairoColor_t & CairoColor_t::FromNamedConstant(const CairoColorSpec_t &namedConstant) {
     CairoColor_t namedColor = GetCairoColor(namedConstant);
     SetRed(namedColor.Red());
     SetGreen(namedColor.Green());
     SetBlue(namedColor.Blue());
     SetAlpha(namedColor.Alpha());
     return *this;
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
	    return GetCairoColor(239, 41, 159);
        case CR_CYAN:
	    return GetCairoColor(0, 195, 215);
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
	    return GetCairoColor(255, 255, 255, 0x7f);
        case CR_SOLID_BLACK:
	    return GetCairoColor(0, 0, 0, 255);
        case CR_SOLID_WHITE:
	    return GetCairoColor(255, 255, 255, 255);
	case CR_LIGHT_GRAY:
	    return GetCairoColor(46, 52, 54);
	default:
            return GetCairoColor(0, 0, 0, 255);
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

CairoColor_t & CairoColor_t::Lighten(float pct) {
     FromFLColorType(ColorUtil::Lighter(ToHexInteger(), TRUNCPCT(pct)));
     return *this;     
}

CairoColor_t & CairoColor_t::Darken(float pct) {
     FromFLColorType(ColorUtil::Darker(ToHexInteger(), TRUNCPCT(pct)));
     return *this;     
}

CairoColor_t & CairoColor_t::ToGrayscale() {
     CairoRGBA_t grayComp = TRUNCRGBA(R * 0.299 + G * 0.587 + B * 0.114);
     R = G = B = grayComp;
     return *this;
}

CairoColor_t & CairoColor_t::ToTransparent() {
     A = CAIRO_COLOR_TRANSPARENT;
     return *this;
}

CairoColor_t & CairoColor_t::ToOpaque() {
     A = CAIRO_COLOR_OPAQUE;
     return *this;
}

CairoColor_t & CairoColor_t::Tint(const CairoColor_t &tintColor, float pct) {
     Fl_Color tintFlColor = tintColor.ToFLColorType();
     Fl_Color curFlColor = ToFLColorType();
     FromFLColorType(fl_color_average(curFlColor, tintFlColor, TRUNCPCT(pct)));
     return *this;
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
     return((rgbHexColor >> ColorUtil::RGBHEX_RED_RSHIFT) & 0x000000ff);
}

uint32_t CairoContext_t::CairoColor_t::ColorUtil::RGBGetGreen(uint32_t rgbHexColor) { 
     return((rgbHexColor >> ColorUtil::RGBHEX_GREEN_RSHIFT) & 0x000000ff);
}

uint32_t CairoContext_t::CairoColor_t::ColorUtil::RGBGetBlue(uint32_t rgbHexColor) { 
     return((rgbHexColor >> ColorUtil::RGBHEX_BLUE_RSHIFT) & 0x000000ff);
}

uint32_t ColorUtil::RGBHexTupleFromFLColor(Fl_Color flColor) {
     uchar R, G, B;
     Fl::get_color(flColor, R, G, B);
     return GetRGBColor(R, G, B);
}

void CairoContext_t::FreeCairoStructures() {
     if(cairoSurface != NULL) {
          cairo_surface_destroy(cairoSurface);
	  cairoSurface = NULL;
     }
     if(cairoContext != NULL) {
          cairo_destroy(cairoContext); 
          cairoContext = NULL;
     }
}

bool CairoContext_t::InitCairoStructures(size_t w, size_t h) {
     FreeCairoStructures();
     width = w;
     height = h;
     cairoSurface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
     cairoContext = cairo_create(cairoSurface);
     BlankFillCanvas();
     return Initialized();
}

bool CairoContext_t::InitCairoStructures(cairo_t *crContext) {
     FreeCairoStructures();
     cairoSurface = cairo_get_target(crContext);
     cairoContext = crContext;
     width = cairo_image_surface_get_width(cairoSurface);
     height = cairo_image_surface_get_height(cairoSurface);
     BlankFillCanvas();
     return Initialized();
}

bool CairoContext_t::BlankFillCanvas() {
     if(!Initialized()) {
          return false;
     }
     cairo_save(cairoContext);
     SetColor(CairoColor_t::GetCairoColor(CairoColorSpec_t::CR_TRANSPARENT));
     cairo_rectangle(cairoContext, 0, 0, width, height);
     cairo_fill(cairoContext);
     cairo_restore(cairoContext);
     return true;
}

bool CairoContext_t::CopyContextData(const CairoContext_t &crContext) {
     InitCairoStructures(crContext.width, crContext.height);
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

CairoContext_t::CairoContext_t(size_t w, size_t h) : CairoContext_t() {
     InitCairoStructures(w, h);
}

CairoContext_t::CairoContext_t(cairo_t *crContext) : CairoContext_t() { 
     InitCairoStructures(crContext);     
}

CairoContext_t::CairoContext_t(const CairoContext_t &cctxt) {
     CopyContextData(cctxt);
}

CairoContext_t & CairoContext_t::operator=(const CairoContext_t &rhsContext) {
     if(this != &rhsContext) {
          CopyContextData(rhsContext);
     }
     return *this;
}

CairoContext_t::~CairoContext_t() {
     FreeCairoStructures();
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
     return cairoSurface;
}

bool CairoContext_t::LoadFromImage(const char *imageFilePath) {
     if(imageFilePath == NULL) {
          return false;
     }
     FreeCairoStructures();
     cairoSurface = cairo_image_surface_create_from_png(imageFilePath);
     cairoContext = cairo_create(cairoSurface);
     width = cairo_image_surface_get_width(cairoSurface);
     height = cairo_image_surface_get_height(cairoSurface);
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
     CairoContext_t contextCopy = *this;
     if(scalePrevPixels) {
          cairo_scale(contextCopy.cairoContext, newWidth / width, newHeight / height);
     }
     InitCairoStructures(newWidth, newHeight);
     cairo_save(cairoContext);
     cairo_set_source_surface(cairoContext, contextCopy.cairoSurface, 0, 0);
     cairo_rectangle(cairoContext, 0, 0, newWidth, newHeight);
     cairo_paint(cairoContext);
     cairo_restore(cairoContext);
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

bool CairoContext_t::DrawBaseNode(int centerX, int centerY, char baseChar, 
		                  size_t baseIdx, size_t nodeSize, 
				  CairoColor_t cairoBaseColor, 
				  NodeStyle_t nodeStyle) {
     // TODO: Much more to implement here ... 
     SaveSettings();
     SetColor(cairoBaseColor);
     char pairStr[16];
     snprintf(pairStr, 16, "[%c]\0", toupper(baseChar));
     DrawText(centerX, centerY, pairStr, CENTER);
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
          baseX += textStrDims.width / 2;
	  baseY += textStrDims.height / 2;
     }
     cairo_move_to(cairoContext, baseX, baseY);
     cairo_show_text(cairoContext, text);
     return true;
}