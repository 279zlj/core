#pragma once
#include "../PPTXShape.h"

namespace OOXMLShapes
{
	class CRoundRect : public CPPTXShape
	{
		public:
			CRoundRect()
			{
				LoadFromXML(
					_T("<ooxml-shape>")
					_T("<avLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<gd name=\"adj\" fmla=\"val 16667\" />")
					_T("</avLst>")
					_T("<gdLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<gd name=\"a\" fmla=\"pin 0 adj 50000\" />")
					  _T("<gd name=\"x1\" fmla=\"*/ ss a 100000\" />")
					  _T("<gd name=\"x2\" fmla=\"+- r 0 x1\" />")
					  _T("<gd name=\"y2\" fmla=\"+- b 0 x1\" />")
					  _T("<gd name=\"il\" fmla=\"*/ x1 29289 100000\" />")
					  _T("<gd name=\"ir\" fmla=\"+- r 0 il\" />")
					  _T("<gd name=\"ib\" fmla=\"+- b 0 il\" />")
					_T("</gdLst>")
					_T("<ahLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<ahXY gdRefX=\"adj\" minX=\"0\" maxX=\"50000\">")
						_T("<pos x=\"x1\" y=\"t\" />")
					  _T("</ahXY>")
					_T("</ahLst>")
					_T("<cxnLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<cxn ang=\"3cd4\">")
						_T("<pos x=\"hc\" y=\"t\" />")
					  _T("</cxn>")
					  _T("<cxn ang=\"cd2\">")
						_T("<pos x=\"l\" y=\"vc\" />")
					  _T("</cxn>")
					  _T("<cxn ang=\"cd4\">")
						_T("<pos x=\"hc\" y=\"b\" />")
					  _T("</cxn>")
					  _T("<cxn ang=\"0\">")
						_T("<pos x=\"r\" y=\"vc\" />")
					  _T("</cxn>")
					_T("</cxnLst>")
					_T("<rect l=\"il\" t=\"il\" r=\"ir\" b=\"ib\" xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\" />")
					_T("<pathLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<path>")
						_T("<moveTo>")
						  _T("<pt x=\"l\" y=\"x1\" />")
						_T("</moveTo>")
						_T("<arcTo wR=\"x1\" hR=\"x1\" stAng=\"cd2\" swAng=\"cd4\" />")
						_T("<lnTo>")
						  _T("<pt x=\"x2\" y=\"t\" />")
						_T("</lnTo>")
						_T("<arcTo wR=\"x1\" hR=\"x1\" stAng=\"3cd4\" swAng=\"cd4\" />")
						_T("<lnTo>")
						  _T("<pt x=\"r\" y=\"y2\" />")
						_T("</lnTo>")
						_T("<arcTo wR=\"x1\" hR=\"x1\" stAng=\"0\" swAng=\"cd4\" />")
						_T("<lnTo>")
						  _T("<pt x=\"x1\" y=\"b\" />")
						_T("</lnTo>")
						_T("<arcTo wR=\"x1\" hR=\"x1\" stAng=\"cd4\" swAng=\"cd4\" />")
						_T("<close />")
					  _T("</path>")
					_T("</pathLst>")
					_T("</ooxml-shape>")
				);
			}
	};
}