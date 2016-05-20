﻿#include "XlsConverter.h"

#include "../XlsFormat/Binary/CompoundFile.h"
#include "../XlsFormat/Binary/CFStreamCacheReader.h"

#include "../XlsFormat/Logic/GlobalWorkbookInfo.h"
#include "../XlsFormat/Logic/WorkbookStreamObject.h"
#include "../XlsFormat/Logic/WorksheetSubstream.h"
#include "../XlsFormat/Logic/GlobalsSubstream.h"
#include "../XlsFormat/Logic/ChartSheetSubstream.h"

#include "../XlsFormat/Logic/BinProcessor.h"
#include "../XlsFormat/Logic/SummaryInformationStream/SummaryInformation.h"

#include "../XlsFormat/Logic/Biff_unions/FORMATTING.h"
#include "../XlsFormat/Logic/Biff_unions/THEME.h"
#include "../XlsFormat/Logic/Biff_unions/GLOBALS.h"
#include "../XlsFormat/Logic/Biff_unions/COLUMNS.h"
#include "../XlsFormat/Logic/Biff_unions/SHAREDSTRINGS.h"
#include "../XlsFormat/Logic/Biff_unions/HLINK.h"
#include "../XlsFormat/Logic/Biff_unions/LBL.h"
#include "../XlsFormat/Logic/Biff_unions/OBJECTS.h"
#include "../XlsFormat/Logic/Biff_unions/MSODRAWINGGROUP.h"
#include "../XlsFormat/Logic/Biff_unions/OBJ.h"
#include "../XlsFormat/Logic/Biff_unions/TEXTOBJECT.h"
#include "../XlsFormat/Logic/Biff_unions/CHART.h"

#include <Logic/Biff_records/HLink.h>
#include <Logic/Biff_records/MsoDrawingGroup.h>
#include <Logic/Biff_records/MsoDrawing.h>
#include <Logic/Biff_records/Obj.h>
#include <Logic/Biff_records/TxO.h>

#include <Logic/Biff_structures/URLMoniker.h>
#include <Logic/Biff_structures/FileMoniker.h>

#include <Logic/Biff_structures/ODRAW/OfficeArtBStoreContainer.h>
#include <Logic/Biff_structures/ODRAW/SimpleOfficeArtContainers.h>
#include <Logic/Biff_structures/ODRAW/OfficeArtFOPT.h>
#include <Logic/Biff_structures/ODRAW/OfficeArtFOPTE.h>
#include <Logic/Biff_structures/ODRAW/OfficeArtFSP.h>
#include <Logic/Biff_structures/ODRAW/OfficeArtBlip.h>

#include "xlsx_conversion_context.h"
#include "xlsx_package.h"

#include <simple_xml_writer.h>

#include <boost/lexical_cast.hpp>
#include <boost/utility.hpp>
#include "../../../DesktopEditor/common/File.h"

#if !defined(_WIN32) && !defined(_WIN64)

    typedef struct tagBITMAPINFOHEADER
    {
            _UINT32      biSize;
            _INT32       biWidth;
            _INT32       biHeight;
            _UINT16       biPlanes;
            _UINT16       biBitCount;
            _UINT32      biCompression;
            _UINT32      biSizeImage;
            _INT32       biXPelsPerMeter;
            _INT32       biYPelsPerMeter;
            _UINT32      biClrUsed;
            _UINT32      biClrImportant;
    } BITMAPINFOHEADER;
#endif

XlsConverter::XlsConverter(const std::wstring & xls_file, const std::wstring & _xlsx_path, const ProgressCallback* CallBack) 
{
	xlsx_path		= _xlsx_path;
	output_document = NULL;
	xlsx_context	= NULL;
	
	pCallBack			= CallBack;
	bUserStopConvert	= false;

	try{
		XLS::CompoundFile cfile(xls_file, XLS::CompoundFile::cf_ReadMode);

		XLS::CFStreamPtr summary;
		XLS::CFStreamPtr doc_summary;

		try
		{
			summary = cfile.getSummaryInformationStream();
		}
		catch (...)
		{
		}

		try
		{
			doc_summary = cfile.getDocumentSummaryInformationStream();
		}
		catch (...)
		{
			return;
		}

        _UINT16 workbook_code_page = XLS::WorkbookStreamObject::DefaultCodePage;
		if(summary)
		{
			OLEPS::SummaryInformation summary_info(summary);
			workbook_code_page = summary_info.GetCodePage();
		}
		else if(doc_summary)
		{
			OLEPS::SummaryInformation doc_summary_info(doc_summary);
			workbook_code_page = doc_summary_info.GetCodePage();
		}

		if(1200/* UTF-16 */ == workbook_code_page || 0/*error*/ == workbook_code_page)
		{
			workbook_code_page = XLS::WorkbookStreamObject::DefaultCodePage;
		}

		xls_global_info = boost::shared_ptr<XLS::GlobalWorkbookInfo>(new XLS::GlobalWorkbookInfo(workbook_code_page, this));


		XLS::CFStreamCacheReader stream_reader(cfile.getWorkbookStream(), xls_global_info);

		xls_document = boost::shared_ptr<XLS::WorkbookStreamObject>(new XLS::WorkbookStreamObject(workbook_code_page));

		XLS::BinReaderProcessor proc(stream_reader , xls_document.get() , true);
		proc.mandatory(*xls_document.get());

	}
	catch(...)
	{
		return;
	}
	if (UpdateProgress(400000))return;

	output_document		= new oox::package::xlsx_document();
    xlsx_context		= new oox::xlsx_conversion_context(output_document);
}

XlsConverter::~XlsConverter() 
{
	if (xlsx_context)		delete xlsx_context;
	if (output_document)	delete output_document;
}

#define PROGRESSEVENT_ID	0

bool XlsConverter::UpdateProgress(long nComplete)
{
	if (pCallBack)
	{
		pCallBack->OnProgress (pCallBack->caller, PROGRESSEVENT_ID, nComplete);

		bUserStopConvert = 0;
		pCallBack->OnProgressEx (pCallBack->caller, PROGRESSEVENT_ID, nComplete, &bUserStopConvert);

		if (bUserStopConvert !=0 ) return TRUE;
	}

	return FALSE;
}
void XlsConverter::write()
{
	if (!output_document)return;
	output_document->write(xlsx_path);

	delete output_document; output_document = NULL;

	if (UpdateProgress(1000000))return;
}

void XlsConverter::convertDocument()
{
    if (!xls_document)     return;
    if (!output_document)   return;
    if (!xlsx_context)       return;

	xlsx_context->start_document();

	convert((XLS::WorkbookStreamObject*)xls_document.get());

	if (UpdateProgress(800000))return;

	xlsx_context->end_document();

	if (UpdateProgress(850000))return;
}

void XlsConverter::convert(XLS::BaseObject	*xls_unknown)
{
	if (xls_unknown == NULL) return;

	XLS::ElementType type = xls_unknown->get_type();

	switch (type)
	{
	case XLS::typeHLINK:
		{
			XLS::HLINK * hlink = dynamic_cast<XLS::HLINK *>(xls_unknown);
			convert(hlink);
		}break;
	case XLS::typeLBL:	
		{
			XLS::LBL * lbl = dynamic_cast<XLS::LBL *>(xls_unknown);
			convert(lbl);
		}break;
	case XLS::typeOBJECTS:
		{
			XLS::OBJECTS * obj = dynamic_cast<XLS::OBJECTS *>(xls_unknown);
			convert(obj);
		}break;
	case XLS::typeAnyObject:	
	default:
		{
			for (std::list<XLS::BaseObjectPtr>::iterator it = xls_unknown->elements_.begin(); it != xls_unknown->elements_.end(); it++)
			{
				(*it)->serialize(xlsx_context->current_sheet().sheetData());
			}
		}
	}

}

void XlsConverter::convert(XLS::WorkbookStreamObject* woorkbook)
{
	if (woorkbook == NULL) return;

	convert((XLS::GlobalsSubstream*)woorkbook->m_GlobalsSubstream.get());

    for (int i=0 ; i < woorkbook->m_arWorksheetSubstream.size(); i++)
	{
		xlsx_context->start_table(xls_global_info->sheets_names[i]);

		if (woorkbook->m_arWorksheetSubstream[i]->get_type() == XLS::typeWorksheetSubstream)
		{
			convert(dynamic_cast<XLS::WorksheetSubstream*>(woorkbook->m_arWorksheetSubstream[i].get()));
		}
		else if (woorkbook->m_arWorksheetSubstream[i]->get_type() == XLS::typeChartSheetSubstream)
		{
			convert_chart_sheet(dynamic_cast<XLS::ChartSheetSubstream*>(woorkbook->m_arWorksheetSubstream[i].get()));
		}

		xlsx_context->end_table();
	}

	for (std::list<XLS::BaseObjectPtr>::iterator it = woorkbook->elements_.begin(); it != woorkbook->elements_.end(); it++)
	{
		convert(it->get());
	}
}

void XlsConverter::convert(XLS::WorksheetSubstream* sheet)
{
	if (sheet == NULL) return;

	if (sheet->m_Dimensions)
	{
		sheet->m_Dimensions->serialize(xlsx_context->current_sheet().dimension());
	}
	if (sheet->m_GLOBALS)
	{
		XLS::GLOBALS * globals  = dynamic_cast<XLS::GLOBALS *>(sheet->m_GLOBALS.get());
		XLS::COLUMNS * columns = dynamic_cast<XLS::COLUMNS *>(sheet->m_COLUMNS.get());

		if (globals && columns)
		{
			globals->m_DefColWidth = columns->m_DefColWidth; 
		}
		sheet->m_GLOBALS->serialize(xlsx_context->current_sheet().sheetFormat());
	}
	if (sheet->m_COLUMNS)
	{
		sheet->m_COLUMNS->serialize(xlsx_context->current_sheet().cols());
	}
	if (sheet->m_CELLTABLE)
	{
		sheet->m_CELLTABLE->serialize(xlsx_context->current_sheet().sheetData());
	}
	if (sheet->m_arMergeCells.size() > 0)
	{
		CP_XML_WRITER(xlsx_context->current_sheet().mergeCells())    
		{
			CP_XML_NODE(L"mergeCells")
			{  		
                for (int i = 0 ; i < sheet->m_arMergeCells.size(); i++)
				{
					sheet->m_arMergeCells[i]->serialize(CP_XML_STREAM());
				}
			}
		}
	}
    for (int i = 0 ; i < sheet->m_arHLINK.size(); i++)
	{
		convert((XLS::HLINK*)sheet->m_arHLINK[i].get());
	}
	convert((XLS::OBJECTS*)sheet->m_OBJECTS.get());

	if (sheet->m_PAGESETUP)
	{
		sheet->m_PAGESETUP->serialize(xlsx_context->current_sheet().pageProperties());
	}
}

void XlsConverter::convert(XLS::GlobalsSubstream* global)
{
	if (global == NULL) return;
	
	convert((XLS::FORMATTING*)global->m_Formating.get());
	
	convert((XLS::THEME*)global->m_Theme.get());

	convert((XLS::SHAREDSTRINGS*)global->m_SHAREDSTRINGS.get());

    for (int i = 0 ; i < global->m_arLBL.size(); i++)
	{
		convert((XLS::LBL*)global->m_arLBL[i].get());
	}

    for (int i = 0 ; i < global->m_arMSODRAWINGGROUP.size(); i++)
	{
		convert((XLS::MSODRAWINGGROUP*)global->m_arMSODRAWINGGROUP[i].get());
	}
}

typedef boost::unordered_map<XLS::FillInfo, int>	mapFillInfo;
typedef boost::unordered_map<XLS::BorderInfo, int>	mapBorderInfo;

void XlsConverter::convert(XLS::FORMATTING* formating)
{
	if (formating == NULL) return;

	std::wstringstream strm;
    CP_XML_WRITER(strm)    
    {
        CP_XML_NODE(L"styleSheet")
        {   
			CP_XML_ATTR(L"xmlns", L"http://schemas.openxmlformats.org/spreadsheetml/2006/main");

			formating->serialize1(CP_XML_STREAM()); //важен порядок в styles

			CP_XML_NODE(L"fills")
			{
				std::vector<XLS::FillInfo> fills_out;
				fills_out.resize(xls_global_info->fill_x_ids.size());

				for (mapFillInfo::iterator it = xls_global_info->fill_x_ids.begin(); it != xls_global_info->fill_x_ids.end(); it++)
				{
					fills_out[it->second] = it->first;
				}

				CP_XML_ATTR(L"count", fills_out.size());
				for (int i = 0 ;i < fills_out.size(); i++)
				{
					fills_out[i].serialize(CP_XML_STREAM());
				}
			}
			CP_XML_NODE(L"borders")
			{				
				std::vector<XLS::BorderInfo> borders_out;
				borders_out.resize(xls_global_info->border_x_ids.size());

				for (mapBorderInfo::iterator it = xls_global_info->border_x_ids.begin(); it != xls_global_info->border_x_ids.end(); it++)
				{
					borders_out[it->second] = it->first;
				}
				
				CP_XML_ATTR(L"count", borders_out.size());
				for (int i = 0 ;i < borders_out.size(); i++)
				{
					borders_out[i].serialize(CP_XML_STREAM());
				}
			}

			formating->serialize2(CP_XML_STREAM());

		}
	}
    
	output_document->get_xl_files().set_styles( oox::package::simple_element::create(L"styles.xml", strm.str()) );
}

std::wstring XlsConverter::GetTargetMoniker(XLS::BiffStructure *moniker)
{
	if (moniker == NULL) return L"";

    if (moniker->getClassName() == "URLMoniker")
	{
		OSHARED::URLMoniker* urlMoniker = dynamic_cast<OSHARED::URLMoniker* >(moniker);
		if (urlMoniker)return urlMoniker->url;
	}
    else if (moniker->getClassName() == "FileMoniker")
	{
		OSHARED::FileMoniker* fileMoniker = dynamic_cast<OSHARED::FileMoniker* >(moniker);
		if (fileMoniker)
		{
			if (!fileMoniker->unicodePath.empty()) return fileMoniker->unicodePath;
			else return std::wstring(fileMoniker->ansiPath.begin(), fileMoniker->ansiPath.end());//codePage ??? todooo
		}
	}

	return L"";
}

void XlsConverter::convert(XLS::MSODRAWINGGROUP * mso_drawing)
{
	if ( mso_drawing == NULL) return;
	XLS::MsoDrawingGroup * mso_group = dynamic_cast<XLS::MsoDrawingGroup*>(mso_drawing->m_MsoDrawingGroup.get());
	if (mso_group == NULL) return;

	//files 
	convert (dynamic_cast<ODRAW::OfficeArtBStoreContainer*>(mso_group->rgChildRec.m_OfficeArtBStoreContainer.get()));

		
}

std::wstring XlsConverter::WriteMediaFile	(char *data, int size, std::wstring type_ext, int id)
{
	if (size < 1 || !data) return L"";
	
	if (id < 0)		id = xlsx_context->get_mediaitems().count_image + 1000; // встроенные в поток 

	xlsx_context->get_mediaitems().create_media_path(xlsx_path);

	bool res = false;
	std::wstring file_name = L"image" + boost::lexical_cast<std::wstring>(id);
	
	if (type_ext == L"dib_data")file_name += std::wstring(L".bmp");
	else						file_name += type_ext;

	NSFile::CFileBinary file;
	if (file.CreateFileW(xlsx_context->get_mediaitems().media_path() + file_name))
	{
		if (type_ext == L"dib_data")
		{
			BITMAPINFOHEADER * header = (BITMAPINFOHEADER *)data;

			WORD vtType		= 0x4D42;				file.WriteFile((BYTE*)&vtType,	2);
			DWORD dwLen		= header->biSizeImage;	file.WriteFile((BYTE*)&dwLen,	4);
			DWORD dwRes		= 0;					file.WriteFile((BYTE*)&dwRes,	4);
			DWORD dwOffset	= 2;					file.WriteFile((BYTE*)&dwOffset, 4);
		}
		
		file.WriteFile((BYTE*)data, size);
		file.CloseFile();
		res = true;
	}
	if (res)
	{
		xlsx_context->get_mediaitems().add_image(L"media/" + file_name, id);
		return L"media/" + file_name;
	}
	return L"";
}

void XlsConverter::convert(ODRAW::OfficeArtBStoreContainer* art_bstore)
{
	if (art_bstore == NULL) return;
	if (art_bstore->rgfb.size() < 1) return;

    for (int i =0 ; i < art_bstore->rgfb.size(); i++)
	{
		int bin_id = i + 1;

		WriteMediaFile(art_bstore->rgfb[i]->pict_data, art_bstore->rgfb[i]->pict_size, art_bstore->rgfb[i]->pict_type, bin_id);
	}
}

void XlsConverter::convert(XLS::HLINK * HLINK_)
{
	XLS::HLink * hLink = dynamic_cast<XLS::HLink*>(HLINK_->m_HLink.get());
	
	std::wstring target;
	
	bool bExternal = false;
	
	if (hLink->hyperlink.hlstmfHasMoniker)
	{
		target = GetTargetMoniker(hLink->hyperlink.oleMoniker.data.get());
		bExternal = true;
	}
	else if (hLink->hyperlink.hlstmfHasLocationStr)
	{
		target = hLink->hyperlink.location.value();
	}

	std::wstring display = hLink->hyperlink.displayName;

	if (display.empty())	display = target;

	xlsx_context->get_table_context().add_hyperlink( hLink->ref8.toString(), target, display, bExternal);
}

void XlsConverter::convert(XLS::LBL * def_name)
{
	if (def_name == NULL) return;

	def_name->serialize(xlsx_context->defined_names());
}

void XlsConverter::convert(XLS::THEME* theme)
{
	if (theme == NULL) return;
}

struct _group_object
{
	_group_object() {spgr = NULL; ind = count = 0;}
	ODRAW::OfficeArtSpgrContainer	*spgr;
	int ind;
	int count;
};

void XlsConverter::convert(XLS::OBJECTS* objects)
{
	if (objects == NULL) return;

	std::vector<_group_object> group_objects;

	_group_object gr;		
	gr.spgr		= dynamic_cast<ODRAW::OfficeArtSpgrContainer*>(objects->m_MsoDrawing.get()->rgChildRec.m_OfficeArtSpgrContainer.get());
	gr.count	= gr.spgr->child_records.size();
	group_objects.push_back(gr);

	bool note		= false;

	if ((group_objects.back().spgr) && (group_objects.back().ind < group_objects.back().spgr->child_records.size()))
	{
		ODRAW::OfficeArtSpContainer *s	= dynamic_cast<ODRAW::OfficeArtSpContainer*>(group_objects.back().spgr->child_records[group_objects.back().ind].get());
		if (s)
		{
			ODRAW::OfficeArtFSP *fsp = dynamic_cast<ODRAW::OfficeArtFSP*>(s->m_OfficeArtFSP.get());
			if ((fsp) && (fsp->fPatriarch)) group_objects.back().ind++;
		}
	}
	int count = 0;
	
	for ( std::list<XLS::BaseObjectPtr>::iterator elem = objects->elements_.begin(); elem != objects->elements_.end(); elem++)
	{
		count++;
		short type_object = -1;

		XLS::OBJ		* OBJ			= dynamic_cast<XLS::OBJ*>		(elem->get());

		XLS::TEXTOBJECT	* TEXTOBJECT	= NULL;
		XLS::CHART		* CHART			= NULL;
		
		std::list<XLS::BaseObjectPtr>::iterator elem_next = boost::next(elem);
		if ( elem_next !=objects->elements_.end() )
		{
			TEXTOBJECT	= dynamic_cast<XLS::TEXTOBJECT*>(elem_next->get());
			CHART		= dynamic_cast<XLS::CHART*>		(elem_next->get());
		}

		XLS::Obj					* obj		= NULL;
		XLS::TxO					* text_obj	= NULL; 
		XLS::ChartSheetSubstream	* chart		= NULL;

		if (OBJ)		obj			= dynamic_cast<XLS::Obj *>(OBJ->m_Obj.get());		
		if (TEXTOBJECT) text_obj	= dynamic_cast<XLS::TxO *>(TEXTOBJECT->m_TxO.get());
		if (CHART)		chart		= dynamic_cast<XLS::ChartSheetSubstream *>(CHART->m_ChartSheetSubstream.get());
		
		if (obj)	type_object = obj->cmo.ot;
		if (chart)	type_object = 0x0005;

		if (text_obj)
		{ 
			if (type_object <0) type_object = 0x0006;			
		}

//-----------------------------------------------------------------------------
		if (type_object < 0)continue;
		
		ODRAW::OfficeArtSpContainer *sp			= NULL;

		if (type_object == 0)
		{
			_group_object gr;		
			if (group_objects.back().ind < group_objects.back().spgr->child_records.size())
			{
				gr.spgr		= dynamic_cast<ODRAW::OfficeArtSpgrContainer*>(group_objects.back().spgr->child_records[group_objects.back().ind++].get());
				gr.count	= gr.spgr->child_records.size();
				group_objects.push_back(gr);
			}
			else //сюда попадать не должно !!!!
				continue;
		}
		if ((group_objects.back().spgr ) && ( group_objects.back().ind < group_objects.back().count))
		{
			sp	= dynamic_cast<ODRAW::OfficeArtSpContainer*>(group_objects.back().spgr->child_records[group_objects.back().ind++].get());
		}	
		
		if (xlsx_context->get_drawing_context().start_drawing(type_object))		
		{
			convert(sp);

			if ((!sp) || (!sp->m_OfficeArtAnchor))
			{
				if ((group_objects.back().spgr ) && ( group_objects.back().ind < group_objects.back().count))
				{
					sp		= dynamic_cast<ODRAW::OfficeArtSpContainer*>(group_objects.back().spgr->child_records[group_objects.back().ind++].get());
				}
				convert(sp, true);
			}
			convert(text_obj);
			convert(chart);

			xlsx_context->get_drawing_context().end_drawing();
		}
		if (TEXTOBJECT || CHART)
		{
			elem++;
			count++;
		}		
		
		while ((group_objects.size() >0) && (group_objects.back().ind >= group_objects.back().count))
		{
			group_objects.back().spgr = NULL;
			group_objects.pop_back();
		}
	}
}

void XlsConverter::convert(ODRAW::OfficeArtSpContainer *sp, bool anchor_only)
{
	if (sp == NULL) return;

	if (!anchor_only)
	{
		convert(sp->m_OfficeArtFSP.get());

		for (int i = 0; i < sp->child_records.size(); i++)
		{
			convert(sp->child_records[i].get());
		}
	}
	convert(sp->m_OfficeArtAnchor.get());
}

void XlsConverter::convert(ODRAW::OfficeArtRecord * art)
{
	if (art == NULL) return;

	std::wstringstream strm;

	switch(art->get_type())
	{
	case XLS::typeOfficeArtFOPT://properties
		{
			convert(dynamic_cast<ODRAW::OfficeArtFOPT *>(art));
		}break;
	case XLS::typeOfficeArtFSP:
		{
			convert(dynamic_cast<ODRAW::OfficeArtFSP *>(art));
		}break;
	case XLS::typeOfficeArtChildAnchor:
		{
			//todoooo привязать к группам !!!
			art->serialize(strm);
            xlsx_context->get_drawing_context().set_anchor(strm.str());
		}break;
	case XLS::typeOfficeArtClientAnchorSheet:
		{
			art->serialize(strm);
            xlsx_context->get_drawing_context().set_anchor(strm.str());
		}break;
	}
}

void XlsConverter::convert(ODRAW::OfficeArtFSP * fsp)
{
	if (fsp == NULL) return;

	if (fsp->fFlipH) xlsx_context->get_drawing_context().set_FlipH();
	if (fsp->fFlipV) xlsx_context->get_drawing_context().set_FlipV();

	xlsx_context->get_drawing_context().set_id(fsp->spid);
	xlsx_context->get_drawing_context().set_shape_id(fsp->shape_id);
}
void XlsConverter::convert_fill_style(std::vector<ODRAW::OfficeArtFOPTEPtr> & props)
{
	for (int i = 0 ; i < props.size() ; i++)
	{
		switch(props[i]->opid)
		{
			case 0x0180:
			{
				switch(props[i]->op)
				{
				case 1://fillPattern:
					{
						xlsx_context->get_drawing_context().set_fill_type(2);
						//texture + change black to color2, white to color1
					}break;
					case 2://fillTexture :
					{
						xlsx_context->get_drawing_context().set_fill_type(3);
						xlsx_context->get_drawing_context().set_fill_texture_mode(0);
					}break;
					case 3://fillPicture :
					{
						xlsx_context->get_drawing_context().set_fill_type(3);
						xlsx_context->get_drawing_context().set_fill_texture_mode(1);
					}break;
					case 4://fillShadeCenter://1 color
					case 5://fillShadeShape:
					{
						xlsx_context->get_drawing_context().set_fill_type(5);
					}break;//
					case 6://fillShadeTitle://2 colors and more
					case 7://fillShade : 
					case 8://fillShadeScale: 
					{
						xlsx_context->get_drawing_context().set_fill_type(4);
					}break;
					case 9://fillBackground:
					{
						xlsx_context->get_drawing_context().set_fill_type(0);
					}break;				
				}
			}break;
			case 0x0181:
			{
				ODRAW::fillColor * fill = (ODRAW::fillColor *)(props[i].get());
				ODRAW::OfficeArtCOLORREF color(fill->op);
				if (!color.sColorRGB.empty())
					xlsx_context->get_drawing_context().set_fill_color(color.nColorRGB, color.sColorRGB);
				else if (color. fPaletteIndex)
				{
					std::map<int,  std::wstring>::iterator it = xls_global_info->colors_palette.find(color.index);
					if (it != xls_global_info->colors_palette.end())
					{					
						//убрать 0!!! todooo
						xlsx_context->get_drawing_context().set_fill_color(0, it->second);
					}
				}
				else
					xlsx_context->get_drawing_context().set_fill_color(color.index, color.fSchemeIndex ? 1 : 3 );
			}break;
			case 0x0182:
			{
				ODRAW::FixedPoint * fixed_point = static_cast<ODRAW::FixedPoint *>(props[i].get());
				xlsx_context->get_drawing_context().set_fill_opacity(fixed_point->dVal);
			}break;
			case 0x0183:
			{
				ODRAW::fillColor * fill = (ODRAW::fillColor *)(props[i].get());
				ODRAW::OfficeArtCOLORREF color(fill->op);
				if (!color.sColorRGB.empty())
					xlsx_context->get_drawing_context().set_fill_color(color.nColorRGB,color.sColorRGB, true );
				else if (color. fPaletteIndex)
				{
					std::map<int,  std::wstring>::iterator it = xls_global_info->colors_palette.find(color.index);
					if (it != xls_global_info->colors_palette.end())
					{					
						//todooo убрать 0 !!!
						xlsx_context->get_drawing_context().set_fill_color(0, it->second, true );
					}
				}
				else
					xlsx_context->get_drawing_context().set_fill_color(color.index, color.fSchemeIndex ? 1 : 3, true );
			}break;
			case 0x0184:
			{
				ODRAW::FixedPoint * fixed_point = static_cast<ODRAW::FixedPoint *>(props[i].get());
				xlsx_context->get_drawing_context().set_fill_opacity(fixed_point->dVal, true);
			}break;
			case 0x0186:
			{
				std::wstring target;
				ODRAW::fillBlip *fillBlip = (ODRAW::fillBlip *)(props[i].get());
				if ((fillBlip) && (fillBlip->blip))
				{
					target = WriteMediaFile(fillBlip->blip->pict_data,fillBlip->blip->pict_size, fillBlip->blip->pict_type);
				}
				else
				{
					bool isIternal = false;
					std::wstring rId = xlsx_context->get_mediaitems().find_image(props[i]->op , target, isIternal);
				}
				xlsx_context->get_drawing_context().set_fill_texture(target);
			}break;
			case 0x018B:
			{
				ODRAW::fillAngle * angle = (ODRAW::fillAngle *)(props[i].get());
				xlsx_context->get_drawing_context().set_fill_angle(angle->dVal);
			}break;
			case 0x197:
			{
				ODRAW::fillShadeColors *shadeColors = (ODRAW::fillShadeColors *)(props[i].get());

				for (int i = 0 ; (shadeColors) && (i < shadeColors->fillShadeColors_complex.data.size()); i++)
				{
					ODRAW::OfficeArtCOLORREF & color = shadeColors->fillShadeColors_complex.data[i].color;

					std::wstring strColor;
					if (!color.sColorRGB.empty())	strColor = color.sColorRGB;
					else if (color. fPaletteIndex)
					{
						std::map<int,  std::wstring>::iterator it = xls_global_info->colors_palette.find(color.index);
						if (it != xls_global_info->colors_palette.end())	strColor = it->second;
					}
					if (!strColor.empty())
						xlsx_context->get_drawing_context().add_fill_colors( shadeColors->fillShadeColors_complex.data[i].dPosition, strColor);
					else
					{
						xlsx_context->get_drawing_context().add_fill_colors( shadeColors->fillShadeColors_complex.data[i].dPosition, 
																									color.index, color.fSchemeIndex ? 1 : 3 );
					}
				}
			}break;
			case 0x01BF:
			{
				ODRAW::FillStyleBooleanProperties * fill = (ODRAW::FillStyleBooleanProperties *)(props[i].get());
				if (fill)
				{
					if (fill->fFilled == false) 
						xlsx_context->get_drawing_context().set_fill_type(0);
				}
			}break;
		
		}
	}
}
void XlsConverter::convert_line_style(std::vector<ODRAW::OfficeArtFOPTEPtr> & props)
{
	for (int i = 0 ; i < props.size() ; i++)
	{
		switch(props[i]->opid)
		{
			case 0x01C0:
			{
				ODRAW::OfficeArtCOLORREF color(props[i]->op);
				if (!color.sColorRGB.empty())
					xlsx_context->get_drawing_context().set_line_color(color.nColorRGB, color.sColorRGB);
				else if (color. fPaletteIndex)
				{
					std::map<int,  std::wstring>::iterator it = xls_global_info->colors_palette.find(color.index);
					if (it != xls_global_info->colors_palette.end())
					{					
						//todooo убрать 0 !!
						xlsx_context->get_drawing_context().set_line_color(0, it->second);
					}
				}
				else
					xlsx_context->get_drawing_context().set_line_color(color.index, color.fSchemeIndex ? 1 : 3 );
			}break;
			case 0x01C4:
			{
				xlsx_context->get_drawing_context().set_line_type(props[i]->op);
			}break;
			case 0x01C5: //blip 
			{
			}break;
			case 0x01CB: 
				xlsx_context->get_drawing_context().set_line_width(props[i]->op);
			{
			}break;
			case 0x01CD:
			{
				xlsx_context->get_drawing_context().set_line_style(props[i]->op);
			}break;
			case 0x01CE:
			{
				xlsx_context->get_drawing_context().set_line_dash(props[i]->op);
			}break;
		}
	}
}
void XlsConverter::convert_blip(std::vector<ODRAW::OfficeArtFOPTEPtr> & props)
{
	for (int i = 0 ; i < props.size() ; i++)
	{
		ODRAW::FixedPoint * fixed_point = static_cast<ODRAW::FixedPoint *>(props[i].get());
		switch(props[i]->opid)
		{
		case 0x100:
			{
				if (fixed_point)
					xlsx_context->get_drawing_context().set_crop_top(fixed_point->dVal);
			}break;
		case 0x101:
			{
				if (fixed_point)
					xlsx_context->get_drawing_context().set_crop_bottom(fixed_point->dVal);
			}break;
		case 0x102:
			{
				if (fixed_point)
					xlsx_context->get_drawing_context().set_crop_left(fixed_point->dVal);
			}break;
		case 0x103:
			{
				if (fixed_point)
					xlsx_context->get_drawing_context().set_crop_right(fixed_point->dVal);
			}break;
		case 0x104:
			{
				bool isIternal = false;
				std::wstring target;
				std::wstring rId = xlsx_context->get_mediaitems().find_image(props[i]->op , target, isIternal);
				xlsx_context->get_drawing_context().set_fill_texture(target);
			}break;
		}
	}
}
void XlsConverter::convert_geometry(std::vector<ODRAW::OfficeArtFOPTEPtr> & props)
{
	std::vector<ODRAW::MSOPOINT>		points;
	std::vector<ODRAW::MSOPATHINFO>		command;
	ODRAW::ShapePath::msoShapePathType	shapeType;
	oox::_rect rect;
	
	for (int i = 0 ; i < props.size() ; i++)
	{
		switch(props[i]->opid)
		{
		case 0x0140:	rect.left	= props[i]->op; break;
		case 0x0141:	rect.top	= props[i]->op; break;
		case 0x0142:	rect.right	= props[i]->op; break;
		case 0x0143:	rect.bottom = props[i]->op; break;
		case 0x0144:	shapeType	= (ODRAW::ShapePath::msoShapePathType)props[i]->op; break;
		case 0x0145:
			{
				ODRAW::PVertices * v = (ODRAW::PVertices *)(props[i].get());
				points = v->path_complex.data;
			}break;
		case 0x0146:
			{
				ODRAW::PSegmentInfo * s = (ODRAW::PSegmentInfo *)(props[i].get());
				command = s->path_complex.data;
			}break;
		}
	}

	if (points.size() > 0 || command.size() > 0)
	{
		std::wstringstream strm;
		CP_XML_WRITER(strm)    
		{
			if (command.size() == 0)
			{
				for (int i = 0 ; i < points.size(); i++)
				{
					CP_XML_NODE(L"a:lnTo")
					{
						CP_XML_NODE(L"a:pt")
						{
							CP_XML_ATTR(L"x", points[i].x);
							CP_XML_ATTR(L"y", points[i].y);
						}
					}
				}
			}
			else
			{
				int ind_point = 0;
				std::wstring comm[] = { L"a:lnTo", L"a:cubicBezTo", L"a:moveTo", L"a:close" };
				int count_point[] = { 1, 3, 1, 0};
				for (int i = 0 ; i < command.size(); i++)
				{
					if (ind_point >= points.size())
						break;				
					
					//if (command[i].typeSegment == ODRAW::msopathEnd) break;
					if (command[i].typeSegment > 3) continue;

					CP_XML_NODE(comm[command[i].typeSegment])
					{
						for (int j=0 ; j < count_point[command[i].typeSegment]; j ++)
						{
							if (ind_point < points.size())
							{
								CP_XML_NODE(L"a:pt")
								{
									CP_XML_ATTR(L"x", points[ind_point].x);
									CP_XML_ATTR(L"y", points[ind_point].y);
									ind_point++;
								}
							}
							else
							{
								CP_XML_NODE(L"a:pt")
								{
									CP_XML_ATTR(L"x", 0);
									CP_XML_ATTR(L"y", 0);
								}
							}
						}
					}
				}
			}
		}
		//xlsx_context->get_drawing_context().set_shapeType
		xlsx_context->get_drawing_context().set_path_rect(rect);
		xlsx_context->get_drawing_context().set_path(strm.str());
	}
}
void XlsConverter::convert_geometry_text(std::vector<ODRAW::OfficeArtFOPTEPtr> & props)
{
	for (int i = 0 ; i < props.size() ; i++)
	{
	}
}
void XlsConverter::convert_text(std::vector<ODRAW::OfficeArtFOPTEPtr> & props)
{
}
void XlsConverter::convert_shadow(std::vector<ODRAW::OfficeArtFOPTEPtr> & props)
{
	for (int i = 0 ; i < props.size() ; i++)
	{
	}
}
void XlsConverter::convert_shape(std::vector<ODRAW::OfficeArtFOPTEPtr> & props)
{
	for (int i = 0 ; i < props.size() ; i++)
	{

	}
}
void XlsConverter::convert_group_shape(std::vector<ODRAW::OfficeArtFOPTEPtr> & props)
{
	for (int i = 0 ; i < props.size() ; i++)
	{
		switch(props[i]->opid)
		{
		case 0x380:
			{
				ODRAW::anyString *str = dynamic_cast<ODRAW::anyString*>(props[i].get());
				xlsx_context->get_drawing_context().set_name(str->string_);
			}break;
		case 0x381:
			{
				ODRAW::anyString *str = dynamic_cast<ODRAW::anyString*>(props[i].get());
				xlsx_context->get_drawing_context().set_description(str->string_);
			}break;
		case 0x0382:
			{
				ODRAW::pihlShape *pihlShape = dynamic_cast<ODRAW::pihlShape*>(props[i].get());
				if (pihlShape)
				{
					std::wstring target = GetTargetMoniker(pihlShape->IHlink_complex.hyperlink.oleMoniker.data.get());
					xlsx_context->get_drawing_context().set_hyperlink(target);
				}
			}break;
		}
	}
}
void XlsConverter::convert_transform(std::vector<ODRAW::OfficeArtFOPTEPtr> & props)
{
	for (int i = 0 ; i < props.size() ; i++)
	{
		switch(props[i]->opid)
		{
			case 0x0004:
			{
				double d = props[i]->op / 65536.;
				d *= 60000; //60 000 per 1 gr - 19.5.5 oox 

				xlsx_context->get_drawing_context().set_rotation((int)d);
			}break;
		}
	}
}
void XlsConverter::convert(ODRAW::OfficeArtFOPT * fort)
{
	if (fort == NULL) return;

	convert_shape			(fort->fopt.Shape_props);
	convert_group_shape		(fort->fopt.GroupShape_props);
	convert_transform		(fort->fopt.Transform_props);
	convert_blip			(fort->fopt.Blip_props);
	convert_geometry		(fort->fopt.Geometry_props);
	convert_fill_style		(fort->fopt.FillStyle_props);
	convert_line_style		(fort->fopt.LineStyle_props);
	convert_shadow			(fort->fopt.Shadow_props);
	convert_text			(fort->fopt.Text_props);
	convert_geometry_text	(fort->fopt.GeometryText_props);
}

void XlsConverter::convert(XLS::SHAREDSTRINGS* sharedstrings)
{
	if (sharedstrings == NULL) return;
	
	for (std::list<XLS::BaseObjectPtr>::iterator it = sharedstrings->elements_.begin(); it != sharedstrings->elements_.end(); it++)
	{
		(*it)->serialize(xlsx_context->shared_strings());
	}
}

void XlsConverter::convert(XLS::TxO * text_obj)
{
	if (text_obj == NULL) return;
	
	std::wstringstream strm;
	text_obj->serialize(strm);

	xlsx_context->get_drawing_context().set_text(strm.str());
}

void XlsConverter::convert_chart_sheet(XLS::ChartSheetSubstream * chart)
{
	if (chart == NULL) return;
		
	ODRAW::OfficeArtSpContainer *sp			= NULL;
/*		if (( spgr) && (ind < spgr->child_records.size()))
	{
		sp		= dynamic_cast<ODRAW::OfficeArtSpContainer*>(spgr->child_records[ind++].get());
	}*/	
		
	if (xlsx_context->get_drawing_context().start_drawing(0x0005))		
	{
		convert(chart);
		xlsx_context->get_drawing_context().set_chart_sheet_anchor(xls_global_info->currentChartWidth, xls_global_info->currentChartHeight);//todooo - size chart
		xlsx_context->get_drawing_context().set_id(1);

		xlsx_context->get_drawing_context().end_drawing();
	}
}
void XlsConverter::convert(XLS::ChartSheetSubstream * chart)
{
	if (chart == NULL) return;

	chart->serialize(xlsx_context->current_chart().chartData());	
	//convert(chart->m_OBJECTSCHART.get());непонятные какие то текстбоксы - пустые и бз привязок
}
