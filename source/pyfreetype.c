#include <Python.h>

#include "pyfreetype_font.h"
#include "pyfreetype_charmapiter.h"

#include <ft2build.h>
#include FT_FREETYPE_H

FT_Library ftlib = NULL;

static PyObject * pyfreetype_ft_version(PyObject * self, PyObject * args)
{
	return Py_BuildValue("(iii)", FREETYPE_MAJOR, FREETYPE_MINOR, FREETYPE_PATCH);
}

static PyObject * pyfreetype_open_font(PyObject * self, PyObject * args)
{
	pyfreetype_Font * font = (pyfreetype_Font*)pyfreetype_FontType.tp_alloc(&pyfreetype_FontType, 0);

	const char * filename;
	int index = 0;
	FT_Error err = 0;

	if (!PyArg_ParseTuple(args, "s|i", &filename, &index))
		return NULL;

	err = FT_New_Face(ftlib, filename, index, &font->m_face);

	if (err)
		return NULL;

	font->m_numCharMaps = PyInt_FromLong(font->m_face->num_charmaps);
	font->m_familyName = PyString_FromString(font->m_face->family_name);
	font->m_styleName = PyString_FromString(font->m_face->style_name);

	font->m_is_flag_scalable = PyBool_FromLong(font->m_face->face_flags & FT_FACE_FLAG_SCALABLE);
	font->m_is_flag_fixed_sizes = PyBool_FromLong(font->m_face->face_flags & FT_FACE_FLAG_FIXED_SIZES);
	font->m_is_flag_fixed_width = PyBool_FromLong(font->m_face->face_flags & FT_FACE_FLAG_FIXED_WIDTH);
	font->m_is_flag_sfnt = PyBool_FromLong(font->m_face->face_flags & FT_FACE_FLAG_SFNT);
	font->m_is_flag_horizontal = PyBool_FromLong(font->m_face->face_flags & FT_FACE_FLAG_HORIZONTAL);
	font->m_is_flag_vertical = PyBool_FromLong(font->m_face->face_flags & FT_FACE_FLAG_VERTICAL);
	font->m_is_flag_kerning = PyBool_FromLong(font->m_face->face_flags & FT_FACE_FLAG_KERNING);
	font->m_is_flag_fast_glyphs = PyBool_FromLong(font->m_face->face_flags & FT_FACE_FLAG_FAST_GLYPHS);
	font->m_is_flag_multiple_masters = PyBool_FromLong(font->m_face->face_flags & FT_FACE_FLAG_MULTIPLE_MASTERS);
	font->m_is_flag_names = PyBool_FromLong(font->m_face->face_flags & FT_FACE_FLAG_GLYPH_NAMES);
	font->m_is_flag_external_stream = PyBool_FromLong(font->m_face->face_flags & FT_FACE_FLAG_EXTERNAL_STREAM);
	font->m_is_flag_hinter = PyBool_FromLong(font->m_face->face_flags & FT_FACE_FLAG_HINTER);
	font->m_is_flag_cid_keyed = PyBool_FromLong(font->m_face->face_flags & FT_FACE_FLAG_CID_KEYED);
	font->m_is_flag_tricky = PyBool_FromLong(font->m_face->face_flags & FT_FACE_FLAG_TRICKY);

	font->m_is_italic = PyBool_FromLong(font->m_face->style_flags & FT_STYLE_FLAG_ITALIC);
	font->m_is_bold = PyBool_FromLong(font->m_face->style_flags & FT_STYLE_FLAG_BOLD);

	return (PyObject*)font;
}

static PyMethodDef ModuleMethods[] = 
{
	{"version",		pyfreetype_ft_version,	METH_VARARGS,	"Retrieve the free type version used by this module."},
	{"open_font",	pyfreetype_open_font,	METH_VARARGS,	"Return a new font object."},

	{NULL, NULL, 0, NULL}        /* Sentinel */
};

PyMODINIT_FUNC initpyfreetype(void)
{
	PyObject * module = NULL;
	FT_Error res = FT_Init_FreeType(&ftlib);

	if (res)
	{
		// SERIOUS ERROR NEEDS HANDLING!
	}

	module = Py_InitModule("pyfreetype", ModuleMethods);

	pyfreetype_register_font_type(module);
	pyfreetype_register_charmapiter_type(module);
}