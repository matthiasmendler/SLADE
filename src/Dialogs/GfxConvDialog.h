
#ifndef __GFXCONVDIALOG_H__
#define	__GFXCONVDIALOG_H__

#include "UI/SDialog.h"
#include "UI/WxBasicControls.h"
#include "Graphics/SImage/SImage.h"
#include "Graphics/SImage/SIFormat.h"

/* Convert from anything to:
 * Doom Gfx
 * Doom Flat
 * PNG 32bit
 * PNG Paletted
 *
 * Conversion options:
 *	Colours:
 *		- Specify target palette (only if converting to paletted)
 *		- Specify palette conversion type:
 *			- Keep palette indices (only if converting from 8bit)
 *			- Nearest colour matching
 *
 *	Transparency:
 *		- Specify threshold alpha, anything above is opaque (optional if converting from 32bit)
 *		- Specify transparency info:
 *			- Keep existing transparency (threshold comes into play from 32bit-paletted)
 *			- Select transparency colour (to 32bit - select colour, to paletted - select from target palette)
 */

class Archive;
class ArchiveEntry;
class CTexture;
class Palette;

struct gcd_item_t
{
	ArchiveEntry*	entry;
	CTexture*		texture;
	SImage			image;
	bool			modified;
	SIFormat*		new_format;
	Palette*	palette;
	Archive*		archive;
	bool			force_rgba;

	gcd_item_t(ArchiveEntry* entry = nullptr)
	{
		this->entry = entry;
		this->texture = nullptr;
		this->modified = false;
		this->new_format = nullptr;
		this->palette = nullptr;
		this->archive = nullptr;
		this->force_rgba = false;
	}

	gcd_item_t(CTexture* texture, Palette* palette = nullptr, Archive* archive = nullptr, bool force_rgba = false)
	{
		this->entry = nullptr;
		this->texture = texture;
		this->modified = false;
		this->new_format = nullptr;
		this->palette = palette;
		this->archive = archive;
		this->force_rgba = force_rgba;
	}
};

class GfxCanvas;
class PaletteChooser;
class ColourBox;
class GfxConvDialog : public SDialog
{
private:
	struct conv_format_t
	{
		SIFormat*	format;
		int			coltype;

		conv_format_t(SIFormat* format = nullptr, int coltype = RGBA)
		{
			this->format = format;
			this->coltype = coltype;
		}
	};

	vector<gcd_item_t>		items;
	size_t					current_item;
	vector<conv_format_t>	conv_formats;
	conv_format_t			current_format;

	wxStaticText*	label_current_format;
	GfxCanvas*		gfx_current;
	GfxCanvas*		gfx_target;
	wxButton*		btn_convert;
	wxButton*		btn_convert_all;
	wxButton*		btn_skip;
	wxButton*		btn_skip_all;
	wxChoice*		combo_target_format;
	PaletteChooser*	pal_chooser_current;
	PaletteChooser*	pal_chooser_target;
	wxButton*		btn_colorimetry_settings;

	wxCheckBox*		cb_enable_transparency;
	wxRadioButton*	rb_transparency_existing;
	wxRadioButton*	rb_transparency_colour;
	wxRadioButton*	rb_transparency_brightness;
	wxSlider*		slider_alpha_threshold;
	ColourBox*		colbox_transparent;

	// Conversion options
	Palette		target_pal;
	uint8_t			pal_convert_type;	// 0=nearest colour, 1=keep indices
	uint8_t			alpha_threshold;
	bool			keep_trans;
	rgba_t			colour_trans;

	bool		nextItem();

	// Static
	static string	current_palette_name;
	static string	target_palette_name;

public:
	GfxConvDialog(wxWindow* parent);
	~GfxConvDialog();

	void	setupLayout();

	void	openEntry(ArchiveEntry* entry);
	void	openEntries(vector<ArchiveEntry*> entries);
	void	openTextures(vector<CTexture*> textures, Palette* palette = nullptr, Archive* archive = nullptr, bool force_rgba = false);
	void	updatePreviewGfx();
	void	updateControls();
	void	getConvertOptions(SIFormat::convert_options_t& opt);

	bool			itemModified(int index);
	SImage*			getItemImage(int index);
	SIFormat*		getItemFormat(int index);
	Palette*	getItemPalette(int index);

	void	applyConversion();

	// Events
	void	onResize(wxSizeEvent& e);
	void	onBtnConvert(wxCommandEvent& e);
	void	onBtnConvertAll(wxCommandEvent& e);
	void	onBtnSkip(wxCommandEvent& e);
	void	onBtnSkipAll(wxCommandEvent& e);
	void	onTargetFormatChanged(wxCommandEvent& e);
	void	onCurrentPaletteChanged(wxCommandEvent& e);
	void	onTargetPaletteChanged(wxCommandEvent& e);
	void	onAlphaThresholdChanged(wxCommandEvent& e);
	void	onEnableTransparencyChanged(wxCommandEvent& e);
	void	onTransTypeChanged(wxCommandEvent& e);
	void	onTransColourChanged(wxEvent& e);
	void	onPreviewCurrentMouseDown(wxMouseEvent& e);
	void	onBtnColorimetrySettings(wxCommandEvent& e);
};

#endif //__GFXCONVDIALOG_H__
