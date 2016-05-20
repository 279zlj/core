﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace Joiner
{
    class Program
    {
        static void Main(string[] args)
        {
            string strApplication = Directory.GetCurrentDirectory();
            strApplication = Path.GetDirectoryName(strApplication);
            strApplication = Path.GetDirectoryName(strApplication);
            strApplication = Path.GetDirectoryName(strApplication);
            strApplication = Path.GetDirectoryName(strApplication);
            strApplication = Path.GetDirectoryName(strApplication);
            strApplication = Path.GetDirectoryName(strApplication);

            string strRoot = strApplication + "\\OfficeWeb\\";
            List<string> files = new List<string>();

            /*
            string[] arrFilesConfig = {
                "Common/Build/License.js",
                "Common/browser.js",
                "Common/docscoapisettings.js",
                "Common/docscoapicommon.js",
                "Common/docscoapi.js",
                "Common/downloaderfiles.js",
                "Common/apiCommon.js",
                "Common/commonDefines.js",
                "Common/editorscommon.js",
                "Common/NumFormat.js",
                "Common/Charts/charts.js",
				"Common/Charts/DrawingArea.js",
                "Common/Charts/DrawingObjects.js",
				"Common/Charts/ChartsDrawer.js",
                "Common/FontsFreeType/font_engine.js",
                "Common/FontsFreeType/FontFile.js",
                "Common/FontsFreeType/FontManager.js",
                "Word/Drawing/HatchPattern.js",
                "Word/Drawing/Externals.js",
                "Word/Drawing/Metafile.js",
                "Excel/model/DrawingObjects/GlobalLoaders.js",
                "Common/trackFile.js",
                "Excel/apiDefines.js",
                "Excel/document/empty-workbook.js",
                "Excel/utils/utils.js",
                "Excel/model/clipboard.js",
                "Excel/model/autofilters.js",
                "Excel/graphics/DrawingContext.js",
                "Excel/graphics/pdfprinter.js",
                "Excel/model/CollaborativeEditing.js",
                "Excel/model/ConditionalFormatting.js",
                "Excel/model/FormulaObjects/parserFormula.js",
                "Excel/model/FormulaObjects/dateandtimeFunctions.js",
                "Excel/model/FormulaObjects/engineeringFunctions.js",
                "Excel/model/FormulaObjects/cubeFunctions.js",
                "Excel/model/FormulaObjects/databaseFunctions.js",
                "Excel/model/FormulaObjects/textanddataFunctions.js",
                "Excel/model/FormulaObjects/statisticalFunctions.js",
                "Excel/model/FormulaObjects/financialFunctions.js",
                "Excel/model/FormulaObjects/mathematicFunctions.js",
                "Excel/model/FormulaObjects/lookupandreferenceFunctions.js",
                "Excel/model/FormulaObjects/informationFunctions.js",
                "Excel/model/FormulaObjects/logicalFunctions.js",
                "Excel/model/Serialize.js",
                "Excel/model/WorkbookElems.js",
                "Excel/model/Workbook.js",
                "Excel/model/CellInfo.js",
                "Excel/model/AdvancedOptions.js",
                "Excel/model/History.js",
                "Excel/model/UndoRedo.js",
                "Excel/model/CellComment.js",
				"Excel/view/mobileTouch.js",
				"Excel/view/iscroll.js",
				"Excel/view/PopUpSelector.js",
                "Excel/view/StringRender.js",
                "Excel/view/CellTextRender.js",
                "Excel/view/CellEditorView.js",
                "Excel/view/WorksheetView.js",
                "Excel/view/HandlerList.js",
                "Excel/view/EventsController.js",
                "Excel/view/WorkbookView.js",
                "Common/scroll.js",
                "Word/Editor/SerializeCommon.js",
                "Common/Drawings/Math.js",
                "Common/Drawings/ArcTo.js",
                "Word/Drawing/ColorArray.js",
                "Common/Shapes/Serialize.js",
                "Common/Shapes/SerializeWriter.js",
                "Common/SerializeCommonWordExcel.js",
				"Common/SerializeChart.js",
                "Common/Charts/libraries/OfficeExcel.common.core.js",
                "Common/Charts/libraries/OfficeExcel.common.key.js",
                "Common/Charts/libraries/OfficeExcel.bar.js",
                "Common/Charts/libraries/OfficeExcel.hbar.js",
                "Common/Charts/libraries/OfficeExcel.line.js",
                "Common/Charts/libraries/OfficeExcel.pie.js",
                "Common/Charts/libraries/OfficeExcel.scatter.js",
                "Common/Charts/libraries/OfficeExcel.chartProperties.js",

                "Common/Drawings/Hit.js",
                "Common/Drawings/States.js",
                "Common/Drawings/TrackObjects/AdjustmentTracks.js",
                "Common/Drawings/TrackObjects/ResizeTracks.js",
                "Common/Drawings/TrackObjects/RotateTracks.js",
                "Common/Drawings/TrackObjects/NewShapeTracks.js",
                "Common/Drawings/TrackObjects/PolyLine.js",
                "Common/Drawings/TrackObjects/Spline.js",
                "Common/Drawings/TrackObjects/MoveTracks.js",

                "Common/Drawings/Format/Constants.js",
                "Common/Drawings/Format/Format.js",
                "Common/Drawings/Format/CreateGeometry.js",
                "Common/Drawings/Format/Geometry.js",
                "Common/Drawings/Format/Path.js",
                "Common/Drawings/Format/Shape.js",
                "Common/Drawings/Format/GroupShape.js",
                "Common/Drawings/Format/Image.js",
                "Common/Drawings/Format/ChartSpace.js",
                "Common/Drawings/Format/ChartFormat.js",
                "Common/Drawings/Format/TextBody.js",
                "Common/Drawings/CommonController.js",
                "Excel/view/DrawingObjectsController.js",

                "Excel/model/DrawingObjects/Graphics.js",
                "Excel/model/DrawingObjects/Overlay.js",
                "Excel/model/DrawingObjects/Controls.js",
                "Excel/model/DrawingObjects/ShapeDrawer.js",


                "Word/Editor/CollaborativeEditing.js",

                "Excel/model/DrawingObjects/DrawingDocument.js",

                "Excel/model/DrawingObjects/Format/ShapePrototype.js",
                "Excel/model/DrawingObjects/Format/ImagePrototype.js",
                "Excel/model/DrawingObjects/Format/GroupPrototype.js",
                "Excel/model/DrawingObjects/Format/ChartSpacePrototype.js",


                "Word/Math/mathTypes.js",
                "Word/Math/mathText.js",
                "Word/Math/mathContent.js",
                "Word/Math/base.js",
                "Word/Math/fraction.js",
                "Word/Math/degree.js",
                "Word/Math/matrix.js",
                "Word/Math/limit.js",
                "Word/Math/nary.js",
                "Word/Math/radical.js",
                "Word/Math/operators.js",
                "Word/Math/accent.js",
                "Word/Math/borderBox.js",

				"Word/Editor/Comments.js",
				"Word/Editor/Styles.js",
				"Word/Editor/FlowObjects.js",
				"Word/Editor/ParagraphContent.js",
				"Word/Editor/Hyperlink.js",
				"Word/Editor/Run.js",
				"Word/Editor/Math.js",
				"Word/Editor/Paragraph.js",
				"Word/Editor/Sections.js",
				"Word/Editor/Numbering.js",
				"Word/Editor/HeaderFooter.js",
				"Word/Editor/Document.js",
				"Word/Editor/DocumentContent.js",
				"Word/Editor/Table.js",
				"Word/Editor/Serialize2.js",
				"Word/Editor/FontClassification.js",
				"Word/Editor/Spelling.js",
				
				"Word/Math/mathTypes.js",
				"Word/Math/print.js",
				"Word/Math/mathText.js",
				"Word/Math/mathContent.js",
				"Word/Math/actions.js",
				"Word/Math/base.js",
				"Word/Math/subBase.js",
				"Word/Math/fraction.js",
				"Word/Math/degree.js",
				"Word/Math/matrix.js",
				"Word/Math/limit.js",
				"Word/Math/nary.js",
				"Word/Math/radical.js",
				"Word/Math/operators.js",
				"Word/Math/accent.js",
				"Word/Math/drawingUnion.js",
				"Word/Math/borderBox.js",
				"Word/Math/test_composition.js",
				
                "Excel/model/DrawingObjects/GlobalCounters.js",

                "Word/apiCommon.js",
                "Excel/api.js"
            };
            */
            string[] arrFilesConfig = {
            	"Common/Build/License.js",
                "Common/browser.js",
                "Common/docscoapisettings.js",
                "Common/docscoapicommon.js",
                "Common/docscoapi.js",
                "Common/downloaderfiles.js",
                "Common/apiCommon.js",
                "Common/commonDefines.js",
                "Common/editorscommon.js",
                "Common/NumFormat.js",
                "Common/Charts/charts.js",
				"Common/Charts/DrawingArea.js",
                "Common/Charts/DrawingObjects.js",
				"Common/Charts/ChartsDrawer.js",
                "Common/FontsFreeType/font_engine.js",
                "Common/FontsFreeType/FontFile.js",
                "Common/FontsFreeType/FontManager.js",
                "Word/Drawing/HatchPattern.js",
                "Word/Drawing/WorkEvents.js",
                "Word/Drawing/Externals.js",
                "Word/Drawing/Metafile.js",
                "Excel/model/DrawingObjects/GlobalLoaders.js",
                "Common/trackFile.js",
                "Excel/apiDefines.js",
                "Excel/document/empty-workbook.js",
                "Excel/utils/utils.js",
                "Excel/model/clipboard.js",
                "Excel/model/autofilters.js",
                "Excel/graphics/DrawingContext.js",
                "Excel/graphics/pdfprinter.js",
                "Excel/model/CollaborativeEditing.js",
                "Excel/model/ConditionalFormatting.js",
                "Excel/model/FormulaObjects/parserFormula.js",
                "Excel/model/FormulaObjects/dateandtimeFunctions.js",
                "Excel/model/FormulaObjects/engineeringFunctions.js",
                "Excel/model/FormulaObjects/cubeFunctions.js",
                "Excel/model/FormulaObjects/databaseFunctions.js",
                "Excel/model/FormulaObjects/textanddataFunctions.js",
                "Excel/model/FormulaObjects/statisticalFunctions.js",
                "Excel/model/FormulaObjects/financialFunctions.js",
                "Excel/model/FormulaObjects/mathematicFunctions.js",
                "Excel/model/FormulaObjects/lookupandreferenceFunctions.js",
                "Excel/model/FormulaObjects/informationFunctions.js",
                "Excel/model/FormulaObjects/logicalFunctions.js",
                "Excel/model/Serialize.js",
                "Excel/model/WorkbookElems.js",
                "Excel/model/Workbook.js",
                "Excel/model/CellInfo.js",
                "Excel/model/AdvancedOptions.js",
                "Excel/model/History.js",
                "Excel/model/UndoRedo.js",
                "Excel/model/CellComment.js",
				"Excel/view/mobileTouch.js",
				"Excel/view/iscroll.js",
				"Excel/view/PopUpSelector.js",
                "Excel/view/StringRender.js",
                "Excel/view/CellTextRender.js",
                "Excel/view/CellEditorView.js",
                "Excel/view/WorksheetView.js",
                "Excel/view/HandlerList.js",
                "Excel/view/EventsController.js",
                "Excel/view/WorkbookView.js",
                "Common/scroll.js",
                "Word/Editor/SerializeCommon.js",
                "Common/Drawings/Math.js",
                "Common/Drawings/ArcTo.js",
                "Word/Drawing/ColorArray.js",
                "Common/Shapes/Serialize.js",
                "Common/Shapes/SerializeWriter.js",
                "Common/SerializeCommonWordExcel.js",
				"Common/SerializeChart.js",

                "Common/Drawings/Hit.js",
                "Common/Drawings/States.js",
                "Common/Drawings/DrawingObjectsHandlers.js",
                "Common/Drawings/TrackObjects/AdjustmentTracks.js",
                "Common/Drawings/TrackObjects/ResizeTracks.js",
                "Common/Drawings/TrackObjects/RotateTracks.js",
                "Common/Drawings/TrackObjects/NewShapeTracks.js",
                "Common/Drawings/TrackObjects/PolyLine.js",
                "Common/Drawings/TrackObjects/Spline.js",
                "Common/Drawings/TrackObjects/MoveTracks.js",


                "Common/Drawings/CommonController.js",
                "Common/Drawings/Format/Constants.js",
                "Common/Drawings/Format/Format.js",
                "Common/Drawings/Format/CreateGeometry.js",
                "Common/Drawings/Format/Geometry.js",
                "Common/Drawings/Format/Path.js",
                "Common/Drawings/Format/Shape.js",
                "Common/Drawings/Format/GroupShape.js",
                "Common/Drawings/Format/Image.js",
                "Common/Drawings/Format/ChartSpace.js",
                "Common/Drawings/Format/ChartFormat.js",
                "Common/Drawings/Format/TextBody.js",
				"Common/wordcopypaste.js",
                "Excel/view/DrawingObjectsController.js",

                "Excel/model/DrawingObjects/Graphics.js",
                "Excel/model/DrawingObjects/Overlay.js",
                "Excel/model/DrawingObjects/Controls.js",
                "Excel/model/DrawingObjects/ShapeDrawer.js",



                "Excel/model/DrawingObjects/DrawingDocument.js",

                "Excel/model/DrawingObjects/Format/ShapePrototype.js",
                "Excel/model/DrawingObjects/Format/ImagePrototype.js",
                "Excel/model/DrawingObjects/Format/GroupPrototype.js",
                "Excel/model/DrawingObjects/Format/ChartSpacePrototype.js",


				"Word/Editor/Comments.js",
				"Word/Editor/Styles.js",
				"Word/Editor/FlowObjects.js",
				"Word/Editor/ParagraphContent.js",
                "Word/Editor/ParagraphContentBase.js",
				"Word/Editor/Hyperlink.js",
				"Word/Editor/Run.js",
				"Word/Editor/Math.js",
				"Word/Editor/Paragraph.js",
				"Word/Editor/Sections.js",
				"Word/Editor/Numbering.js",
				"Word/Editor/HeaderFooter.js",
				"Word/Editor/Document.js",
				"Word/Editor/DocumentContent.js",
				"Word/Editor/Table.js",
				"Word/Editor/Serialize2.js",
				"Word/Editor/FontClassification.js",
				"Word/Editor/Spelling.js",
				"Word/Editor/GraphicObjects/WrapManager.js",
				
				"Word/Math/mathTypes.js",
				"Word/Math/mathText.js",
				"Word/Math/mathContent.js",
				"Word/Math/base.js",
				"Word/Math/fraction.js",
				"Word/Math/degree.js",
				"Word/Math/matrix.js",
				"Word/Math/limit.js",
				"Word/Math/nary.js",
				"Word/Math/radical.js",
				"Word/Math/operators.js",
				"Word/Math/accent.js",
				"Word/Math/borderBox.js",
				
                "Excel/model/DrawingObjects/GlobalCounters.js",

                "Word/apiCommon.js",
                "Excel/api.js",

                "Common/Shapes/EditorSettings.js",
                "Common/commonDefines.js",
                "Excel/apiDefines.js"
            };

            if (true)
            {
                files.Clear();
                for (int i = 0; i < arrFilesConfig.Length; ++i)
                {
                    files.Add(arrFilesConfig[i]);
                }
            }

            StringBuilder oBuilder = new StringBuilder();

            for (int i = 0; i < files.Count; i++)
            {
                StreamReader oReader = new StreamReader(strRoot + files[i]);
                oBuilder.Append(oReader.ReadToEnd());

                oBuilder.Append("\n\n");
            }

            string strDestPath = strApplication + "\\OfficeWeb\\Excel\\sdk-all.js";
            StreamWriter oWriter = new StreamWriter(strDestPath, false, Encoding.UTF8);
            
            oWriter.Write(oBuilder.ToString());
            oWriter.Close();
        }
    }
}
